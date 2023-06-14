#include <check.h>
#include <math.h>

#include "../Project/3DViewer.h"

START_TEST(test_parsing_1) {
  data_t data = {0, 0, NULL, NULL, NULL, 0, NULL, 0};
  char file[] = "Objects/cube.obj";
  int result = parse(file, &data);
  ck_assert_int_eq(result, 0);
  ck_assert_int_eq(data.count_of_vertexes, 8);
  ck_assert_int_eq(data.count_of_facets, 12);
  ck_assert_int_eq(data.array_polygons_size, 72);
  ck_assert_double_eq(data.matrix_3d[0][0], -0.5);
  ck_assert_double_eq(data.matrix_3d[0][1], -0.5);
  ck_assert_double_eq(data.matrix_3d[0][2], 0.5);
  ck_assert_double_eq(data.matrix_3d[7][0], 0.5);
  ck_assert_double_eq(data.matrix_3d[7][1], -0.5);
  ck_assert_double_eq(data.matrix_3d[7][2], -0.5);
  rm_data(&data);
}
END_TEST

START_TEST(test_parsing_2) {
  data_t data = {0, 0, NULL, NULL, NULL, 0, NULL, 0};
  char file[] = "none.obj";
  int result = parse(file, &data);
  ck_assert_int_eq(result, 1);
  rm_data(&data);
}
END_TEST

START_TEST(test_affine_1) {
  data_t data_1 = {0, 0, NULL, NULL, NULL, 0, NULL, 0};
  data_t data_2 = {0, 0, NULL, NULL, NULL, 0, NULL, 0};
  char file[] = "Objects/cube.obj";
  parse(file, &data_1);
  parse(file, &data_2);
  move_x(&data_2, 1);
  for (int i = 0; i < data_1.count_of_vertexes; i++)
    ck_assert_double_eq(data_1.matrix_3d[i][0] + 1, data_2.matrix_3d[i][0]);
  rm_data(&data_1);
  rm_data(&data_2);
}
END_TEST

START_TEST(test_affine_2) {
  data_t data_1 = {0, 0, NULL, NULL, NULL, 0, NULL, 0};
  data_t data_2 = {0, 0, NULL, NULL, NULL, 0, NULL, 0};
  char file[] = "Objects/cube.obj";
  parse(file, &data_1);
  parse(file, &data_2);
  move_y(&data_2, -0.75);
  for (int i = 0; i < data_1.count_of_vertexes; i++)
    ck_assert_double_eq(data_1.matrix_3d[i][1] - 0.75, data_2.matrix_3d[i][1]);
  rm_data(&data_1);
  rm_data(&data_2);
}
END_TEST

START_TEST(test_affine_3) {
  data_t data_1 = {0, 0, NULL, NULL, NULL, 0, NULL, 0};
  data_t data_2 = {0, 0, NULL, NULL, NULL, 0, NULL, 0};
  char file[] = "Objects/cube.obj";
  parse(file, &data_1);
  parse(file, &data_2);
  move_z(&data_2, -0.45);
  for (int i = 0; i < data_1.count_of_vertexes; i++)
    ck_assert_double_eq(data_1.matrix_3d[i][2] - 0.45, data_2.matrix_3d[i][2]);
  rm_data(&data_1);
  rm_data(&data_2);
}
END_TEST

START_TEST(test_affine_4) {
  double temp_y, temp_z;
  double angle = 15 * PI / 180.0;
  data_t data_1 = {0, 0, NULL, NULL, NULL, 0, NULL, 0};
  data_t data_2 = {0, 0, NULL, NULL, NULL, 0, NULL, 0};
  char file[] = "Objects/cube.obj";
  parse(file, &data_1);
  parse(file, &data_2);
  rotation_by_ox(&data_2, 60, 75);
  for (int i = 0; i < data_1.count_of_vertexes; i++) {
    for (int j = 0; j < COORDINATES_COUNT; j++) {
      temp_y = data_1.matrix_3d[i][1];
      temp_z = data_1.matrix_3d[i][2];
      if (j == 1)
        ck_assert_double_eq_tol(cos(angle) * temp_y + sin(angle) * temp_z,
                                data_2.matrix_3d[i][j], 0.0000001);
      else if (j == 2)
        ck_assert_double_eq_tol(-sin(angle) * temp_y + cos(angle) * temp_z,
                                data_2.matrix_3d[i][j], 0.0000001);
    }
  }
  rm_data(&data_1);
  rm_data(&data_2);
}
END_TEST

START_TEST(test_affine_5) {
  double temp_x, temp_z;
  double angle = 60 * PI / 180.0;
  data_t data_1 = {0, 0, NULL, NULL, NULL, 0, NULL, 0};
  data_t data_2 = {0, 0, NULL, NULL, NULL, 0, NULL, 0};
  char file[] = "Objects/cube.obj";
  parse(file, &data_1);
  parse(file, &data_2);
  rotation_by_oy(&data_2, 0, 60);
  for (int i = 0; i < data_1.count_of_vertexes; i++) {
    for (int j = 0; j < COORDINATES_COUNT; j++) {
      temp_x = data_1.matrix_3d[i][0];
      temp_z = data_1.matrix_3d[i][2];
      if (j == 0)
        ck_assert_double_eq_tol(cos(angle) * temp_x - sin(angle) * temp_z,
                                data_2.matrix_3d[i][j], 0.0000001);
      else if (j == 2)
        ck_assert_double_eq_tol(sin(angle) * temp_x + cos(angle) * temp_z,
                                data_2.matrix_3d[i][j], 0.0000001);
    }
  }
  rm_data(&data_1);
  rm_data(&data_2);
}
END_TEST

START_TEST(test_affine_6) {
  double temp_x, temp_y;
  double angle = 64 * PI / 180.0;
  data_t data_1 = {0, 0, NULL, NULL, NULL, 0, NULL, 0};
  data_t data_2 = {0, 0, NULL, NULL, NULL, 0, NULL, 0};
  char file[] = "Objects/cube.obj";
  parse(file, &data_1);
  parse(file, &data_2);
  rotation_by_oz(&data_2, 30, 94);
  for (int i = 0; i < data_1.count_of_vertexes; i++) {
    for (int j = 0; j < COORDINATES_COUNT; j++) {
      temp_x = data_1.matrix_3d[i][0];
      temp_y = data_1.matrix_3d[i][1];
      if (j == 0)
        ck_assert_double_eq_tol(cos(angle) * temp_x + sin(angle) * temp_y,
                                data_2.matrix_3d[i][j], 0.0000001);
      else if (j == 1)
        ck_assert_double_eq_tol(-sin(angle) * temp_x + cos(angle) * temp_y,
                                data_2.matrix_3d[i][j], 0.0000001);
    }
  }
  rm_data(&data_1);
  rm_data(&data_2);
}
END_TEST

START_TEST(test_affine_7) {
  data_t data_1 = {0, 0, NULL, NULL, NULL, 0, NULL, 0};
  data_t data_2 = {0, 0, NULL, NULL, NULL, 0, NULL, 0};
  char file[] = "Objects/cube.obj";
  parse(file, &data_1);
  parse(file, &data_2);
  scale_x(&data_2, 2);
  for (int i = 0; i < data_1.count_of_vertexes; i++)
    ck_assert_double_eq(data_1.matrix_3d[i][0] * 2, data_2.matrix_3d[i][0]);
  rm_data(&data_1);
  rm_data(&data_2);
}
END_TEST

START_TEST(test_affine_8) {
  data_t data_1 = {0, 0, NULL, NULL, NULL, 0, NULL, 0};
  data_t data_2 = {0, 0, NULL, NULL, NULL, 0, NULL, 0};
  char file[] = "Objects/cube.obj";
  parse(file, &data_1);
  parse(file, &data_2);
  scale_y(&data_2, 0.75);
  for (int i = 0; i < data_1.count_of_vertexes; i++)
    ck_assert_double_eq(data_1.matrix_3d[i][1] * 0.75, data_2.matrix_3d[i][1]);
  rm_data(&data_1);
  rm_data(&data_2);
}
END_TEST

START_TEST(test_affine_9) {
  data_t data_1 = {0, 0, NULL, NULL, NULL, 0, NULL, 0};
  data_t data_2 = {0, 0, NULL, NULL, NULL, 0, NULL, 0};
  char file[] = "Objects/cube.obj";
  parse(file, &data_1);
  parse(file, &data_2);
  scale_z(&data_2, 4.5);
  for (int i = 0; i < data_1.count_of_vertexes; i++)
    ck_assert_double_eq(data_1.matrix_3d[i][2] * 4.5, data_2.matrix_3d[i][2]);
  rm_data(&data_1);
  rm_data(&data_2);
}
END_TEST

int main(void) {
  Suite* s1 = suite_create("parser");
  TCase* test_case_1 = tcase_create("parser");
  SRunner* sr = srunner_create(s1);
  int res;
  srunner_set_fork_status(sr, CK_NOFORK);
  suite_add_tcase(s1, test_case_1);

  tcase_add_test(test_case_1, test_parsing_1);
  tcase_add_test(test_case_1, test_parsing_2);

  tcase_add_test(test_case_1, test_affine_1);
  tcase_add_test(test_case_1, test_affine_2);
  tcase_add_test(test_case_1, test_affine_3);
  tcase_add_test(test_case_1, test_affine_4);
  tcase_add_test(test_case_1, test_affine_5);
  tcase_add_test(test_case_1, test_affine_6);
  tcase_add_test(test_case_1, test_affine_7);
  tcase_add_test(test_case_1, test_affine_8);
  tcase_add_test(test_case_1, test_affine_9);

  srunner_run_all(sr, CK_ENV);
  res = srunner_ntests_failed(sr);

  srunner_free(sr);
  return res == 0 ? 0 : 1;
}
