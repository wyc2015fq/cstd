#include "cnn.h"
#include "test_cnn.inl"
//#include "test_linear_regression.inl"
#define reverse_endian(p)  (revBytes((char*)(&p), sizeof(p)), p)
static void revBytes(char* f, int len)
{
  char* l = f+len;
  for (--l; f <= l; ++f, --l) {
    char a = *f;
    char b = *l;
    *f = b;
    *l = a;
  }
}
CC_INLINE int is_little_endian()
{
  int x = 1;
  return *(char*)&x != 0;
}
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;
struct sample_t {
  DataSize size;
  unsigned char* data;
  unsigned char* label;
  int classes;
};
static int sample_free(struct sample_t* s)
{
  if (s->data) {
    free(s->data);
  }
  if (s->label) {
    free(s->label);
  }
  setzero(s);
  return 0;
}

static int load_mnist_images(const char* file_path, DataSize* psize, uint8_t** data)
{
  int ret = 0;
  FILE* ifs_images = fopen(file_path, "rb");
  if (ifs_images) {
    //detect platform information
    const int is_little_endian_flag = is_little_endian();
    //magic n
    uint32_t magic_number;
    uint32_t images_total_count = 0;
    int magic_number_validate = 0;
    fread((char*)&magic_number, sizeof(magic_number), 1, ifs_images);
    if (is_little_endian_flag) {
      magic_number = reverse_endian(magic_number);
    }
    magic_number_validate = (magic_number == 2051);
    assert(magic_number_validate);
    if (magic_number_validate) {
      uint32_t w = 0, h = 0;
      fread((char*)&images_total_count, sizeof(images_total_count), 1, ifs_images);
      //image property
      fread((char*)&h, sizeof(h), 1, ifs_images);
      fread((char*)&w, sizeof(w), 1, ifs_images);
      if (is_little_endian_flag) {
        images_total_count = reverse_endian(images_total_count);
        w = reverse_endian(w);
        h = reverse_endian(h);
      }
      *psize = dataSize(images_total_count, 1, h, w);
      if (data) {
        resetArr(data, Data_total(*psize));
        fread(*data, images_total_count * w * h, 1, ifs_images);
      }
      ret = 1;
    }
    fclose(ifs_images);
  }
  return ret;
}
static int load_mnist_labels(const char* file_path, int* pn, uint8_t** data)
{
  int ret = 0;
  FILE* ifs_labels = fopen(file_path, "rb");
  if (!ifs_labels) {
    return 0;
  }
  int do_shuffle = 0;
  const int is_little_endian_flag = is_little_endian();
  //magic n
  uint32_t magic_number = 0;
  uint32_t labels_total_count = 0;
  int magic_number_validate;
  
  fread((char*)&magic_number, sizeof(magic_number), 1, ifs_labels);
  if (is_little_endian_flag) {
    magic_number = reverse_endian(magic_number);
  }
  magic_number_validate = (magic_number == 2049);
  assert(magic_number_validate);
  
  if (magic_number_validate) {
    fread((char*)&labels_total_count, sizeof(labels_total_count), 1, ifs_labels);
    if (is_little_endian_flag) {
      labels_total_count = reverse_endian(labels_total_count);
    }
    *pn = labels_total_count;
    if (data) {
      resetArr(data, labels_total_count);
      fread(*data, 1, labels_total_count, ifs_labels);
    }
  }
  ret = 1;

  fclose(ifs_labels);
  return ret;
}
static int load_mnist(struct sample_t* s, const char* images_file, const char* labels_file)
{
  int n = 0;
  int ret = load_mnist_labels(labels_file, &n, &s->label) && load_mnist_images(images_file, &s->size, &s->data);
  if (s->size.n != n) {
    sample_free(s);
    ret = 0;
  }
  if (1) {
    int* shuffle = NULL;
    int* arr = resetArr(&shuffle, n);
    init_perm(n, shuffle);
    random_shuffle(n, shuffle);
    mem_shuffle(n, s->data, s->size.h*s->size.w, shuffle);
    mem_shuffle(n, s->label, 1, shuffle);
    FREE(arr);
  }
  s->classes = 10;
  return ret;
}
static int buildConvNet(struct Net* net, DataSize size, int classes, int* pinput)
{
  int t = *pinput = net_input(net, size);
  t = add_conv(net, t, 6, 5);
  t = add_active_layer(net, t);
  t = add_pool(net, t);
  t = add_conv(net, t, 12, 5);
  t = net_bn(net, t);
  t = net_scale(net, t);
  t = add_active_layer(net, t);
  t = add_pool(net, t);
  t = add_fc_layer(net, t, 512);
  t = add_active_layer(net, t);
  //t = net_dropout(net, t, 0.5f);
  t = add_fc_layer(net, t, classes);
  t = net_softmax(net, t);
  return t;
}
static int buildMLPNet(struct Net* net, DataSize size, int classes, int* pinput)
{
  int t = *pinput = net_input(net, size);
  t = add_fc_layer(net, t, 512);
  t = add_active_layer(net, t);
  t = add_fc_layer(net, t, 256);
  t = add_active_layer(net, t);
  t = add_fc_layer(net, t, classes);
  t = net_softmax(net, t);
  return t;
}
static int buildFCNNet(struct Net* net, DataSize size, int classes, int* pinput)
{
  int t = *pinput = net_input(net, size);
  for (; net->blobs[t].size.w>4; ) {
    t = add_conv(net, t, 10, 5);
    t = add_active_layer(net, t);
    t = add_pool(net, t);
  }
  t = add_fc_layer(net, t, classes);
  t = net_softmax(net, t);
  //t = net_dropout(net, t, 0.5f);
  return t;
}
#if 0
static float mnist_param[] = {
#include "D:/code/cnn/mnist/mnist_model.txt"
};
#endif
static int mnist_fetch_data(void* s0, struct Blob* inputBlob, struct Blob* labelBlob, int start, int shuffle_len, bool onehot)
{
  int ret = 0;
  struct sample_t* s = (struct sample_t*)s0;
  if (labelBlob && inputBlob) {
    assert(inputBlob->size.n == labelBlob->size.n);
  }
  if (shuffle_len > 0) {
    int n = shuffle_len;
    if (inputBlob) {
      inputBlob->size = dataSize(n, s->size.c, s->size.h, s->size.w);
      //scale to 0.0f~1.0f
      const int sizePerImage = Data_3DSize(inputBlob->size);
      float* inputData = setData(inputBlob);
      float scaleRate = 1.0f / 255.0f;
      assert(sizePerImage == Data_3DSize(s->size));
      image2float(inputData, inputBlob->size, NULL, s->data + start * sizePerImage, s->size.h, s->size.w, s->size.w, s->size.c, scaleRate, 0);
    }
    if (labelBlob) {
      Blob_init_label_u8(labelBlob, n, s->classes, s->label+start, onehot);
    }
    ret = 1;
  }
  return ret;
}
static int buildConvLrnNet(struct Net* net, DataSize size, int classes, int* pinput)
{
  int t = *pinput = net_input(net, size);
  t = add_conv(net, t, 6, 5);
  t = add_active_layer(net, t);
  t = add_pool(net, t);
  //t = net_lrn(net, t);
  t = add_conv(net, t, 12, 5);
  //t = net_bn(net, t);
  //t = net_scale(net, t);
  t = add_active_layer(net, t);
  t = add_pool(net, t);
  //t = net_lrn(net, t);
  t = net_dropout(net, t, 0.5f);
  t = add_fc_layer(net, t, 512);
  t = add_active_layer(net, t);
  t = net_dropout(net, t, 0.5f);
  t = add_fc_layer(net, t, classes);
  t = net_softmax(net, t);
  return t;
}

static int mnist_net(struct Net* net, int input, int label, int classes)
{
  int t = input;
  if (1) {
    t = add_conv(net, t, 6, 5);
    t = add_active_layer(net, t);
    t = add_pool(net, t);
    //t = net_lrn(net, t);
    t = add_conv(net, t, 12, 5);
    //t = net_bn(net, t);
    //t = net_scale(net, t);
    t = add_active_layer(net, t);
    t = add_pool(net, t);
    //t = net_lrn(net, t);
    //t = net_dropout(net, t, 0.5f);
    t = add_fc_layer(net, t, 512);
    t = add_active_layer(net, t);
    //t = net_dropout(net, t, 0.5f);
    t = add_fc_layer(net, t, classes);
    t = net_softmax(net, t);
  }
  if (0) {
    int a[2];
    int i;
    for (i=0; i<2; ++i) {
      a[0] = add_conv_same(net, t, 5, 1);
      a[1] = add_conv_same(net, t, 5, 5);
      t = net_concat(net, 2, a, 1);
      t = add_active_layer(net, t);
      t = add_pool(net, t);
    }
    t = add_fc_layer(net, t, classes);
    t = net_softmax(net, t);
  }
  if (0) {
    int i;
    for (i=0; i<2; ++i) {
      t = add_conv_same(net, t, 5, 1);
      t = add_active_layer(net, t);
      t = add_pool(net, t);
    }
    t = add_fc_layer(net, t, 20);
    if (1) {
      int c;
      net_centerloss(net, t, label, &c, classes, 1.f);
    }
    t = add_active_layer(net, t);
    t = add_fc_layer(net, t, classes);
    t = net_softmax(net, t);
  }
  net_loss(net, t, label, CrossEntropyLoss);
  return t;
}
#include "test_autoencoder.inl"
int test_mnist() {CPCODETO("D:/code/end_code/rywfacedetect");
  const char* mnist_train_images_file = "train-images.idx3-ubyte";
  const char* mnist_train_labels_file = "train-labels.idx1-ubyte";
  const char* mnist_test_images_file = "t10k-images.idx3-ubyte";
  const char* mnist_test_labels_file = "t10k-labels.idx1-ubyte";
  const char* model_file = "mnist_model.txt";
  const char* param_file = "mnist_param.dat";
  const char* param_file2 = "mnist_param.dat_0.dat";
  struct Net net[1] = {0};
  struct sample_t s[1] = {0};
  struct net_train_test_t t[1] = {0};
  int input, output, label;
  int batch = 64, n_train;
  struct test_result_t val;
  setLogRedirect(defaultLogRoute);
  sys_chdir("D:/code/cnn/mnist");
  if (1) {
    //printf("%f", data[1]);
    int success = 0;
    setLogLevel(EASYCNN_LOG_LEVEL_CRITICAL);
    //load train images and labels
    logCritical("loading training data...");
    success = load_mnist(s, mnist_train_images_file, mnist_train_labels_file);
    assert(success && s->size.n > 0);
    //train data & validate data
    //train
    n_train = s->size.n * 0.9;
    net_train_test_init(t, s, mnist_fetch_data, n_train, s->size.n - n_train, batch);
    //validate
    logCritical("c:%d , w:%d , h:%d", s->size.c, s->size.w, s->size.h);
    logCritical("construct net begin...");
    input = net_addBlob(net, dataSize(batch, s->size.c, s->size.w, s->size.h), "data", "0");
    label = net_addBlob(net, dataSize(batch, t->onehot ? s->classes : 1, 1, 1), "label", "0");
    if (0 && file_exist(model_file) && file_exist(param_file2)) {
      success = net_load(net, model_file, param_file2);
      assert(success);
      input = find_or_insert_blob(net, "input", 0);
      output = find_or_insert_blob(net, "softmax_o", 0);
    } else {
      //net_load(net, model_file, param_file);
      output = mnist_net(net, input, label, s->classes);
      //output = buildFCNNet(net, dataSize(batch, s->size.c, s->size.w, s->size.h), s->classes, &input);
      net_save(net, model_file, NULL, -1);
    }
    net_print(net, stdout, 0);
    //net_loss(net, output, &label, MSELoss);
    net_train(t, net, input, output, label, model_file, param_file);
    logCritical("construct net done.");
    sample_free(s);
    net_free(net);
  }
  if (0) {
    //test(net, mnist_test_images_file, mnist_test_labels_file, model_file);
    int success = false;
    setLogLevel(EASYCNN_LOG_LEVEL_CRITICAL);
    //load train images
    logCritical("loading test data...");
    success = load_mnist(s, mnist_test_images_file, mnist_test_labels_file);
    assert(success && s->size.n > 0);
    logCritical("load test data done. images' size is %d", s->size.n);
    batch = 64;
    n_train = s->size.n * 0.9;
    net_train_test_init(t, s, mnist_fetch_data, n_train, s->size.n - n_train, batch);
    logCritical("c:%d , w:%d , h:%d", s->size.c, s->size.w, s->size.h);
    logCritical("construct net begin...");
    if (1) {
      success = net_load(net, model_file, param_file2);
      assert(success);
      input = find_or_insert_blob(net, "input", 0);
      output = find_or_insert_blob(net, "softmax_o", 0);
      //net_load(net, model_file, NULL);
    } else {
#if 0
      //net_load(net, model_file, param_file);
      output = buildConvNet(net, dataSize(batch, s->size.c, s->size.w, s->size.h), s->classes, &input);
      net_print(net, stdout, 0);
      net_set_param(net, mnist_param, countof(mnist_param));
#endif
    }
    logCritical("construct net done.");
    //train
    logCritical("begin test...");
    if (0) {
      int i;
      for (i=0; i<s->size.n; ++i) {
        t->n_train = i;
        t->n_test = 1;
        val = net_test(t, net, input, output, label);
        logCritical("accuracy : %.4f%%\n", val.accuracy * 100.0f);
      }
    }
    val = net_test(t, net, input, output, label);
    logCritical("accuracy : %.4f%%", val.accuracy * 100.0f);
    logCritical("finished test.");
    net_free(net);
  }
  return 0;
}
