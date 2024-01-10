#include "transform_obj.h"

void rotate_x(vertexes_data *vertex, float angle) {
  angle = angle * S21_PI / 180;
  for (int i = 0; i < vertex->vertex_count * AXIS_COUNT; i += AXIS_COUNT) {
    float axis_y = vertex->vertexes_list[i + Y];
    float axis_z = vertex->vertexes_list[i + Z];
    vertex->vertexes_list[i + Y] = axis_y * cos(angle) + axis_z * sin(angle);
    vertex->vertexes_list[i + Z] = -axis_y * sin(angle) + axis_z * cos(angle);
  }
}

void rotate_y(vertexes_data *vertex, float angle) {
  angle = angle * S21_PI / 180;
  for (int i = 0; i < vertex->vertex_count * AXIS_COUNT; i += AXIS_COUNT) {
    float axis_x = vertex->vertexes_list[i + X];
    float axis_z = vertex->vertexes_list[i + Z];
    vertex->vertexes_list[i + X] = axis_x * cos(angle) + axis_z * sin(angle);
    vertex->vertexes_list[i + Z] = -axis_x * sin(angle) + axis_z * cos(angle);
  }
}

void rotate_z(vertexes_data *vertex, float angle) {
  angle = angle * S21_PI / 180;
  for (int i = 0; i < vertex->vertex_count * AXIS_COUNT; i += AXIS_COUNT) {
    float axis_x = vertex->vertexes_list[i + X];
    float axis_y = vertex->vertexes_list[i + Y];
    vertex->vertexes_list[i + X] = axis_x * cos(angle) + axis_y * sin(angle);
    vertex->vertexes_list[i + Y] = -axis_x * sin(angle) + axis_y * cos(angle);
  }
}

void move_x(vertexes_data *vertex, const float val) {
  for (int i = X; i < vertex->vertex_count * AXIS_COUNT; i += AXIS_COUNT) {
    vertex->vertexes_list[i] += val;
  }
}

void move_y(vertexes_data *vertex, const float val) {
  for (int i = Y; i < vertex->vertex_count * AXIS_COUNT; i += AXIS_COUNT) {
    vertex->vertexes_list[i] += val;
  }
}

void move_z(vertexes_data *vertex, const float val) {
  for (int i = Z; i < vertex->vertex_count * AXIS_COUNT; i += AXIS_COUNT) {
    vertex->vertexes_list[i] += val;
  }
}
