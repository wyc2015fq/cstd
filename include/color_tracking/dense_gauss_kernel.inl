int dense_gauss_kernel(double sigma, const img_t* x, const img_t* y, img_t* k_out) {

// k = dense_gauss_kernel(sigma, x, y)
//
// Computes the kernel output for multi-dimensional feature maps x and y
// using a Gaussian kernel with standard deviation sigma.
  img_t im[10] = {0};
  int i=0, j, k;
  img_t* xf = im+i++;
  img_t* yf = im+i++;
  img_t* xyf = im+i++;
  img_t* xy = im+i++;
  img_t* xyc = im+i++;
  double xx=0, yy=0;
  int cn = x->c/sizeof(double);
  imfft2_real_cn(x, CV_DXT_FORWARD, xf);  //x in Fourier domain
  //imshowmat_f8_cn(x, 2); 
  //imshowmat_f8_cn(xf, 0); cvWaitKey(-1);

  //xx = x(:)' * x(:);  //squared norm of x
  for (i=0; i<(int)(x->h*x->w*x->c/sizeof(double)); ++i) {
    xx += x->tt.f8[i] * x->tt.f8[i];
  }
  
  if (y) {  //general case, x and y are different
    imfft2_real_cn(y, CV_DXT_FORWARD, yf);
    //yy = y(:)' * y(:);
    for (i=0; i<(int)(y->h*y->w*y->c/sizeof(double)); ++i) {
      yy += y->tt.f8[i] * y->tt.f8[i];
    }
  } else {
    //auto-correlation of x, avoid repeating a few operations
    //yf = xf;
    imclone2(xf, yf);
    yy = xx;
  }

//cross-correlation term in Fourier domain
  //xyf = xf .* conj(yf);
//xy = real(ifft2(sum(xyf,3)));  //to spatial domain
  imsetsize(xyf, xf->h, xf->w, sizeof(CvComplex64f), 1);
  for (i=0; i<xf->h; ++i) {
    CvComplex64f* xyf1 = (CvComplex64f*)(xyf->tt.data + xyf->s*i);
    const CvComplex64f* xf1 = (const CvComplex64f*)(xf->tt.data + xf->s*i);
    const CvComplex64f* yf1 = (const CvComplex64f*)(yf->tt.data + yf->s*i);
    for (j=0; j<xf->w; ++j) {
      CvComplex64f cc = {0};
      for (k=0; k<cn; ++k) {
        CvComplex64f c = {0}, cy = {0};
        FCOMPLEX_CONJ(cy, yf1[k]);
        FCOMPLEX_MUL(c, xf1[k], cy);
        FCOMPLEX_ADD(cc, cc, c);
      }
      xyf1[j] = cc;
      xf1+=cn;
      yf1+=cn;
    }
  }
  //imshowmat_f8_cn(xyf, 0); cvWaitKey(-1);
  imfft2(xyf, CV_DXT_INV_SCALE, xyc);
  //imshowmat_f8_cn(xyc, 0); cvWaitKey(-1);
  imsetsize(xy, xf->h, xf->w, sizeof(double), 1);
  for (i=0; i<xf->h; ++i) {
    double* xy1 = (double*)(xy->tt.data + xy->s*i);
    const CvComplex64f* xyc1 = (const CvComplex64f*)(xyc->tt.data + xyc->s*i);
    for (j=0; j<xf->w; ++j) {
      xy1[j] = xyc1[j].re;
    }
  }
  //imshowmat_f8_cn(xy, 0); cvWaitKey(-1);

//calculate gaussian response for all positions
//k = exp(-1 / sigma^2 * max(0, (xx + yy - 2 * xy) / numel(x)));
  
  imsetsize(k_out, xf->h, xf->w, sizeof(double), 1);
  for (i=0; i<xf->h; ++i) {
    double* xy1 = (double*)(xy->tt.data + xy->s*i);
    double* k_out1 = (double*)(k_out->tt.data + k_out->s*i);
    int numel_x = x->h*x->w*cn;
    for (j=0; j<xf->w; ++j) {
      double xy2 = 2*xy1[j];
      double t1 = (xx + yy - xy2)/numel_x;
      k_out1[j] = exp((-1 * MAX(0, t1) / (sigma*sigma)));
    }
  }
  //imshowmat_f8_cn(k_out, 0); cvWaitKey(-1);

  imfrees2(im);
  return 0;
}