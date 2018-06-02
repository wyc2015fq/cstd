
#include "cstd.h"
#include "cfile.h"
#include <direct.h>
#include "ui/window.inl"
#include "cnnff1.inl"
#include "cmath.h"


#include "loadmat.inl"
#include "cnntrain.inl"

int test_cnntrain() {
  buf_t bf[1] = {0};
  img_t im[10] = {0};
  int i=0;
  img_t* train_x = im+i++;
  img_t* train_y = im+i++;
  cnntrain_t cnn[1] = {0};
  double alpha = 1; //%学习率
  int batchsize = 100; //%1个batch中含有50个样本，
  //%CNN训练时，每处理一个batch，就会更新一次参数，
  //%所以batchsize值偏小，会使得参数进行多次更新，性能会上升（萝卜验证test集上的识别错误率降低）
  int numepochs = 50;
  
  _chdir("E:/code/cstd/src/lobo/DeepLearnToolbox-master - 总结/tests");
  buf_load("cnnsetup.dat", bf);
  cnnload(bf->data, cnn);
  loadmat_fromfile("train_x_f8.dat", 8, train_x);
  loadmat_fromfile("train_y_f8.dat", 8, train_y);
  //imshowmat_f8(train_x);cvWaitKey(-1);
  cnntrain(cnn, train_x, train_y, alpha, batchsize, numepochs);
  bffrees(bf);
  imfrees2(im);
  return 0;
}
void set_console_color(unsigned short color_index)
{
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_index);
}

int main1()
{
  set_console_color(6);
  printf("%s", "hello");
  set_console_color(9);
  printf("%s\n", " world");
  
  return 0;
}
//时间8.071140/(10000个字符) err = 146个
int test_DeepLearn() {
  buf_t bf[4] = {0};
  buf_t* test_x = bf+1;
  buf_t* test_y = bf+2;
  buf_t* cnn = bf+3;
  int i, err=0;
  main1();
  test_cnntrain();
  bfsetsize(bf, 1000*1000*10);
  _chdir("E:/code/cstd/src/lobo/DeepLearnToolbox-master - 总结/tests");
  buf_load("test_x.dat", test_x);
  buf_load("test_y.dat", test_y);
  buf_load("cnn_30000_6c_2s_12c_2s_100epoch.dat", cnn);
  buf_load("cnn_6c_2s_12c_2s_120c_84f_100epoch.dat", cnn);
  //cvShowMat("test_y", "%d", 100, 1, test_y->data, 4, 4, -1);
  //cvShowImage("test_x", 28, 28, test_x->data+(28*28)*1, 28, 1);
  {
    int* y1 = (int*)test_y->data;
    int n = 10000;
    {utime_start(_start_time);
    for (i=0; i<n; ++i) {
      uchar* data = test_x->data + i*28*28;
      buf_t bf1 = *bf;
      int y;
      //cvShowImage("test_x", 28, 28, data, 28, 1); cvWaitKey(-1);
      cnnff1(cnn->data, 28, 28, data, &y, &bf1);
      //printf("%d %d\n", y[i], y1[i]);
      if (y!=y1[i]) {
        ++err;
      }
    }
    printf("时间%f秒 (%d个字符) err = %d个\n", utime_elapsed(_start_time), n, err);}
  }
  cvWaitKey(-1);
  bffree(test_x);
  bffree(test_y);
  bffree(cnn);
  bffree(bf);
  return 0;
}
