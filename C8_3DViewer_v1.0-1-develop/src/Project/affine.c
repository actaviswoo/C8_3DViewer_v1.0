#include "3DViewer.h"

void move_x(data_t *data, double a) {
  for (int i = 0; i < data->count_of_vertexes; i++) data->matrix_3d[i][0] += a;
}

void move_y(data_t *data, double a) {
  for (int i = 0; i < data->count_of_vertexes; i++) data->matrix_3d[i][1] += a;
}

void move_z(data_t *data, double a) {
  for (int i = 0; i < data->count_of_vertexes; i++) data->matrix_3d[i][2] += a;
}

void rotation_by_ox(data_t *data, double old_angle, double new_angle) {
  double temp_y, temp_z;
  old_angle *= PI / 180.0;
  new_angle *= PI / 180.0;
  for (int i = 0; i < data->count_of_vertexes; i++) {
    temp_y = data->matrix_3d[i][1];
    temp_z = data->matrix_3d[i][2];
    data->matrix_3d[i][1] = cos(new_angle - old_angle) * temp_y +
                            sin(new_angle - old_angle) * temp_z;
    data->matrix_3d[i][2] = -sin(new_angle - old_angle) * temp_y +
                            cos(new_angle - old_angle) * temp_z;
  }
}

void rotation_by_oy(data_t *data, double old_angle, double new_angle) {
  double temp_x, temp_z;
  old_angle *= PI / 180.0;
  new_angle *= PI / 180.0;
  for (int i = 0; i < data->count_of_vertexes; i++) {
    temp_x = data->matrix_3d[i][0];
    temp_z = data->matrix_3d[i][2];
    data->matrix_3d[i][0] = cos(new_angle - old_angle) * temp_x -
                            sin(new_angle - old_angle) * temp_z;
    data->matrix_3d[i][2] = sin(new_angle - old_angle) * temp_x +
                            cos(new_angle - old_angle) * temp_z;
  }
}

void rotation_by_oz(data_t *data, double old_angle, double new_angle) {
  double temp_x, temp_y;
  old_angle *= PI / 180.0;
  new_angle *= PI / 180.0;
  for (int i = 0; i < data->count_of_vertexes; i++) {
    temp_x = data->matrix_3d[i][0];
    temp_y = data->matrix_3d[i][1];
    data->matrix_3d[i][0] = cos(new_angle - old_angle) * temp_x +
                            sin(new_angle - old_angle) * temp_y;
    data->matrix_3d[i][1] = -sin(new_angle - old_angle) * temp_x +
                            cos(new_angle - old_angle) * temp_y;
  }
}

void scale_x(data_t *data, double scale) {
  if (scale > EPS)
    for (int i = 0; i < data->count_of_vertexes; i++)
      data->matrix_3d[i][0] *= scale;
}

void scale_y(data_t *data, double scale) {
  if (scale > EPS)
    for (int i = 0; i < data->count_of_vertexes; i++)
      data->matrix_3d[i][1] *= scale;
}

void scale_z(data_t *data, double scale) {
  if (scale > EPS)
    for (int i = 0; i < data->count_of_vertexes; i++)
      data->matrix_3d[i][2] *= scale;
}
