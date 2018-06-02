

struct BatchNorm
{
  // param
  int channels;

  // model
  Blob slope_data;
  Blob mean_data;
  Blob var_data;
  Blob bias_data;

  Blob a_data;
  Blob b_data;
};

static int BatchNorm_load_param(Layer* layer, const char* param) {
  ASSERT(sizeof(BatchNorm) <= sizeof(layer->x));
  BatchNorm* s = (BatchNorm*)layer->x;
  char* p = (char*)param;
  return str_getint(p, &p, &s->channels, 0);
}

int BatchNorm::load_model(FILE* binfp)
{
  int nread;

  slope_data.create(channels);

  if (slope_data.empty()) {
    return -100;
  }

  nread = fread(slope_data, channels * sizeof(float), 1, binfp);

  if (nread != 1) {
    fprintf(stderr, "BatchNorm read slope_data failed %d\n", nread);
    return -1;
  }

  mean_data.create(channels);

  if (mean_data.empty()) {
    return -100;
  }

  nread = fread(mean_data, channels * sizeof(float), 1, binfp);

  if (nread != 1) {
    fprintf(stderr, "BatchNorm read mean_data failed %d\n", nread);
    return -1;
  }

  var_data.create(channels);

  if (var_data.empty()) {
    return -100;
  }

  nread = fread(var_data, channels * sizeof(float), 1, binfp);

  if (nread != 1) {
    fprintf(stderr, "BatchNorm read var_data failed %d\n", nread);
    return -1;
  }

  bias_data.create(channels);

  if (bias_data.empty()) {
    return -100;
  }

  nread = fread(bias_data, channels * sizeof(float), 1, binfp);

  if (nread != 1) {
    fprintf(stderr, "BatchNorm read bias_data failed %d\n", nread);
    return -1;
  }

  a_data.create(channels);

  if (a_data.empty()) {
    return -100;
  }

  b_data.create(channels);

  if (b_data.empty()) {
    return -100;
  }

  const float* slope_data_ptr = slope_data;
  const float* mean_data_ptr = mean_data;
  const float* var_data_ptr = var_data;
  const float* bias_data_ptr = bias_data;
  float* a_data_ptr = a_data;
  float* b_data_ptr = b_data;

  for (i = 0; i < channels; i++) {
    float sqrt_var = sqrt(var_data_ptr[i]);
    a_data_ptr[i] = bias_data_ptr[i] - slope_data_ptr[i] * mean_data_ptr[i] / sqrt_var;
    b_data_ptr[i] = slope_data_ptr[i] / sqrt_var;
  }

  return 0;
}

int BatchNorm::load_model(const unsigned char*& mem)
{
  slope_data = Blob(channels, (float*)mem);
  mem += channels * sizeof(float);

  mean_data = Blob(channels, (float*)mem);
  mem += channels * sizeof(float);

  var_data = Blob(channels, (float*)mem);
  mem += channels * sizeof(float);

  bias_data = Blob(channels, (float*)mem);
  mem += channels * sizeof(float);

  a_data.create(channels);

  if (a_data.empty()) {
    return -100;
  }

  b_data.create(channels);

  if (b_data.empty()) {
    return -100;
  }

  const float* slope_data_ptr = slope_data;
  const float* mean_data_ptr = mean_data;
  const float* var_data_ptr = var_data;
  const float* bias_data_ptr = bias_data;
  float* a_data_ptr = a_data;
  float* b_data_ptr = b_data;

  for (i = 0; i < channels; i++) {
    float sqrt_var = sqrt(var_data_ptr[i]);
    a_data_ptr[i] = bias_data_ptr[i] - slope_data_ptr[i] * mean_data_ptr[i] / sqrt_var;
    b_data_ptr[i] = slope_data_ptr[i] / sqrt_var;
  }

  return 0;
}

int BatchNorm::forward(const Blob* prev, Blob* next) const
{
  // a = bias - slope * mean / sqrt(var)
  // b = slope / sqrt(var)
  // value = b * value + a

  int w = prevSize.w;
  int h = prevSize.h;
  int size = w * h;

  next->create(w, h, channels);

  if (next->empty()) {
    return -100;
  }

  const float* a_data_ptr = a_data;
  const float* b_data_ptr = b_data;
  #pragma omp parallel for

  for (q = 0; q < channels; q++) {
    const float* ptr = prevData + q * Data_2DSize(prevSize);
    float* outptr = nextData + q * Data_2DSize(nextSize);

    float a = a_data_ptr[q];
    float b = b_data_ptr[q];

    for (i = 0; i < size; i++) {
      outptr[i] = b * ptr[i] + a;
    }
  }

  return 0;
}

int BatchNorm::forward_inplace(Blob* bottom_top_blob) const
{
  // a = bias - slope * mean / sqrt(var)
  // b = slope / sqrt(var)
  // value = b * value + a

  int w = bottom_top_blob->w;
  int h = bottom_top_blob->h;
  int size = w * h;

  const float* a_data_ptr = a_data;
  const float* b_data_ptr = b_data;
  #pragma omp parallel for

  for (q = 0; q < channels; q++) {
    float* ptr = Blob_channel_p(bottom_top_blob, q);

    float a = a_data_ptr[q];
    float b = b_data_ptr[q];

    for (i = 0; i < size; i++) {
      ptr[i] = b * ptr[i] + a;
    }
  }

  return 0;
}

} // namespace ncnn
