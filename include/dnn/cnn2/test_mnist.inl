#include "cnn.inl"
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

static int mnist_fetch_data(void* s0, struct Blob* inputBlob, struct Blob* labelBlob, int start, int shuffle_len, const int* shuffle)
{
  int ret = 0;
  struct sample_t* s = (struct sample_t*)s0;
  assert(inputBlob->size.n == labelBlob->size.n);
  if (shuffle_len > 0) {
    int i, j, n = shuffle_len;
    if (inputBlob) {
      inputBlob->size = dataSize(n, s->size.c, s->size.h, s->size.w);
      //scale to 0.0f~1.0f
      {
        const int sizePerImage = Data_3DSize(inputBlob->size);
        float* inputData = setData(inputBlob);
        float scaleRate = 1.0f / 255.0f;
        assert(sizePerImage == Data_3DSize(s->size));
        for (i = 0; i < n; ++i) {
          //image data
          //float* inputData = getData(inputBlob) + i * sizePerImage;
          int k = shuffle ? shuffle[i + start] : (i + start);
          const unsigned char* imageData = s->data + k * sizePerImage;
          for (j = 0; j < sizePerImage; j++) {
            inputData[j] = (float)imageData[j] * scaleRate;
          }
          inputData += sizePerImage;
        }
      }
    }
    if (labelBlob) {
      labelBlob->size = dataSize(n, s->classes, 1, 1);
      {
        float* labelData = setData(labelBlob);
        const int sizePerLabel = Data_3DSize(labelBlob->size);
        for (i = 0; i < n; ++i) {
          //label data
          int k = shuffle ? shuffle[i + start] : (i + start);
          const unsigned char label = s->label[k];
          for (j = 0; j < sizePerLabel; j++) {
            if (j == label) {
              labelData[j] = 1.0f;
            }
            else {
              labelData[j] = 0.0f;
            }
          }
          labelData += sizePerLabel;
        }
      }
    }
    ret = 1;
  }
  return ret;
}
static int load_mnist_images(const char* file_path, DataSize* psize, uint8_t** data)
{
  int ret = 0;
  FILE* ifs = fopen(file_path, "rb");
  if (ifs) {
    //detect platform information
    const int is_little_endian_flag = is_little_endian();
    //magic n
    uint32_t magic_number;
    uint32_t images_total_count = 0;
    int magic_number_validate = 0;
    fread((char*)&magic_number, sizeof(magic_number), 1, ifs);
    if (is_little_endian_flag) {
      magic_number = reverse_endian(magic_number);
    }
    magic_number_validate = (magic_number == 2051);
    assert(magic_number_validate);
    if (magic_number_validate) {
      uint32_t w = 0, h = 0;
      fread((char*)&images_total_count, sizeof(images_total_count), 1, ifs);
      //image property
      fread((char*)&h, sizeof(h), 1, ifs);
      fread((char*)&w, sizeof(w), 1, ifs);
      if (is_little_endian_flag) {
        images_total_count = reverse_endian(images_total_count);
        w = reverse_endian(w);
        h = reverse_endian(h);
      }
      *psize = dataSize(images_total_count, 1, h, w);
      if (data) {
        resetArr(data, Data_total(*psize));
        fread(*data, images_total_count * w * h, 1, ifs);
      }
      ret = 1;
    }
    fclose(ifs);
  }
  return ret;
}
static int load_mnist_labels(const char* file_path, int* pn, uint8_t** data)
{
  int ret = 0;
  FILE* ifs = fopen(file_path, "rb");
  if (ifs) {
    const int is_little_endian_flag = is_little_endian();
    //magic n
    uint32_t magic_number = 0;
    uint32_t labels_total_count = 0;
    int magic_number_validate;
    fread((char*)&magic_number, sizeof(magic_number), 1, ifs);
    if (is_little_endian_flag) {
      magic_number = reverse_endian(magic_number);
    }
    magic_number_validate = (magic_number == 2049);
    assert(magic_number_validate);
    if (magic_number_validate) {
      fread((char*)&labels_total_count, sizeof(labels_total_count), 1, ifs);
      if (is_little_endian_flag) {
        labels_total_count = reverse_endian(labels_total_count);
      }
      *pn = labels_total_count;
      if (data) {
        resetArr(data, labels_total_count);
        fread(*data, 1, labels_total_count, ifs);
      }
      
    }
    ret = 1;
    fclose(ifs);
    
  }
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
  s->classes = 10;
  return ret;
}
static int add_active_layer(struct NetWork* net, int input)
{
  return net_relu(net, input);
}
static int add_conv(struct NetWork* net, int input, int n)
{
  int w, b;
  //return net_conv(net, input, &w, &b, n, 3, 3, 1, 1, M_SAME);
  return net_conv(net, input, &w, &b, n, 5, 5, 1, 1, M_VALID);
}
static int add_pool(struct NetWork* net, int input)
{
  return net_pooling(net, input, M_Max, 2, 2, 2, 2, M_SAME);
}
static int add_fc_layer(struct NetWork* net, int input, int output_count)
{
  int w, b;
  return net_fc(net, input, &w, &b, output_count);
}
static int buildConvNet(struct NetWork* net, DataSize size, int classes, int* pinput)
{
  int t;
  *pinput = net_input(net, size);
  t = add_conv(net, *pinput, 6);
  t = add_active_layer(net, t);
  t = add_pool(net, t);
  t = add_conv(net, t, 12);
  t = add_active_layer(net, t);
  t = add_pool(net, t);
  t = add_fc_layer(net, t, 512);
  t = add_active_layer(net, t);
  //t = net_dropout(net, t, 0.5f);
  t = add_fc_layer(net, t, classes);
  t = net_softmax(net, t);
  return t;
}
static int buildMLPNet(struct NetWork* net, DataSize size, int classes, int* pinput)
{
  int t;
  *pinput = net_input(net, size);
  t = add_fc_layer(net, t, 512);
  t = add_active_layer(net, t);
  t = add_fc_layer(net, t, 256);
  t = add_active_layer(net, t);
  t = add_fc_layer(net, t, classes);
  t = net_softmax(net, t);
  return t;
}

#if 0
static float mnist_param[] = {
#include "D:/code/cnn/mnist/mnist_model.txt"
};
#endif
int test_mnist(int argc, char* argv[])
{
  const char* mnist_train_images_file = "train-images.idx3-ubyte";
  const char* mnist_train_labels_file = "train-labels.idx1-ubyte";
  const char* mnist_test_images_file = "t10k-images.idx3-ubyte";
  const char* mnist_test_labels_file = "t10k-labels.idx1-ubyte";
  const char* model_file = "mnist_model.txt";
  const char* param_file = "mnist_param.dat";
  const char* param_file2 = "mnist_param.dat_0.dat";
  struct NetWork net[1] = {0};
  struct sample_t s[1] = {0};
  struct net_train_test_t t[1] = {0};
  int input, output, label;
  int batch = 128;
  struct test_result_t val;
  sys_chdir("D:/code/cnn/mnist");
  if (0) {
    //printf("%f", data[1]);
    int success = 0;
    setLogLevel(EASYCNN_LOG_LEVEL_CRITICAL);
    //load train images and labels
    logCritical("loading training data...");
    success = load_mnist(s, mnist_train_images_file, mnist_train_labels_file);
    assert(success && s->size.n > 0);
    //train data & validate data
    //train
    net_train_test_init(t, s, mnist_fetch_data, s->size.n, 0.9, batch);
    //validate
    logCritical("c:%d , w:%d , h:%d", s->size.c, s->size.w, s->size.h);
    logCritical("construct net begin...");
    if (1) {
      success = net_load(net, model_file, param_file2);
      assert(success);
      input = find_or_insert_blob(net, "input_0", 0);
      output = find_or_insert_blob(net, "softmax_o_18", 0);
    } else {
      //net_load(net, model_file, param_file);
      output = buildConvNet(net, dataSize(batch, s->size.c, s->size.w, s->size.h), s->classes, &input);
      net_print(net, stdout, 0);
      net_save(net, model_file, NULL, -1);
    }
    //net.setLossFunctor(make_shared<CrossEntropyFunctor>());
    net_loss(net, output, &label, MSEFunctor);
    net_train(t, net, input, output, label, param_file);
    logCritical("construct net done.");
    sample_free(s);
    net_free(net);
  }
  if (1) {
    //test(net, mnist_test_images_file, mnist_test_labels_file, model_file);
    int success = false;
    setLogLevel(EASYCNN_LOG_LEVEL_CRITICAL);
    //load train images
    logCritical("loading test data...");
    success = load_mnist(s, mnist_test_images_file, mnist_test_labels_file);
    assert(success && s->size.n > 0);
    logCritical("load test data done. images' size is %d", s->size.n);
    batch = 64;
    net_train_test_init(t, s, mnist_fetch_data, s->size.n, 0, batch);
    logCritical("c:%d , w:%d , h:%d", s->size.c, s->size.w, s->size.h);
    logCritical("construct net begin...");
    if (1) {
      success = net_load(net, model_file, param_file2);
      assert(success);
      input = find_or_insert_blob(net, "input_0", 0);
      output = find_or_insert_blob(net, "softmax_o_18", 0);
      //net_load(net, model_file, NULL);
    } else {
#if 0
      //net_load(net, model_file, param_file);
      output = buildConvNet(net, dataSize(batch, s->size.c, s->size.w, s->size.h), s->classes, &input);
      net_print(net, stdout, 0);
      net_set_param(net, mnist_param, countof(mnist_param));
#endif
    }
    net_loss(net, output, &label, MSEFunctor);
    logCritical("construct net done.");
    //train
    logCritical("begin test...");
    if (0) {
      int i;
      for (i=0; i<s->size.n; ++i) {
        t->test_start = i;
        t->test_end = i+1;
        val = net_test(t, net, input, output, label, NULL);
        logCritical("accuracy : %.4f%%\n", val.accuracy * 100.0f);
      }
    }
    val = net_test(t, net, input, output, label, NULL);
    logCritical("accuracy : %.4f%%", val.accuracy * 100.0f);
    logCritical("finished test.");
    net_free(net);
  }
  return 0;
}

struct gender_t {
  DataSize size;
  int fsize;
  int datalen;
  int n;
  int classes;
  FILE* pf;
};

static int buildConvNet2(struct NetWork* net, DataSize size, int classes, int* pinput)
{
  int t, w = (size.w-4)/2;
  *pinput = net_input(net, size);
  t = add_conv(net, *pinput, 6);
  t = add_active_layer(net, t);
  t = add_pool(net, t);
  for (; w>4; ) {
    t = add_conv(net, t, 12);
    t = add_active_layer(net, t);
    t = add_pool(net, t);
    w = (w-4)/2;
  }
  t = add_fc_layer(net, t, 512);
  t = add_active_layer(net, t);
  //t = net_dropout(net, t, 0.5f);
  t = add_fc_layer(net, t, classes);
  t = net_softmax(net, t);
  return t;
}
static int gender_fetch_data(void* s0, struct Blob* inputBlob, struct Blob* labelBlob, int start, int shuffle_len, const int* shuffle)
{
  int ret = 0;
  struct gender_t* s = (struct gender_t*)s0;
  assert(inputBlob->size.n == labelBlob->size.n);
  uchar* buf = NULL;
  MYREALLOC(buf, s->datalen);
  if (shuffle_len > 0) {
    int i, j, n = shuffle_len;
    inputBlob->size = dataSize(n, s->size.c, s->size.h, s->size.w);
    labelBlob->size = dataSize(n, s->classes, 1, 1);
    //scale to 0.0f~1.0f
    const int sizePerImage = Data_3DSize(inputBlob->size);
    const int sizePerLabel = Data_3DSize(labelBlob->size);
    float* inputData = setData(inputBlob);
    float* labelData = setData(labelBlob);
    float scaleRate = 1.0f / 255.0f;
    assert(sizePerImage == Data_3DSize(s->size));
    for (i = 0; i < n; ++i) {
      //image data
      //float* inputData = getData(inputBlob) + i * sizePerImage;
      int k = shuffle ? shuffle[i + start] : (i + start);
      fseek(s->pf, k * s->datalen, SEEK_SET);
      fread(buf, s->datalen, 1, s->pf);
      //ShowImagePal("asdf", 64, 64, buf+4, 64, 1, PixFmtMax, NULL);WaitKey(-1);
      if (labelBlob) {
        int label = *(int*)buf;
        for (j = 0; j < sizePerLabel; j++) {
          if (j == label) {
            labelData[j] = 1.0f;
          }
          else {
            labelData[j] = 0.0f;
          }
        }
        labelData += sizePerLabel;
      }
      if (inputBlob) {
        uchar* imageData = buf+4;
        for (j = 0; j < sizePerImage; j++) {
          inputData[j] = (float)imageData[j] * scaleRate;
        }
        inputData += sizePerImage;
      }
    }
    ret = 1;
  }
  FREE(buf);
  return ret;
}
#include "cap.h"
#include "img/imgopt.inl"
static const float gender_model[] = {
#include "D:/code/cnn/gender/gender_model.txt"
};
#include "face/face_recog.inl"
#include "cv1/drawing.inl"
int get_gender(const img_t* img, IRECT rc) {
  struct NetWork net[1] = {0};
  int input;
  DataSize size = dataSize(1, 1, 64, 64);
  int output = buildConvNet2(net, size, 2, &input);
  net_set_param(net, gender_model, countof(gender_model));
  int id = 0;
  img_t im2[1] = {0};
  img_t im[2] = {0};
  img_t *imgray = im;
  imsubref(img, rc, im2);
  im2gry(im2, imgray);
  imresize(imgray, 64, 64, imgray);
  uchar* imageData = imgray->data;
  float scaleRate = 1.0f / 255.0f;
  Blob* inputBlob = BLOB(input);
  Blob* outputBlob = BLOB(output);
  inputBlob->size = size;
  //scale to 0.0f~1.0f
  //imshow_(imgray);WaitKey(-1);
  int j, sizePerImage = Data_3DSize(inputBlob->size);
  float* inputData = setData(inputBlob);
  for (j = 0; j < sizePerImage; j++) {
    inputData[j] = (float)imageData[j] * scaleRate;
  }
  net_forward(net);
  const float* outputData = getData(outputBlob);
  id = outputData[1]>outputData[0];
  net_free(net);
  imfrees2(im);
  return id;
}
int test_gender_cap() {
  const char* model_file = "gender_model.txt";
  const char* param_file = "gender_param.dat";
  const char* param_file2 = "gender_param.dat_4.dat";
  struct NetWork net[1] = {0};
  sys_chdir("D:/code/cnn/gender");
  setLogRedirect(NULL);
  if (1) {
    int h = 1920, w = 1280;
    capdev cap[1] = {0};
    
    if (capdshow_open(cap, 0, w, h) > 0) {
      int fram = 0;
      char ch = 0;
      img_t im[2] = {0};
      
      for (; 'q' != ch && 'Q' != ch; ++fram) {
        cap_getframe(cap, im, CAP_RGB);
        //printf("%d w=%d h=%d\n", fram, im->w, im->h);
        if (1) {
          int xywh[12] = {0};
          int face_cnt = face_detect_defaut( im->h, im->w, im->data, im->s, im->c, "bgr", 0, 0.1, xywh);
          if (face_cnt && xywh[3]>0) {
            IRECT rc = iRECT2(xywh[0], xywh[1], xywh[2], xywh[3]);
            int e = xywh[3]*0.5;
            rc = iRectInflate(rc, e, e, e, e);
            //cvRectangle2(im, *(CRect*)xywh, cScalar(0, 0, 255, 0), 2, 8, 0);
            int gender_id = get_gender(im, rc);
            printf("%d\n", gender_id);
            cvRectangle2(im, cRectRC(rc), gender_id ? cScalar(255, 0, 0, 0) : cScalar(0, 0, 255, 0), 2, 8, 0);
          }
        }
        imshow_(im);
        ch = WaitKey(10);
      }
      
      cap_close(cap);
      imfrees2(im);
    }
  }
  return 0;
}
int test_gender_train()
{
  return test_gender_cap();
  const char* data_file = "gender_1x64x64_copy.dat";
  const char* model_file = "gender_model.txt";
  const char* param_file = "gender_param.dat";
  const char* param_file2 = "gender_param.dat_4.dat";
  struct NetWork net[1] = {0};
  struct net_train_test_t t[1] = {0};
  int input, output, label;
  int batch = 64;
  gender_t s[1] = {0};
  struct test_result_t val;
  sys_chdir("D:/code/cnn/gender");
  s->fsize = filesize(data_file);
  s->datalen = 1*64*64 + 4;
  s->pf = fopen(data_file, "rb");
  s->classes = 2;
  s->size = dataSize(s->fsize/s->datalen, 1, 64, 64);
  ASSERT(s->pf);
  if (0) {
    int i=0, err=0;
    uchar* buf = NULL;
    MYREALLOC(buf, s->datalen);
    setLogRedirect(NULL);
    for (i=0; i<s->size.n; ++i) {
      fread(buf, s->datalen, 1, s->pf);
      img_t im[1] = {0};
      int id = *(int*)buf;
      IMINIT(im, 64, 64, buf+4, 64, 1, 1);
      IRECT rc = iRECT2(0, 0, 64, 64);
      int id2 = get_gender(im, rc);
      err += (id!=id2);
      printf("id=%d id2=%d err %d/%d, rate %3.1f\n", id, id2, err, i+1, err*100./(i+1));
      //imshow_(im); WaitKey(-1);
    }
    FREE(buf);
  }
  if (0) {
    //printf("%f", data[1]);
    int success = 0;
    setLogLevel(EASYCNN_LOG_LEVEL_CRITICAL);
    //load train images and labels
    logCritical("loading training data...");
    assert(s->size.n > 0);
    //train data & validate data
    //train
    net_train_test_init(t, s, gender_fetch_data, s->size.n, 0.9, batch);
    //validate
    logCritical("c:%d , w:%d , h:%d", s->size.c, s->size.w, s->size.h);
    logCritical("construct net begin...");
    if (1) {
      success = net_load(net, model_file, param_file2);
      input = find_or_insert_blob(net, "input_0", 0);
      output = find_or_insert_blob(net, "softmax_o_23", 0);
    } else {
      //net_load(net, model_file, param_file);
      output = buildConvNet2(net, dataSize(batch, s->size.c, s->size.w, s->size.h), s->classes, &input);
      net_print(net, stdout, 0);
      net_save(net, model_file, NULL, -1);
    }
    //net.setLossFunctor(make_shared<CrossEntropyFunctor>());
    net_loss(net, output, &label, MSEFunctor);
    net_train(t, net, input, output, label, param_file);
    logCritical("construct net done.");
    fclose(s->pf);
    net_free(net);
  }
  if (0) {
    //test(net, mnist_test_images_file, mnist_test_labels_file, model_file);
    int success = 1;
    setLogLevel(EASYCNN_LOG_LEVEL_CRITICAL);
    //load train images
    logCritical("loading test data...");
    assert(success && s->size.n > 0);
    logCritical("load test data done. images' size is %d", s->size.n);
    batch = 64;
    net_train_test_init(t, s, gender_fetch_data, s->size.n, 0, batch);
    logCritical("c:%d , w:%d , h:%d", s->size.c, s->size.w, s->size.h);
    logCritical("construct net begin...");
    if (0) {
      success = net_load(net, model_file, param_file2);
      assert(success);
      input = find_or_insert_blob(net, "input_0", 0);
      output = find_or_insert_blob(net, "softmax_o_23", 0);
      //net_load(net, model_file, NULL);
      //net_save(net, model_file, NULL, 0);
    } else {
#if 1
      //net_load(net, model_file, param_file);
      output = buildConvNet2(net, dataSize(batch, s->size.c, s->size.w, s->size.h), s->classes, &input);
      net_print(net, stdout, 0);
      net_set_param(net, gender_model, countof(gender_model));
#endif
    }
    net_loss(net, output, &label, MSEFunctor);
    logCritical("construct net done.");
    //train
    logCritical("begin test...");
    if (0) {
      int i;
      for (i=0; i<s->size.n; ++i) {
        t->test_start = i;
        t->test_end = i+1;
        val = net_test(t, net, input, output, label, NULL);
        logCritical("accuracy : %.4f%%\n", val.accuracy * 100.0f);
      }
    }
    t->test_end = s->size.n;
    t->test_start = s->size.n-1000;
    val = net_test(t, net, input, output, label, NULL);
    logCritical("accuracy : %.4f%%", val.accuracy * 100.0f);
    logCritical("finished test.");
    net_free(net);
  }
  return 0;
}
