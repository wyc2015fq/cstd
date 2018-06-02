

// MultiTrackCLM.cpp : Defines the entry point for the multiple face tracking console application.



#define INFO_STREAM( stream ) \
  std::cout << stream << std::endl

#define WARN_STREAM( stream ) \
  std::cout << "Warning: " << stream << std::endl

#define ERROR_STREAM( stream ) \
  std::cout << "Error: " << stream << std::endl

static void printErrorAndAbort(const std::string& error)
{
  std::cout << error << std::endl;
  abort();
}

#define FATAL_STREAM( stream ) \
  printErrorAndAbort( std::string( "Fatal error: " ) + stream )

using namespace std;
using namespace cv;

vector<string> get_arguments(int argc, char** argv)
{

  vector<string> arguments;

  for (int i = 0; i < argc; ++i) {
    arguments.push_back(string(argv[i]));
  }

  return arguments;
}

int MultiTrackCLM(int argc, char** argv)
{
#ifdef _WIN32
  _chdir("D:/code/CLM-framework-master/Release");
#endif
  Rect_<double> a, b, c;
  c = a & b;
  vector<string> arguments = get_arguments(argc, argv);

  // Some initial parameters that can be overriden from command line
  vector<string> files, depth_directories, pose_output_files, tracked_videos_output, landmark_output_files, landmark_3D_output_files;

  // By default try webcam 0
  int device = 0;

  // cx and cy aren't necessarilly in the image center, so need to be able to override it (start with unit vals and init them if none specified)
  float fx = 600, fy = 600, cx = 0, cy = 0;

  CLMParameters clm_params(arguments);
  clm_params.use_face_template = true;
  // This is so that the model would not try re-initialising itself
  clm_params.reinit_video_every = -1;

  //clm_params.curr_face_detector = CLMParameters::HOG_SVM_DETECTOR;

  vector<CLMParameters> clm_parameters;
  clm_parameters.push_back(clm_params);

  // Get the input output file parameters
  bool use_world_coords;
  get_video_input_output_params(files, depth_directories, pose_output_files, tracked_videos_output, landmark_output_files, landmark_3D_output_files, use_world_coords, arguments);
  // Get camera parameters
  get_camera_params(device, fx, fy, cx, cy, arguments);

  // The modules that are being used for tracking
  vector<CLM> clm_models;
  vector<bool> active_models;

  int num_faces_max = 4;

  CLM clm_model(clm_parameters[0].model_location);
  //clm_model.face_detector_HAAR.load(clm_parameters[0].face_detector_location);
  //clm_model.face_detector_location = clm_parameters[0].face_detector_location;

  clm_models.reserve(num_faces_max);

  clm_models.push_back(clm_model);
  active_models.push_back(false);

  for (int i = 1; i < num_faces_max; ++i) {
    clm_models.push_back(clm_model);
    active_models.push_back(false);
    clm_parameters.push_back(clm_params);
  }

  // If multiple video files are tracked, use this to indicate if we are done
  bool done = false;
  int f_n = -1;

  // If cx (optical axis centre) is undefined will use the image size/2 as an estimate
  bool cx_undefined = false;

  if (cx == 0 || cy == 0) {
    cx_undefined = true;
  }

  while (!done) { // this is not a for loop as we might also be reading from a webcam
    string current_file;
    // We might specify multiple video files as arguments
    if (files.size() > 0) {
      f_n++;
      current_file = files[f_n];
    }

    bool use_depth = !depth_directories.empty();

    // Do some grabbing
    VideoCapture video_capture;

    if (0 && current_file.size() > 0) {
      INFO_STREAM("Attempting to read from file: " << current_file);
      video_capture = VideoCapture(current_file);
    }
    else {
      INFO_STREAM("Attempting to capture from device: " << device);
      video_capture = VideoCapture(device);

      // Read a first frame often empty in camera
      Mat captured_image;
      //video_capture.set(CAP_PROP_FRAME_WIDTH, 1280);
      //video_capture.set(CAP_PROP_FRAME_HEIGHT, 720);
      video_capture >> captured_image;
    }

    if (!video_capture.isOpened()) {
      FATAL_STREAM("Failed to open video source");
    }
    else {
      INFO_STREAM("Device or file opened");
    }

    Mat captured_image;
    video_capture >> captured_image;

    // If optical centers are not defined just use center of image
    if (cx_undefined) {
      cx = captured_image.cols / 2.0f;
      cy = captured_image.rows / 2.0f;
    }

    // Creating output files
    std::ofstream pose_output_file;

    if (!pose_output_files.empty()) {
      pose_output_file.open(pose_output_files[f_n]);
    }

    std::ofstream landmarks_output_file;

    if (!landmark_output_files.empty()) {
      landmarks_output_file.open(landmark_output_files[f_n]);
    }

    int frame_count = 0;

    // saving the videos
    VideoWriter writerFace;

    if (!tracked_videos_output.empty()) {
      writerFace = VideoWriter(tracked_videos_output[f_n], CV_FOURCC('D', 'I', 'V', 'X'), 30, captured_image.size(), true);
    }

    // For measuring the timings
    int64 t1, t0 = getTickCount();
    double fps = 10;

    INFO_STREAM("Starting tracking");

    while (!captured_image.empty()) {
      // Reading the images
      Mat_<float> depth_image;
      Mat_<uchar> grayscale_image;

      Mat disp_image = captured_image.clone();

      if (captured_image.channels() == 3) {
        cvtColor(captured_image, grayscale_image, CV_BGR2GRAY);
      }
      else {
        grayscale_image = captured_image.clone();
      }

      // Get depth image
      if (use_depth) {
        char* dst = new char[100];
        std::stringstream sstream;

        sstream << depth_directories[f_n] << "\\depth%05d.png";
        sprintf(dst, sstream.str().c_str(), frame_count + 1);
        // Reading in 16-bit png image representing depth
        Mat_<short> depth_image_16_bit = imread(string(dst), -1);

        // Convert to a floating point depth image
        if (!depth_image_16_bit.empty()) {
          depth_image_16_bit.convertTo(depth_image, CV_32F);
        }
        else {
          WARN_STREAM("Can't find depth image");
        }
      }

      vector<Rect_<double> > face_detections;

      bool all_models_active = true;

      for (unsigned int model = 0; model < clm_models.size(); ++model) {
        if (!active_models[model]) {
          all_models_active = false;
        }
      }

	  img_t grayscale_image_ = mi(grayscale_image);
      // Get the detections (every 8th frame and when there are free models available for tracking)
      if (frame_count % 8 == 0 && !all_models_active) {
#ifdef _USE_DLIB

        if (clm_parameters[0].curr_face_detector == CLMParameters::HOG_SVM_DETECTOR) {
          vector<double> confidences;
          DetectFacesHOG(face_detections, grayscale_image, clm_models[0].face_detector_HOG, confidences);
        }
        else
#endif
#ifdef _USE_CVFACE
        {
          DetectFaces(face_detections, &grayscale_image_);
        }
#endif

      }

      // Keep only non overlapping detections (also convert to a concurrent vector
      NonOverlapingDetections(clm_models, face_detections);

      //vector<bool > face_detections_used(face_detections.size());

      // Go through every model and update the tracking TODO pull out as a separate parallel/non-parallel method
      //tbb::parallel_for(0, (int)clm_models.size(), [&](int model)
      myparallel_for(0, (int)clm_models.size(), model) {
        //for(unsigned int model = 0; model < clm_models.size(); ++model)
        //{

        bool detection_success = false;

        // If the current model has failed more than 4 times in a row, remove it
        if (clm_models[model].failures_in_a_row > 4) {
          active_models[model] = false;
          clm_models[model].Reset();
        }

		img_t depth_image_ = mi(depth_image);
        // If the model is inactive reactivate it with new detections
        if (!active_models[model]) {

          for (size_t detection_ind = 0; detection_ind < face_detections.size(); ++detection_ind) {
            // if it was not taken by another tracker take it (if it is false swap it to true and enter detection, this makes it parallel safe)
            //if(face_detections_used[detection_ind].compare_and_swap(true, false) == false)
            {
              // Reinitialise the model
              clm_models[model].Reset();

              // This ensures that a wider window is used for the initial landmark localisation
              clm_models[model].detection_success = false;
              detection_success = DetectLandmarksInVideo(&grayscale_image_, &depth_image_, face_detections[detection_ind], clm_models[model], clm_parameters[model]);

              // This activates the model
              active_models[model] = true;

              // break out of the loop as the tracker has been reinitialised
              //break;
            }

          }
        }
        else {
          // The actual facial landmark detection / tracking
          detection_success = DetectLandmarksInVideo(&grayscale_image_, &depth_image_, clm_models[model], clm_parameters[model]);
        }
      }

      // Go through every model and visualise the results
      for (size_t model = 0; model < clm_models.size(); ++model) {
        // Visualising the results
        // Drawing the facial landmarks on the face and the bounding box around it if tracking is successful and initialised
        double detection_certainty = clm_models[model].detection_certainty;

        double visualisation_boundary = -0.1;

        // Only draw if the reliability is reasonable, the value is slightly ad-hoc
        if (detection_certainty < visualisation_boundary) {
          Draw(disp_image, clm_models[model], 1);

          if (detection_certainty > 1) {
            detection_certainty = 1;
          }

          if (detection_certainty < -1) {
            detection_certainty = -1;
          }

          detection_certainty = (detection_certainty + 1) / (visualisation_boundary + 1);
          
          // A rough heuristic for box around the face width
          int thickness = (int)std::ceil(2.0 * ((double)captured_image.cols) / 640.0);
          
#ifdef _GET_POSE
          if (1) {
            // Work out the pose of the head from the tracked model
            Vec6d pose_estimate_CLM = GetCorrectedPoseWorld(clm_models[model], fx, fy, cx, cy);
            
            // Draw it in reddish if uncertain, blueish if certain
            DrawBox(disp_image, pose_estimate_CLM, Scalar((1 - detection_certainty) * 255.0, 0, detection_certainty * 255), thickness, fx, fy, cx, cy);
          }
#endif
        }
      }

      // Work out the framerate
      if (frame_count % 10 == 0) {
        t1 = getTickCount();
        fps = 10.0 / (double(t1 - t0) / getTickFrequency());
        t0 = t1;
      }

      // Write out the framerate on the image before displaying it
      char fpsC[255];
      sprintf(fpsC, "%d", (int)fps);
      string fpsSt("FPS:");
      fpsSt += fpsC;
      putText(disp_image, fpsSt, Point(10, 20), CV_FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(255, 0, 0));

      int num_active_models = 0;

      for (size_t active_model = 0; active_model < active_models.size(); active_model++) {
        if (active_models[active_model]) {
          num_active_models++;
        }
      }

      char active_m_C[255];
      sprintf(active_m_C, "%d", num_active_models);
      string active_models_st("Active models:");
      active_models_st += active_m_C;
      putText(disp_image, active_models_st, Point(10, 60), CV_FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(255, 0, 0));

      if (!clm_parameters[0].quiet_mode) {
        namedWindow("tracking_result", 0);
        imshow("tracking_result", disp_image);

        if (!depth_image.empty()) {
          // Division needed for visualisation purposes
          imshow("depth", depth_image / 2000.0);
        }
      }

      // output the tracked video
      if (!tracked_videos_output.empty()) {
        writerFace << disp_image;
      }

      video_capture >> captured_image;

      // detect key presses
      char character_press = waitKey(1);

      // restart the trackers
      if (character_press == 'r') {
        for (size_t i = 0; i < clm_models.size(); ++i) {
          clm_models[i].Reset();
          active_models[i] = false;
        }
      }
      // quit the application
      else if (character_press == 'q') {
        return(0);
      }

      // Update the frame count
      frame_count++;
    }

    frame_count = 0;

    // Reset the model, for the next video
    for (size_t model = 0; model < clm_models.size(); ++model) {
      clm_models[model].Reset();
      active_models[model] = false;
    }

    pose_output_file.close();
    landmarks_output_file.close();

    // break out of the loop if done with all the files
    if (f_n == files.size() - 1) {
      done = true;
    }
  }

  return 0;
}

