#pragma once

#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200112L
#define BUFFER_SIZE 1024
#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <stddef.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <unistd.h>

int create_server(char *port);
void run_server(int socket_fd);

void read_request(int client_fd);
