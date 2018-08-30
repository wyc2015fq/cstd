
#include "cstd.h"
#include "img/imgio.inl"

int test_pca1() {
  double data[] = {
    -1, -1, 0, 2, 0, 
      -2, 0, 0, 1, 1
  };
  double avg[2] = {0};
  double evals[2];
  double evects[4];
  
  cvCalcPCA(data, 5, avg, evals, evects, 2, CC_PCA_DATA_AS_COL|CC_PCA_USE_AVG, 2, 2, 5);
  return 0;
}

#if 0
int imPCA(img_t* data, double* avg, double* evals, img_t* evects, CC_PCA_DATA_AS_ROW) {
  ISIZE size = iSIZE(data->w, data->h);
  cvCalcPCA(data->tt.f8, data->w, size, avg, evals, evects->w, )
  return 0;
}
#endif

int test_pca() {
  //获取了mean_face  
  int num_sample = 38;  
  int norm_row = 64, norm_col = 56;
  int num, i=1, j;
  int numvec = 20;
  img_t im[10] = {0};
  img_t* imgs = im+i++;
  img_t* mean_face = im+i++;
  img_t* mean_face_total = im+i++;
  img_t* data = im+i++;
  img_t* mean = im+i++;
  img_t* evals = im+i++;
  img_t* evecs = im+i++;
  img_t* eigen_face = im+i++;
  const char* path;
  vstr_t sv[1] = {0};
  path = "D:/pub/faceplusplus/out/std";
  sys_chdir(path);
  if (!file_exist("imgs.df")) {
    vstr_load("list.txt", sv);
    imload_images2(imgs, 1, sv->n, sv->v, 0);
    df_saveimage_filename("imgs.df", imgs);
  } else {
    df_loadimage_filename("imgs.df", imgs);
  }
  imresize2(imgs, 32, 32, imgs);
  num_sample = imgs->f;
  norm_row = imgs->h;
  norm_col = imgs->w;
  imsetsize(mean_face, norm_row, norm_col, 1, 1);
  imsetsize_f8(mean_face_total, norm_row, norm_col, 1);
  num = num_sample;
  //num = 2000;
  for (i = 0; i < num; i++) {
    uchar* imgs1 = imgs->tt.data + imgs->h*imgs->s*i;
    for (j = 0; j < norm_row * norm_col; j++) {  
      mean_face_total->tt.f8[j] += imgs1[j];
    }
  }
  for (j = 0; j < norm_row * norm_col; j++) {
    mean_face->tt.data[j] = (uchar)(mean_face_total->tt.f8[j] / (num));  
  }  
  
  //imwrite("C:/Users/dhj555/Desktop/YelaFaces/eigen_face/0001.jpg", mean_face);  
  //imshow2("eigen_face", mean_face);cvWaitKey(-1);
  
  numvec = 20;
  //1、初始化数据  
  imsetsize_f8(data, num_sample, norm_row * norm_col, 1);
  imsetsize_f8(mean, 1, norm_row * norm_col, 1);
  //每个数标志一个特征值
  imsetsize_f8(evals, 1, numvec, 1);
  //每行表示一个特征向量  
  imsetsize_f8(evecs, numvec, norm_row * norm_col, 1);
  
  for (i = 0; i < num_sample; i++) {
    double* data1 = data->tt.f8 + i * data->w;
    uchar* imgs1 = imgs->tt.data + imgs->h*imgs->s*i;
    for (j = 0; j < norm_row * norm_col; j++) {
      data1[j] = imgs1[j];
    }
  }
  
  //2、PCA处理  
  cvCalcPCA(data->tt.f8, data->w, mean->tt.f8, evals->tt.f8, evecs->tt.f8, evecs->w, CC_PCA_DATA_AS_ROW, numvec, data->h, data->w);  
  
  //3、选出前P个特征向量(主成份),然后投影,结果保存在pResult中，pResult中包含了P个系数  
  //img_t* pResult = imsetsize_f8(num_sample, 20, CC_32FC1);  
  //cvProjectPCA(data, mean, evecs, pResult);  
  
  //4、重构, 结果保存在pRecon中  
  //img_t* pRecon = imsetsize_f8(num_sample, norm_row*norm_col, CC_32FC1);  
  //cvBackProjectPCA(pResult, mean, evecs, pRecon);  
  
  //5、显示重构的图像  
  //img_t* mRecon = img_t*(pRecon);  
  
  mkdirs("eigen_face");
  //4、显示与保存特征向量  
  for (i = 0; i < numvec; i++)  
  {
    double min = FLT_MAX, max = FLT_MIN, span = 0.0;
    double* evecs1 = evecs->tt.f8 + evecs->w*i;

    for (j = 0; j < norm_row*norm_col; j++)  
    {
      double d = evecs1[j];
      if (d>max)  
        max = d;  
      if (d < min)  
        min = d;  
    }
    span = max - min;
    span = span<FLT_EPSILON ? 0 : 255.0/span;
    imsetsize(eigen_face, norm_row, norm_col, 1, 1);  
    for (j = 0; j < norm_row*norm_col; j++) {  
      double d = evecs1[j];
      eigen_face->tt.data[j] = (d - min) * span;
    }
    
    //imshow(eigen_face);
    imwrite(_format("eigen_face/%05d.bmp", i), eigen_face);  
  }
  imfrees(im, 10);
  return 0;
}
