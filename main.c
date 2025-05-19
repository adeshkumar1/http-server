#include "map_html_util.h"
#include "server.h"
#include <assert.h>
#include <map.h>
#include <signal.h>
#include <stdio.h>

volatile sig_atomic_t endSession = 0;
map *m;

void clean_up() { map_delete(m); }

void handle_sigint(int sig) {
  assert(sig == SIGINT);
  endSession = 1;
}

int main(int argc, char **argv) {

  if (argc != 3) {
    fprintf(stderr, "usage: ./server <port> <folder to website>\n");
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

  m = create_map();

  map_html_files(m, argv[2], "");

  char *port = argv[1];

  int socket_fd = create_server(port);
  run_server(socket_fd);

  clean_up();
  return 0;
}
