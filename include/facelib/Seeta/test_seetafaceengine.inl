
//#define USE_SSE
#include "cstd.h"

#include <stdlib.h>
#include <stdio.h>

#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

#include "FaceDetection.inl"
#include "FaceAlignment.inl"
#include "FaceIdentification.inl"

#include "img/imgio.inl"
#include "img/imgopt.inl"
#include "cv/imgproc/drawing.inl"

#define FACE_DETECTION_MODEL_PATH       "D:/data/model/seeta_fd_frontal_v1.0.bin"
#define FACE_ALIGNMENT_MODEL_PATH       "D:/data/model/seeta_fa_v1.1.bin"
#define FACE_IDENTIFICATION_MODEL_PATH  "D:/data/model/seeta_fr_v1.0.bin"


//#define TEST(major, minor) major##_##minor##_Tester()
#define EXPECT_NE(a, b) if ((a) == (b)) std::cout << "ERROR: "
#define EXPECT_EQ(a, b) if ((a) != (b)) std::cout << "ERROR: "

std::string DATA_DIR = "D:/code/git/SeetaFaceEngine/FaceIdentification/data/";


int test_facedetection() {
	const char* img_path = NULL;
	const char* model_path = NULL;
	img_path = "D:/Î¢ÐÅÍ¼Æ¬_20171222184549.jpg";
	img_path = "D:/2f74e7ab30b2b0202e3da82f1e592d64.jpg";
	img_path = "F:/dataset/facedetect/group/aaa641cc3150d847c2cc03768a13eded.jpg";
	img_path = "C:/code/cstd/include/facelib/SeetaFaceEngine/FaceDetection/test/0_1_1.jpg";
	model_path = FACE_DETECTION_MODEL_PATH;
	FaceDetection detector(model_path);

	detector.SetMinFaceSize(40);
	detector.SetScoreThresh(2.f);
	detector.SetImagePyramidScaleFactor(0.8f);
	detector.SetWindowStep(4, 4);

	img_t img[10] = { 0 };
	img_t* img_gray = img + 1;
	imread(img_path, 3, 1, img);
	im2gry(img, img_gray);

	ImageData img_data;
	img_data.data = img_gray->data;
	img_data.width = img_gray->cols;
	img_data.height = img_gray->rows;
	img_data.num_channels = 1;

	std::vector<FaceInfo> faces;
	tic;
	faces = detector.Detect(img_data);
	toc;
  tictoc;

#ifdef USE_OPENMP
	printf("OpenMP is used.\n");
#else
	printf("OpenMP is not used.\n");
#endif

#ifdef USE_SSE
	printf("SSE is used.\n");
#else
	printf("SSE is not used.");
#endif

	printf("Image size (wxh): %dx%d\n", img_data.width, img_data.height);

	CRect face_rect;
	int32_t num_face = static_cast<int32_t>(faces.size());

	for (int32_t i = 0; i < num_face; i++) {
		face_rect = faces[i].bbox;

    ASSERT(156==face_rect.x);
    ASSERT(217==face_rect.y);
    ASSERT(222==face_rect.w);
    ASSERT(222==face_rect.h);
		rectangle(img, face_rect, CC_RGB(0, 0, 255), 4, 8, 0);
	}

	imshow_(img);
	WaitKey(-1);

	imfrees2(img);
	return 0;
}

int test_face_alignment(int argc, char** argv) {
	// Initialize face detection model
	FaceDetection detector(FACE_DETECTION_MODEL_PATH);
	detector.SetMinFaceSize(40);
	detector.SetScoreThresh(2.f);
	detector.SetImagePyramidScaleFactor(0.8f);
	detector.SetWindowStep(4, 4);

	// Initialize face alignment model 
	FaceAlignment point_detector(FACE_ALIGNMENT_MODEL_PATH);
	img_t im[10] = { 0 };
	//load image
	img_t *img_color = im;
	img_t *img_grayscale = im + 1;
	const char* image_path = "C:/code/cstd/include/facelib/SeetaFaceEngine/FaceAlignment/image_0001.png";
	img_color = imread(image_path, 3, 1, img_color);
	if (img_color == NULL)
	{
		return 0;
	}
	im2gry(img_color, img_grayscale);
	int pts_num = 5;
	int im_width = img_grayscale->width;
	int im_height = img_grayscale->height;
	unsigned char* data = new unsigned char[im_width * im_height];
	unsigned char* data_ptr = data;
	unsigned char* image_data_ptr = (unsigned char*)img_grayscale->imageData;
	int h = 0;
	for (h = 0; h < im_height; h++) {
		memcpy(data_ptr, image_data_ptr, im_width);
		data_ptr += im_width;
		image_data_ptr += img_grayscale->widthStep;
	}

	ImageData image_data;
	image_data.data = data;
	image_data.width = im_width;
	image_data.height = im_height;
	image_data.num_channels = 1;

	// Detect faces
	std::vector<FaceInfo> faces;
	tic;
	faces = detector.Detect(image_data);
	toc_;
	int32_t face_num = static_cast<int32_t>(faces.size());

	if (face_num == 0)
	{
		delete[]data;
		cvReleaseImage(&img_grayscale);
		cvReleaseImage(&img_color);
		return 0;
	}

	// Detect 5 facial landmarks
	FacialLandmark points[5];
	tic;
	point_detector.PointDetectLandmarks(image_data, faces[0], points);
	toc_;

	// Visualize the results
	cvRectangle(img_color, cPoint(faces[0].bbox.x, faces[0].bbox.y),
		cPoint(faces[0].bbox.x + faces[0].bbox.width - 1, faces[0].bbox.y + faces[0].bbox.height - 1),
		CC_RGB(255, 0, 0), 1, 8, 0);
	for (int i = 0; i<pts_num; i++)
	{
		cvCircle(img_color, cPoint(points[i].x, points[i].y), 2, CC_RGB(0, 255, 0), CC_FILLED, 8, 0);
	}
	imshow_(img_color);
	WaitKey(-1);

	delete[]data;
	imfrees2(im);
	return 0;
}

int test_face_verification(int argc, char* argv[]) {
	// Initialize face detection model
	FaceDetection detector(FACE_DETECTION_MODEL_PATH);
	detector.SetMinFaceSize(40);
	detector.SetScoreThresh(2.f);
	detector.SetImagePyramidScaleFactor(0.8f);
	detector.SetWindowStep(4, 4);

	// Initialize face alignment model 
	FaceAlignment point_detector(FACE_ALIGNMENT_MODEL_PATH);

	// Initialize face Identification model 
	FaceIdentification face_recognizer(FACE_IDENTIFICATION_MODEL_PATH);
	std::string test_dir = "D:/code/git/SeetaFaceEngine/FaceIdentification/data/test_face_recognizer/";

	//load image
	img_t im[10] = { 0 };
	img_t* im1 = im;
	img_t* gallery_img_color = imread((test_dir + "images/compare_im/Aaron_Peirsol_0001.jpg").c_str(), 3, 1, im1++);
	img_t* gallery_img_gray = im1++;
	im2gry(gallery_img_color, gallery_img_gray);

	img_t* probe_img_color = imread((test_dir + "images/compare_im/Aaron_Peirsol_0004.jpg").c_str(), 3, 1, im1++);
	img_t* probe_img_gray = im1++;
	im2gry(probe_img_color, probe_img_gray);

	ImageData gallery_img_data_color(gallery_img_color->data, gallery_img_color->cols, gallery_img_color->rows, gallery_img_color->c);
	ImageData gallery_img_data_gray(gallery_img_gray->data, gallery_img_gray->cols, gallery_img_gray->rows, gallery_img_gray->c);
	ImageData probe_img_data_color(probe_img_color->data, probe_img_color->cols, probe_img_color->rows, probe_img_color->c);
	ImageData probe_img_data_gray(probe_img_gray->data, probe_img_gray->cols, probe_img_gray->rows, probe_img_gray->c);

	// Detect faces
	std::vector<FaceInfo> gallery_faces = detector.Detect(gallery_img_data_gray);
	int32_t gallery_face_num = static_cast<int32_t>(gallery_faces.size());

	std::vector<FaceInfo> probe_faces = detector.Detect(probe_img_data_gray);
	int32_t probe_face_num = static_cast<int32_t>(probe_faces.size());

	if (gallery_face_num == 0 || probe_face_num == 0)
	{
		printf("Faces are not detected.\n");
		return 0;
	}

	// Detect 5 facial landmarks
	FacialLandmark gallery_points[5];
	point_detector.PointDetectLandmarks(gallery_img_data_gray, gallery_faces[0], gallery_points);

	FacialLandmark probe_points[5];
	point_detector.PointDetectLandmarks(probe_img_data_gray, probe_faces[0], probe_points);

	for (int i = 0; i<5; i++)
	{
		circle(gallery_img_color, cPoint(gallery_points[i].x, gallery_points[i].y), 2, CC_RGB(0, 255, 0));
		circle(probe_img_color, cPoint(probe_points[i].x, probe_points[i].y), 2, CC_RGB(0, 255, 0));
	}
	imshow_(gallery_img_color);
	imshow_(probe_img_color);

	// Extract face identity feature
	float gallery_fea[2048];
	float probe_fea[2048];
#if 0
  FacialLandmark gallery_points_[5] = {
		107.00814056396484,
		112.77738189697266,

		144.81147766113281,
		112.44226074218750,

		129.58927917480469,
		140.18258666992187,

		108.34762573242187,
		154.93479919433594,

		145.34555053710937,
		154.77014160156250,
  };
  FacialLandmark probe_points_[5] = {

		108.84412384033203,
		111.63375091552734,

		147.77804565429688,
		113.10516357421875,

		122.81320190429687,
		138.63569641113281,

		109.18836975097656,
		158.60026550292969,

		141.86750793457031,
		160.88662719726562,
  };
#endif

#if 1
  FacialLandmark gallery_points_[5] = { 108.01280212402344, 115.21343994140625, 147.99578857421875, 114.51779174804688, 129.07791137695312, 142.24993896484375, 108.84745788574219, 156.11947631835938, 148.44784545898438, 155.27120971679688 };
  FacialLandmark probe_points_[5] = { 109.75684356689453, 113.79313659667969, 148.48760986328125, 115.44265747070312, 123.31971740722656, 140.52932739257812, 111.93360137939453, 162.70193481445312, 145.51885986328125, 163.10049438476562 };
#endif


  MEMCPY(gallery_points, gallery_points_, 5);
  MEMCPY(probe_points, probe_points_, 5);

	tic;
	face_recognizer.ExtractFeatureWithCrop(gallery_img_data_color, gallery_points, gallery_fea);
	face_recognizer.ExtractFeatureWithCrop(probe_img_data_color, probe_points, probe_fea);
	toc;

	// Caculate similarity of two faces
	double sim;
	tic;
	sim = face_recognizer.CalcSimilarity(gallery_fea, probe_fea);
	toc;
  tictoc;
	printf("%lf\n", sim);
	WaitKey(-1);
#if 0
#endif
	imfrees2(im);
	return 0;
}

#if 0

void TestFaceRecognizer_CropFace() {
	FaceIdentification face_recognizer(FACE_IDENTIFICATION_MODEL_PATH);
	std::string test_dir = DATA_DIR + "test_face_recognizer/";
	/* data initialize */
	std::ifstream ifs;
	std::string img_name;
	FacialLandmark pt5[5];
	ifs.open(test_dir + "test_file_list.txt", std::ifstream::in);
	clock_t start, count = 0;
	int img_num = 0;
	img_t im[10] = { 0 };
	while (ifs >> img_name) {
		img_t* im1 = im;
		img_num++;
		// read image
		img_t* src_img = imread((test_dir + img_name).c_str(), 3, 1, im1++);
		EXPECT_NE(src_img->data, NULL) << "Load image error!";

		// ImageData store data of an image without memory alignment.
		ImageData src_img_data(src_img->cols, src_img->rows, src_img->c);
		src_img_data.data = src_img->data;

		// 5 located landmark points (left eye, right eye, nose, left and right 
		// corner of mouse).
		for (int i = 0; i < 5; ++i) {
			ifs >> pt5[i].x >> pt5[i].y;
		}

		// Create a image to store crop face.
		img_t* dst_img = im1++;
		imsetsize(dst_img, face_recognizer.crop_height(), face_recognizer.crop_width(), face_recognizer.crop_channels(), 1);
		ImageData dst_img_data(dst_img->cols, dst_img->rows, dst_img->c);
		dst_img_data.data = dst_img->data;
		/* Crop Face */
		start = clock();
		face_recognizer.CropFace(src_img_data, pt5, dst_img_data);
		count += clock() - start;
		// Show crop face
		//    cv::imshow("Crop Face", dst_img);
		//    cv::waitKey(0);
		//    cv::destroyWindow("Crop Face");
	}
	ifs.close();
	std::cout << "Test successful! \nAverage crop face time: "
		<< 1000.0 * count / CLOCKS_PER_SEC / img_num << "ms" << std::endl;
	imfrees2(im);
}

void TestFaceRecognizer_ExtractFeature() {
	FaceIdentification face_recognizer(FACE_IDENTIFICATION_MODEL_PATH);
	std::string test_dir = DATA_DIR + "test_face_recognizer/";

	int feat_size = face_recognizer.feature_size();
	EXPECT_EQ(feat_size, 2048);

	FILE* feat_file = NULL;

	// Load features extract from caffe
	fopen_s(&feat_file, (test_dir + "feats.dat").c_str(), "rb");
	int n, c, h, w;
	EXPECT_EQ(fread(&n, sizeof(int), 1, feat_file), (unsigned int)1);
	EXPECT_EQ(fread(&c, sizeof(int), 1, feat_file), (unsigned int)1);
	EXPECT_EQ(fread(&h, sizeof(int), 1, feat_file), (unsigned int)1);
	EXPECT_EQ(fread(&w, sizeof(int), 1, feat_file), (unsigned int)1);
	float* feat_caffe = new float[n * c * h * w];
	float* feat_sdk = new float[n * c * h * w];
	EXPECT_EQ(fread(feat_caffe, sizeof(float), n * c * h * w, feat_file),
		n * c * h * w);
	EXPECT_EQ(feat_size, c * h * w);

	int cnt = 0;

	/* Data initialize */
	std::ifstream ifs(test_dir + "crop_file_list.txt");
	std::string img_name;

	clock_t start, count = 0;
	int img_num = 0, lb;
	double average_sim = 0.0;
	img_t im[10] = { 0 };
	while (ifs >> img_name >> lb) {
		img_t* im1 = im;
		// read image
		img_t* src_img = imread((test_dir + img_name).c_str(), 3, 1, im1++);
		EXPECT_NE(src_img->data, NULL) << "Load image error!";
		CSize size = cSize(face_recognizer.crop_height(), face_recognizer.crop_width());
		imresize(src_img, size.h, size.w, src_img);

		// ImageData store data of an image without memory alignment.
		ImageData src_img_data(src_img->cols, src_img->rows, src_img->c);
		src_img_data.data = src_img->data;

		/* Extract feature */
		start = clock();
		face_recognizer.ExtractFeature(src_img_data,
			feat_sdk + img_num * feat_size);
		count += clock() - start;

		/* Caculate similarity*/
		float* feat1 = feat_caffe + img_num * feat_size;
		float* feat2 = feat_sdk + img_num * feat_size;
		float sim = face_recognizer.CalcSimilarity(feat1, feat2);
		average_sim += sim;
		img_num++;
	}
	ifs.close();
	average_sim /= img_num;
	if (1.0 - average_sim >  0.01) {
		std::cout << "average similarity: " << average_sim << std::endl;
	}
	else {
		std::cout << "Test successful!\nAverage extract feature time: "
			<< 1000.0 * count / CLOCKS_PER_SEC / img_num << "ms" << std::endl;
	}
	delete[]feat_caffe;
	delete[]feat_sdk;
	imfrees2(im);
}

void TestFaceRecognizer_ExtractFeatureWithCrop() {
	FaceIdentification face_recognizer(FACE_IDENTIFICATION_MODEL_PATH);
	std::string test_dir = DATA_DIR + "test_face_recognizer/";

	int feat_size = face_recognizer.feature_size();
	EXPECT_EQ(feat_size, 2048);

	FILE* feat_file = NULL;

	// Load features extract from caffe
	fopen_s(&feat_file, (test_dir + "feats.dat").c_str(), "rb");
	int n, c, h, w;
	EXPECT_EQ(fread(&n, sizeof(int), 1, feat_file), (unsigned int)1);
	EXPECT_EQ(fread(&c, sizeof(int), 1, feat_file), (unsigned int)1);
	EXPECT_EQ(fread(&h, sizeof(int), 1, feat_file), (unsigned int)1);
	EXPECT_EQ(fread(&w, sizeof(int), 1, feat_file), (unsigned int)1);
	float* feat_caffe = new float[n * c * h * w];
	float* feat_sdk = new float[n * c * h * w];
	EXPECT_EQ(fread(feat_caffe, sizeof(float), n * c * h * w, feat_file),
		n * c * h * w);
	EXPECT_EQ(feat_size, c * h * w);

	int cnt = 0;

	/* Data initialize */
	std::ifstream ifs(test_dir + "test_file_list.txt");
	std::string img_name;
	FacialLandmark pt5[5];

	clock_t start, count = 0;
	int img_num = 0;
	double average_sim = 0.0;
	img_t im[10] = { 0 };
	while (ifs >> img_name) {
		img_t* im1 = im;
		// read image
		img_t* src_img = imread((test_dir + img_name).c_str(), 3, 1, im1++);
		EXPECT_NE(src_img->data, NULL) << "Load image error!";

		// ImageData store data of an image without memory alignment.
		ImageData src_img_data(src_img->cols, src_img->rows, src_img->c);
		src_img_data.data = src_img->data;

		// 5 located landmark points (left eye, right eye, nose, left and right 
		// corner of mouse).
		for (int i = 0; i < 5; ++i) {
			ifs >> pt5[i].x >> pt5[i].y;
		}

		/* Extract feature: ExtractFeatureWithCrop */
		start = clock();
		face_recognizer.ExtractFeatureWithCrop(src_img_data, pt5,
			feat_sdk + img_num * feat_size);
		count += clock() - start;

		/* Caculate similarity*/
		float* feat1 = feat_caffe + img_num * feat_size;
		float* feat2 = feat_sdk + img_num * feat_size;
		float sim = face_recognizer.CalcSimilarity(feat1, feat2);
		average_sim += sim;
		img_num++;
	}
	ifs.close();
	average_sim /= img_num;
	if (1.0 - average_sim >  0.02) {
		std::cout << "average similarity: " << average_sim << std::endl;
	}
	else {
		std::cout << "Test successful!\nAverage extract feature time: "
			<< 1000.0 * count / CLOCKS_PER_SEC / img_num << "ms" << std::endl;
	}
	delete[]feat_caffe;
	delete[]feat_sdk;
	imfrees2(im);
}

int test_face_recognizer() {
	TestFaceRecognizer_CropFace();
	TestFaceRecognizer_ExtractFeature();
	TestFaceRecognizer_ExtractFeatureWithCrop();
	return 0;
}

#endif