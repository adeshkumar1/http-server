#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int create_server(char *port);
void run_server(int socket_fd);

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "usage: ./server 3000");
    exit(1);
  }

  char *port = argv[1];

  create_server(port);
  return 0;
}
