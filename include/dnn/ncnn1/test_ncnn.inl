

#include "cstd.h"
#include "test_mnist.inl"

int test_ncnn() {
  sys_chdir("F:/dataset/mnist");
  img_t im[1] = {0};
	//int testLabel=read_lable("test-labels.idx1-ubyte");
	//img_t testImg[1];
  read_images("train-images.idx3-ubyte", im);
  imshow_(im);
  imfree(im);
  WaitKey(-1);
  return 0;
}
