
#include "cstd.h"
#include "cfile.h"
#include "tpool.inl"
#include "img/imgopt.inl"
//#include "cv1/meansdv.inl"
#include "loadmat.inl"
//#include "math/rand.inl"
#include "math/rand.inl"
#include "randfill.inl"
#include "blas.inl"
#include "nn.inl"

int test_nn()
{
  img_t im[10] = {0};
  int i = 0;
  img_t* test_x = im + i++;
  img_t* test_y = im + i++;
  img_t* train_x = im + i++;
  img_t* train_y = im + i++;
  //test_mt19937ar();
  sys_chdir("C:/Program Files (x86)/MATLAB71/work");
  loadmat_fromfile("test_x.dat", 1, test_x);
  loadmat_fromfile("test_y.dat", 1, test_y);
  loadmat_fromfile("train_x.dat", 1, train_x);
  loadmat_fromfile("train_y.dat", 1, train_y);

  if (0) {
    img_t im1[1] = {0};
    i = 2;
    IMINIT(im1, 28, 28, test_x->tt.data + 28 * 28 * i, 28 * test_x->c, test_x->c, 1);
    printf("%d\n", test_y->tt.data[i]);
    imshowmat_u1(test_y);
    imshow(im1);
    cvWaitKey(-1);
  }

  //{utime_start(_start_time);
  imdouble(train_x, CV_1U, train_x, 1 / 255., 0);
  imdouble(train_y, CV_1U, train_y, 1, 0);
  imdouble(test_x, CV_1U, test_x, 1 / 255., 0);
  imdouble(test_y, CV_1U, test_y, 1, 0);

  //printf("%f\n", utime_elapsed(_start_time));}
  if (0) {
    img_t im1[1] = {0};
    i = 2;
    IMINIT(im1, 28, 28, test_x->tt.data + 28 * 28 * i * test_x->c, 28 * test_x->c, test_x->c, 1);
    printf("%d\n", test_y->tt.data[i]);
    imshowmat_f8(test_y);
    imshowfmt(im1, PF_64bppGrayDouble);
    cvWaitKey(-1);
  }

  {
    nn_t nn[1] = {0};
    int size[] = {784, 100, 10};
    img_t* mu = im + i++;
    img_t* sigma = im + i++;
    mtsrand(10);
    imzscore(train_x, train_x, mu, sigma);
    imnormalize(test_x, mu, sigma, test_x);
    nnsetup(nn, size, countof(size));
    nntrain_opt_t opts[1] = {0};
    //nn->weightPenaltyL2 = 1e-4;  //  L2 weight decay
    opts->numepochs =  1;        //  Number of full sweeps through data
    opts->batchsize = 100;       //  Take a mean gradient step over this many samples

    nntrain(nn, train_x, train_y, opts, NULL, NULL, NULL);
  }

  imfrees(im, 10);
  return 0;
}

//#include "test_matmul2.inl"
int test_DeepLearn()
{
  //test_tpool();
  //test_matmul2();
  test_nn();
  return 0;
}
