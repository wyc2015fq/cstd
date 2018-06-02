
static int innerProduct(buf_t* bf, const img_t* bottom_blob, img_t* top_blob, int num_output, const float* weight_data, const float* bias_data)
{
  int w = bottom_blob->w;
  int h = bottom_blob->h;
  int channels = pBox_channels(bottom_blob);
  int p, q, size = w * h;
  const float* weight_data_ptr = weight_data;

  bf_pBoxsetsize(bf, top_blob, 1, 1, num_output, 1);
  // num_output

  #pragma omp parallel for
  for (p = 0; p < num_output; p++) {
    float* outptr = pBox_channel_p(top_blob, p);
    float sum = 0.f;
    const float* w = weight_data_ptr + size * channels * p;
    const float* w2 = w + size;

    if (bias_data) {
      sum = bias_data[p];
    }

#if __ARM_NEON
    float32x4_t _sum = vdupq_n_f32(0.f);
    float32x4_t _sum2 = vdupq_n_f32(0.f);
#endif // __ARM_NEON

    // channels
    for (q = 0; q < channels; q++) {
      const float* m = pBox_channel_p(bottom_blob, q);
      int remain = size;

#if __ARM_NEON
      int nn = size >> 3;
      remain = size & 7;
#if __aarch64__

      for (; nn > 0; nn--) {
        float32x4_t _m = vld1q_f32(m);
        float32x4_t _w = vld1q_f32(w);
        _sum = vfmaq_f32(_sum, _m, _w);

        _m = vld1q_f32(m + 4);
        _w = vld1q_f32(w + 4);
        _sum2 = vfmaq_f32(_sum2, _m, _w);

        m += 8;
        w += 8;
      }

#else

      if (nn > 0) {
        asm volatile(
            "0:                             \n"
            "pld        [%1, #256]          \n"
            "vld1.f32   {d0-d3}, [%1 :128]! \n"
            "pld        [%2, #256]          \n"
            "vld1.f32   {d4-d7}, [%2]!      \n"
            "vmla.f32   %q3, q0, q2         \n"
            "subs       %0, #1              \n"
            "vmla.f32   %q4, q1, q3         \n"
            "bne        0b                  \n"
            : "=r"(nn),     // %0
            "=r"(m),      // %1
            "=r"(w),      // %2
            "=w"(_sum),   // %3
            "=w"(_sum2)   // %4
            : "0"(nn),
            "1"(m),
            "2"(w),
            "3"(_sum),
            "4"(_sum2)
            : "cc", "memory", "q0", "q1", "q2", "q3"
        );
      }

#endif // __aarch64__
#endif // __ARM_NEON

      for (; remain > 0; remain--) {
        sum += *m * *w;

        m++;
        w++;
      }
    }

#if __ARM_NEON
    _sum = vaddq_f32(_sum, _sum2);
#if __aarch64__
    sum += vaddvq_f32(_sum);
#else
    float32x2_t _sumss = vadd_f32(vget_low_f32(_sum), vget_high_f32(_sum));
    _sumss = vpadd_f32(_sumss, _sumss);
    sum += vget_lane_f32(_sumss, 0);
#endif // __aarch64__
#endif // __ARM_NEON

    outptr[0] = sum;
  }

#if 0
  #pragma omp parallel for

  for (p = 0; p < num_output; p++) {
    float* outptr = pBox_channel_p(top_blob, p);
    float sum = 0.f;

    if (bias_data) {
      sum = bias_data[p];
    }

    // channels
    for (q = 0; q < channels; q++) {
      const float* w = weight_data_ptr + size * channels * p + size * q;
      const float* m = pBox_channel_p(bottom_blob, q);

      for (i = 0; i < size; i++) {
        sum += m[i] * w[i];
      }
    }

    outptr[0] = sum;
  }

#endif

  return 0;
}

struct InnerProduct
{
  // param
  int num_output;
  int bias_term;
  int weight_data_size;

  // model
  float* weight_data;
  float* bias_data;
};
static int InnerProduct_load_param(Layer* layer, const char* param) {
  struct InnerProduct* s = (struct InnerProduct*)layer->x;
  char* p = (char*)param;
  str_getint(p, &p, &s->num_output, 0);
  str_getint(p, &p, &s->bias_term, 0);
  str_getint(p, &p, &s->weight_data_size, 0);
  return 0;
}
static int InnerProduct_forward(buf_t* bf, const Layer* layer, const img_t* bottom_blob, img_t* top_blob) {
  struct InnerProduct* s = (struct InnerProduct*)layer->x;
  innerProduct(bf, bottom_blob, top_blob, s->num_output, s->weight_data, s->bias_data);
  return 0;
}


#if 0

#if NCNN_STDIO
int InnerProduct::load_model(FILE* binfp)
{
  int nread;

  union {
    struct {
      unsigned char f0;
      unsigned char f1;
      unsigned char f2;
      unsigned char f3;
    };
    unsigned int tag;
  } flag_struct;

  nread = fread(&flag_struct, sizeof(flag_struct), 1, binfp);

  if (nread != 1) {
    fprintf(stderr, "InnerProduct read flag_struct failed %d\n", nread);
    return -1;
  }

  unsigned int flag = flag_struct.f0 + flag_struct.f1 + flag_struct.f2 + flag_struct.f3;

  weight_data.create(weight_data_size);

  if (weight_data.empty()) {
    return -100;
  }

  if (flag_struct.tag == 0x01306B47) {
    // half-precision weight data
    int align_weight_data_size = alignSize(weight_data_size * sizeof(unsigned short), 4);
    std::vector<unsigned short> float16_weights;
    float16_weights.resize(align_weight_data_size);
    nread = fread(float16_weights.data(), align_weight_data_size, 1, binfp);

    if (nread != 1) {
      fprintf(stderr, "InnerProduct read float16_weights failed %d\n", nread);
      return -1;
    }

    weight_data = img_t::from_float16(float16_weights.data(), weight_data_size);

    if (weight_data.empty()) {
      return -100;
    }
  }
  else if (flag != 0) {
    // quantized weight data
    float quantization_value[256];
    nread = fread(quantization_value, 256 * sizeof(float), 1, binfp);

    if (nread != 1) {
      fprintf(stderr, "InnerProduct read quantization_value failed %d\n", nread);
      return -1;
    }

    int align_weight_data_size = alignSize(weight_data_size * sizeof(unsigned char), 4);
    std::vector<unsigned char> index_array;
    index_array.resize(align_weight_data_size);
    nread = fread(index_array.data(), align_weight_data_size, 1, binfp);

    if (nread != 1) {
      fprintf(stderr, "InnerProduct read index_array failed %d\n", nread);
      return -1;
    }

    float* weight_data_ptr = weight_data;

    for (int i = 0; i < weight_data_size; i++) {
      weight_data_ptr[i] = quantization_value[ index_array[i] ];
    }
  }
  else if (flag_struct.f0 == 0) {
    // raw weight data
    nread = fread(weight_data, weight_data_size * sizeof(float), 1, binfp);

    if (nread != 1) {
      fprintf(stderr, "InnerProduct read weight_data failed %d\n", nread);
      return -1;
    }
  }

  if (bias_term) {
    bias_data.create(num_output);

    if (bias_data.empty()) {
      return -100;
    }

    nread = fread(bias_data, num_output * sizeof(float), 1, binfp);

    if (nread != 1) {
      fprintf(stderr, "InnerProduct read bias_data failed %d\n", nread);
      return -1;
    }
  }

  return 0;
}
#endif // NCNN_STDIO

int InnerProduct::load_model(const unsigned char*& mem)
{
  union {
    struct {
      unsigned char f0;
      unsigned char f1;
      unsigned char f2;
      unsigned char f3;
    };
    unsigned int tag;
  } flag_struct;

  memcpy(&flag_struct, mem, sizeof(flag_struct));
  mem += sizeof(flag_struct);

  unsigned int flag = flag_struct.f0 + flag_struct.f1 + flag_struct.f2 + flag_struct.f3;

  if (flag_struct.tag == 0x01306B47) {
    // half-precision weight data
    weight_data = img_t::from_float16((unsigned short*)mem, weight_data_size);
    mem += alignSize(weight_data_size * sizeof(unsigned short), 4);

    if (weight_data.empty()) {
      return -100;
    }
  }
  else if (flag != 0) {
    // quantized weight data
    const float* quantization_value = (const float*)mem;
    mem += 256 * sizeof(float);

    const unsigned char* index_array = (const unsigned char*)mem;
    mem += alignSize(weight_data_size * sizeof(unsigned char), 4);

    weight_data.create(weight_data_size);

    if (weight_data.empty()) {
      return -100;
    }

    float* weight_data_ptr = weight_data;

    for (int i = 0; i < weight_data_size; i++) {
      weight_data_ptr[i] = quantization_value[ index_array[i] ];
    }
  }
  else if (flag_struct.f0 == 0) {
    // raw weight data
    weight_data = img_t(weight_data_size, (float*)mem);
    mem += weight_data_size * sizeof(float);
  }

  if (bias_term) {
    bias_data = img_t(num_output, (float*)mem);
    mem += num_output * sizeof(float);
  }

  return 0;
}
#endif
