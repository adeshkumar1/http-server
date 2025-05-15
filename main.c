#include "server.h"
#include <stdio.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "usage: ./server <port>\n");
    exit(1);
  }

  char *port = argv[1];

  int socket_fd = create_server(port);
  run_server(socket_fd);
  return 0;
}
