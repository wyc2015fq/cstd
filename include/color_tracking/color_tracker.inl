
#ifndef _COLOR_TRACKER_INL_
#define _COLOR_TRACKER_INL_

#define W2C_H  32768
#define W2C_W  10

#include "math/gencoswin.inl"
#include "img/cxdxt.inl"
#include "get_subwindow.inl"
#include "feature_projection.inl"
#include "cv/svd.inl"
#include "dense_gauss_kernel.inl"

typedef struct color_tracker_t {
  double padding;
  double output_sigma_factor;
  double sigma;
  double lambda;
  double learning_rate;
  double compression_learning_rate;
  int non_compressed_features;
  int compressed_features;
  int num_compressed_dim;
  int init_pos[2];
  int wsize[2];
  double* w2c;//32768*10
  img_t imbuf[30];
  double scale;
} color_tracker_t;

int color_tracker_init(color_tracker_t* params, IRECT rc) {
  int n = (RCW(&rc)+RCH(&rc));
  if (n<=0) {
    return 0;
  }
  //cvWaitKey(-1);
  params->padding = 1.5;         			   // extra area surrounding the target 搜索区域
  params->output_sigma_factor = 1./16;		   // spatial bandwidth (proportional to target)
  params->sigma = 0.2;         			   // gaussian kernel bandwidth
  params->lambda = 1e-2;					   // regularization (denoted "lambda" in the paper)
  params->learning_rate = 0.075;			   // learning rate for appearance model update scheme (denoted "gamma" in the paper)
  params->compression_learning_rate = 0.15;   // learning rate for the adaptive dimensionality reduction (denoted "mu" in the paper)
  params->non_compressed_features = ft_gray; // features that are not compressed, a cell with strings (possible choices: 'gray', 'cn')
  params->compressed_features = ft_cn;       // features that are compressed, a cell with strings (possible choices: 'gray', 'cn')
  params->num_compressed_dim = 2;             // the dimensionality of the compressed features
  
  params->scale = MIN(1, 200./n);
  rc = iRectScale(rc, params->scale);
  // x y 反过来
  params->wsize[1] = (int)(rc.r-rc.l);
  params->wsize[0] = (int)(rc.b-rc.t);
  
  params->init_pos[1] = (int)(rc.l+params->wsize[1]/2);
  params->init_pos[0] = (int)(rc.t+params->wsize[0]/2);
  return 0;
}
int color_tracker_free(color_tracker_t* params) {
  imfrees(params->imbuf, 30);
  return 0;
}

int color_tracker_run(color_tracker_t* params, int frame, const img_t* im, IRECT* prc) {
  //function [positions, fps] = color_tracker(params)
  //% [positions, fps] = color_tracker(params)
  //%% 本论文追踪的核心,包括样本预处理、样本训练，得到训练参数及参数的更新（本文重点）、检测过程，更新目标框、投影矩阵的更新（本文重点）
  //%% 输入：params为一结构体，详情看run_tracker.m
  //%% 输出：postions，每一帧计算得出的目标框位置、大小，每组数据结构为（x,y,width,height）
  //%% fps：frames per second
  
  //% parameters
  double padding = params->padding;
  double output_sigma_factor = params->output_sigma_factor;
  double sigma = params->sigma;
  double lambda = params->lambda;
  double learning_rate = params->learning_rate;
  double compression_learning_rate = params->compression_learning_rate;
  int non_compressed_features = params->non_compressed_features;
  int compressed_features = params->compressed_features;
  int num_compressed_dim = params->num_compressed_dim;
  int use_dimensionality_reduction=0;
  double* w2c = params->w2c;
  
  //video_path = params->video_path;
  //img_files = params->img_files;
  int* pos = params->init_pos;
  int* target_sz = params->wsize;
  int sz[2];
  double output_sigma;
  int i=0, j;
  img_t* im0 = params->imbuf;
  img_t* im00 = im0+i++;
  img_t* y = im0+i++;
  img_t* cos_window = im0+i++;
  img_t* hann0 = im0+i++;
  img_t* hann1 = im0+i++;
  img_t* xo_npca = im0+i++;
  img_t* xo_pca = im0+i++;
  img_t* z_npca = im0+i++;
  img_t* z_pca = im0+i++;
  img_t* old_cov_matrix = im0+i++;
  img_t* yf = im0+i++;
  img_t* projection_matrix = im0+i++;
  img_t* projection_variances = im0+i++;
  img_t* x = im0+i++;
  img_t* ker = im0+i++;
  img_t* kf = im0+i++;
  img_t* new_alphaf_num = im0+i++;
  img_t* new_alphaf_den = im0+i++;
  img_t* alphaf_num = im0+i++;
  img_t* alphaf_den = im0+i++;
  img_t* positions = im0+i++;
  
  imresize1(im, params->scale, im00);
  im = im00;
  //visualization = params->visualization;
  
  //num_frames = numel(img_files);
  
  //% load the normalized Color Name matrix
  //%  Color Name matrix is the mapping of every RGB value to one Color
  //%  Name,矩阵大小原为2^15 x 11,后归一化为2^15 x 10。（该矩阵每个元素为映射的可能性，得到方法是参考文献【23】，本文直接利用该矩阵）
  //temp = load('w2crs');
  
  use_dimensionality_reduction = compressed_features!=0;//%判断是否进行降维，如果是CN，就进行降维，由10维降到2维
  
  //% window size, taking padding into account
  //%扩大搜索框，padding = 1.0，所以搜索框为目标框的2倍，本文固定了搜索框大小
  sz[0] = (int)floor(target_sz[0] * (1 + padding));
  sz[1] = (int)floor(target_sz[1] * (1 + padding));
  
  if (frame==0) {
    imsetsize(y, sz[0], sz[1], 2*sizeof(double), 1);
    //% desired output (gaussian shaped), bandwidth proportional to target size
    output_sigma = sqrt(target_sz[0]*target_sz[1]) * output_sigma_factor; //%参考文献【9】提到的高斯函数时域带宽s=sqrt（m*n）/0.16
    //[rs, cs] = ndgrid((1:sz(1)) - floor(sz(1)/2), (1:sz(2)) - floor(sz(2)/2));//%目标框相对于搜索框，所有可能的shift，（row，col）
    //y = exp(-0.5 / output_sigma^2 * (rs.^2 + cs.^2));%对上述shift进行高斯处理，详情见本文section 2.
    //yf = single(fft2(y));
    for (i=0; i<sz[0]; ++i) {
      int half_0 = (sz[0]/2)-1;
      int half_1 = (sz[1]/2)-1;
      double inv_sig = -0.5/(output_sigma*output_sigma);
      double* y1 = (double*)(y->tt.data + i*y->s);
      for (j=0; j<sz[1]; ++j) {
        int rs = i-half_0;
        int cs = j-half_1;
        y1[j*2+0] = exp(inv_sig * (rs*rs + cs*cs));
        y1[j*2+1] = 0; //虚部
      }
    }
    imfft2(y, CV_DXT_FORWARD, yf);
    //imshowmat_f8_cn(yf, 0); cvWaitKey(-1);
    
    //cvShowMat("y", "%5.4I64f", sz[0], sz[1], y, sizeof(double)*sz[1], sizeof(double), -1);cvWaitKey(-1);
    
    //% store pre-computed cosine window
    //cos_window = single(hann(sz(1)) * hann(sz(2))');
    imsetsize(cos_window, sz[0], sz[1], sizeof(double), 1);
    imsetsize(hann0, sz[0], 1, sizeof(double), 1);
    imsetsize(hann1, sz[1], 1, sizeof(double), 1);
    gencoswin(sz[0], cw_hann, hann0->tt.f8);
    gencoswin(sz[1], cw_hann, hann1->tt.f8);
    for (i=0; i<sz[0]; ++i) {
      double* cos_window1 = (double*)(cos_window->tt.data + i*cos_window->s);
      for (j=0; j<sz[1]; ++j) {
        cos_window1[j] = hann0->tt.f8[i] * hann1->tt.f8[j];
      }
    }
  }
  
  if (frame > 0) {  //frame = 1时，由下面进行各参数的初始化，包括训练参数alphaf=alphaf_num/alphaf_den，投影矩阵，
    img_t im1[10] = {0};
    int i=0;
    img_t* zp = im1+i++;
    img_t* response = im1+i++;
    img_t* tt1 = im1+i++;
    img_t* tt2 = im1+i++;
    int row=0, col=0;
    double ttmax=-100000000000;
    // compute the compressed learnt appearance
    feature_projection(z_npca, z_pca, projection_matrix, cos_window, zp);
  //imshowmat_f8(zp); cvWaitKey(-1);
    // extract the feature map of the local image patch
    get_subwindow(im, pos, sz, non_compressed_features, compressed_features, w2c, xo_npca, xo_pca);
    
    //imshowmat_f8(xo_npca, 0); cvWaitKey(-1);
    // do the dimensionality reduction and windowing
    feature_projection(xo_npca, xo_pca, projection_matrix, cos_window, x);
    //imshowmat_f8_cn(x, 0); cvWaitKey(-1);
    
    // calculate the response of the classifier
    //kf = fft2(dense_gauss_kernel(sigma, x, zp));//高斯核函数处理
    //response = real(ifft2(alphaf_num .* kf ./ alphaf_den)); //检测，详情见【9】的公式（9）
    
    // target location is at the maximum response
    //[row, col] = find(response == max(response(:)), 1);
    //pos = pos - floor(sz/2) + [row, col];//update the position of the present frame on the base of the former frame
    
    dense_gauss_kernel(sigma, x, zp, ker);
    imfft2_real_cn(ker, CV_DXT_FORWARD, kf);
    imsetsize(tt1, kf->h, kf->w, sizeof(CvComplex64f), 1);
    for (i=0; i<kf->h; ++i) {
      CvComplex64f* tt11=(CvComplex64f*)(tt1->tt.data + tt1->s*i);
      CvComplex64f* kf1=(CvComplex64f*)(kf->tt.data + kf->s*i);
      CvComplex64f* alphaf_num1=(CvComplex64f*)(alphaf_num->tt.data + alphaf_num->s*i);
      CvComplex64f* alphaf_den1=(CvComplex64f*)(alphaf_den->tt.data + alphaf_den->s*i);
      for (j=0; j<kf->w; ++j) {
        CvComplex64f c = {0};
        FCOMPLEX_MUL(c, alphaf_num1[j], kf1[j]);
        FCOMPLEX_DIV(tt11[j], c, alphaf_den1[j]);
      }
    }
    imfft2(tt1, CV_DXT_INV_SCALE, tt2);
    imsetsize(response, kf->h, kf->w, sizeof(double), 1);
    for (i=0; i<kf->h; ++i) {
      double* response1=(double*)(response->tt.data + response->s*i);
      CvComplex64f* tt21=(CvComplex64f*)(tt2->tt.data + tt2->s*i);
      for (j=0; j<kf->w; ++j) {
        response1[j] = tt21[j].re;
        if (response1[j]>ttmax) {
          row = i, col = j;
          ttmax = response1[j];
        }
      }
    }
    //printf("frame=%d %d %d %f\n", frame, row, col, ttmax);
    pos[0] = pos[0] - sz[0]/2 + row+1;
    pos[1] = pos[1] - sz[1]/2 + col+1;
    //imshowmat_f8_cn(response, 0); imshowmat_f8_cn(kf, 0); imshowmat_f8_cn(alphaf_num, 0); imshowmat_f8_cn(alphaf_den, 0); cvWaitKey(-1);
    //imshowmat_f8_cn(response, 0); cvWaitKey(-1);
    imfrees2(im1);
  }
  
  // extract the feature map of the local image patch to train the classifer
  get_subwindow(im, pos, sz, non_compressed_features, compressed_features, w2c, xo_npca, xo_pca);
  //imshowmat_f8(xo_npca); imshowmat_f8(xo_pca); cvWaitKey(-1);
  if (frame == 0) {
    // initialize the appearance
    //z_npca = xo_npca;
    //z_pca = xo_pca;
    imclone2(xo_npca, z_npca);
    imclone2(xo_pca, z_pca);
    
    // set number of compressed dimensions to maximum if too many
    num_compressed_dim = MIN(num_compressed_dim, xo_pca->w);
  } else {
    //imshowmat_f8_cn(xo_npca, 0); 
    //imshowmat_f8_cn(z_npca, 0); cvWaitKey(-1);
    // update the appearance
    //z_npca = (1 - learning_rate) * z_npca + learning_rate * xo_npca; //本文的公式（5c）
    //z_pca = (1 - learning_rate) * z_pca + learning_rate * xo_pca;
    imscale2(z_npca, 1 - learning_rate, xo_npca, learning_rate, 0, z_npca);
    imscale2(z_pca, 1 - learning_rate, xo_pca, learning_rate, 0, z_pca);
    //imshowmat_f8_cn(z_npca, 0); cvWaitKey(-1);
  }
  
  // if dimensionality reduction is used: update the projection matrix
  if (use_dimensionality_reduction) {
    // compute the mean appearance
    //data_mean = mean(z_pca, 1);
    img_t im1[10] = {0};
    img_t* data_mean = im1;
    img_t* data_matrix = im1+1;
    img_t* cov_matrix = im1+2;
    img_t* pca_basis = im1+3;
    img_t* pca_variances = im1+4;
    immean_f8(z_pca, 1, data_mean);
    //imshowmat_f8(data_mean); cvWaitKey(-1);
    // substract the mean from the appearance to get the data matrix
    //data_matrix = bsxfun(@minus, z_pca, data_mean);
    imsetsize(data_matrix, z_pca->h, z_pca->w, sizeof(double), 1);
    mat_minus_dim_f8(z_pca->h, z_pca->w, z_pca->tt.f8, z_pca->s/sizeof(double), data_mean->tt.f8, 1, data_matrix->tt.f8, data_matrix->s/sizeof(double));
    
    //imshowmat_f8(data_matrix); cvWaitKey(-1);
    
    // calculate the covariance matrix
    //cov_matrix = 1/(prod(sz) - 1) * (data_matrix' * data_matrix);
    imsetsize(cov_matrix, data_matrix->w, data_matrix->w, sizeof(double), 1);
    mat_mul_ATA(data_matrix->h, data_matrix->w, data_matrix->tt.f8, data_matrix->s/sizeof(double), cov_matrix->tt.f8, cov_matrix->s/sizeof(double));
    mat_scale(cov_matrix->h, cov_matrix->w, cov_matrix->tt.f8, cov_matrix->s/sizeof(double), 1./(sz[0]*sz[1]-1), 0, cov_matrix->tt.f8, cov_matrix->s/sizeof(double));
    //imshowmat_f8(cov_matrix); cvWaitKey(-1);
    // calculate the principal components (pca_basis) and corresponding variances
    //test_svd();
    if (frame == 0) {
      //[pca_basis, pca_variances,] = svd(cov_matrix);
      imsvd(cov_matrix, pca_basis, pca_variances, 0, 0);
    } else {
      img_t im2[1] = {0};
      //[pca_basis, pca_variances, ] = svd((1 - compression_learning_rate) * old_cov_matrix + compression_learning_rate * cov_matrix);
      imscale2(old_cov_matrix, 1 - compression_learning_rate, cov_matrix, compression_learning_rate, 0, im2);
      imsvd(im2, pca_basis, pca_variances, 0, 0);
      imfree(im2);
      //imshowmat_f8(pca_basis); cvWaitKey(-1);
    }
    
    // calculate the projection matrix as the first principal
    // components and extract their corresponding variances
    //projection_matrix = pca_basis(:, 1:num_compressed_dim);
    //projection_variances = pca_variances(1:num_compressed_dim, 1:num_compressed_dim);
    {
      img_t im0[1] = {0};
      imref(pca_basis, 0, 0, num_compressed_dim, pca_basis->h,im0);
      imclone2(im0, projection_matrix);
      imref(pca_variances, 0, 0, num_compressed_dim, 1,im0);
      imclone2(im0, projection_variances);
      //imshowmat_f8(projection_matrix); imshowmat_f8(projection_variances); cvWaitKey(-1);
    }
    
    if (frame == 0) {
      // initialize the old covariance matrix using the computed
      // projection matrix and variances
      //old_cov_matrix = projection_matrix * projection_variances * projection_matrix';
      imsetsize(old_cov_matrix, pca_basis->h, pca_basis->w, sizeof(double), 1);
      mat_mul_AATw(projection_matrix->h, projection_matrix->w, projection_matrix->tt.f8, projection_matrix->s/sizeof(double), projection_variances->tt.f8, old_cov_matrix->tt.f8, old_cov_matrix->s/sizeof(double));
      //imshowmat_f8(old_cov_matrix); cvWaitKey(-1);
    } else {
      // update the old covariance matrix using the computed
      // projection matrix and variances
      //old_cov_matrix = (1 - compression_learning_rate) * old_cov_matrix + compression_learning_rate * (projection_matrix * projection_variances * projection_matrix');
      img_t im2[1] = {0};
      imsetsize(im2, pca_basis->h, pca_basis->w, sizeof(double), 1);
      imsetsize(old_cov_matrix, pca_basis->h, pca_basis->w, sizeof(double), 1);
      mat_mul_AATw(projection_matrix->h, projection_matrix->w, projection_matrix->tt.f8, projection_matrix->s/sizeof(double), projection_variances->tt.f8, old_cov_matrix->tt.f8, old_cov_matrix->s/sizeof(double));
      imscale2(old_cov_matrix, 1 - compression_learning_rate, im2, compression_learning_rate, 0, old_cov_matrix);
      imfree(im2);
    }
    imfrees(im1, 10);
    }
  
  // project the features of the new appearance example using the new
  // projection matrix
  feature_projection(xo_npca, xo_pca, projection_matrix, cos_window, x);
  
  // calculate the new classifier coefficients
  // 参考文献【9】中，用于训练参数a的求解
  dense_gauss_kernel(sigma, x, 0, ker);
  imfft2_real_cn(ker, CV_DXT_FORWARD, kf);
  //imshowmat_f8_cn(yf, 0); imshowmat_f8_cn(kf, 0); cvWaitKey(-1);
  
  //【9】中的公式（17），用于求解训练参数a
  //new_alphaf_num = yf .* kf;//【23】中的公式（17）
  //new_alphaf_den = kf .* (kf + lambda);
  imsetsize(new_alphaf_num, yf->h, yf->w, sizeof(CvComplex64f), 1);
  imsetsize(new_alphaf_den, yf->h, yf->w, sizeof(CvComplex64f), 1);
  for (i=0; i<yf->h; ++i) {
    CvComplex64f* new_alphaf_num1 = (CvComplex64f*)(new_alphaf_num->tt.data + new_alphaf_num->s*i);
    CvComplex64f* new_alphaf_den1 = (CvComplex64f*)(new_alphaf_den->tt.data + new_alphaf_den->s*i);
    CvComplex64f* yf1 = (CvComplex64f*)(yf->tt.data + yf->s*i);
    CvComplex64f* kf1 = (CvComplex64f*)(kf->tt.data + kf->s*i);
    for (j=0; j<yf->w; ++j) {
      CvComplex64f c;
      FCOMPLEX_MUL(new_alphaf_num1[j], yf1[j], kf1[j]);
      FCOMPLEX_SET(c, kf1[j].re + lambda, kf1[j].im);
      FCOMPLEX_MUL(new_alphaf_den1[j], kf1[j], c);
    }
  }
  //imshowmat_f8_cn(new_alphaf_num, 1); imshowmat_f8_cn(new_alphaf_den, 0); cvWaitKey(-1);
  
  //解训练参数a分子、分母的求解及更新本文公式（5）
  if (frame == 0) {
    // first frame, train with a single image
    //alphaf_num = new_alphaf_num;
    //alphaf_den = new_alphaf_den;
    imclone2(new_alphaf_num, alphaf_num);
    imclone2(new_alphaf_den, alphaf_den);
  } else {
    // subsequent frames, update the model
    //alphaf_num = (1 - learning_rate) * alphaf_num + learning_rate * new_alphaf_num;//update the numeratorof a，本文公式（5a）
    //alphaf_den = (1 - learning_rate) * alphaf_den + learning_rate * new_alphaf_den;//update the denominator of a，本文公式（5b）
    imscale2(alphaf_num, 1 - learning_rate, new_alphaf_num, learning_rate, 0, alphaf_num);
    imscale2(alphaf_den, 1 - learning_rate, new_alphaf_den, learning_rate, 0, alphaf_den);
    //imshowmat_f8_cn(new_alphaf_num, 0); imshowmat_f8_cn(new_alphaf_den, 0); cvWaitKey(-1);
    //imshowmat_f8_cn(alphaf_num, 0); imshowmat_f8_cn(alphaf_den, 0); cvWaitKey(-1);
  }
  if (prc) {
    int r1[4];
    r1[0] = pos[0];
    r1[1] = pos[1];
    r1[2] = target_sz[0];
    r1[3] = target_sz[1];
    {
      int r[4] = {r1[1] - r1[3]/2, r1[0] - r1[2]/2, r1[3], r1[2]};
      *prc = iRECT(r[0], r[1], r[0]+r[2], r[1]+r[3]);
      *prc = iRectScale(*prc, 1./params->scale);
    }
  }
  return 0;
}

#endif // _COLOR_TRACKER_INL_
