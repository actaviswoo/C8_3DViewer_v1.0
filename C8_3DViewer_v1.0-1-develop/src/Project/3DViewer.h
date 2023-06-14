#ifndef VIEWER_H_
#define VIEWER_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PI 3.14159265358979323846
#define COORDINATES_COUNT 3
#define EPS 0.001

typedef struct {
  unsigned *vertexes;
  unsigned numbers_of_vertexes_in_facets;
} polygon_t;

typedef struct {
  int count_of_vertexes;
  int count_of_facets;
  double **matrix_3d;
  double **matrix_3d_original;
  polygon_t *polygons;
  unsigned array_polygons_size;
  unsigned *array_polygons;
  double max_c;
} data_t;

// parsing.c
int count_of_facets(char *str);
int create_matrix(data_t *data);
int polygons_struct_record(data_t *data, char *line, unsigned polygon_index);
int array_polygons_record(data_t *data);
int record(data_t *data, FILE *fp);
int parse(char *file, data_t *data);
double max(double a, double b);
double max_coord(data_t *data, int k);
void rm_data(data_t *data);

// affine.c
void move_x(data_t *data, double a);
void move_y(data_t *data, double a);
void move_z(data_t *data, double a);
void rotation_by_ox(data_t *data, double old_angle, double new_angle);
void rotation_by_oy(data_t *data, double old_angle, double new_angle);
void rotation_by_oz(data_t *data, double old_angle, double new_angle);
void scale_x(data_t *data, double scale);
void scale_y(data_t *data, double scale);
void scale_z(data_t *data, double scale);

#endif  // VIEWER_H_
