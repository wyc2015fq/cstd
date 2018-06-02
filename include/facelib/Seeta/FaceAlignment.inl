
#ifndef _FACEALIGNMENT_INL_
#define _FACEALIGNMENT_INL_

#include <string.h>
#include <algorithm>

typedef unsigned char BYTE;

class SIFT{
 public:
  SIFT();
  ~SIFT();

  /** Initialize the SIFT extractor.
	  *  @param im_width The width of the input image
	  *  @param im_height The height of the input image
	  *  @param patch_size The size of one patch for extracting SIFT
	  *  @param grid_spacing The stride for extracting SIFT
	  */
  void InitSIFT(int im_width, int im_height, int patch_size, int grid_spacing);

  /** Compute SIFT feature
  *  @param gray_im A grayscale image
  *  @param[out] sift_feature The output SIFT feature
  */
  void CalcSIFT(BYTE* gray_im, double* sift_feature);

 private:
  /** Implement convolutional function "filter2" same in Matlab.
  *  @param gray_im A grayscale image
  *  @param kernel A convolutional kernel
  *  @param kernel_size The size of convolutional kernel
  *  @param[out] filter_im The output image map after convolution
  */
  void filter2(double* gray_im, double* kernel, int kernel_size, double* filter_im);

  /** Sparse convolution for speed-up
  *  @param gray_im A grayscale image
  *  @param kernel A convolutional kernel
  *  @param kernel_size The size of convolutional kernel
  *  @param[out] filter_im The output image map after sparse convolution
  */
  void SparseFilter2(double* gray_im, double* kernel, int kernel_size, double* filter_im);

  /** Calculate image orientation
  *  @param gray_im A grayscale image
  *  @param[out] image_orientation The output image orientation
  */
  void ImageOrientation(double* gray_im, double* image_orientation);

  /** Calculate image orientation
  *  @param image_orientation A image orientation map
  *  @param[out] conv_im The output convolutional image
  */
  void ConvImage(double* image_orientation, double* conv_im);

  private:
  struct SIFTParam
  {
	  int image_width;
	  int image_height;
	  int patch_size;
	  int grid_spacing;
	  int angle_nums;
	  int bin_nums;

	  int image_pixel;
	  int sample_nums;
	  int sample_pixel;
	  int patch_cnt_width;
	  int patch_cnt_height;
	  int patch_dims;
	  int image_dims;

	  int filter_size;
	  double sigma;
	  double alpha;
		
  };

  SIFTParam param;

  static double delta_gauss_x[25];
  static double delta_gauss_y[25];

};


//#define M_PI 3.1415926
double SIFT::delta_gauss_x[25] = 
{0.0284161904936934,0.0260724940559495,0,-0.0260724940559495,-0.0284161904936934,
0.127352530356230,0.116848811647003,0,-0.116848811647003,-0.127352530356230,
0.209968825675801,0.192651121218447,0,-0.192651121218447,-0.209968825675801,
0.127352530356230,0.116848811647003,0,-0.116848811647003,-0.127352530356230,
0.0284161904936934,0.0260724940559495,0,-0.0260724940559495,-0.0284161904936934};

double SIFT::delta_gauss_y[25] = 
{0.0284161904936934,0.127352530356230,0.209968825675801,0.127352530356230,0.0284161904936934,
0.0260724940559495,0.116848811647003,0.192651121218447,0.116848811647003,0.0260724940559495,
0,0,0,0,0,
-0.0260724940559495,-0.116848811647003,-0.192651121218447,-0.116848811647003,-0.0260724940559495,
-0.0284161904936934,-0.127352530356230,-0.209968825675801,-0.127352530356230,-0.0284161904936934};

SIFT::SIFT(void)
{
}


SIFT::~SIFT(void)
{
}

/** Initialize the SIFT extractor.
 *  @param im_width The width of the input image
 *  @param im_height The height of the input image
 *  @param patch_size The size of one patch for extracting SIFT
 *  @param grid_spacing The stride for extracting SIFT
 */
void SIFT::InitSIFT(int im_width, int im_height, int patch_size, int grid_spacing)
{
  param.image_width = im_width;
  param.image_height = im_height;
  param.patch_size = patch_size;
  param.grid_spacing = grid_spacing;
  param.angle_nums   = 8;
  param.bin_nums     = 4;

  param.image_pixel  = param.image_width * param.image_height;      
  param.sample_nums  = param.bin_nums * param.bin_nums;    
  param.sample_pixel = param.patch_size / param.bin_nums;   
  param.patch_cnt_width = (param.image_width - param.patch_size) / param.grid_spacing + 1;   
  param.patch_cnt_height = (param.image_height - param.patch_size) / param.grid_spacing + 1;  
  param.patch_dims = param.sample_nums * param.angle_nums;                      
  param.image_dims = param.patch_cnt_width * param.patch_cnt_height * param.patch_dims;  

  param.filter_size = 5;
  param.sigma = 1;
  param.alpha = 3;	
}

/** Implement convolutional function "filter2" same in Matlab.
 *  @param gray_im A grayscale image
 *  @param kernel A convolutional kernel
 *  @param kernel_size The size of convolutional kernel
 *  @param[out] filter_im The output image map after convolution
 */
void SIFT::filter2(double* gray_im, double* kernel, int kernel_size, double* filter_im)
{
  // Padding the image
  int pad_size = (kernel_size - 1) / 2;
  double* gray_img_ex = new double[(param.image_width + (kernel_size - 1)) * (param.image_height + (kernel_size - 1))];
	int i, j;
  for(i = 0; i < pad_size; i++)
  {
	  for (j = 0; j < param.image_width + (kernel_size - 1); j++)
	  {
		  gray_img_ex[i * (param.image_width + (kernel_size - 1)) + j] = 0;
	  }
  }

  for (i = param.image_height + pad_size; i < param.image_height + (kernel_size - 1); i++)
  {
	  for (j = 0; j < param.image_width + (kernel_size - 1); j++)
	  {
		  gray_img_ex[i * (param.image_width + (kernel_size - 1)) + j] = 0;
	  }
  }

  for(i = pad_size; i < param.image_height + pad_size; i++)
  {
	  for(j = 0; j < pad_size; j++)
	  {
		  gray_img_ex[i * (param.image_width + (kernel_size - 1)) + j] = 0;
	  }
	  for (j = param.image_width + pad_size; j < param.image_width + (kernel_size - 1); j++)
	  {
		  gray_img_ex[i * (param.image_width + (kernel_size - 1)) + j] = 0;
	  }
	  for(j = pad_size; j < param.image_width + pad_size; j++)
	  {
		  gray_img_ex[i * (param.image_width + (kernel_size - 1)) + j] = gray_im[(i - pad_size) * param.image_width + (j - pad_size)];
	  }
  }

  // Sliding filter on padding image
  for(i = 0; i < param.image_height; i++)
  {
	  for(j = 0; j < param.image_width; j++)
	  {
		  double tmp = 0.000000;

		  for (int ki = 0; ki < kernel_size; ki++)
		  {
			  for (int kj = 0; kj < kernel_size; kj++)
			  {
				  double tmp1 = gray_img_ex[(i + ki) * (param.image_width + (kernel_size - 1)) + (j + kj)];
				  tmp += tmp1 * kernel[ki * kernel_size + kj];
			  }
		  }
		  filter_im[i * param.image_width + j] = tmp;
	  }
  }
  delete [] gray_img_ex;
}

/** Sparse convolution for speed-up
 *  @param gray_im A grayscale image
 *  @param kernel A convolutional kernel
 *  @param kernel_size The size of convolutional kernel
 *  @param[out] filter_im The output image map after sparse convolution
 */
void SIFT::SparseFilter2(double* gray_im, double* kernel, int kernel_size, double* filter_im)
{
  // Padding the image
  int pad_size = (kernel_size-1)/2;
  double* gray_img_ex = new double[(param.image_width + (kernel_size-1)) * (param.image_height + (kernel_size-1))];
	int i, j;
  for(i = 0; i < pad_size; i++)
  {
	  for(j = 0; j < param.image_width + (kernel_size-1); j++)
	  {
		  gray_img_ex[i * (param.image_width + (kernel_size-1)) + j] = 0;
	  }
  }

  for(i = param.image_height + pad_size; i < param.image_height + (kernel_size-1); i++)
  {
	  for(j = 0; j < param.image_width + (kernel_size-1); j++)
	  {
		  gray_img_ex[i * (param.image_width + (kernel_size-1)) + j] = 0;
	  }
  }

  for(i = pad_size; i < param.image_height + pad_size; i++)
  {
	  for(j = 0; j < pad_size; j++)
	  {
		  gray_img_ex[i * (param.image_width + (kernel_size-1)) + j] = 0;
	  }
	  for(j = param.image_width + pad_size; j < param.image_width + (kernel_size-1); j++) 
	  {
		  gray_img_ex[i * (param.image_width + (kernel_size-1)) + j] = 0;
	  }
	  for(j = pad_size; j < param.image_width + pad_size; j++)
	  {
		  gray_img_ex[i * (param.image_width + (kernel_size - 1)) + j] = gray_im[(i - pad_size) * param.image_width + (j - pad_size)];
	  }
  }

  // Sliding filter on padding image
  for(i = 0; i < param.image_height; i += param.sample_pixel)
  {
	  for(j = 0; j < param.image_width; j += param.sample_pixel)
	  {
		  double tmp = 0.000000;

		  for(int ki = 0; ki < kernel_size; ki++)
		  {
			  for(int kj = 0; kj < kernel_size; kj++)
			  {
				  double tmp1 = gray_img_ex[(i + ki) * (param.image_width + (kernel_size - 1)) + (j + kj)];
				  tmp += tmp1 * kernel[ki * kernel_size + kj];
			  }
		  }
		  filter_im[i * param.image_width + j] = tmp;
	  }
  }
  delete [] gray_img_ex;
}

/** Calculate image orientation
 *  @param image_orientation A image orientation map
 *  @param[out] conv_im The output convolutional image
 */
void SIFT::ConvImage(double* image_orientation, double* conv_im)
{
  double* weight = new double[param.patch_size];
  double* kernel = new double[param.patch_size * param.patch_size];

  for(int k = 0; k < param.patch_size; k++)
  {
	  weight[k] = abs(k - double(param.patch_size - 1)/2)/(param.sample_pixel);

	  if(weight[k] <= 1)
		  weight[k] = 1 - weight[k];
	  else
		  weight[k] = 0;
  }

  for(int i = 0; i < param.patch_size; i++)
  {
	  for(int j = 0; j < param.patch_size; j++)
	  {
		  kernel[i * param.patch_size + j] = weight[i] * weight[j];
	  }
  }

  double* angle_im = new double[param.image_pixel];
  double* angle_conv_im = new double[param.image_pixel];

  for(int index = 0; index < param.angle_nums; index++)
  {
	  memset(angle_im, 0, param.image_pixel * sizeof(double));
	  memcpy(angle_im, &image_orientation[index * param.image_pixel], param.image_pixel * sizeof(double));
	  SparseFilter2(angle_im, kernel, param.patch_size, angle_conv_im);
	  memcpy(&conv_im[index * param.image_pixel], angle_conv_im, param.image_pixel * sizeof(double));
  }
	
  delete [] weight;
  delete [] kernel;
  delete [] angle_im;
  delete [] angle_conv_im;
}

/** Compute SIFT feature
 *  @param gray_im A grayscale image
 *  @param[out] sift_feature The output SIFT feature
 */
void SIFT::CalcSIFT(BYTE* gray_im, double* sift_feature)
{
  double* lf_gray_im = new double[param.image_pixel];
  double max = 0.000001;
  int pt;
  for (pt = 0; pt < param.image_pixel; pt++)
  {
	  lf_gray_im[pt] = gray_im[pt];
	  if (lf_gray_im[pt] > max)
		  max = lf_gray_im[pt];
  }

  for (pt = 0; pt < param.image_pixel; pt++)
  {
	  lf_gray_im[pt] = lf_gray_im[pt] / max;
  }

  double* im_orientation = new double[param.image_pixel * param.angle_nums];
  double* conv_im = new double[param.image_pixel * param.angle_nums];
  memset(conv_im, 0, param.image_pixel * param.angle_nums * sizeof(double));

  ImageOrientation(lf_gray_im, im_orientation);
  ConvImage(im_orientation, conv_im);

  // Generate denseSIFT feature vector
  double* patch_feature = new double[param.patch_dims];
  int patch_cnt = 0;

  // Sliding windows on overlapping patches. (px,py) are centroids
  for (int location_x = param.patch_size / 2; location_x <= param.image_height - (param.patch_size / 2); location_x += param.grid_spacing)
  {
	  for (int location_y = param.patch_size / 2; location_y <= param.image_width - (param.patch_size / 2); location_y += param.grid_spacing)
	  {
		  memset(patch_feature, 0, param.patch_dims * sizeof(double));

		  double l2_norm = 0.000001;
		  int Point_cnt = 0;

		  for (int p_x = -param.patch_size / 2; p_x <= param.patch_size / 2 - param.sample_pixel; p_x += param.sample_pixel)
		  {
			  for (int p_y = -param.patch_size / 2; p_y <= param.patch_size / 2 - param.sample_pixel; p_y += param.sample_pixel)
			  {
				  int i = location_x + p_x;
				  int j = location_y + p_y;

				  for (int index = 0; index < param.angle_nums; index++)
				  {
					  patch_feature[Point_cnt] = conv_im[index * param.image_pixel + j * param.image_height + i];
					  l2_norm += pow(patch_feature[Point_cnt], 2);
					  Point_cnt += 1;
				  }
			  }
		  }
		  // Patch-wise L2-norm
		  double norm = 1.0 / sqrt(l2_norm);
		  for (int pt = 0; pt < param.patch_dims; pt++)
		  {
			  patch_feature[pt] = patch_feature[pt] * norm;
		  }

		  memcpy(&sift_feature[patch_cnt * param.patch_dims], patch_feature, param.patch_dims * sizeof(double));
		  patch_cnt += 1;
	  }
  }

  delete[] lf_gray_im;
  delete[] im_orientation;
  delete[] conv_im;
  delete[] patch_feature;
}


/** Calculate image orientation
 *  @param image_orientation A image orientation map
 *  @param[out] conv_im The output convolutional image
 */
void SIFT::ImageOrientation(double* gray_im, double* image_orientation)
{
  double* im_vert_edge = new double[param.image_pixel];
  double* im_hori_edge = new double[param.image_pixel];

  filter2(gray_im, delta_gauss_x, param.filter_size, im_vert_edge);
  filter2(gray_im, delta_gauss_y, param.filter_size, im_hori_edge);

  double* im_magnitude = new double[param.image_pixel];
  double* im_cos_theta = new double[param.image_pixel];
  double* im_sin_theta = new double[param.image_pixel];

  for (int i = 0; i < param.image_height; i++)
  {
	  for (int j = 0; j < param.image_width; j++)
	  {
		  double tmpV = im_vert_edge[i * param.image_width + j];
		  double tmpH = im_hori_edge[i * param.image_width + j];
		  double tmpMagnitude = sqrt(pow(tmpV, 2) + pow(tmpH, 2));
		  im_magnitude[i * param.image_width + j] = tmpMagnitude;
		  im_cos_theta[i * param.image_width + j] = tmpV / tmpMagnitude;
		  im_sin_theta[i * param.image_width + j] = tmpH / tmpMagnitude;
	  }
  }

  delete[] im_vert_edge;
  delete[] im_hori_edge;

  double cos_array[8];
  double sin_array[8];
  cos_array[0] = 1.0;
  cos_array[1] = 0.7071;
  cos_array[2] = 0.0;
  cos_array[3] = -0.7071;
  cos_array[4] = -1.0;
  cos_array[5] = -0.7071;
  cos_array[6] = 0.0;
  cos_array[7] = 0.7071;

  sin_array[0] = 0.0;
  sin_array[1] = 0.7071;
  sin_array[2] = 1.0;
  sin_array[3] = 0.7071;
  sin_array[4] = 0.0;
  sin_array[5] = -0.7071;
  sin_array[6] = -1.0;
  sin_array[7] = -0.7071;
  for (int index = 0; index < param.angle_nums; index++)
  {
	  for (int pt = 0; pt < param.image_pixel; pt++)
	  {
		  double tmp1 = im_cos_theta[pt] * cos_array[index] + im_sin_theta[pt] * sin_array[index];
		  double tmp = pow(tmp1,3);

		  if (tmp > 0)
			  tmp = tmp;
		  else
			  tmp = 0;
		  image_orientation[index * param.image_pixel + pt] = tmp * im_magnitude[pt];
	  }
  }

  delete[] im_magnitude;
  delete[] im_cos_theta;
  delete[] im_sin_theta;
}

class CCFAN{
 public:
  /** A constructor.
   *  Initialize basic parameters.
   */
  CCFAN(void);

  /** A destructor which should never be called explicitly.
   *  Release all dynamically allocated resources.
   */
  ~CCFAN(void);

  /** Initialize the facial landmark detection model.
    *  @param model_path Path of the model file, either absolute or relative to
    *                   the working directory.
    */
  void InitModel(const char *model_path);

  /** Detect five facial landmarks, i.e., two eye centers, nose tip and two mouth corners.
    *  @param gray_im A grayscale image
    *  @param im_width The width of the inpute image
    *  @param im_height The height of the inpute image
    *  @param face_loc The face bounding box
    *  @param[out] facial_loc The locations of detected facial points
    */
  void FacialPointLocate(const unsigned char *gray_im, int im_width, int im_height, FaceInfo face_loc, float *facial_loc);

 private:
  /** Extract shape indexed SIFT features.
    *  @param gray_im A grayscale image
    *  @param im_width The width of the inpute image
    *  @param im_height The height of the inpute image
    *  @param face_shape The locations of facial points
    *  @param patch_size The size of the patch used for extracting SIFT feature
    *  @param[out] sift_fea the extracted shape indexed SIFT features which are concatenated into a vector
    */
  void TtSift(const unsigned char *gray_im, int im_width, int im_height, float *face_shape, int patch_size, double *sift_fea);

  /** Extract a image patch which is centered at point(point_x, point_y) with a given patch size.
  *  @param gray_im A grayscale image
  *  @param im_width The width of the inpute image
  *  @param im_height The height of the inpute image
  *  @param point_x The X coordinate of one point
  *  @param point_y The Y coordinate of one point
  *  @param patch_size The size of the extracted patch
  *  @param[out] sub_img A grayscale image patch
  */
  void GetSubImg(const unsigned char *gray_im, int im_width, int im_height, float point_x, float point_y, int patch_size, BYTE *sub_img);

  /** Resize the image by bilinear interpolation.
    *  @param src_im A source image in grayscale
    *  @param src_width The width of the source image
    *  @param src_height The height of the source image
    *  @param[out] dst_im The target image in grayscale
    *  @param dst_width The width of the target image
    *  @param dst_height The height of the target image
    */
  bool ResizeImage(const unsigned char *src_im, int src_width, int src_height,
    unsigned char* dst_im, int dst_width, int dst_height);

 private:
  /*The number of facial points*/
  int pts_num_;
  /*The dimension of the shape indexed features*/
  int fea_dim_;
  /*The mean face shape containing five landmarks*/
  float *mean_shape_;

  /*The parameters of the first local stacked autoencoder network*/
  float **lan1_w_;
  float **lan1_b_;
  int *lan1_structure_;
  int lan1_size_;

  /*The parameters of the second local stacked autoencoder network*/
  float **lan2_w_;
  float **lan2_b_;
  int *lan2_structure_;
  int lan2_size_;

};


/** A constructor.
  *  Initialize basic parameters.
  */
CCFAN::CCFAN(void)
{
  pts_num_ = 5;
  fea_dim_ = pts_num_ * 128;

  lan1_w_ = NULL;
  lan1_b_ = NULL;
  lan1_structure_ = NULL;

  lan2_w_ = NULL;
  lan2_b_ = NULL;
  lan2_structure_ = NULL;

  mean_shape_ = NULL;
}

/** A destructor which should never be called explicitly.
  *  Release all dynamically allocated resources.
  */
CCFAN::~CCFAN(void)
{
  if (lan1_structure_ != NULL)
  {
    delete[]lan1_structure_;
    lan1_structure_ = NULL;
  }
  if (lan1_w_ != NULL)
  {
    for (int i = 0; i < lan1_size_ - 1; i++)
    {
      delete[](lan1_w_[i]);
      delete[](lan1_b_[i]);
    }
    delete[]lan1_w_;
    delete[]lan1_b_;
    lan1_w_ = NULL;
    lan1_b_ = NULL;
  }

  if (lan2_structure_ != NULL)
  {
    delete[]lan2_structure_;
  }
  if (lan2_w_ != NULL)
  {
    for (int i = 0; i < lan2_size_ - 1; i++)
    {
      delete[](lan2_w_[i]);
      delete[](lan2_b_[i]);
    }
    delete[]lan2_w_;
    delete[]lan2_b_;
    lan2_w_ = NULL;
    lan2_b_ = NULL;
  }

  if (mean_shape_)
  {
    delete[]mean_shape_;
    mean_shape_ = NULL;
  }
}

/** Initialize the facial landmark detection model.
  *  @param model_path Path of the model file, either absolute or relative to
  *                   the working directory.
  */
void CCFAN::InitModel(const char *model_path)
{
  /*Open the model file*/
  FILE *fp = fopen(model_path, "rb+");
  mean_shape_ = new float[pts_num_ * 2];
  fread(mean_shape_, sizeof(float), pts_num_ * 2, fp);

  /*Load the parameters of the first local stacked autoencoder network*/
  fread(&lan1_size_, sizeof(int), 1, fp);
  lan1_structure_ = new int[lan1_size_];
  fread(lan1_structure_, sizeof(int), lan1_size_, fp);

  lan1_w_ = new float *[lan1_size_ - 1];
  lan1_b_ = new float *[lan1_size_ - 1];
  int i;
  for (i = 0; i < lan1_size_ - 1; i++)
  {
    int layer_size = lan1_structure_[i] * lan1_structure_[i + 1];
    lan1_w_[i] = new float[layer_size];
    fread(lan1_w_[i], sizeof(float), layer_size, fp);

    lan1_b_[i] = new float[lan1_structure_[i + 1]];
    fread(lan1_b_[i], sizeof(float), lan1_structure_[i + 1], fp);
  }

  /*Load the parameters of the second local stacked autoencoder network*/
  fread(&lan2_size_, sizeof(int), 1, fp);
  lan2_structure_ = new int[lan2_size_];
  fread(lan2_structure_, sizeof(int), lan2_size_, fp);

  lan2_w_ = new float *[lan2_size_ - 1];
  lan2_b_ = new float *[lan2_size_ - 1];
  for (i = 0; i < lan2_size_ - 1; i++)
  {
    int layer_size = lan2_structure_[i] * lan2_structure_[i + 1];
    lan2_w_[i] = new float[layer_size];
    fread(lan2_w_[i], sizeof(float), layer_size, fp);

    lan2_b_[i] = new float[lan2_structure_[i + 1]];
    fread(lan2_b_[i], sizeof(float), lan2_structure_[i + 1], fp);
  }
  fclose(fp);
}

/** Detect five facial landmarks, i.e., two eye centers, nose tip and two mouth corners.
  *  @param gray_im A grayscale image
  *  @param im_width The width of the inpute image
  *  @param im_height The height of the inpute image
  *  @param face_loc The face bounding box
  *  @param[out] facial_loc The locations of detected facial points
  */
void CCFAN::FacialPointLocate(const unsigned char *gray_im, int im_width, int im_height, FaceInfo face_loc, float *facial_loc)
{
  int sift_patch_size = 32;
  int left_x = face_loc.bbox.x;
  int left_y = face_loc.bbox.y;
  int bbox_w = face_loc.bbox.width;
  int bbox_h = face_loc.bbox.height;
  int right_x = left_x + bbox_w - 1;
  int right_y = left_y + bbox_h - 1;

  float extend_factor = 0.05;
  float extend_revised_y = 0.05;

  /*Compute the extended region of the detected face*/
  int extend_lx = MAX(int(floor(left_x - extend_factor*bbox_w)), int(0));
  int extend_rx = MIN(int(floor(right_x + extend_factor*bbox_w)), int(im_width - 1));
  int extend_ly = MAX(int(floor(left_y - (extend_factor - extend_revised_y)*bbox_h)), int(0));
  int extend_ry = MIN(int(floor(right_y + (extend_factor + extend_revised_y)*bbox_h)), int(im_height - 1));

  int face_w = extend_rx - extend_lx + 1;
  int face_h = extend_ry - extend_ly + 1;

  int i, j, k, h;
  /*Get the face image based on the extended face region*/
  unsigned char *face_patch = new unsigned char[face_w*face_h];
  for (h = 0; h < face_h; h++)
  {
    const unsigned char *p_origin = gray_im + (h + extend_ly)*im_width + extend_lx;
    unsigned char *p_dest = face_patch + h*face_w;
    memcpy(p_dest, p_origin, face_w);
  }

  /*The first local stacked autoencoder network*/
  double *fea = new double[fea_dim_];
  int lan1_resize_w = 80;
  int lan1_resize_h = 80;
  BYTE *lan1_patch = new BYTE[lan1_resize_w*lan1_resize_h];
  ResizeImage(face_patch, face_w, face_h, lan1_patch, lan1_resize_w, lan1_resize_h);

  for (i = 0; i < pts_num_; i++)
  {
    facial_loc[i * 2] = mean_shape_[i * 2] - 1;
    facial_loc[i * 2 + 1] = mean_shape_[i * 2 + 1] - 1;
  }
#define isnan _isnan
  /*Extract the shape indexed SIFT features*/
  TtSift(lan1_patch, lan1_resize_w, lan1_resize_h, facial_loc, 32, fea);

  float *re_fea = new float[fea_dim_];
  for (i = 0; i < 128; i++)
  {
    for (j = 0; j < pts_num_; j++)
    {
      if (isnan(fea[j * 128 + i]))
      {
        re_fea[i*pts_num_ + j] = 0;
      }
      else
      {
        re_fea[i*pts_num_ + j] = fea[j * 128 + i];
      }
    }
  }

  float ** lan1_a = new float *[lan1_size_];

  for (i = 0; i < lan1_size_; i++)
  {
    lan1_a[i] = new float[lan1_structure_[i]];
  }

  for (i = 0; i < fea_dim_; i++)
  {
    lan1_a[0][i] = re_fea[i];
  }

  for (i = 0; i < lan1_size_ - 1; i++)
  {
    for (j = 0; j < lan1_structure_[i + 1]; j++)
    {
      float inner_product = 0;
      int fea_dim = lan1_structure_[i];
      for (k = 0; k < fea_dim; k++)
      {
        inner_product = inner_product + lan1_a[i][k] * lan1_w_[i][j*fea_dim + k];
      }
      if (i == lan1_size_ - 2)
      {
        lan1_a[i + 1][j] = inner_product + lan1_b_[i][j];
      }
      else
      {
        lan1_a[i + 1][j] = 1.0 / (1 + exp(-inner_product - lan1_b_[i][j]));
      }

    }
  }
  for (i = 0; i < pts_num_ * 2; i++)
  {
    facial_loc[i] = facial_loc[i] + lan1_a[lan1_size_ - 1][i];
  }
  for (i = 0; i < lan1_size_; i++)
  {
    delete[](lan1_a[i]);
  }
  delete[]lan1_a;
  delete[]lan1_patch;

  /*The second local stacked autoencoder network*/
  int lan2_resize_w = 140;
  int lan2_resize_h = 140;
  BYTE *lan2_patch = new BYTE[lan2_resize_w*lan2_resize_h];
  ResizeImage(face_patch, face_w, face_h, lan2_patch, lan2_resize_w, lan2_resize_h);

  float x_scale = float(lan1_resize_w) / lan2_resize_w;
  float y_scale = float(lan1_resize_h) / lan2_resize_h;

  for (i = 0; i < pts_num_; i++)
  {
    facial_loc[i * 2] = (facial_loc[i * 2]) / x_scale;
    facial_loc[i * 2 + 1] = (facial_loc[i * 2 + 1]) / y_scale;
  }
  /*Extract the shape indexed SIFT features*/
  TtSift(lan2_patch, lan2_resize_w, lan2_resize_h, facial_loc, 32, fea);

  for (i = 0; i < 128; i++)
  {
    for (j = 0; j < pts_num_; j++)
    {
      if (isnan(fea[j * 128 + i]))
      {
        re_fea[i*pts_num_ + j] = 0;
      }
      else
      {
        re_fea[i*pts_num_ + j] = fea[j * 128 + i];
      }
    }
  }

  float ** lan2_a = new float *[lan2_size_];

  for (i = 0; i < lan2_size_; i++)
  {
    lan2_a[i] = new float[lan2_structure_[i]];
  }

  for (i = 0; i < fea_dim_; i++)
  {
    lan2_a[0][i] = re_fea[i];
  }

  for (i = 0; i < lan2_size_ - 1; i++)
  {
    for (j = 0; j < lan2_structure_[i + 1]; j++)
    {
      float inner_product = 0;
      int fea_dim = lan2_structure_[i];
      for (int k = 0; k < fea_dim; k++)
      {
        inner_product = inner_product + lan2_a[i][k] * lan2_w_[i][j*fea_dim + k];
      }
      if (i == lan2_size_ - 2)
      {
        lan2_a[i + 1][j] = inner_product + lan2_b_[i][j];
      }
      else
      {
        lan2_a[i + 1][j] = 1.0 / (1 + exp(-inner_product - lan2_b_[i][j]));
      }

    }
  }
  for (i = 0; i < pts_num_ * 2; i++)
  {
    facial_loc[i] = facial_loc[i] + lan2_a[lan2_size_ - 1][i];
  }
  for (i = 0; i < lan2_size_; i++)
  {
    delete[](lan2_a[i]);
  }
  delete[]lan2_a;
  delete[]lan2_patch;

  delete[]fea;
  delete[]re_fea;
  delete[]face_patch;

  x_scale = float(lan2_resize_w) / face_w;
  y_scale = float(lan2_resize_h) / face_h;

  for (i = 0; i < pts_num_; i++)
  {
    facial_loc[i * 2] = (facial_loc[i * 2]) / x_scale + extend_lx;
    facial_loc[i * 2 + 1] = (facial_loc[i * 2 + 1]) / y_scale + extend_ly;
  }
}

/** Extract shape indexed SIFT features.
  *  @param gray_im A grayscale image
  *  @param im_width The width of the inpute image
  *  @param im_height The height of the inpute image
  *  @param face_shape The locations of facial points
  *  @param patch_size The size of the patch used for extracting SIFT feature
  *  @param[out] sift_fea the extracted shape indexed SIFT features which are concatenated into a vector
  */
void CCFAN::TtSift(const unsigned char *gray_im, int im_width, int im_height, float *face_shape, int patch_size, double *sift_fea)
{
  unsigned char *sub_img = new unsigned char[patch_size*patch_size];
  SIFT* sift_extractor = new SIFT();
  sift_extractor->InitSIFT(patch_size, patch_size, 32, 16);
  double *one_sift_fea = new double[128];
  double *fea_header = sift_fea;

  for (int i = 0; i < pts_num_; i++)
  {
    /*Get one image patch*/
    GetSubImg(gray_im, im_width, im_height, face_shape[i * 2], face_shape[i * 2 + 1], patch_size, sub_img);
    /*Extract  one SIFT feature of one image patch*/
    sift_extractor->CalcSIFT(sub_img, one_sift_fea);
    memcpy(fea_header + i * 128, one_sift_fea, 128 * 8);
  }
  delete[]one_sift_fea;
  delete[]sub_img;
  delete sift_extractor;
}

/** Extract a image patch which is centered at point(point_x, point_y) with a given patch size.
  *  @param gray_im A grayscale image
  *  @param im_width The width of the inpute image
  *  @param im_height The height of the inpute image
  *  @param point_x The X coordinate of one point
  *  @param point_y The Y coordinate of one point
  *  @param patch_size The size of the extracted patch
  *  @param[out] sub_img A grayscale image patch
  */
void CCFAN::GetSubImg(const unsigned char *gray_im, int im_width, int im_height, float point_x, float point_y, int patch_size, BYTE *sub_img)
{
  memset(sub_img, 128, patch_size*patch_size);
  int center_x = floor(point_x + 0.5);
  int center_y = floor(point_y + 0.5);
  int patch_left = MAX((center_x + 1) - patch_size / 2, 0);
  int patch_right = MIN((center_x + 1) + patch_size / 2 - 1, im_width - 1);
  int patch_top = MAX((center_y + 1) - patch_size / 2, 0);
  int patch_bottom = MIN((center_y + 1) + patch_size / 2 - 1, im_height - 1);

  int lx = abs(patch_left - ((center_x + 1) - patch_size / 2));
  int rx = patch_size - abs(patch_right - ((center_x + 1) + patch_size / 2 - 1)) - 1;
  int ty = abs(patch_top - ((center_y + 1) - patch_size / 2));
  int by = patch_size - abs(patch_bottom - ((center_y + 1) + patch_size / 2 - 1)) - 1;
  int h, w, ph, pw;
  for (h = ty, ph = patch_top; h < by + 1; h++, ph++) {
    for (w = lx, pw = patch_left; w < rx + 1; w++, pw++) {
      sub_img[h*patch_size + w] = gray_im[ph*im_width + pw];
    }
  }
}

/** Resize the image by bilinear interpolation.
  *  @param src_im A source image in grayscale
  *  @param src_width The width of the source image
  *  @param src_height The height of the source image
  *  @param[out] dst_im The target image in grayscale
  *  @param dst_width The width of the target image
  *  @param dst_height The height of the target image
  */
bool CCFAN::ResizeImage(const unsigned char *src_im, int src_width, int src_height,
  unsigned char* dst_im, int dst_width, int dst_height)
{

  double	lfx_scl, lfy_scl;
  if (src_width == dst_width && src_height == dst_height) {
    memcpy(dst_im, src_im, src_width * src_height * sizeof(unsigned char));
    return true;
  }

  lfx_scl = double(src_width + 0.0) / dst_width;
  lfy_scl = double(src_height + 0.0) / dst_height;

  for (int n_y_d = 0; n_y_d < dst_height; n_y_d++) {
    for (int n_x_d = 0; n_x_d < dst_width; n_x_d++) {
      double lf_x_s = lfx_scl * n_x_d;
      double lf_y_s = lfy_scl * n_y_d;

      int n_x_s = int(lf_x_s);
      n_x_s = (n_x_s <= (src_width - 2) ? n_x_s : (src_width - 2));
      int n_y_s = int(lf_y_s);
      n_y_s = (n_y_s <= (src_height - 2) ? n_y_s : (src_height - 2));

      double lf_weight_x = lf_x_s - n_x_s;
      double lf_weight_y = lf_y_s - n_y_s;

      double lf_new_gray = (1 - lf_weight_y) * ((1 - lf_weight_x) * src_im[n_y_s * src_width + n_x_s] +
        lf_weight_x * src_im[n_y_s * src_width + n_x_s + 1]) +
        lf_weight_y * ((1 - lf_weight_x) * src_im[(n_y_s + 1) * src_width + n_x_s] +
        lf_weight_x * src_im[(n_y_s + 1) * src_width + n_x_s + 1]);

      dst_im[n_y_d * dst_width + n_x_d] = (unsigned char)(lf_new_gray);
    }
  }
  return true;
}


class FaceAlignment{
 public:
  /** A constructor with an optional argument specifying path of the model file.
  *  If called with no argument, the model file is assumed to be stored in the
  *  the working directory as "seeta_fa_v1.1.bin".
  *
  *  @param model_path Path of the model file, either absolute or relative to
  *  the working directory.
  */
   FaceAlignment(const char* model_path = NULL);

  /** A Destructor which should never be called explicitly.
  *  Release all dynamically allocated resources.
  */
   ~FaceAlignment();

  /** Detect five facial landmarks, i.e., two eye centers, nose tip and two mouth corners.
  *  @param gray_im A grayscale image
  *  @param face_info The face bounding box
  *  @param[out] points The locations of detected facial points
  */
   bool PointDetectLandmarks(ImageData gray_im, FaceInfo face_info, FacialLandmark *points);

 private:
  CCFAN *facial_detector;
};

/** A constructor with an optional argument specifying path of the model file.
*  If called with no argument, the model file is assumed to be stored in the
*  the working directory as "seeta_fa_v1.1.bin".
*
*  @param model_path Path of the model file, either absolute or relative to
*  the working directory.
*/
FaceAlignment::FaceAlignment(const char * model_path){
  facial_detector = new CCFAN();
  if (model_path == NULL)
    model_path = "seeta_fa_v1.1.bin";
  facial_detector->InitModel(model_path);
}

/** Detect five facial landmarks, i.e., two eye centers, nose tip and two mouth corners.
*  @param gray_im A grayscale image
*  @param face_info The face bounding box
*  @param[out] points The locations of detected facial points
*/
bool FaceAlignment::PointDetectLandmarks(ImageData gray_im, FaceInfo face_info, FacialLandmark *points)
{
  if (gray_im.num_channels != 1) {
    return false;
  }
  int pts_num = 5;
  float *facial_loc = new float[pts_num * 2];
  facial_detector->FacialPointLocate(gray_im.data, gray_im.width, gray_im.height, face_info, facial_loc);
  
  for (int i = 0; i < pts_num; i++) {
    points[i].x = facial_loc[i * 2];
    points[i].y = facial_loc[i * 2 + 1];
  }
  
  delete[]facial_loc;
  return true;
}

/** A Destructor which should never be called explicitly.
*  Release all dynamically allocated resources.
*/
FaceAlignment::~FaceAlignment() {
  if (facial_detector != NULL) {
    delete facial_detector;
    facial_detector = NULL;
  }
}


#endif // _FACEALIGNMENT_INL_
