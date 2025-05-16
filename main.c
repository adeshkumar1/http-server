#include "server.h"
#include <assert.h>
#include <signal.h>
#include <stdio.h>

volatile sig_atomic_t endSession = 0;

void handle_sigint(int sig) {
  assert(sig == SIGINT);
  endSession = 1;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "usage: ./server <port>\n");
    exit(1);
  }

  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = handle_sigint;
  sa.sa_flags = 0;
  sigemptyset(&sa.sa_mask);

  if (sigaction(SIGINT, &sa, NULL) == -1) {
    perror("sigaction");
    return 1;
  }

  char *port = argv[1];

  int socket_fd = create_server(port);
  run_server(socket_fd);
  return 0;
}
