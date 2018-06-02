
int CLM_MultiTrack(const img_t* captured_image, char character_press, bool do_detect_face, img_t* disp_image) {
	static int inited = 0;
	static vector<CLMParameters> clm_parameters;
	static vector<CLM> clm_models;
	static vector<bool> active_models;
	if (!inited) {
		inited = 1;
    char* argv[] = {"CLM_MultiTrack"};
		CLMParameters clm_params(argv, countof(argv));
		clm_params.use_face_template = true;
		// This is so that the model would not try re-initialising itself
		clm_params.reinit_video_every = -1;

		//clm_params.curr_face_detector = CLMParameters::HOG_SVM_DETECTOR;

		clm_parameters.push_back(clm_params);

		int num_faces_max = 1;

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
	}
	// Reading the images
	img_t *depth_image = NULL;
	IM_BEGIN(im, 10);
	cvCloneMat(disp_image, captured_image);

	double ss = 1;
	if (ss < 1) {
		CSize sz;
		sz.width = captured_image->cols*ss;
		sz.height = captured_image->rows*ss;
		captured_image = cvResize(captured_image, im++, sz);
	}
	const img_t *grayscale_image = NULL;
	if (captured_image->c == 3) {
		img_t* grayscale_image1 = im++;
		cvSetMat(grayscale_image1, captured_image->h, captured_image->w, captured_image->tid, 1);
		grayscale_image = cvCvtColor(captured_image, grayscale_image1, CC_BGR2GRAY);
	}
	else {
		grayscale_image = captured_image;
	}

	vector<CRectd > face_detections;

	bool all_models_active = true;
  int model;
	for (model = 0; model < clm_models.size(); ++model) {
		if (!active_models[model]) {
			all_models_active = false;
		}
	}

	// Get the detections (every 8th frame and when there are free models available for tracking)
	if (do_detect_face && !all_models_active) {
		DetectFaces(face_detections, grayscale_image);
		if (0 && face_detections.size() > 0) {
			FILE* pf = fopen("D:/code/CLM-framework-master/Release/videos/0000.txt", "wb");
			double x = face_detections[0].x, y = face_detections[0].y;
			fprintf(pf, "%lf %lf %lf %lf", x, y, x+face_detections[0].w, y+face_detections[0].h);
			fclose(pf);
			imwrite("D:/code/CLM-framework-master/Release/videos/0000.jpg", captured_image);
		}
	}

	// Keep only non overlapping detections (also convert to a concurrent vector
	NonOverlapingDetections(clm_models, face_detections);

	//vector<bool > face_detections_used(face_detections.size());

	// Go through every model and update the tracking TODO pull out as a separate parallel/non-parallel method
	//tbb::parallel_for(0, (int)clm_models.size(), [&](int model)
	//myparallel_for(0, (int)clm_models.size(), model) {
  for(model = 0; model < clm_models.size(); ++model) {

		bool detection_success = false;

		// If the current model has failed more than 4 times in a row, remove it
		if (clm_models[model].failures_in_a_row > 4) {
			active_models[model] = false;
			clm_models[model].Reset();

		}

		// If the model is inactive reactivate it with new detections
		if (!active_models[model]) {

			for (size_t detection_ind = 0; detection_ind < face_detections.size(); ++detection_ind) {
				// if it was not taken by another tracker take it (if it is false swap it to true and enter detection, this makes it parallel safe)
				//if(face_detections_used[detection_ind].compare_and_swap(true, false) == false)
					// Reinitialise the model
				clm_models[model].Reset();
				// This ensures that a wider window is used for the initial landmark localisation
				clm_models[model].detection_success = false;
				detection_success = DetectLandmarksInVideo(grayscale_image, depth_image, face_detections[detection_ind], clm_models[model], clm_parameters[model]);

				// This activates the model
				active_models[model] = true;

				// break out of the loop as the tracker has been reinitialised
				//break;
			}
			//printf("%s %d\n", __FUNCTION__, __LINE__);
		}
		else {
			// The actual facial landmark detection / tracking
			detection_success = DetectLandmarksInVideo(grayscale_image, depth_image, clm_models[model], clm_parameters[model]);
		}
	}

	// Go through every model and visualise the results
	for (model = 0; model < clm_models.size(); ++model) {
		// Visualising the results
		// Drawing the facial landmarks on the face and the bounding box around it if tracking is successful and initialised
		double detection_certainty = clm_models[model].detection_certainty;

		double visualisation_boundary = -0.1;

		Draw(disp_image, clm_models[model], 1. / ss);
		// Only draw if the reliability is reasonable, the value is slightly ad-hoc
		if (detection_certainty < visualisation_boundary) {

#ifdef _GET_POSE
			if (detection_certainty > 1) {
				detection_certainty = 1;
			}

			if (detection_certainty < -1) {
				detection_certainty = -1;
			}

			detection_certainty = (detection_certainty + 1) / (visualisation_boundary + 1);

			// A rough heuristic for box around the face width
			int thickness = (int)ceil(2.0 * ((double)captured_image->cols) / 640.0);

			float fx = 600, fy = 600, cx = 0, cy = 0;

			if (1) {
				cx = grayscale_image->cols / 2.0f;
				cy = grayscale_image->rows / 2.0f;
			}

			// Use a rough guess-timate of focal length
			if (1) {
				fx = 500 * (grayscale_image->cols / 640.0);
				fy = 500 * (grayscale_image->rows / 480.0);

				fx = (fx + fy) / 2.0;
				fy = fx;
			}
			// Work out the pose of the head from the tracked model
			Vec6d pose_estimate_CLM = GetCorrectedPoseWorld(clm_models[model], fx, fy, cx, cy);

			// Draw it in reddish if uncertain, blueish if certain
			DrawBox(disp_image, pose_estimate_CLM, cScalar((1 - detection_certainty) * 255.0, 0, detection_certainty * 255), thickness, fx, fy, cx, cy);
#endif // _GET_POSE
		}
	}

	int num_active_models = 0;

	for (size_t active_model = 0; active_model < active_models.size(); active_model++) {
		if (active_models[active_model]) {
			num_active_models++;
		}
	}

	// restart the trackers
	if (character_press == 'r') {
		for (size_t i = 0; i < clm_models.size(); ++i) {
			clm_models[i].Reset();
			active_models[i] = false;
		}
	}

	IM_END(im);
	// Update the frame count
	return 0;
}

#include "cstd.h"
#include "cap.h"
#include "net.h"

#define PORT 6665
#if 1
#define HEIGHT 720
#define WIDTH 1280
#else
#define HEIGHT 480
#define WIDTH  640
#endif

#ifndef _WIN32
#define _USE_NET
#endif

int test_MultiTrack() {
	int high = 0;
	capdev cap[1] = { 0 };
	IM_BEGIN(im, 10);

	if (capdshow_open(cap, 0, WIDTH, HEIGHT)) {
		int fram = 0;
		char ch = 0;
		img_t *img = im++;
		img_t *dispimg = im++;
		img_t *dispimg4 = im++;

		int cli_sockfd = 0;

#ifdef _USE_NET
		if (1) {
			addr_in addr[1] = { 0 };
			int ser_sockfd = sock_open_tcp_server(NULL, PORT, SOCK_TCP, addr);
			if (ser_sockfd > 0) {
				char buf[256];
				int len, ch = 's';
				int err = sock_listen(ser_sockfd, 5);
				if (err == -1) {
					printf("listen error:%s\n", sys_errmsg(0));
					return -1;
				}
				addr_in cli_addr[1] = { 0 };
				cli_sockfd = sock_accept(ser_sockfd, cli_addr);
				if (cli_sockfd == -1) {
					printf("accept error:%s\n", sys_errmsg(0));
				}
				addr_tostr(cli_addr, buf, countof(buf));
				printf("accept %s\n", buf);
			}
			else {
				ch = 'q';
	}
}
#endif

		for (; 'q' != ch && 'Q' != ch; ++fram) {
			cap_getframe(cap, img, CAP_RGB);
#if 1
			//printf("%d w=%d h=%d\n", fram, im->w, im->h);
			if (1) {
				tic;
				{utime_start(_start_time);
				CLM_MultiTrack(img, 0, fram % 4 == 0, dispimg);
				printf("%f\n", utime_elapsed(_start_time)); }
				toc;
			}
			else {
				cvCloneMat(dispimg, img);
			}
			tictoc;
			imshow_(dispimg);
#else
			imshow_(im);

#endif
			if (cli_sockfd) {
				//cvSetMat(dispimg4, dispimg->rows, dispimg->cols, CC_8U, 4);
				//cvCvtColor(dispimg, dispimg4, CC_RGB2RGBA);
				int n = sock_send_safe(cli_sockfd, dispimg->tt.data, dispimg->s*dispimg->h, NULL);
				if (n < 0) {
					sock_close(cli_sockfd);
					break;
				}
			}
			ch = WaitKey(10);
		}
		cap_close(cap);
		imfree(im);
	}
	IM_END(im);

	return 0;
}

