
//傅里叶正变换
void fft2(const img_t *src, img_t *dst)
{
  //实部、虚部
  img_t image_Re[1] = {0};
  img_t image_Im[1] = {0};
  img_t Fourier[1] = {0};
  //   int i, j;
  cvSetMat(image_Re, cvGetSize(src), CC_64F, 1);  //实部
  //Imaginary part
  cvSetMat(image_Im, cvGetSize(src), CC_64F, 1);  //虚部
  //2 channels (image_Re, image_Im)
  cvSetMat(Fourier, cvGetSize(src), CC_64F, 2);
  // Real part conversion from u8 to 64f (double)
  cvConvertScale(src, image_Re, 1, 0);
  // Imaginary part (zeros)
  cvZeroMat(image_Im);
  // Join real and imaginary parts and stock them in Fourier image
  cvMerge(image_Re, image_Im, 0, 0, Fourier);
  // Application of the forward Fourier transform
  cvDFT(Fourier, dst, CC_DXT_FORWARD, 0);
  imfree(image_Re);
  imfree(image_Im);
  imfree(Fourier);
}

int ifftshift(const img_t *src, img_t *dst) {
  int nRow, nCol, i, j, cy, cx;
  const double* _src = img_ptr(double, src);
  double* _dst = img_ptr(double, dst);
  int sstep = src->s/src->c;
  int dstep = dst->s/dst->c;
  nRow = src->height;
  nCol = src->width;
  cy = nRow/2; // image center
  cx = nCol/2;
  for( j = 0; j < cy; j++ ) {
    for( i = 0; i < cx; i++ ) {
      //中心化，将整体份成四块进行对角交换
      double t1 = _src[j*sstep+i];
      double t2 = _src[(j+cy)*sstep+i];
      double t3 = _src[j*sstep+i+cx];
      double t4 = _src[(j+cy)*sstep+i+cx];

      _dst[j*dstep+i] = t4;
      _dst[(j+cy)*dstep+i] = t3;
      _dst[j*dstep+i+cx] = t2;
      _dst[(j+cy)*dstep+i+cx] = t1;
    }
  }
  return 0;
}
void fft2shift(img_t *src, img_t *dst)
{
  img_t image_Re[1] = {0};
  img_t image_Im[1] = {0};
  int nRow, nCol, i, j, cy, cx;
  double scale, shift, tmp13, tmp24;
  cvSetMat(image_Re, cvGetSize(src), CC_64F, 1);
  //Imaginary part
  cvSetMat(image_Im, cvGetSize(src), CC_64F, 1);
  cvSplit( src, image_Re, image_Im, 0, 0 );
  //具体原理见冈萨雷斯数字图像处理p123
  // Compute the magnitude of the spectrum Mag = sqrt(Re^2 + Im^2)
  //计算傅里叶谱
 // cvAddS( image_Re, cScalar(1.0, 0, 0, 0), image_Re, 0 ); // 1 + Mag
  cvPow( image_Re, image_Re, 2.0);
  cvPow( image_Im, image_Im, 2.0);
  cvAdd( image_Re, image_Im, image_Re, 0);
  cvPow( image_Re, image_Re, 0.5 );
  //对数变换以增强灰度级细节(这种变换使以窄带低灰度输入图像值映射
  //一宽带输出值，具体可见冈萨雷斯数字图像处理p62)
  // Compute log(1 + Mag);
  cvAddS( image_Re, cScalar(1.0, 0, 0, 0), image_Re, 0 ); // 1 + Mag
  cvLog( image_Re, image_Re ); // log(1 + Mag)
  //Rearrange the quadrants of Fourier image so that the origin is at the image center
  nRow = src->height;
  nCol = src->width;
  cy = nRow/2; // image center
  cx = nCol/2;
  //CC_IMAGE_ELEM为OpenCV定义的宏，用来读取图像的像素值，这一部分就是进行中心变换
  for( j = 0; j < cy; j++ ){
    for( i = 0; i < cx; i++ ){
      //中心化，将整体份成四块进行对角交换
      tmp13 = CC_IMAGE_ELEM( image_Re, double, j, i);
      CC_IMAGE_ELEM( image_Re, double, j, i) = CC_IMAGE_ELEM( image_Re, double, j+cy, i+cx);
      CC_IMAGE_ELEM( image_Re, double, j+cy, i+cx) = tmp13;
      
      tmp24 = CC_IMAGE_ELEM( image_Re, double, j, i+cx);
      CC_IMAGE_ELEM( image_Re, double, j, i+cx) = CC_IMAGE_ELEM( image_Re, double, j+cy, i);
      CC_IMAGE_ELEM( image_Re, double, j+cy, i) = tmp24;
    }
  }
  {
    //归一化处理将矩阵的元素值归一为[0,255]
    //[(f(x,y)-minVal)/(maxVal-minVal)]*255
    double minVal = 0, maxVal = 0;
    // Localize minimum and maximum values
    cvMinMaxLoc( image_Re, &minVal, &maxVal, 0, 0, 0 );
    // Normalize image (0 - 255) to be observed as an u8 image
    scale = 255/(maxVal - minVal);
    shift = -minVal * scale;
    cvConvertScale(image_Re, dst, scale, shift);
    imfree(image_Re);
    imfree(image_Im);
  }
}

#define cvReal(src, ImageRe) cvSplit(src,ImageRe,0,0,0)

#include "img/imgio.inl"

int test_fft2(int argc, char* argv[]) 
{
  int i = 0;
  img_t im[10] = {0};
  img_t* s = im+i++;
  img_t* d = im+i++;
  img_t* s2 = im+i++;
  cvSetMat(s, 1,32,CC_32F, 1); 
  cvSetMat(d, 1,32,CC_32F, 1); 
  cvSetMat(s2, 1,32,CC_32F, 1); 
  for(i=0;i<32;i++) {
    cvSetReal2D(s,0,i,i); 
  }
  
  //显示一下输入信号 
  for(i=0;i<32;i++) 
    printf("%6.2f ", cvGetReal2D(s,0,i)); 
  printf("\n"); 
  
  //DFT 离散傅立叶变换 
  cvDFT(s, d, CC_DXT_FORWARD);     //CC_DXT_FORWARD 代表了正变换：空域－〉频域 
  
  printf("The result of DFT: "); 
  for(i=0;i<32;i++) 
    printf("%6.2f ", cvGetReal2D(d,0,i)); 
  
  printf("\n"); 
  //DFT 离散傅立叶逆变换 
  cvDFT(d,s2,CC_DXT_INVERSE); //逆变换 
  printf(" The result of IDFT: ");
  for(i=0;i<32;i++) 
    printf("%6.2f ",cvGetReal2D(s2,0,i)); 
  printf(" "); 
  
  imfrees2(im);
  
  return 0; 
} 
int test_fft3()
{
  //aa+=1;
  //test_cpcode();
  IM_BEGIN(im, 10);
  double m, M;
  double scale;
  double shift;
  {
    double aaa[] = {1,2,3,4};
  }
  sys_chdir("C:/code/lib/opencv-3.2.0/samples/data");
  //加载源图像，第二个参数表示将输入的图片转为单信道
  img_t* src = im++;
  imread("lena.jpg", 1, 1, src);
  img_t* Fourier = cvSetMat(im++, cvGetSize(src),CC_64F,2);
  img_t* dst = cvSetMat(im++, cvGetSize(src),CC_64F,2);
  img_t* ImageRe = cvSetMat(im++, cvGetSize(src),CC_64F,1);
  img_t* ImageIm = cvSetMat(im++, cvGetSize(src),CC_64F,1);
  img_t* Image = cvSetMat(im++, cvGetSize(src), src->tid, 1);
  img_t* ImageDst = cvSetMat(im++, cvGetSize(src),src->tid,1);
  fft2(src,Fourier);                  //傅里叶变换
  //cvOutMatToFile(Fourier, );
  fft2shift(Fourier, Image);          //中心化
  cvDFT(Fourier,dst,CC_DXT_INV_SCALE, 0);//实现傅里叶逆变换，并对结果进行缩放
  cvSplit(dst,ImageRe,ImageIm,0,0);
  imshow2("源图像",src);
  //对数组每个元素平方并存储在第二个参数中
  cvPow(ImageRe,ImageRe,2);
  cvPow(ImageIm,ImageIm,2);
  cvAdd(ImageRe,ImageIm,ImageRe,NULL);
  cvPow(ImageRe,ImageRe,0.5);
  cvMinMaxLoc(ImageRe,&m,&M,NULL,NULL, 0);
  scale = 255/(M - m);
  shift = -m * scale;
  //将shift加在ImageRe各元素按比例缩放的结果上，存储为ImageDst
  cvConvertScale(ImageRe,ImageDst,scale,shift);
  NamedWindow("傅里叶谱",0);
  imshow2("傅里叶谱",Image);
  NamedWindow("傅里叶逆变换",0);
  imshow2("傅里叶逆变换",ImageDst);
  //释放图像
  WaitKey(-1);
  IM_END(im);
  return 0;
}
