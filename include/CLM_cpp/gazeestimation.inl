

CPoint3f RaySphereIntersect(CPoint3f rayOrigin, CPoint3f rayDir, CPoint3f sphereOrigin, float sphereRadius)
{
  float dx = rayDir.x;
  float dy = rayDir.y;
  float dz = rayDir.z;
  float x0 = rayOrigin.x;
  float y0 = rayOrigin.y;
  float z0 = rayOrigin.z;
  float cx = sphereOrigin.x;
  float cy = sphereOrigin.y;
  float cz = sphereOrigin.z;
  float r = sphereRadius;

  float a = dx * dx + dy * dy + dz * dz;
  float b = 2 * dx * (x0 - cx) + 2 * dy * (y0 - cy) + 2 * dz * (z0 - cz);
  float c = cx * cx + cy * cy + cz * cz + x0 * x0 + y0 * y0 + z0 * z0 + -2 * (cx * x0 + cy * y0 + cz * z0) - r * r;

  float disc = b * b - 4 * a * c;

  float t = (-b - sqrt(b * b - 4 * a * c)) / 2 * a;
  CPoint3f ret;
  // This implies that the lines did not intersect, point straight ahead
  if (b * b - 4 * a * c < 0) {
	  ret = cPoint3f(0, 0, -1);
  } else {
	  ret = rayOrigin + rayDir * t;
  }
  return ret;
}

CPoint3f GetPupilPosition(Mat eyeLdmks3d)
{
  eyeLdmks3d = eyeLdmks3d.t();

  Mat irisLdmks3d = eyeLdmks3d.rowRange(0, 8);

  CPoint3f p = cPoint3f(mean(irisLdmks3d.col(0))[0], mean(irisLdmks3d.col(1))[0], mean(irisLdmks3d.col(2))[0]);
  return p;
}

void EstimateGaze(const CLM& clm_model, CPoint3f& gaze_absolute, CPoint3f& gaze_head, float fx, float fy, float cx, float cy, bool left_eye)
{
  Vec6d headPose = GetPoseCamera(clm_model, fx, fy, cx, cy);
  Vec3d eulerAngles(headPose(3), headPose(4), headPose(5));
  Matx33d rotMat = Euler2RotationMatrix(eulerAngles);

  int part = -1;

  for (size_t i = 0; i < clm_model.hierarchical_models.size(); ++i) {
    char* str = clm_model.hierarchical_model_names[i];
    if (left_eye && strcmp(str, "left_eye_28") == 0) {
      part = i;
    }

    if (!left_eye && strcmp(str, "right_eye_28") == 0) {
      part = i;
    }
  }

  if (part == -1) {
    printf("Couldn't find the eye model, something wrong\n");
  }

  Mat eyeLdmks3d = clm_model.hierarchical_models[part].GetShape(fx, fy, cx, cy);

  CPoint3f pupil = GetPupilPosition(eyeLdmks3d);
  CPoint3f rayDir = pupil / norm(pupil);

  Mat faceLdmks3d = clm_model.GetShape(fx, fy, cx, cy);
  faceLdmks3d = faceLdmks3d.t();
  double offset_[] = { 0, -3.50, 0 };
  Mat offset(3, 1, CC_64F, offset_);
  int eyeIdx = 1;

  if (left_eye) {
    eyeIdx = 0;
  }

  Mat eyeballCentreMat = (faceLdmks3d.row(36 + eyeIdx * 6) + faceLdmks3d.row(39 + eyeIdx * 6)) / 2.0f + (Mat(rotMat) * offset).t();

  //CPoint3f eyeballCentre = Point3f(eyeballCentreMat);
  CPoint3f eyeballCentre;
  double* eyeballCentreMat_ptr = img_ptr(double, &eyeballCentreMat);
  eyeballCentre.x = eyeballCentreMat_ptr[0];
  eyeballCentre.y = eyeballCentreMat_ptr[1];
  eyeballCentre.z = eyeballCentreMat_ptr[2];

  CPoint3f gazeVecAxis = RaySphereIntersect(cPoint3f(0, 0, 0), rayDir, eyeballCentre, 12) - eyeballCentre;

  gaze_absolute = gazeVecAxis / norm(gazeVecAxis);

  gaze_head = cPoint3f(rotMat * Vec3d(gaze_absolute.x, gaze_absolute.y, gaze_absolute.z));
}


void DrawGaze(img_t* img, const CLM& clm_model, CPoint3f gazeVecAxisLeft, CPoint3f gazeVecAxisRight, float fx, float fy, float cx, float cy)
{
#if 0
  Mat cameraMat = (Mat_<double>(3, 3) << fx, 0, cx, 0, fy, cy, 0, 0, 0);

  int part_left = -1;
  int part_right = -1;

  for (size_t i = 0; i < clm_model.hierarchical_models.size(); ++i) {
    if (clm_model.hierarchical_model_names[i].compare("left_eye_28") == 0) {
      part_left = i;
    }

    if (clm_model.hierarchical_model_names[i].compare("right_eye_28") == 0) {
      part_right = i;
    }
  }

  Mat eyeLdmks3d_left = clm_model.hierarchical_models[part_left].GetShape(fx, fy, cx, cy);
  Point3f pupil_left = GetPupilPosition(eyeLdmks3d_left);

  Mat eyeLdmks3d_right = clm_model.hierarchical_models[part_right].GetShape(fx, fy, cx, cy);
  Point3f pupil_right = GetPupilPosition(eyeLdmks3d_right);

  vector<Point3d> points_left;
  points_left.push_back(Point3d(pupil_left));
  points_left.push_back(Point3d(pupil_left + gazeVecAxisLeft * 50.0));

  vector<Point3d> points_right;
  points_right.push_back(Point3d(pupil_right));
  points_right.push_back(Point3d(pupil_right + gazeVecAxisRight * 50.0));

  vector<Point2d> imagePoints_left;
  projectPoints(points_left, Mat::eye(3, 3, DataType<double>::type), Mat::zeros(1, 3, DataType<double>::type), cameraMat, Mat::zeros(4, 1, DataType<double>::type), imagePoints_left);
  line(img, imagePoints_left[0], imagePoints_left[1], Scalar(110, 220, 0), 2, 8);

  vector<Point2d> imagePoints_right;
  projectPoints(points_right, Mat::eye(3, 3, DataType<double>::type), Mat::zeros(1, 3, DataType<double>::type), cameraMat, Mat::zeros(4, 1, DataType<double>::type), imagePoints_right);
  line(img, imagePoints_right[0], imagePoints_right[1], Scalar(110, 220, 0), 2, 8);
#endif
}