#include "map.h"

map *create_map() {
  map *m = malloc(sizeof(map));

  pair *hash_table = m->hash_table;

  for (int i = 0; i < TABLE_SIZE; ++i) {
    pair *p = &hash_table[i];
    p->key = NULL;
    p->value = NULL;
  }

  return m;
}

unsigned int hash(char *input) {
  unsigned long hash = 5381;
  int c;
  while ((c = *input++))
    hash = ((hash << 5) + hash) + c;
  return hash % TABLE_SIZE;
}

int map_contains(map *map, char *input) {
  unsigned long h = hash(input);
  pair *hash_table = map->hash_table;

  return hash_table[h].key != NULL;
}

char *map_get(map *map, char *input) {
  unsigned long h = hash(input);
  pair *hash_table = map->hash_table;

  return hash_table[h].value;
}

void map_delete(map *map) {
  pair *hash_table = map->hash_table;

  for (int i = 0; i < TABLE_SIZE; ++i) {
    pair *p = &hash_table[i];
    if (p->key) {
      free(p->key);
      free(p->value);
    }
  }

  free(map);
  map = NULL;
}

void map_insert(map *map, char *key, char *value) {
  unsigned long h = hash(key);

  pair *hash_table = map->hash_table;
  pair *p = &hash_table[h];

  p->key = strdup(key);
  p->value = strdup(value);
}
