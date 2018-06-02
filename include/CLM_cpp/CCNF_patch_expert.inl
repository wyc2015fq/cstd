


// Compute sigmas for all landmarks for a particular view and window size
// #ifdef _USE_OPENCV
#if 0
void CCNF_patch_expert::ComputeSigmas(const std::vector<Mat >& sigma_components, int window_size)
{
	for(size_t i=0; i < window_sizes.size(); ++i)
	{
		if( window_sizes[i] == window_size)
			return;
	}
	// Each of the landmarks will have the same connections, hence constant number of sigma components
	int n_betas = sigma_components.size();

	// calculate the sigmas based on alphas and betas
	float sum_alphas = 0;

	int n_alphas = this->neurons.size();

	// sum the alphas first
	for(int a = 0; a < n_alphas; ++a)
	{
		sum_alphas = sum_alphas + this->neurons[a].alpha;
	}

	Mat  q1 = sum_alphas * Mat::eye(window_size*window_size, window_size*window_size, CC_32F);

	Mat q2 = Mat::zeros(window_size*window_size, window_size*window_size, CC_32F);
	for (int b=0; b < n_betas; ++b)
	{			
		q2 = q2 + ((float)this->betas[b]) * sigma_components[b];
	}

	Mat SigmaInv = 2 * (q1 + q2);
	
	Mat Sigma_f;
	invert(SigmaInv, Sigma_f, DECOMP_CHOLESKY);

	window_sizes.push_back(window_size);
	Sigmas.push_back(Sigma_f);

}
#else
void CCNF_patch_expert::ComputeSigmas(const std::vector<Mat >& sigma_components, int window_size)
{
	for (size_t i = 0; i < window_sizes.size(); ++i) {
		if (window_sizes[i] == window_size) {
			return;
		}
	}

	IM_BEGIN(im1, 10);
	// Each of the landmarks will have the same connections, hence constant number of sigma components
	int n_betas = sigma_components.size();

	// calculate the sigmas based on alphas and betas
	float sum_alphas = 0;

	int n_alphas = this->neurons.size();

	// sum the alphas first
	for (int a = 0; a < n_alphas; ++a) {
		sum_alphas = sum_alphas + this->neurons[a].alpha;
	}

	int n = window_size * window_size;
	img_t* q1 = cvSetIdentityToReal(im1++, n, n, CC_32F, sum_alphas);
	img_t* q2 = cvSetZero(im1++, n, n, CC_32F, 1);
	for (int b = 0; b < n_betas; ++b) {
		//q2 = q2 + ((float)this->betas[b]) * sigma_components[b];
		const img_t* sigma_components_b = &(sigma_components[b]);
		axpy(sigma_components_b, this->betas[b], q2, q2);
	}

	//SigmaInv = 2 * (q1 + q2);
	img_t* SigmaInv = cvAdd(q1, q2, im1++, NULL);
	cvMulS(SigmaInv, 2, SigmaInv);

	Mat Sigma_f(n, n, CV_32F);

	//CHECK_OR_PUT(SigmaInv);
	//printf("%d ", SigmaInv.rows);
	tic;
	invert(SigmaInv, &Sigma_f, CC_CHOLESKY);
	toc;

	window_sizes.push_back(window_size);
	Sigmas.push_back(Sigma_f);

	IM_END(im1);
}
#endif


void CCNF_neuron_Read(CCNF_neuron* s, FILE* stream)
{
	// Sanity check
	int read_type;
	fread((char*)&read_type, 4, 1, stream);
	assert(read_type == 2);

	fread((char*)&s->neuron_type, 4, 1, stream);
	fread((char*)&s->norm_weights, 8, 1, stream);
	fread((char*)&s->bias, 8, 1, stream);
	fread((char*)&s->alpha, 8, 1, stream);

	ReadMatBin(stream, s->weights, CV_64F);
}
//===========================================================================
void CCNF_neuron_Response(CCNF_neuron* s, const Mat& img, Mat& im_dft, Mat& integral_img, Mat& integral_img_sq, Mat& resp)
{
	int h = img.rows - s->weights.rows + 1;
	int w = img.cols - s->weights.cols + 1;

	// the patch area on which we will calculate reponses
	Mat I;

	if (s->neuron_type == 3) {
		// Perform normalisation across whole patch (ignoring the invalid values indicated by <= 0

		Scalar mean;
		Scalar std;

		// ignore missing values
		Mat mask = img > 0;
		meanStdDev(img, mean, std, mask);

		// if all values the same don't divide by 0
		if (std[0] != 0) {
			I = (img - mean[0]) / std[0];
		}
		else {
			I = (img - mean[0]);
		}

		I.setTo(0, mask == 0);
	}
	else {
		if (s->neuron_type == 0) {
			I = img;
		}
		else {
			printf("ERROR(%s,%d): Unsupported patch type %d!\n", __FILE__, __LINE__, s->neuron_type);
			abort();
		}
	}

	if (resp.empty()) {
		resp.create(h, w, CV_32F);
	}

	// The response from neuron before activation
	if (s->neuron_type == 3) {
		// In case of depth we use per area, rather than per patch normalisation
		matchTemplate_m(I, im_dft, integral_img, integral_img_sq, s->weights, s->weights_dfts, resp, CV_TM_CCOEFF); // the linear multiplication, efficient calc of response
	}
	else {
		matchTemplate_m(I, im_dft, integral_img, integral_img_sq, s->weights, s->weights_dfts, resp, CV_TM_CCOEFF_NORMED); // the linear multiplication, efficient calc of response
	}

	//CHECK_OR_PUT(im_dft);
	//CHECK_OR_PUT(integral_img);
	//CHECK_OR_PUT(resp);
	//assert(resp.is_cout());
	// TODO a single iterator?
	float* p = (float*)resp.data;
	const float* q1 = (float*)resp.data; // respone for each pixel
	const float* q2 = q1 + resp.rows * resp.cols;

	// the logistic function (sigmoid) applied to the response
	while (q1 != q2) {
		*p++ = (2 * s->alpha) * 1.0 / (1.0 + exp(-(*q1++ * s->norm_weights + s->bias)));
	}

}
//===========================================================================
void CCNF_patch_expert::Read(FILE* stream, const std::vector<int>& window_sizes, const std::vector<std::vector<Mat > >& sigma_components)
{
  // Sanity check
  int read_type;

  fread((char*)&read_type, 4, 1, stream);
  assert(read_type == 5);

  // the number of neurons for this patch
  int num_neurons;
  fread((char*)&width, 4, 1, stream);
  fread((char*)&height, 4, 1, stream);
  fread((char*)&num_neurons, 4, 1, stream);

  if (num_neurons == 0) {
    // empty patch due to landmark being invisible at that orientation

    // read an empty int (due to the way things were written out)
    fread((char*)&num_neurons, 4, 1, stream);
    return;
  }

  neurons.resize(num_neurons);

  for (int i = 0; i < num_neurons; i++) {
    CCNF_neuron_Read(&neurons[i], stream);
  }

  int n_sigmas = window_sizes.size();

  int n_betas = 0;

  if (n_sigmas > 0) {
    n_betas = sigma_components[0].size();

    betas.resize(n_betas);

    for (int i = 0; i < n_betas;  ++i) {
      fread((char*)&betas[i], 8, 1, stream);
    }
  }

  // Read the patch confidence
  fread((char*)&patch_confidence, 8, 1, stream);

}

//===========================================================================
void CCNF_patch_expert::Response(const Mat& area_of_interest, Mat& response)
{
  int response_height = area_of_interest.rows - height + 1;
  int response_width = area_of_interest.cols - width + 1;

  if (response.rows != response_height || response.cols != response_width) {
    response.create(response_height, response_width, CV_32F);
  }

  response.setTo(0);

  // the placeholder for the DFT of the image, the integral image, and squared integral image so they don't get recalculated for every response
  Mat area_of_interest_dft;
  Mat integral_image, integral_image_sq;

  Mat neuron_response;
  size_t i;
  // responses from the neural layers
  for (i = 0; i < neurons.size(); i++) {
    // Do not bother with neuron response if the alpha is tiny and will not contribute much to overall result
    if (neurons[i].alpha > 1e-4) {
      CCNF_neuron_Response(&neurons[i], area_of_interest, area_of_interest_dft, integral_image, integral_image_sq, neuron_response);
	  //CHECK_OR_PUT(neuron_response);
      response = response + neuron_response;
    }
  }

  //CHECK_OR_PUT(response);

  int s_to_use = -1;
  // Find the matching sigma
  for (i = 0; i < window_sizes.size(); ++i) {
    if (window_sizes[i] == response_height) {
      // Found the correct sigma
      s_to_use = i;
      break;
    }
  }

  Mat resp_vec_f = response.reshape(1, response_height * response_width);

  Mat out = Sigmas[s_to_use] * resp_vec_f;

  response = out.reshape(1, response_height);

  // Making sure the response does not have negative numbers
  double min;

  minMaxIdx(response, &min, 0);

  if (min < 0) {
    response = response - min;
  }

}
