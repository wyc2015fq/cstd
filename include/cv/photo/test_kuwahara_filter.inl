
#include "cstd.h"
#include "img/imgio.inl"
#include "kuwahara_filter.inl"

//积分数组  
double my_sum[4096][4096];//最大支持4096*4096分辨率的图像  
long double my_sqsum[4096][4096]; 

int get_mean(int i, int j, int neibourhood,int num)  
{  
  int val; 
  switch (num)  
  {
  case 1: val = (int)(my_sum[i+1][j+1] - my_sum[i - neibourhood+1][j+1] - my_sum[i+1][j - neibourhood+1] + my_sum[i - neibourhood+1][j - neibourhood+1]) / (neibourhood*neibourhood); break; 
  case 2: val = (int)(my_sum[i+1][j + neibourhood] - my_sum[i+1][j] - my_sum[i - neibourhood+1][j + neibourhood] + my_sum[i - neibourhood+1][j]) / (neibourhood*neibourhood); break; 
  case 3: val = (int)(my_sum[i + neibourhood][j+1] - my_sum[i][j+1] - my_sum[i + neibourhood][j - neibourhood+1] + my_sum[i][j - neibourhood+1]) / (neibourhood*neibourhood); break; 
  case 4: val = (int)(my_sum[i + neibourhood][j + neibourhood] - my_sum[i + neibourhood][j] - my_sum[i][j + neibourhood] + my_sum[i][j ]) / (neibourhood*neibourhood); break; 
  }  
  return val; 
}

int test_kuwahara_filter()  
{
  int kernel = 7;
  int i = 0, j;
  img_t im[10] = {0};
  img_t* image_in = im+i++;
  img_t* image_out = im+i++;

  sys_chdir("F:/pub/bin/photo/pics");
  imread("22c28182d36aeabc83302986d35f281c.jpg", 1, 1, image_in);

  imclone2(image_in, image_out); 
  //Mat image_in(10, 10, CV_8U); 
  
  const int height = image_in->h; 
  const int width = image_in->w; 
  
  if (kernel<3||kernel>15) {  
    printf("the kernel is out of range"); 
    return -1; 
  }  
  if (kernel/2==0) {  
    printf("the kernel must be odd"); 
    return -2; 
  }

  //计算积分数组
  for (i = 0; i < image_in->h; i++) {
    //得到第i行的首地址
    uchar* image_in1 = img_row(uchar, image_in, i); 
    for (j = 0; j < image_in->w; j++) {
      my_sum[i+1][j+1] = image_in1[j] + my_sum[i][j+1] + my_sum[i+1][j] - my_sum[i][j]; 
    }  
  }  
  
  for (i = 0; i < image_in->h; i++) {  
    //得到第i行的首地址  
    uchar* image_in1 = img_row(uchar, image_in, i); 
    for (j = 0; j < image_in->w; j++) {  
      my_sqsum[i+1][j+1] = image_in1[j] * image_in1[j] + my_sqsum[i + 1][j] + my_sqsum[i][j + 1] - my_sqsum[i][j]; 
    }  
  }  
  
  //
  //kuwahara 滤波器
  int neibourhood = kernel / 2 + 1;
  float N = neibourhood*neibourhood;
  for (i = neibourhood; i < height-neibourhood;i++) {
    uchar* image_out1 = img_row(uchar, image_out, i);
    for (j = neibourhood; j < width-neibourhood;j++) {
      //左上
      double L_T_sqsum = my_sqsum[i+1][j+1] - my_sqsum[i - neibourhood+1][j+1] - my_sqsum[i+1 ][j - neibourhood+1] + my_sqsum[i - neibourhood+1][j - neibourhood+1]; 
      double L_T_sum = my_sum[i+1][j+1] - my_sum[i - neibourhood+1][j+1] - my_sum[i+1][j - neibourhood+1] + my_sum[i - neibourhood+1][j - neibourhood+1]; 
      double left_top = L_T_sqsum - L_T_sum*L_T_sum / N; 
      
      //右上
      double R_T_sqsum = my_sqsum[i+1][j  + neibourhood] - my_sqsum[i+1][j] - my_sqsum[i - neibourhood+1][j + neibourhood] + my_sqsum[i - neibourhood+1][j]; 
      double R_T_sum = my_sum[i+1][j + neibourhood] - my_sum[i+1][j] - my_sum[i - neibourhood+1][j + neibourhood] + my_sum[i - neibourhood+1][j]; 
      double right_top = R_T_sqsum - R_T_sum*R_T_sum / N; 
      
      //left_bottom
      double L_B_sqsum = my_sqsum[i  + neibourhood][j+1] - my_sqsum[i][j+1] - my_sqsum[i + neibourhood][j - neibourhood+1] + my_sqsum[i][j - neibourhood+1]; 
      double L_B_sum = my_sum[i + neibourhood][j+1] - my_sum[i][j+1] - my_sum[i + neibourhood][j - neibourhood+1] + my_sum[i][j - neibourhood+1]; 
      double left_bottom = L_B_sqsum - L_B_sum*L_B_sum / N; 
      
      //right_bottom  
      double R_B_sqsum = my_sqsum[i + neibourhood][j + neibourhood] - my_sqsum[i][j  + neibourhood] - my_sqsum[i + neibourhood][j ] + my_sqsum[i ][j ]; 
      double R_B_sum = my_sum[i + neibourhood][j + neibourhood] - my_sum[i][j + neibourhood] - my_sum[i + neibourhood][j ] + my_sum[i ][j ]; 
      double right_bottom = R_B_sqsum - R_B_sum*R_B_sum / N; 
      
      double min_var = (left_top > right_top) ? right_top : left_top; 
      min_var = (left_bottom > min_var) ? min_var : left_bottom; 
      min_var = (right_bottom > min_var) ? min_var : right_bottom; 
      
      //
      uchar kuwahara_val = 0;
      if (min_var == left_top)  kuwahara_val = get_mean(i, j, neibourhood, 1);
      else if (min_var == right_top) kuwahara_val= get_mean( i, j, neibourhood, 2);
      else if (min_var == left_bottom) kuwahara_val = get_mean(i, j, neibourhood, 3);
      else if(min_var==right_bottom)  kuwahara_val = get_mean( i, j, neibourhood, 4); 
      
      if (kuwahara_val > 255)  kuwahara_val = 255; 
      else if (kuwahara_val < 0) kuwahara_val = 0; 
      
      image_out1[j] = kuwahara_val; 
    } 
  }  
  
  imshow2("original_image", image_in);
  imshow2("kuwahara", image_out);

  cvWaitKey(-1);
  imfrees(im, 10);
  return 1; 
}
   
