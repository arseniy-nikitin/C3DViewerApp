#include "transform_obj.h"

int read_file(char* filename, vertexes_data* vertex_t, lines_data* line_t,
              count* schet) {
  int err_code = 0;
  char file_str[4096];
  int size_vertex = 128;
  int size_vector = 256;
  vertex_t->vertexes_list = malloc(384 * sizeof(float));
  line_t->lines_list = malloc(256 * sizeof(int));
  FILE* f1 = fopen(filename, "r");
  if (f1 && !err_code) {
    while (fgets(file_str, 4096, f1)) {
      if (strlen(file_str) > 1) {
        if (file_str[0] == 'v' && file_str[1] == ' ') {
          err_code = vertex_t_realloc(vertex_t, &size_vertex);
          err_code = write_vertex_t(file_str, vertex_t, schet);
        } else if (file_str[0] == 'f' && file_str[1] == ' ') {
          err_code = polygon_realloc(line_t, &size_vector);
          err_code = write_polygon(file_str, line_t, (*vertex_t));
        }
      }
    }
  } else
    err_code = 1;
  fclose(f1);
  center(vertex_t, schet);
  return err_code;
}

void center(vertexes_data* vertex_t, count* schet) {
  float xcenter = schet->xmin + (schet->xmax - schet->xmin) / 2;
  float ycenter = schet->ymin + (schet->ymax - schet->ymin) / 2;
  float zcenter = schet->zmin + (schet->zmax - schet->zmin) / 2;
  for (int i = 0; i < vertex_t->vertex_count; i++) {
    vertex_t->vertexes_list[i * 3] -= xcenter;
    vertex_t->vertexes_list[i * 3 + 1] -= ycenter;
    vertex_t->vertexes_list[i * 3 + 2] -= zcenter;
  }
}

int polygon_realloc(lines_data* line_t, int* size_vector) {
  int err_code = 0;
  if (line_t->line_count >= *size_vector - 128) {
    *size_vector = *size_vector * 2;
    line_t->lines_list =
        realloc(line_t->lines_list, *size_vector * sizeof(float));
  }
  return err_code;
}

int write_polygon(char* str, lines_data* line_t, vertexes_data vertex_t) {
  int err_code = 0, i = 2;
  int p = 0;
  i += write_int(str + i, &p, vertex_t, &err_code);
  int first_vert = p;
  line_t->lines_list[line_t->line_count] = p;
  while (str[i] != '\0') {
    line_t->line_count = line_t->line_count + 2;
    i += write_int(str + i, &p, vertex_t, &err_code);
    if (p >= 0) {
      line_t->lines_list[line_t->line_count - 1] = p;
      line_t->lines_list[line_t->line_count] = p;
    } else
      line_t->line_count = line_t->line_count - 2;
  }
  line_t->line_count = line_t->line_count + 1;
  line_t->lines_list[line_t->line_count] = first_vert;
  line_t->line_count = line_t->line_count + 1;
  return err_code;
}

int write_int(char* str, int* res, vertexes_data vertex_t, int* err_code) {
  int i = 0;
  char bufer[256] = {0};
  char* numstr = "-0123456789";
  while (strchr(numstr, str[i]) && str[i]) i++;
  int j = 0;
  for (; j < i; j++) {
    bufer[j] = str[j];
  }
  bufer[j + 1] = '\0';
  *res = atoi(bufer);
  if (*res > vertex_t.vertex_count) *err_code = 1;
  if (*res < 0) *res = vertex_t.vertex_count + *res + 1;
  while (str[i] != ' ' && str[i] != '\0') i++;
  if (str[i] != '\0') i++;
  *res -= 1;
  return i;
}

int vertex_t_realloc(vertexes_data* vertex_t, int* size_vertex) {
  int err_code = 0;
  if (vertex_t->vertex_count == *size_vertex) {
    *size_vertex = *size_vertex * 3;
    vertex_t->vertexes_list =
        realloc(vertex_t->vertexes_list, *size_vertex * 3 * sizeof(float));
  }
  return err_code;
}

int write_vertex_t(char* str, vertexes_data* vertex_t, count* schet) {
  int err_code = 0;
  int i = 2;
  float x = 0, y = 0, z = 0;
  i += write_float(str + i, &x) + 1;
  i += write_float(str + i, &y) + 1;
  i += write_float(str + i, &z);
  if (x > schet->xmax) schet->xmax = x;
  if (y > schet->ymax) schet->ymax = y;
  if (z > schet->zmax) schet->zmax = z;
  if (x < schet->xmin) schet->xmin = x;
  if (y < schet->ymin) schet->ymin = y;
  if (z < schet->zmin) schet->zmin = z;
  vertex_t->vertexes_list[vertex_t->vertex_count * 3] = x;
  vertex_t->vertexes_list[(vertex_t->vertex_count * 3) + 1] = y;
  vertex_t->vertexes_list[(vertex_t->vertex_count * 3) + 2] = z;
  vertex_t->vertex_count = vertex_t->vertex_count + 1;
  return err_code;
}

int write_float(const char* str, float* res) {
  int i = 0;
  char bufer[256] = {0};
  char* numstr = "0123456789";
  if (str[i] == '+' || str[i] == '-') i++;
  while ((strchr(numstr, str[i]) || str[i] == '.') && str[i]) {
    i++;
  }
  if (str[i] == 'e' || str[i] == 'E') {
    i++;
    if (str[i] == '+' || str[i] == '-') i++;
    while (strchr(numstr, str[i]) && str[i]) i++;
  }
  int j = 0;
  for (; j < i; j++) {
    bufer[j] = str[j];
  }
  bufer[j + 1] = '\0';
  *res = atof(bufer);
  return i;
}

void delete_struct(vertexes_data* vertex, lines_data* lines, count* schet) {
  if (vertex->vertexes_list != NULL) {
    free(vertex->vertexes_list);
  }
  vertex->vertex_count = 0;
  if (lines->lines_list != NULL) {
    free(lines->lines_list);
  }
  lines->line_count = 0;
  schet->xmax = 0;
  schet->xmin = 0;
  schet->ymax = 0;
  schet->ymin = 0;
  schet->zmax = 0;
  schet->zmin = 0;
}
