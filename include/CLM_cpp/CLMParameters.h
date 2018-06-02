

//  Parameters of the CLNF, CLM-Z and CLM trackers
//
//  Tadas Baltrusaitis
//  01/05/2012
#ifndef __CLM_PARAM_H
#define __CLM_PARAM_H

struct CLMParameters {

  // A number of RLMS or NU-RLMS iterations
  int num_optimisation_iteration;

  // Should pose be limited to 180 degrees frontal
  bool limit_pose;

  // Should face validation be done
  bool validate_detections;

  // Landmark detection validator boundary for correct detection, the regressor output -1 (perfect alignment) 1 (bad alignment),
  double validation_boundary;

  // Used when tracking is going well
  vector<int> window_sizes_small;

  // Used when initialising or tracking fails
  vector<int> window_sizes_init;

  // Used for the current frame
  vector<int> window_sizes_current;

  // How big is the tracking template that helps with large motions
  double face_template_scale;
  bool use_face_template;

  // Where to load the model from
  char model_location[256];

  // this is used for the smooting of response maps (KDE sigma)
  double sigma;

  double reg_factor;  // weight put to regularisation
  double weight_factor; // factor for weighted least squares

  // should multiple views be considered during reinit
  bool multi_view;

  // How often should face detection be used to attempt reinitialisation, every n frames (set to negative not to reinit)
  int reinit_video_every;

  // Determining which face detector to use for (re)initialisation, HAAR is quicker but provides more false positives and is not goot for in-the-wild conditions
  // Also HAAR detector can detect smaller faces while HOG SVM is only capable of detecting faces at least 70px across
  //enum FaceDetector {HAAR_DETECTOR, HOG_SVM_DETECTOR};

  //FaceDetector curr_face_detector;
  //string face_detector_location1;
  //string face_detector_location;

  // Should the results be visualised and reported to console
  bool quiet_mode;

  // Should the model be refined hierarchically (if available)
  bool refine_hierarchical;

  // Should the parameters be refined for different scales
  bool refine_parameters;

  // Using the brand new and experimental gaze tracker
  bool track_gaze;

  CLMParameters() {
    // initialise the default values
    init();
  }

  // possible parameters are -clm 'file' which specifies the default location of main clm root
  CLMParameters(char** argv, int argc) {
    // initialise the default values
    init();
    char buf[256];
    // First element is reserved for the executable location (useful for finding relative model locs)
    char* root = parent_path(argv[0], buf, 256);

    int i;
    for (i = 1; i < argc; ++i) {
      if (!argv[i]) continue;

      if (0==strcmp(argv[i],"-mloc")) {
        char* model_loc = argv[i + 1];
        strncpy(model_location, model_loc, 256);
        argv[i] = NULL;
        argv[i + 1] = NULL;
        i++;

      }

      if (0==strcmp(argv[i],"-clm_sigma")) {
        sigma = atof(argv[i + 1]);
        argv[i] = NULL;
        argv[i + 1] = NULL;
        i++;
      }
      else if (0==strcmp(argv[i],"-w_reg")) {
        weight_factor = atof(argv[i + 1]);
        argv[i] = NULL;
        argv[i + 1] = NULL;
        i++;
      }
      else if (0==strcmp(argv[i],"-reg")) {
        reg_factor = atof(argv[i + 1]);
        argv[i] = NULL;
        argv[i + 1] = NULL;
        i++;
      }
      else if (0==strcmp(argv[i],"-multi_view")) {
        int m_view;
        m_view = atoi(argv[i + 1]);
        multi_view = (bool)(m_view != 0);
        argv[i] = NULL;
        argv[i + 1] = NULL;
        i++;
      }
      else if (0==strcmp(argv[i],"-validate_detections")) {
        int v_det;
        v_det = atoi(argv[i + 1]);
        validate_detections = (bool)(v_det != 0);
        argv[i] = NULL;
        argv[i + 1] = NULL;
        i++;
      }
      else if (0==strcmp(argv[i],"-n_iter")) {
        num_optimisation_iteration = atoi(argv[i + 1]);
        argv[i] = NULL;
        argv[i + 1] = NULL;
        i++;
      }
      else if (0==strcmp(argv[i],"-gaze")) {
        track_gaze = true;

        argv[i] = NULL;
        i++;
      }
      else if (0==strcmp(argv[i],"-q")) {

        quiet_mode = true;

        argv[i] = NULL;
      }
      else if (0==strcmp(argv[i],"-clmwild")) {
        // For in the wild fitting these parameters are suitable
        window_sizes_init = vector<int>(4);
        window_sizes_init[0] = 15;
        window_sizes_init[1] = 13;
        window_sizes_init[2] = 11;
        window_sizes_init[3] = 9;

        sigma = 1.25;
        reg_factor = 35;
        weight_factor = 2.5;
        num_optimisation_iteration = 10;

        argv[i] = NULL;

        // For in-the-wild images use an in-the wild detector
        //curr_face_detector = HOG_SVM_DETECTOR;

      }
      else if (0==strcmp(argv[i],"-help")) {
        printf("CLM parameters are defined as follows: -mloc <location of model file> -pdm_loc <override pdm location> -w_reg <weight term for patch rel.> -reg <prior regularisation> -clm_sigma <float sigma term> -fcheck <should face checking be done 0/1> -n_iter <num EM iterations> -clwild (for in the wild images) -q (quiet mode)\n"); // Inform the user of how to use the program
      }
    }

    // Make sure model_location is argv
    if (!path_exists(model_location)) {
      _snprintf(model_location, 256, "%s/%s", root, model_location);

      if (!path_exists(model_location)) {
        printf("Could not find the landmark detection model to load\n");
      }
    }

  }

public:
  void init() {

    // number of iterations that will be performed at each clm scale
    num_optimisation_iteration = 5;

    // using an external face checker based on SVM
    validate_detections = true;

    // Using hierarchical refinement by default (can be turned off)
    refine_hierarchical = true;

    // Refining parameters by default
    refine_parameters = true;

    window_sizes_small = vector<int>(4);
    window_sizes_init = vector<int>(4);

    // For fast tracking
    window_sizes_small[0] = 0;
    window_sizes_small[1] = 9;
    window_sizes_small[2] = 7;
    window_sizes_small[3] = 5;

    // Just for initialisation
    window_sizes_init[0] = 11;
    window_sizes_init[1] = 9;
    window_sizes_init[2] = 7;
    window_sizes_init[3] = 5;

    face_template_scale = 0.3;
    // Off by default (as it might lead to some slight inaccuracies in slowly moving faces)
    use_face_template = false;

    // For first frame use the initialisation
    window_sizes_current = window_sizes_init;

    strncpy(model_location, "model/main_ccnf_general.txt", 256);

    sigma = 1.5;
    reg_factor = 25;
    weight_factor = 0; // By default do not use NU-RLMS for videos as it does not work as well for them

    validation_boundary = -0.45;

    limit_pose = true;
    multi_view = false;

    reinit_video_every = 4;

    quiet_mode = false;

    // By default use HOG SVM
    //curr_face_detector = HOG_SVM_DETECTOR;

    // The gaze tracking has to be explicitly initialised
    track_gaze = false;
  }
};



#endif // __CLM_PARAM_H
