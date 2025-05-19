#include "map_html_util.h"
#include "map.h"
#include <dirent.h>
#include <sys/stat.h>

int html_file(const char *filename) {
  char *dot = strrchr(filename, '.');
  if (!dot) {
    return 0;
  } else {
    return !strcmp(dot + 1, "html") || !strcmp(dot + 1, "ico");
  }
}

void map_html_files(map *m, const char *base_dir, const char *rel_path) {
  char path[1024];
  snprintf(path, sizeof(path), "%s/%s", base_dir, rel_path);
  DIR *dir = opendir(path);

  struct dirent *curr = NULL;
  while ((curr = readdir(dir)) != NULL) {
    if (!strcmp(curr->d_name, ".") || !strcmp(curr->d_name, "..")) {
      continue;
    }

    char curr_path[2048];

    snprintf(curr_path, sizeof(curr_path), "%s%s", path, curr->d_name);

    if (html_file(curr->d_name)) {
      char key[1024];
      memset(key, 0, 1024);
      char *iter = curr_path + 7;

      int i = 0;
      while (*iter && *iter != '.') {
        key[i++] = *iter++;
      }

      if (!strcmp(key, "/index")) {
        key[1] = 0;
      }
      map_insert(m, key, curr_path);
    }

    struct stat st;
    if (stat(curr_path, &st) == 0 && S_ISDIR(st.st_mode)) {
      int len = strlen(curr->d_name);
      char curr_dir[len + 2];
      strcpy(curr_dir, curr->d_name);
      curr_dir[len] = '/';
      curr_dir[len + 1] = 0;
      map_html_files(m, base_dir, curr_dir);
    }
  }

  closedir(dir);
}
