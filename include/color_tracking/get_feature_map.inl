
enum {
  ft_null,
  ft_gray,
  ft_cn,
};

#include "im2c.inl"

int get_feature_map(const img_t* im_patch, int features, const double* w2c, img_t* out) {
  
  // out = get_feature_map(im_patch, features, w2c)
  //
  // Extracts the given features from the image patch. 
  // w2c is the Color Names matrix, if used.
  
  // the names of the features that can be used
  int valid_features[2] = {ft_gray, ft_cn};
  
  // the dimension of the valid features
  int feature_levels[] = {1, 10};
  int num_feature_levels=0;
  
  enum { num_valid_features = countof(valid_features) };
  int used_features[num_valid_features] = {0};
  int i, j, level;
  
  // get the used features
  for (i = 0; i<num_valid_features; ++i) {
    used_features[i] = valid_features[i] == features;
    num_feature_levels += feature_levels[i] * used_features[i];
  }
  
  // total number of used feature levels
  //num_feature_levels = sum(feature_levels .* used_features);
  
  level = 0;
  
  // If grayscale image
  if (im_patch->c == 1) {
    // Features that are available for grayscale sequances
    
    // Grayscale values (image intensity)
    //out = single(im_patch)/255 - 0.5;//normalized to [-0.5 0.5]
    imsetsize(out, im_patch->h, im_patch->w, sizeof(double), 1);
    for (i=0; i<im_patch->h; ++i) {
      const uchar* im_patch1 = im_patch->tt.data + im_patch->s*i;
      double* out1 = (double*)(out->tt.data + out->s*i);
      for (j=0; j<im_patch->w; ++j) {
        out1[j] = im_patch1[i]/255. - 0.5;
      }
    }
  } else {
    // Features that are available for color sequances
    
    // allocate space (for speed)
    //out = zeros(size(im_patch, 1), size(im_patch, 2), num_feature_levels, 'single');
    imsetsize(out, im_patch->h, im_patch->w, num_feature_levels * sizeof(double), 1);
    
    // Grayscale values (image intensity)
    if (used_features[0]) {
      //out(:,:,level+1) = single(rgb2gray(im_patch))/255 - 0.5;
      for (i=0; i<im_patch->h; ++i) {
        const uchar* im_patch1 = im_patch->tt.data + im_patch->s*i;
        double* out1 = (double*)(out->tt.data + out->s*i);
        for (j=0; j<im_patch->w; ++j) {
          const uchar* bgr = im_patch1 + j*im_patch->c;
          int R = bgr[2], G = bgr[1], B = bgr[0];
          int gry = (int)(0.2989 * R + 0.5870 * G + 0.1140 * B+0.5);
          if (j==189) {
            int adsf=0;
          }
          out1[j] = gry/255. - 0.5;
        }
      }
      level = level + feature_levels[0];
      //imshow(im_patch);
      //imshowmat_u1_cn(im_patch, 1); imshowmat_f8(out); cvWaitKey(-1);
    }
    
    // Color Names
    if (used_features[1]) {
      // extract color descriptor
      //out(:,:,level+(1:10)) = im2c(single(im_patch), w2c, -2);//将图像块用RGB值映射为CN的可能性表示，即图像块每个元素为像素值对应的CN概率，该图像块即为CN概率图。
      im2c(im_patch, w2c, -2, out->tt.f8 + level, out->s/sizeof(double));
      level = level + feature_levels[1];
    }
  }
  
  return 0;
}
