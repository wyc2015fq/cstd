
#include "img/imgopt.inl"
int imset_f8(img_t* im, double x) {
  int i, j;
  for (i=0; i<im->h*im->f; ++i) {
    double* im1 = (double*)(im->tt.data + im->s*i);
    for (j=0; j<im->w; ++j) {
      im1[j] = x;
    }
  }
  return 0;
}

int cnnff(cnntrain_t* net, img_t* im) {
  int l, i, j, inputmaps = 1, f = im->f;
  img_t im1[10] = {0};
  int n = net->n;
  cnn_layer_t* cnnl = net->layers;
  imclone2(im, cnnl[0].a);
  for (l=1; l<n; ++l) {
    if ('c'==cnnl[l].type) {
      img_t* a = cnnl[l-1].a;
      img_t* z = cnnl[l].a;
      img_t* b = cnnl[l].b;
      img_t* k = cnnl[l].k;
      int kh = cnnl[l].kernelsize;
      imsetsize_f8(z, a->h-kh+1, a->w-kh+1, cnnl[l].outputmaps*f);
      imset_f8(z, 0);
      for (j=0; j<cnnl[l].outputmaps; ++j) {
        double* z1 = (double*)(z->tt.data + z->h*z->s*f*j);
        double bj = b->tt.f8[j];
        for (i=0; i<inputmaps; ++i) {
          double* a1 = (double*)(a->tt.data + a->h*a->s*f*i);
          double* k1 = (double*)(k->tt.data + k->s*k->h*(j*inputmaps+i));
          convn_valid_f64(a->h, a->w, a1, a->w, kh, kh, k1, z1, z->w, f);
          if (l>2 && i>0) {
            //imshowmat_f8_f(a, 0);
            //imshowmat_f8_f(z, 0); cvWaitKey(-1);
          }
        }
        //imshowmat_f8_f(z, 0); cvWaitKey(-1);
        for (i=0; i<z->h*z->w*f; ++i) {
          z1[i] = sigm(z1[i]+bj);
        }
      }
      //if (l>2 && 0) { imshowmat_f8_f(a, 0); imshowmat_f8_f(z, 0); cvWaitKey(-1); }
      inputmaps = cnnl[l].outputmaps;
    } else if ('s'==cnnl[l].type) {
      img_t* a = cnnl[l-1].a+0;
      img_t* z = cnnl[l].a;
      int scale = cnnl[l].scale;
      imsetsize_f8(z, a->h/2, a->w/2, inputmaps*f);
      imset_f8(z, 0);
      subsampling_2x2_f64(a->h, a->w, a->tt.f8, a->w, z->tt.f8, z->w, inputmaps*f);
      //imshowmat_f8_f(z, 1); cvWaitKey(-1);
    } else if ('f'==cnnl[l].type) {
      img_t* a = cnnl[l-1].a;
      img_t* av = cnnl[l-1].av;
      img_t* z = cnnl[l].z;
      img_t* a1 = cnnl[l].a;
      img_t* b = cnnl[l].b;
      img_t* w = cnnl[l].w;
      int avw = a->h*a->w*a->f/f;
      //imshowmat_f8_f(w, 0); cvWaitKey(-1);
      imsetsize_f8(av, avw, f, 1);
      memcpy(av->tt.data, a->tt.data, a->h*a->s*a->f);
      //imshowmat_f8_f(av, 0); cvWaitKey(-1);
      imsetsize_f8(z, w->h, f, 1);
      imsetsize_f8(a1, w->h, f, 1);
      mat_mul_AB(z->h, z->w, avw, w->tt.f8, w->w, av->tt.f8, av->w, z->tt.f8, z->w);
      mat_add_row(z->h, z->w, z->tt.f8, z->w, b->tt.f8, z->tt.f8, z->w);
      //imshowmat_f8_f(z, 0); cvWaitKey(-1);
      for (i=0; i<z->h*z->w; ++i) {
        a1->tt.f8[i] = sigm(z->tt.f8[i]);
      }
    } else {
      ASSERT(0);
    }
  }
  {
    img_t* a = cnnl[n-1].a;
    img_t* ffb = net->ffb;
    img_t* ffW = net->ffW;
    img_t* z = net->z;
    imclone2(a, net->fv);
    imsetsize_f8(z, ffW->h, a->w, 1);
    imsetsize_f8(net->o, ffW->h, a->w, 1);
    mat_mul_AB(ffW->h, a->w, ffW->w, ffW->tt.f8, ffW->w, a->tt.f8, a->w, z->tt.f8, z->w);
    mat_add_row(z->h, z->w, z->tt.f8, z->w, ffb->tt.f8, z->tt.f8, z->w);
    for (i=0; i<z->h*z->w; ++i) {
      net->o->tt.f8[i] = sigm(z->tt.f8[i]);
    }
    
    //imshowmat_f8_f(net->o, 0); cvWaitKey(-1);
  }
  imfrees2(im1);
  return 0;
}