
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

#include "cnn.h"

//#include "cv1/cv.inl"

typedef struct Bbox {
  float score;
  float x1;
  float y1;
  float x2;
  float y2;
  float area;
  int exist;
  Dtype ppoint[10];
  Dtype regreCoord[4];
} Bbox;


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
  int i=0;
  for (; i<vecBbox_size; ++i) {
    struct Bbox* it = vecBbox + i;
    if (it->exist) {
      float bbh = it->x2 - it->x1 + 1;
      float bbw = it->y2 - it->y1 + 1;
      float x1 = it->x1 + it->regreCoord[1] * bbh;
      float y1 = it->y1 + it->regreCoord[0] * bbw;
      float x2 = it->x2 + it->regreCoord[3] * bbh;
      float y2 = it->y2 + it->regreCoord[2] * bbw;

      float h = x2 - x1 + 1;
      float w = y2 - y1 + 1;

      float maxSide = (h > w) ? h : w;
      x1 = x1 + h * 0.5 - maxSide * 0.5;
      y1 = y1 + w * 0.5 - maxSide * 0.5;
      it->x2 = (x1 + maxSide - 1);
      it->y2 = (y1 + maxSide - 1);
      it->x1 = (x1);
      it->y1 = (y1);

      //boundary check
      it->x1 = MAX(it->x1, 0);
      it->y1 = MAX(it->y1, 0);
      it->x2 = MIN(it->x2, height - 1);
      it->y2 = MIN(it->y2, width - 1);

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
  Dtype* p = pBox_datap(score) + score->size.w * score->size.h;
  Dtype* plocal = pBox_datap(location);
  struct Bbox bbox;

  for (row = 0; row < score->size.h && count < boundingBox_size; row++) {
    for (col = 0; col < score->size.w && count < boundingBox_size; col++) {
      if (*p > Pthreshold) {
        bbox.score = *p;
        bbox.x1 = ((stride * row + 1) / scale);
        bbox.y1 = ((stride * col + 1) / scale);
        bbox.x2 = ((stride * row + 1 + cellsize) / scale);
        bbox.y2 = ((stride * col + 1 + cellsize) / scale);
        bbox.exist = true;
        bbox.area = (bbox.x2 - bbox.x1) * (bbox.y2 - bbox.y1);

        for (channel = 0; channel < 4; channel++) {
          bbox.regreCoord[channel] = *(plocal + channel * location->size.w * location->size.h);
        }

        boundingBox_[count++] = (bbox);
      }

      p++;
      plocal++;
    }
  }

  return count;
}
const static float Pnet_data[] = {
#include "model/mtcnn/Pnet.txt"
};
const static float Rnet_data[] = {
#include "model/mtcnn/Rnet.txt"
};
const static float Onet_data[] = {
#include "model/mtcnn/Onet.txt"
};

static int Pnet_run(img_t* image, float scale, struct Bbox* boundingBox_, int boundingBox_size)
{
  float Pthreshold = 0.6;
  int count;
  pBox w[2] = {0};
  float* p = (float*)Pnet_data;
  pBox* rgb = image2Matrix(image, w, 3, 1./127.5, -1, NULL, 0);
  pBox* conv1 = doConv(&p, rgb, w, 10, 3, 3, 1, 0);
  pBox* maxPooling1 = maxPooling(dopRelu(&p, conv1, 10), w, 2, 2);
  pBox* conv2 = doConv(&p, maxPooling1, w, 16, 10, 3, 1, 0);
  pBox* conv3 = doConv(&p, dopRelu(&p, conv2, 16), w, 32, 16, 3, 1, 0);
  pBox* score_ = doConv(&p, dopRelu(&p, conv3, 32), w+1, 2, 32, 1, 1, 0);
  pBox* location_ = doConv(&p, conv3, w, 4, 32, 1, 1, 0);
  softmax(score_);
  ASSERT((p-Pnet_data)==countof(Pnet_data));
  count = generateBbox(Pthreshold, score_, location_, scale, boundingBox_, boundingBox_size);
  Blob_frees2(w);
  return count;
}

static int Rnet_run(img_t* image, struct Bbox* it)
{
  pBox w[2] = {0};

  float* p = (float*)Rnet_data;
  float Rthreshold = 0.6;
  //CHECK_OR_PUT(image);
  pBox* rgb = image2Matrix(image, w, 3, 1./127.5, -1, NULL, 0);
  pBox* conv1_out = doConv(&p, rgb, w, 28, 3, 3, 1, 0);
  pBox* pooling1_out = maxPooling(dopRelu(&p, conv1_out, 28), w, 3, 2);
  pBox* conv2_out = doConv(&p, pooling1_out, w, 48, 28, 3, 1, 0);
  pBox* pooling2_out = maxPooling(dopRelu(&p, conv2_out, 48), w, 3, 2);
  pBox* conv3_out = doConv(&p, pooling2_out, w, 64, 48, 2, 1, 0);
  pBox* fc4_out = doFc(&p, dopRelu(&p, conv3_out, 64), w, 128, 576, 1, 1, 0);
  pBox* score_ = doFc(&p, dopRelu(&p, fc4_out, 128), w+1, 2, 128, 1, 1, 0);
  pBox* location_ = doFc(&p, fc4_out, w, 4, 128, 1, 1, 0);
  softmax(score_);

  ASSERT((p-Rnet_data)==countof(Rnet_data));

  umem_getu(score_->data);
  umem_getu(location_->data);

  {
    int f, n = image->f;
    
    for (f = 0; f < n; ++f, ++it) {
      const float* scorep = pBox_datapf(score_, f);
      
      //printf("scorep=%f\n", scorep[1]);
      if (scorep[1] > Rthreshold) {
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

  Blob_frees2(w);
  return 0;
}

static int Onet_run(img_t* image, struct Bbox* it)
{
  pBox w[4] = {0};
  float* p = (float*)Onet_data;
  pBox* rgb = image2Matrix(image, w, 3, 1./127.5, -1, NULL, 0);
  pBox* conv1_out = doConv(&p, rgb, w, 32, 3, 3, 1, 0);
  pBox* pooling1_out = maxPooling(dopRelu(&p, conv1_out, 32), w, 3, 2);
  pBox* conv2_out = doConv(&p, pooling1_out, w, 64, 32, 3, 1, 0);
  pBox* pooling2_out = maxPooling(dopRelu(&p, conv2_out, 64), w, 3, 2);
  pBox* conv3_out = doConv(&p, pooling2_out, w, 64, 64, 3, 1, 0);
  pBox* pooling3_out = maxPooling(dopRelu(&p, conv3_out, 64), w, 2, 2);
  pBox* conv4_out = doConv(&p, pooling3_out, w, 128, 64, 2, 1, 0);
  pBox* fc5_out = doFc(&p, dopRelu(&p, conv4_out, 128), w, 256, 1152, 1, 1, 0);
  pBox* score_ = softmax(doFc(&p, dopRelu(&p, fc5_out, 256), w+1, 2, 256, 1, 1, 0));
  pBox* location_ = doFc(&p, fc5_out, w+2, 4, 256, 1, 1, 0);
  pBox* keyPoint_ = doFc(&p, fc5_out, w, 10, 256, 1, 1, 0);
  float Othreshold = 0.8;

  ASSERT((p-Onet_data)==countof(Onet_data));
  umem_getu(score_->data);
  umem_getu(location_->data);
  umem_getu(keyPoint_->data);
  {
    int j, f, n = image->f;
    
    for (f = 0; f < n; ++f, ++it) {
      const float* scorep = pBox_datapf(score_, f);
      
      if (scorep[1] > Othreshold) {
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
  Blob_frees2(w);
  return 0;
}

//    Pitch = eav[0]
//    Yaw = eav[1]
//    Roll  = eav[2]
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
resize1(const img_t* src, img_t* dst, CSize dsize, double fx CC_DEFAULT(0),
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

static int mtcnn_findFace(const img_t* image, Bbox* out, int maxout, int minsize CC_DEFAULT(60))
{
  int i, j = 0, count = 0;
//MEM_CHECK_BEGIN;
  const float nms_threshold[3] = {0.7, 0.7, 0.7};
  int row = image->h, col = image->w;
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
  float simpleFace_nms_threshold = 0.5;

  minl *= m;

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
    resize1(image, reImage, cSize(changedW, changedH), 0, 0, CC_INTER_LINEAR);
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
    resize1(image, reImage, cSize(changedW, changedH), 0, 0, CC_INTER_LINEAR);
    tic;
    boundingBox_size = Pnet_run(reImage, scales_[i], box + box_pos, box_size - box_pos);
    toc;
    //boundingBox_size = nms(box + box_pos, boundingBox_size, s->simpleFace_->nms_threshold, RO_Union);
    box_pos += boundingBox_size;
    //imshow_(reImage);WaitKey(-1);
  }

  count = nms(box, box_pos, simpleFace_nms_threshold, RO_Union);

#if 0
  if (1) {
    img_t im[1] = {0};
    int num;
    imclone2(image, im);
    
    for (i = 0; i < count; ++i) {
      Bbox* it = box + i;
      printf("(%3d %3d %3d %3d)\n", (int)it->y1, (int)it->x1, (int)(it->y2 - it->y1), (int)(it->x2 - it->x1));
      cvRectangle(im, cPoint(it->y1, it->x1), cPoint(it->y2, it->x2), cScalar(0, 0, 255, 0), 2, 8, 0);
      
      for (num = 0; num < 5; num++) {
        cvCircle(im, cPoint((int) it->ppoint[num], (int) it->ppoint[num + 5]), 3, cScalar(0, 255, 255, 0), -1, CC_AA, 0);
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
    count = nms(box, count, nms_threshold[0], RO_Union);
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

      resize1(temp, secImage, cSize(24, 24), 0, 0, CC_INTER_LINEAR);
      tic;
      Rnet_run(secImage, it);
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
      resize1(temp, frame, cSize(24, 24), 0, 0, CC_INTER_LINEAR);
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


    if (count > 0) {
      count = nms(box, count, nms_threshold[1], RO_Union);
      refineAndSquareBbox(box, count, image->rows, image->cols);
      //third stage

#if 1
//#pragma omp parallel for
      for (i = 0; i < count; ++i) {
        Bbox* it = box + i;
        img_t *thirdImage = im;
        img_t temp[1] = {0};
        cvGetSubRect1(image, temp, it->y1, it->x1, it->y2 - it->y1, it->x2 - it->x1);

        resize1(temp, thirdImage, cSize(48, 48), 0, 0, CC_INTER_LINEAR);
        ASSERT(it->exist);
        tic;
        Onet_run(thirdImage, it);
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
        resize1(temp, frame, cSize(48, 48), 0, 0, CC_INTER_LINEAR);
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
        count = nms(box, count, nms_threshold[2], RO_Min);
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
