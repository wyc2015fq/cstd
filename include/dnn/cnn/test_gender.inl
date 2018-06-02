
#include "cstd.h"
//#include "test_mnist.inl"
#include "img/imgio.inl"
#include "agegender.h"
//#include "util/db_lmdb.inl"
//#include "tools/convert_imageset.inl"

int str_to_label(const char* str, const char** strs, int len, int delims) {
  int i;
  for (i=0; i<len; ++i) {
    const char* p = str;
    const char const * s = strs[i];
    int l = strlen(s);
    for (;*p;++p) {
      if (delims==*p && delims==p[l+1] && 0==memicmp(p+1, s, l)) {
        return i;
      }
    }
  }
    printf("error : %s unkown label\n", str);
  if (1) {
    int asdf=0;
  }
  return -1;
}
//#include "math/rand.inl"
#define SHUFFLE(r, array, arrlen, t) { \
  int m = arrlen; \
  while (m) { \
    int i = rng_int32(r) % m--; \
    CC_SWAP(array[m], array[i], t); \
  } \
}


int test_gender() {
  const char* list_file = "list.txt";
  char out_file[256] = "";
  FILE* pout;
  vstr_t sv[1] = {0};
  int i;
  img_t im[1] = {0};
  DataSize size;
  int delims = '\\';
  str_t t;
  sys_chdir("C:/code/cnn/gender");
  vstr_load(list_file, sv);
  SHUFFLE(NULL, sv->v, sv->n, t);
  if (0) {
    for (i=0; i<sv->n; ++i) {
      int gender_id = str_to_label(sv->v[i].s, gender_tag, countof(gender_tag), delims);
      int age_id = str_to_label(sv->v[i].s, age_tag, countof(age_tag), delims);
    }
  }
  size = dataSize(sv->n, 1, 128, 128);
  _snprintf(out_file, 256, "gender_age_%dx%dx%d.dat", size.c, size.h, size.w);
  pout = fopen(out_file, "wb");
  if (pout) {
    //fwrite(&size, sizeof(size), 1, pout);
    for (i=0; i<sv->n; ++i) {
      int gender_id = str_to_label(sv->v[i].s, gender_tag, countof(gender_tag), delims);
      int age_id = str_to_label(sv->v[i].s, age_tag, countof(age_tag), delims);
      if (gender_id>=0 && imread(sv->v[i].s, size.c, 1, im)) {
        int bytesize = Data_3DSize(size);
        imresize(im, size.h, size.w, im);
        fwrite(&gender_id, sizeof(gender_id), 1, pout);
        fwrite(&age_id, sizeof(age_id), 1, pout);
        fwrite(im->data, bytesize, 1, pout);
        printf("%7d/%-7d %4.1f%% %10s %14s\n", i, sv->n, i*100./sv->n, gender_tag[gender_id], age_tag[age_id]);
        //imshow_(im);WaitKey(-1);
      }
    }
    fclose(pout);
  }
  imfree(im);
  vstr_free(sv);
  return 0;
}

//#include "mtcnn/mtcnn.inl"
#include "facelib/facedetect/facedetect.inl"
#include "face/face_recog.inl"

// 生成归一化人脸数据文件
int test_gender_std() {
  const char* list_file = "list.txt";
  char out_file[256] = "";
  FILE* pout;
  vstr_t sv[1] = {0};
  int i;
  img_t im[2] = {0};
  img_t* im_std = im+1;
  DataSize size;
  int delims = '\\';
  str_t t;
  sys_chdir("C:/code/cnn/gender");
  vstr_load(list_file, sv);
  SHUFFLE(NULL, sv->v, sv->n, t);
  if (0) {
    for (i=0; i<sv->n; ++i) {
      int gender_id = str_to_label(sv->v[i].s, gender_tag, countof(gender_tag), delims);
      int age_id = str_to_label(sv->v[i].s, age_tag, countof(age_tag), delims);
    }
  }
  size = dataSize(sv->n, 1, 128, 128);
  imsetsize(im_std, size.h, size.w, 1, 1 );
  _snprintf(out_file, 256, "gender_age_std_%dx%dx%d.dat", size.c, size.h, size.w);
  pout = fopen(out_file, "wb");
  if (pout) {
    int cnt=0, cntdet=0;
    //fwrite(&size, sizeof(size), 1, pout);
    for (i=0; i<sv->n; ++i) {
      int gender_id = str_to_label(sv->v[i].s, gender_tag, countof(gender_tag), delims);
      int age_id = str_to_label(sv->v[i].s, age_tag, countof(age_tag), delims);
      if (gender_id>=0 && imread(sv->v[i].s, 3, 1, im)) {
        Bbox box[200];
        int nfaces = mtcnn_findFace(im, box, countof(box), 40);
        if (nfaces>0) {
          float eav[3] = {0};
          float* p = box[0].ppoint;
          EstimateHeadPose(p, eav);
          ++cntdet;
          if (fabs(eav[0])<20 && fabs(eav[1])<20) {
            double a = 0.3;
            im2gry(im, im);
            //ShowImagePal("imrgb", im->h, im->w, im->data, im->s, im->c, PixFmtMax, NULL);
            CPM_FaceStd(im->h, im->w, im->data, im->s, im->c,
              im_std->h, im_std->w, im_std->data, im_std->s, im_std->c,
              iPOINT(p[0], p[5]),iPOINT(p[1], p[6]), a, 0.4, 1-a, 0.4);
            //imshow_(im);            imshow_(im_std);            WaitKey(1);
            imresize(im, size.h, size.w, im);
            fwrite(&gender_id, sizeof(gender_id), 1, pout);
            fwrite(&age_id, sizeof(age_id), 1, pout);
            fwrite(im_std->data, im_std->h*im_std->s, 1, pout);
            ++cnt;
            printf("%7d/%7d/%7d/%-7d %4.1f%% %10s %14s\n", cnt, cntdet, i, sv->n, i*100./sv->n, gender_tag[gender_id], age_tag[age_id]);
            //imshow_(im);WaitKey(-1);
          }
        }
      }
    }
    fclose(pout);
  }
  imfrees2(im);
  vstr_free(sv);
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

// 数据读取回调
static int gender_fetch_data(void* s0, struct Blob* inputBlob, struct Blob* labelBlob, int start, int shuffle_len, bool onehot)
{
  int ret = 0;
  struct gender_t* s = (struct gender_t*)s0;
  uchar* buf = NULL;
  assert(inputBlob->size.n == labelBlob->size.n);
  MYREALLOC(buf, s->datalen);
  if (shuffle_len > 0) {
    int i, j, n = shuffle_len, gender_id, age_id;
    float* inputData = setDataSize(inputBlob, dataSize(n, s->size.c, s->size.h, s->size.w));
    float* labelData = setDataSize(labelBlob, dataSize(n, onehot ? s->classes : 1, 1, 1));
    //scale to 0.0f~1.0f
    const int sizePerImage = Data_3DSize(inputBlob->size);
    const int sizePerLabel = Data_3DSize(labelBlob->size);
    float scaleRate = 1.0f / 255.0f;
    assert(sizePerImage == Data_3DSize(s->size));
    for (i = 0; i < n; ++i) {
      //image data
      //float* inputData = getData(inputBlob) + i * sizePerImage;
      int k = (i + start);
      fseek(s->pf, k * s->datalen, SEEK_SET);
      fread(buf, s->datalen, 1, s->pf);
      gender_id = *(int*)buf;
      age_id = *(int*)(buf+4);
      //printf("%10s %10s err %d/%d, rate %3.1f\n", gender_tag[gender_id], age_tag[age_id]);
      //ShowImagePal("asdf", 64, 64, buf+8, 64, 1, PixFmtMax, NULL);WaitKey(-1);
      if (labelBlob) {
        if (onehot) {
          onehot_f32(sizePerLabel, labelData, gender_id);
          labelData += sizePerLabel;
        } else {
          ((int*)labelData)[i] = gender_id;
        }
      }
      if (inputBlob) {
        uchar* imageData = buf+8;
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
// 构建网络
static int buildConvNet2(struct Net* net, DataSize size, int classes, int* pinput, int* plable = NULL)
{
  int i=0, t, w = size.w;
  t = *pinput = net_input(net, size);
  if (plable) {
    *plable = net_addBlob(net, dataSize(size.n, classes, 1, 1), "", "lable");
  }
  t = add_conv(net, t, 16, 5);
  t = add_active_layer(net, t);
  t = add_pool(net, t);
  w -= 4;
  w /= 2;
  for (i=0; i<2; ++i) {
    //t = add_conv_same(net, t, 12, 5);
    //t = add_conv(net, t, i<3 ? ker[i] : 16);
    t = add_conv(net, t, 16, 3);
    //t = net_bn(net, t);
    //t = net_scale(net, t);
    t = add_active_layer(net, t);
    t = add_pool(net, t);
    w = (w-2)/2;
  }
  t = net_dropout(net, t, 0.5f);
  t = add_fc_layer(net, t, 128);
  t = add_active_layer(net, t);
  t = net_dropout(net, t, 0.5f);
  t = add_fc_layer(net, t, classes);
  t = net_softmax(net, t);
  return t;
}

#include "cap.h"
#include "img/imgopt.inl"
//#include "face/face_recog.inl"
#include "cv/imgproc/drawing.inl"
// 识别函数
int get_gender(const img_t* img, IRECT rc) {
  struct Net net[1] = {0};
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
// 在视频中测试
int test_gender_cap() {
  const char* model_file = "gender_model.txt";
  const char* param_file = "gender_param.dat";
  const char* param_file2 = "gender_param.dat_4.dat";
  struct Net net[1] = {0};
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
#if 0
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
#endif
        imshow_(im);
        ch = WaitKey(10);
      }
      
      cap_close(cap);
      imfrees2(im);
    }
  }
  return 0;
}
// 训练性别分类器
int test_gender_train()
{
  g_logfile = "log.txt";
  //return test_gender_cap();
#if 0
  const char* data_file = "gender_age_1x64x64.dat";
  const char* model_file = "gender_model.txt";
  const char* param_file = "gender_param.dat";
  const char* param_file2 = "gender_param.dat_4.dat";
#endif
#if 1
  const char* data_file = "gender_age_std_1x64x64.dat";
  const char* model_file = "gender_std_model.txt";
  const char* param_file = "gender_std_param.dat";
  const char* param_file2 = "gender_std_param.dat_6.dat";
#endif
  int c=1, h=64, w=64;
  struct Net net[1] = {0};
  struct net_train_test_t t[1] = {0};
  int input, output, label;
  int batch = 64;
  gender_t s[1] = {0};
  struct test_result_t val;
  sys_chdir("C:/code/cnn/gender");
  s->fsize = filesize(data_file);
  s->datalen = 8 + c*h*w;
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
  if (1) {
    //printf("%f", data[1]);
    int success = 0;
    setLogLevel(EASYCNN_LOG_LEVEL_CRITICAL);
    //load train images and labels
    logCritical("loading training data...");
    assert(s->size.n > 0);
    //train data & validate data
    //train
    int n_test=MIN(5000, s->size.n/10);
    net_train_test_init(t, s, gender_fetch_data, s->size.n-n_test, n_test, batch);
    //validate
    logCritical("c:%d , w:%d , h:%d", s->size.c, s->size.w, s->size.h);
    logCritical("construct net begin...");
    if (file_exist(model_file) && file_exist(param_file2)) {
      success = net_load(net, model_file, param_file2);
      input = find_or_insert_blob(net, "input", 0);
      output = find_or_insert_blob(net, "softmax_o", 0);
    } else {
      //net_load(net, model_file, param_file);
      output = buildConvNet2(net, dataSize(batch, s->size.c, s->size.w, s->size.h), s->classes, &input, &label);
      net_print(net, stdout, 0);
      net_save(net, model_file, NULL, -1);
    }
    net_loss(net, output, label, CrossEntropyLoss);
    net_train(t, net, input, output, label, model_file, param_file);
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
    net_train_test_init(t, s, gender_fetch_data, 0, s->size.n, batch);
    logCritical("c:%d , w:%d , h:%d", s->size.c, s->size.w, s->size.h);
    logCritical("construct net begin...");
    if (0) {
      success = net_load(net, model_file, param_file2);
      assert(success);
      input = find_or_insert_blob(net, "input", 0);
      output = find_or_insert_blob(net, "softmax_o", 0);
      //net_load(net, model_file, NULL);
      //net_save(net, model_file, NULL, 0);
    } else {
#if 0
      //net_load(net, model_file, param_file);
      output = buildConvNet2(net, dataSize(batch, s->size.c, s->size.w, s->size.h), s->classes, &input);
      net_print(net, stdout, 0);
      net_set_param(net, gender_model, countof(gender_model));
#endif
    }
    net_loss(net, output, label, MSELoss);
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
    t->n_train = s->size.n-1000;
    t->n_test = 1000;
    val = net_test(t, net, input, output, label);
    logCritical("accuracy : %.4f%%", val.accuracy * 100.0f);
    logCritical("finished test.");
    net_free(net);
  }
  return 0;
}

// 读取年龄数据回调
static int age_fetch_data(void* s0, struct Blob* inputBlob, struct Blob* labelBlob, int start, int shuffle_len, bool onehot)
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
      int k = (i + start);
      fseek(s->pf, k * s->datalen, SEEK_SET);
      fread(buf, s->datalen, 1, s->pf);
      int gender_id = *(int*)(buf);
      int age_id = *(int*)(buf+4);
      if (labelBlob) {
        float* ptr = labelData + i*sizePerLabel;
        if (s->classes>1) {
          if (age_id>=s->classes) {
            printf("age_id>=s->classes\n");
          }
          if (onehot) {
            onehot_f32(sizePerLabel, ptr, age_id);
          } else {
            ((int*)labelData)[i] = age_id;
          }
        } else {
          ptr[0] = age_id/20. + 0.025;
        }
      }
      if (inputBlob) {
        uchar* imageData = buf+8;
        float* ptr = inputData + i*sizePerImage;
        if (s->size.c==3) {
          int n = sizePerImage/3;
          for (j = 0; j < n; j++) {
            ptr[j+n*0] = (float)imageData[j*3+0] * scaleRate;
            ptr[j+n*1] = (float)imageData[j*3+1] * scaleRate;
            ptr[j+n*2] = (float)imageData[j*3+2] * scaleRate;
          }
        }
        if (s->size.c==1) {
          for (j = 0; j < sizePerImage; j++) {
            ptr[j] = (float)imageData[j] * scaleRate;
          }
        }
      }
    }
    //printf("%10s %10s err %d/%d, rate %3.1f\n", gender_tag[gender_id], age_tag[age_id]);
    //ShowImagePal("asdf", 64, 64, buf+8, 64, 1, PixFmtMax, NULL);WaitKey(-1);
    //showmat_1("inputData", inputData, inputBlob->size, 10, 0);WaitKey(-1);
    ret = 1;
  }
  FREE(buf);
  return ret;
}
// 训练年龄分类器
int test_age_train()
{
  //return test_gender_cap();
#if 0
  const char* data_file = "gender_age_1x64x64.dat";
  const char* model_file = "age_model.txt";
  const char* param_file = "age_param.dat";
  const char* param_file2 = "age_param.dat_4.dat";
#endif
#if 1
  const char* data_file = "gender_age_std_1x64x64.dat";
  const char* model_file = "age_std_64_r_model.txt";
  const char* param_file = "age_std_64_r_param.dat";
  const char* param_file2 = "age_std_64_r_param.dat_4.dat";
  int c=1, h=64, w=64;
#endif
  struct Net net[1] = {0};
  struct net_train_test_t t[1] = {0};
  int input, output, label;
  int batch = 64;
  gender_t s[1] = {0};
  struct test_result_t val;
  sys_chdir("C:/code/cnn/gender");
  s->fsize = filesize(data_file);
  s->datalen = 8 + c*h*w;
  s->size = dataSize(s->fsize/s->datalen, c, h, w);
  s->pf = fopen(data_file, "rb");
  s->classes = 1;
  printf("s->classes=%d\n", s->classes);
  printf("size=%d %d %d %d\n", s->size.n, s->size.c, s->size.h, s->size.w);
  ASSERT(s->pf);
  if (0) {
    int i=0, err=0;
    uchar* buf = NULL;
    MYREALLOC(buf, s->datalen);
    setLogRedirect(NULL);
    for (i=0; i<s->size.n; ++i) {
      fread(buf, s->datalen, 1, s->pf);
      img_t im[1] = {0};
      int gender_id = *(int*)buf;
      int age_id = *(int*)(buf+4);
      IMINIT(im, 64, 64, buf+8, 64, 1, 1);
      //IRECT rc = iRECT2(0, 0, 64, 64);
      //int id2 = get_gender(im, rc);
      //err += (id!=id2);
      printf("%10s %10s err %d/%d, rate %3.1f\n", gender_tag[gender_id], age_tag[age_id], err, i+1, err*100./(i+1));
      imshow_(im); WaitKey(-1);
    }
    FREE(buf);
  }
  if (1) {
    //printf("%f", data[1]);
    int success = 0;
    setLogLevel(EASYCNN_LOG_LEVEL_CRITICAL);
    //load train images and labels
    logCritical("loading training data...");
    assert(s->size.n > 0);
    //train data & validate data
    //train
    int n_test=MIN(10000, s->size.n/5);
    net_train_test_init(t, s, age_fetch_data, s->size.n-n_test, n_test, batch);
    t->learningRate = 0.1f;
    //validate
    logCritical("c:%d , w:%d , h:%d", s->size.c, s->size.w, s->size.h);
    logCritical("construct net begin...");
    if (0 && file_exist(model_file) && file_exist(param_file2)) {
      success = net_load(net, model_file, param_file2);
      input = find_or_insert_blob(net, "input", 0);
      output = find_or_insert_blob(net, "softmax_o", 0);
    } else {
      //net_load(net, model_file, param_file);
      output = buildConvNet2(net, dataSize(batch, s->size.c, s->size.w, s->size.h), s->classes, &input);
      //output = buildMLPNet(net, dataSize(batch, s->size.c, s->size.w, s->size.h), s->classes, &input);
      net_save(net, model_file, NULL, -1);
    }
    net_print(net, stdout, 0);
    //net.setLoss(make_shared<CrossEntropyLoss>());
    net_loss(net, output, label, MSELoss);
    net_train(t, net, input, output, label, model_file, param_file);
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
    net_train_test_init(t, s, age_fetch_data, 0, s->size.n, batch);
    logCritical("c:%d , w:%d , h:%d", s->size.c, s->size.w, s->size.h);
    logCritical("construct net begin...");
    if (0) {
      success = net_load(net, model_file, param_file2);
      assert(success);
      input = find_or_insert_blob(net, "input", 0);
      output = find_or_insert_blob(net, "softmax_o", 0);
      //net_load(net, model_file, NULL);
      //net_save(net, model_file, NULL, 0);
    } else {
#if 0
      //net_load(net, model_file, param_file);
      //output = buildConvNet2(net, dataSize(batch, s->size.c, s->size.w, s->size.h), s->classes, &input);
      output = buildMLPNet(net, dataSize(batch, s->size.c, s->size.w, s->size.h), s->classes, &input);
      net_print(net, stdout, 0);
      net_set_param(net, gender_model, countof(gender_model));
#endif
    }
    net_loss(net, output, label, MSELoss);
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
    t->n_train = s->size.n-1000;
    t->n_test = 1000;
    val = net_test(t, net, input, output, label);
    logCritical("accuracy : %.4f%%", val.accuracy * 100.0f);
    logCritical("finished test.");
    net_free(net);
  }
  return 0;
}
// 年龄数据读取回调
static int age_fetch_data2(void* s0, struct Blob* inputBlob, struct Blob* labelBlob, int start, int shuffle_len, bool onehot)
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
      int k = (i + start);
      fseek(s->pf, k * s->datalen, SEEK_SET);
      fread(buf, s->datalen, 1, s->pf);
      int gender_id = *(int*)(buf);
      int age_id = *(int*)(buf+4)/2;
      if (labelBlob) {
        float* ptr = labelData + i * sizePerLabel;
        if (s->classes>1) {
          if (age_id>=s->classes) {
            printf("age_id>=s->classes\n");
          }
          if (onehot) {
            onehot_f32(sizePerLabel, ptr, age_id);
          } else {
            ((int*)labelData)[i] = age_id;
          }
        } else {
          ptr[0] = age_id/20. + 0.025;
        }
      }
      if (inputBlob) {
        uchar* imageData = buf+8;
        float* ptr = inputData + i * sizePerImage;
        if (s->size.c==3) {
          int n = sizePerImage/3;
          for (j = 0; j < n; j++) {
            ptr[j+n*0] = (float)imageData[j*3+0] * scaleRate;
            ptr[j+n*1] = (float)imageData[j*3+1] * scaleRate;
            ptr[j+n*2] = (float)imageData[j*3+2] * scaleRate;
          }
        }
        if (s->size.c==1) {
          for (j = 0; j < sizePerImage; j++) {
            ptr[j] = (float)imageData[j] * scaleRate;
          }
        }
      }
      //printf("%10s %10s err %d/%d, rate %3.1f\n", gender_tag[gender_id], age_tag[age_id]);
      //ShowImagePal("asdf", 64, 128, buf+8, 128, 1, PixFmtMax, NULL);WaitKey(-1);
    }
    //showmat_1("inputData", inputData, inputBlob->size, 1, 0);WaitKey(-1);
    ret = 1;
  }
  FREE(buf);
  return ret;
}
// 构建网络
static int buildConvNet3(struct Net* net, DataSize size, int classes, int* pinput)
{
  int t, w = size.w, i=0, _minw=4;
  //int ker[] = {32, 32, 16};
  t = *pinput = net_input(net, size);
  t = add_conv(net, t, 96, 5);
  t = add_active_layer(net, t);
  t = add_pool(net, t);
  w -= 4;
  w /= 2;
  for (i=0; i<2; ++i) {
    //t = add_conv_same(net, t, 12, 5);
    //t = add_conv(net, t, i<3 ? ker[i] : 16);
    t = add_conv(net, t, i==0 ? 64 : 32, 3);
    w-=2;
    //t = net_scale(net, t);
    t = add_active_layer(net, t);
    t = add_pool(net, t);
    w /= 2;
  }
  t = net_dropout(net, t, 0.5f);
  t = add_fc_layer(net, t, 128);
  t = add_active_layer(net, t);
  t = net_dropout(net, t, 0.5f);
  t = add_fc_layer(net, t, classes);
  t = net_softmax(net, t);
  return t;
}
// 训练年龄分类器
int test_age_train2()
{
  g_logfile = "log.txt";
  //return test_gender_cap();
#if 0
  const char* data_file = "gender_age_1x128x128.dat";
  const char* model_file = "age_128_model.txt";
  const char* param_file = "age_128_param.dat";
  const char* param_file2 = "age_128_param.dat_1.dat";
  int c=1, h=128, w=128;
#endif
#if 0
  const char* data_file = "gender_age_1x64x64.dat";
  const char* model_file = "age_64_model.txt";
  const char* param_file = "age_64_param.dat";
  const char* param_file2 = "age_64_param.dat_4.dat";
  int c=1, h=64, w=64;
#endif
#if 0
  const char* data_file = "gender_age_std_1x64x64.dat";
  const char* model_file = "age_std_64_model.txt";
  const char* param_file = "age_std_64_param.dat";
  const char* param_file2 = "age_std_64_param.dat_3.dat";
  int c=1, h=64, w=64;
#endif
#if 0
  const char* data_file = "gender_age_std_1x64x64.dat";
  const char* model_file = "age_std_64_model_32_16.txt";
  const char* param_file = "age_std_64_param_32_16.dat";
  const char* param_file2 = "age_std_64_param_32_16.dat_3.dat";
  int c=1, h=64, w=64;
#endif
#if 0
  const char* data_file = "gender_age_std_1x64x64.dat";
  const char* model_file = "age_std_64_model_64_32_16.txt";
  const char* param_file = "age_std_64_param_64_32_16.dat";
  const char* param_file2 = "age_std_64_param_64_32_16.dat_3.dat";
  int c=1, h=64, w=64;
#endif
#if 1
  const char* data_file = "gender_age_std_1x64x64.dat";
  const char* model_file = "age_std_64_model_96_64_32.txt";
  const char* param_file = "age_std_64_param_96_64_32.dat";
  const char* param_file2 = "age_std_64_param_96_64_32.dat_3.dat";
  int c=1, h=64, w=64;
#endif
#if 0
  const char* data_file = "gender_age_std_1x64x64.dat";
  const char* model_file = "age_std_64_model_32.txt";
  const char* param_file = "age_std_64_param_32.dat";
  const char* param_file2 = "age_std_64_param_32.dat_4.dat";
  int c=1, h=64, w=64;
#endif
#if 0
  const char* data_file = "gender_age_std_1x64x64.dat";
  const char* model_file = "age_std_64_model_64.txt";
  const char* param_file = "age_std_64_param_64.dat";
  const char* param_file2 = "age_std_64_param_64.dat_4.dat";
  int c=1, h=64, w=64;
#endif
#if 0
  const char* data_file = "gender_age_3x64x64.dat";
  const char* model_file = "age_3x64_model.txt";
  const char* param_file = "age_3x64_param.dat";
  const char* param_file2 = "age_3x64_param.dat_4.dat";
  int c=3, h=64, w=64;
#endif
  struct Net net[1] = {0};
  struct net_train_test_t t[1] = {0};
  int input, output, label;
  int batch = 64;
  gender_t s[1] = {0};
  struct test_result_t val;
  g_logfile = "log.txt";
  sys_chdir("C:/code/cnn/gender");
  s->fsize = filesize(data_file);
  s->datalen = 8 + c*h*w;
  s->size = dataSize(s->fsize/s->datalen, c, h, w);
  s->pf = fopen(data_file, "rb");
  s->classes = countof(age_tag)/2;
  printf("s->classes=%d\n", s->classes);
  printf("size=%d %d %d %d\n", s->size.n, s->size.c, s->size.h, s->size.w);
  ASSERT(s->pf);
  if (0) {
    int i=0, err=0;
    uchar* buf = NULL;
    MYREALLOC(buf, s->datalen);
    setLogRedirect(NULL);
    for (i=0; i<s->size.n; ++i) {
      fread(buf, s->datalen, 1, s->pf);
      img_t im[1] = {0};
      int gender_id = *(int*)buf;
      int age_id = *(int*)(buf+4);
      IMINIT(im, h, w, buf+8, w, 1, 1);
      //IRECT rc = iRECT2(0, 0, 64, 64);
      //int id2 = get_gender(im, rc);
      //err += (id!=id2);
      printf("%10s %10s err %d/%d, rate %3.1f\n", gender_tag[gender_id], age_tag[age_id], err, i+1, err*100./(i+1));
      imshow_(im); WaitKey(-1);
    }
    FREE(buf);
  }
  if (1) {
    //printf("%f", data[1]);
    int success = 0;
    setLogLevel(EASYCNN_LOG_LEVEL_CRITICAL);
    //load train images and labels
    logCritical("loading training data...");
    assert(s->size.n > 0);
    //train data & validate data
    //train
    int n_test=MIN(5000, s->size.n/10);
    net_train_test_init(t, s, age_fetch_data2, s->size.n-n_test, n_test, batch);
    t->learningRate = 0.1f;
    //t->isclass = 1;
    //validate
    logCritical("c:%d , w:%d , h:%d", s->size.c, s->size.w, s->size.h);
    logCritical("construct net begin...");
    if (file_exist(model_file) && file_exist(param_file2)) {
      success = net_load(net, model_file, param_file2);
      input = find_or_insert_blob(net, "input", 0);
      output = find_or_insert_blob(net, "softmax_o", 0);
    } else {
      //net_load(net, model_file, param_file);
      output = buildConvNet3(net, dataSize(batch, s->size.c, s->size.w, s->size.h), s->classes, &input);
      //output = buildMLPNet(net, dataSize(batch, s->size.c, s->size.w, s->size.h), s->classes, &input);
      net_save(net, model_file, NULL, -1);
    }
    net_print(net, stdout, 0);
    //net.setLossLoss(make_shared<CrossEntropyLoss>());
    net_loss(net, output, label, CrossEntropyLoss);
    net_train(t, net, input, output, label, model_file, param_file);
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
    net_train_test_init(t, s, age_fetch_data2, 0, s->size.n, batch);
    logCritical("c:%d , w:%d , h:%d", s->size.c, s->size.w, s->size.h);
    logCritical("construct net begin...");
    if (1) {
      success = net_load(net, model_file, param_file2);
      assert(success);
      input = find_or_insert_blob(net, "input", 0);
      output = find_or_insert_blob(net, "softmax_o", 0);
      //net_load(net, model_file, NULL);
      //net_save(net, model_file, NULL, 0);
    } else {
#if 0
      //net_load(net, model_file, param_file);
      //output = buildConvNet2(net, dataSize(batch, s->size.c, s->size.w, s->size.h), s->classes, &input);
      output = buildMLPNet(net, dataSize(batch, s->size.c, s->size.w, s->size.h), s->classes, &input);
      net_print(net, stdout, 0);
      net_set_param(net, gender_model, countof(gender_model));
#endif
    }
    net_loss(net, output, label, CrossEntropyLoss);
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
    t->n_train = s->size.n-1000;
    t->n_test = 1000;
    val = net_test(t, net, input, output, label);
    logCritical("accuracy : %.4f%%", val.accuracy * 100.0f);
    logCritical("finished test.");
    net_free(net);
  }
  return 0;
}

// 洗牌算法
str_t* str_random_shuffle(int n, str_t* arr)
{
  int i, j;
  str_t t;
  for (i = n - 1; i > 0; --i) {
    j = rand() % i;
    t = arr[i], arr[i] = arr[j], arr[j] = t;
  }
  return arr;
}
// 测试函数
static int test_gender_age() {
  vstr_t sv[1] = {0};
  img_t im[1] = {0};
  int acc = 0, detcnt = 0;
  int cnt = 0;
  int accage = 0;
  float age_err_sum=0;
  float age_err_abssum = 0;
  int gender_id = 0;
  int age_id = 0;
  int i=0;
  struct Net age_net[1] = {0};
  struct Net gender_net[1] = {0};
  double tage=0, tgender=0;
  
  const char* list_file = "list.txt";
  sys_chdir("D:/code/cnn/gender");
  net_load(age_net, "age_64_model.txt", "age_64_param.dat_6.dat");
  net_load(gender_net, "gender_model.txt", "gender_param.dat_3.dat");
  DataSize size;
  int delims = '\\';
  vstr_load(list_file, sv);
  str_random_shuffle(sv->n, sv->v);
  size = dataSize(sv->n, 3, 64, 64);
  for (i=sv->n*0; i<sv->n; ++i) {
    int delims = '\\';
    ++cnt;
    gender_id = str_to_label(sv->v[i].s, gender_tag, countof(gender_tag), delims);
    age_id = str_to_label(sv->v[i].s, age_tag, countof(age_tag), delims);
    int age = age_id*5 + 3;
    
    if (gender_id>=0 && imread(sv->v[i].s, 3, 1, im)) {
      int ageid=0;
      int genderid=0;
      {utime_start(_start_time);
      net_run(age_net, "input", "softmax_o", im, &ageid, NULL);
      tage += utime_elapsed(_start_time); }
      {utime_start(_start_time);
      net_run(gender_net, "input", "softmax_o", im, &genderid, NULL);
      tgender += utime_elapsed(_start_time); }
      int age1=ageid*10 + 5;
      
      acc += gender_id==genderid;
      accage += fabs(age - age1)<10;
      age_err_sum += (age - age1);
      age_err_abssum += fabs(age - age1);
      ++detcnt;
      if (0) {
      printf("A %d / %d / %d  G: %3.1f FDR: %3.1f %3.1f EM: %3.1f EAM: %3.1f %3.1f %3.1f\n",
        acc, detcnt, cnt, acc*100. / detcnt, detcnt*100. / (cnt), accage*100. / detcnt,
        age_err_sum / detcnt, age_err_abssum / detcnt, tage*1000. / detcnt, tgender*1000. / detcnt);
      }
      if (1) {
        printf("%d %d %d %d\n", gender_id, age, genderid, age1);
        imshow_(im);WaitKey(-1);
      }
    }
  }
  net_free(age_net);
  net_free(gender_net);
  vstr_free(sv);
  return 0;
}

int test_gender_age2() {
  vstr_t sv[1] = {0};
  img_t im[3] = {0};
  int acc = 0, detcnt = 0;
  int cnt = 0;
  int accage = 0;
  float age_err_sum=0;
  float age_err_abssum = 0;
  int gender_id = 0;
  int age_id = 0;
  int i=0;
  struct Net age_net[1] = {0};
  struct Net age_std_net[1] = {0};
  struct Net gender_net[1] = {0};
  double tage=0, tgender=0;
  
  const char* list_file = "list.txt";
  sys_chdir("D:/code/cnn/gender");

  net_load(age_std_net, "age_std_64_model_96_64_32.txt", "age_std_64_param_96_64_32.dat_7.dat");
  net_load(age_net, "age_64_model.txt", "age_64_param.dat_6.dat");
  net_load(gender_net, "gender_model.txt", "gender_param.dat_3.dat");
  if (0) {
    net_save(age_std_net, "age_std_64_model.txt", NULL, 0);
    net_save(age_net, "age_64_model.txt", NULL, 0);
    net_save(gender_net, "gender_model.txt", NULL, 0);
  }
  DataSize size;
  int delims = '\\';
  sys_chdir("D:/code/facedetect/1111/180117");
  sys_chdir("D:/code/cnn/gender");
  vstr_load(list_file, sv);
  str_random_shuffle(sv->n, sv->v);
  size = dataSize(sv->n, 3, 64, 64);
  for (i=sv->n*0; i<sv->n; ++i) {
    int delims = '\\';
    ++cnt;
    //gender_id = str_to_label(sv->v[i].s, gender_tag, countof(gender_tag), delims);
    //age_id = str_to_label(sv->v[i].s, age_tag, countof(age_tag), delims);
    int age = age_id*5 + 3;
    
    if (imread(sv->v[i].s, 3, 1, im)) {
      int ageid=0;
      float fage = 0;
      int genderid=0;
      img_t* im_std = im+1;
      int is_std = 0;
#if 0
      if (is_std && face_std_64(im, im_std)) {
        //imshow_(im_std);WaitKey(-1);
        {utime_start(_start_time);
        net_run(age_std_net, "input", "softmax_o", im_std, &ageid, &fage);
        tage += utime_elapsed(_start_time); }
      } else
#endif
      {
        {utime_start(_start_time);
        net_run(age_net, "input", "softmax_o", im, &ageid, &fage);
        tage += utime_elapsed(_start_time); }
      }
      {utime_start(_start_time);
      net_run(gender_net, "input", "softmax_o", im, &genderid, NULL);
      tgender += utime_elapsed(_start_time); }
      int age1=fage*10 + 5;
      
      acc += gender_id==genderid;
      accage += fabs(age - age1)<10;
      age_err_sum += (age - age1);
      age_err_abssum += fabs(age - age1);
      ++detcnt;
      if (1) {
        printf("%d %d\n", genderid, age1);
        //imshow_(im);WaitKey(-1);
      }
    }
  }
  net_free(age_net);
  net_free(gender_net);
  net_free(age_std_net);
  vstr_free(sv);
  imfrees2(im);
  return 0;
}
