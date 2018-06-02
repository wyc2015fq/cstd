
struct ComputeSigmas_thd_t {
	CCNF_patch_expert* ccnf_expert_intensity;
	int id[100];
	int n;
	const vector<Mat >* sigma_components;
	int window_size;
};

int ComputeSigmas_thd(void* s0) {
	int i;
	ComputeSigmas_thd_t* s = (ComputeSigmas_thd_t*)s0;
	for (i = 0; i < s->n; ++i) {
		int id = s->id[i];
		s->ccnf_expert_intensity[id].ComputeSigmas(*s->sigma_components, s->window_size);
	}
	return 0;
}

int ComputeSigmas_work(void* arg, int beg, int end) {
	void** args = (void**)arg;
	CCNF_patch_expert* ccnf_expert_intensity = (CCNF_patch_expert*)args[0];
	const vector<Mat >* sigma_components = (const vector<Mat >*)args[1];
	int* window_size = (int*)args[2];
	Mat* visibilities = (Mat*)args[3];
	std::vector<int>* lmarks = (std::vector<int>*)args[4];
	for (int i = beg; i < end; i++) {
		int lmark = (*lmarks)[i];
		// Only for visible landmarks
		// Precompute sigmas if they are not computed yet
		ccnf_expert_intensity[lmark].ComputeSigmas(*sigma_components, *window_size);
	}
	return 0;
}

//#define PUSHARG(args, j, T, NAME)     args[j++] =& NAME;
//#define POPARG(args, j, T, NAME)      T& NAME = *(T*)args[j++];

#define PATCH_EXPERTS_RESPONSE_ARGS(args1, j, OPT_DEF) \
	OPT_DEF(args1, j, int, view_id) \
	OPT_DEF(args1, j, int, scale) \
	OPT_DEF(args1, j, int, window_size) \
	OPT_DEF(args1, j, int, n) \
	OPT_DEF(args1, j, Mat, landmark_locations) \
	OPT_DEF(args1, j, double, a1) \
	OPT_DEF(args1, j, double, b1) \
	OPT_DEF(args1, j, bool, use_ccnf) \
	OPT_DEF(args1, j, std::vector<int>, i_vec) \
	OPT_DEF(args1, j, vector<vector<vector<Multi_SVR_patch_expert> > >,  svr_expert_depth) \
	OPT_DEF(args1, j, const img_t*, depth_image) \
	OPT_DEF(args1, j, vector<vector<Mat > >, visibilities) \
	OPT_DEF(args1, j, vector<Mat >, patch_expert_responses) \
	OPT_DEF(args1, j, vector<vector<vector<CCNF_patch_expert> > >,     ccnf_expert_intensity) \
	OPT_DEF(args1, j, vector<vector<vector<Multi_SVR_patch_expert> > >,  svr_expert_intensity) \
	OPT_DEF(args1, j, const img_t*, grayscale_image) \

#if 0
int calculate_the_patch_responses_work(void* arg, int beg, int end) {
	int i = 0, j=0;
	void** args = (void**)arg;
	PATCH_EXPERTS_RESPONSE_ARGS(args, j, POPARG);
	for (j = beg; j < end; ++j) {
		IM_BEGIN(im, 10);
		int i = i_vec[j];

		// Work out how big the area of interest has to be to get a response of window size
		int area_of_interest_width;
		int area_of_interest_height;

		if (use_ccnf) {
			area_of_interest_width = window_size + ccnf_expert_intensity[scale][view_id][i].width - 1;
			area_of_interest_height = window_size + ccnf_expert_intensity[scale][view_id][i].height - 1;
		}
		else {
			area_of_interest_width = window_size + svr_expert_intensity[scale][view_id][i].width - 1;
			area_of_interest_height = window_size + svr_expert_intensity[scale][view_id][i].height - 1;
		}

		// scale and rotate to mean shape to reference frame
		double6 sim = { a1, -b1, landmark_locations.at<double>(i, 0), b1, a1, landmark_locations.at<double>(i + n, 0) };

		// Extract the region of interest around the current landmark location
		img_t *area_of_interest = cvSetMat(im++, area_of_interest_height, area_of_interest_width, CC_32F);
		//Mat area_of_interest(area_of_interest_height, area_of_interest_width, CV_32F);

		// Using C style openCV as it does what we need
		//CvMat area_of_interest_o = area_of_interest;
		//CvMat sim_o = sim;
		//Mat tt = ma(grayscale_image);
		//IplImage im_o = tt;
		//tic;
		cvGetQuadrangleSubPix(grayscale_image, area_of_interest, sim);

		// get the correct size response window
		patch_expert_responses[i] = Mat(window_size, window_size, CV_32F);

		// Get intensity response either from the SVR or CCNF patch experts (prefer CCNF)
		if (!ccnf_expert_intensity.empty()) {
			ccnf_expert_intensity[scale][view_id][i].Response(ma(area_of_interest), patch_expert_responses[i]);
		}
		else {
			svr_expert_intensity[scale][view_id][i].Response(ma(area_of_interest), patch_expert_responses[i]);
		}
		//toc;

		// if we have a corresponding depth patch and it is visible
		if (!svr_expert_depth.empty() && !imempty(depth_image) && visibilities[scale][view_id].at<int>(i, 0)) {

			Mat dProb = patch_expert_responses[i].clone();
			img_t* depthWindow = cvSetMat(im++, area_of_interest_height, area_of_interest_width, CC_32F);


			img_t* dimg_o = depthWindow;
			img_t* maskWindow = cvSetMat(im++, area_of_interest_height, area_of_interest_width, CC_32F);
			img_t* mimg_o = maskWindow;

			//img_t* d_o = ma(depth_image);
			//img_t* m_o = mask;

			cvGetQuadrangleSubPix(depth_image, dimg_o, sim);
			cvGetQuadrangleSubPix(depth_image, mimg_o, sim);

			img_t* mask1 = im++;
			//cmaskWindow < 1;
			cvCmpS(maskWindow, 1, mask1, CC_CMP_LT);
			cvSetReal(depthWindow, 0, mask1);

			svr_expert_depth[scale][view_id][i].ResponseDepth(ma(depthWindow), dProb);

			// Sum to one
			double sum1 = sum(patch_expert_responses[i])[0];

			// To avoid division by 0 issues
			if (sum1 == 0) {
				sum1 = 1;
			}

			patch_expert_responses[i] /= sum1;

			// Sum to one
			sum1 = sum(dProb)[0];

			// To avoid division by 0 issues
			if (sum1 == 0) {
				sum1 = 1;
			}

			dProb /= sum1;

			patch_expert_responses[i] = patch_expert_responses[i] + dProb;

		}
		IM_END(im);
	}
	return 0;
}
#endif

// Returns the patch expert responses given a grayscale and an optional depth image.
// Additionally returns the transform from the image coordinates to the response coordinates (and vice versa).
// The computation also requires the current landmark locations to compute response around, the PDM corresponding to the desired model, and the parameters describing its instance
// Also need to provide the size of the area of interest and the desired scale of analysis
void Patch_experts::Response(vector<Mat >& patch_expert_responses, Matx22d& sim_ref_to_img, Matx22d& sim_img_to_ref, const img_t* grayscale_image, const img_t* depth_image,
    const PDM& pdm, const Vec6d& params_global, const Mat& params_local, int window_size, int scale)
{
	int j=0;
	//void* args1[20];
	int view_id;
	int n;
	double a1, b1;
	Mat landmark_locations;
	bool use_ccnf;
	std::vector<int> i_vec;
	//PATCH_EXPERTS_RESPONSE_ARGS(args1, j, PUSHARG);
	{
		view_id = GetViewIdx(params_global, scale);
		n = pdm.NumberOfPoints();
		// Compute the current landmark locations (around which responses will be computed)
		pdm.CalcShape2D(landmark_locations, params_local, params_global);
		Mat reference_shape;
		// Initialise the reference shape on which we'll be warping
		Vec6d global_ref(patch_scaling[scale], 0, 0, 0, 0, 0);
		// Compute the reference shape
		pdm.CalcShape2D(reference_shape, params_local, global_ref);

		// similarity and inverse similarity transform to and from image and reference shape
		Mat reference_shape_2D = (reference_shape.reshape(1, 2).t());
		Mat image_shape_2D = landmark_locations.reshape(1, 2).t();
		//CHECK_OR_PUT(image_shape_2D);
		//CHECK_OR_PUT(reference_shape_2D);
#ifdef _USE_OPENCV
		sim_img_to_ref = AlignShapesWithScale(&image_shape_2D, &reference_shape_2D);
#else
		void AlignShapesWithScale(const img_t* src, img_t* dst, double4 A);
		AlignShapesWithScale(&mi(image_shape_2D), &mi(reference_shape_2D), sim_img_to_ref.val);
#endif
#if 0
		Matx22d sim_ref_to_img_d = sim_img_to_ref.inv(CC_DECOMP_LU);
#else
		Matx22d sim_ref_to_img_d;
		inv2(sim_ref_to_img_d.val, sim_img_to_ref.val);
#endif
		Mat sim_img_to_ref_mat(sim_img_to_ref);
		//CHECK_OR_PUT(sim_img_to_ref_mat);
		a1 = sim_ref_to_img_d(0, 0);
		b1 = -sim_ref_to_img_d(0, 1);

		sim_ref_to_img(0, 0) = (float)sim_ref_to_img_d(0, 0);
		sim_ref_to_img(0, 1) = (float)sim_ref_to_img_d(0, 1);
		sim_ref_to_img(1, 0) = (float)sim_ref_to_img_d(1, 0);
		sim_ref_to_img(1, 1) = (float)sim_ref_to_img_d(1, 1);

		// Indicates the legal pixels in a depth image, if available (used for CLM-Z area of interest (window) interpolation)
		Mat mask;

		if (!imempty(depth_image)) {
			mask = ma(depth_image) > 0;
			mask = mask / 255;
		}


		use_ccnf = !this->ccnf_expert_intensity.empty();

		// If using CCNF patch experts might need to precalculate Sigmas
		if (use_ccnf) {
			vector<Mat > sigma_components;

			// Retrieve the correct sigma component size
			for (size_t w_size = 0; w_size < this->sigma_components.size(); ++w_size) {
				if (!this->sigma_components[w_size].empty()) {
					if (window_size * window_size == this->sigma_components[w_size][0].rows) {
						sigma_components = this->sigma_components[w_size];
					}
				}
			}
#if 1
			// Go through all of the landmarks and compute the Sigma for each
			for (int lmark = 0; lmark < n; lmark++) {
				// Only for visible landmarks
        int* ptr = img_ptr(int, &visibilities[scale][view_id]);
				if (ptr[lmark]) {
					// Precompute sigmas if they are not computed yet
					if (!ccnf_expert_intensity[scale][view_id][lmark].HasWindow(window_size)) {
						ccnf_expert_intensity[scale][view_id][lmark].ComputeSigmas(sigma_components, window_size);
					}
				}
			}
#endif
#if 0
			{
				//threadpool_init(pool, 4);
				std::vector<int> lmarks;
				void* args[] = { &ccnf_expert_intensity[scale][view_id][0], &sigma_components, &window_size, &visibilities[scale][view_id], &lmarks };
				for (int lmark = 0; lmark < n; lmark++) {
					// Only for visible landmarks
					if (visibilities[scale][view_id].at<int>(lmark, 0)) {
						// Precompute sigmas if they are not computed yet
						if (!ccnf_expert_intensity[scale][view_id][lmark].HasWindow(window_size)) {
							//ccnf_expert_intensity[scale][view_id][lmark].ComputeSigmas(sigma_components, window_size);
							lmarks.push_back(lmark);
						}
					}
				}
				if (lmarks.size() > 0) {
					parallel_for(args, 0, lmarks.size(), ComputeSigmas_work);
				}
			}
#endif
#if 0
			{
				enum { THREADS_COUNT = 4 };
				ComputeSigmas_thd_t s[THREADS_COUNT] = { 0 };
				thread_t threads[THREADS_COUNT] = { 0 };
				int count = 0;
				for (int lmark = 0; lmark < n; lmark++) {
					// Only for visible landmarks
					if (visibilities[scale][view_id].at<int>(lmark, 0)) {
						// Precompute sigmas if they are not computed yet
						//ccnf_expert_intensity[scale][view_id][lmark].ComputeSigmas(sigma_components, window_size);
						int i = count%THREADS_COUNT;
						s[i].id[s[i].n++] = lmark;
						threads[i].run = ComputeSigmas_thd;
						threads[i].arg = s + i;
						++count;
					}
				}
				for (int i = 0; i < THREADS_COUNT; ++i) {
					s[i].ccnf_expert_intensity = &ccnf_expert_intensity[scale][view_id][0];
					s[i].sigma_components = &sigma_components;
					s[i].window_size = window_size;
					sys_thread_create(threads + i);
				}
				for (int i = 0; i < THREADS_COUNT; ++i) {
					sys_thread_join(threads + i);
				}
			}

#endif
		}

	}

  // calculate the patch responses for every landmark, Actual work happens here. If openMP is turned on it is possible to do this in parallel,
  // this might work well on some machines, while potentially have an adverse effect on others
  tic;
#if 0
#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (int i = 0; i < (int)n; ++i) {
	  if (visibilities[scale][view_id].rows == n) {
		  if (visibilities[scale][view_id].at<int>(i, 0) != 0) {
			  i_vec.push_back(i);
		  }
	  }
  }
  if (i_vec.size() > 0) {
	  parallel_for(args1, 0, i_vec.size(), calculate_the_patch_responses_work);
  }
#else
  myparallel_for(0, (int)n, i) 
    //for(int i = 0; i < n; i++)
    {

      if (visibilities[scale][view_id].rows == n) {
        int* ptr = img_ptr(int, &visibilities[scale][view_id]);
        if (ptr[i] != 0) {
			IM_BEGIN(im, 10);

          // Work out how big the area of interest has to be to get a response of window size
          int area_of_interest_width;
          int area_of_interest_height;

          if (use_ccnf) {
            area_of_interest_width = window_size + ccnf_expert_intensity[scale][view_id][i].width - 1;
            area_of_interest_height = window_size + ccnf_expert_intensity[scale][view_id][i].height - 1;
          }
          else {
            area_of_interest_width = window_size + svr_expert_intensity[scale][view_id][i].width - 1;
            area_of_interest_height = window_size + svr_expert_intensity[scale][view_id][i].height - 1;
          }

		  //CHECK_OR_PUT(landmark_locations);
          // scale and rotate to mean shape to reference frame
          double* landmark_locations_ptr = img_ptr(double, &landmark_locations);
		  double6 sim = { a1, -b1, landmark_locations_ptr[i], b1, a1, landmark_locations_ptr[i + n] };
		  Mat sim_mat(2, 3, CC_64F, sim);
		  //CHECK_OR_PUT(sim_mat);
          // Extract the region of interest around the current landmark location
          img_t *area_of_interest = cvSetMat(im++, area_of_interest_height, area_of_interest_width, CC_32F);
		  //Mat area_of_interest(area_of_interest_height, area_of_interest_width, CV_32F);
		  
		  //CHECK_OR_PUT(grayscale_image);
          // Using C style openCV as it does what we need
          //CvMat area_of_interest_o = area_of_interest;
          //CvMat sim_o = sim;
		  //Mat tt = ma(grayscale_image);
          //IplImage im_o = tt;
		  //tic;
          cvGetQuadrangleSubPix(grayscale_image, area_of_interest, sim);

		  //CHECK_OR_PUT(area_of_interest);
          // get the correct size response window
          patch_expert_responses[i] = Mat(window_size, window_size, CV_32F);

          // Get intensity response either from the SVR or CCNF patch experts (prefer CCNF)
          if (!ccnf_expert_intensity.empty()) {
            ccnf_expert_intensity[scale][view_id][i].Response(ma(area_of_interest), patch_expert_responses[i]);
          }
          else {
            svr_expert_intensity[scale][view_id][i].Response(ma(area_of_interest), patch_expert_responses[i]);
          }
		  //toc;

          // if we have a corresponding depth patch and it is visible
          if (!svr_expert_depth.empty() && !imempty(depth_image) && ptr[i]) {

            Mat dProb = patch_expert_responses[i].clone();
			img_t* depthWindow = cvSetMat(im++, area_of_interest_height, area_of_interest_width, CC_32F);


            img_t* dimg_o = depthWindow;
			img_t* maskWindow = cvSetMat(im++, area_of_interest_height, area_of_interest_width, CC_32F);
			img_t* mimg_o = maskWindow;

            //img_t* d_o = ma(depth_image);
			//img_t* m_o = mask;

            cvGetQuadrangleSubPix(depth_image, dimg_o, sim);
            cvGetQuadrangleSubPix(depth_image, mimg_o, sim);

			img_t* mask1 = im++;
			//cmaskWindow < 1;
			cvCmpS(maskWindow, 1, mask1, CC_CMP_LT);
			cvSetReal(depthWindow, 0, mask1);

            svr_expert_depth[scale][view_id][i].ResponseDepth(ma(depthWindow), dProb);

            // Sum to one
            double sum1 = sum(patch_expert_responses[i])[0];

            // To avoid division by 0 issues
            if (sum1 == 0) {
              sum1 = 1;
            }

            patch_expert_responses[i] /= sum1;

            // Sum to one
            sum1 = sum(dProb)[0];

            // To avoid division by 0 issues
            if (sum1 == 0) {
              sum1 = 1;
            }

            dProb /= sum1;

            patch_expert_responses[i] = patch_expert_responses[i] + dProb;

          }
		  IM_END(im);
        }
      }
    }
#endif
  toc;

}


//=============================================================================
// Getting the closest view center based on orientation
int Patch_experts::GetViewIdx(const Vec6d& params_global, int scale) const
{
  int idx = 0;

  double dbest;

  for (int i = 0; i < this->nViews(scale); i++) {
    double v1 = params_global[1] - centers[scale][i][0];
    double v2 = params_global[2] - centers[scale][i][1];
    double v3 = params_global[3] - centers[scale][i][2];

    double d = v1 * v1 + v2 * v2 + v3 * v3;

    if (i == 0 || d < dbest) {
      dbest = d;
      idx = i;
    }
  }

  return idx;
}


//===========================================================================
void Patch_experts::Read(char** intensity_svr_expert_locations, char** depth_svr_expert_locations, char** intensity_ccnf_expert_locations)
{

  // initialise the SVR intensity patch expert parameters
  int num_intensity_svr = ppstrlen(intensity_svr_expert_locations);
  centers.resize(num_intensity_svr);
  visibilities.resize(num_intensity_svr);
  patch_scaling.resize(num_intensity_svr);

  svr_expert_intensity.resize(num_intensity_svr);
  int scale;
  // Reading in SVR intensity patch experts for each scales it is defined in
  for (scale = 0; scale < num_intensity_svr; ++scale) {
    char* location = intensity_svr_expert_locations[scale];
    printf("Reading the intensity SVR patch experts from: %s....", location);
    Read_SVR_patch_experts(location,  centers[scale], visibilities[scale], svr_expert_intensity[scale], patch_scaling[scale]);
  }

  // Initialise and read CCNF patch experts (currently only intensity based),
  int num_intensity_ccnf = ppstrlen(intensity_ccnf_expert_locations);

  // CCNF experts override the SVR ones
  if (num_intensity_ccnf > 0) {
    centers.resize(num_intensity_ccnf);
    visibilities.resize(num_intensity_ccnf);
    patch_scaling.resize(num_intensity_ccnf);
    ccnf_expert_intensity.resize(num_intensity_ccnf);
  }

  for (scale = 0; scale < num_intensity_ccnf; ++scale) {
    char* location = intensity_ccnf_expert_locations[scale];
    printf("Reading the intensity CCNF patch experts from: %s....", location);
    Read_CCNF_patch_experts(location,  centers[scale], visibilities[scale], ccnf_expert_intensity[scale], patch_scaling[scale]);
  }


  // initialise the SVR depth patch expert parameters
  int num_depth_scales = ppstrlen(depth_svr_expert_locations);
  int num_intensity_scales = centers.size();

  if (num_depth_scales > 0 && num_intensity_scales != num_depth_scales) {
    printf("Intensity and depth patch experts have a different number of scales, can't read depth\n");
    return;
  }

  // Have these to confirm that depth patch experts have the same number of views and scales and have the same visibilities
  vector<vector<Vec3d> > centers_depth(num_depth_scales);
  vector<vector<Mat > > visibilities_depth(num_depth_scales);
  vector<double> patch_scaling_depth(num_depth_scales);

  svr_expert_depth.resize(num_depth_scales);

  // Reading in SVR intensity patch experts for each scales it is defined in
  for (scale = 0; scale < num_depth_scales; ++scale) {
    char* location = depth_svr_expert_locations[scale];
    printf("Reading the depth SVR patch experts from: %s....", location);
    Read_SVR_patch_experts(location,  centers_depth[scale], visibilities_depth[scale], svr_expert_depth[scale], patch_scaling_depth[scale]);

    // Check if the scales are identical
    if (patch_scaling_depth[scale] != patch_scaling[scale]) {
      printf("Intensity and depth patch experts have a different scales, can't read depth\n");
      svr_expert_depth.clear();
      return;
    }

    int num_views_intensity = centers[scale].size();
    int num_views_depth = centers_depth[scale].size();

    // Check if the number of views is identical
    if (num_views_intensity != num_views_depth) {
      printf("Intensity and depth patch experts have a different number of scales, can't read depth\n");
      svr_expert_depth.clear();
      return;
    }

    for (int view = 0; view < num_views_depth; ++view) {
      if (countNonZero(centers_depth[scale][view] != centers[scale][view]) || countNonZero(visibilities[scale][view] != visibilities_depth[scale][view])) {
        printf("Intensity and depth patch experts have different visibilities or centers\n");
        svr_expert_depth.clear();
        return;
      }
    }
  }

}
//======================= Reading the SVR patch experts =========================================//
void Patch_experts::Read_SVR_patch_experts(char* expert_location, std::vector<Vec3d>& centers, std::vector<Mat >& visibility, std::vector<std::vector<Multi_SVR_patch_expert> >& patches, double& scale)
{
  FILE* patchesFile = fopen(expert_location, "rb");

  if (patchesFile) {
    SkipComments(patchesFile);

    fscanf(patchesFile, "%lf", &scale);

    SkipComments(patchesFile);

    int numberViews;

    fscanf(patchesFile, "%d", &numberViews);

    // read the visibility
    centers.resize(numberViews);
    visibility.resize(numberViews);

    patches.resize(numberViews);

    SkipComments(patchesFile);
    int i, j;
    // centers of each view (which view corresponds to which orientation)
    for (i = 0; i < centers.size(); i++) {
      Mat center;
      ReadMat(patchesFile, center);
      center.copyTo(centers[i]);
      centers[i] = centers[i] * (M_PI / 180.0);
    }

    SkipComments(patchesFile);

    // the visibility of points for each of the views (which verts are visible at a specific view
    for (i = 0; i < visibility.size(); i++) {
      ReadMat(patchesFile, visibility[i]);
    }

    int numberOfPoints = visibility[0].rows;

    SkipComments(patchesFile);

    // read the patches themselves
    for (i = 0; i < patches.size(); i++) {
      // number of patches for each view
      patches[i].resize(numberOfPoints);

      // read in each patch
      for (j = 0; j < numberOfPoints; j++) {
        patches[i][j].Read(patchesFile);
      }
    }

    printf("Done\n");
    fclose(patchesFile);
  }
  else {
    printf("Can't find/open the patches file\n");
  }
}

//======================= Reading the CCNF patch experts =========================================//
void Patch_experts::Read_CCNF_patch_experts(char* patchesFileLocation, std::vector<Vec3d>& centers, std::vector<Mat >& visibility, std::vector<std::vector<CCNF_patch_expert> >& patches, double& patchScaling)
{

  FILE* patchesFile = fopen(patchesFileLocation, "rb");

  if (patchesFile) {
    fread((char*)&patchScaling, 8, 1, patchesFile);

    int numberViews;
    fread((char*)&numberViews, 4, 1, patchesFile);

    // read the visibility
    centers.resize(numberViews);
    visibility.resize(numberViews);

    patches.resize(numberViews);
    int i;
    // centers of each view (which view corresponds to which orientation)
    for (i = 0; i < centers.size(); i++) {
      Mat center;
      ReadMatBin(patchesFile, center, CV_64F);
      center.copyTo(centers[i]);
      centers[i] = centers[i] * (M_PI / 180.0);
    }

    // the visibility of points for each of the views (which verts are visible at a specific view
    for (i = 0; i < visibility.size(); i++) {
      ReadMatBin(patchesFile, visibility[i], CV_32S);
    }

    int numberOfPoints = visibility[0].rows;

    // Read the possible SigmaInvs (without beta), this will be followed by patch reading (this assumes all of them have the same type, and number of betas)
    int num_win_sizes;
    int num_sigma_comp;
    fread((char*)&num_win_sizes, 4, 1, patchesFile);

    vector<int> windows;
    windows.resize(num_win_sizes);

    vector<vector<Mat > > sigma_components;
    sigma_components.resize(num_win_sizes);
    int w, s;
    for (w = 0; w < num_win_sizes; ++w) {
      fread((char*)&windows[w], 4, 1, patchesFile);

      fread((char*)&num_sigma_comp, 4, 1, patchesFile);

      sigma_components[w].resize(num_sigma_comp);

      for (s = 0; s < num_sigma_comp; ++s) {
        ReadMatBin(patchesFile, sigma_components[w][s], CV_32F);
      }
    }

    this->sigma_components = sigma_components;

    // read the patches themselves
    for (i = 0; i < patches.size(); i++) {
      // number of patches for each view
      patches[i].resize(numberOfPoints);

      // read in each patch
      for (int j = 0; j < numberOfPoints; j++) {
        patches[i][j].Read(patchesFile, windows, sigma_components);
      }
    }

    printf("Done\n");
  }
  else {
    printf("Can't find/open the patches file\n");
  }
}

