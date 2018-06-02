
struct Embed : public Layer
{
public:
  Embed();
  virtual ~Embed();

  virtual int load_param(const ParamDict& pd);

#if NCNN_STDIO
  virtual int load_model(FILE* binfp);
#endif // NCNN_STDIO
  virtual int load_model(const unsigned char*& mem);

  virtual int forward(const Blob* prev, Blob* next) const;

public:
  // param
  int num_output;
  int input_dim;
  int bias_term;

  int weight_data_size;

  // model
  Blob weight_data;
  Blob bias_data;
};

Embed::Embed()
{
  one_blob_only = true;
  support_inplace = false;
}

Embed::~Embed()
{
}

int Embed::load_param(const ParamDict& pd)
{
  num_output = pd.get(0, 0);
  input_dim = pd.get(1, 0);
  bias_term = pd.get(2, 0);
  weight_data_size = pd.get(3, 0);

  return 0;
}

#if NCNN_STDIO
int Embed::load_model(FILE* binfp)
{
  int nread;

  struct {
    unsigned char f0;
    unsigned char f1;
    unsigned char f2;
    unsigned char f3;
  } flag_struct;

  nread = fread(&flag_struct, sizeof(flag_struct), 1, binfp);

  if (nread != 1) {
    fprintf(stderr, "Embed read flag_struct failed %d\n", nread);
    return -1;
  }

  unsigned int flag = flag_struct.f0 + flag_struct.f1 + flag_struct.f2 + flag_struct.f3;

  weight_data.create(weight_data_size);

  if (weight_data.empty()) {
    return -100;
  }

  if (flag != 0) {
    // quantized weight data
    float quantization_value[256];
    nread = fread(quantization_value, 256 * sizeof(float), 1, binfp);

    if (nread != 1) {
      fprintf(stderr, "Embed read quantization_value failed %d\n", nread);
      return -1;
    }

    std::vector<unsigned char> index_array;
    index_array.resize(weight_data_size);
    nread = fread(index_array.data(), weight_data_size * sizeof(unsigned char), 1, binfp);

    if (nread != 1) {
      fprintf(stderr, "Embed read index_array failed %d\n", nread);
      return -1;
    }

    float* weight_data_ptr = weight_data;

    for (i = 0; i < weight_data_size; i++) {
      weight_data_ptr[i] = quantization_value[ index_array[i] ];
    }
  }
  else if (flag_struct.f0 == 0) {
    // raw weight data
    nread = fread(weight_data, weight_data_size * sizeof(float), 1, binfp);

    if (nread != 1) {
      fprintf(stderr, "Embed read weight_data failed %d\n", nread);
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
      fprintf(stderr, "Embed read bias_data failed %d\n", nread);
      return -1;
    }
  }

  return 0;
}
#endif // NCNN_STDIO

int Embed::load_model(const unsigned char*& mem)
{
  struct {
    unsigned char f0;
    unsigned char f1;
    unsigned char f2;
    unsigned char f3;
  } flag_struct;

  memcpy(&flag_struct, mem, sizeof(flag_struct));
  mem += sizeof(flag_struct);

  unsigned int flag = flag_struct.f0 + flag_struct.f1 + flag_struct.f2 + flag_struct.f3;

  if (flag != 0) {
    // quantized weight data
    const float* quantization_value = (const float*)mem;
    mem += 256 * sizeof(float);

    const unsigned char* index_array = (const unsigned char*)mem;
    mem += weight_data_size * sizeof(unsigned char);

    weight_data.create(weight_data_size);

    if (weight_data.empty()) {
      return -100;
    }

    float* weight_data_ptr = weight_data;

    for (i = 0; i < weight_data_size; i++) {
      weight_data_ptr[i] = quantization_value[ index_array[i] ];
    }
  }
  else if (flag_struct.f0 == 0) {
    // raw weight data
    weight_data = Blob(weight_data_size, (float*)mem);
    mem += weight_data_size * sizeof(float);
  }

  if (bias_term) {
    bias_data = Blob(num_output, (float*)mem);
    mem += num_output * sizeof(float);
  }

  return 0;
}

int Embed::forward(const Blob* prev, Blob* next) const
{
  int words = prev->total();

  next->create(num_output, words, 1);

  if (next->empty()) {
    return -100;
  }

  // num_output
  const float* word_ptr = prev;
  const float* dict_ptr = weight_data;
  #pragma omp parallel for

  for (q = 0; q < words; q++) {
    float* outptr = nextData + next->w * q;

    int word_index = (int)word_ptr[q];

    // check word_index >= 0 && word_index < input_dim

    const float* em = dict_ptr + num_output * word_index;

    memcpy(outptr, em, num_output * sizeof(float));

    if (bias_term) {
      for (p = 0; p < num_output; p++) {
        outptr[p] += bias_data.data[p];
      }
    }
  }

  return 0;
}

} // namespace ncnn
