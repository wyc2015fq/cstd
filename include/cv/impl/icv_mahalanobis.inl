
{
  int i, j;
  double result = 0;

  double* vec = 0;

  vec = MALLOC(arrtype, len);

  for (i = 0; i < len - 3; i += 4) {
    vec[i] = vec1[i] - vec2[i];
    vec[i + 1] = vec1[i + 1] - vec2[i + 1];
    vec[i + 2] = vec1[i + 2] - vec2[i + 2];
    vec[i + 3] = vec1[i + 3] - vec2[i + 3];
  }

  for (; i < len; ++i) {
    vec[i] = vec1[i] - vec2[i];
  }


  for (i = 0; i < len; i++, mat += matstep) {
    double row_sum = 0;

    for (j = 0; j <= len - 4; j += 4) {
      row_sum += vec[j] * mat[j] + vec[j + 1] * mat[j + 1] + vec[j + 2] * mat[j + 2] + vec[j + 3] * mat[j + 3];
    }

    for (; j < len; j++) {
      row_sum += vec[j] * mat[j];
    }

    result += row_sum * vec[i];
  }

  dist = sqrt(dist);

  FREE(vec);
}
