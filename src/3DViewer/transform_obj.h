#ifndef TRANSFORM_OBJ_H
#define TRANSFORM_OBJ_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define S21_PI 3.1415927

#define X 0
#define Y 1
#define Z 2
#define AXIS_COUNT 3

typedef struct vertexes_data {
  float* vertexes_list;
  int vertex_count;
} vertexes_data;

typedef struct lines_data {
  int* lines_list;
  int line_count;
} lines_data;

typedef struct count {
  float xmax;
  float ymax;
  float zmax;
  float xmin;
  float ymin;
  float zmin;
} count;

int read_file(char* filename, vertexes_data* vertex_t, lines_data* line_t,
              count* schet);
int write_vertex_t(char* str, vertexes_data* vertex_t, count* schet);
int write_float(const char* str, float* res);
int vertex_t_realloc(vertexes_data* vertex_t, int* size_vertex);
int polygon_realloc(lines_data* line_t, int* size_vector);
int write_polygon(char* str, lines_data* line_t, vertexes_data vertex_t);
int write_int(char* str, int* res, vertexes_data vertex_t, int* err_code);
void center(vertexes_data* vertex_t, count* schet);
void rotate_x(vertexes_data* vertex, float angle);
void rotate_y(vertexes_data* vertex, float angle);
void rotate_z(vertexes_data* vertex, float angle);
void move_x(vertexes_data* vertex, const float val);
void move_y(vertexes_data* vertex, const float val);
void move_z(vertexes_data* vertex, const float val);
void delete_struct(vertexes_data* vertex, lines_data* lines, count* schet);

#endif
