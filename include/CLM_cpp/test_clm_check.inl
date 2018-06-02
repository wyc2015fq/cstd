
#include <time.h>

void convert_to_grayscale(const img_t* in, img_t* out)
{
	IM_BEGIN(im, 10);
	if (in == out) {
		in = cvCloneMat(im++, im);
	}
	cvSetMat(out, in->rows, in->cols, CC_8U);
	if (CC_MAT_CN(in) == 3) {
		// Make sure it's in a correct format
		if (in->tid != CC_8U) {
			if (in->tid == CC_16U) {
				img_t* tmp = cvConvertTo(in, im++, CC_8U, 1./ 256, 0);
				cvCvtColor(tmp, out, CC_BGR2GRAY);
			}
		}
		else {
			cvCvtColor(in, out, CC_BGR2GRAY);
		}
	}
	else if (CC_MAT_CN(in) == 4) {
		cvCvtColor(in, out, CC_BGRA2GRAY);
	}
	else {
		if (in->tid == CC_16U) {
			img_t* tmp = cvConvertTo(in, out, CC_8U, 1. / 256, 0);
		}
		else if (in->tid != CV_8U) {
			cvConvertTo(in, out, CC_8U);
		}
		else {
			cvCloneMat(out, in);
		}
	}
	IM_END(im);
}

// This will only be accurate when camera parameters are accurate, useful for work on 3D data
void write_out_pose_landmarks(const char* outfeatures, const Mat& shape3D, const Vec6d& pose, const Point3f& gaze0, const Point3f& gaze1)
{
	create_directory_from_file(outfeatures);
	FILE* featuresFile = fopen(outfeatures, "wb");

	if (featuresFile) {
		int n = shape3D.cols;
		fprintf(featuresFile, "version: 1\n");
		fprintf(featuresFile, "npoints: %d\n", n);
		fprintf(featuresFile, "{\n");
    double* shape3D_ptr = img_ptr(double, &shape3D);
		for (int i = 0; i < n; ++i) {
			// Use matlab format, so + 1
			fprintf(featuresFile, " %f %f %f\n", shape3D_ptr[i], shape3D_ptr[i + n], shape3D_ptr[i + 2 * n]);
		}

		fprintf(featuresFile, "}\n");

		// Do the pose and eye gaze if present as well
		fprintf(featuresFile, "pose: eul_x, eul_y, eul_z: \n");
		fprintf(featuresFile, "{\n");
		fprintf(featuresFile, "%f %f %f\n", pose[3], pose[4], pose[5]);
		fprintf(featuresFile, "}\n");

		// Do the pose and eye gaze if present as well
		fprintf(featuresFile, "gaze: dir_x_1, dir_y_1, dir_z_1, dir_x_2, dir_y_2, dir_z_2: \n");
		fprintf(featuresFile, "{\n");
		fprintf(featuresFile, "%f %f %f %f %f %f\n", gaze0.x, gaze0.y, gaze0.z, gaze1.x, gaze1.y, gaze1.z);
		fprintf(featuresFile, "}\n");
		fclose(featuresFile);
	}
}

void write_out_landmarks(const char* outfeatures, const img_t* mat) {
	create_directory_from_file(outfeatures);
	FILE* featuresFile = fopen(outfeatures, "wb");

	if (featuresFile) {
		int n = mat->rows*mat->cols/2;
		fprintf(featuresFile, "version: 1\n");
		fprintf(featuresFile, "npoints: %d\n", n);
		fprintf(featuresFile, "{\n");
		double *it = img_ptr(double, mat);

		for (int i = 0; i < n; ++i) {
			// Use matlab format, so + 1
			fprintf(featuresFile, "%f %f\n", it[i] + 1, it[i + n] + 1);
		}

		fprintf(featuresFile, "}\n");
    fclose(featuresFile);
	}
}

void read_landmarks(const char* outfeatures, img_t* output_mat, TypeId tid) {
	FILE* stream = fopen(outfeatures, "rb");
	char ss[256];
	int ver = 0, n=0;
	fgets(ss, countof(ss), stream);// version
	fscanf(stream, "%d", &ver);
	fgets(ss, countof(ss), stream); // npoints
	fscanf(stream, "%d", n);
	fgets(ss, countof(ss), stream); // {
  img_t mat[1] = {0};
	cvSetMat(mat, 2, n, tid);
	double* it = img_ptr(double, mat);
	//double* end_it = img_end(double, output_mat);

	for (int i = 0; i < n; ++i) {
		double x, y;
		fscanf(stream, "%lf %lf", &x, &y);
		it[i] = x - 1;
		it[i + n] = y - 1;
	}
	fgets(ss, countof(ss), stream); // }
  cvConvertTo(mat, output_mat, tid);
  imfree(mat);
}

//#include "img/imgio.inl"

#ifndef countof
#define countof(_ARR)  (int)(sizeof(_ARR)/sizeof((_ARR)[0]))
#endif // countof

int test_CLM_check(int argc, char** argv)
{
	Mat Jx = Mat::zeros(10, 10, CV_64F);
	//Jx = ss;
	//Mat a=Mat::zeros(10, 10, CV_64F), b;	b = -a;
	//Mat a(1, 2, CV_64F);
	//Mat b = Mat::diag(a);
#ifdef _WIN32
	_chdir("D:/code/CLM-framework-master/Release");
#endif
	char** files = NULL;
	dirlist("./videos", &files, ".bmp");
	printf("%d\n", ppstrlen(files));
	//return 0;
  char* myargv[] = { "test_SimpleCLMImg", "-fdir", "./videos/", "-clmwild", "-oidir", "./videos/images_track/", "-ofdir", "./videos/images_track/" };
  argc = countof(myargv);
  argv = myargv;
  
  //Convert arguments to more convenient vector form
  //vector<char*> arguments = get_arguments(argc, argv);
  
  // Some initial parameters that can be overriden from command line
#if 0
  char** files = NULL;
  char** depth_files = NULL;
  char** output_images = NULL;
  char** output_landmark_locations = NULL;
  char** output_pose_locations = NULL;
#endif

	// Bounding boxes for a face in each image (optional)
	//vector<CRectd > bounding_boxes;
	const char* ext;
#ifdef _WIN32
	ext = ".bmp";
#else
	ext = ".jpg";
#endif

	//get_image_input_output_params(&files, &depth_files, &output_landmark_locations, &output_pose_locations, &output_images, bounding_boxes, argv, argc, ext);
	CLMParameters clm_parameters(argv, argc);
	// No need to validate detections, as we're not doing tracking
	clm_parameters.validate_detections = false;

  int i;
	for (i = 0; i < ppstrlen(files); i++) {
		char* outfeatures = files[i];
		char* fn = filename(outfeatures);
		printf("%s \n", outfeatures);
	}
	// Grab camera parameters if provided (only used for pose and eye gaze and are quite important for accurate estimates)
	float fx = 0, fy = 0, cx = 0, cy = 0;
	int device = -1;
	get_camera_params(device, fx, fy, cx, cy, argv, argc);

	// If cx (optical axis centre) is undefined will use the image size/2 as an estimate
	bool cx_undefined = false;
	bool fx_undefined = false;

	if (cx == 0 || cy == 0) {
		cx_undefined = true;
	}

	if (fx == 0 || fy == 0) {
		fx_undefined = true;
	}

	// The modules that are being used for tracking
	printf("Loading the model\n");
	CLM clm_model(clm_parameters.model_location);
	printf("Model loaded\n");

#ifdef _USE_DLIB
	dlib::frontal_face_detector face_detector_hog = dlib::get_frontal_face_detector();
#endif

	bool visualise = !clm_parameters.quiet_mode;

  IM_BEGIN(im, 10);
  //img_t *read_image = im++;
  img_t *im1 = im++;
  img_t *grayscale_image = im++;
  img_t *landmarks = im++;
	// Do some image loading
  int i;
	for (i = 0; i < ppstrlen(files); i++) {
		char* file = files[i];
		imread(file, 3, 1, im1);
		Mat read_image = ma(im1);
		//cvCopy(&mi(tt), read_image);
		//printf("%3d %3d ", read_image.rows, read_image.cols);

		if (read_image.rows < 2) {
			continue;
		}
    CRectd bounding_boxes = {0};
    
    char buf[256];
    char* boxfile = replace_extension(file, "txt", buf, 256);
    
    FILE* in_bbox = fopen(boxfile, "rb");
    if (in_bbox) {
      double min_x, min_y, max_x, max_y;
      fscanf(in_bbox, "%lf %lf %lf %lf", &min_x, &min_y, &max_x, &max_y);
      bounding_boxes = cRectd(min_x, min_y, max_x - min_x, max_y - min_y);
      fclose(in_bbox);
    } else {
      continue;
    }
    

		// Making sure the image is in uchar grayscale
		convert_to_grayscale(&read_image, (grayscale_image));

		// If optical centers are not defined just use center of image
		if (cx_undefined) {
			cx = grayscale_image->cols / 2.0f;
			cy = grayscale_image->rows / 2.0f;
		}

		// Use a rough guess-timate of focal length
		if (fx_undefined) {
			fx = 500 * (grayscale_image->cols / 640.0);
			fy = 500 * (grayscale_image->rows / 480.0);

			fx = (fx + fy) / 2.0;
			fy = fx;
		}

		// if no pose defined we just use a face detector

		// Have provided bounding boxes
		tic;
		DetectLandmarksInImageWithBounding(grayscale_image, bounding_boxes, clm_model, clm_parameters);
		toc;
		tictoc;
		// Estimate head pose and eye gaze
#ifdef _GET_POSE
		Vec6d headPose = GetCorrectedPoseWorld(clm_model, fx, fy, cx, cy);
#endif

		// Gaze tracking, absolute gaze direction
		CPoint3f gazeDirection0 = cPoint3f(0, 0, -1);
		CPoint3f gazeDirection1 = cPoint3f(0, 0, -1);

		// Gaze with respect to head rather than camera (for example if eyes are rolled up and the head is tilted or turned this will be stable)
		CPoint3f gazeDirection0_head = cPoint3f(0, 0, -1);
		CPoint3f gazeDirection1_head = cPoint3f(0, 0, -1);

		if (clm_parameters.track_gaze) {
			EstimateGaze(clm_model, gazeDirection0, gazeDirection0_head, fx, fy, cx, cy, true);
			EstimateGaze(clm_model, gazeDirection1, gazeDirection1_head, fx, fy, cx, cy, false);
		}
		if ( clm_parameters.track_gaze)
		{
#ifdef _GET_POSE
			Vec6d pose_estimate_to_draw = GetCorrectedPoseWorld(clm_model, fx, fy, cx, cy);

			// Draw it in reddish if uncertain, blueish if certain
			DrawBox(&read_image, pose_estimate_to_draw, cScalar(255.0, 0, 0), 3, fx, fy, cx, cy);
			DrawGaze(&read_image, clm_model, gazeDirection0, gazeDirection1, fx, fy, cx, cy);
#endif
		}
    Draw(&read_image, clm_model, 1);
    imshow2("adsf", &read_image); WaitKey(-1);
#if 0
		if (1) {
			char* outfeatures = output_landmark_locations[i];
			Mat& m1 = clm_model.detected_landmarks;
			read_landmarks(outfeatures, landmarks, CC_64F);
			int n = landmarks->cols;
			Mat m2 = m1.reshape(1, 2);
			char* fn = filename(outfeatures);
			printf("%20s ", fn);
			char* outname = "./tmp/" + fn;
			//write_out_landmarks(outname, m2);
			//write_out_landmarks("D:/tmp/mat.txt", mat);
			//write_out_landmarks("D:/tmp/m11.txt", clm_model);
			img_t m2_ = mi(m2);
			double err = cvNorm(landmarks, &m2_);
			double nn = cvNorm(landmarks);
			printf("err = %8.5f  \t%s\n", err, err<0.1 ? "OK" : "fail");
			//tictoc;
			assert(err<0.1);
		}
#endif
	}
	tictoc;
	IM_END(im);
	//test_MultiTrack();
	return 0;
}
