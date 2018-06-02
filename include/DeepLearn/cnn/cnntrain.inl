
typedef struct cnn_layer_t {
  img_t a[1];
  img_t b[1];
  img_t k[1];
  img_t w[1];
  img_t z[1];
  img_t av[1];
  int outputmaps;
  int type;
  int kernelsize;
  int scale;
} cnn_layer_t;

typedef struct cnntrain_t {
  cnn_layer_t* layers;
  int n;
  img_t ffb[1];
  img_t ffW[1];
  img_t rL[1];
  img_t fv[1];
  img_t z[1];
  img_t o[1];
} cnntrain_t;

#include "cnnload.inl"
#include "cnnsetup.inl"
#include "cnnff.inl"

int cnntrain(cnntrain_t* net, const img_t* x, const img_t* y, double alpha, int batchsize, int numepochs) {
  int i=0, j=0, k, l, m = x->f;
  int numbatches = m / batchsize;
  img_t im[10] = {0};
  img_t* kk = im+i++;
  img_t* batch_x = im+i++;
  img_t* batch_y = im+i++;
    //imshowmat_f8(train_y); cvWaitKey(-1);
  for (i = 0; i<numepochs; ++i) {
    //printf("epoch %d/%d\n", inumepochs);
    loadmat_fromfile("kk_u4.dat", 4, kk);
    //imshowmat_u4(kk); cvWaitKey(-1);
    for (l = 0; l<numbatches; ++l) {
      int* k1 = kk->tt.s4 + l*batchsize;
      //batch_x = x(:, :, kk((l - 1) * batchsize + 1 : l * batchsize));
      //batch_y = y(:,    kk((l - 1) * batchsize + 1 : l * batchsize));
      imsetsize(batch_x, x->h, x->w, x->c, batchsize);
      imsetsize(batch_y, batchsize, y->w, x->c, 1);
      for (j=0; j<batchsize; ++j) {
        int sz;
        k = k1[j]-1;
        sz = x->s*x->h;
        memcpy(batch_x->tt.data+sz*j, x->tt.data + sz*k, sz);
        sz = y->w*y->c;
        memcpy(batch_y->tt.data+sz*j, y->tt.data + sz*k, sz);
      }
      //imshowmat_f8(batch_x); imshowmat_f8(batch_y); cvWaitKey(-1);
      cnnff(net, batch_x);
    }
  }
  return 0;
}
