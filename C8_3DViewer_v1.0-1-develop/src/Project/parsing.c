#include "3DViewer.h"

int count_of_facets(char *str) {
  int ch_count = 0;
  while (*str) {
    if (*str == ' ' && *(str + 1) && strchr("123456789", *(str + 1)))
      ch_count++;
    str++;
  }
  return ch_count;
}

int create_matrix(data_t *data) {
  int return_value = 0;
  data->matrix_3d =
      malloc(data->count_of_vertexes * COORDINATES_COUNT * sizeof(double) +
             data->count_of_vertexes * sizeof(double *));
  data->matrix_3d_original =
      malloc(data->count_of_vertexes * COORDINATES_COUNT * sizeof(double) +
             data->count_of_vertexes * sizeof(double *));
  if (data->matrix_3d && data->matrix_3d_original) {
    double *ptr = (double *)(data->matrix_3d + data->count_of_vertexes);
    for (int i = 0; i < data->count_of_vertexes; i++)
      data->matrix_3d[i] = ptr + COORDINATES_COUNT * i;
    ptr = (double *)(data->matrix_3d_original + data->count_of_vertexes);
    for (int i = 0; i < data->count_of_vertexes; i++)
      data->matrix_3d_original[i] = ptr + COORDINATES_COUNT * i;
  } else {
    return_value = 1;
  }
  return return_value;
}

int polygons_struct_record(data_t *data, char *line, unsigned polygon_index) {
  int return_value = 0;
  unsigned vertex_in_polygon_index = 0;
  char *endstr = NULL;
  data->polygons[polygon_index].numbers_of_vertexes_in_facets =
      count_of_facets(line);
  data->polygons[polygon_index].vertexes =
      malloc(data->polygons[polygon_index].numbers_of_vertexes_in_facets *
             sizeof(unsigned));
  if (data->polygons[polygon_index].vertexes) {
    while (*line) {
      if (*line == ' ' && *(line + 1) && strchr("123456789", *(line + 1))) {
        data->polygons[polygon_index].vertexes[vertex_in_polygon_index++] =
            (unsigned)(strtoul(line, &endstr, 10));
        line = endstr;
      } else {
        line++;
      }
    }
    polygon_index++;
  } else {
    return_value = 1;
  }
  return return_value;
}

int array_polygons_record(data_t *data) {
  int return_value = 0;
  unsigned array_of_polygons_index = 0;
  for (int i = 0; i < data->count_of_facets; i++)
    data->array_polygons_size +=
        data->polygons[i].numbers_of_vertexes_in_facets *
        (data->polygons[i].numbers_of_vertexes_in_facets - 1);
  data->array_polygons = malloc(data->array_polygons_size * sizeof(unsigned));
  if (data->array_polygons) {
    for (int k = 0; k < data->count_of_facets; k++) {
      for (unsigned i = 0;
           i < data->polygons[k].numbers_of_vertexes_in_facets - 1; i++) {
        for (unsigned j = i + 1;
             j < data->polygons[k].numbers_of_vertexes_in_facets; j++) {
          data->array_polygons[array_of_polygons_index++] =
              data->polygons[k].vertexes[i] - 1;
          data->array_polygons[array_of_polygons_index++] =
              data->polygons[k].vertexes[j] - 1;
        }
      }
    }
  } else {
    return_value = 1;
  }
  return return_value;
}

int record(data_t *data, FILE *fp) {
  int return_value = 0;
  char *line = NULL;
  size_t line_len = 0;
  int matrix_3d_index = 0;
  unsigned polygon_index = 0;
  while (getline(&line, &line_len, fp) != -1 && return_value == 0) {
    if (*line == 'v' && *(line + 1) == ' ') {
      sscanf(line, "v %lf %lf %lf", &data->matrix_3d[matrix_3d_index][0],
             &data->matrix_3d[matrix_3d_index][1],
             &data->matrix_3d[matrix_3d_index][2]);
      sscanf(line, "v %lf %lf %lf",
             &data->matrix_3d_original[matrix_3d_index][0],
             &data->matrix_3d_original[matrix_3d_index][1],
             &data->matrix_3d_original[matrix_3d_index][2]);
      matrix_3d_index++;
    } else if (*line == 'f' && *(line + 1) == ' ') {
      return_value = polygons_struct_record(data, line, polygon_index);
      polygon_index++;
    }
  }
  if (return_value == 0) return_value = array_polygons_record(data);
  if (line) free(line);
  return return_value;
}

int parse(char *file, data_t *data) {
  data->count_of_facets = 0;
  data->count_of_vertexes = 0;
  data->array_polygons_size = 0;
  int return_value = 0;
  FILE *fp = NULL;
  char *line = NULL;
  size_t line_len = 0;
  if ((fp = fopen(file, "r")) != NULL) {
    while (getline(&line, &line_len, fp) != -1) {
      if (*line == 'v' && *(line + 1) == ' ')
        data->count_of_vertexes++;
      else if (*line == 'f' && *(line + 1) == ' ')
        data->count_of_facets++;
    }
    rewind(fp);
    return_value = create_matrix(data);
    if (return_value == 0) {
      data->polygons = malloc(data->count_of_facets * sizeof(polygon_t));
      if (data->polygons)
        return_value = record(data, fp);
      else
        return_value = 1;
    }
    data->max_c =
        max(max_coord(data, 0), max(max_coord(data, 1), max_coord(data, 2)));
    if (line) free(line);
    fclose(fp);
  } else {
    return_value = 1;
  }
  return return_value;
}

double max(double a, double b) { return ((a > b) ? a : b); }

double max_coord(data_t *data, int k) {
  double max = data->matrix_3d[0][0];
  for (int i = 0; i < data->count_of_vertexes; i++)
    if (data->matrix_3d[i][k] > max) max = data->matrix_3d[i][k];
  return max;
}

void rm_data(data_t *data) {
  for (int i = 0; i < data->count_of_facets; i++)
    if (data->polygons[i].vertexes) free(data->polygons[i].vertexes);
  if (data->polygons) free(data->polygons);
  if (data->matrix_3d) free(data->matrix_3d);
  if (data->matrix_3d_original) free(data->matrix_3d_original);
  if (data->array_polygons) free(data->array_polygons);
}

/*int main(void) {
    data_t data;
    parse("seat.obj", &data);

    for (int i = 0; i < data.count_of_facets; i++) {
        printf("%d\n", data.polygons[i].numbers_of_vertexes_in_facets);
        for (unsigned j = 0; j < data.polygons[i].numbers_of_vertexes_in_facets;
j++) { printf("%d ", data.polygons[i].vertexes[j]);
        }
        printf("\n");
    }
    printf("\n");
    printf("%d\n", data.array_polygons_size);
    for (unsigned i = 0; i < data.array_polygons_size; i++)
        printf("%d ", data.array_polygons[i]);


    rm_data(&data);
    return 0;
}*/
