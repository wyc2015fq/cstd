



//===========================================================================
// Computing the image gradient
void Grad(const Mat& im, Mat& grad)
{

  /*float filter[3] = {1, 0, -1};
  float dfilter[1] = {1};
  Mat filterX = Mat(1,3,CV_32F, filter).clone();
  Mat filterY = Mat(1,1,CV_32F, dfilter).clone();

  Mat gradX;
  Mat gradY;
  sepFilter2D(im, gradX, CV_32F, filterY, filterX, Point(-1,-1), 0);
  sepFilter2D(im, gradY, CV_32F, filterX.t(), filterY, Point(-1,-1), 0);
  pow(gradX,2, gradX);
  pow(gradY,2, gradY);
  grad = gradX + gradY;

  grad.row(0).setTo(0);
  grad.col(0).setTo(0);
  grad.col(grad.cols-1).setTo(0);
  grad.row(grad.rows-1).setTo(0);   */

  // A quicker alternative
  int x, y, h = im.rows, w = im.cols;
  float vx, vy;

  // Initialise the gradient
  grad.create(im.size(), CV_32F);
  grad.setTo(0.0f);

  float* gp  = ((float*)grad.data) + w + 1;
  const float* px1 = ((float*)im.data)   + w + 2;
  const float* px2 = ((float*)im.data)   + w;
  const float* py1 = ((float*)im.data)   + 2 * w + 1;
  const float* py2 = ((float*)im.data)   + 1;

  for (y = 1; y < h - 1; y++) {
    for (x = 1; x < w - 1; x++) {
      vx = *px1++ - *px2++;
      vy = *py1++ - *py2++;
      *gp++ = vx * vx + vy * vy;
    }

    px1 += 2;
    px2 += 2;
    py1 += 2;
    py2 += 2;
    gp += 2;
  }

}

//===========================================================================
void SVR_patch_expert::Read(FILE* stream)
{

  // A sanity check when reading patch experts
  int read_type=0;
  fscanf(stream, "%d", &read_type);
  assert(read_type == 2);

  fscanf(stream, "%d %lf %lf %lf", &type, &confidence, &scaling, &bias);
  ReadMat(stream, weights);

  // OpenCV and Matlab matrix cardinality is different, hence the transpose
  weights = weights.t();

}

//===========================================================================
void SVR_patch_expert::Response(const Mat& area_of_interest, Mat& response)
{

  int response_height = area_of_interest.rows - weights.rows + 1;
  int response_width = area_of_interest.cols - weights.cols + 1;

  // the patch area on which we will calculate reponses
  Mat normalised_area_of_interest;

  if (response.rows != response_height || response.cols != response_width) {
    response.create(response_height, response_width, CV_32F);
  }

  // If type is raw just normalise mean and standard deviation
  if (type == 0) {
    // Perform normalisation across whole patch
    Scalar mean;
    Scalar std;

    meanStdDev(area_of_interest, mean, std);

    // Avoid division by zero
    if (std[0] == 0) {
      std[0] = 1;
    }

    normalised_area_of_interest = (area_of_interest - mean[0]) / std[0];
  }
  // If type is gradient, perform the image gradient computation
  else if (type == 1) {
    Grad(area_of_interest, normalised_area_of_interest);
  }
  else {
    printf("ERROR(%s,%d): Unsupported patch type %d!\n", __FILE__, __LINE__, type);
    abort();
  }

  Mat svr_response;

  // The empty matrix as we don't pass precomputed dft's of image
  Mat empty_matrix_0 = Mat::zeros(0, 0, CV_64F);
  Mat empty_matrix_1 = Mat::zeros(0, 0, CV_32F);
  Mat empty_matrix_2 = Mat::zeros(0, 0, CV_32F);

  // Efficient calc of patch expert SVR response across the area of interest
  matchTemplate_m(normalised_area_of_interest, empty_matrix_0, empty_matrix_1, empty_matrix_2, weights, weights_dfts, svr_response, CV_TM_CCOEFF_NORMED);

  response.create(svr_response.size(), CV_32F);
  float* p = ((float*)response.data);

  float* q1 = ((float*)svr_response.data); // respone for each pixel
  float* q2 = q1 + svr_response.rows * svr_response.cols;

  while (q1 != q2) {
    // the SVR response passed into logistic regressor
    *p++ = 1.0 / (1.0 + exp(-(*q1++ * scaling + bias)));
  }

}

void SVR_patch_expert::ResponseDepth(const Mat& area_of_interest, Mat& response)
{

  // How big the response map will be
  int response_height = area_of_interest.rows - weights.rows + 1;
  int response_width = area_of_interest.cols - weights.cols + 1;

  // the patch area on which we will calculate reponses
  Mat normalised_area_of_interest;

  if (response.rows != response_height || response.cols != response_width) {
	  response.create(response_height, response_width, CV_32F);
  }

  if (type == 0) {
    // Perform normalisation across whole patch
    Scalar mean;
    Scalar std;

    // ignore missing values
    Mat mask = area_of_interest > 0;
    meanStdDev(area_of_interest, mean, std, mask);

    // if all values the same don't divide by 0
    if (std[0] == 0) {
      std[0] = 1;
    }

    normalised_area_of_interest = (area_of_interest - mean[0]) / std[0];

    // Set the invalid pixels to 0
    normalised_area_of_interest.setTo(0, mask == 0);
  }
  else {
    printf("ERROR(%s,%d): Unsupported patch type %d!\n", __FILE__, __LINE__, type);
    abort();
  }

  Mat svr_response;

  // The empty matrix as we don't pass precomputed dft's of image
  Mat empty_matrix_0 = Mat::zeros(0, 0, CV_64F);
  Mat empty_matrix_1 = Mat::zeros(0, 0, CV_32F);
  Mat empty_matrix_2 = Mat::zeros(0, 0, CV_32F);

  // Efficient calc of patch expert response across the area of interest

  matchTemplate_m(normalised_area_of_interest, empty_matrix_0, empty_matrix_1, empty_matrix_2, weights, weights_dfts, svr_response, CV_TM_CCOEFF);

  response.create(svr_response.size(), CV_32F);
  float* p = ((float*)response.data);

  float* q1 = ((float*)svr_response.data); // respone for each pixel
  float* q2 = q1 + svr_response.rows * svr_response.cols;

  while (q1 != q2) {
    // the SVR response passed through a logistic regressor
    *p++ = 1.0 / (1.0 + exp(-(*q1++ * scaling + bias)));
  }
}

//===========================================================================
void Multi_SVR_patch_expert::Read(FILE* stream)
{
  // A sanity check when reading patch experts
  int type;
  fscanf(stream, "%d", &type);
  assert(type == 3);

  // The number of patch experts for this view (with different modalities)
  int number_modalities;

  fscanf(stream, "%d %d %d", &width , &height , &number_modalities);

  svr_patch_experts.resize(number_modalities);

  for (int i = 0; i < number_modalities; i++) {
    svr_patch_experts[i].Read(stream);
  }

}
//===========================================================================
void Multi_SVR_patch_expert::Response(const Mat& area_of_interest, Mat& response)
{

  int response_height = area_of_interest.rows - height + 1;
  int response_width = area_of_interest.cols - width + 1;

  if (response.rows != response_height || response.cols != response_width) {
    response.create(response_height, response_width, CV_32F);
  }

  // For the purposes of the experiment only use the response of normal intensity, for fair comparison

  if (svr_patch_experts.size() == 1) {
    svr_patch_experts[0].Response(area_of_interest, response);
  }
  else {
    // responses from multiple patch experts these can be gradients, LBPs etc.
    response.setTo(1.0);

    Mat modality_resp(response_height, response_width, CV_32F);

    for (size_t i = 0; i < svr_patch_experts.size(); i++) {
      svr_patch_experts[i].Response(area_of_interest, modality_resp);
      response = response.mul(modality_resp);
    }

  }

}

void Multi_SVR_patch_expert::ResponseDepth(const Mat& area_of_interest, Mat& response)
{
  int response_height = area_of_interest.rows - height + 1;
  int response_width = area_of_interest.cols - width + 1;

  if (response.rows != response_height || response.cols != response_width) {
    response.create(response_height, response_width, CV_32F);
  }

  // With depth patch experts only do raw data modality
  svr_patch_experts[0].ResponseDepth(area_of_interest, response);
}
//===========================================================================
