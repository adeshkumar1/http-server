#pragma once
#include "map.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

void map_html_files(map *m, const char *base_dir, const char *rel_path);
