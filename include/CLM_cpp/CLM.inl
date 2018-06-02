

//=============================================================================
//=============================================================================

// Constructors
// A default constructor
CLM::CLM()
{
  CLMParameters parameters;
  this->Read(parameters.model_location);
}

// Constructor from a model file
CLM::CLM(const char* fname)
{
  hierarchical_model_names = NULL;
  this->Read(fname);
}

// Copy constructor (makes a deep copy of CLM)
CLM::CLM(const CLM& other): pdm(other.pdm), params_local(other.params_local.clone()), params_global(other.params_global), detected_landmarks(other.detected_landmarks.clone()),
  landmark_likelihoods(other.landmark_likelihoods.clone()), patch_experts(other.patch_experts), landmark_validator(other.landmark_validator),
  hierarchical_mapping(other.hierarchical_mapping), hierarchical_models(other.hierarchical_models), hierarchical_model_names(other.hierarchical_model_names),
  hierarchical_params(other.hierarchical_params)
{
  this->detection_success = other.detection_success;
  this->tracking_initialised = other.tracking_initialised;
  this->detection_certainty = other.detection_certainty;
  this->model_likelihood = other.model_likelihood;
  this->failures_in_a_row = other.failures_in_a_row;

  // Load the CascadeClassifier (as it does not have a proper copy constructor)

  // Make sure the matrices are allocated properly
  this->triangulations.resize(other.triangulations.size());

  for (size_t i = 0; i < other.triangulations.size(); ++i) {
    // Make sure the matrix is copied.
    this->triangulations[i] = other.triangulations[i].clone();
  }

  // Make sure the matrices are allocated properly
  for (std::map<int, Mat>::const_iterator it = other.kde_resp_precalc.begin(); it != other.kde_resp_precalc.end(); it++) {
    // Make sure the matrix is copied.
    this->kde_resp_precalc.insert(std::pair<int, Mat>(it->first, it->second.clone()));
  }

#ifdef _USE_DLIB
  this->face_detector_HOG = dlib::get_frontal_face_detector();
#endif

}

// Assignment operator for lvalues (makes a deep copy of CLM)
CLM& CLM::operator= (const CLM& other)
{
  if (this != &other) { // protect against invalid self-assignment
    pdm = PDM(other.pdm);
    params_local = other.params_local.clone();
    params_global = other.params_global;
    detected_landmarks = other.detected_landmarks.clone();

    landmark_likelihoods = other.landmark_likelihoods.clone();
    patch_experts = Patch_experts(other.patch_experts);
    landmark_validator = DetectionValidator(other.landmark_validator);
    //face_detector_location = other.face_detector_location;

    this->detection_success = other.detection_success;
    this->tracking_initialised = other.tracking_initialised;
    this->detection_certainty = other.detection_certainty;
    this->model_likelihood = other.model_likelihood;
    this->failures_in_a_row = other.failures_in_a_row;
	
    // Make sure the matrices are allocated properly
    this->triangulations.resize(other.triangulations.size());

    for (size_t i = 0; i < other.triangulations.size(); ++i) {
      // Make sure the matrix is copied.
      this->triangulations[i] = other.triangulations[i].clone();
    }

    // Make sure the matrices are allocated properly
    for (std::map<int, Mat>::const_iterator it = other.kde_resp_precalc.begin(); it != other.kde_resp_precalc.end(); it++) {
      // Make sure the matrix is copied.
      this->kde_resp_precalc.insert(std::pair<int, Mat>(it->first, it->second.clone()));
    }
  }

#ifdef _USE_DLIB
  face_detector_HOG = dlib::get_frontal_face_detector();
#endif

  return *this;
}

#if 0
// Move constructor
CLM::CLM(const CLM && other)
{
  this->detection_success = other.detection_success;
  this->tracking_initialised = other.tracking_initialised;
  this->detection_certainty = other.detection_certainty;
  this->model_likelihood = other.model_likelihood;
  this->failures_in_a_row = other.failures_in_a_row;

  pdm = other.pdm;
  params_local = other.params_local;
  params_global = other.params_global;
  detected_landmarks = other.detected_landmarks;
  landmark_likelihoods = other.landmark_likelihoods;
  patch_experts = other.patch_experts;
  landmark_validator = other.landmark_validator;
  //face_detector_location = other.face_detector_location;

  //face_detector_HAAR = other.face_detector_HAAR;

  triangulations = other.triangulations;
  kde_resp_precalc = other.kde_resp_precalc;

#ifdef _USE_DLIB
  face_detector_HOG = dlib::get_frontal_face_detector();
#endif

}

// Assignment operator for rvalues
CLM& CLM::operator= (const CLM && other)
{
  this->detection_success = other.detection_success;
  this->tracking_initialised = other.tracking_initialised;
  this->detection_certainty = other.detection_certainty;
  this->model_likelihood = other.model_likelihood;
  this->failures_in_a_row = other.failures_in_a_row;

  pdm = other.pdm;
  params_local = other.params_local;
  params_global = other.params_global;
  detected_landmarks = other.detected_landmarks;
  landmark_likelihoods = other.landmark_likelihoods;
  patch_experts = other.patch_experts;
  landmark_validator = other.landmark_validator;
  //face_detector_location = other.face_detector_location;

  //face_detector_HAAR = other.face_detector_HAAR;

  triangulations = other.triangulations;
  kde_resp_precalc = other.kde_resp_precalc;

#ifdef _USE_DLIB
  face_detector_HOG = dlib::get_frontal_face_detector();
#endif

  return *this;
}
#endif


void CLM::Read_CLM(const char* clm_location)
{
  // Location of modules
  FILE* locations = fopen(clm_location, "rb");

  if (!locations) {
    printf("Couldn't open the CLM model file aborting\n");
    return;
  }

  char line[1024];

  char** intensity_expert_locations = NULL;
  char** depth_expert_locations = NULL;
  char** ccnf_expert_locations = NULL;
char buf[256];
  // The other module locations should be defined as relative paths from the main model
  char* root = parent_path(clm_location, buf, 256);

  // The main file contains the references to other files
  while (!feof(locations)) {
    fgets(line, countof(line), locations);

    char* lineStream = (line);

    char module[256];
    char location[256];
    char location2[256];

    // figure out which module is to be read from which file
    sscanf(lineStream, "%s %s", module, location);
    int location_len = strlen(location);
    if (location_len > 0) {
      //location.erase(location.begin());  // remove the first space
    }

    // remove carriage return at the end for compatibility with unix systems
    if (location_len > 0 && location[location_len - 1] == '\r') {
      location[--location_len] = 0;
    }

    // append the lovstion to root location (boost syntax)
    _snprintf(location2, 256, "%s/%s", root, location);
    strcpy(location, location2);

    if (strcmp(module, "PDM") == 0) {
      printf("Reading the PDM module from: %s....", location);
      pdm.Read(location);
      printf("Done\n");
    }
    else if (strcmp(module, "Triangulations") == 0) {
      printf("Reading the Triangulations module from: %s....", location);
      FILE* triangulationFile = fopen(location, "rb");

      SkipComments(triangulationFile);

      int numViews;
      fscanf(triangulationFile, "%d", &numViews);

      // read in the triangulations
      triangulations.resize(numViews);

      for (int i = 0; i < numViews; ++i) {
        SkipComments(triangulationFile);
        ReadMat(triangulationFile, triangulations[i]);
      }

      fclose(triangulationFile);
      printf("Done\n");
    }
    else if (strcmp(module, "PatchesIntensity") == 0) {
      pstrcat(ppstrnew(&intensity_expert_locations), location);
    }
    else if (strcmp(module, "PatchesDepth") == 0) {
      pstrcat(ppstrnew(&depth_expert_locations), location);
    }
    else if (strcmp(module, "PatchesCCNF") == 0) {
      pstrcat(ppstrnew(&ccnf_expert_locations), location);
    }
  }

  // Initialise the patch experts
  patch_experts.Read(intensity_expert_locations, depth_expert_locations, ccnf_expert_locations);

#ifdef _USE_DLIB
  // Read in a face detector
  face_detector_HOG = dlib::get_frontal_face_detector();
#endif
  ppstr_free(&intensity_expert_locations);
  ppstr_free(&depth_expert_locations);
  ppstr_free(&ccnf_expert_locations);
}

void CLM::Read(const char* main_location)
{
  printf("Reading the CLM landmark detector/tracker from: %s\n", main_location);

  FILE* locations = fopen(main_location, "rb");

  if (!locations) {
    printf("Couldn't open the model file, aborting\n");
    return;
  }

  char line[1024];

  char buf[256];
  // The other module locations should be defined as relative paths from the main model
  char* root = parent_path(main_location, buf, 256);
  // The main file contains the references to other files
  while (!feof(locations)) {
    fgets(line, countof(line), locations);

    str_t lineStream = STR1(line);
    int j=0;
    char module[256];
    char location[256];
    char location2[256];

    // figure out which module is to be read from which file
    cstr_getstr(lineStream, &j, NULL, module, 256, " \r\n\t");
    cstr_getstr(lineStream, &j, NULL, location, 256, " \r\n\t");
    // append to root
    _snprintf(location2, 256, "%s/%s", root, location);
    strcpy(location, location2);

    if (strcmp(module, "CLM") == 0) {
      printf("Reading the CLM module from: %s\n", location);

      // The CLM module includes the PDM and the patch experts
      Read_CLM(location);
    }
    else if (strcmp(module, "CLM_part") == 0) {
      char part_name[256];
      cstr_getstr(lineStream, &j, NULL, part_name, 256, " \r\n\t");
      printf("Reading part based module....%s\n", part_name);

      vector<pair<int, int> > mappings;

      while (j<lineStream.l) {
        int ind_in_main;
        int ind_in_part;
        cstr_getint(lineStream, &j, &ind_in_main, 0);
        cstr_getint(lineStream, &j, &ind_in_part, 0);
        mappings.push_back(pair<int, int>(ind_in_main, ind_in_part));
      }

      this->hierarchical_mapping.push_back(mappings);

      CLM part_model(location);

      this->hierarchical_models.push_back(part_model);
      pstrcat(ppstrnew(&hierarchical_model_names), part_name);

      CLMParameters params;
      params.validate_detections = false;
      params.refine_hierarchical = false;
      params.refine_parameters = false;

      if (strcmp(part_name, "left_eye") == 0 || strcmp(part_name, "right_eye") == 0) {

        vector<int> windows_large;
        windows_large.push_back(5);
        windows_large.push_back(3);

        vector<int> windows_small;
        windows_small.push_back(5);
        windows_small.push_back(3);

        params.window_sizes_init = windows_large;
        params.window_sizes_small = windows_small;
        params.window_sizes_current = windows_large;

        params.reg_factor = 0.1;
        params.sigma = 2;
      }
      else if (strcmp(part_name, "left_eye_28") == 0 || strcmp(part_name, "right_eye_28") == 0) {
        vector<int> windows_large;
        windows_large.push_back(3);
        windows_large.push_back(5);
        windows_large.push_back(9);

        vector<int> windows_small;
        windows_small.push_back(3);
        windows_small.push_back(5);
        windows_small.push_back(9);

        params.window_sizes_init = windows_large;
        params.window_sizes_small = windows_small;
        params.window_sizes_current = windows_large;

        params.reg_factor = 0.5;
        params.sigma = 1.0;
      }
      else if (strcmp(part_name, "mouth") == 0) {
        vector<int> windows_large;
        windows_large.push_back(7);
        windows_large.push_back(7);

        vector<int> windows_small;
        windows_small.push_back(7);
        windows_small.push_back(7);

        params.window_sizes_init = windows_large;
        params.window_sizes_small = windows_small;
        params.window_sizes_current = windows_large;

        params.reg_factor = 1.0;
        params.sigma = 2.0;
      }
      else if (strcmp(part_name, "brow") == 0) {
        vector<int> windows_large;
        windows_large.push_back(11);
        windows_large.push_back(9);

        vector<int> windows_small;
        windows_small.push_back(11);
        windows_small.push_back(9);

        params.window_sizes_init = windows_large;
        params.window_sizes_small = windows_small;
        params.window_sizes_current = windows_large;

        params.reg_factor = 10.0;
        params.sigma = 3.5;
      }
      else if (strcmp(part_name, "inner") == 0) {
        vector<int> windows_large;
        windows_large.push_back(9);

        vector<int> windows_small;
        windows_small.push_back(9);

        params.window_sizes_init = windows_large;
        params.window_sizes_small = windows_small;
        params.window_sizes_current = windows_large;

        params.reg_factor = 2.5;
        params.sigma = 1.75;
        params.weight_factor = 2.5;
      }

      this->hierarchical_params.push_back(params);

      printf("Done\n");
    }
    else if (strcmp(module, "DetectionValidator") == 0) {
      printf("Reading the landmark validation module....");
      landmark_validator.Read(location);
      printf("Done\n");
    }
  }

  detected_landmarks.create(2 * pdm.NumberOfPoints(), 1, CV_64F);
  detected_landmarks.setTo(0);

  detection_success = false;
  tracking_initialised = false;
  model_likelihood = -10; // very low
  detection_certainty = 1; // very uncertain

  // Initialising default values for the rest of the variables

  // local parameters (shape)
  params_local.create(pdm.NumberOfModes(), 1, CV_64F);
  params_local.setTo(0.0);

  // global parameters (pose) [scale, euler_x, euler_y, euler_z, tx, ty]
  params_global = Vec6d(1, 0, 0, 0, 0, 0);

  failures_in_a_row = -1;
}

// Resetting the model (for a new video, or complet reinitialisation
void CLM::Reset()
{
  detected_landmarks.setTo(0);

  detection_success = false;
  tracking_initialised = false;
  model_likelihood = -10;  // very low
  detection_certainty = 1; // very uncertain

  // local parameters (shape)
  params_local.setTo(0.0);

  // global parameters (pose) [scale, euler_x, euler_y, euler_z, tx, ty]
  params_global = Vec6d(1, 0, 0, 0, 0, 0);

  failures_in_a_row = -1;
  face_template = Mat();
}

// Resetting the model, choosing the face nearest (x,y)
void CLM::Reset(double x, double y)
{

  // First reset the model overall
  this->Reset();

  // Now in the following frame when face detection takes place this is the point at which it will be preffered
  this->preference_det.x = x;
  this->preference_det.y = y;

}

//#define PUSHARG(args, j, T, NAME)     args[j++] =& NAME;
//#define POPARG(args, j, T, NAME)      T& NAME = *(T*)args[j++];

#define DETECTLANDMARKS_WORK_ARGS(args1, j, OPT_DEF) \
	OPT_DEF(args1, j, char**, hierarchical_model_names) \
	OPT_DEF(args1, j, vector<CLM>,           hierarchical_models) \
	OPT_DEF(args1, j, CLMParameters, params) \
	OPT_DEF(args1, j, CLM, me) \
	OPT_DEF(args1, j, bool, parts_used) \
	OPT_DEF(args1, j, const img_t*, depth) \
	OPT_DEF(args1, j, const img_t*, image) \
	OPT_DEF(args1, j, vector<int>, part_models) \

#if 0
int DetectLandmarks_work(void* arg, int beg, int end) {
	int j = 0;
	void** args = (void**)arg;
	DETECTLANDMARKS_WORK_ARGS(args, j, POPARG);
	for (int i = beg; i < end; ++i) {
		int part_model = part_models[i];
		// Only do the synthetic eye models if we're doing gaze

			int n_part_points = hierarchical_models[part_model].pdm.NumberOfPoints();

			vector<pair<int, int>> mappings = me.hierarchical_mapping[part_model];

			Mat part_model_locs(n_part_points * 2, 1, CV_64F, 0.0);

			// Extract the corresponding landmarks
			for (size_t mapping_ind = 0; mapping_ind < mappings.size(); ++mapping_ind) {
				part_model_locs.at<double>(mappings[mapping_ind].second) = me.detected_landmarks.at<double>(mappings[mapping_ind].first);
				part_model_locs.at<double>(mappings[mapping_ind].second + n_part_points) = me.detected_landmarks.at<double>(mappings[mapping_ind].first + me.pdm.NumberOfPoints());
			}

			// Fit the part based model PDM
			//tic;
			hierarchical_models[part_model].pdm.CalcParams(hierarchical_models[part_model].params_global, hierarchical_models[part_model].params_local, part_model_locs);
			//toc;
			// Only do this if we don't need to upsample
			if (me.params_global[0] > 0.9 * hierarchical_models[part_model].patch_experts.patch_scaling[0]) {
				parts_used = true;

				me.hierarchical_params[part_model].window_sizes_current = me.hierarchical_params[part_model].window_sizes_init;

				// Do the actual landmark detection
				hierarchical_models[part_model].DetectLandmarks(image, depth, me.hierarchical_params[part_model]);

        double* detected_landmarks_ptr = img_ptr(double, &hierarchical_models[part_model].detected_landmarks);
				// Reincorporate the models into main tracker
				for (size_t mapping_ind = 0; mapping_ind < mappings.size(); ++mapping_ind) {
					me.detected_landmarks.at<double>(mappings[mapping_ind].first) = detected_landmarks_ptr[mappings[mapping_ind].second];
					me.detected_landmarks.at<double>(mappings[mapping_ind].first + me.pdm.NumberOfPoints()) = detected_landmarks_ptr[mappings[mapping_ind].second + hierarchical_models[part_model].pdm.NumberOfPoints()];
				}
			}
			else {
				hierarchical_models[part_model].pdm.CalcShape2D(hierarchical_models[part_model].detected_landmarks, hierarchical_models[part_model].params_local, hierarchical_models[part_model].params_global);
			}
		
	}
	return 0;
}
#endif

// The main internal landmark detection call (should not be used externally?)
bool CLM::DetectLandmarks(const img_t* image, const img_t* depth, CLMParameters& params)
{
  // Fits from the current estimate of local and global parameters in clm_model
  bool fit_success;
  tic;
  fit_success = Fit(image, depth, params.window_sizes_current, params);
  toc;
  // Store the landmarks converged on in detected_landmarks
  pdm.CalcShape2D(detected_landmarks, params_local, params_global);

	if (params.refine_hierarchical && hierarchical_models.size() > 0) {
		bool parts_used = false;

#if 0
		{
			void* args[20];
			int j = 0;
			CLM& me = *this;
			std::vector<int> part_models;
			DETECTLANDMARKS_WORK_ARGS(args, j, PUSHARG);
			for (int part_model = 0; part_model< (int)hierarchical_models.size(); ++part_model) {
				if (!((hierarchical_model_names[part_model].compare("right_eye_28") == 0 ||
					hierarchical_model_names[part_model].compare("left_eye_28") == 0)
					&& !params.track_gaze)) {
					part_models.push_back(part_model);
				}
			}
			parallel_for(args, 0, part_models.size(), DetectLandmarks_work);

		}
#else
		// Do the hierarchical models in parallel
		myparallel_for(0, (int)hierarchical_models.size(), part_model) {
			// Only do the synthetic eye models if we're doing gaze
      char* str = hierarchical_model_names[part_model];
			if (!((strcmp(str, "right_eye_28") == 0 || strcmp(str, "left_eye_28") == 0)
				&& !params.track_gaze)) {

				int n_part_points = hierarchical_models[part_model].pdm.NumberOfPoints();

				vector<pair<int, int> > mappings = this->hierarchical_mapping[part_model];

				Mat part_model_locs(n_part_points * 2, 1, CV_64F, 0.0);

        double* part_model_locs_ptr = img_ptr(double, &part_model_locs);
        double* detected_landmarks_ptr = img_ptr(double, &detected_landmarks);
				tic;
				// Extract the corresponding landmarks
				for (size_t mapping_ind = 0; mapping_ind < mappings.size(); ++mapping_ind) {
          int f = mappings[mapping_ind].first;
          int s = mappings[mapping_ind].second;
					part_model_locs_ptr[s]                 = detected_landmarks_ptr[f];
					part_model_locs_ptr[s + n_part_points] = detected_landmarks_ptr[f + this->pdm.NumberOfPoints()];
				}

				// Fit the part based model PDM
				//tic;
				hierarchical_models[part_model].pdm.CalcParams(hierarchical_models[part_model].params_global, hierarchical_models[part_model].params_local, part_model_locs);
				toc;
				// Only do this if we don't need to upsample
				if (params_global[0] > 0.9 * hierarchical_models[part_model].patch_experts.patch_scaling[0]) {
					parts_used = true;

					this->hierarchical_params[part_model].window_sizes_current = this->hierarchical_params[part_model].window_sizes_init;

					// Do the actual landmark detection
					hierarchical_models[part_model].DetectLandmarks(image, depth, hierarchical_params[part_model]);
          double* detected_landmarks_ptr = img_ptr(double, &hierarchical_models[part_model].detected_landmarks);
					// Reincorporate the models into main tracker
					for (size_t mapping_ind = 0; mapping_ind < mappings.size(); ++mapping_ind) {
						detected_landmarks_ptr[mappings[mapping_ind].first] = detected_landmarks_ptr[mappings[mapping_ind].second];
						detected_landmarks_ptr[mappings[mapping_ind].first + pdm.NumberOfPoints()] = detected_landmarks_ptr[mappings[mapping_ind].second + hierarchical_models[part_model].pdm.NumberOfPoints()];
					}
				}
				else {
					hierarchical_models[part_model].pdm.CalcShape2D(hierarchical_models[part_model].detected_landmarks, hierarchical_models[part_model].params_local, hierarchical_models[part_model].params_global);
				}
			}
		}
#endif

		// Recompute main model based on the fit part models
		if (parts_used) {
			pdm.CalcParams(params_global, params_local, detected_landmarks);
			pdm.CalcShape2D(detected_landmarks, params_local, params_global);
		}
	}

	// Check detection correctness
	if (params.validate_detections && fit_success) {
		Vec3d orientation(params_global[1], params_global[2], params_global[3]);
		//tic;
		detection_certainty = landmark_validator.Check(orientation, ma(image), detected_landmarks);
		//toc;

		detection_success = detection_certainty < params.validation_boundary;
	}
	else {
		detection_success = fit_success;

		if (fit_success) {
			detection_certainty = -1;
		}
		else {
			detection_certainty = 1;
		}

	}

	return detection_success;
}

//=============================================================================
bool CLM::Fit(const img_t* img, const img_t* depthImg, const std::vector<int>& window_sizes, const CLMParameters& clm_parameters)
{
	//CHECK_OR_PUT(img);
  // Making sure it is a single channel image
  assert(img->channels() == 1);

  // Placeholder for the landmarks
  Mat current_shape(2 * pdm.NumberOfPoints() , 1, CV_64F, 0.0);

  int n = pdm.NumberOfPoints();

  Mat depth_img_no_background;

  // Background elimination from the depth image
  if (!imempty(depthImg)) {
    bool success = RemoveBackground(depth_img_no_background, ma(depthImg));

    // The attempted background removal can fail leading to tracking failure
    if (!success) {
      return false;
    }
  }

  int num_scales = patch_experts.patch_scaling.size();

  // Storing the patch expert response maps
  vector<Mat > patch_expert_responses(n);

  // Converting from image space to patch expert space (normalised for rotation and scale)
  Matx22d sim_ref_to_img;
  Matx22d sim_img_to_ref;

  CLMParameters tmp_parameters = clm_parameters;
  // Optimise the model across a number of areas of interest (usually in descending window size and ascending scale size)
  for (int scale = 0; scale < num_scales; scale++) {
    
    int window_size = window_sizes[scale];
    
    if (window_size == 0 ||  0.9 * patch_experts.patch_scaling[scale] > params_global[0]) {
      continue;
    }
    
    tic;
    // The patch expert response computation
    if (scale != window_sizes.size() - 1) {
      //{utime_start(_start_time);
      patch_experts.Response(patch_expert_responses, sim_ref_to_img, sim_img_to_ref, (img), &depth_img_no_background, pdm, params_global, params_local, window_size, scale);
      //printf("%f\n", utime_elapsed(_start_time)); }
    }
    else {
      // Do not use depth for the final iteration as it is not as accurate
      patch_experts.Response(patch_expert_responses, sim_ref_to_img, sim_img_to_ref, (img), NULL, pdm, params_global, params_local, window_size, scale);
    }
    toc;
    if (clm_parameters.refine_parameters == true) {
      // Adapt the parameters based on scale (wan't to reduce regularisation as scale increases, but increa sigma and tikhonov)
      tmp_parameters.reg_factor = clm_parameters.reg_factor - 15 * log(patch_experts.patch_scaling[scale] / 0.25) / log(2);
      
      if (tmp_parameters.reg_factor <= 0) {
        tmp_parameters.reg_factor = 0.001;
      }
      
      tmp_parameters.sigma = clm_parameters.sigma + 0.25 * log(patch_experts.patch_scaling[scale] / 0.25) / log(2);
      tmp_parameters.weight_factor = clm_parameters.weight_factor + 2 * clm_parameters.weight_factor *  log(patch_experts.patch_scaling[scale] / 0.25) / log(2);
    }

    // Get the current landmark locations
    pdm.CalcShape2D(current_shape, params_local, params_global);

	//CHECK_OR_PUT(current_shape);
	//CHECK_OR_PUT(params_local);
	Mat params_global_mat(params_global);
	//CHECK_OR_PUT(params_global_mat);
    // Get the view used by patch experts
    int view_id = patch_experts.GetViewIdx(params_global, scale);

    Mat params_local0 = convertTo(params_local, CV_64F);
	tic;
    // the actual optimisation step
    this->NU_RLMS(params_global, (params_local0), patch_expert_responses, Vec6d(params_global), params_local.clone(), current_shape, sim_img_to_ref, sim_ref_to_img, window_size, view_id, true, scale, this->landmark_likelihoods, tmp_parameters);
	
    // non-rigid optimisation
    this->model_likelihood = this->NU_RLMS(params_global, (params_local0), patch_expert_responses, Vec6d(params_global), params_local.clone(), current_shape, sim_img_to_ref, sim_ref_to_img, window_size, view_id, false, scale, this->landmark_likelihoods, tmp_parameters);
	toc;
    params_local = convertTo(params_local0, CV_64F);

    // Can't track very small images reliably (less than ~30px across)
    if (params_global[0] < 0.25) {
      printf("Detection too small for CLM\n");
      return false;
    }
  }

  return true;
}

void CLM::NonVectorisedMeanShift_precalc_kde(Mat& out_mean_shifts, const vector<Mat >& patch_expert_responses,
	const Mat& dxs, const Mat& dys, int resp_size, float a, int scale, int view_id, map<int, Mat >& kde_resp_precalc)
{
	IM_BEGIN(im, 10);
  int n = dxs.rows;

  img_t *kde_resp = im++;
  float step_size = 0.1;

  // if this has not been precomputer, precompute it, otherwise use it
  if (kde_resp_precalc.find(resp_size) == kde_resp_precalc.end()) {
	  int n = (int)(resp_size / step_size);
    cvSetMat(kde_resp, n*n, resp_size * resp_size, CC_32F);
	int x, y, ii, jj;
	img_t* kde_resp_map = cvSetMat(im, n, resp_size, CC_32F);
	float* kde_it;
	kde_it  = img_ptr(float, kde_resp);
	float* kde_resp_map1 = img_ptr(float, kde_resp_map);

	for (y = 0; y < n; y++) {
		float dy = y * step_size;
		float* kde_resp_map1 = img_row(float, kde_resp_map, y);
		for (ii = 0; ii < resp_size; ii++) {
			kde_resp_map1[ii] = a* (dy - ii) * (dy - ii);
		}
	}
	for (x = 0; x < n; x++) {
		float *kde_resp_map2 = kde_resp_map1 + x*resp_size;
		for (y = 0; y < n; y++) {
			float *kde_resp_map3 = kde_resp_map1 + y*resp_size;
			for (ii = 0; ii < resp_size; ii++) {
				float vx = kde_resp_map3[ii];
#if 1
				jj = 0;
				for (; jj < resp_size-3; jj+=4, kde_it+=4) {
					kde_it[0] = vx + kde_resp_map2[jj + 0];
					kde_it[1] = vx + kde_resp_map2[jj + 1];
					kde_it[2] = vx + kde_resp_map2[jj + 2];
					kde_it[3] = vx + kde_resp_map2[jj + 3];
				}
				for (; jj < resp_size; jj++) {
					*kde_it++ = (vx + kde_resp_map2[jj]);
				}
#else
				for (jj = 0; jj < resp_size; jj++) {
					*kde_it++ = (vx + kde_resp_map2[jj]);
				}
#endif

			}
		}
	}

	cvExp(kde_resp, kde_resp);
#if 0
	kde_it = kde_resp.ptr<float>();
	tic;
    for (int x = 0; x < n; x++) {
      float dx = x * step_size;

      for (int y = 0; y < n; y++) {
        float dy = y * step_size;

        int ii, jj;
        float v, vx, vy;

        for (ii = 0; ii < resp_size; ii++) {
          vx = (dy - ii) * (dy - ii);

          for (jj = 0; jj < resp_size; jj++) {
            vy = (dx - jj) * (dx - jj);

            // the KDE evaluation of that point
            v = exp(a * (vx + vy));

            *kde_it++ = v;
          }
        }
      }
    }
	toc_;
#endif
    kde_resp_precalc[resp_size] = ma(kde_resp);
  }
  else {
    // use the precomputed version
	  img_t* tt = &(kde_resp_precalc.find(resp_size)->second);
    cvCloneMat(kde_resp, tt);
  }
  Mat& visibilities = patch_experts.visibilities[scale][view_id];
  CHECK_OR_PUT(kde_resp);
  CHECK_OR_PUT(visibilities);
  // for every point (patch) calculating mean-shift
  int* visibilities_ptr = img_ptr(int, &visibilities);
  float* out_mean_shifts_ptr = img_ptr(float, &out_mean_shifts);
  float* dxs_ptr = img_ptr(float, &dxs);
  float* dys_ptr = img_ptr(float, &dys);
  for (int i = 0; i < n; i++) {
    if (visibilities_ptr[i] == 0) {
      out_mean_shifts_ptr[i] = 0;
      out_mean_shifts_ptr[i + n] = 0;
      continue;
    }

    // indices of dx, dy
    float dx = dxs_ptr[i];
    float dy = dys_ptr[i];

    // Ensure that we are within bounds (important for precalculation)
    if (dx < 0) {
      dx = 0;
    }

    if (dy < 0) {
      dy = 0;
    }

    if (dx > resp_size - step_size) {
      dx = resp_size - step_size;
    }

    if (dy > resp_size - step_size) {
      dy = resp_size - step_size;
    }

    // Pick the row from precalculated kde that approximates the current dx, dy best
    int closest_col = (int)(dy / step_size + 0.5); // Plus 0.5 is there, as C++ rounds down with int cast
    int closest_row = (int)(dx / step_size + 0.5); // Plus 0.5 is there, as C++ rounds down with int cast

    int idx = closest_row * ((int)(resp_size / step_size + 0.5)) + closest_col; // Plus 0.5 is there, as C++ rounds down with int cast

    float* kde_it = img_ptr(float, kde_resp) + kde_resp->cols * idx;

    double mx = 0.0;
	double my = 0.0;
	double sum = 0.0;

    // Iterate over the patch responses here
	const Mat& patch_expert_responses_i = patch_expert_responses[i];
	CHECK_OR_PUT(patch_expert_responses_i);
  const float* p = img_ptr(float, &patch_expert_responses[i]);

    for (int ii = 0; ii < resp_size; ii++) {
      for (int jj = 0; jj < resp_size; jj++) {

        // the KDE evaluation of that point multiplied by the probability at the current, xi, yi
        float v = (*p++) * (*kde_it++);

        sum += v;

        // mean shift in x and y
        mx += v * jj;
        my += v * ii;

      }
    }

	double msx = (mx / sum - dx);
	double msy = (my / sum - dy);

    out_mean_shifts_ptr[i] = (float)msx;
    out_mean_shifts_ptr[i + n] = (float)msy;
  }
  CHECK_OR_PUT(out_mean_shifts);

  IM_END(im);
}

void CLM::GetWeightMatrix(Mat& WeightMatrix, int scale, int view_id, const CLMParameters& parameters)
{
  int n = pdm.NumberOfPoints();

  // Is the weight matrix needed at all
  if (parameters.weight_factor > 0) {
    WeightMatrix = Mat::zeros(n * 2, n * 2, CV_32F);

    for (int p = 0; p < n; p++) {
  		float* WeightMatrix_p = img_row(float, &WeightMatrix, p);
		  float* WeightMatrix_pn = img_row(float, &WeightMatrix, p+n);
      if (!patch_experts.ccnf_expert_intensity.empty()) {

        // for the x dimension
		  WeightMatrix_p[p] = WeightMatrix_p[p]  + patch_experts.ccnf_expert_intensity[scale][view_id][p].patch_confidence;

        // for they y dimension
		  WeightMatrix_pn[p + n] = WeightMatrix_p[p];

      }
      else {
        // Across the modalities add the confidences
        for (size_t pc = 0; pc < patch_experts.svr_expert_intensity[scale][view_id][p].svr_patch_experts.size(); pc++) {
          // for the x dimension
			WeightMatrix_p[p] = WeightMatrix_p[p] + patch_experts.svr_expert_intensity[scale][view_id][p].svr_patch_experts.at(pc).confidence;
        }

        // for the y dimension
		WeightMatrix_pn[p + n] = WeightMatrix_p[p];
      }
    }

    WeightMatrix = parameters.weight_factor * WeightMatrix;
  }
  else {
    WeightMatrix = Mat::eye(n * 2, n * 2, CV_32F);
  }

}

//=============================================================================
double CLM::NU_RLMS(Vec6d& final_global, Mat& final_local, const vector<Mat >& patch_expert_responses, const Vec6d& initial_global, const Mat& initial_local,
    const Mat& base_shape, const Matx22d& sim_img_to_ref, const Matx22d& sim_ref_to_img, int resp_size, int view_id, bool rigid, int scale, Mat& landmark_lhoods,
    const CLMParameters& parameters)
{
  int n = pdm.NumberOfPoints();

  // Mean, eigenvalues, eigenvectors
  Mat M = this->pdm.mean_shape;
  Mat E = this->pdm.eigen_values;
  //Mat V = this->pdm.princ_comp;

  int m = pdm.NumberOfModes();

  //CHECK_OR_PUT(M);
  //CHECK_OR_PUT(E);
  Vec6d current_global(initial_global);

  Mat current_local;
  initial_local.convertTo(current_local, CV_32F);
  //CHECK_OR_PUT(current_global);
  //CHECK_OR_PUT(current_local);

  Mat current_shape;
  Mat previous_shape;

  // Pre-calculate the regularisation term
  Mat regTerm;

  if (rigid) {
    regTerm = Mat::zeros(6, 6, CV_32F);
  }
  else {
    Mat regularisations = Mat::zeros(1, 6 + m, CV_64F);

    // Setting the regularisation to the inverse of eigenvalues
    //Mat(parameters.reg_factor / E).copyTo(regularisations(Rect(6, 0, m, 1)));
	copyTo(parameters.reg_factor / E, regularisations, 6, 0, m, 1);
    Mat regTerm_d = Mat::diag(regularisations.t());
    regTerm_d.convertTo(regTerm, CV_32F);
  }

  Mat WeightMatrix;
  GetWeightMatrix(WeightMatrix, scale, view_id, parameters);

  //CHECK_OR_PUT(WeightMatrix);
  Mat dxs, dys;

  // The preallocated memory for the mean shifts
  Mat mean_shifts(2 * pdm.NumberOfPoints(), 1, CV_32F, 0.0);

  // Number of iterations
  for (int iter = 0; iter < parameters.num_optimisation_iteration; iter++) {
    // get the current estimates of x
    pdm.CalcShape2D(current_shape, convertTo(current_local, CV_64F), current_global);
	//CHECK_OR_PUT(current_local);

    if (iter > 0) {
      // if the shape hasn't changed terminate
      if (norm(current_shape, previous_shape) < 0.01) {
        break;
      }
    }

    current_shape.copyTo(previous_shape);
	CHECK_OR_PUT(current_shape);

    // Jacobian, and transposed weighted jacobian
    Mat J, J_w_t;

    // calculate the appropriate Jacobians in 2D, even though the actual behaviour is in 3D, using small angle approximation and oriented shape
    if (rigid) {
      pdm.ComputeRigidJacobian(current_local, current_global, J, WeightMatrix, J_w_t);
    }
    else {
      pdm.ComputeJacobian(current_local, current_global, J, WeightMatrix, J_w_t);
    }

	//CHECK_OR_PUT(J_w_t);

    // useful for mean shift calculation
    double a = -0.5 / (parameters.sigma * parameters.sigma);

    Mat current_shape_2D = current_shape.reshape(1, 2).t();
    Mat base_shape_2D = base_shape.reshape(1, 2).t();

    Mat offsets;
    Mat((current_shape_2D - base_shape_2D) * Mat(sim_img_to_ref).t()).convertTo(offsets, CV_32F);

    dxs = offsets.col(0) + (resp_size - 1) / 2;
    dys = offsets.col(1) + (resp_size - 1) / 2;

	CHECK_OR_PUT(dxs);
	CHECK_OR_PUT(dys);
    NonVectorisedMeanShift_precalc_kde(mean_shifts, patch_expert_responses, dxs, dys, resp_size, a, scale, view_id, kde_resp_precalc);
	CHECK_OR_PUT(mean_shifts);
    // Now transform the mean shifts to the the image reference frame, as opposed to one of ref shape (object space)
    Mat mean_shifts_2D = (mean_shifts.reshape(1, 2)).t();

	Mat sim_ref_to_imgf;
	Mat(sim_ref_to_img).convertTo(sim_ref_to_imgf, CV_32F);
    mean_shifts_2D = mean_shifts_2D * sim_ref_to_imgf.t();
    mean_shifts = Mat(mean_shifts_2D.t()).reshape(1, n * 2);
    float* mean_shifts_ptr = img_ptr(float, &mean_shifts);
    // remove non-visible observations
    for (int i = 0; i < n; ++i) {
      // if patch unavailable for current index
      int* ptr = img_ptr(int, &patch_experts.visibilities[scale][view_id]);
      if (ptr[i] == 0) {
        Mat Jx = J.row(i);
        //Jx = cvScalar(0);
		cvZero(&Jx);
        Mat Jy = J.row(i + n);
        //Jy = cvScalar(0);
		cvZero(&Jy);
        mean_shifts_ptr[i] = 0.0f;
        mean_shifts_ptr[i + n] = 0.0f;
      }
    }

    // projection of the meanshifts onto the jacobians (using the weighted Jacobian, see Baltrusaitis 2013)
    Mat J_w_t_m = J_w_t * mean_shifts;

    // Add the regularisation term
    if (!rigid) {
      //J_w_t_m(Rect(0, 6, 1, m)) = J_w_t_m(Rect(0, 6, 1, m)) - regTerm(Rect(6, 6, m, m)) * current_local;
		copyTo(mi(J_w_t_m, cRect(0, 6, 1, m)) - mi(regTerm, cRect(6, 6, m, m)) * current_local, J_w_t_m, 0, 6, 1, m);
    }

    // Calculating the Hessian approximation
    Mat Hessian = J_w_t * J;

    // Add the Tikhonov regularisation
    Hessian = Hessian + regTerm;

    // Solve for the parameter update (from Baltrusaitis 2013 based on eq (36) Saragih 2011)
    Mat param_update(J_w_t_m.rows, J_w_t_m.cols, CV_32F);
	tic;
    solve(&Hessian, &J_w_t_m, &param_update, CC_CHOLESKY);
	toc;
	CHECK_OR_PUT(Hessian);
	CHECK_OR_PUT(J_w_t_m);
	CHECK_OR_PUT(param_update);

    // update the reference
    pdm.UpdateModelParameters(param_update, current_local, current_global);

    // clamp to the local parameters for valid expressions
    pdm.Clamp(current_local, current_global, parameters);
	CHECK_OR_PUT(current_global);
	CHECK_OR_PUT(current_local);
  }

  // compute the log likelihood
  double loglhood = 0;

  landmark_lhoods = Mat(n, 1, CV_64F, -1e8);

  int ii, jj;
  std::vector<double> vv;
  vv.resize(resp_size);
  float* dxs_ptr = img_ptr(float, &dxs);
  float* dys_ptr = img_ptr(float, &dys);
  double* landmark_lhoods_ptr = img_ptr(double, &landmark_lhoods);
  for (int i = 0; i < n; i++) {
    int* ptr = img_ptr(int, &patch_experts.visibilities[scale][view_id]);
    if (ptr[i] == 0) {
      continue;
    }

    float dx = dxs_ptr[i];
    float dy = dys_ptr[i];

    float v, vx, vy, sum = 0.0;

    // Iterate over the patch responses here
    const float* p = img_ptr(float, &patch_expert_responses[i]);

#if 1
	Mat vv_vv;
	double* vv_ = &vv[0];
	for (ii = 0; ii < resp_size; ii++) {
		vv_[ii] = -0.5 * (dy - ii) * (dy - ii)/ parameters.sigma * parameters.sigma;
	}
	for (ii = 0; ii < resp_size; ii++) {
		for (jj = 0; jj < resp_size; jj++) {
			v = *p++;
			v *= exp(vv_[ii] + vv_[jj]);
			sum += v;
		}
	}
#else
    for (ii = 0; ii < resp_size; ii++) {
      vx = (dy - ii) * (dy - ii);

      for (jj = 0; jj < resp_size; jj++) {
        vy = (dx - jj) * (dx - jj);

        // the probability at the current, xi, yi
        v = *p++;

        // the KDE evaluation of that point
        v *= exp(-0.5 * (vx + vy) / (parameters.sigma * parameters.sigma));

        sum += v;
      }
    }
#endif

    landmark_lhoods_ptr[i] = (double)sum;

    // the offset is there for numerical stability
    loglhood += log(sum + 1e-8);
  }

  loglhood = loglhood / sum(patch_experts.visibilities[scale][view_id])[0];

  final_global = current_global;
  final_local = current_local;

  CHECK_OR_PUT(final_global);
  CHECK_OR_PUT(final_local);
  return loglhood;
}


bool CLM::RemoveBackground(Mat& out_depth_image, const Mat& depth_image)
{
  // use the current estimate of the face location to determine what is foreground and background
  double tx = this->params_global[4];
  double ty = this->params_global[5];

  // if we are too close to the edge fail
  if (tx - 9 <= 0 || ty - 9 <= 0 || tx + 9 >= depth_image.cols || ty + 9 >= depth_image.rows) {
    printf("Face estimate is too close to the edge, tracking failed\n");
    return false;
  }

  Mat current_shape;

  pdm.CalcShape2D(current_shape, params_local, params_global);

  double min_x, max_x, min_y, max_y;

  int n = this->pdm.NumberOfPoints();

  //minMaxLoc(current_shape(Range(0, n), Range(0, 1)), &min_x, &max_x);
  minMaxLoc(mi(current_shape, cRect(0, 0, 1, n)), &min_x, &max_x);
  //minMaxLoc(current_shape(Range(n, n * 2), Range(0, 1)), &min_y, &max_y);
  minMaxLoc(mi(current_shape, cRect(0, n, 1, n)), &min_y, &max_y);

  // the area of interest: size of face with some scaling ( these scalings are fairly ad-hoc)
  double width = 3 * (max_x - min_x);
  double height = 2.5 * (max_y - min_y);

  // getting the region of interest from the depth image,
  // so we don't get other objects lying at same depth as head in the image but away from it
  CRect roi = cRect((int)(tx - width / 2), (int)(ty - height / 2), (int)width, (int)height);

  // clamp it if it does not lie fully in the image
  if (roi.x < 0) {
    roi.x = 0;
  }

  if (roi.y < 0) {
    roi.y = 0;
  }

  if (roi.width + roi.x >= depth_image.cols) {
    roi.x = depth_image.cols - roi.width;
  }

  if (roi.height + roi.y >= depth_image.rows) {
    roi.y = depth_image.rows - roi.height;
  }

  if (width > depth_image.cols) {
    roi.x = 0;
    roi.width = depth_image.cols;
  }

  if (height > depth_image.rows) {
    roi.y = 0;
    roi.height = depth_image.rows;
  }

  if (roi.width == 0) {
    roi.width = depth_image.cols;
  }

  if (roi.height == 0) {
    roi.height = depth_image.rows;
  }

  if (roi.x >= depth_image.cols) {
    roi.x = 0;
  }

  if (roi.y >= depth_image.rows) {
    roi.y = 0;
  }

  // Initialise the mask
  Mat mask(depth_image.rows, depth_image.cols, (uchar)0);

  Mat valid_pixels = depth_image > 0;

  // check if there is any depth near the estimate
  if (sum(mi(valid_pixels, cRect((int)tx - 8, (int)ty - 8, 16, 16)) / 255)[0] > 0) {
    double Z = mean(mi(depth_image, cRect((int)tx - 8, (int)ty - 8, 16, 16)), mi(valid_pixels, cRect((int)tx - 8, (int)ty - 8, 16, 16)))[0]; // Z offset from the surface of the face

    // Only operate within region of interest of the depth image
    Mat dRoi = mi(depth_image, roi);

    Mat mRoi = mi(mask, roi);

    // Filter all pixels further than 20cm away from the current pose depth estimate
    inRange(dRoi, Z - 200, Z + 200, mRoi);

    // Convert to be either 0 or 1
    mask = mask / 255;

    Mat maskF;
    mask.convertTo(maskF, CV_32F);

    //Filter the depth image
    out_depth_image = depth_image.mul(maskF);
  }
  else {
    printf("No depth signal found in foreground, tracking failed\n");
    return false;
  }

  return true;
}

// Getting a 3D shape model from the current detected landmarks (in camera space)
Mat CLM::GetShape(double fx, double fy, double cx, double cy) const
{
  int n = this->detected_landmarks.rows / 2;

  Mat shape3d(n * 3, 1, CV_64F);

  this->pdm.CalcShape3D(shape3d, this->params_local);

  // Need to rotate the shape to get the actual 3D representation

  // get the rotation matrix from the euler angles
  Matx33d R = Euler2RotationMatrix(Vec3d(params_global[1], params_global[2], params_global[3]));

  shape3d = shape3d.reshape(1, 3);

  shape3d = shape3d.t() * Mat(R).t();

  // from the weak perspective model can determine the average depth of the object
  double Zavg = fx / params_global[0];

  Mat outShape(n, 3, CV_64F, 0.0);

  // this is described in the paper in section 3.4 (equation 10) (of the CLM-Z paper)
  double* shape3d_ptr = img_ptr(double, &shape3d);
  double* detected_landmarks_ptr = img_ptr(double, &this->detected_landmarks);
  double* outShape_ptr = img_ptr(double, &outShape);
  for (int i = 0; i < n; i++) {
    double Z = Zavg + shape3d_ptr[i*3+2];

    double X = Z * ((detected_landmarks_ptr[i] - cx) / fx);
    double Y = Z * ((detected_landmarks_ptr[i + n] - cy) / fy);

    outShape_ptr[i*3+0] = (double)X;
    outShape_ptr[i*3+1] = (double)Y;
    outShape_ptr[i*3+2] = (double)Z;

  }

  // The format is 3 rows - n cols
  return outShape.t();

}

// A utility bounding box function
CRectd CLM::GetBoundingBox() const
{
  Mat xs = mi(this->detected_landmarks, cRect(0, 0, 1, this->detected_landmarks.rows / 2));
  Mat ys = mi(this->detected_landmarks, cRect(0, this->detected_landmarks.rows / 2, 1, this->detected_landmarks.rows / 2));

  double min_x, max_x;
  double min_y, max_y;
  minMaxLoc(xs, &min_x, &max_x);
  minMaxLoc(ys, &min_y, &max_y);

  // See if the detections intersect
  CRectd model_rect = cRectd(min_x, min_y, max_x - min_x, max_y - min_y);
  return model_rect;
}

// Legacy function not used at the moment
void CLM::NonVectorisedMeanShift(Mat& out_mean_shifts, const vector<Mat >& patch_expert_responses, const Mat& dxs, const Mat& dys, int resp_size, double a, int scale, int view_id)
{

  int n = dxs.rows;

  int* ptr = img_ptr(int, &patch_experts.visibilities[scale][view_id]);
  double* out_mean_shifts_ptr = img_ptr(double, &out_mean_shifts);
  double* dxs_ptr = img_ptr(double, &dxs);
  double* dys_ptr = img_ptr(double, &dys);
  for (int i = 0; i < n; i++) {
    if (ptr[i] == 0  || sum(patch_expert_responses[i])[0] == 0) {
      out_mean_shifts_ptr[i] = 0;
      out_mean_shifts_ptr[i + n] = 0;
      continue;
    }

    // indices of dx, dy
    double dx = dxs_ptr[i];
    double dy = dys_ptr[i];

    int ii, jj;
    double v, vx, vy, mx = 0.0, my = 0.0, sum = 0.0;

    // Iterate over the patch responses here
    const float* p = img_ptr(float, &patch_expert_responses[i]);

    for (ii = 0; ii < resp_size; ii++) {
      vx = (dy - ii) * (dy - ii);

      for (jj = 0; jj < resp_size; jj++) {
        vy = (dx - jj) * (dx - jj);

        // the probability at the current, xi, yi
        v = *p++;

        // the KDE evaluation of that point
        double kd = exp(a * (vx + vy));
        v *= kd;

        sum += v;

        // mean shift in x and y
        mx += v * jj;
        my += v * ii;

      }
    }

    // setting the actual mean shift update
    double msx = (mx / sum - dx);
    double msy = (my / sum - dy);

    out_mean_shifts_ptr[i] = msx;
    out_mean_shifts_ptr[i + n] = msy;

  }
}

void NonOverlapingDetections(const vector<CLM>& clm_models, vector<CRectd >& face_detections)
{
	// Go over the model and eliminate detections that are not informative (there already is a tracker there)
	for (size_t model = 0; model < clm_models.size(); ++model) {
		// See if the detections intersect
		CRectd model_rect = clm_models[model].GetBoundingBox();

		for (int detection = face_detections.size() - 1; detection >= 0; --detection) {
			//double intersection_area = (model_rect & face_detections[detection]).area();
			double intersection_area = cRectAread(cRectAndd(model_rect, face_detections[detection]));
			double union_area = cRectAread(model_rect) + cRectAread(face_detections[detection]) - 2 * intersection_area;

			// If the model is already tracking what we're detecting ignore the detection, this is determined by amount of overlap
			if (intersection_area / union_area > 0.5) {
				face_detections.erase(face_detections.begin() + detection);
			}
		}
	}
}
