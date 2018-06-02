
#ifndef _MTCNN_INL_
#define _MTCNN_INL_

#if 0
#include "cblas.h"
#define USE_OPENBLAS
#ifdef _WIN32
// cblas_sgemm
// cblas_sgemv
#pragma comment(lib,"libopenblas.lib")
#endif
#endif

#include "ncnn.inl"

//#include "cv1/cv.inl"

typedef struct Bbox {
  float score;
  float x1;
  float y1;
  float x2;
  float y2;
  float area;
  bool exist;
  Dtype ppoint[10];
  Dtype regreCoord[4];
} Bbox;

typedef struct Pnet {
  Dtype nms_threshold;
  Dtype Pthreshold;

  //Weight
  struct Weight conv1_wb[1];
  struct pRelu prelu_gmma1[1];
  struct Weight conv2_wb[1];
  struct pRelu prelu_gmma2[1];
  struct Weight conv3_wb[1];
  struct pRelu prelu_gmma3[1];
  struct Weight conv4c1_wb[1];
  struct Weight conv4c2_wb[1];
} Pnet;

typedef struct Rnet {
  float Rthreshold;

  //Weight
  struct Weight conv1_wb[1];
  struct pRelu prelu_gmma1[1];
  struct Weight conv2_wb[1];
  struct pRelu prelu_gmma2[1];
  struct Weight conv3_wb[1];
  struct pRelu prelu_gmma3[1];
  struct Weight fc4_wb[1];
  struct pRelu prelu_gmma4[1];
  struct Weight score_wb[1];
  struct Weight location_wb[1];
} Rnet;

typedef struct Onet {
  float Othreshold;

  //Weight
  struct Weight conv1_wb[1];
  struct pRelu prelu_gmma1[1];
  struct Weight conv2_wb[1];
  struct pRelu prelu_gmma2[1];
  struct Weight conv3_wb[1];
  struct pRelu prelu_gmma3[1];
  struct Weight conv4_wb[1];
  struct pRelu prelu_gmma4[1];
  struct Weight fc5_wb[1];
  struct pRelu prelu_gmma5[1];
  struct Weight score_wb[1];
  struct Weight location_wb[1];
  struct Weight keyPoint_wb[1];
} Onet;

typedef struct mtcnn {
  float nms_threshold[3];
  Pnet simpleFace_[1];
  Rnet refineNet[1];
  Onet outNet[1];
} mtcnn;

#if 0
static void pBoxShow(const pBox* pbox)
{
  if (pbox->data->p == NULL) {
    printf("pbox is NULL, please check it!\n");
    return;
  }

  printf("the data is :\n");
  Dtype* p = pbox->data->p;

  //pbox->channel
  for (int channel = 0; channel < pbox->channel; channel++) {
    printf("the %d th channel data is :\n", channel);

    //pbox->height
    for (int i = 0; i < pbox->height; i++) {
      for (int k = 0; k < pbox->width; k++) {
        printf("%f ", *p++);
      }

      printf("\n");
    }
  }

  p = NULL;
}

static void pBoxShowE(const pBox* pbox, int channel, int row)
{
  if (pbox->data->p == NULL) {
    printf("the pbox is NULL, please check it!\n");
    return;
  }

  printf("the data is :\n");
  Dtype* p = pbox->data->p + channel * pbox->width * pbox->height;
  //pbox->channel
  printf("the %d th channel data is :\n", channel);
  //pbox->height

  for (int i = 0; i < pbox->height; i++) {
    if (i < 0) {
      for (int k = 0; k < pbox->width; k++) {
        printf("%f ", *p++);
      }

      printf("\n");
    }
    else if (i == row) {
      p += i * pbox->width;

      for (int k = 0; k < pbox->width; k++) {
        if (k % 4 == 0) {
          printf("\n");
        }

        printf("%f ", *p++);
      }

      printf("\n");
    }
  }

  p = NULL;
}

static void pReluShow(const struct pRelu* prelu)
{
  if (prelu->data->p == NULL) {
    printf("the prelu is NULL, please check it !\n");
    return;
  }

  printf("the data is :\n");
  Dtype* p = prelu->data->p;

  for (int i = 0; i < prelu->width; i++) {
    printf("%f ", *p++);
  }

  printf("\n");
  p = NULL;
}

static void weightShow(const struct Weight* weight)
{
  if (weight->data->p == NULL) {
    printf("the weight is NULL, please check it !\n");
    return;
  }

  printf("the weight data is :\n");
  Dtype* p = weight->data->p;

  for (int channel = 0; channel < weight->selfChannel; channel++) {
    printf("the %d th channel data is :\n", channel);

    for (int i = 0; i < weight->lastChannel; i++) {
      for (int k = 0; k < weight->kernelSize * weight->kernelSize; k++) {
        printf("%f ", *p++);
      }

      printf("\n");
    }
  }

  p = NULL;
}
#endif


typedef enum RectOpt { RO_Union, RO_Min, } RectOpt;
#if 0
typedef struct orderScore { Dtype score; int oriOrder; } orderScore;

bool cmpScore(struct orderScore lsh, struct orderScore rsh)
{
  if (lsh.score < rsh.score) {
    return true;
  }
  else {
    return false;
  }
}

static int nms1(Bbox* boundingBox_, int boundingBox_size, const float overlap_threshold, RectOpt modelname CC_DEFAULT(RO_Union))
{
  int* heros = NULL;
  int heros_size = 0;
  struct orderScore* bboxScore_ = NULL;
  int i, j, bboxScore_size;
  int order = 0, count;
  float IOU = 0;
  float maxX = 0;
  float maxY = 0;
  float minX = 0;
  float minY = 0;

  if (boundingBox_size <= 0) {
    return 0;
  }

  BF_MALLOC(heros, boundingBox_size);
  BF_MALLOC(bboxScore_, boundingBox_size);

  for (i = 0; i < boundingBox_size; ++i) {
    heros[i] = 0;
    bboxScore_[i].score = boundingBox_[i].score;
    bboxScore_[i].oriOrder = i;
  }

  bboxScore_size = boundingBox_size;
  //sort the score
#define CMPSCORE(a, b)   ((a).score < (b).score)
  QSORT(bboxScore_, bboxScore_ + boundingBox_size, CMPSCORE, struct orderScore);
#undef CMPSCORE

  while (bboxScore_size > 0) {
    order = bboxScore_[--bboxScore_size].oriOrder;

    if (order < 0) {
      continue;
    }

    heros[heros_size++] = (order);
    boundingBox_[(order)].exist = false;//delete it

    for (i = 0; i < boundingBox_size; i++) {
      if (boundingBox_[i].exist) {
        //the iou
        maxX = (boundingBox_[i].x1 > boundingBox_[order].x1) ? boundingBox_[i].x1 : boundingBox_[order].x1;
        maxY = (boundingBox_[i].y1 > boundingBox_[order].y1) ? boundingBox_[i].y1 : boundingBox_[order].y1;
        minX = (boundingBox_[i].x2 < boundingBox_[order].x2) ? boundingBox_[i].x2 : boundingBox_[order].x2;
        minY = (boundingBox_[i].y2 < boundingBox_[order].y2) ? boundingBox_[i].y2 : boundingBox_[order].y2;
        //maxX1 and maxY1 reuse
        maxX = ((minX - maxX + 1) > 0) ? (minX - maxX + 1) : 0;
        maxY = ((minY - maxY + 1) > 0) ? (minY - maxY + 1) : 0;
        //IOU reuse for the area of two bbox
        IOU = maxX * maxY;

        if (modelname == RO_Union) {
          IOU = IOU / (boundingBox_[i].area + boundingBox_[order].area - IOU);
        }
        else if (modelname == RO_Min) {
          IOU = IOU / ((boundingBox_[i].area < boundingBox_[order].area) ? boundingBox_[i].area : boundingBox_[order].area);
        }

        if (IOU > overlap_threshold) {
          boundingBox_[i].exist = false;

          for (j = 0; j < bboxScore_size; j++) {
            if (bboxScore_[j].oriOrder == i) {
              bboxScore_[j].oriOrder = -1;
              break;
            }
          }
        }
      }
    }
  }

  for (i = 0; i < heros_size; i++) {
    boundingBox_[heros[i]].exist = true;
  }

  BF_FREE(bboxScore_);
  BF_FREE(heros);
  count = 0;

  for (i = 0; i < boundingBox_size; i++) {
    if (boundingBox_[i].exist) {
      boundingBox_[count++] = boundingBox_[i];
    }
  }

  return count;
}
#endif

static int nms(Bbox* boundingBox_, int boundingBox_size, const float overlap_threshold, RectOpt modelname CC_DEFAULT(RO_Union) )
{
  int i, j;
  int count = 0;
  //sort the score
#define CMPSCORE(a, b)   ((a).score > (b).score)
  QSORT(boundingBox_, boundingBox_ + boundingBox_size, CMPSCORE, struct Bbox);
#undef CMPSCORE

  for (j=0; j<boundingBox_size; ++j) {
    if (boundingBox_[j].exist) {
      for (i = j+1; i < boundingBox_size; i++) {
        if (boundingBox_[i].exist) {
          //the iou
          float maxX = (boundingBox_[i].x1 > boundingBox_[j].x1) ? boundingBox_[i].x1 : boundingBox_[j].x1;
          float maxY = (boundingBox_[i].y1 > boundingBox_[j].y1) ? boundingBox_[i].y1 : boundingBox_[j].y1;
          float minX = (boundingBox_[i].x2 < boundingBox_[j].x2) ? boundingBox_[i].x2 : boundingBox_[j].x2;
          float minY = (boundingBox_[i].y2 < boundingBox_[j].y2) ? boundingBox_[i].y2 : boundingBox_[j].y2;
          //maxX1 and maxY1 reuse
          float maxW = ((minX - maxX + 1) > 0) ? (minX - maxX + 1) : 0;
          float maxH = ((minY - maxY + 1) > 0) ? (minY - maxY + 1) : 0;
          //IOU reuse for the area of two bbox
          float IOU = maxW * maxH;
          
          if (modelname == RO_Union) {
            IOU = IOU / (boundingBox_[i].area + boundingBox_[j].area - IOU);
          }
          else if (modelname == RO_Min) {
            IOU = IOU / ((boundingBox_[i].area < boundingBox_[j].area) ? boundingBox_[i].area : boundingBox_[j].area);
          }
          
          if (IOU > overlap_threshold) {
            boundingBox_[i].exist = false;
          }
        }
      }
      boundingBox_[count++] = boundingBox_[j];
    }
  }
  return count;
}

static void refineAndSquareBbox(struct Bbox* vecBbox, int vecBbox_size, int height, int width)
{
  float bbw = 0, bbh = 0, maxSide = 0;
  float h = 0, w = 0;
  float x1 = 0, y1 = 0, x2 = 0, y2 = 0;
  struct Bbox* vecBbox_end = vecBbox + vecBbox_size, *it;

  if (vecBbox_size <= 0) {
    printf("Bbox is empty!!\n");
    return;
  }

  for (it = vecBbox; it != vecBbox_end; it++) {
    if ((*it).exist) {
      bbh = (*it).x2 - (*it).x1 + 1;
      bbw = (*it).y2 - (*it).y1 + 1;
      x1 = (*it).x1 + (*it).regreCoord[1] * bbh;
      y1 = (*it).y1 + (*it).regreCoord[0] * bbw;
      x2 = (*it).x2 + (*it).regreCoord[3] * bbh;
      y2 = (*it).y2 + (*it).regreCoord[2] * bbw;

      h = x2 - x1 + 1;
      w = y2 - y1 + 1;

      maxSide = (h > w) ? h : w;
      x1 = x1 + h * 0.5 - maxSide * 0.5;
      y1 = y1 + w * 0.5 - maxSide * 0.5;
      (*it).x2 = (x1 + maxSide - 1);
      (*it).y2 = (y1 + maxSide - 1);
      (*it).x1 = (x1);
      (*it).y1 = (y1);

      //boundary check
      if ((*it).x1 < 0) {
        (*it).x1 = 0;
      }

      if ((*it).y1 < 0) {
        (*it).y1 = 0;
      }

      if ((*it).x2 > height) {
        (*it).x2 = height - 1;
      }

      if ((*it).y2 > width) {
        (*it).y2 = width - 1;
      }

      it->area = (it->x2 - it->x1) * (it->y2 - it->y1);
    }
  }
}

static int generateBbox(float Pthreshold, pBox* score, const pBox* location, Dtype scale, struct Bbox* boundingBox_, int boundingBox_size)
{
  //for pooling
  int stride = 2;
  int cellsize = 12;
  int count = 0, row, col, channel;
  //score p
  Dtype* p = pBox_datap(score) + score->width * score->height;
  Dtype* plocal = pBox_datap(location);
  struct Bbox bbox;

  for (row = 0; row < score->height && count < boundingBox_size; row++) {
    for (col = 0; col < score->width && count < boundingBox_size; col++) {
      if (*p > Pthreshold) {
        bbox.score = *p;
        bbox.x1 = ((stride * row + 1) / scale);
        bbox.y1 = ((stride * col + 1) / scale);
        bbox.x2 = ((stride * row + 1 + cellsize) / scale);
        bbox.y2 = ((stride * col + 1 + cellsize) / scale);
        bbox.exist = true;
        bbox.area = (bbox.x2 - bbox.x1) * (bbox.y2 - bbox.y1);

        for (channel = 0; channel < 4; channel++) {
          bbox.regreCoord[channel] = *(plocal + channel * location->width * location->height);
        }

        boundingBox_[count++] = (bbox);
      }

      p++;
      plocal++;
    }
  }

  return count;
}
#define MTCNN_ISMEM  1
#if MTCNN_ISMEM==1
const static float Pnet_data[] = {
#include "mtcnn/Pnet.txt"
};
const static float Rnet_data[] = {
#include "mtcnn/Rnet.txt"
};
const static float Onet_data[] = {
#include "mtcnn/Onet.txt"
};

#define NETDATA(net)   (const char*)net ## _data
#else
#define NETDATA(net)   #net
#endif

static void Pnet_init(Pnet* s)
{
  //  w       sc  lc ks s  p
  long conv1 = initConvAndFc(s->conv1_wb, 10, 3, 3, 1, 0);
  int prelu_gmma1 = initpRelu(s->prelu_gmma1, 10);
  long conv2 = initConvAndFc(s->conv2_wb, 16, 10, 3, 1, 0);
  int prelu_gmma2 = initpRelu(s->prelu_gmma2, 16);
  long conv3 = initConvAndFc(s->conv3_wb, 32, 16, 3, 1, 0);
  int prelu_gmma3 = initpRelu(s->prelu_gmma3, 32);
  long conv4c1 = initConvAndFc(s->conv4c1_wb, 2, 32, 1, 1, 0);
  long conv4c2 = initConvAndFc(s->conv4c2_wb, 4, 32, 1, 1, 0);
  long dataNumber[13] = {conv1, 10, 10, conv2, 16, 16, conv3, 32, 32, conv4c1, 2, conv4c2, 4};
  umem_t* pointTeam[] = {s->conv1_wb->data, s->conv1_wb->bias, s->prelu_gmma1->data, \
      s->conv2_wb->data, s->conv2_wb->bias, s->prelu_gmma2->data, \
      s->conv3_wb->data, s->conv3_wb->bias, s->prelu_gmma3->data, \
      s->conv4c1_wb->data, s->conv4c1_wb->bias, \
      s->conv4c2_wb->data, s->conv4c2_wb->bias \
                        };
  readData1(MTCNN_ISMEM, NETDATA(Pnet), countof(pointTeam), dataNumber, pointTeam);
  s->Pthreshold = 0.6;
  s->nms_threshold = 0.5;
}

static void Pnet_free(Pnet* s)
{
  freeWeight(s->conv1_wb);
  freepRelu(s->prelu_gmma1);
  freeWeight(s->conv2_wb);
  freepRelu(s->prelu_gmma2);
  freeWeight(s->conv3_wb);
  freepRelu(s->prelu_gmma3);
  freeWeight(s->conv4c1_wb);
  freeWeight(s->conv4c2_wb);
}

static int Pnet_run(const Pnet* s, img_t* image, float scale, struct Bbox* boundingBox_, int boundingBox_size)
{
  int count;
  pBox w1[20] = {0};
  pBox* w = w1;
  //the image for mxnet conv
  pBox* rgb = w++;
  //the 1th layer's out conv
  pBox* conv1 = w++;
  pBox* maxPooling1 = w++;
  //the 3th layer's out
  pBox* conv2 = w++;
  //the 4th layer's out   out
  pBox* conv3 = w++;
  //the 4th layer's out   out
  pBox* score_ = w++;
  //the 4th layer's out   out
  pBox* location_ = w++;

  //if(firstFlag)
  image2Matrix(image, rgb, 3, 1./127.5, -1);
  // feature2Matrix(rgb, conv1_matrix, s->conv1_wb);
  convolution(s->conv1_wb, rgb, conv1);
  prelu(conv1, s->conv1_wb->bias, s->prelu_gmma1->data);
  //Pooling layer
  maxPooling(conv1, maxPooling1, 2, 2);
  // feature2Matrix(maxPooling1, maxPooling_matrix, s->conv2_wb);
  convolution(s->conv2_wb, maxPooling1, conv2);
  prelu(conv2, s->conv2_wb->bias, s->prelu_gmma2->data);
  //conv3
  // feature2Matrix(conv2, conv3_matrix, s->conv3_wb);
  convolution(s->conv3_wb, conv2, conv3);
  prelu(conv3, s->conv3_wb->bias, s->prelu_gmma3->data);
  //conv4c1   score
  // feature2Matrix(conv3, score_matrix, s->conv4c1_wb);
  convolution(s->conv4c1_wb, conv3, score_);
  addbias(score_, s->conv4c1_wb->bias);
  softmax(score_);
  // pBoxShow(s->score_);
  //conv4c2   location
  // feature2Matrix(conv3, location_matrix, s->conv4c2_wb);
  convolution(s->conv4c2_wb, conv3, location_);
  addbias(location_, s->conv4c2_wb->bias);
  //softmax layer
  count = generateBbox(s->Pthreshold, score_, location_, scale, boundingBox_, boundingBox_size);
  //pBoxfrees(w__, countof(w__));
  imfrees2(w1);
  return count;
}

static void Rnet_init(Rnet* s)
{
  //                             w         sc  lc ks s  p
  long conv1 = initConvAndFc(s->conv1_wb, 28, 3, 3, 1, 0);
  int prelu_gmma1 = initpRelu(s->prelu_gmma1, 28);
  long conv2 = initConvAndFc(s->conv2_wb, 48, 28, 3, 1, 0);
  int prelu_gmma2 = initpRelu(s->prelu_gmma2, 48);
  long conv3 = initConvAndFc(s->conv3_wb, 64, 48, 2, 1, 0);
  int prelu_gmma3 = initpRelu(s->prelu_gmma3, 64);
  long fc4 = initConvAndFc(s->fc4_wb, 128, 576, 1, 1, 0);
  int prelu_gmma4 = initpRelu(s->prelu_gmma4, 128);
  long score = initConvAndFc(s->score_wb, 2, 128, 1, 1, 0);
  long location = initConvAndFc(s->location_wb, 4, 128, 1, 1, 0);
  long dataNumber[16] = {conv1, 28, 28, conv2, 48, 48, conv3, 64, 64, fc4, 128, 128, score, 2, location, 4};
  umem_t* pointTeam[] = {s->conv1_wb->data, s->conv1_wb->bias, s->prelu_gmma1->data, \
      s->conv2_wb->data, s->conv2_wb->bias, s->prelu_gmma2->data, \
      s->conv3_wb->data, s->conv3_wb->bias, s->prelu_gmma3->data, \
      s->fc4_wb->data, s->fc4_wb->bias, s->prelu_gmma4->data, \
      s->score_wb->data, s->score_wb->bias, \
      s->location_wb->data, s->location_wb->bias \
                        };
  readData1(MTCNN_ISMEM, NETDATA(Rnet), countof(pointTeam), dataNumber, pointTeam);
  s->Rthreshold = 0.6;
}
static void Rnet_free(Rnet* s)
{
  freeWeight(s->conv1_wb);
  freepRelu(s->prelu_gmma1);
  freeWeight(s->conv2_wb);
  freepRelu(s->prelu_gmma2);
  freeWeight(s->conv3_wb);
  freepRelu(s->prelu_gmma3);
  freeWeight(s->fc4_wb);
  freepRelu(s->prelu_gmma4);
  freeWeight(s->score_wb);
  freeWeight(s->location_wb);
}
static int Rnet_run(const Rnet* s, img_t* image, struct Bbox* it)
{
  pBox w1[20] = {0};
  pBox* w = w1;
  pBox* w0 = w;
  pBox* score_ = w++;
  pBox* location_ = w++;
  pBox* rgb = w++;
  pBox* conv1_out = w++;
  pBox* pooling1_out = w++;
  pBox* conv2_out = w++;
  pBox* pooling2_out = w++;
  pBox* conv3_out = w++;
  pBox* fc4_out = w++;

  //CHECK_OR_PUT(image);
  image2Matrix(image, rgb, 3, 1./127.5, -1);
  //CHECK_OR_PUT(rgb);

  // feature2Matrix(rgb, conv1_matrix, s->conv1_wb);
  convolution(s->conv1_wb, rgb, conv1_out);
  prelu(conv1_out, s->conv1_wb->bias, s->prelu_gmma1->data);
  maxPooling(conv1_out, pooling1_out, 3, 2);
  //CHECK_OR_PUT(conv1_out);

  // feature2Matrix(pooling1_out, conv2_matrix, s->conv2_wb);
  convolution(s->conv2_wb, pooling1_out, conv2_out);
  prelu(conv2_out, s->conv2_wb->bias, s->prelu_gmma2->data);
  maxPooling(conv2_out, pooling2_out, 3, 2);

  //CHECK_OR_PUT(conv2_out);
  //conv3
  // feature2Matrix(pooling2_out, conv3_matrix, s->conv3_wb);
  convolution(s->conv3_wb, pooling2_out, conv3_out);
  //CHECK_OR_PUT(conv3_out);
  prelu(conv3_out, s->conv3_wb->bias, s->prelu_gmma3->data);

  //flatten
  fullconnect(s->fc4_wb, conv3_out, fc4_out);
  prelu(fc4_out, s->fc4_wb->bias, s->prelu_gmma4->data);

  //conv51   score
  fullconnect(s->score_wb, fc4_out, score_);
  addbias(score_, s->score_wb->bias);
  softmax(score_);

  //conv5_2   location
  fullconnect(s->location_wb, fc4_out, location_);
  addbias(location_, s->location_wb->bias);
  // pBoxShow(location_);
  //CHECK_OR_PUT(score_);
  //CHECK_OR_PUT(location_);

  umem_getu(score_->data);
  umem_getu(location_->data);

  {
    int f, n = image->f;
    
    for (f = 0; f < n; ++f, ++it) {
      const float* scorep = pBox_datapf(score_, f);
      
      //printf("scorep=%f\n", scorep[1]);
      if (scorep[1] > s->Rthreshold) {
        const float* locationp = pBox_datapf(location_, f);
        memcpy(it->regreCoord, locationp, 4 * sizeof(Dtype));
        it->area = (it->x2 - it->x1) * (it->y2 - it->y1);
        it->score = *(scorep + 1);
      }
      else {
        it->exist = false;
      }
    }
  }

  imfrees2(w1);
  return 0;
}

static void Onet_init(Onet* s)
{
  //                             w        sc  lc ks s  p
  long conv1 = initConvAndFc(s->conv1_wb, 32, 3, 3, 1, 0);
  int prelu_gmma1 = initpRelu(s->prelu_gmma1, 32);
  long conv2 = initConvAndFc(s->conv2_wb, 64, 32, 3, 1, 0);
  int prelu_gmma2 = initpRelu(s->prelu_gmma2, 64);
  long conv3 = initConvAndFc(s->conv3_wb, 64, 64, 3, 1, 0);
  int prelu_gmma3 = initpRelu(s->prelu_gmma3, 64);
  long conv4 = initConvAndFc(s->conv4_wb, 128, 64, 2, 1, 0);
  int prelu_gmma4 = initpRelu(s->prelu_gmma4, 128);
  long fc5 = initConvAndFc(s->fc5_wb, 256, 1152, 1, 1, 0);
  int prelu_gmma5 = initpRelu(s->prelu_gmma5, 256);
  long score = initConvAndFc(s->score_wb, 2, 256, 1, 1, 0);
  long location = initConvAndFc(s->location_wb, 4, 256, 1, 1, 0);
  long keyPoint = initConvAndFc(s->keyPoint_wb, 10, 256, 1, 1, 0);
  long dataNumber[21] = {conv1, 32, 32, conv2, 64, 64, conv3, 64, 64, conv4, 128, 128, fc5, 256, 256, score, 2, location, 4, keyPoint, 10};
  umem_t* pointTeam[] = {s->conv1_wb->data, s->conv1_wb->bias, s->prelu_gmma1->data, \
      s->conv2_wb->data, s->conv2_wb->bias, s->prelu_gmma2->data, \
      s->conv3_wb->data, s->conv3_wb->bias, s->prelu_gmma3->data, \
      s->conv4_wb->data, s->conv4_wb->bias, s->prelu_gmma4->data, \
      s->fc5_wb->data, s->fc5_wb->bias, s->prelu_gmma5->data, \
      s->score_wb->data, s->score_wb->bias, \
      s->location_wb->data, s->location_wb->bias, \
      s->keyPoint_wb->data, s->keyPoint_wb->bias \
                        };
  readData1(MTCNN_ISMEM, NETDATA(Onet), countof(pointTeam), dataNumber, pointTeam);
  s->Othreshold = 0.8;
}
static void Onet_free(Onet* s)
{

  freeWeight(s->conv1_wb);
  freepRelu(s->prelu_gmma1);
  freeWeight(s->conv2_wb);
  freepRelu(s->prelu_gmma2);
  freeWeight(s->conv3_wb);
  freepRelu(s->prelu_gmma3);
  freeWeight(s->conv4_wb);
  freepRelu(s->prelu_gmma4);
  freeWeight(s->fc5_wb);
  freepRelu(s->prelu_gmma5);
  freeWeight(s->score_wb);
  freeWeight(s->location_wb);
  freeWeight(s->keyPoint_wb);
}
static int Onet_run(const Onet* s, img_t* image, struct Bbox* it)
{
  pBox w1[20] = {0};
  pBox* w = w1;
  pBox* score_ = w++;
  pBox* location_ = w++;
  pBox* keyPoint_ = w++;

  pBox* rgb = w++;
  //pBox* conv1_matrix = w++;
  pBox* conv1_out = w++;
  pBox* pooling1_out = w++;

  //pBox* conv2_matrix = w++;
  pBox* conv2_out = w++;
  pBox* pooling2_out = w++;

  //pBox* conv3_matrix = w++;
  pBox* conv3_out = w++;
  pBox* pooling3_out = w++;

  //pBox* conv4_matrix = w++;
  pBox* conv4_out = w++;

  pBox* fc5_out = w++;

  image2Matrix(image, rgb, 3, 1./127.5, -1);

  // feature2Matrix(rgb, conv1_matrix, s->conv1_wb);
  convolution(s->conv1_wb, rgb, conv1_out);
  prelu(conv1_out, s->conv1_wb->bias, s->prelu_gmma1->data);

  //Pooling layer
  maxPooling(conv1_out, pooling1_out, 3, 2);

  // feature2Matrix(pooling1_out, conv2_matrix, s->conv2_wb);
  convolution(s->conv2_wb, pooling1_out, conv2_out);
  prelu(conv2_out, s->conv2_wb->bias, s->prelu_gmma2->data);
  maxPooling(conv2_out, pooling2_out, 3, 2);

  //conv3
  // feature2Matrix(pooling2_out, conv3_matrix, s->conv3_wb);
  convolution(s->conv3_wb, pooling2_out, conv3_out);
  prelu(conv3_out, s->conv3_wb->bias, s->prelu_gmma3->data);
  maxPooling(conv3_out, pooling3_out, 2, 2);

  //conv4
  // feature2Matrix(pooling3_out, conv4_matrix, s->conv4_wb);
  convolution(s->conv4_wb, pooling3_out, conv4_out);
  prelu(conv4_out, s->conv4_wb->bias, s->prelu_gmma4->data);

  fullconnect(s->fc5_wb, conv4_out, fc5_out);
  prelu(fc5_out, s->fc5_wb->bias, s->prelu_gmma5->data);

  //conv6_1   score
  fullconnect(s->score_wb, fc5_out, score_);
  addbias(score_, s->score_wb->bias);
  softmax(score_);
  // pBoxShow(s->score_);

  //conv6_2   location
  fullconnect(s->location_wb, fc5_out, location_);
  addbias(location_, s->location_wb->bias);
  // pBoxShow(location_);

  //conv6_2   location
  fullconnect(s->keyPoint_wb, fc5_out, keyPoint_);
  addbias(keyPoint_, s->keyPoint_wb->bias);
  // pBoxShow(keyPoint_);


  umem_getu(score_->data);
  umem_getu(location_->data);
  umem_getu(keyPoint_->data);
  {
    int j, f, n = image->f;
    
    for (f = 0; f < n; ++f, ++it) {
      const float* scorep = pBox_datapf(score_, f);
      
      if (scorep[1] > s->Othreshold) {
        Dtype* keyPoint_p;
        const float* locationp = pBox_datapf(location_, f);
        memcpy(it->regreCoord, locationp, 4 * sizeof(Dtype));
        it->area = (it->x2 - it->x1) * (it->y2 - it->y1);
        it->score = *(scorep + 1);
        keyPoint_p = pBox_datapf(keyPoint_, f);
        
        for (j = 0; j < 5; j++) {
          it->ppoint[j] = it->y1 + (it->y2 - it->y1) * keyPoint_p[j];
        }
        
        for (j = 0; j < 5; j++) {
          it->ppoint[j + 5] = it->x1 + (it->x2 - it->x1) * keyPoint_p[j + 5];
        }
      }
      else {
        it->exist = false;
      }
    }
  }
  imfrees2(w1);
  return 0;
}

static void mtcnn_init(mtcnn* s)
{
  Pnet_init(s->simpleFace_);
  Rnet_init(s->refineNet);
  Onet_init(s->outNet);
  s->nms_threshold[0] = 0.7;
  s->nms_threshold[1] = 0.7;
  s->nms_threshold[2] = 0.7;
}

static void mtcnn_free(mtcnn* s)
{
  Pnet_free(s->simpleFace_);
  Rnet_free(s->refineNet);
  Onet_free(s->outNet);
}

//    Pitch = eav[0]
//    Yaw = eav[1]
//    Roll  = eav[2]
// current_shape 左眼中心 右眼中心 鼻尖 左嘴角 右嘴角
static void EstimateHeadPose(const float* current_shape, float* eav)
{
  static const int samplePdim = 7;
  static const int estimateHeadPosePointIndexs[] = {0, 0, 1, 1, 2, 3, 4};
  //左右眼睛的四个眼角，鼻尖点，左右两个嘴角，一共七个点，估计头部姿态，2*7+1维度的数据
  static const float estimateHeadPose2dArray2[] = {
    0.139791, 27.4028, 7.02636,
    -2.48207, 9.59384, 6.03758,
    1.27402, 10.4795, 6.20801,
    1.17406, 29.1886, 1.67768,
    0.306761, -103.832, 5.66238,
    4.78663, 17.8726, -15.3623,
    -5.20016, 9.29488, -11.2495,
    -25.1704, 10.8649, -29.4877,
    -5.62572, 9.0871, -12.0982,
    -5.19707, -8.25251, 13.3965,
    -23.6643, -13.1348, 29.4322,
    67.239, 0.666896, 1.84304,
    -2.83223, 4.56333, -15.885,
    -4.74948, -3.79454, 12.7986,
    -16.1, 1.47175, 4.03941
  };

  float miny = 10000000000.0f;
  float maxy = 0.0f;
  float sumx = 0.0f;
  float sumy = 0.0f;
  float dist, y;
  float tmp[15];
  int i;

  for (i = 0; i < samplePdim; i++) {
    sumx += current_shape[estimateHeadPosePointIndexs[i]];
    y = current_shape[estimateHeadPosePointIndexs[i] + 5];
    sumy += y;

    if (miny > y) {
      miny = y;
    }

    if (maxy < y) {
      maxy = y;
    }
  }

  dist = maxy - miny;
  sumx = sumx / samplePdim;
  sumy = sumy / samplePdim;

  for (i = 0; i < samplePdim; i++) {
    tmp[i] = (current_shape[estimateHeadPosePointIndexs[i]] - sumx) / dist;
    tmp[i + samplePdim] = (current_shape[estimateHeadPosePointIndexs[i] + 5] - sumy) / dist;
  }

  tmp[2 * samplePdim] = 1.0f;
  //    cv::Mat predict = tmp*estimateHeadPoseMat;
  //    double _pm[12] = {predict.at<float>(0), predict.at<float>(1), predict.at<float>(2), 0,
  //                      predict.at<float>(3), predict.at<float>(4), predict.at<float>(5), 0,
  //                      predict.at<float>(6), predict.at<float>(7), predict.at<float>(8), 0};
  //    cv::Mat tmp0,tmp1,tmp2,tmp3,tmp4,tmp5;
  //    cv::decomposeProjectionMatrix(cv::Mat(3,4,CV_64FC1,_pm),tmp0,tmp1,tmp2,tmp3,tmp4,tmp5,eav);
  // predict = tmp*estimateHeadPoseMat2;
  matMulv(estimateHeadPose2dArray2, tmp, eav, 1, 3, 3, 15);
  //    std::cout << eav[0] << "  " << eav[1] << "  " << eav[2] << std::endl;
  //    Pitch = eav[0];
  //    Yaw = eav[1];
  //    Roll  = eav[2];
  return;
}

#include "img/imgopt.inl"

CC_IMPL img_t*
resize(const img_t* src, img_t* dst, CSize dsize, double fx CC_DEFAULT(0),
       double fy CC_DEFAULT(0), int method CC_DEFAULT(CC_INTER_LINEAR))
{
  imresize(src, dsize.h, dsize.w, dst);
  return dst;
}

static void arch_print() {
  printf("sse2 %d\n", CC_SSE2);
  printf("neon %d\n", CC_NEON);
  printf("vfp %d\n", CC_VFP);
  printf("OPENMP %d\n", CC_OPENMP);
  return ;
}
int face_cnt = 0;
static int mtcnn_findFace(const img_t* image, Bbox* out, int maxout, int minsize CC_DEFAULT(60))
{
  int i, j = 0, count = 0;
//MEM_CHECK_BEGIN;
  static mtcnn g_s[1] = {0};
  static int inited = 0;
  const mtcnn* s = g_s;
  int row = image->height, col = image->width;
  float minl = row > col ? row : col;
  int MIN_DET_SIZE = 12;
  float m = (float)MIN_DET_SIZE / minsize;
  float factor = 0.709;
  //factor = 0.6;
  int factor_count = 0;
  float scales__[256];
  float* scales_ = scales__;
  int scales_size = 0;
  float minside;
  Bbox* box = NULL;
  int box_size = 0;
  int box_pos = 0;
  img_t im[1] = {0};

  minl *= m;

  if (!inited) {
    inited = 1;
    mtcnn_init(g_s);
  }

  while (minl > MIN_DET_SIZE && scales_size < countof(scales__)) {
    if (factor_count > 0) {
      m = m * factor;
    }

    scales_[scales_size++] = (m);
    minl *= factor;
    factor_count++;
    box_size += (image->h*image->w*factor*factor)/1000;
  }
  box_size = BOUND(box_size, 200, 40000);
  MYREALLOC(box, box_size);
  if (NULL==box) {
    return -1;
  }

  minside = row < col ? row : col;

  for (i = 0; i < scales_size; ++i) {
    if (scales_[i] > 1) {
      printf("the minsize is too small\n");
      return 0;
    }

    if (scales_[i] < (MIN_DET_SIZE / minside)) {
      scales_size = (count);
      break;
    }

    count++;
  }
  count = 0;
#if 0

  for (i = 0; i < scales_size && count < box_size; i++) {
    img_t *reImage = im;
    int boundingBox_size = 0;
    int changedH = (int)ceil(image->rows * scales_[i]);
    int changedW = (int)ceil(image->cols * scales_[i]);
    int j;
    tic;
    resize(image, reImage, cSize(changedW, changedH), 0, 0, CC_INTER_LINEAR);
    toc;
    tic;
    boundingBox_size = Pnet_run(&bf1, s->simpleFace_, reImage, scales_[i], box + box_pos, box_size - box_pos);
    toc;
    boundingBox_size = nms(box + box_pos, boundingBox_size, s->simpleFace_->nms_threshold);
    box_pos += boundingBox_size;
    Bbox* boundingBox_end = box + box_pos + boundingBox_size;

    for (j = 0; j < (boundingBox_size) && count < box_size; j++) {
      Bbox* it = box + box_pos + j;
      if (it->exist) {
        box[count++] = *it;
      }
    }
  } 

#else

  for (i = 0; i < scales_size && count < box_size; i++) {
    int boundingBox_size = 0;
    img_t *reImage = im;
    int changedH = (int)ceil(image->rows * scales_[i]);
    int changedW = (int)ceil(image->cols * scales_[i]);
    resize(image, reImage, cSize(changedW, changedH), 0, 0, CC_INTER_LINEAR);
    tic;
    boundingBox_size = Pnet_run(s->simpleFace_, reImage, scales_[i], box + box_pos, box_size - box_pos);
    toc;
    //boundingBox_size = nms(box + box_pos, boundingBox_size, s->simpleFace_->nms_threshold, RO_Union);
    box_pos += boundingBox_size;
    //imshow_(reImage);WaitKey(-1);
  }

  count = nms(box, box_pos, s->simpleFace_->nms_threshold, RO_Union);

#if 0
  if (1) {
    img_t im[1] = {0};
    int num;
    imclone2(image, im);
    
    for (i = 0; i < count; ++i) {
      Bbox* it = box + i;
      printf("(%3d %3d %3d %3d)\n", (int)(*it).y1, (int)(*it).x1, (int)((*it).y2 - (*it).y1), (int)((*it).x2 - (*it).x1));
      cvRectangle(im, cPoint((*it).y1, (*it).x1), cPoint((*it).y2, (*it).x2), cScalar(0, 0, 255, 0), 2, 8, 0);
      
      for (num = 0; num < 5; num++) {
        cvCircle(im, cPoint((int) it->ppoint[num], (int) it->ppoint[num + 5]), 3, cScalar(0, 255, 255, 0), -1, 8, 0);
      }
    }
    imshow_(im); WaitKey(-1);
    imfree(im);
  }
#endif

#endif

  //count = 0;
  //the first stage's nms
  if (count > 0) {
    count = nms(box, count, s->nms_threshold[0], RO_Union);
    refineAndSquareBbox(box, count, image->rows, image->cols);

    //CHECK_OR_PUT_ARR2(box, count);
    //second stage
#if 1
    //#pragma omp parallel for
    for (i = 0; i < count; ++i) {
#if CC_OPENMP
  //printf("%d - %d\n", omp_get_num_threads(), omp_get_thread_num());
#endif
      Bbox* it = box + i;
      img_t* secImage = im;
      img_t temp[1] = {0};
      ASSERT(it->exist);
      cvGetSubRect1(image, temp, it->y1, it->x1, it->y2 - it->y1, it->x2 - it->x1);

      resize(temp, secImage, cSize(24, 24), 0, 0, CC_INTER_LINEAR);
      tic;
      Rnet_run(s->refineNet, secImage, it);
      toc;
    }

    for (j = i = 0; i < count; ++i) {
      Bbox* it = box + i;
      if (it->exist) {
        box[j++] = (*it);
      }
    }
    count = j;

#else
    imsetsize(secImage, 24, 24, im->c, firstBbox_end - box);

    for (it = box; it != firstBbox_end; it++) {
      img_t frame[1] = {0};
      img_t temp[1] = {0};
      cvGetSubRect1(image, temp, it->y1, it->x1, it->y2 - it->y1, it->x2 - it->x1);
      cvGetFrame(secImage, frame, it - box);
      resize(temp, frame, cSize(24, 24), 0, 0, CC_INTER_LINEAR);
    }

    tic;
    Rnet_run(s->refineNet, workbox, secImage, box);
    toc;

    for (it = box; it != firstBbox_end; it++) {
      if (it->exist) {
        box[count++] = (*it);
      }
    }

#endif

face_cnt = count;
    if (count > 0) {
      count = nms(box, count, s->nms_threshold[1], RO_Union);
      refineAndSquareBbox(box, count, image->rows, image->cols);
      //third stage

#if 1
//#pragma omp parallel for
      for (i = 0; i < count; ++i) {
        Bbox* it = box + i;
        img_t *thirdImage = im;
        img_t temp[1] = {0};
        cvGetSubRect1(image, temp, it->y1, it->x1, it->y2 - it->y1, it->x2 - it->x1);

        resize(temp, thirdImage, cSize(48, 48), 0, 0, CC_INTER_LINEAR);
        ASSERT(it->exist);
        tic;
        Onet_run(s->outNet, thirdImage, it);
        toc;
      }
      
      for (j = i = 0; i < count; ++i) {
        Bbox* it = box + i;
        if (it->exist) {
          box[j++] = (*it);
        }
      }
      count = j;

#else
      imsetsize(thirdImage, 48, 48, im->c, secondBbox_end - box);

      for (it = box; it != secondBbox_end; ++it) {
        img_t frame[1] = {0};
        cvGetSubRect1(image, temp, it->y1, it->x1, it->y2 - it->y1, it->x2 - it->x1);
        cvGetFrame(thirdImage, frame, it - box);
        resize(temp, frame, cSize(48, 48), 0, 0, CC_INTER_LINEAR);
      }

      tic;
      Onet_run(s->outNet, workbox, thirdImage, box);
      toc;

      for (it = box; it != secondBbox_end; ++it) {
        if (it->exist) {
          box[count++] = (*it);
        }
      }

#endif

      if (count > 0) {
        refineAndSquareBbox(box, count, image->rows, image->cols);
        count = nms(box, count, s->nms_threshold[2], RO_Min);
      }
    }
  }

  for (j = i = 0; i < count && j < maxout; ++i) {
    if (box[i].exist) {
      out[j++] = box[i];
    }
  }
  count = j;

#define BBOX_LESS(a, b)   ((a).area>(b).area)
  QSORT(out, out + count, BBOX_LESS, Bbox);
#undef BBOX_LESS
  
  FREE(box);
  imfrees2(im);
//MEM_CHECK_END;
  return count;
}

#endif // _MTCNN_INL_
