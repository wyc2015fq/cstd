
#include "cfile.h"
#include "net.inl"
#include "mnist.inl"
#include "codec/lz4.inl"
#include "tools/convert_mnist_data.inl"
//#include "test/test_softmax_layer.inl"
//#include "test/test_inner_product_layer.inl"
//#include "test/test_conv_layer.inl"
//#include "test/test_pooling_layer.inl"
//#include "test/test_relu_layer.inl"
#include "test/test_tanh_layer.inl"
#include "test_linear_regression.inl"

int net_print(Net* net) {
  int i, j;
  for (i=0; i<net->layers_size; ++i) {
    Layer* layer = net->layers+i;
    printf("%8s %d %d %d", layer->name, layer->bottoms_size, layer->tops_size, layer->weights_size);
    for (j=0; j<LayerArgCount; ++j) {
      enum LayerArg arg = (enum LayerArg)j;
      if (layer->x[arg].i>0) {
        switch (ifArgType[arg]) {
        case IF_Index:
        case IF_WIndex:
        case IF_Int:
          printf(" %s=%d", layerarg_name[arg], layer->x[arg].i);
          break;
        case IF_Float:
          printf(" %s=%f", layerarg_name[arg], layer->x[arg].f);
          break;
        case IF_Enum:
          printf(" %s=%s", layerarg_name[arg], method_name[layer->x[arg].e]);
          break;
        default:
          ASSERT(0);
          break;
        }
      }
    }
    printf("\n");
  }
  for (i=0; i<net->blobs_size; ++i) {
    Blob* blob = net->blobs+i;
    printf("%16s %3d %3d %3d %3d\n", blob->name, blob->data->w, blob->data->h, pBox_channels(blob->data), blob->data->f);
  }
  return 0;
}

static int shuffle_perm(int n, int* perm) {
  int t1, i;
  for (i=0; i<n; ++i) {
    perm[i] = i;
  }
  SHUFFLE(NULL, perm, n, t1);
	return 0;
}
static img_t* imbatch(const img_t* src, img_t* dst, const int* perm, int batch) {
  int i, size = src->w * src->h * src->c, c = src->c;
  imsetsize(dst, src->h, src->w, c, batch);
  dst->t = src->t;
  for (i=0; i<batch; ++i) {
    //int j = i + rng_int32(src->f-i);
    int j = perm[i];
    memcpy(dst->data + i*size, src->data + j*size, size);
  }
  return dst;
}

int test_mnist() {
  sys_chdir("E:/MNIST");
  img_t im1[10] = {0};
  img_t* im = im1;
  //test_linear_regression();
	//int testLabel=read_lable("test-labels.idx1-ubyte");
	//img_t testImg[1];
  //convert_mnist_data("train-images.idx3-ubyte", "train-labels.idx1-ubyte", "lmdb_out");
	img_t* trainLabel=read_lable("train-labels.idx1-ubyte", im++);
	img_t* trainImg=read_images("train-images.idx3-ubyte", im++);
	img_t* testLabel=read_lable("t10k-labels.idx1-ubyte", im++);
	img_t* testImg=read_images("t10k-images.idx3-ubyte", im++);
	ISIZE inputSize=iSIZE(trainImg->w, trainImg->h);
	int outSize = 10;
  Net net[1] = {0};
  int i;
  int w1=0, b1=0, w2=0, b2=0, w3=0, b3=0, w4=0, b4=0;
  //trainLabel->f=trainImg->f=100;
  int batch = 50;
  int data_x = net_addBlob(net, "data_x", inputSize.w, inputSize.h, 1, batch);
  int data_y = net_addBlob(net, "data_y", outSize, 1, 1, batch);
#if 1
  enum Method poolType;
  poolType = Method_MAX;
  poolType = Method_AVE;
  int conv1 = net_sigmoid(net, net_conv2d(net, data_x, &w1, &b1, 5, 12));
  int pool1 = net_pool(net, conv1, poolType, 2, 2, 2);
  int conv2 = net_sigmoid(net, net_conv2d(net, pool1, &w2, &b2, 5, 32));
  int pool2 = net_pool(net, conv2, poolType, 2, 2, 2);
  int output = net_sigmoid(net, net_matmul(net, pool2, &w3, &b3, outSize));
  //int output = net_softmax(net, fc1);
#else
  //int conv1 = net_conv2d(net, data_x, &w1, &b1, 7, 6);
  int matmul1 = net_matmul(net, data_x, &w1, &b1, 1024);
  int fc1 = net_matmul(net, matmul1, &w2, &b2, outSize);
  int output = net_softmax(net, fc1);
#endif
  if (w1>0) net_rng_xavier(net, w1);
  if (b1>0) net_constant(net, b1, 0.);
  if (w2>0) net_rng_xavier(net, w2);
  if (b2>0) net_constant(net, b2, 0.);
  if (w3>0) net_rng_xavier(net, w3);
  if (b3>0) net_constant(net, b3, 0.);

  net_print(net);
  int* perm = NULL;
  MYREALLOC(perm, trainImg->f);
  Dtype rate = 0.1;
  img_t* batchImg = im++;
  img_t* batchLabel = im++;
  //net->debug_info = 1;
  //训练
  for (i=0; i<10000; ++i) { //训练1000次
    shuffle_perm(trainImg->f, perm);
    imbatch(trainImg, batchImg, perm, batch);
    imbatch(trainLabel, batchLabel, perm, batch);
    if (0) {
      for (int j=0; j<batch; ++j) {
        printf("%d\n", trainLabel->data[j]);
        imshow_f(trainImg, j);
        WaitKey(-1);
      }
    }
    onehot(batchLabel, outSize, net->blobs[data_y].data);
    image2Matrix(batchImg, net->blobs[data_x].data, 1, 1./127.0, -1);
    //_,loss_value = sess.run([train_step,loss],feed_dict={xs:x_data,ys:y_data}) //进行梯度下降运算，并计算每一步的损失
    Dtype loss = net_trainonce(net, output, data_y, rate, 1);
    Dtype accuracy = net_accuracy(net, output, data_y);
    if (rate>0.0001 && loss<1 && loss>=0) {
      Dtype t = pow(loss, 0.1);
      t = MAX(0.95, t);
      //rate *= t;
    }
    printf("%d rate=%f loss=%f accuracy=%f\n", i, rate, loss, accuracy);
    //if(i%50==49)
  }
  if(1) {
    Dtype err=0;
    onehot(testLabel, outSize, net->blobs[data_y].data);
    image2Matrix(testImg, net->blobs[data_x].data, 1, 1./127.5, -1, NULL, 0);
    net_Forward(net);
    err = net_accuracy(net, output, data_y);
    printf("  err=%f\n", err);
  }

  FREE(perm);
  //imshow_(im1);
  WaitKey(-1);
  imfrees2(im1);
  return 0;
}

