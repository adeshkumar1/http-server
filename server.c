#include "server.h"

#include <signal.h>
#include <sys/socket.h>

extern volatile sig_atomic_t endSession;

int create_server(char *port) {
  struct addrinfo hints, *result;
  memset(&hints, 0, sizeof(struct addrinfo));

  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  int getaddr_result = getaddrinfo(NULL, port, &hints, &result);

  if (getaddr_result != 0) {
    fprintf(stderr, "gettaddrinfo %s\n", gai_strerror(getaddr_result));
    exit(1);
  }

  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  int optval = 1;
  setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));
  setsockopt(socket_fd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(int));

  int bind_result = bind(socket_fd, result->ai_addr, result->ai_addrlen);
  if (bind_result == -1) {
    perror("bind result");
    exit(1);
  }

  int listen_result = listen(socket_fd, 1024);
  if (listen_result == -1) {
    perror("listen result");
    exit(1);
  }

  freeaddrinfo(result);
  return socket_fd;
}

void run_server(int socket_fd) {
  while (!endSession) {
    int client_fd = accept(socket_fd, NULL, NULL);
    if (client_fd == -1) {
      if (errno == EINTR) {
        if (endSession) {
          break;
        }
        continue;
      }
      perror("accept failed");
      continue;
    } else {
      read_request(client_fd);
      close(client_fd);
    }
  }
}

void read_request(int client_fd) {

  char buffer[BUFFER_SIZE];
  memset(buffer, 0, BUFFER_SIZE);

  ssize_t bytes_read = read(client_fd, buffer, BUFFER_SIZE - 1);
  buffer[BUFFER_SIZE - 1] = 0;

  if (bytes_read <= 0) {
    return;
  }

  // fprintf(stderr, "%s\n", buffer);

  char page[1024];

  parse_request(buffer, page);

  fprintf(stderr, "page: %s\n", page);

  const char *body = "<html><body><h1>Welcome to site</h1></body></html>";
  size_t body_len = strlen(body);

  // build header + body
  char response[512];
  int header_len =
      snprintf(response, sizeof(response),
               "HTTP/1.1 200 OK\r\n"
               "Content-Type: text/html\r\n"
               "Content-Length: %zu\r\n"
               "Connection: close\r\n" // tell the browser we're done
               "\r\n"
               "%s",
               body_len, body);

  // write out exactly header_len bytes
  write(client_fd, response, header_len);
  ssize_t total_written = 0;
  while (total_written < header_len) {
    ssize_t written =
        write(client_fd, response + total_written, header_len - total_written);
    if (written <= 0)
      break;
    total_written += written;
  }
}

int parse_request(char *buffer, char *page) {
  sscanf(buffer, "%*s %s", page);
  return 1;
}
