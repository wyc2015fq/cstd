

void FUN(transpose_fwd_bak)(const int count, const Dtype* from_data, Dtype* to_data, const DataShape from_counts, const DataShape to_counts, const DataShape map, const int num_axes) {
  int from_inds[kMaxBlobAxes] = { 0 };
  int index, i;
  for (index = 0; index < count; index++) {
    int from_index = index, to_index = 0;
    for (i = 0; i < num_axes; i++) {
      from_inds[i] = from_index / from_counts.dim[i];
      from_index = from_index % from_counts.dim[i];
    }
    for (i = 0; i < num_axes; i++) {
      to_index += from_inds[map.dim[i]] * to_counts.dim[i];
    }
    *(to_data + to_index) = *(from_data + index);
  }
}
