int feature_projection(const img_t* x_npca, const img_t* x_pca, const img_t* projection_matrix, const img_t* cos_window, img_t* z) {

// z = feature_projection(x_npca, x_pca, projection_matrix, cos_window)
//
// Calculates the compressed feature map by mapping the PCA features with
// the projection matrix and concatinates this with the non-PCA features.
// The feature map is then windowed.

  if (NULL==x_pca || (x_pca->h*x_pca->w==0)) {
    // if no PCA-features exist, only use non-PCA
    //z = x_npca;
    imclone2(x_npca, z);
  } else {
    // get dimensions
    int height = cos_window->h, width = cos_window->w;
    int num_pca_in = projection_matrix->h, num_pca_out = projection_matrix->w;
    img_t x_proj_pca[1] = {0};
    
    // project the PCA-features using the projection matrix and reshape
    // to a window
    //x_proj_pca = reshape(x_pca * projection_matrix, [height, width, num_pca_out]);
    imsetsize(x_proj_pca, height, width, num_pca_out*sizeof(double), 1);
    mat_mul_AB(x_pca->h, projection_matrix->w, x_pca->w, x_pca->tt.f8, x_pca->s/sizeof(double), projection_matrix->tt.f8, projection_matrix->s/sizeof(double), x_proj_pca->tt.f8, projection_matrix->w);
    
    //imshowmat_f8_cn(x_proj_pca, 0); cvWaitKey(-1);
    // concatinate the feature windows
    if (NULL==x_npca) {
      //z = x_proj_pca;
      imclone2(x_proj_pca, z);
    } else {
        //z = cat(3, x_npca, x_proj_pca);
      imcat3(x_npca, x_proj_pca, z);
    }
    //imshowmat_f8_cn(x_proj_pca, 0); 
    //imshowmat_f8_cn(x_npca, 0); cvWaitKey(-1);
    imfree(x_proj_pca);
  }
  
  // do the windowing of the output
  //z = bsxfun(@times, cos_window, z);
  {
    int i, j, k, cn = z->c/sizeof(double);
    for (i=0; i<z->h; ++i) {
      double* z1 = (double*)(z->tt.data + z->s*i);
      double* cos_window1 = (double*)(cos_window->tt.data + cos_window->s*i);
      for (j=0; j<z->w; ++j) {
        double cc = cos_window1[j];
        for (k=0; k<cn; ++k) {
          *z1++ *= cc;
        }
      }
    }
  }
  //imshowmat_f8_cn(cos_window, 1);
  //imshowmat_f8_cn(z, 1); cvWaitKey(-1);

  return 0;
}