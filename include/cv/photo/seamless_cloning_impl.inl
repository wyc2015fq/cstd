
#if 0

void scalarProduct(Mat mat, float r, float g, float b)
{
    vector <Mat> channels;
    split(mat,channels);
    multiply(channels[2],r,channels[2]);
    multiply(channels[1],g,channels[1]);
    multiply(channels[0],b,channels[0]);
    merge(channels,mat);
}

void localColorChange(Cloning* s, img_t* I, img_t* mask, img_t* wmask, img_t* cloned, float red_mul=1.0,
                                 float green_mul=1.0, float blue_mul=1.0)
{
    computeDerivatives(I,mask,wmask);

    arrayProduct(patchGradientX,binaryMaskFloat, patchGradientX);
    arrayProduct(patchGradientY,binaryMaskFloat, patchGradientY);
    scalarProduct(patchGradientX,red_mul,green_mul,blue_mul);
    scalarProduct(patchGradientY,red_mul,green_mul,blue_mul);

    evaluate(s, I,wmask,cloned);
}

void illuminationChange(Cloning* s, img_t* I, img_t* mask, img_t* wmask, img_t* cloned, float alpha, float beta)
{
    CC_INSTRUMENT_REGION()

    computeDerivatives(I,mask,wmask);

    arrayProduct(patchGradientX,binaryMaskFloat, patchGradientX);
    arrayProduct(patchGradientY,binaryMaskFloat, patchGradientY);

    Mat mag = Mat(I.size(),CC_32FC3);
    magnitude(patchGradientX,patchGradientY,mag);

    Mat multX, multY, multx_temp, multy_temp;

    multiply(patchGradientX,pow(alpha,beta),multX);
    pow(mag,-1*beta, multx_temp);
    multiply(multX,multx_temp, patchGradientX);
    patchNaNs(patchGradientX);

    multiply(patchGradientY,pow(alpha,beta),multY);
    pow(mag,-1*beta, multy_temp);
    multiply(multY,multy_temp,patchGradientY);
    patchNaNs(patchGradientY);

    Mat zeroMask = (patchGradientX != 0);

    patchGradientX.copyTo(patchGradientX, zeroMask);
    patchGradientY.copyTo(patchGradientY, zeroMask);

    evaluate(s, I,wmask,cloned);
}

void textureFlatten(Cloning* s, img_t* I, img_t* mask, img_t* wmask, float low_threshold,
        float high_threshold, int kernel_size, img_t* cloned)
{
    computeDerivatives(I,mask,wmask);

    Mat out = Mat(mask.size(),CC_8UC1);
    Canny(mask,out,low_threshold,high_threshold,kernel_size);

    Mat zeros(patchGradientX.size(), CC_32FC3);
    zeros.setTo(0);
    Mat zerosMask = (out != 255);
    zeros.copyTo(patchGradientX, zerosMask);
    zeros.copyTo(patchGradientY, zerosMask);

    arrayProduct(patchGradientX,binaryMaskFloat, patchGradientX);
    arrayProduct(patchGradientY,binaryMaskFloat, patchGradientY);

    evaluate(s, I,wmask,cloned);
}

#endif

static void initVariables(Cloning* s, const img_t* destination, const img_t* binaryMask)
{
  cvSetMat(s->destinationGradientX, destination->h, destination->w, CC_32F, 3);
  cvSetMat(s->destinationGradientY, destination->h, destination->w, CC_32F, 3);
  cvSetMat(s->patchGradientX, destination->h, destination->w, CC_32F, 3);
  cvSetMat(s->patchGradientY, destination->h, destination->w, CC_32F, 3);
  cvSetMat(s->binaryMaskFloat, binaryMask->h, binaryMask->w, CC_32F, 1);
  cvSetMat(s->binaryMaskFloatInverted, binaryMask->h, binaryMask->w, CC_32F, 1);
  
  //init of the filters used in the dst
  const int w = destination->w;
  int i, j;
  MYREALLOC(s->filter_X, w - 2);
  for(i = 0 ; i < w-2 ; ++i) {
    s->filter_X[i] = 2.0f * cos((float)(CC_PI) * (i + 1) / (w - 1));
  }
  
  const int h  = destination->h;
  MYREALLOC(s->filter_Y, h - 2);
  for(j = 0 ; j < h - 2 ; ++j) {
    s->filter_Y[j] = 2.0f * cos((float)(CC_PI) * (j + 1) / (h - 1));
  }
}

static void computeGradientX( const img_t* img, img_t* gx)
{
  int i, j;
  //Mat kernel = Mat::zeros(1, 3, CC_8S);  kernel.at<char>(0,2) = 1;  kernel.at<char>(0,1) = -1;
  if (img->h<=0 || img->w<=0) {return ;}
  cvSetMat(gx, img->h, img->w, CC_32F, 3);
  
  if(img->c == 3)
  {
    //filter2D(img, gx, CC_32F, kernel);
    for (i=0; i<img->h; ++i) {
      const uchar* img1 = img->tt.data + i*img->s+3;
      float* gx1 = (float*)(gx->tt.data + i*gx->s) + 3;
      for (j=1; j<img->w-1; ++j, gx1+=3, img1+=3) {
        gx1[0] = img1[0+3] - img1[0];
        gx1[1] = img1[1+3] - img1[1];
        gx1[2] = img1[2+3] - img1[2];
      }
    }
  }
  else if (img->c == 1)
  {
    for (i=0; i<img->h; ++i) {
      const uchar* img1 = img->tt.data + i*img->s;
      float* gx1 = (float*)(gx->tt.data + i*gx->s) + 3;
      for (j=1; j<img->w-1; ++j, gx1+=3) {
        int t = img1[j+1] - img1[j];
        gx1[0] = t;
        gx1[1] = t;
        gx1[2] = t;
      }
    }
  }
}

static void computeGradientY( const img_t* img, img_t* gy)
{
  int i, j;
  //Mat kernel = Mat::zeros(3, 1, CC_8S);  kernel.at<char>(2,0) = 1;  kernel.at<char>(1,0) = -1;
  if (img->h<=0 || img->w<=0) {return ;}
  cvSetMat(gy, img->h, img->w, CC_32F, 3);
  
  if(img->c == 3)
  {
    //filter2D(img, gy, CC_32F, kernel);
    for (i=1; i<img->h-1; ++i) {
      const uchar* img1 = img->tt.data + (i)*img->s;
      const uchar* img2 = img->tt.data + (i+1)*img->s;
      float* gy1 = (float*)(gy->tt.data + i*gy->s);
      for (j=0; j<img->w; ++j, gy1+=3, img1+=3, img2+=3) {
        gy1[0] = img2[0] - img1[0];
        gy1[1] = img2[1] - img1[1];
        gy1[2] = img2[2] - img1[2];
      }
    }
  }
  else if (img->c == 1)
  {
    for (i=1; i<img->h-1; ++i) {
      const uchar* img1 = img->tt.data + (i-1)*img->s;
      const uchar* img2 = img->tt.data + (i+1)*img->s;
      float* gy1 = (float*)(gy->tt.data + i*gy->s);
      for (j=0; j<img->w; ++j, gy1+=3, img1+=1, img2+=1) {
        int t = img2[0] - img1[0];
        gy1[0] = t;
        gy1[1] = t;
        gy1[2] = t;
      }
    }
  }
}

#include "img/imgopt.inl"
#include "cv/cv.inl"

static void computeDerivatives(Cloning* s, const img_t* destination, const img_t* patch, const img_t* binaryMask)
{
  img_t binaryMask1[1] = {0};
  initVariables(s, destination, binaryMask);
  
  computeGradientX(destination,s->destinationGradientX);
  computeGradientY(destination,s->destinationGradientY);
  
  computeGradientX(patch,s->patchGradientX);
  computeGradientY(patch,s->patchGradientY);
  //imshow(patch);cvWaitKey(-1);
  //imshowfmt(s->patchGradientX, PF_96bppBGRFloat);cvWaitKey(-1);
  
  //Mat Kernel(Size(3, 3), CC_8UC1);
  //Kernel.setTo(Scalar(1));
  //erode(binaryMask, binaryMask, Kernel, Point(-1, -1), 3);
  //binaryMask.convertTo(binaryMaskFloat,CC_32FC1,1.0/255.0);
  imerode(binaryMask, binaryMask1, iSIZE(3, 3), CC_MORPH_RECT, iPOINT(1, 1), BD_CONSTANT);
  cvSetMat(s->binaryMaskFloat, binaryMask->h, binaryMask->w, CC_32F, 1);
  int i;
  for (i=0; i<binaryMask1->h * binaryMask1->w; ++i) {
    s->binaryMaskFloat->tt.f4[i] = binaryMask1->tt.data[i]/255.0f;
  }
  
  if (0) {
    //imsavetxt("destination.txt", destination, "(%I8u %I8u %I8u), ");
    //imsavetxt("destinationGradientX.txt", s->destinationGradientX, "(%I32f %I32f %I32f), ");
    //imsavetxt("destinationGradientY.txt", s->destinationGradientY, "(%I32f %I32f %I32f), ");
    //imsavetxt("patchGradientY.txt", s->patchGradientX, "(%I32f %I32f %I32f), ");
    //imsavetxt("patchGradientX.txt", s->patchGradientX, "(%I32f %I32f %I32f), ");
    //imsavetxt("binaryMaskFloat.txt", s->binaryMaskFloat, "%7.3I32f, ");
  }
  //imshow(binaryMask1);cvWaitKey(-1);
  imfree(binaryMask1);
}


static void arrayProduct(const img_t* lhs, const img_t* rhs, img_t* result)
{
#if 0
  vector <Mat> lhs_channels;
  vector <Mat> result_channels;
  
  split(lhs,lhs_channels);
  split(result,result_channels);
  
  for(int chan = 0 ; chan < 3 ; ++chan)
    multiply(lhs_channels[chan],rhs, result_channels[chan]);
  
  merge(result_channels,result);
#endif
  int i, j;
  for (i=0; i<lhs->h; ++i) {
    const float* lhs1 = img_row(float, lhs, i);
    const float* rhs1 = img_row(float, rhs, i);
    float* result1 = img_row(float, result, i);
    for (j=0; j<lhs->w; ++j, result1+=3, lhs1+=3) {
      float r = rhs1[j];
      if (lhs1[0]>0 && r>0) {
        int asdf=9;
      }
      result1[0] = lhs1[0] * r;
      result1[1] = lhs1[1] * r;
      result1[2] = lhs1[2] * r;
    }
  }
}

static void computeLaplacianX( const img_t* img, img_t* laplacianX)
{
  //Mat kernel = Mat::zeros(1, 3, CC_8S);  kernel.at<char>(0,0) = -1;  kernel.at<char>(0,1) = 1;
  //filter2D(img, laplacianX, CC_32F, kernel);
  int i = 0, j;
  img_t im[1] = {0};
  if (img==laplacianX) {
    imclone2(img, im);
    img = im;
  }
  for (i=0; i<img->h; ++i) {
    const float* img1 = img_row(float, img, i);
    float* laplacianX1 = img_row(float, laplacianX, i);
    laplacianX1[0]=0;
    laplacianX1[1]=0;
    laplacianX1[2]=0;
    laplacianX1+=3;
    img1+=3;
    for (j=1; j<img->w-1; ++j) {
      laplacianX1[0] = img1[0] - img1[0-3];
      laplacianX1[1] = img1[1] - img1[1-3];
      laplacianX1[2] = img1[2] - img1[2-3];
      laplacianX1+=3;
      img1+=3;
    }
    laplacianX1[0]=0;
    laplacianX1[1]=0;
    laplacianX1[2]=0;
  }
  imfree(im);
}

static void computeLaplacianY( const img_t* img, img_t* laplacianY)
{
  //Mat kernel = Mat::zeros(3, 1, CC_8S);  kernel.at<char>(0,0) = -1;  kernel.at<char>(1,0) = 1;
  //filter2D(img, laplacianY, CC_32F, kernel);
  int i = 0, j;
  img_t im[1] = {0};
  if (img==laplacianY) {
    imclone2(img, im);
    img = im;
  }
  float* laplacianY1 = img_row(float, laplacianY, 0);
  float* laplacianY2 = img_row(float, laplacianY, img->h-1);
  for (j=0; j<img->w; ++j) {
    laplacianY1[j] = 0;
    laplacianY2[j] = 0;
  }
  for (i=1; i<img->h-1; ++i) {
    const float* img1 = img_row(float, img, i-1);
    const float* img2 = img_row(float, img, i);
    laplacianY1 = img_row(float, laplacianY, i);
    for (j=0; j<img->w; ++j) {
      laplacianY1[0] = img2[0] - img1[0];
      laplacianY1[1] = img2[1] - img1[1];
      laplacianY1[2] = img2[2] - img1[2];
      laplacianY1+=3;
      img1+=3;
      img2+=3;
    }
  }
  imfree(im);
}

//#include "cv1/dxt.inl"
//#include "math/fftw.inl"

static void dft(const img_t* src, img_t* dst, int flag, int outcn, bool is64f) {
  //ISIZE size = iSIZE(src->w, src->h);
  //fftw_dft(src, dst, flag, outcn, is64f);  return ;
  img_t im1[1] = {0};
  if (src==dst) {
    imclone2(src, im1);
    src = im1;
  }
  cvSetMat(dst, src->h, src->w, src->tid, outcn);
  //cvDFT(src->tt.data, src->s, src->c, dst->tt.data, dst->s, dst->c, flag, 0, size, is64f);
  cvDFT(src, dst, flag, 0);
  imfree(im1);
}

static void dst(const img_t* src, img_t* dest, bool invert)
{
  img_t im[10] = {0};
  int i=0, j, flag;
  img_t* temp = im+i++;
  img_t* complex = im+i++;
  img_t* temp1 = im+i++;
  imzeros(temp, src->h, 2 * src->w + 2, sizeof(float), 1);
  temp->tid = CC_32F;
  
  // src.copyTo(temp(Rect(1,0, src->w, src->h)));
  //imcopyto(src, 0, 0, temp, 1, 0, src->w, src->h);
  
  for(j = 0; j < src->h; ++j) {
    float* tempLinePtr = img_row(float, temp, j);
    const float* srcLinePtr = img_row(float, src, j);
    for(i = 0; i < src->w; ++i) {
      tempLinePtr[i+1] = srcLinePtr[i];
      tempLinePtr[src->w + 2 + i] = - srcLinePtr[src->w - 1 - i];
    }
  }
  //imshowfmt(temp, PF_32bppGrayFloat); cvWaitKey(-1);
  flag = invert ? (CC_DXT_ROWS + CC_DXT_SCALE + CC_DXT_INVERSE) : (CC_DXT_ROWS+CC_DXT_FORWARD);
//  Mat planes[] = {temp, Mat::zeros(temp.size(), CC_32F)};
  cvSetMat(complex, temp->h, temp->w, CC_32F, 2);
  for(j = 0; j < temp->h; ++j) {
    float* tempLinePtr = img_row(float, temp, j);
    float* complex1 = img_row(float, complex, j);
    for(i = 0; i < temp->w; ++i) {
      complex1[i*2+0] = tempLinePtr[i];
      complex1[i*2+1] = 0;
    }
  }
  //imshowfmt(temp, PF_32bppGrayFloat); cvWaitKey(-1);
  
  dft(complex, complex, flag, 2, 0);
  //flag = !invert ? CC_DXT_ROWS + CC_DXT_SCALE + CC_DXT_INVERSE : CC_DXT_ROWS;
  //dft(complex, complex, flag, 2, 0);
  //imshowfmt(temp, PF_32bppGrayFloat);
  //imsplit(complex, temp1, 0, sizeof(float));
  //imshowfmt(temp1, PF_32bppGrayFloat);  cvWaitKey(-1);

  imzeros(temp, src->w, 2 * src->h + 2, sizeof(float), 1);
  temp->tid = CC_32F;
  for(j = 0 ; j < src->w ; ++j) {
    float* tempLinePtr = img_row(float, temp, j);
    for(i = 0 ; i < src->h ; ++i) {
      float val = img_row(float, complex, i)[(j+1)*2+1];
      tempLinePtr[i + 1] = val;
      tempLinePtr[temp->w - 1 - i] = - val;
    }
  }
  //imshowfmt(temp, PF_32bppGrayFloat);  cvWaitKey(-1);
  //Mat planes2[] = {temp, Mat::zeros(temp.size(), CC_32F)};
  //merge(planes2, 2, complex);
  cvSetMat(complex, temp->h, temp->w, CC_32F, 2);
  for(j = 0; j < temp->h; ++j) {
    float* tempLinePtr = img_row(float, temp, j);
    float* complex1 = img_row(float, complex, j);
    for(i = 0; i < temp->w; ++i) {
      complex1[i*2+0] = tempLinePtr[i];
      complex1[i*2+1] = 0;
    }
  }
  dft(complex, complex, flag, 2, false);
  //imsplit(complex, temp1, 0, sizeof(float));
  //imshowfmt(temp1, PF_32bppGrayFloat);  cvWaitKey(-1);
  //split(complex, planes2);
  //temp = planes2[1].t();
  imzeros(dest, src->h, src->w, sizeof(float), 1);
  dest->tid = CC_32F;
  //temp(Rect( 0, 1, src->w, src->h)).copyTo(dest);
  for(j = 0; j < dest->h; ++j) {
    float* dest1 = img_row(float, dest, j);
    for(i = 0; i < dest->w; ++i) {
      float val = img_row(float, complex, i)[(j+1)*2+1];
      dest1[i] = val;
    }
  }
  //imshowfmt(dest, PF_32bppGrayFloat);  cvWaitKey(-1);
  imfrees(im, 10);
}

static void idst(const img_t* src, img_t* dest)
{
  dst(src, dest, true);
}

static void Cloning_solve(Cloning* s, const img_t* img, img_t* mod_diff, img_t* result)
{
  const int w = img->w;
  const int h = img->h;
  int i=0, j;
  img_t im[10] = {0};
  img_t* res = im+i++;
  
  //imshowfmt(mod_diff, PF_32bppGrayFloat);  cvWaitKey(-1);
  dst(mod_diff, res, false);

  for(j = 0 ; j < h-2; j++) {
    float* resLinePtr = img_row(float, res, j);
    for(i = 0 ; i < w-2; i++) {
      resLinePtr[i] /= (s->filter_X[i] + s->filter_Y[j] - 4);
    }
  }
  
  idst(res, mod_diff);
  //imshowfmt(res, PF_32bppGrayFloat);
  //imshowfmt(mod_diff, PF_32bppGrayFloat);  cvWaitKey(-1);
  
  uchar *resLinePtr = img_row(uchar, result, 0);
  const uchar * imgLinePtr = img_row(uchar, img, 0);
  const float * interpLinePtr = NULL;
  
  //first col
  for(i = 0 ; i < w ; ++i) {
    resLinePtr[i] = imgLinePtr[i];
  }
  
  //cvSetMat("asdf", "%I32f", h-2, w-2, mod_diff->tt.data, mod_diff->s, mod_diff->c, -1); cvWaitKey(-1);
  for(j = 1 ; j < h-1 ; ++j)
  {
    resLinePtr = img_row(uchar, result, j);
    imgLinePtr  = img_row(uchar, img, j);
    interpLinePtr = img_row(float, mod_diff, j-1);
    
    //first row
    resLinePtr[0] = imgLinePtr[0];
    
    for(i = 1 ; i < w-1 ; ++i) {
      //saturate cast is not used here, because it behaves differently from the previous implementation
      //most notable, saturate_cast rounds before truncating, here it's the opposite.
      float value = interpLinePtr[i-1];
      if(value < 0.) {
        resLinePtr[i] = 0;
      }
      else if (value > 255.0) {
        resLinePtr[i] = 255;
      } else {
        resLinePtr[i] = (uchar)(value);
      }
    }
    
    //last row
    resLinePtr[w-1] = imgLinePtr[w-1];
  }
  
  //last col
  resLinePtr = img_row(uchar, result, h-1);
  imgLinePtr = img_row(uchar, img, h-1);
  for(i = 0 ; i < w ; ++i) {
    resLinePtr[i] = imgLinePtr[i];
  }
  //imshow(result);  cvWaitKey(-1);
  imfrees(im, 10);
}

static void Laplacian(const img_t* im, img_t* lap) {
  int i, j;
  float* lap1 = img_row(float, lap, 0);
  float* lap2 = img_row(float, lap, im->h-1);
  for (j=1; j<im->w-1; ++j) {
    lap1[0] = lap2[0] = 0;
  }
  for (i=1; i<im->h-1; ++i) {
    const uchar* im0 = img_row(uchar, im, i-1);
    const uchar* im1 = img_row(uchar, im, i);
    const uchar* im2 = img_row(uchar, im, i+1);
    float* lap1 = img_row(float, lap, i);
    lap1[0] = lap1[im->w-1] = 0;
    for (j=1; j<im->w-1; ++j) {
      lap1[j] = (im0[j] + im2[j] + im1[j-1] + im1[j+1] - im1[j]*4);
    }
  }
  return ;
}

// img u8
static void poissonSolver(Cloning* s, const img_t* img, const img_t* laplacianX , const img_t* laplacianY, img_t* result)
{
  const int w = img->w;
  const int h = img->h;
  int i=0, j;
  img_t im[10] = {0};
  img_t* lap = im+i++;
  img_t* bound = im+i++;
  img_t* boundary_points = im+i++;
  
  cvSetMat(lap, img->h, img->w, CC_32F, 1);

  //lap = laplacianX + laplacianY;
  for (i=0; i<img->h; ++i) {
    const float* laplacianX1 = img_row(float, laplacianX, i);
    const float* laplacianY1 = img_row(float, laplacianY, i);
    float* lap1 = img_row(float, lap, i);
    for (j=0; j<img->w; ++j) {
      lap1[j] = laplacianX1[j] + laplacianY1[j];
    }
  }
  imclone2(img, bound);
  //imshowfmt(lap, PF_32bppGrayFloat);      cvWaitKey(-1);

  //rectangle(bound, Point(1, 1), Point(img->w-2, img->h-2), Scalar::all(0), -1);
  
  for (i=2; i<img->h-2; ++i) {
    uchar* bound1 = img_row(uchar, bound, i);
    for (j=2; j<img->w-2; ++j) {
      bound1[j] = 0;// - boundary_points1[j];
    }
  }

  imzeros(boundary_points, img->h, img->w, sizeof(float), 1);
  boundary_points->tid = CC_32F;
  Laplacian(bound, boundary_points);
  
  //imshow(img);
  //imshow(bound);
  //imshowfmt(boundary_points, PF_32bppGrayFloat);      cvWaitKey(-1);
  //boundary_points = lap - boundary_points;
  
  for (i=0; i<img->h; ++i) {
    const float* lap1 = img_row(float, lap, i);
    float* boundary_points1 = img_row(float, boundary_points, i);
    for (j=0; j<img->w; ++j) {
      boundary_points1[j] = lap1[j] - boundary_points1[j];
    }
  }

  if (0) {
    //imwrite("bound.bmp", bound);
    //imsavetxt("boundary_points.txt", boundary_points, "%I32f,");
	}
  //imshowfmt(boundary_points, PF_32bppGrayFloat);      cvWaitKey(-1);
  //Mat mod_diff = boundary_points(Rect(1, 1, w-2, h-2));
  
  img_t mod_diff = imroi(boundary_points, iRECT2(1, 1, w-2, h-2));
  Cloning_solve(s, img, &mod_diff, result);
  //imshow(result);      cvWaitKey(-1);
  imfrees(im, 10);
}

static void poisson(Cloning* s, const img_t* destination)
{
  img_t im[10] = {0};
  int i = 0, j, chan;
  img_t* laplacianX = im+i++;
  img_t* laplacianY = im+i++;
  cvSetMat(laplacianX, destination->h, destination->w, CC_32F, 3);
  cvSetMat(laplacianY, destination->h, destination->w, CC_32F, 3);
  
  //laplacianX = destinationGradientX + patchGradientX;
  //laplacianY = destinationGradientY + patchGradientY;
  for (i=0; i<destination->h; ++i) {
    const float* destinationGradientX1 = img_row(float, s->destinationGradientX, i);
    const float* destinationGradientY1 = img_row(float, s->destinationGradientY, i);
    const float* patchGradientX1 = img_row(float, s->patchGradientX, i);
    const float* patchGradientY1 = img_row(float, s->patchGradientY, i);
    float* laplacianX1 = img_row(float, laplacianX, i);
    float* laplacianY1 = img_row(float, laplacianY, i);
    for (j=0; j<destination->w*3; ++j) {
      laplacianX1[j] = destinationGradientX1[j] + patchGradientX1[j];
      laplacianY1[j] = destinationGradientY1[j] + patchGradientY1[j];
    }
  }
  
  if (1) {
    //imsavetxt("laplacianX.txt", laplacianX, "(%I32f %I32f %I32f), ");
    //imsavetxt("laplacianY.txt", laplacianY, "(%I32f %I32f %I32f), ");
  }
  //imshowfmt(laplacianX, PF_96bppBGRFloat); imshowfmt(laplacianY, PF_96bppBGRFloat);      cvWaitKey(-1);
  computeLaplacianX(laplacianX,laplacianX);
  computeLaplacianY(laplacianY,laplacianY);
  //imshowfmt(laplacianX, PF_96bppBGRFloat); imshowfmt(laplacianY, PF_96bppBGRFloat);      cvWaitKey(-1);
  
  if (1) {
    //imsavetxt("laplacianX1.txt", laplacianX, "(%I32f %I32f %I32f), ");
    //imsavetxt("laplacianY1.txt", laplacianY, "(%I32f %I32f %I32f), ");
  }
  //imshowfmt(laplacianX, PF_96bppBGRFloat);
  //imshowfmt(laplacianY, PF_96bppBGRFloat);cvWaitKey(-1);
  //split(laplacianX,rgbx_channel);
  //split(laplacianY,rgby_channel);
  //imsplit(destination,output);
  
  for(chan = 0 ; chan < 3 ; ++chan)
  {
    imsplit(laplacianX,s->rgbx_channel+chan, chan*sizeof(float), sizeof(float));
    imsplit(laplacianY,s->rgby_channel+chan, chan*sizeof(float), sizeof(float));
    imsplit(destination,s->output+chan, chan, 1);
    (s->rgbx_channel+chan)->tid = laplacianX->tid;
    (s->rgby_channel+chan)->tid = laplacianY->tid;
    //imshow2("destination", (s->output+chan));WaitKey(-1);
    poissonSolver(s, s->output+chan, s->rgbx_channel+chan, s->rgby_channel+chan, s->output+chan);
    //imshow2("destination", (s->output+chan));WaitKey(-1);
    //imshowfmt((s->rgbx_channel+chan), PF_32bppGrayFloat);    imshowfmt((s->rgby_channel+chan), PF_32bppGrayFloat);    imshow((s->output+chan));    cvWaitKey(-1);
  }
  imfrees(im, 10);
}

static void evaluate(Cloning* s, const img_t* I, const img_t* wmask, img_t* cloned)
{
  //bitwise_not(wmask,wmask);
  //wmask.convertTo(binaryMaskFloatInverted,CC_32FC1,1.0/255.0);
  int i, j;
  for (i=0;i<wmask->h; ++i) {
    const uchar* wmask1 = img_row(uchar, wmask, i);
    float* binaryMaskFloatInverted1 = img_row(float, s->binaryMaskFloatInverted, i);
    for (j=0; j<wmask->w; ++j) {
      binaryMaskFloatInverted1[j] = (255 - wmask1[j])/255.0;
    }
  }
  
  arrayProduct(s->destinationGradientX,s->binaryMaskFloatInverted, s->destinationGradientX);
  arrayProduct(s->destinationGradientY,s->binaryMaskFloatInverted, s->destinationGradientY);
  if (0) {
    //imsavetxt("destinationGradientX1.txt", s->destinationGradientX, "(%I32f %I32f %I32f), ");
    //imsavetxt("destinationGradientY1.txt", s->destinationGradientY, "(%I32f %I32f %I32f), ");
  }
  //imshowfmt(s->destinationGradientX, PF_96bppBGRFloat);
  //imshowfmt(s->destinationGradientY, PF_96bppBGRFloat);cvWaitKey(-1);
  
  poisson(s, I);
  //merge(output,cloned);
  cvSetMat(cloned, I->h, I->w, CC_8U, 3);
  for (i=0;i<I->h; ++i) {
    const uchar* output0 = img_row(uchar, s->output+0, i);
    const uchar* output1 = img_row(uchar, s->output+1, i);
    const uchar* output2 = img_row(uchar, s->output+2, i);
    uchar* cloned1 = img_row(uchar, cloned, i);
    for (j=0; j<I->w; ++j) {
      cloned1[0] = output0[j];
      cloned1[1] = output1[j];
      cloned1[2] = output2[j];
      cloned1+=3;
    }
  }
}

static void normalClone(Cloning* s, const img_t* destination, const img_t* patch, const img_t* binaryMask, img_t* cloned, int flag)
{
  int i;
  const int w = destination->w;
  const int h = destination->h;
  const int channel = destination->c;
  const int n_elem_in_line = w * channel;
  
  computeDerivatives(s, destination,patch,binaryMask);
  
  switch(flag)
  {
  case NORMAL_CLONE:
    //imshowfmt(s->patchGradientX, PF_96bppBGRFloat);
    //imshowfmt(s->patchGradientY, PF_96bppBGRFloat);cvWaitKey(-1);
    arrayProduct(s->patchGradientX,s->binaryMaskFloat, s->patchGradientX);
    arrayProduct(s->patchGradientY,s->binaryMaskFloat, s->patchGradientY);
    ///imshowfmt(s->patchGradientY, PF_96bppBGRFloat);cvWaitKey(-1);
    if (0) {
      //imsavetxt("patchGradientX1.txt", s->patchGradientX, "(%I32f %I32f %I32f), ");
      //imsavetxt("patchGradientY1.txt", s->patchGradientY, "(%I32f %I32f %I32f), ");
    }
    break;
    
  case MIXED_CLONE:
    {
      int* maskIndices = MALLOC(int, n_elem_in_line);
      for (i = 0; i < n_elem_in_line; ++i)
        maskIndices[i] = i / channel;
      
      for(i=0;i < h; i++)
      {
        float * patchXLinePtr = img_row(float, s->patchGradientX, i);
        float * patchYLinePtr = img_row(float, s->patchGradientY, i);
        const float * destinationXLinePtr = img_row(float, s->destinationGradientX, i);
        const float * destinationYLinePtr = img_row(float, s->destinationGradientY, i);
        const float * binaryMaskLinePtr = img_row(float, s->binaryMaskFloat, i);
        
        for(int j=0; j < n_elem_in_line; j++)
        {
          int maskIndex = maskIndices[j];
          
          if(abs(patchXLinePtr[j] - patchYLinePtr[j]) >
            abs(destinationXLinePtr[j] - destinationYLinePtr[j]))
          {
            patchXLinePtr[j] *= binaryMaskLinePtr[maskIndex];
            patchYLinePtr[j] *= binaryMaskLinePtr[maskIndex];
          }
          else
          {
            patchXLinePtr[j] = destinationXLinePtr[j]
              * binaryMaskLinePtr[maskIndex];
            patchYLinePtr[j] = destinationYLinePtr[j]
              * binaryMaskLinePtr[maskIndex];
          }
        }
      }
      FREE(maskIndices);
    }
    break;
    
  case MONOCHROME_TRANSFER:
    {
      img_t gray[1] = {0};
      imsetsize(gray, patch->h, patch->w, 1, 1);
      //cvtColor(patch, gray, COLOR_BGR2GRAY );
      imcolorcvt(patch, gray, T_BGR, T_GRAY);
      
      computeGradientX(gray,s->patchGradientX);
      computeGradientY(gray,s->patchGradientY);
      
      arrayProduct(s->patchGradientX, s->binaryMaskFloat, s->patchGradientX);
      arrayProduct(s->patchGradientY, s->binaryMaskFloat, s->patchGradientY);
    }
    break;
    
  }
  
  evaluate(s, destination,binaryMask,cloned);
}
