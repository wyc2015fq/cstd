
#define imset_vec_f8(im, h, w, f, vec)  imset_vec(im, h, w, sizeof(double), f, vec)

int imset_vec(img_t * im, int h, int w, int c, int f, const void* vec) {
  imsetsize(im, h, w, c, f);
  if (vec) {
    memcpy(im->tt.data, vec, h*w*c*f);
  }
  return 0;
}

int cnnload(const void* bufptr, cnntrain_t* net) {
  char* p = (char*)bufptr;
  int nclass = *getarg(int, p);
  int tt = *getarg(int, p);
  const double* ffb = getargs(double, p, nclass);
  const double* ffW = getargs(double, p, nclass*tt);
  int l, inputmaps=1, n = *getarg(int, p);
  net->layers = MALLOC(cnn_layer_t, n);
  MEMSET(net->layers, 0, n);
  net->n = n;
  imset_vec_f8(net->ffb, nclass, 1, 1, ffb);
  imset_vec_f8(net->ffW, nclass, tt, 1, ffW);
  //imshowmat_f8(net->ffW);cvWaitKey(-1);
  for (l=1; l<n; ++l) {
    char type = *getarg(char, p);
    net->layers[l].type = type;
    if ('c'==type) {
      int outputmaps = *getarg(int, p);
      int kernelsize = *getarg(int, p);
      const double* k = getargs(double, p, inputmaps*outputmaps*kernelsize*kernelsize);
      const double* b = getargs(double, p, outputmaps);
      imset_vec_f8(net->layers[l].k, kernelsize, kernelsize, inputmaps*outputmaps, k);
      //imshowmat_f8_f(net->layers[l].k, 1);cvWaitKey(-1);
      imset_vec_f8(net->layers[l].b, outputmaps, 1, 1, b);
      //imshowmat_f8(net->layers[l].k);cvWaitKey(-1);
      net->layers[l].outputmaps = outputmaps;
      net->layers[l].kernelsize = kernelsize;
      inputmaps = outputmaps;
    } else if ('s'==type) {
      net->layers[l].scale = *getarg(int, p);
      net->layers[l].outputmaps = inputmaps; 
    } else if ('f'==type) {
      int nrow = *getarg(int, p);
      int ncol = *getarg(int, p);
      double* b = getargs(double, p, nrow);
      double* weights = getargs(double, p, nrow*ncol);
      imset_vec_f8(net->layers[l].b, nrow, 1, 1, b);
      imset_vec_f8(net->layers[l].w, nrow, ncol, 1, weights);
      net->layers[l].outputmaps = 1; 
      //imshowmat_f8(net->layers[l].w);cvWaitKey(-1);
    }
  }
  return 0;
}
