#pragma once

#define _GNU_SOURCE

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#define BUFFER_SIZE 1024
#include <stdio.h>
#include <stdlib.h>

#include "map.h"
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int create_server(char *port);
void run_server(int socket_fd);

void read_request(int client_fd);
void respond_request(int client_fd, char *page);

int parse_request(char *buffer, char *page);

int write_to_server(char *buffer, int socket_fd, ssize_t bytes_to_write);

void router(char *page, char *file_path);
