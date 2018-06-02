{
  typedef double worktype;
  worktype* centers = 0;
  worktype* old_centers = 0;
  worktype* temp = 0;
  int* counters = 0;
  int i, j, k;
  int iter;
  double max_dist;
  double epsilon = 1e-6;
  int max_iter = 100;

  if (cluster_count < 1) {
    CC_ERROR(CC_StsOutOfRange, "Number of clusters should be positive");
  }

  // cluster_idx should be 1D vector of the same number of elements as samples' number of rows

  if (cluster_count > sample_count) {
    cluster_count = sample_count;
  }

  centers = MALLOC(worktype, cluster_count * dims);
  old_centers = MALLOC(worktype, cluster_count * dims);
  counters = MALLOC(int, cluster_count);

  // init centers
  for (i = 0; i < sample_count; i++) {
    labels[i] = cvRandInt() % cluster_count;
  }

  max_dist = epsilon * 2;

  for (iter = 0; iter < max_iter; iter++) {
    worktype* c;
    const arrtype* s;

    // computer centers
    for (i = 0; i < cluster_count * dims; i++) {
      centers[i] = 0;
    }

    MEMSET(counters, 0, cluster_count);

    for (i = 0; i < sample_count; i++) {
      s = (samples + i * samples_step);
      k = labels[i];
      c = (centers + k * dims);

      for (j = 0; j <= dims - 4; j += 4) {
        double t0 = c[j] + s[j];
        double t1 = c[j + 1] + s[j + 1];

        c[j] = t0;
        c[j + 1] = t1;

        t0 = c[j + 2] + s[j + 2];
        t1 = c[j + 3] + s[j + 3];

        c[j + 2] = t0;
        c[j + 3] = t1;
      }

      for (; j < dims; j++) {
        c[j] += s[j];
      }

      counters[k]++;
    }

    if (iter > 0) {
      max_dist = 0;
    }

    for (k = 0; k < cluster_count; k++) {
      c = (centers + k * dims);

      if (counters[k] != 0) {
        double scale = 1. / counters[k];

        for (j = 0; j < dims; j++) {
          c[j] *= scale;
        }
      }
      else {
        i = cvRandInt() % sample_count;
        s = (samples + i * samples_step);

        for (j = 0; j < dims; j++) {
          c[j] = s[j];
        }
      }

      if (iter > 0) {
        double dist = 0;
        worktype* c_o = (old_centers + k * dims);

        for (j = 0; j < dims; j++) {
          double t = c[j] - c_o[j];
          dist += t * t;
        }

        if (max_dist < dist) {
          max_dist = dist;
        }
      }
    }

    // assign labels
    for (i = 0; i < sample_count; i++) {
      const arrtype* s = (samples + i * samples_step);
      int k_best = 0;
      double min_dist = DBL_MAX;

      for (k = 0; k < cluster_count; k++) {
        double* c = (double*)(centers + k * dims);
        double dist = 0;

        j = 0;

        for (; j <= dims - 4; j += 4) {
          double t0 = c[j] - s[j];
          double t1 = c[j + 1] - s[j + 1];
          dist += t0 * t0 + t1 * t1;
          t0 = c[j + 2] - s[j + 2];
          t1 = c[j + 3] - s[j + 3];
          dist += t0 * t0 + t1 * t1;
        }

        for (; j < dims; j++) {
          double t = c[j] - s[j];
          dist += t * t;
        }

        if (min_dist > dist) {
          min_dist = dist;
          k_best = k;
        }
      }

      labels[i] = k_best;
    }

    if (max_dist < epsilon) {
      break;
    }

    CC_SWAP(centers, old_centers, temp);
  }

  MEMSET(counters, 0, cluster_count);

  for (i = 0; i < sample_count; i++) {
    counters[labels[i]]++;
  }

  // ensure that we do not have empty clusters
  for (k = 0; k < cluster_count; k++) {
    if (counters[k] == 0) {
      for (;;) {
        i = cvRandInt() % sample_count;
        j = labels[i];

        if (counters[j] > 1) {
          labels[i] = k;
          counters[j]--;
          counters[k]++;
          break;
        }
      }
    }
  }

  FREE(centers);
  FREE(old_centers);
  FREE(counters);
}