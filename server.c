#include "server.h"
#include <sys/socket.h>

int create_server(char *port) {
  struct addrinfo hints, *result;
  memset(&hints, 0, sizeof(struct addrinfo));

  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  int getaddr_result = getaddrinfo(NULL, port, &hints, &result);

  if (getaddr_result != 0) {
    fprintf(stderr, "gettaddrinfo: %s\n", gai_strerror(getaddr_result));
    exit(1);
  }

  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

  int optval = 1;
  setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));
  setsockopt(socket_fd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(int));

  int bind_result = bind(socket_fd, result->ai_addr, result->ai_addrlen);
  if (bind_result == -1) {
    perror("bind result:");
    exit(1);
  }

  int listen_result = listen(socket_fd, 1024);
  if (listen_result == -1) {
    perror("listen result:");
    exit(1);
  }

  freeaddrinfo(result);
  return socket_fd;
}

void run_server(int socket_fd) {
  while (1) {
    int client_fd = accept(socket_fd, NULL, NULL);
    if (client_fd == -1) {
      perror("accept failed:");
      continue;
    } else {
      read_request(client_fd);
    }
  }
}

void read_request(int client_fd) {
  size_t bytes_read = 0;

  char buffer[BUFFER_SIZE];
  memset(buffer, 0, BUFFER_SIZE);

  while ((bytes_read = read(client_fd, buffer, sizeof(buffer))) > 0) {
    fprintf(stderr, "%s\n", buffer);
    memset(buffer, 0, BUFFER_SIZE);
  }
}
