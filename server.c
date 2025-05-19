#include "server.h"
#include "map.h"

extern volatile sig_atomic_t endSession;
extern map *m;

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

  char page[1024];
  memset(page, 0, 1024);

  parse_request(buffer, page);
  respond_request(client_fd, page);
}

int parse_request(char *buffer, char *page) {
  sscanf(buffer, "%*s %s", page);
  return 1;
}

int write_to_server(char *buffer, int socket_fd, ssize_t bytes_to_write) {
  ssize_t total_written = 0;
  while (total_written < bytes_to_write) {
    ssize_t written = write(socket_fd, buffer + total_written,
                            bytes_to_write - total_written);
    if (written <= 0)
      break;
    total_written += written;
  }

  return total_written;
}

void respond_request(int client_fd, char *page) {
  char file_path[256];
  memset(file_path, 0, 256);

  router(page, file_path);

  int file = open(file_path, O_RDONLY);

  struct stat st;
  stat(file_path, &st);

  char *body = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, file, 0);

  size_t body_len = st.st_size;

  char response[512];
  int header_len = snprintf(response, sizeof(response),
                            "HTTP/1.1 200 OK\r\n"
                            "Content-Type: text/html\r\n"
                            "Content-Length: %zu\r\n"
                            "Connection: close\r\n"
                            "\r\n"
                            "%s",
                            body_len, body);

  write_to_server(response, client_fd, header_len);
  close(file);
}

void router(char *page, char *file_path) {
  if (m) {
    if (map_contains(m, page)) {
      strcpy(file_path, map_get(m, page));
    } else {
      strcpy(file_path, "website/unauthorized.html");
    }
  }
}
