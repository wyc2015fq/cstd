

// Getting a head pose estimate from the currently detected landmarks (rotation with respect to point camera)
// The format returned is [Tx, Ty, Tz, Eul_x, Eul_y, Eul_z]
Vec6d GetPoseCamera(const CLM& clm_model, double fx, double fy, double cx, double cy)
{
  if (!clm_model.detected_landmarks.empty() && clm_model.params_global[0] != 0) {
    double Z = fx / clm_model.params_global[0];

    double X = ((clm_model.params_global[4] - cx) * (1.0 / fx)) * Z;
    double Y = ((clm_model.params_global[5] - cy) * (1.0 / fy)) * Z;

    return Vec6d(X, Y, Z, clm_model.params_global[1], clm_model.params_global[2], clm_model.params_global[3]);
  }
  else {
    return Vec6d(0, 0, 0, 0, 0, 0);
  }
}

// Getting a head pose estimate from the currently detected landmarks (rotation in world coordinates)
// The format returned is [Tx, Ty, Tz, Eul_x, Eul_y, Eul_z]
Vec6d GetPoseWorld(const CLM& clm_model, double fx, double fy, double cx, double cy)
{
  if (!clm_model.detected_landmarks.empty() && clm_model.params_global[0] != 0) {
    double Z = fx / clm_model.params_global[0];

    double X = ((clm_model.params_global[4] - cx) * (1.0 / fx)) * Z;
    double Y = ((clm_model.params_global[5] - cy) * (1.0 / fy)) * Z;

    // Here we correct for the camera orientation, for this need to determine the angle the camera makes with the head pose
    double z_x = sqrt(X * X + Z * Z);
    double eul_x = atan2(Y, z_x);

    double z_y = sqrt(Y * Y + Z * Z);
    double eul_y = -atan2(X, z_y);

    Matx33d camera_rotation = Euler2RotationMatrix(Vec3d(eul_x, eul_y, 0));
    Matx33d head_rotation = AxisAngle2RotationMatrix(Vec3d(clm_model.params_global[1], clm_model.params_global[2], clm_model.params_global[3]));

	Matx33d corrected_rotation;
	Mat(camera_rotation.t() * head_rotation).copyTo(corrected_rotation);

    Vec3d euler_corrected = RotationMatrix2Euler(corrected_rotation);

    return Vec6d(X, Y, Z, euler_corrected[0], euler_corrected[1], euler_corrected[2]);
  }
  else {
    return Vec6d(0, 0, 0, 0, 0, 0);
  }
}

#ifdef _GET_POSE
// Getting a head pose estimate from the currently detected landmarks, with appropriate correction due to orthographic camera issue
// This is because rotation estimate under orthographic assumption is only correct close to the centre of the image
// This method returns a corrected pose estimate with respect to world coordinates (Experimental)
// The format returned is [Tx, Ty, Tz, Eul_x, Eul_y, Eul_z]
Vec6d GetCorrectedPoseWorld(const CLM& clm_model, double fx, double fy, double cx, double cy)
{
  if (!clm_model.detected_landmarks.empty() && clm_model.params_global[0] != 0) {
    // This is used as an initial estimate for the iterative PnP algorithm
    double Z = fx / clm_model.params_global[0];

    double X = ((clm_model.params_global[4] - cx) * (1.0 / fx)) * Z;
    double Y = ((clm_model.params_global[5] - cy) * (1.0 / fy)) * Z;

    // Correction for orientation

    // 2D points
    Mat landmarks_2D = clm_model.detected_landmarks;

    landmarks_2D = landmarks_2D.reshape(1, 2).t();

    // 3D points
    Mat landmarks_3D;
    clm_model.pdm.CalcShape3D(landmarks_3D, clm_model.params_local);

    landmarks_3D = landmarks_3D.reshape(1, 3).t();

    // Solving the PNP model

    // The camera matrix
    Matx33d camera_matrix(fx, 0, cx, 0, fy, cy, 0, 0, 1);

    Vec3d vec_trans(X, Y, Z);
    Vec3d vec_rot(clm_model.params_global[1], clm_model.params_global[2], clm_model.params_global[3]);
#if 1
    solvePnP(landmarks_3D, landmarks_2D, camera_matrix, Mat(), Mat(vec_rot), Mat(vec_trans), true);
#else
    icvSolvePnP(landmarks_3D.data, landmarks_2D.data, camera_matrix.val, NULL, vec_rot.val, vec_trans.val, true, 0, landmarks_3D.rows, (TypeId)landmarks_3D.depth());
#endif

    Vec3d euler = AxisAngle2Euler(vec_rot);

    return Vec6d(vec_trans[0], vec_trans[1], vec_trans[2], vec_rot[0], vec_rot[1], vec_rot[2]);
  }
  else {
    return Vec6d(0, 0, 0, 0, 0, 0);
  }
}

// Getting a head pose estimate from the currently detected landmarks, with appropriate correction due to perspective projection
// This method returns a corrected pose estimate with respect to a point camera (NOTE not the world coordinates) (Experimental)
// The format returned is [Tx, Ty, Tz, Eul_x, Eul_y, Eul_z]
Vec6d GetCorrectedPoseCamera(const CLM& clm_model, double fx, double fy, double cx, double cy)
{
  if (!clm_model.detected_landmarks.empty() && clm_model.params_global[0] != 0) {

    double Z = fx / clm_model.params_global[0];

    double X = ((clm_model.params_global[4] - cx) * (1.0 / fx)) * Z;
    double Y = ((clm_model.params_global[5] - cy) * (1.0 / fy)) * Z;

    // Correction for orientation

    // 3D points
    Mat landmarks_3D;
    clm_model.pdm.CalcShape3D(landmarks_3D, clm_model.params_local);

    landmarks_3D = landmarks_3D.reshape(1, 3).t();

    // 2D points
    Mat landmarks_2D = clm_model.detected_landmarks;

    landmarks_2D = landmarks_2D.reshape(1, 2).t();

    // Solving the PNP model

    // The camera matrix
    Matx33d camera_matrix(fx, 0, cx, 0, fy, cy, 0, 0, 1);

    Vec3d vec_trans(X, Y, Z);
    Vec3d vec_rot(clm_model.params_global[1], clm_model.params_global[2], clm_model.params_global[3]);
#if 1
    solvePnP(landmarks_3D, landmarks_2D, camera_matrix, Mat(), Mat(vec_rot), Mat(vec_trans), true);
#else
	solvePnP(landmarks_3D, landmarks_2D, camera_matrix, Mat(), vec_rot, vec_trans, true);
	//icvSolvePnP(landmarks_3D.data, landmarks_2D.data, camera_matrix.val, NULL, vec_rot.val, vec_trans.val, true, 0, landmarks_3D.rows, (TypeId)landmarks_3D.depth());
#endif

    // Here we correct for the camera orientation, for this need to determine the angle the camera makes with the head pose
    double z_x = sqrt(vec_trans[0] * vec_trans[0] + vec_trans[2] * vec_trans[2]);
    double eul_x = atan2(vec_trans[1], z_x);

    double z_y = sqrt(vec_trans[1] * vec_trans[1] + vec_trans[2] * vec_trans[2]);
    double eul_y = -atan2(vec_trans[0], z_y);

    Matx33d camera_rotation = Euler2RotationMatrix(Vec3d(eul_x, eul_y, 0));
    Matx33d head_rotation = AxisAngle2RotationMatrix(vec_rot);

    Matx33d corrected_rotation = camera_rotation * head_rotation;

    Vec3d euler_corrected = RotationMatrix2Euler(corrected_rotation);

    return Vec6d(vec_trans[0], vec_trans[1], vec_trans[2], euler_corrected[0], euler_corrected[1], euler_corrected[2]);
  }
  else {
    return Vec6d(0, 0, 0, 0, 0, 0);
  }
}
#endif

// If landmark detection in video succeeded create a template for use in simple tracking
void UpdateTemplate(const img_t* grayscale_image, CLM& clm_model)
{
  CRect bounding_box;

  clm_model.pdm.CalcBoundingBox(bounding_box, clm_model.params_global, clm_model.params_local);
  // Make sure the box is not out of bounds
  bounding_box = bounding_box & cRect(0, 0, grayscale_image->cols, grayscale_image->rows);

  IM_DEF(tmp, 2);
  //clm_model.face_template = ma(cvGetSubRect(grayscale_image, tmp, bounding_box.x, bounding_box.y, bounding_box.width, bounding_box.height));
  clm_model.face_template = mi(ma(grayscale_image), bounding_box);
}

// This method uses basic template matching in order to allow for better tracking of fast moving faces
void CorrectGlobalParametersVideo(const img_t* grayscale_image, CLM& clm_model, const CLMParameters& params)
{
	//IM_DEF(tmp, 10);
  CRect init_box;
  clm_model.pdm.CalcBoundingBox(init_box, clm_model.params_global, clm_model.params_local);

  CRect roi = cRect(init_box.x - init_box.width / 2, init_box.y - init_box.height / 2, init_box.width * 2, init_box.height * 2);
  roi = roi & cRect(0, 0, grayscale_image->cols, grayscale_image->rows);

  int off_x = roi.x;
  int off_y = roi.y;

  double scaling = params.face_template_scale / clm_model.params_global[0];
  Mat image;

  if (scaling < 1) {
    resize(clm_model.face_template, clm_model.face_template, Size(), scaling, scaling);
    resize(mi(ma(grayscale_image), roi), image, Size(), scaling, scaling);
  }
  else {
    scaling = 1;
    //image = ma(cvGetSubRect(grayscale_image, tmp, roi.x, roi.y, roi.width, roi.height));
	image = mi(ma(grayscale_image), roi);
  }

  // Resizing the template
  Mat corr_out;
  matchTemplate(image, clm_model.face_template, corr_out, CC_TM_CCOEFF_NORMED);

  // Actually matching it
  //double min, max;
  int max_loc[2];

  minMaxIdx(corr_out, NULL, NULL, NULL, max_loc);

  CRectd out_bbox = cRectd(max_loc[1] / scaling + off_x, max_loc[0] / scaling + off_y, clm_model.face_template.rows / scaling, clm_model.face_template.cols / scaling);

  double shift_x = out_bbox.x - (double)init_box.x;
  double shift_y = out_bbox.y - (double)init_box.y;

  clm_model.params_global[4] = clm_model.params_global[4] + shift_x;
  clm_model.params_global[5] = clm_model.params_global[5] + shift_y;

}

bool DetectLandmarksInVideo(const img_t* grayscale_image, const img_t* depth_image, CLM& clm_model, CLMParameters& params)
{
  // First need to decide if the landmarks should be "detected" or "tracked"
  // Detected means running face detection and a larger search area, tracked means initialising from previous step
  // and using a smaller search area

  // Indicating that this is a first detection in video sequence or after restart
  bool initial_detection = !clm_model.tracking_initialised;

  // Only do it if there was a face detection at all
  if (clm_model.tracking_initialised) {

    // The area of interest search size will depend if the previous track was successful
    if (!clm_model.detection_success) {
      params.window_sizes_current = params.window_sizes_init;
    }
    else {
      params.window_sizes_current = params.window_sizes_small;
    }

    // Before the expensive landmark detection step apply a quick template tracking approach
    if (params.use_face_template && !clm_model.face_template.empty() && clm_model.detection_success) {
      CorrectGlobalParametersVideo(grayscale_image, clm_model, params);
    }

    bool track_success = clm_model.DetectLandmarks((grayscale_image), (depth_image), params);

    if (!track_success) {
      // Make a record that tracking failed
      clm_model.failures_in_a_row++;
    }
    else {
      // indicate that tracking is a success
      clm_model.failures_in_a_row = -1;
      UpdateTemplate(grayscale_image, clm_model);
    }
  }

  // This is used for both detection (if it the tracking has not been initialised yet) or if the tracking failed (however we do this every n frames, for speed)
  // This also has the effect of an attempt to reinitialise just after the tracking has failed, which is useful during large motions
  if ((!clm_model.tracking_initialised && (clm_model.failures_in_a_row + 1) % (params.reinit_video_every * 6) == 0)
      || (clm_model.tracking_initialised && !clm_model.detection_success && params.reinit_video_every > 0 && clm_model.failures_in_a_row % params.reinit_video_every == 0)) {
	 // assert(0);
#if 1
    CRectd bounding_box;

    CPoint preference_det = cPoint(-1, -1);

    if (clm_model.preference_det.x != -1 && clm_model.preference_det.y != -1) {
      preference_det.x = clm_model.preference_det.x * grayscale_image->cols;
      preference_det.y = clm_model.preference_det.y * grayscale_image->rows;
      clm_model.preference_det = cPoint2d(-1, -1);
    }

    bool face_detection_success;
	
	face_detection_success = DetectSingleFace(bounding_box, grayscale_image, preference_det);
    

    // Attempt to detect landmarks using the detected face (if unseccessful the detection will be ignored)
    if (face_detection_success) {
      // Indicate that tracking has started as a face was detected
      clm_model.tracking_initialised = true;

      // Keep track of old model values so that they can be restored if redetection fails
      Vec6d params_global_init = clm_model.params_global;
      Mat params_local_init = clm_model.params_local.clone();
      double likelihood_init = clm_model.model_likelihood;
      Mat detected_landmarks_init = clm_model.detected_landmarks.clone();
      Mat landmark_likelihoods_init = clm_model.landmark_likelihoods.clone();

      // Use the detected bounding box and empty local parameters
      clm_model.params_local.setTo(0);
      clm_model.pdm.CalcParams(clm_model.params_global, bounding_box, clm_model.params_local);

      // Make sure the search size is large
      params.window_sizes_current = params.window_sizes_init;

      // Do the actual landmark detection (and keep it only if successful)
      bool landmark_detection_success = clm_model.DetectLandmarks(grayscale_image, depth_image, params);

      // If landmark reinitialisation unsucessful continue from previous estimates
      // if it's initial detection however, do not care if it was successful as the validator might be wrong, so continue trackig
      // regardless
      if (!initial_detection && !landmark_detection_success) {

        // Restore previous estimates
        clm_model.params_global = params_global_init;
        clm_model.params_local = params_local_init.clone();
        clm_model.pdm.CalcShape2D(clm_model.detected_landmarks, clm_model.params_local, clm_model.params_global);
        clm_model.model_likelihood = likelihood_init;
        clm_model.detected_landmarks = detected_landmarks_init.clone();
        clm_model.landmark_likelihoods = landmark_likelihoods_init.clone();

        return false;
      }
      else {
        clm_model.failures_in_a_row = -1;
        UpdateTemplate(grayscale_image, clm_model);
        return true;
      }
    }
#endif
  }

  // if the model has not been initialised yet class it as a failure
  if (!clm_model.tracking_initialised) {
    clm_model.failures_in_a_row++;
  }

  // un-initialise the tracking
  if (clm_model.failures_in_a_row > 100) {
    clm_model.tracking_initialised = false;
  }

  return clm_model.detection_success;
}

bool DetectLandmarksInVideo(const img_t* grayscale_image, const img_t* depth_image, const CRectd bounding_box, CLM& clm_model, CLMParameters& params)
{
  if (bounding_box.width > 0) {
    // calculate the local and global parameters from the generated 2D shape (mapping from the 2D to 3D because camera params are unknown)
    clm_model.params_local.setTo(0);
    clm_model.pdm.CalcParams(clm_model.params_global, bounding_box, clm_model.params_local);

    // indicate that face was detected so initialisation is not necessary
    clm_model.tracking_initialised = true;
  }

  return DetectLandmarksInVideo(grayscale_image, depth_image, clm_model, params);

}

bool DetectLandmarksInVideo(const img_t* grayscale_image, CLM& clm_model, CLMParameters& params)
{
  return DetectLandmarksInVideo(grayscale_image, NULL, clm_model, params);
}

bool DetectLandmarksInVideo(const img_t* grayscale_image, const CRectd bounding_box, CLM& clm_model, CLMParameters& params)
{
  return DetectLandmarksInVideo(grayscale_image, NULL, clm_model, params);
}

//================================================================================================================
// Landmark detection in image, need to provide an image and optionally CLM model together with parameters (default values work well)
// Optionally can provide a bounding box in which detection is performed (this is useful if multiple faces are to be detected in images)
//================================================================================================================

// This is the one where the actual work gets done, other DetectLandmarksInImage calls lead to this one
bool DetectLandmarksInImage(const img_t* grayscale_image, const img_t* depth_image, const CRectd bounding_box, CLM& clm_model, CLMParameters& params)
{
  // Can have multiple hypotheses
  vector<Vec3d> rotation_hypotheses;

  if (params.multi_view) {
    // Try out different orientation initialisations
    // It is possible to add other orientation hypotheses easilly by just pushing to this vector
    rotation_hypotheses.push_back(Vec3d(0, 0, 0));
    rotation_hypotheses.push_back(Vec3d(0, 0.5236, 0));
    rotation_hypotheses.push_back(Vec3d(0, -0.5236, 0));
    rotation_hypotheses.push_back(Vec3d(0.5236, 0, 0));
    rotation_hypotheses.push_back(Vec3d(-0.5236, 0, 0));
  }
  else {
    // Assume the face is close to frontal
    rotation_hypotheses.push_back(Vec3d(0, 0, 0));
  }

  // Use the initialisation size for the landmark detection
  params.window_sizes_current = params.window_sizes_init;

  // Store the current best estimate
  double best_likelihood;
  Vec6d best_global_parameters;
  Mat best_local_parameters;
  Mat best_detected_landmarks;
  Mat best_landmark_likelihoods;
  bool best_success;

  // The hierarchical model parameters
  int n = clm_model.hierarchical_models.size();
  vector<double> best_likelihood_h(n);
  vector<Vec6d> best_global_parameters_h(n);
  vector<Mat> best_local_parameters_h(n);
  vector<Mat> best_detected_landmarks_h(n);
  vector<Mat> best_landmark_likelihoods_h(n);
  int hypothesis, part;
  for (hypothesis = 0; hypothesis < rotation_hypotheses.size(); ++hypothesis) {
    // Reset the potentially set clm_model parameters
    clm_model.params_local.setTo(0.0);
    
    for (part = 0; part < n; ++part) {
      clm_model.hierarchical_models[part].params_local.setTo(0.0);
    }
    
    
    // calculate the local and global parameters from the generated 2D shape (mapping from the 2D to 3D because camera params are unknown)
    clm_model.pdm.CalcParams(clm_model.params_global, bounding_box, clm_model.params_local, rotation_hypotheses[hypothesis]);
    
    bool success;
    
    success  = clm_model.DetectLandmarks((grayscale_image), (depth_image), params);
    
    if (hypothesis == 0 || best_likelihood < clm_model.model_likelihood) {
      best_likelihood = clm_model.model_likelihood;
      best_global_parameters = clm_model.params_global;
      best_local_parameters = clm_model.params_local.clone();
      best_detected_landmarks = clm_model.detected_landmarks.clone();
      best_landmark_likelihoods = clm_model.landmark_likelihoods.clone();
      best_success = success;
    }

    for (part = 0; part < n; ++part) {
      if (hypothesis == 0 || best_likelihood < clm_model.hierarchical_models[part].model_likelihood) {
        best_likelihood_h[part] = clm_model.hierarchical_models[part].model_likelihood;
        best_global_parameters_h[part] = clm_model.hierarchical_models[part].params_global;
        best_local_parameters_h[part] = clm_model.hierarchical_models[part].params_local.clone();
        best_detected_landmarks_h[part] = clm_model.hierarchical_models[part].detected_landmarks.clone();
        best_landmark_likelihoods_h[part] = clm_model.hierarchical_models[part].landmark_likelihoods.clone();
      }
    }
  }

  // Store the best estimates in the clm_model
  clm_model.model_likelihood = best_likelihood;
  clm_model.params_global = best_global_parameters;
  clm_model.params_local = best_local_parameters.clone();
  clm_model.detected_landmarks = best_detected_landmarks.clone();
  clm_model.detection_success = best_success;
  clm_model.landmark_likelihoods = best_landmark_likelihoods.clone();

  for (part = 0; part < n; ++part) {
    clm_model.hierarchical_models[part].params_global = best_global_parameters_h[part];
    clm_model.hierarchical_models[part].params_local = best_local_parameters_h[part].clone();
    clm_model.hierarchical_models[part].detected_landmarks = best_detected_landmarks_h[part].clone();
    clm_model.hierarchical_models[part].landmark_likelihoods = best_landmark_likelihoods_h[part].clone();
  }

  return best_success;
}

bool DetectLandmarksInImage(const img_t* grayscale_image, const img_t* depth_image, CLM& clm_model, CLMParameters& params)
{

  CRectd bounding_box;

  // If the face detector has not been initialised read it in

  // Detect the face first
#ifdef _USE_DLIB

  if (params.curr_face_detector == CLMParameters::HOG_SVM_DETECTOR) {
    double confidence;
    DetectSingleFaceHOG(bounding_box, grayscale_image, clm_model.face_detector_HOG, confidence);
  }
  else if (params.curr_face_detector == CLMParameters::HAAR_DETECTOR)
#endif
#ifdef _USE_CVFACE
  {
	  CPoint pt;
	  pt.x = -1;
	  pt.y = -1;
    DetectSingleFace(bounding_box, grayscale_image, pt);
  }
#endif

  if (bounding_box.width == 0) {
    return false;
  }
  else {
    return DetectLandmarksInImage(grayscale_image, depth_image, bounding_box, clm_model, params);
  }
}

// Versions not using depth images
bool DetectLandmarksInImageWithBounding(const img_t* grayscale_image, const CRectd bounding_box, CLM& clm_model, CLMParameters& params)
{
  return DetectLandmarksInImage(grayscale_image, NULL, bounding_box, clm_model, params);
}

