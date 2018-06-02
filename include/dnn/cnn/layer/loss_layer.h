

const float kLOG_THRESHOLD = 1e-20;

#define ASSERT_SIZE_EQ(a, b)  ASSERT((a).n==(b).n && (a).c==(b).c && (a).h==(b).h && (a).w==(b).w)

#define net_CrossEntropyLoss(net, input, prob) net_loss(net, input, prob, CrossEntropyLoss, "CrossEntropy")
static Dtype CrossEntropyLoss(enum LayerOp op, struct Net* net, struct Layer* layer)
{
  struct Blob* prev = IBLOB(0);
  struct Blob* prob = IBLOB(1);
  int i;
  Dtype loss = 0;
  bool onehot = prob->size.c==prev->size.n;
  float scala = 1;//1.f / (prev->size.n);
  
  if (onehot) {
    ASSERT_SIZE_EQ(prev->size, prob->size);
    switch (op) {
    case Op_Forward:
      {
        const float* prevData = getData(prev);
        const float* probData = getData(prob);
        for (i = 0; i < Data_total(prev->size); i++) {
          const float curLoss = -probData[i] * logf(MAX(prevData[i], FLT_MIN));
          loss += curLoss;
        }
        loss *= scala;
      }
      break;
    case Op_Backward:
      {
        float* prevDiff = setDiffZero(prev);
        const float* probData = getData(prob);
        const float* prevData = getData(prev);
        ASSERT_SIZE_EQ(prev->size, prob->size);
        for (i = 0; i < Data_total(prev->size); i++) {
          prevDiff[i] -= scala * probData[i] / prevData[i];
        }
      }
      break;
    default:
      break;
    }
  } else {
    struct Blob* label = IBLOB(1);
    int outer_num = dataCount(prev->size, 0, 1);
    int inner_num = dataCount(prev->size, 1, 4);
    switch (op) {
    case Op_Forward:
      {
        const float* prevData = getData(prev);
        const int* labelData = (int*)getData(label);
        for (i = 0; i < outer_num; i++) {
          int k = i * inner_num + labelData[i];
          const float curLoss = -logf(MAX(prevData[k], FLT_MIN));
          loss += curLoss;
        }
        
        loss *= scala;
      }
      break;
    case Op_Backward:
      {
        float* prevDiff = setDiffZero(prev);
        const int* labelData = (int*)getData(label);
        const float* prevData = getData(prev);
        for (i = 0; i < Data_total(prev->size); i++) {
          prevDiff[i] = 0;
        }
        for (i = 0; i < outer_num; i++) {
          int k = i * inner_num + labelData[i];
          prevDiff[k] -= scala / prevData[k];
        }
      }
      break;
    default:
      break;
    }
  }
  return loss;
}

#if 0
static int EuclideanLossLayer(enum LayerOp op, struct Net* net, struct Layer* layer) {
  Blob* prev = IBLOB(0);
  Blob* prob = IBLOB(1);
  int count = Blob_count(prev->size);
  Dtype* diff_ = NULL;
  Dtype loss = 0;

  switch (op) {
  case LO_Init:
    break;
  case Op_Forward:
    {
      const float* prevData = getData(prev);
      const float* probData = getData(prob);
      diff_ = setBuff(next, 0);
      //diff_ = bottom[0] - bottom[1]
      //dot = ||diff_||^2
      Dtype dot = 0;
      blas_dot(count, diff_, diff_);
      loss = dot / (prevData->f * (Dtype)(2));
    }
    break;
  case Op_Backward:
    {
      const DataSize probSize = prob->size;
      const DataSize diffSize = prev->size;
      float* prevDiff0 = setDiffZero(prev);
      diff_ = Blob_datap(next->im);
      for (i = 0; i < 2; ++i) {
        const Dtype sign = (i == 0) ? 1 : -1;
        const Dtype alpha = sign * nextDiff[0] / prevData->f;
        blas_axpby(count, alpha, diff_, (Dtype)(0), Blob_datap(LBLOB(i)->diff));  // b
      }
    }
    break;
  }
  return loss;
}
#endif

#define net_MSELoss(net, input, prob) net_loss(net, input, prob, MSELoss, "MSE")
static Dtype MSELoss(enum LayerOp op, struct Net* net, struct Layer* layer)
{
  struct Blob* prev = IBLOB(0);
  struct Blob* prob = layer->i_size > 1 ? IBLOB(1) : NULL;
  //struct Blob* next = OBLOB(0);
  Dtype loss = 0;
  int i;
  if (prob) {
    assert(Data_count(prev->size)==Data_count(prob->size));
  }
  switch (op) {
  case Op_Forward:
    if (prob) {
      const float* probData = getData(prob);
      const float* prevData = getData(prev);
      for (i = 0; i < Data_total(prev->size); i++) {
        float t = prevData[i] - probData[i];
        const float curLoss = t * t;
        loss += curLoss;
      }
    } else {
      const float* prevData = getData(prev);
      for (i = 0; i < Data_total(prev->size); i++) {
        const float curLoss = (prevData[i]) * (prevData[i]);
        loss += curLoss;
      }
    }
    loss = loss / (prev->size.n);
  break;
  case Op_Backward:
    if (prob) {
      float* prevDiff = setDiffZero(prev);
      float scala = 2.f / prev->size.n;
      const float* probData = getData(prob);
      const float* prevData = getData(prev);
      for (i = 0; i < Data_total(prev->size); i++) {
        prevDiff[i] += scala * (prevData[i] - probData[i]);
      }
    } else {
      const float* prevData = getData(prev);
      float* prevDiff = setDiffZero(prev);
      float scala = 2.f / prev->size.n;
      for (i = 0; i < Data_total(prev->size); i++) {
        prevDiff[i] += scala * prevData[i];
      }
    }
    break;
  default:
    break;
  }
  return loss;
}
// test_euclidean_loss_layer
static void net_loss(struct Net* net, int input, int label, LayerFun run)
{
  //struct Layer* layer = net_addLayer(net, 2, 1, 0, run, type);
  const char* type = "loss";
  struct Layer* layer = net_addLayer(net, 2, 0, 0, run, type);
  LIBLOB(0) = input;
  LIBLOB(1) = label;
  return ;//LOBLOB(0) = net_addBlob(net, dataSize(1, 1, 1, 1), type, "o");
}
#if 0
static float net_get_loss(struct Net* net)
{
  float loss = 0;
  struct Layer* layer = net->layers;
  layer = net->lossFunctor;
  Layer_forward(net->lossFunctor);
  assert(Data_4DEqu(OBLOB(0)->size, dataSize(1, 1, 1, 1)));
  loss = OBLOB(0)->data[0];
  return loss;
}
#endif




