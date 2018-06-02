
#if 0
static Dtype CenterLossLayer1(enum LayerOp op, struct Net* net, struct Layer* layer) {
  struct Blob* prev = IBLOB(0);
  struct Blob* prev1 = IBLOB(1);
  struct Blob* center = IBLOB(2);
  //struct Blob* label = OBLOB(0);
  int i, count = Data_count(prev->size);
  const Dtype alpha = FARG(alpha);
  const Dtype loss_weight = FARG(loss_weight);
  int channels = prev->c;
  int num = prev->n;
  int clusterNum = center->n;
  Dtype loss = 0;
  ASSERT(prev->h==1);
  ASSERT(prev->w==1);
  
  switch (op) {
  case Op_Forward:
    //label->size = prev->size;
    {
      const Dtype* labelData = setDataZero(label);
      const Dtype *prevData = getData(prev);
      Dtype *center_loss_data = setBuffSize(prev, 0, dataSize(num, channels, 1, 1));
      int *center_update_count = NULL;
      Dtype* centerData = setData(center);
      Dtype* centerDiff = setDiff(center);
      MYREALLOC(center_update_count, clusterNum);
      ASSERT(center->c==channels);
      blas_sset(clusterNum * channels, Dtype(0.0), centerData);
      for(i = 0; i < clusterNum; ++i){
        center_update_count[i] = 1;
      }
      for(i = 0; i < num; ++i) {
        int targetLabel = label[i];
        Dtype* center_loss_data_i = center_loss_data + i * channels;
        blas_ssub(channels, prevData + i * channels, centerData + targetLabel * channels, center_loss_data_i);
        // store the update loss and number
        blas_sadd(channels, center_loss_data_i, centerDiff + targetLabel * channels, centerDiff + targetLabel * channels);
        center_update_count[targetLabel]++;
        loss += blas_sdot(channels, center_loss_data_i, center_loss_data_i);
      }
      loss *= loss_weight / (2.0 * num);
      // update center loss.
      for(int i = 0; i < clusterNum; ++i) {
        Dtype scale = -alpha * loss_weight / Dtype(center_update_count[i]);
        blas_sscal(channels, scale, centerDiff + i * channels);
      }
      Blob_Update(center);
      //center.Update();
      FREE(center_update_count);
    }
    break;
  case Op_Backward:
    {
      Dtype* prevDiff = setDiffZero(prev);
      const Dtype *center_loss_data = getBuff(prev, 0);
      //blas_scopy(num * channels, center_loss_data, prevDiff);
      //blas_sscal(num * channels, loss_weight, prevDiff);
      blas_saxpy(num * channels, loss_weight, center_loss_data, prevDiff);
    }
    break;
  default:
    break;
  }
  return loss;
}
#endif

static Dtype CenterLossLayer(enum LayerOp op, struct Net* net, struct Layer* layer) {
  struct Blob* prev = IBLOB(0);
  struct Blob* label = IBLOB(1);
  struct Blob* center = WBLOB(0);
  int i, count = Data_count(prev->size);
  enum Method distance_type = MARG(distance_type);
  int N = center->size.n;
  //const int axis = IARG(axis);
  // Dimensions starting from "axis" are "flattened" into a single
  // length K vector. For example, if prev's shape is (N, C, H, W),
  // and axis == 1, N inner products with dimension CHW are performed.
  int K = Data_2DSize(prev->size);
  // Check if we need to set up the weights
  int M = prev->size.n;
  Dtype loss = 0;
  //ASSERT(layer->i_size==3);
  ASSERT(Data_count(center->size)==(N*K));
  if (distance_type!=M_L1) {
    distance_type = M_L2;
  }

  switch (op) {
  case Op_Forward:
    {
      const Dtype* prevData = getData(prev);
      const int* labelData = (int*)getData(label);
      Dtype* distance = reset(&label->bufs[0], prev->size);
      const Dtype* centerData = getData(center);
      Dtype dot;
      // the i-th 
      for (i = 0; i < M; i++) {
        const int j = (int)(labelData[i]);
        // D(i,:) = X(i,:) - C(y(i),:)
        blas_ssub(K, prevData + i * K, centerData + j * K, distance + i * K);
      }
      if (distance_type == M_L1) {
        dot = blas_sasum(M * K, distance);
        loss = dot / M;
        blas_ssign(M * K, distance, distance);
      }
      else if (distance_type == M_L2) {
        dot = blas_sdot(M * K, distance, distance);
        loss = dot / M / (Dtype)(2);
      }
      else {
        logCritical("distance_type must be L1 or L2!");
      }
    }
    break;
  case Op_Backward:
    {
      const int* labelData = (int*)getData(label);
      const Dtype* prevData = getData(prev);
      Dtype* prevDiff = prev->diff;
      const Dtype* distance = label->bufs[0];
      Dtype* varsumData = MALLOC(Dtype, N * K);
      int* count_vec = MALLOC(int, N);
      int m;
      // Gradient with respect to centers
      
      Dtype* centerDiff = setDiffZero(center);
      int* count_data = count_vec;
      
      // \sum_{y_i==j}
      blas_sset(N * K, (Dtype)0., varsumData);
      blas_iset(N, 0, count_vec);
      
      for (m = 0; m < M; m++) {
        const int j = (int)(labelData[m]);
        blas_ssub(K, varsumData + j * K, distance + m * K, varsumData + j * K);
        count_data[j]++;
      }
      for (m = 0; m < M; m++) {
        const int j = (int)(labelData[m]);
        blas_saxpby(K, (Dtype)1. / (count_data[j] + (Dtype)1.), varsumData + j * K, (Dtype)1., centerDiff + j * K);
      }
    
      // Gradient with respect to bottom data
      if (1) {
        blas_scopy(M * K, distance, prevDiff);
        blas_sscal(M * K, 1.f / M, prevDiff);
      }
      if (0) {
        logCritical("%s Layer cannot backpropagate to label inputs.", layer->type);
      }
      FREE(varsumData);
      FREE(count_vec);
    }
    break;
  default:
    break;
  }
  loss *= FARG(alpha);
  return loss;
}
static void net_centerloss(struct Net* net, int input, int label, int* center, int classes, float alpha)
{
  const char* type = "centerloss";
  struct Layer* layer = net_addLayer(net, 2, 0, 1, CenterLossLayer, type);
  DataSize size = BLOB(input)->size;
  int dim_in = Data_2DSize(size);
  FARG(alpha) = alpha;
  MARG(distance_type) = M_L2;
  LIBLOB(0) = input;
  LIBLOB(1) = label;
  LWBLOB(0) = *center = net_addBlob(net, dataSize(classes, dim_in, 1, 1), type, "b");
  Blob_init_const(BLOB(*center), 0.f);
  return ;
}
