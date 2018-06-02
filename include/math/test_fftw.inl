
#include <stdio.h>

#include "fftw.inl"

int test_fftw()
{
  enum { N = 8 };
  int i;
  fftw_complex *din,*out;
  fftw_plan p;
  din = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
  out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
  if((din==NULL)||(out==NULL))
  {
    printf("Error:insufficient available memory\n");
  }
  else
  {
    for(i=0; i<N; i++)/*测试数据*/
    {
      din[i][0] = i+1;
      din[i][1] = 0;
    }
  }
  p = fftw_plan_dft_1d(N, din, out, FFTW_FORWARD,FFTW_ESTIMATE);
  fftw_execute(p); /* repeat as needed */
  fftw_destroy_plan(p);
  fftw_cleanup();
  for(i=0;i<N;i++)/*OUTPUT*/
  {
    printf("%f,%fi\n",din[i][0],din[i][1]);
  }
  printf("\n");
  for(i=0;i<N;i++)/*OUTPUT*/
  {
    printf("%f,%fi\n",out[i][0],out[i][1]);
  }
  
  if(din!=NULL) fftw_free(din);
  if(out!=NULL) fftw_free(out);
  //getchar();
  return 0;
}


#include "img/imgio.inl"
int test_fftw_img() {
  sys_chdir("D:/code/年画合成/合成素材");
  /* load original image */
  IM_BEGIN(im, 10);
  img_t* img_src = imread("test.JPG", 1, 1, im++);
  if (img_src == 0)
  {
    printf("cannot load file\n");
    return 0;
  }
  
  /* create new image for FFT & IFFT result */
  img_t *img_fft = cvCreateImage(im++, cSize(img_src->width, img_src->height), IMG_DEPTH_8U, 1);
  img_t *img_ifft = cvCreateImage(im++, cSize(img_src->width, img_src->height), IMG_DEPTH_8U, 1);
  
  /* get image properties */
  int width = img_src->width;
  int height = img_src->height;
  int step = img_src->widthStep;
  uchar *img_src_data = (uchar *)img_src->imageData;
  uchar *img_fft_data = (uchar *)img_fft->imageData;
  uchar *img_ifft_data = (uchar *)img_ifft->imageData;
  
  /* initialize arrays for fftw operations */
  fftw_complex *data_in = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * width * height);
  fftw_complex *fft = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * width * height);
  fftw_complex *ifft = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * width * height);
  
  /* create plans */
  fftw_plan plan_f = fftw_plan_dft_2d(height, width, data_in, fft, FFTW_FORWARD, FFTW_ESTIMATE);
  fftw_plan plan_b = fftw_plan_dft_2d(height, width, fft, ifft, FFTW_BACKWARD, FFTW_ESTIMATE);
  
  int i, j, k;
  /* load img_src's data to fftw input */
  for (i = 0, k = 0; i < height; ++i)
  {
    for (j = 0; j < width; ++j)
    {
      // method 1: 输入数据乘以(-1)^（i+j），即可中心化
      data_in[k][0] = /*pow(-1, i + j) * */(double)img_src_data[i * step + j];
      data_in[k][1] = 0.0;
      k++;
    }
  }
  
  /* perform FFT */
  fftw_execute(plan_f);
  
  /* perform IFFT */
  fftw_execute(plan_b);
  
  /* normalize FFT result */
  double maxx = 0.0, minn = 10000000000.0;
  for (i = 0; i < width * height; ++i)
  {
    fft[i][0] = log(sqrt(fft[i][0] * fft[i][0] + fft[i][1] * fft[i][1]));
    maxx = fft[i][0] > maxx ? fft[i][0] : maxx;
    minn = fft[i][0] < minn ? fft[i][0] : minn;
  }
  
  for (i = 0; i < width * height; ++i)
  {
    fft[i][0] = 255.0 * (fft[i][0] - minn) / (maxx - minn);
  }
  
  /* copy FFT result to img_fft's data */
  int i0, j0;
  for (i = 0, k = 0; i < height; ++i)
  {
    for (j = 0; j < width; ++j)
    {
      if (i < height / 2)
        i0 = i + height / 2;
      else
        i0 = i - height / 2;
      if (j < width / 2)
        j0 = j + width / 2;   // method 2
      else
        j0 = j - width / 2;
      
      img_fft_data[i * step + j] = (uchar)fft[/*k++*/i0 * width + j0][0];
    }
  }
  
  /* normalize IFFT result */
  for (i = 0; i < width * height; ++i)
  {
    ifft[i][0] /= width * height;
  }
  
  /* copy IFFT result to img_ifft's data */
  for (i = 0, k = 0; i < height; ++i)
  {
    for (j = 0; j < width; ++j)
    {
      img_ifft_data[i * step + j] = (uchar)ifft[k++][0];
    }
  }
  
  /* display images */
  imshow2("original_image", img_src);
  imshow2("FFT", img_fft);
  imshow2("IFFT", img_ifft);
  WaitKey(0);
  
  /* free memory */
  fftw_destroy_plan(plan_f);
  fftw_destroy_plan(plan_b);
  fftw_free(data_in);
  fftw_free(fft);
  fftw_free(ifft);
#if 0
#endif
  IM_END(im);
  return 0;
}
