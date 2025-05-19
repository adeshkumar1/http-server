#pragma once

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 10000

typedef struct {
  char *key;
  char *value;
} pair;

typedef struct {
  pair hash_table[TABLE_SIZE];
} map;

map *create_map();

unsigned int hash(char *input);

int map_contains(map *map, char *input);

char *map_get(map *map, char *input);

void map_delete(map *map);

void map_insert(map *map, char *key, char *value);
