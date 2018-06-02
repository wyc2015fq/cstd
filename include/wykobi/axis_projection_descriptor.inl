
int generate_axis_projection_descriptor(int n, const DPOINT2* polygon, double* descriptor)
{
  enum {axis_count = 36 };
  double value[axis_count];
  T largest_value;
  int i, smallest_it;
  //value.reserve(axis_count);

  DPOINT2 origin_point = d2_make_point((0.0), (0.0));
  DPOINT2 rotated_point = d2_make_point((1.0), (0.0));

  for (i = 0; i < axis_count; ++i) {
    value[i] = se2_distance(po2_project_onto_axis(n, polygon, pt2_make_line(origin_point, rotated_point)));
    rotated_point = pt2_rotate_o((10.0), rotated_point, origin_point);
  }

  largest_value = -InfinityT;

  for (i = 0; i < axis_count; ++i) {
    if (value[i] > largest_value) {
      largest_value = value[i];
    }
  }

  for (i = 0; i < axis_count; ++i) {
    value[i] /= largest_value;
  }

  smallest_it = 0;

  for (i = 1; i < axis_count; ++i) {
    if ((value[i]) < value[smallest_it]) {
      smallest_it = i;
    }
  }

  MEMCPY(descriptor, value + smallest_it, axis_count - smallest_it);
  MEMCPY(descriptor + axis_count - smallest_it, value, smallest_it);
  return 0;
}

