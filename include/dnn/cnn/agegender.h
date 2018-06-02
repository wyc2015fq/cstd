
#ifndef _AGEGENDER_INL_
#define _AGEGENDER_INL_
#include "cnn.h"

static const char* age_tag[] = {
    "region1_5",
    "region6_10",
    "region11_15",
    "region16_20",
    "region21_25",
    "region26_30",
    "region31_35",
    "region36_40",
    "region41_45",
    "region46_50",
    "region51_55",
    "region56_60",
    "region61_65",
    "region66_70",
    "region71_75",
    "region76_80",
};
static const char* gender_tag[] = {
  "Female",
  "Male",
};

#include "img/imgio.inl"

int net_run(struct Net* net, const char* input_name, const char* output_name, const img_t* im, int* outputId, float* pRegress) {
  int input = find_or_insert_blob(net, input_name, 0);
  int output = find_or_insert_blob(net, output_name, 0);
  if (input>=0 && output>=0) {
    Blob* inputBlob = BLOB(input);
    Blob* outputBlob = BLOB(output);
    float* inputData = setData(inputBlob);
    int i;
    float scaleRate = 1.0f / 255.0f;
    img_t im1[2] = {0};
    Layer* layer = net->layers;
    int h = IARG(input_h);
    int w = IARG(input_w);
    int c = IARG(input_c);
    im = imresize(im, h, w, im1);
    //imshow_(im);WaitKey(-1);
    inputBlob = image2Matrix(im, inputBlob, c, scaleRate, 0, NULL, 0);
    imfrees2(im1);
    MNARG(phase) = M_Test;
    setLogRedirect(NULL);
    net_forward(net);
    const float* outputData = getData(outputBlob);
    int labelSize = Data_3DSize(outputBlob->size);
    for (i=0; i<outputBlob->size.n; ++i) {
      outputId[i] = getMaxIdxInArray(outputData+i*labelSize, labelSize);
      if (pRegress) {
        pRegress[i] = getMaxIdxInArrayRegress(outputData+i*labelSize, labelSize, outputId[i]);
      }
    }
    return 1;
  }
  ASSERT(0);
  return 0;
}

#if 1
#define AGEGENDER_USE_INLINE
const static float age_64_model[] = {
#include "./model/age_64_model.txt"
};
const static float age_std_64_model[] = {
#include "./model/age_std_64_model.txt"
};

const static float gender_model[] = {
#include "./model/gender_model.txt"
};

static int build_age_std_net(struct Net* net, int* pinput)
{
  int i=0, t;
  DataSize size = dataSize(1, 1, 64, 64);
  t = *pinput = net_input(net, size);
  t = add_conv_pool_relu(net, t, 96, 5);
  t = add_conv_pool_relu(net, t, 64, 3);
  t = add_conv_pool_relu(net, t, 32, 3);
  t = net_dropout(net, t, 0.5f);
  t = add_fc_layer(net, t, 128);
  t = add_active_layer(net, t);
  t = net_dropout(net, t, 0.5f);
  t = add_fc_layer(net, t, 8);
  t = net_softmax(net, t);
  return t;
}
static int build_age_net(struct Net* net, int* pinput)
{
  int i=0, t;
  DataSize size = dataSize(1, 1, 64, 64);
  t = *pinput = net_input(net, size);
  t = add_conv_pool_relu(net, t, 16, 5);
  t = add_conv_pool_relu(net, t, 16, 5);
  t = add_conv_pool_relu(net, t, 16, 5);
  t = net_dropout(net, t, 0.5f);
  t = add_fc_layer(net, t, 256);
  t = add_active_layer(net, t);
  t = net_dropout(net, t, 0.5f);
  t = add_fc_layer(net, t, 8);
  t = net_softmax(net, t);
  return t;
}
static int build_gender_net(struct Net* net, int* pinput)
{
  int i=0, t;
  DataSize size = dataSize(1, 1, 64, 64);
  t = *pinput = net_input(net, size);
  t = add_conv_pool_relu(net, t, 6, 5);
  t = add_conv_pool_relu(net, t, 12, 5);
  t = add_conv_pool_relu(net, t, 12, 5);

  t = add_fc_layer(net, t, 512);
  t = add_active_layer(net, t);
  t = add_fc_layer(net, t, 2);
  t = net_softmax(net, t);
  return t;
}
#endif

#include "mtcnn.h"

//#include "face/faceDetect.inl"

#if 0
static int face_std_64(const img_t* im, img_t* im_std) {
  Bbox box[200];
  int nfaces = mtcnn_findFace(im, box, countof(box), 40);
  if (nfaces>0) {
    img_t im1[1] = {0};
    float* p = box[0].ppoint;
    double a = 0.3;
    im = im2gry(im, im1);
    //ShowImagePal("imrgb", im->h, im->w, im->data, im->s, im->c, PixFmtMax, NULL);
    imsetsize(im_std, 64, 64, 1, 1);
    CPM_FaceStd(im->h, im->w, im->data, im->s, im->c,
      im_std->h, im_std->w, im_std->data, im_std->s, im_std->c,
      iPOINT(p[0], p[5]),iPOINT(p[1], p[6]), a, 0.4, 1-a, 0.4);
    imfree(im1);
    return 1;
  }
  return 0;
}

static float face_age_std(const img_t* im, FPOINT l, FPOINT r) {
  static struct Net age_std_net[1] = {0};
  if (0==age_std_net->l_size) {
#ifdef AGEGENDER_USE_INLINE
    int input, output;
    output = build_age_std_net(age_std_net, &input);
    net_set_param(age_std_net, age_std_64_model, countof(age_std_64_model));
#else
    sys_chdir("D:/code/cnn/gender");
    net_load(age_std_net, "age_std_64_model_96_64_32.txt", "age_std_64_param_96_64_32.dat_7.dat");
#endif
  }
  int ageid=0;
  float fage=0;
  img_t im1[2] = {0};
  img_t *im_std = im1 + 1;
  double a = 0.3;
  imsetsize(im_std, 64, 64, 1, 1);
  im = im2gry(im, im1);
  CPM_FaceStd(im->h, im->w, im->data, im->s, im->c,
    im_std->h, im_std->w, im_std->data, im_std->s, im_std->c,
    iPOINT(l.x, l.y), iPOINT(r.x, r.y), a, 0.4, 1-a, 0.4);
  //imshow_(im_std);WaitKey(-1);
  net_run(age_std_net, "input", "softmax_o", im_std, &ageid, &fage);
  imfrees2(im1);
  return fage*10 + 10;
}
#endif

static float face_age(const img_t* im) {
  static struct Net age_net[1] = {0};
  if (0==age_net->l_size) {
#ifdef AGEGENDER_USE_INLINE
    int input, output;
    output = build_age_net(age_net, &input);
    net_set_param(age_net, age_64_model, countof(age_64_model));
#else
    sys_chdir("D:/code/cnn/gender");
    net_load(age_net, "age_64_model.txt", "age_64_param.dat_7.dat");
#endif
  }
  int ageid=0;
  float fage=0;
  net_run(age_net, "input", "softmax_o", im, &ageid, &fage);
  return fage*10 + 10;
}

static int face_gender(const img_t* im) {
  static struct Net gender_net[1] = {0};
  if (0==gender_net->l_size) {
#ifdef AGEGENDER_USE_INLINE
    int input, output;
    output = build_gender_net(gender_net, &input);
    net_set_param(gender_net, gender_model, countof(gender_model));
#else
    sys_chdir("D:/code/cnn/gender");
    net_load(gender_net, "gender_model.txt", "gender_param.dat_3.dat");
#endif
  }
  int genderid=0;
  net_run(gender_net, "input", "softmax_o", im, &genderid, NULL);
  return genderid;
}

#include "facelib/facedetect/facedetect.h"
#if 0
static float face_age_std_face(const img_t* im, const struct Face* face) {
  return face_age_std(im, face->leye_center, face->reye_center);
}
#endif
static float face_age_face(const img_t* im, const struct Face* face) {
  img_t im2[1] = {0};
  imrefrc(im, face->rect, im2);
  int gender = face_age(im2);
  return gender;
}
static float face_gender_face(const img_t* im, const struct Face* face) {
  img_t im2[1] = {0};
  imrefrc(im, face->rect, im2);
  int gender = face_gender(im2);
  return gender;
}

#endif // _AGEGENDER_INL_
