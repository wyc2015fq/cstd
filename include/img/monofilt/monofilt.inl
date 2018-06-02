

int monofilt(const IplImage* im, int nscale, int minWaveLength, double mult, double sigmaOnf, int orientWrap, 
             IplImage** f, IplImage** h1f, IplImage** h2f, IplImage** A, IplImage** theta, IplImage** psi) {
  int rows,cols;
  int i, j, s;
  IplImage* IM;   //傅里叶系数
  IplImage* u1;
  IplImage* u2;
  IplImage* radius;
  IplImage* H1;
  IplImage* H2;
  IplImage* logGabor;
  IplImage* H1s;
  IplImage* H2s;
  IplImage* temp1c2;
  IplImage* temp2c2;
  IplImage* temp3c2;
  IplImage* temp4c2;

  //utime_start(_start_time);
  //if nargin == 5
  //  orientWrap = 0;  // Default is no orientation wrapping
  //end
  
  rows=im->height, cols = im->width;

  IM = cvCreateImage1(cvGetSize(im),IMG_DEPTH_64F,2);
  //printf("%4d:%f\n", __LINE__, utime_elapsed(_start_time));
  fft2(im,IM);
  //printf("%4d:%f\n", __LINE__, utime_elapsed(_start_time));
  //FPRINT1D("im.txt", "%d,\n", rows*cols, ((uchar*)(im->imageData)), 1);
  //FPRINT1D("fft2.txt", "%lf,\n", rows*cols*2, ((double*)(IM->imageData)), 1);
#define MESHFILL(_N, _X, _XI, _A, _S) { int i=0; for (; i<(_N); ++i) *(_X+i*(_XI))=_A+_S*i; }
  u1 = cvCreateImage1(cvGetSize(im),IMG_DEPTH_64F, 1);
  u2 = cvCreateImage1(cvGetSize(im),IMG_DEPTH_64F, 1);
  for( j = 0; j < rows; j++ ){
    for( i = 0; i < cols; i++ ){
      CC_IMAGE_ELEM( u1, double, j, i) = -0.5+i*1./cols;
      CC_IMAGE_ELEM( u2, double, j, i) = -0.5+j*1./rows;
    }
  }

  // Generate horizontal and vertical frequency grids that vary from
  // -0.5 to 0.5 
  //[u1, u2] = meshgrid(([1:cols]-(fix(cols/2)+1))/(cols-mod(cols,2)), ([1:rows]-(fix(rows/2)+1))/(rows-mod(rows,2)));

  //u1 = ifftshift(u1);   // Quadrant shift to put 0 frequency at the corners
  //u2 = ifftshift(u2);
  ifftshift(u1, u1);
  ifftshift(u2, u2);

  radius = cvCreateImage1(cvGetSize(im),IMG_DEPTH_64F, 1);

  //radius = sqrt(u1.^2 + u2.^2);    // Matrix values contain frequency
                                   // values as a radius from centre
                                   // (but quadrant shifted)
  for( j = 0; j < rows; j++ ){
    for( i = 0; i < cols; i++ ){
      double t1 = CC_IMAGE_ELEM( u1, double, j, i);
      double t2 = CC_IMAGE_ELEM( u2, double, j, i);
      CC_IMAGE_ELEM( radius, double, j, i)=sqrt(t1*t1+t2*t2);
    }
  }

  // Get rid of the 0 radius value in the middle (at top left corner after
  // fftshifting) so that taking the log of the radius, or dividing by the
  // radius, will not cause trouble.
  //radius(1,1) = 1;
  CC_IMAGE_ELEM( radius, double, 0, 0)=1;
  
  //FPRINT1D("radius.txt", "%lf,\n", 10000, ((double*)(radius->imageData)), 1);
  H1 = cvCreateImage1(cvGetSize(im),IMG_DEPTH_64F, 1);
  H2 = cvCreateImage1(cvGetSize(im),IMG_DEPTH_64F, 1);
  //H1 = i*u1./radius;   // The two monogenic filters in the frequency domain
  //H2 = i*u2./radius;
  //cvDiv( u1, radius, H1, 0);
  //cvDiv( u2, radius, H2, 0);
  for (i=0; i<rows*cols; ++i) {
    ((double*)(H1->imageData))[i]=((double*)(u1->imageData))[i]/((double*)(radius->imageData))[i];
    ((double*)(H2->imageData))[i]=((double*)(u2->imageData))[i]/((double*)(radius->imageData))[i];
  }
  //FPRINT1D("u1.txt", "%lf,\n", 10000, ((double*)(u1->imageData)), 1);
  //FPRINT1D("radius.txt", "%lf,\n", 10000, ((double*)(radius->imageData)), 1);
  //FPRINT1D("H1.txt", "%lf,\n", 10000, ((double*)(H1->imageData)), 1);
  // The two monogenic filters H1 and H2 are oriented in frequency space
  // but are not selective in terms of the magnitudes of the
  // frequencies.  The code below generates bandpass log-Gabor filters
  // which are point-wise multiplied by H1 and H2 to produce different
  // bandpass versions of H1 and H2
  logGabor = cvCreateImage1(cvGetSize(im),IMG_DEPTH_64F, 1);
  H1s = cvCreateImage1(cvGetSize(im),IMG_DEPTH_64F, 1);
  H2s = cvCreateImage1(cvGetSize(im),IMG_DEPTH_64F, 1);
  temp1c2 = cvCreateImage1(cvGetSize(im),IMG_DEPTH_64F, 2);
  temp2c2 = cvCreateImage1(cvGetSize(im),IMG_DEPTH_64F, 2);
  temp3c2 = cvCreateImage1(cvGetSize(im),IMG_DEPTH_64F, 2);
  temp4c2 = cvCreateImage1(cvGetSize(im),IMG_DEPTH_64F, 2);

  for (s = 1; s <= nscale; ++s) {
    double wavelength = minWaveLength*pow(mult, s-1);
    double fo = 1.0/wavelength;                  // Centre frequency of filter.
    //logGabor = exp((-(log(radius/fo)).^2) / (2 * log(sigmaOnf)^2));
    for( j = 0; j < rows; j++ ){
      for( i = 0; i < cols; i++ ){
        double t1 = CC_IMAGE_ELEM( radius, double, j, i);
        double t2 = t1/fo;
        double t3 = (log(t1/fo));
        double t4 = log(sigmaOnf);
        CC_IMAGE_ELEM( logGabor, double, j, i)=exp((-t3*t3) / (2 * t4*t4));
      }
    }
    //logGabor(1,1) = 0;        // undo the radius fudge.
    CC_IMAGE_ELEM( logGabor, double, 0, 0) =0;
    
    // Generate bandpass versions of H1 and H2 at this scale
    //H1s = H1.*logGabor;
    //H2s = H2.*logGabor;
    for( j = 0; j < rows; j++ ) {
      for( i = 0; i < cols; i++ ) {
        double t1 = CC_IMAGE_ELEM( H1, double, j, i);
        double t2 = CC_IMAGE_ELEM( H2, double, j, i);
        double t3 = CC_IMAGE_ELEM( logGabor, double, j, i);
        CC_IMAGE_ELEM( H1s, double, j, i)=t1*t3;
        CC_IMAGE_ELEM( H2s, double, j, i)=t2*t3;
      }
    }
    
    //  Apply filters to image in the frequency domain and get spatial results 
    //f[s] = real(ifft2(IM.*logGabor));
    //h1f[s] = real(ifft2(IM.*H1s));
    //h2f[s] = real(ifft2(IM.*H2s));
    
    for( j = 0; j < rows; j++ ) {
      for( i = 0; i < cols; i++ ) {
        double t1 = CC_IMAGE_ELEM( H1s, double, j, i);
        double t2 = CC_IMAGE_ELEM( H2s, double, j, i);
        double t3 = CC_IMAGE_ELEM( logGabor, double, j, i);
        double c1 = CC_IMAGE_ELEM( IM, double, j, i*2+0);
        double c2 = CC_IMAGE_ELEM( IM, double, j, i*2+1);
        // 注意H1s和H2s是虚数i*i=-1
        CC_IMAGE_ELEM( temp1c2, double, j, i*2+0)=-c2*t1;
        CC_IMAGE_ELEM( temp1c2, double, j, i*2+1)=c1*t1;
        CC_IMAGE_ELEM( temp2c2, double, j, i*2+0)=-c2*t2;
        CC_IMAGE_ELEM( temp2c2, double, j, i*2+1)=c1*t2;
        CC_IMAGE_ELEM( temp3c2, double, j, i*2+0)=c1*t3;
        CC_IMAGE_ELEM( temp3c2, double, j, i*2+1)=c2*t3;
      }
    }
    //printf("%4d:%f\n", __LINE__, utime_elapsed(_start_time));
    cvDFT(temp1c2,temp4c2,CC_DXT_INV_SCALE, 0); cvReal(temp4c2, h1f[s]);
    cvDFT(temp2c2,temp4c2,CC_DXT_INV_SCALE, 0); cvReal(temp4c2, h2f[s]);
    cvDFT(temp3c2,temp4c2,CC_DXT_INV_SCALE, 0); cvReal(temp4c2, f[s]);
    //printf("%4d:%f\n", __LINE__, utime_elapsed(_start_time));
    
    //A[s] = sqrt(f[s].^2 + h1f[s].^2 + h2f[s].^2);  // Magnitude of Energy.
    
    for( j = 0; j < rows; j++ ) {
      for( i = 0; i < cols; i++ ) {
        double t1 = CC_IMAGE_ELEM( f[s], double, j, i);
        double t2 = CC_IMAGE_ELEM( h1f[s], double, j, i);
        double t3 = CC_IMAGE_ELEM( h2f[s], double, j, i);
        CC_IMAGE_ELEM( A[s], double, j, i)=sqrt(t1*t1 + t2*t2 + t3*t3);
      }
    }
    // If requested calculate the orientation and phase angles
    if (theta && psi) {
      //theta[s] = atan2(h2f[s], h1f[s]);              // Orientation.
      // Here phase is measured relative to the h1f-h2f plane as an
      // 'elevation' angle that ranges over +- pi/2
      //psi[s] = atan2(f[s], sqrt(h1f[s].^2 + h2f[s].^2));
      for( j = 0; j < rows; j++ ) {
        for( i = 0; i < cols; i++ ) {
          double t1 = CC_IMAGE_ELEM( h2f[s], double, j, i);
          double t2 = CC_IMAGE_ELEM( h1f[s], double, j, i);
          double t3 = CC_IMAGE_ELEM( f[s], double, j, i);
          double t4 = sqrt(t2*t2 + t1*t1);
          CC_IMAGE_ELEM( theta[s], double, j, i) = atan2(t1, t2);
          CC_IMAGE_ELEM( psi[s], double, j, i) = atan2(t3, t4);
        }
      }
      if (orientWrap) {
        // Wrap orientation values back into the range 0-pi
        //negind = find(theta[s]<0);
        //theta[s](negind) = theta[s](negind) + pi;

        // Where orientation values have been wrapped we should
        // adjust phase accordingly **check**
        //psi[s](negind) = pi-psi[s](negind);
        //morethanpi = find(psi[s]>pi);
        //psi[s](morethanpi) = psi[s](morethanpi)-2*pi;
        
        for( j = 0; j < rows; j++ ) {
          for( i = 0; i < cols; i++ ) {
            double t1 = CC_IMAGE_ELEM( theta[s], double, j, i);
            double t2 = CC_IMAGE_ELEM( psi[s], double, j, i);
            if (t1<0) {
              CC_IMAGE_ELEM( theta[s], double, j, i) = t1+CC_PI;
              CC_IMAGE_ELEM( psi[s], double, j, i) = CC_PI-t2;
            }
            if (t1>CC_PI) {
              CC_IMAGE_ELEM( psi[s], double, j, i) = t2-2*CC_PI;
            }
          }
        }
      }
      
    }
  }
  //printf("%4d:%f\n", __LINE__, utime_elapsed(_start_time));
  cvReleaseImage(&IM);
  cvReleaseImage(&u1);
  cvReleaseImage(&u2);
  cvReleaseImage(&radius);
  cvReleaseImage(&H1);
  cvReleaseImage(&H2);
  cvReleaseImage(&logGabor);
  cvReleaseImage(&H1s);
  cvReleaseImage(&H2s);
  cvReleaseImage(&temp1c2);
  cvReleaseImage(&temp2c2);
  cvReleaseImage(&temp3c2);
  cvReleaseImage(&temp4c2);
  return 0;
}

