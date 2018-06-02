

#if 0
typedef struct XRECT {
	int x, y, w, h;      /* 目标的所有框的位置平均值 */
	int score;           /* 目标的所有框的得分平均值 */
	int count;           /* 目标的所有框的个数       */
						 //INT id;
	}
XRECT;

int CPM_FaceDetect(const void* cas, int height, int width, const unsigned char* data, int datastep,
	const unsigned char* mask, int maskstep, double sc, double ssmin, double ssmax, double ss, int stepxy,
	double thd, int mincnt, XRECT* B, int B_len);

#endif

// Useful utility for creating directories for storing the output files
void create_directory_from_file(const char* output_path)
{

  // Creating the right directory structure

  // First get rid of the file
  char buf[256];
	char* p = parent_path(output_path, buf, 256);

  if (!*p && !path_exists(p)) {
    bool success = mycreate_directories(p);

    if (!success) {
      printf("Failed to create a directory... %s\n", p);
    }
  }
}

// Useful utility for creating directories for storing the output files
bool create_directories(const char* output_path)
{

  // Creating the right directory structure

  // First get rid of the file
  const char* p = output_path;
  bool success = NULL;

  if (!*p && !path_exists(p)) {
    success  = mycreate_directories(p);

    if (!success) {
      printf("Failed to create a directory... %s\n", p);
    }
  }

  return success;
}

// Extracting the following command line argv -f, -fd, -op, -of, -ov (and possible ordered repetitions)
void get_video_input_output_params(char*** input_video_files, char*** depth_dirs,
    char*** output_pose_files, char*** output_video_files, char*** output_2d_landmark_files,
    char*** output_3D_landmark_files, bool& world_coordinates, char** argv, int argc)
{
  size_t i;
  // By default use rotation with respect to camera (not world coordinates)
  world_coordinates = NULL;

  char* root = "";

  // First check if there is a root argument (so that videos and outputs could be defined more easilly)
  for (i = 0; i < argc; ++i) {
    if (stricmp(argv[i],"-root") == 0) {
      root = argv[i + 1];
      // Do not discard root as it might be used in other later steps
      i++;
    }
  }

  for (i = 0; i < argc; ++i) {
    if (stricmp(argv[i],"-f") == 0) {
      pstrcat2(ppstrnew(input_video_files), root, argv[i + 1]);
      argv[i] = NULL;
      argv[i + 1] = NULL;
      i++;
    }
    else if (stricmp(argv[i],"-fd") == 0) {
      pstrcat2(ppstrnew(depth_dirs), root, argv[i + 1]);
      argv[i] = NULL;
      argv[i + 1] = NULL;
      i++;
    }
    else if (stricmp(argv[i],"-op") == 0) {
      char* str = pstrcat2(ppstrnew(output_pose_files), root, argv[i + 1]);
      create_directory_from_file(str);
      argv[i] = NULL;
      argv[i + 1] = NULL;
      i++;
    }
    else if (stricmp(argv[i],"-of") == 0) {
      char* str = pstrcat2(ppstrnew(output_2d_landmark_files), root, argv[i + 1]);
      create_directory_from_file(str);
      argv[i] = NULL;
      argv[i + 1] = NULL;
      i++;
    }
    else if (stricmp(argv[i],"-of3D") == 0) {
      char* str = pstrcat2(ppstrnew(output_3D_landmark_files), root, argv[i + 1]);
      create_directory_from_file(str);
      argv[i] = NULL;
      argv[i + 1] = NULL;
      i++;
    }
    else if (stricmp(argv[i],"-ov") == 0) {
      char* str = pstrcat2(ppstrnew(output_video_files), root, argv[i + 1]);
      create_directory_from_file(str);
      argv[i] = NULL;
      argv[i + 1] = NULL;
      i++;
    }
    else if (stricmp(argv[i],"-world_coord") == 0) {
      world_coordinates = true;
    }
    else if (stricmp(argv[i],"-help") == 0) {
      printf("Input output files are defined as: -f <infile> -fd <indepthdir> -op <outpose> -of <outfeatures> -ov <outvideo>\n");
	  // Inform the user of how to use the program
    }
  }
}

void get_camera_params(int& device, float& fx, float& fy, float& cx, float& cy, char** argv, int argc)
{
  int i;
  for (i = 0; i < argc; ++i) {
    if (!argv[i]) continue;
    if (stricmp(argv[i],"-fx") == 0) {
      fx  = atof(argv[i + 1]);
      argv[i] = NULL;
      argv[i + 1] = NULL;
      i++;
    }
    else if (stricmp(argv[i],"-fy") == 0) {
      fy = atof(argv[i + 1]);
      argv[i] = NULL;
      argv[i + 1] = NULL;
      i++;
    }
    else if (stricmp(argv[i],"-cx") == 0) {
      cx = atof(argv[i + 1]);
      argv[i] = NULL;
      argv[i + 1] = NULL;
      i++;
    }
    else if (stricmp(argv[i],"-cy") == 0) {
      cy = atof(argv[i + 1]);
      argv[i] = NULL;
      argv[i + 1] = NULL;
      i++;
    }
    else if (stricmp(argv[i],"-device") == 0) {
      device = atoi(argv[i + 1]);
      argv[i] = NULL;
      argv[i + 1] = NULL;
      i++;
    }
    else if (stricmp(argv[i],"-help") == 0) {
      printf("Camera parameters are defined as: -device <webcam number> -fx <float focal length x> -fy <float focal length y> -cx <float optical center x> -cy <float optical center y> \n");
	  // Inform the user of how to use the program
    }
  }

}

struct image_input_output_params {
  char* input_image_files;
	char* input_depth_files;
  char* output_feature_files;
	char* output_pose_files;
  char* output_image_files;
  CRectd input_bounding_boxes;
};

#if 0
void get_image_input_output_params(image_input_output_params* params, int maxlen, char** argv, int argc, const char* ext)
{
  char* out_pts_dir = NULL, *out_pose_dir = NULL, *out_img_dir = NULL;
  int i, len=0;
  for (i = 0; i < argc && len<maxlen; ++i) {
    if (stricmp(argv[i],"-f") == 0) {
      pstrcat(ppstrnew(input_image_files), argv[i + 1]);
      argv[i] = NULL;
      argv[i + 1] = NULL;
      i++;
    }
    else if (stricmp(argv[i],"-fd") == 0) {
      pstrcat(ppstrnew(input_depth_files), argv[i + 1]);
      argv[i] = NULL;
      argv[i + 1] = NULL;
      i++;
    }
    else if (stricmp(argv[i],"-fdir") == 0) {

      // parse the -fdir directory by reading in all of the .png and .jpg files in it
      char* image_directory = (argv[i + 1]);
        // does the file exist and is it a directory
      if (path_exists(image_directory) && path_is_directory(image_directory)) {
        
        char** file_in_directory = NULL;
        dirlist(image_directory, &file_in_directory);
        int len = ppstrlen(file_in_directory);
        // Sort the images in the directory first
#define STRLESS(a, b)   (strcmp(a, b)<0)
        QSORT(file_in_directory, file_in_directory+len, STRLESS, char*);
#undef STRLESS
        int i;
        for (i=0; i<len; ++i) {
          char* file_iterator = file_in_directory[i];
          // Possible image extension .jpg and .png
          bool isok;
          //isok = extension(*file_iterator) == (".jpg") || extension(*file_iterator) == (".png") || extension(*file_iterator) == (".bmp");
          isok = 0==strcmp(extension(file_iterator), ext);
          if (isok) {
            pstrcat(ppstrnew(input_image_files), file_iterator);
            
            // If there exists a .txt file corresponding to the image, it is assumed that it contains a bounding box definition for a face
            // [minx, miny, maxx, maxy]
            char buf[256];
            char* current_file = file_iterator;
            char* bbox = replace_extension(current_file, "txt", buf, 256);
            
            // If there is a bounding box file push it to the list of bounding boxes
            if (path_exists(bbox)) {
              
              FILE* in_bbox = fopen(bbox, "rb");
              if (in_bbox) {
                double min_x, min_y, max_x, max_y;
                fscanf(in_bbox, "%f %f %f %f", &min_x, &min_y, &max_x, &max_y);
                fclose(in_bbox);
                ASSERT(0);
                //input_bounding_boxes.push_back(cRectd(min_x, min_y, max_x - min_x, max_y - min_y));
              }
            }
          }
        }
      }
      
      argv[i] = NULL;
      argv[i + 1] = NULL;
      i++;
    }
    else if (stricmp(argv[i],"-ofdir") == 0) {
      out_pts_dir = argv[i + 1];
      create_directories(out_pts_dir);
      argv[i] = NULL;
      argv[i + 1] = NULL;
      i++;
    }
    else if (stricmp(argv[i],"-opdir") == 0) {
      const char* out_pose_dir = argv[i + 1];
      create_directories(out_pose_dir);
      argv[i] = NULL;
      argv[i + 1] = NULL;
      i++;
    }
    else if (stricmp(argv[i],"-oidir") == 0) {
      out_img_dir = argv[i + 1];
      create_directories(out_img_dir);
      argv[i] = NULL;
      argv[i + 1] = NULL;
      i++;
    }
    else if (stricmp(argv[i],"-op") == 0) {
      pstrcat(ppstrnew(output_pose_files), argv[i + 1]);
      argv[i] = NULL;
      argv[i + 1] = NULL;
      i++;
    }
    else if (stricmp(argv[i],"-of") == 0) {
      pstrcat(ppstrnew(output_feature_files), argv[i + 1]);
      argv[i] = NULL;
      argv[i + 1] = NULL;
      i++;
    }
    else if (stricmp(argv[i],"-oi") == 0) {
      pstrcat(ppstrnew(output_image_files), argv[i + 1]);
      argv[i] = NULL;
      argv[i + 1] = NULL;
      i++;
    }
    else if (stricmp(argv[i],"-help") == 0) {
      printf("Input output files are defined as: -f <infile (can have multiple ones)> -of <where detected landmarks should be stored(can have multiple ones)> -oi <where should images with detected landmarks should be written (can have multiple ones)> -fdir <the directory containing .png and .jpg files to be processed (with optional .txt files corresponding to EACH image containing the bounding boxes> \n");
	  // Inform the user of how to use the program
    }
  }
#define strempty(s)  (NULL==(s) || 0==(*(s)))
  // If any output directories are defined populate them based on image names
  if (!strempty(out_img_dir)) {
    int len = ppstrlen(*input_image_files);
    for (i = 0; i < len; ++i) {
      char* image_loc = (*input_image_files)[i];
      char* fname = filename(image_loc);
      char buf[256];
      fname = replace_extension(fname, "jpg", buf, 256);
      pstrcat3(ppstrnew(output_image_files), out_img_dir, "/", fname);
    }

    if (!strempty(input_image_files)) {
      create_directory_from_file((*output_image_files)[0]);
    }
  }

  int input_image_files_size = ppstrlen(*input_image_files);
  if (!strempty(out_pts_dir) && input_image_files_size > 0) {
    for (i = 0; i < input_image_files_size; ++i) {
      char* image_loc = (*input_image_files)[i];
      char* fname = filename(image_loc);
      char buf[256];
      fname = replace_extension(fname, "pts", buf, 256);
      pstrcat3(ppstrnew(output_feature_files), out_pts_dir, "/", fname);
    }

    create_directory_from_file((*output_feature_files)[0]);
  }

  if (!strempty(out_pose_dir)) {
    int input_image_files_size = ppstrlen(*input_image_files);
    for (size_t i = 0; i < input_image_files_size; ++i) {
      char* image_loc = (*input_image_files)[i];
      char* fname = filename(image_loc);
      char buf[256];
      fname = replace_extension(fname, "pose", buf, 256);
      pstrcat3(ppstrnew(output_pose_files), out_pose_dir, "/", fname);
    }

    create_directory_from_file((*output_pose_files)[0]);
  }

  // Make sure the same number of images and bounding boxes is present, if any bounding boxes are defined
  //if (input_bounding_boxes.size() > 0) {    assert(input_bounding_boxes.size() == input_image_files.size());  }
}
#endif

//===========================================================================
// Fast patch expert response computation (linear model across a ROI) using normalised cross-correlation
//===========================================================================

void crossCorr_m(const Mat& img, Mat& img_dft, const Mat& _templ, map<int, Mat >& _templ_dfts, Mat& corr)
{
  // Our model will always be under min block size so can ignore this
  //const double blockScale = 4.5;
  //const int minBlockSize = 256;
  
  int maxDepth = CV_64F;

  Size dftsize;

  dftsize.width = getOptimalDFTSize1(corr.cols + _templ.cols - 1);
  dftsize.height = getOptimalDFTSize1(corr.rows + _templ.rows - 1);

  // Compute block size
  Size blocksize;
  blocksize.width = dftsize.width - _templ.cols + 1;
  blocksize.width = MIN(blocksize.width, corr.cols);
  blocksize.height = dftsize.height - _templ.rows + 1;
  blocksize.height = MIN(blocksize.height, corr.rows);

  Mat dftTempl;

  // if this has not been precomputed, precompute it, otherwise use it
  if (_templ_dfts.find(dftsize.width) == _templ_dfts.end()) {
    dftTempl.create(dftsize.height, dftsize.width, CV_64F);
	dftTempl.setTo(0);

    if (dftTempl.data != _templ.data) {
      convertTo(_templ, dftTempl, 0, 0, _templ.cols, _templ.rows, dftTempl.depth());
    }
	//CHECK_OR_PUT(_templ);

	img_t dst_[1];
	cvGetSubRect1(&dftTempl, dst_, 0, 0, dftsize.width, dftsize.height);

	//CHECK_OR_PUT(dst_);

    // Perform DFT of the template

	cvDFT(dst_, dst_, 0, _templ.rows);

    _templ_dfts[dftsize.width] = dftTempl;

  }
  else {
    // use the precomputed version
    dftTempl = _templ_dfts.find(dftsize.width)->second;
  }
  //CHECK_OR_PUT(dftTempl);

  Size bsz;
  bsz.width = MIN(blocksize.width, corr.cols);
  bsz.height = MIN(blocksize.height, corr.rows);

  Mat dftImg;

  if (img_dft.empty()) {
    dftImg.create(dftsize, CV_64F);
    dftImg.setTo(0.0);

	Size dsz;
	dsz.width = bsz.width + _templ.cols - 1;
	dsz.height = bsz.height + _templ.rows - 1;

    int x2 = MIN(img.cols, dsz.width);
    int y2 = MIN(img.rows, dsz.height);

	//Mat src0(img, Range(0, y2), Range(0, x2));
	img_t src0[1]; cvGetSubRect1(&img, src0, 0, 0, x2, y2);
	//img_t dst[1]; cvGetSubRect1(&dftImg_, dst, 0, 0, dsz.width, dsz.height);
	img_t dst1[1]; cvGetSubRect1(&dftImg, dst1, 0, 0, x2, y2);

    if (dst1->data != src0->data) {
      cvConvertTo(src0, dst1, dst1->tid);
    }

	//CHECK_OR_PUT(dst1);
	cvDFT(&dftImg, &dftImg, 0, dsz.height);
	//CHECK_OR_PUT(dftImg_);
	//CHECK_OR_PUT(dftImg);

    img_dft = dftImg.clone();
  }

  Mat dftTempl1 = mi(dftTempl, cRect(0, 0, dftsize.width, dftsize.height));
  mulSpectrums(img_dft, dftTempl1, dftImg, 0, true);
  //CHECK_OR_PUT(img_dft);
  //CHECK_OR_PUT(dftTempl1);
  //CHECK_OR_PUT(dftImg);

  cvDFT(&dftImg, &dftImg, CC_DXT_INVERSE + CC_DXT_SCALE, bsz.height);
  img_t src[1];
  img_t cdst[1];
  cvGetSubRect1(&dftImg, src, 0, 0, bsz.width, bsz.height);
  cvGetSubRect1(&corr, cdst, 0, 0, bsz.width, bsz.height);
  cvConvertTo(src, cdst, CC_32F);
}

void crossCorr_m(const img_t* img, Mat& img_dft, const img_t* _templ, map<int, Mat >& _templ_dfts, img_t* corr)
{
	// Our model will always be under min block size so can ignore this
	//const double blockScale = 4.5;
	//const int minBlockSize = 256;

	int maxDepth = CV_64F;

	Size dftsize;

	dftsize.width = getOptimalDFTSize(corr->cols + _templ->cols - 1);
	dftsize.height = getOptimalDFTSize(corr->rows + _templ->rows - 1);

	// Compute block size
	Size blocksize;
	blocksize.width = dftsize.width - _templ->cols + 1;
	blocksize.width = MIN(blocksize.width, corr->cols);
	blocksize.height = dftsize.height - _templ->rows + 1;
	blocksize.height = MIN(blocksize.height, corr->rows);
	IM_BEGIN(im, 10);
	img_t* dftTempl = im++;
	IM_DEF(tmp, 10);

	// if this has not been precomputed, precompute it, otherwise use it
	if (_templ_dfts.find(dftsize.width) == _templ_dfts.end()) {
		img_t* tmp1 = tmp;
		cvSetZero(dftTempl, dftsize.height, dftsize.width, CC_64F);

		const img_t* src = _templ;

		// TODO simplify no need for rect?
		img_t* dst = cvGetSubRect1(dftTempl, tmp1++, 0, 0, dftsize.width, dftsize.height);
		img_t* dst1 = cvGetSubRect1(dftTempl, tmp1++, 0, 0, _templ->cols, _templ->rows);

		if (dst1->data != src->data) {
			cvConvertTo(src, dst1, dst1->tid);
		}

		if (dst->cols > _templ->cols) {
			//Mat part(dst, Range(0, _templ.rows), Range(_templ.cols, dst.cols));
			img_t* part = cvGetSubRect1(dst, tmp1++, _templ->cols, 0, dst->cols - _templ->cols, _templ->rows);
			cvZeroMat(part);
		}

		// Perform DFT of the template

		cvDFT(dst, dst, 0, _templ->rows);

		_templ_dfts[dftsize.width] = ma(dftTempl);

	}
	else {
		// use the precomputed version
		img_t* tt = &(_templ_dfts.find(dftsize.width)->second);
		cvCloneMat(dftTempl, tt);
	}
	CHECK_OR_PUT(dftTempl);

	Size bsz;
	bsz.width = MIN(blocksize.width, corr->cols);
	bsz.height = MIN(blocksize.height, corr->rows);
	img_t* src = NULL;

	img_t* cdst = cvGetSubRect1(corr, tmp++, 0, 0, bsz.width, bsz.height);

	img_t* dftImg = im++;

	if (img_dft.empty()) {
		img_t* tmp1 = tmp;
		cvSetZero(dftImg, dftsize.height, dftsize.width, CC_64F);

		Size dsz;
		dsz.width = bsz.width + _templ->cols - 1;
		dsz.height = bsz.height + _templ->rows - 1;

		int x2 = MIN(img->cols, dsz.width);
		int y2 = MIN(img->rows, dsz.height);

		//Mat src0(img, Range(0, y2), Range(0, x2));
		img_t* src0 = cvGetSubRect1(img, tmp1++, 0, 0, x2, y2);
		img_t* dst = cvGetSubRect1(dftImg, tmp1++, 0, 0, dsz.width, dsz.height);
		img_t* dst1 = cvGetSubRect1(dftImg, tmp1++, 0, 0, x2, y2);

		src = src0;

		if (dst1->data != src->data) {
			cvConvertTo(src, dst1, dst1->tid);
		}

		CHECK_OR_PUT(dftImg);

		cvDFT(dftImg, dftImg, 0, dsz.height);

		//cvCloneMat(img_dft, dftImg);
		img_dft = ma(dftImg);
	}

	img_t* dftTempl1 = cvGetSubRect1(dftTempl, tmp++, 0, 0, dftsize.width, dftsize.height);
	//mulSpectrums(img_dft, dftTempl1, dftImg, 0, true);
	cvMulSpectrums(&img_dft, dftTempl1, dftImg, 0);

	cvDFT(dftImg, dftImg, CC_DXT_INVERSE + CC_DXT_SCALE, bsz.height);
	src = cvGetSubRect1(dftImg, tmp++, 0, 0, bsz.width, bsz.height);

	cvConvertTo(src, cdst, CC_32F);
	IM_END(im);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

void matchTemplate_m(const Mat& input_img, Mat& img_dft, Mat& _integral_img, Mat& _integral_img_sq, const Mat&  templ, map<int, Mat >& templ_dfts, Mat& result, int method)
{

  int numType = method == CV_TM_CCORR || method == CV_TM_CCORR_NORMED ? 0 :
      method == CV_TM_CCOEFF || method == CV_TM_CCOEFF_NORMED ? 1 : 2;
  bool isNormed = method == CV_TM_CCORR_NORMED ||
      method == CV_TM_SQDIFF_NORMED ||
      method == CV_TM_CCOEFF_NORMED;

  // Assume result is defined properly
  if (result.empty()) {
	  Size corrSize;
	  corrSize.width = input_img.cols - templ.cols + 1;
	  corrSize.height = input_img.rows - templ.rows + 1;
    result.create(corrSize, CV_32F);
  }
#if 1
  crossCorr_m(input_img, img_dft, templ, templ_dfts, result);
#else
  img_t input_img_ = mi(input_img);
  img_t templ_ = mi(templ);
  img_t result_ = mi(result);
  crossCorr_m(&input_img_, img_dft, &templ_, templ_dfts, &result_);
#endif
  //CHECK_OR_PUT(input_img);
  //CHECK_OR_PUT(img_dft);

  if (method == CV_TM_CCORR) {
    return;
  }

  double invArea = 1. / ((double)templ.rows * templ.cols);

  Mat sum, sqsum;
  CScalar templMean = { 0 }, templSdv = {0};
  double* q0 = 0, *q1 = 0, *q2 = 0, *q3 = 0;
  double templNorm = 0, templSum2 = 0;

  if (method == CV_TM_CCOEFF) {
    // If it has not been precomputed compute it now
    if (_integral_img.empty()) {
      integral(input_img, _integral_img, CV_64F);
    }

    sum = _integral_img;

    templMean = mean(&templ);
  }
  else {
    // If it has not been precomputed compute it now
    if (_integral_img.empty()) {
      integral(input_img, _integral_img, _integral_img_sq, CV_64F);
    }

    sum = _integral_img;
    sqsum = _integral_img_sq;
    meanStdDev(&templ, templMean, templSdv);

    templNorm = templSdv[0] * templSdv[0] + templSdv[1] * templSdv[1] + templSdv[2] * templSdv[2] + templSdv[3] * templSdv[3];

    if (templNorm < DBL_EPSILON && method == CV_TM_CCOEFF_NORMED) {
      result.setTo(1.0);
      return;
    }

    templSum2 = templNorm + templMean[0] * templMean[0] + templMean[1] * templMean[1] + templMean[2] * templMean[2] + templMean[3] * templMean[3];

    if (numType != 1) {
      templMean = cScalarAll(0);
      templNorm = templSum2;
    }

    templSum2 /= invArea;
    templNorm = sqrt(templNorm);
    templNorm /= sqrt(invArea); // care of accuracy here

    q0 = (double*)sqsum.data;
    q1 = q0 + templ.cols;
    q2 = (double*)(sqsum.data + templ.rows * sqsum.step);
    q3 = q2 + templ.cols;
  }

  double* p0 = (double*)sum.data;
  double* p1 = p0 + templ.cols;
  double* p2 = (double*)(sum.data + templ.rows * sum.step);
  double* p3 = p2 + templ.cols;

  int sumstep = sum.data ? (int)(sum.step / sizeof(double)) : 0;
  int sqstep = sqsum.data ? (int)(sqsum.step / sizeof(double)) : 0;

  int i, j;
  //CHECK_OR_PUT(result);

  for (i = 0; i < result.rows; i++) {
    //float* rrow = result.ptr<float>[i];
    float* rrow = ((float*)result.data) + i*result.cols;
    int idx = i * sumstep;
    int idx2 = i * sqstep;

    for (j = 0; j < result.cols; j++, idx += 1, idx2 += 1) {
      double num = rrow[j], t;
      double wndMean2 = 0, wndSum2 = 0;

      if (numType == 1) {

        t = p0[idx] - p1[idx] - p2[idx] + p3[idx];
        wndMean2 += t * t;
        num -= t * templMean[0];

        wndMean2 *= invArea;
      }

      if (isNormed || numType == 2) {

        t = q0[idx2] - q1[idx2] - q2[idx2] + q3[idx2];
        wndSum2 += t;

        if (numType == 2) {
          num = wndSum2 - 2 * num + templSum2;
          num = MAX(num, 0.);
        }
      }

      if (isNormed) {
        t = sqrt(MAX(wndSum2 - wndMean2, 0)) * templNorm;

        if (fabs(num) < t) {
          num /= t;
        }
        else if (fabs(num) < t * 1.125) {
          num = num > 0 ? 1 : -1;
        }
        else {
          num = method != CV_TM_SQDIFF_NORMED ? 0 : 1;
        }
      }

      rrow[j] = (float)num;
    }
  }
}

//===========================================================================
// Point set and landmark manipulation functions
//===========================================================================
// Using Kabsch's algorithm for aligning shapes
//This assumes that align_from and align_to are already mean normalised
Matx22d AlignShapesKabsch2D(const Mat& align_from, const Mat& align_to)
{
  SVD svd(align_from.t() * align_to);

  // make sure no reflection is there
  // corr ensures that we do only rotaitons and not reflections
  double d = determinant(svd.vt.t() * svd.u.t());

  Matx22d corr = Matx22d::eye();

  if (d > 0) {
    corr(1, 1) = 1;
  }
  else {
    corr(1, 1) = -1;
  }

  Matx22d R;
  Mat(svd.vt.t()*Mat(corr)*svd.u.t()).copyTo(R);
  return R;
}

Matx22d AlignShapesKabsch2D(const img_t* align_from, const img_t* align_to)
{
	Matx22d R;
	IM_BEGIN(im, 10);
	IM_DEF(tmp, 10);
	img_t* w=im++, *u = im++, *vt = im++, *ut=im++, *v=im++;
	img_t* aa = cvMatMul(cvT(align_from, im+0), align_to, im+1);
	SVD_compute(aa, w, u, vt, CC_SVD_MODIFY_A);

	// make sure no reflection is there
	// corr ensures that we do only rotaitons and not reflections
	double d = determinant(cvMatMul(cvT(vt, v), cvT(u, ut), im));

	Matx22d corr = Matx22d::eye();

	if (d > 0) {
		corr(1, 1) = 1;
	}
	else {
		corr(1, 1) = -1;
	}

	img_t* bb = cvMatMul(v, cvMat(tmp, 2, 2, CC_64F, 1, corr.val), im + 0);
	cvMatMul(bb, ut, cvMat(tmp+1, 2, 2, CC_64F, 1, R.val));
	IM_END(im);
	return R;
}
//=============================================================================
// Basically Kabsch's algorithm but also allows the collection of points to be different in scale from each other
Matx22d AlignShapesWithScale1(const Mat& src, const Mat& dst)
{
  int n = src.rows;

  Mat src_col0 = src.col(0);
  Scalar ss = sum(src_col0);
  double* aa = (double*)src_col0.data;
  int i;
  double ss1 = 0;
  for (i = 0; i < 68; ++i) {
	  ss1 += aa[i];
  }
  CHECK_OR_PUT(src_col0);
  // First we mean normalise both src and dst
  double mean_src_x = mean(src.col(0))[0];
  double mean_src_y = mean(src.col(1))[0];

  double mean_dst_x = mean(dst.col(0))[0];
  double mean_dst_y = mean(dst.col(1))[0];

  Mat src_mean_normed = src.clone();
  src_mean_normed.col(0) = src_mean_normed.col(0) - mean_src_x;
  src_mean_normed.col(1) = src_mean_normed.col(1) - mean_src_y;

  Mat dst_mean_normed = dst.clone();
  dst_mean_normed.col(0) = dst_mean_normed.col(0) - mean_dst_x;
  dst_mean_normed.col(1) = dst_mean_normed.col(1) - mean_dst_y;

  CHECK_OR_PUT(dst_mean_normed);
  // Find the scaling factor of each
  Mat src_sq;
  pow(src_mean_normed, 2, src_sq);

  Mat dst_sq;
  pow(dst_mean_normed, 2, dst_sq);

  double s_src = sqrt(sum(src_sq)[0] / n);
  double s_dst = sqrt(sum(dst_sq)[0] / n);

  src_mean_normed = src_mean_normed / s_src;
  dst_mean_normed = dst_mean_normed / s_dst;

  double s = s_dst / s_src;

  // Get the rotation
  Matx22d R;
#ifdef _USE_OPENCV
  R = AlignShapesKabsch2D(src_mean_normed, dst_mean_normed);
#else

  void AlignShapesKabsch2D(const img_t* align_from, const img_t* align_to, double4 R);
  AlignShapesKabsch2D(&mi(src_mean_normed), &mi(dst_mean_normed), R.val);

#endif

  Matx22d A;
  Mat(s * R).copyTo(A);

  Mat aligned = (Mat(Mat(A) * src.t())).t();
  Mat offset = dst - aligned;

  double t_x =  mean(offset.col(0))[0];
  double t_y =  mean(offset.col(1))[0];

  return A;
}
Matx22d AlignShapesWithScale(const img_t* src, const img_t* dst)
{
	Matx22d A;
	IM_BEGIN(im, 10);
	IM_DEF(tmp, 10);
	int n = src->rows;

	// First we mean normalise both src and dst
	double mean_src_x = mean(cvGetCol(src, tmp, 0))[0];
	double mean_src_y = mean(cvGetCol(src, tmp, 1))[0];

	double mean_dst_x = mean(cvGetCol(dst, tmp, 0))[0];
	double mean_dst_y = mean(cvGetCol(dst, tmp, 1))[0];

	img_t* src_mean_normed = cvCloneMat(im++, src);
	//src_mean_normed.col(0) = src_mean_normed.col(0) - mean_src_x;
	//src_mean_normed.col(1) = src_mean_normed.col(1) - mean_src_y;
	img_t* src_mean_normed_col;
	src_mean_normed_col = cvGetCol(src_mean_normed, tmp, 0);
	cvSubS_C1(src_mean_normed_col, mean_src_x, src_mean_normed_col, NULL);
	src_mean_normed_col = cvGetCol(src_mean_normed, tmp, 1);
	cvSubS_C1(src_mean_normed_col, mean_src_y, src_mean_normed_col, NULL);

	img_t* dst_mean_normed = cvCloneMat(im++, dst);
	//dst_mean_normed.col(0) = dst_mean_normed.col(0) - mean_dst_x;
	//dst_mean_normed.col(1) = dst_mean_normed.col(1) - mean_dst_y;
	img_t *dst_mean_normed_col;
	dst_mean_normed_col = cvGetCol(src_mean_normed, tmp, 0);
	cvSubS_C1(dst_mean_normed_col, mean_dst_x, dst_mean_normed_col, NULL);
	dst_mean_normed_col = cvGetCol(src_mean_normed, tmp, 1);
	cvSubS_C1(dst_mean_normed_col, mean_dst_y, dst_mean_normed_col, NULL);

	//CHECK_OR_PUT(dst_mean_normed);
	// Find the scaling factor of each
	img_t* src_sq =	cvPow(src_mean_normed, 2, im++);
	img_t* dst_sq = cvPow(dst_mean_normed, 2, im++);

	double s_src = sqrt(sum(src_sq)[0] / n);
	double s_dst = sqrt(sum(dst_sq)[0] / n);

	//src_mean_normed = src_mean_normed / s_src;
	//dst_mean_normed = dst_mean_normed / s_dst;
	cvMulS(src_mean_normed, 1. / s_src, src_mean_normed);
	cvMulS(dst_mean_normed, 1. / s_dst, dst_mean_normed);

	double s = s_dst / s_src;

	// Get the rotation
	Matx22d R;
#ifdef _USE_OPENCV
	R = AlignShapesKabsch2D(src_mean_normed, dst_mean_normed);
#else

	void AlignShapesKabsch2D(const img_t* align_from, const img_t* align_to, double4 R);
	AlignShapesKabsch2D(&mi(src_mean_normed), &mi(dst_mean_normed), R.val);

#endif
	Mat R_(R);
	//CHECK_OR_PUT(R_);
	scale4(A.val, R.val, s);
	//Mat(s * R).copyTo(A);

	Mat aligned = (Mat(Mat(A) * ma(src).t())).t();
	Mat offset = ma(dst) - aligned;

	//CHECK_OR_PUT(aligned);
	double t_x = mean(offset.col(0))[0];
	double t_y = mean(offset.col(1))[0];

	IM_END(im);
	return A;
}


//===========================================================================
// Visualisation functions
//===========================================================================
double* Project(double* projected, const double* mData, int num_points, double fx, double fy, double cx, double cy)
{
  double X, Y, Z;

  for (int i = 0; i < num_points; i++) {
    // Get the points
    X = mData[i*3+0];
    Y = mData[i*3+1];
    Z = mData[i*3+2];

    double x;
    double y;

    // if depth is 0 the projection is different
    if (Z != 0) {
      x = ((X * fx / Z) + cx);
      y = ((Y * fy / Z) + cy);
    }
    else {
      x = X;
      y = Y;
    }

    // Project and store in dest matrix
    projected[i*2+0] = x;
    projected[i*2+1] = y;
  }
  return projected;
}

// Using the XYZ convention R = Rx * Ry * Rz, left-handed positive sign
Matx33d Euler2RotationMatrix(const Vec3d& eulerAngles)
{
	Matx33d rotation_matrix;

	double s1 = sin(eulerAngles[0]);
	double s2 = sin(eulerAngles[1]);
	double s3 = sin(eulerAngles[2]);

	double c1 = cos(eulerAngles[0]);
	double c2 = cos(eulerAngles[1]);
	double c3 = cos(eulerAngles[2]);

	rotation_matrix(0, 0) = c2 * c3;
	rotation_matrix(0, 1) = -c2 * s3;
	rotation_matrix(0, 2) = s2;
	rotation_matrix(1, 0) = c1 * s3 + c3 * s1 * s2;
	rotation_matrix(1, 1) = c1 * c3 - s1 * s2 * s3;
	rotation_matrix(1, 2) = -c2 * s1;
	rotation_matrix(2, 0) = s1 * s3 - c1 * c3 * s2;
	rotation_matrix(2, 1) = c3 * s1 + c1 * s2 * s3;
	rotation_matrix(2, 2) = c1 * c2;

	return rotation_matrix;
}

void DrawBox(img_t* image, Vec6d pose, CScalar color, int thickness, float fx, float fy, float cx, float cy)
{
  double boxVerts[] = { -1, 1, -1,
      1, 1, -1,
      1, 1, 1,
      -1, 1, 1,
      1, -1, 1,
      1, -1, -1,
      -1, -1, -1,
      -1, -1, 1
                      };

#if 0
  vector<pair<int, int> > edges;
  edges.push_back(pair<int, int>(0, 1));
  edges.push_back(pair<int, int>(1, 2));
  edges.push_back(pair<int, int>(2, 3));
  edges.push_back(pair<int, int>(0, 3));
  edges.push_back(pair<int, int>(2, 4));
  edges.push_back(pair<int, int>(1, 5));
  edges.push_back(pair<int, int>(0, 6));
  edges.push_back(pair<int, int>(3, 7));
  edges.push_back(pair<int, int>(6, 5));
  edges.push_back(pair<int, int>(5, 4));
  edges.push_back(pair<int, int>(4, 7));
  edges.push_back(pair<int, int>(7, 6));
#else
  int edges[][2] = {
  {0, 1},
  {1, 2},
  {2, 3},
  {0, 3},
  {2, 4},
  {1, 5},
  {0, 6},
  {3, 7},
  {6, 5},
  {5, 4},
  {4, 7},
  {7, 6},

  };
#endif

  // The size of the head is roughly 200mm x 200mm x 200mm
  Mat box = Mat(8, 3, CV_64F, boxVerts) * 100;

  Matx33d rot = Euler2RotationMatrix(Vec3d(pose[3], pose[4], pose[5]));
  Mat rotBox;

  // Rotate the box
  rotBox = Mat(rot) * box.t();
  rotBox = rotBox.t();

  // Move the bounding box to head position
  rotBox.col(0) = rotBox.col(0) + pose[0];
  rotBox.col(1) = rotBox.col(1) + pose[1];
  rotBox.col(2) = rotBox.col(2) + pose[2];

  // draw the lines
  double rotBoxProj[8*2];
  Project(rotBoxProj, rotBox.tt.db, rotBox.rows, fx, fy, cx, cy);

  CRect image_rect = cRect(0, 0, image->cols, image->rows);

  for (size_t i = 0; i < countof(edges); ++i) {
    double* begin;
    double* end;

    begin = rotBoxProj + 2 * (edges[i][0]);
    end = rotBoxProj + 2* (edges[i][1]);

    CPoint p1 = cPoint((int)begin[0], (int)begin[1]);
    CPoint p2 = cPoint((int)end[0], (int)end[1]);

    // Only draw the line if one of the points is inside the image
    if (inside(p1, image_rect) || inside(p2, image_rect)) {
      cvLine(image, p1, p2, color, thickness);
    }

  }
}

#if 0
vector<pair<Point, Point>> CalculateBox(Vec6d pose, float fx, float fy, float cx, float cy)
{
  double boxVerts[] = { -1, 1, -1,
      1, 1, -1,
      1, 1, 1,
      -1, 1, 1,
      1, -1, 1,
      1, -1, -1,
      -1, -1, -1,
      -1, -1, 1
                      };

  vector<pair<int, int>> edges;
  edges.push_back(pair<int, int>(0, 1));
  edges.push_back(pair<int, int>(1, 2));
  edges.push_back(pair<int, int>(2, 3));
  edges.push_back(pair<int, int>(0, 3));
  edges.push_back(pair<int, int>(2, 4));
  edges.push_back(pair<int, int>(1, 5));
  edges.push_back(pair<int, int>(0, 6));
  edges.push_back(pair<int, int>(3, 7));
  edges.push_back(pair<int, int>(6, 5));
  edges.push_back(pair<int, int>(5, 4));
  edges.push_back(pair<int, int>(4, 7));
  edges.push_back(pair<int, int>(7, 6));

  // The size of the head is roughly 200mm x 200mm x 200mm
  Mat box = Mat(8, 3, CV_64F, boxVerts).clone() * 100;

  Matx33d rot = Euler2RotationMatrix(cVec3d(pose[3], pose[4], pose[5]));
  Mat rotBox;

  // Rotate the box
  rotBox = Mat(rot) * box.t();
  rotBox = rotBox.t();

  // Move the bounding box to head position
  rotBox.col(0) = rotBox.col(0) + pose[0];
  rotBox.col(1) = rotBox.col(1) + pose[1];
  rotBox.col(2) = rotBox.col(2) + pose[2];

  // draw the lines
  Mat rotBoxProj;
  Project(rotBoxProj, rotBox, fx, fy, cx, cy);

  vector<pair<Point, Point>> lines;

  for (size_t i = 0; i < edges.size(); ++i) {
    Mat begin;
    Mat end;
    
    rotBoxProj.row(edges[i].first).copyTo(begin);
    rotBoxProj.row(edges[i].second).copyTo(end);
    
    Point p1, p2;
    p1.x = (int)begin.at<double>(0);
    p1.y = (int)begin.at<double>(1);
    p2.x = (int)end.at<double>(0);
    p2.y = (int)end.at<double>(1);
    
    lines.push_back(pair<Point, Point>(p1, p2));
  }

  return lines;
}

void DrawBox(vector<pair<Point, Point>> lines, Mat image, Scalar color, int thickness)
{
  Rect image_rect(0, 0, image.cols, image.rows);

  for (size_t i = 0; i < lines.size(); ++i) {
    Point p1 = lines.at[i].first;
    Point p2 = lines.at[i].second;

    // Only draw the line if one of the points is inside the image
    if (p1.inside(image_rect) || p2.inside(image_rect)) {
      line(image, p1, p2, color, thickness);
    }

  }
}
#endif

// Computing landmarks (to be drawn later possibly)
vector<Point2d> CalculateLandmarks(const Mat& shape2D, Mat& visibilities)
{
  int n = shape2D.rows / 2;
  vector<Point2d> landmarks;

  for (int i = 0; i < n; ++i) {
	  if (((int*)(visibilities.data))[i]) {
		  Point2d featurePoint;
		  featurePoint.x = ((double*)(shape2D.data))[i];
		  featurePoint.y = ((double*)(shape2D.data))[i + n];
		  landmarks.push_back(featurePoint);
	  }
  }

  return landmarks;
}

// Computing landmarks (to be drawn later possibly)
vector<Point2d> CalculateLandmarks1(Mat img, const Mat& shape2D)
{

  int n;
  vector<Point2d> landmarks;

  if (shape2D.cols == 2) {
    n = shape2D.rows;
  }
  else if (shape2D.cols == 1) {
    n = shape2D.rows / 2;
  }

  for (int i = 0; i < n; ++i) {
    Point2d featurePoint;
    
    if (shape2D.cols == 1) {
      featurePoint.x = ((double*)shape2D.data)[i];
      featurePoint.y = ((double*)shape2D.data)[i + n];
    }
    else {
      featurePoint.x = ((double*)shape2D.data)[shape2D.cols*i+0];
      featurePoint.y = ((double*)shape2D.data)[shape2D.cols*i+1];
    }

    landmarks.push_back(featurePoint);
  }

  return landmarks;
}

//=============================================================================
// Orthonormalising the 3x3 rotation matrix
void Orthonormalise(Matx33d& R)
{
	SVD svd(R, SVD::MODIFY_A);

	// get the orthogonal matrix from the initial rotation matrix
	Mat X = svd.u * svd.vt;

	// This makes sure that the handedness is preserved and no reflection happened
	// by making sure the determinant is 1 and not -1
	Mat W = Mat::eye(3, 3, CV_64F);
	double d = determinant(X);
	((double*)W.data)[2*3+2] = determinant(X);
	Mat Rt = svd.u * W * svd.vt;

	Rt.copyTo(R);

}

// Computing landmarks (to be drawn later possibly)
vector<Point2d> CalculateLandmarks(CLM& clm_model)
{

  int idx = clm_model.patch_experts.GetViewIdx(clm_model.params_global, 0);

  // Because we only draw visible points, need to find which points patch experts consider visible at a certain orientation
  return CalculateLandmarks(clm_model.detected_landmarks, clm_model.patch_experts.visibilities[0][idx]);

}

// Drawing landmarks on a face image
void Draw(img_t* img, const Mat& shape2D, const Mat& visibilities, double ss)
{
#if 1
  int n = shape2D.rows / 2;

  // Drawing feature points
  if (n >= 66) {
	  //img_t im[1] = { 0 };
	  //IMINIT(im, img.rows, img.cols, img.data, img.step, img.channels(), 1);
#if 1
	  if (1) {
		  for (int i = 0; i < n; ++i) {
        int* visibilities_ptr = img_ptr(int, &visibilities);
			  if (visibilities_ptr[i]) {
				  CPoint featurePoint;
				  featurePoint.x = (int)((double*)shape2D.data)[i]*ss;
				  featurePoint.y = (int)((double*)shape2D.data)[i + n]*ss;

				  // A rough heuristic for drawn point size
				  int thickness = (int)ceil(3.0 * ((double)img->cols) / 640.0);
				  int thickness_2 = (int)ceil(1.0 * ((double)img->cols) / 640.0);

				  cvCircle(img, featurePoint, 1, cScalar(0, 0, 255), thickness);
				  cvCircle(img, featurePoint, 1, cScalar(255, 0, 0), thickness_2);
				  //char buf[16];
				  //_snprintf(buf, 16, "%d", i);
				  //putText(img, buf, featurePoint, 0, 0.5, Scalar(255, 0, 0));
				  //pts[i * 2] = featurePoint.x;
				  //pts[i * 2+1] = featurePoint.y;
				  //imdrawaa_circle(im, NULL, featurePoint.x, featurePoint.y, thickness, _RGB(255, 0, 0), _RGB(0,0,255), 2);
			  }
		  }
	  }
#endif
	  if (1) {
		  float pts[200];
		  for (int i = 0; i < n; ++i) {
			  pts[i * 2 + 0] = ((double*)shape2D.data)[i]*ss;
			  pts[i * 2 + 1] = ((double*)shape2D.data)[i + n]*ss;
		  }
		  drawkonhon(img, pts);
	  }
  }
  else if (n == 28) { // drawing eyes
    for (int i = 0; i < n; ++i) {
		Point featurePoint;
		featurePoint.x = (int)((double*)shape2D.data)[i]*ss;
		featurePoint.y = (int)((double*)shape2D.data)[i + n]*ss;

      // A rough heuristic for drawn point size
      int thickness = 1.0;
      int thickness_2 = 1.0;

      int next_point = i + 1;

      if (i == 7) {
        next_point = 0;
      }

      if (i == 19) {
        next_point = 8;
      }

      if (i == 27) {
        next_point = 20;
      }

	  Point nextFeaturePoint;
	  nextFeaturePoint.x = (int)((double*)shape2D.data)[next_point]*ss;
	  nextFeaturePoint.y = (int)((double*)shape2D.data)[next_point + n]*ss;

	  if (i < 8 || i > 19) {
		  //cvLine(img, featurePoint, nextFeaturePoint, cScalar1(255, 0, 0), thickness_2);
	  }
	  else {
		  //cvLine(img, featurePoint, nextFeaturePoint, cScalar1(0, 0, 255), thickness_2);
	  }

      //circle(img, featurePoint, 1, Scalar(0,255,0), thickness);
      //circle(img, featurePoint, 1, Scalar(0,0,255), thickness_2);
    }
  }
  else if (n == 6) {
    for (int i = 0; i < n; ++i) {
		Point featurePoint;
		featurePoint.x = (int)((double*)shape2D.data)[i]*ss;
		featurePoint.y = (int)((double*)shape2D.data)[i + n]*ss;

      // A rough heuristic for drawn point size
      int thickness = 1.0;
      int thickness_2 = 1.0;

      //circle(img, featurePoint, 1, Scalar(0,255,0), thickness);
      //circle(img, featurePoint, 1, Scalar(0,0,255), thickness_2);

      int next_point = i + 1;

      if (i == 5) {
        next_point = 0;
      }

	  Point nextFeaturePoint;
	  nextFeaturePoint.x = (int)((double*)shape2D.data)[next_point]*ss;
	  nextFeaturePoint.x = (int)((double*)shape2D.data)[next_point + n]*ss;
      cvLine(img, featurePoint, nextFeaturePoint, cScalar1(255, 0, 0), thickness_2);
    }
  }
#endif
}

// Drawing landmarks on a face image
void Draw(img_t* img, const Mat& shape2D, double ss)
{
#if 1
  int n;

  if (shape2D.cols == 2) {
    n = shape2D.rows;
  }
  else if (shape2D.cols == 1) {
    n = shape2D.rows / 2;
  }

  for (int i = 0; i < n; ++i) {
    Point featurePoint;
    
    if (shape2D.cols == 1) {
      featurePoint.x = (int)((double*)shape2D.data)[i]*ss;
      featurePoint.y = (int)((double*)shape2D.data)[i + n]*ss;
    }
    else {
      featurePoint.x = (int)((double*)shape2D.data)[shape2D.cols*i+0]*ss;
      featurePoint.x = (int)((double*)shape2D.data)[shape2D.cols*i+1]*ss;
    }

    // A rough heuristic for drawn point size
    int thickness = (int)ceil(5.0 * ((double)img->cols) / 640.0);
    int thickness_2 = (int)ceil(1.5 * ((double)img->cols) / 640.0);

    cvCircle(img, featurePoint, 1, cScalar1(0, 0, 255), thickness);
    cvCircle(img, featurePoint, 1, cScalar1(255, 0, 0), thickness_2);

  }
#endif
}

// Drawing detected landmarks on a face image
void Draw(img_t* img, const CLM& clm_model, double ss)
{

  int idx = clm_model.patch_experts.GetViewIdx(clm_model.params_global, 0);

  // Because we only draw visible points, need to find which points patch experts consider visible at a certain orientation
  Draw(img, clm_model.detected_landmarks, clm_model.patch_experts.visibilities[0][idx], ss);

  if (0) {
	  //CPoint pt1 = cPoint(clm_model.rect.x, clm_model.rect.y);
	  //CPoint pt2 = cPoint(clm_model.rect.x + clm_model.rect.width, clm_model.rect.y + clm_model.rect.height);
	  //cvRectangle(img, pt1, pt2, cScalar(255, 0, 0), 2);
  }
  // If the model has hierarchical updates draw those too
  for (size_t i = 0; i < clm_model.hierarchical_models.size(); ++i) {
    if (clm_model.hierarchical_models[i].pdm.NumberOfPoints() != clm_model.hierarchical_mapping[i].size()) {
      Draw(img, clm_model.hierarchical_models[i], ss);
    }
  }
}

void DrawLandmarks(Mat img, vector<Point> landmarks)
{
#if 0
  for (Point p : landmarks) {
    // A rough heuristic for drawn point size
    int thickness = (int)ceil(5.0 * ((double)img.cols) / 640.0);
    int thickness_2 = (int)ceil(1.5 * ((double)img.cols) / 640.0);

    circle(img, p, 1, Scalar(0, 0, 255), thickness);
    circle(img, p, 1, Scalar(255, 0, 0), thickness_2);
  }
#endif
}

//===========================================================================
// Angle representation conversion helpers
//===========================================================================

// Using the XYZ convention R = Rx * Ry * Rz, left-handed positive sign
Vec3d RotationMatrix2Euler(const Matx33d& rotation_matrix)
{
  double q0 = sqrt(1 + rotation_matrix(0, 0) + rotation_matrix(1, 1) + rotation_matrix(2, 2)) / 2.0;
  double q1 = (rotation_matrix(2, 1) - rotation_matrix(1, 2)) / (4.0 * q0) ;
  double q2 = (rotation_matrix(0, 2) - rotation_matrix(2, 0)) / (4.0 * q0) ;
  double q3 = (rotation_matrix(1, 0) - rotation_matrix(0, 1)) / (4.0 * q0) ;

  double t1 = 2.0 * (q0 * q2 + q1 * q3);

  double yaw  = asin(2.0 * (q0 * q2 + q1 * q3));
  double pitch = atan2(2.0 * (q0 * q1 - q2 * q3), q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3);
  double roll = atan2(2.0 * (q0 * q3 - q1 * q2), q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3);

  return cVec3d(pitch, yaw, roll);
}

Vec3d Euler2AxisAngle(const Vec3d& euler)
{
  Matx33d rotMatrix = Euler2RotationMatrix(euler);
  Vec3d axis_angle;
#if 0
  Rodrigues(rotMatrix, axis_angle);
#else
  img_t mm = cvMat2(3, 3, CC_64F, 1, rotMatrix.val);
  img_t vv = cvMat2(3, 1, CC_64F, 1, axis_angle.val);
  cvRodrigues(&mm, &vv, NULL, CC_RODRIGUES_M2V);
#endif
  return axis_angle;
}

Vec3d AxisAngle2Euler(const Vec3d& axis_angle)
{
  Matx33d rotation_matrix;
#if 0
  Rodrigues(axis_angle, rotation_matrix);
#else
  img_t mm = cvMat2(3, 3, CC_64F, 1, rotation_matrix.val);
  img_t vv = cvMat2(3, 1, CC_64F, 1, axis_angle.val);
  cvRodrigues(&mm, &vv, NULL, CC_RODRIGUES_V2M);
#endif
  return RotationMatrix2Euler(rotation_matrix);
}

Matx33d AxisAngle2RotationMatrix(const Vec3d& axis_angle)
{
  Matx33d rotation_matrix;
#if 0
  Rodrigues(axis_angle, rotation_matrix);
#else
  img_t mm = cvMat2(3, 3, CC_64F, 1, rotation_matrix.val);
  img_t vv = cvMat2(3, 1, CC_64F, 1, axis_angle.val);
  cvRodrigues(&mm, &vv, NULL, CC_RODRIGUES_V2M);
#endif
  return rotation_matrix;
}

Vec3d RotationMatrix2AxisAngle(const Matx33d& rotation_matrix)
{
  Vec3d axis_angle;
#if 0
  Rodrigues(rotation_matrix, axis_angle);
#else
  img_t mm = cvMat2(3, 3, CC_64F, 1, rotation_matrix.val);
  img_t vv = cvMat2(3, 1, CC_64F, 1, axis_angle.val);
  cvRodrigues(&mm, &vv, NULL, CC_RODRIGUES_M2V);
#endif
  return axis_angle;
}

//===========================================================================

//============================================================================
// Face detection helpers
//============================================================================
#ifdef _USE_CVFACE
#ifdef _USE_MYFACE
#include "face/cascadearr.h"
#endif
bool DetectFaces(vector<CRectd >& o_regions, const img_t* grayscale_image)
{
#ifdef _USE_MYFACE
	XRECT out[100];
	ASSERT( grayscale_image->tid==CC_8U );
	int ret = CPM_FaceDetect(vis_nesting_face20110713, grayscale_image->rows, grayscale_image->cols,
		grayscale_image->tt.data, grayscale_image->step, NULL, 0, 0.4, 1, 100, 1.1, 1, 0.8, 5, out, 100);
	if (ret > 0) {
		double d = out[0].w*0.1;
		int i = 0;
		printf("%d %d %d %d \n", out[i].x, out[i].y, out[i].w, out[i].h);
		for (i = 0; i<ret; ++i) {
			CRectd r = cRectd(out[i].x - d, out[i].y, out[i].w + 2 * d, out[i].h + 2 * d);
			o_regions.push_back(r);
		}
	}

#else
	  
  vector<Rect> face_detections;
  classifier.detectMultiScale(intensity, face_detections, 1.2, 2, 0, Size(50, 50));

  // Convert from int bounding box do a double one with corrections
  o_regions.resize(face_detections.size());

  for (size_t face = 0; face < o_regions.size(); ++face) {
    // OpenCV is overgenerous with face size and y location is off
    // CLM expect the bounding box to encompass from eyebrow to chin in y, and from cheeck outline to cheeck outline in x, so we need to compensate

    // The scalings were learned using the Face Detections on LFPW, Helen, AFW and iBUG datasets, using ground truth and detections from openCV

    // Correct for scale
    o_regions[face].width = face_detections[face].width * 0.8924;
    o_regions[face].height = face_detections[face].height * 0.8676;

    // Move the face slightly to the right (as the width was made smaller)
    o_regions[face].x = face_detections[face].x + 0.0578 * face_detections[face].width;
    // Shift face down as OpenCV Haar Cascade detects the forehead as well, and we're not interested
    o_regions[face].y = face_detections[face].y + face_detections[face].height * 0.2166;
  }
#endif

  return o_regions.size() > 0;
}

bool DetectSingleFace(CRectd& o_region, const img_t* intensity_image, CPoint preference )
{
  // The tracker can return multiple faces
  vector<CRectd > face_detections;

  bool detect_success = DetectFaces(face_detections, intensity_image);

  if (detect_success) {

    bool use_preferred = (preference.x != -1) && (preference.y != -1);

    if (face_detections.size() > 1) {
      // keep the closest one if preference point not set
      double best = -1;
      int bestIndex = -1;

      for (size_t i = 0; i < face_detections.size(); ++i) {
        double dist;
        bool better;

        if (use_preferred) {
          dist = sqrt((preference.x) * (face_detections[i].width / 2 + face_detections[i].x) +
              (preference.y) * (face_detections[i].height / 2 + face_detections[i].y));
          better = dist < best;
        }
        else {
          dist = face_detections[i].width;
          better = face_detections[i].width > best;
        }

        // Pick a closest face to preffered point or the biggest face
        if (i == 0 || better) {
          bestIndex = i;
          best = dist;
        }
      }

      o_region = face_detections[bestIndex];

    }
    else {
      o_region = face_detections[0];
    }

  }
  else {
    // if not detected
    o_region = cRectd(0, 0, 0, 0);
  }

  return detect_success;
}
#endif

#ifdef _USE_DLIB
bool DetectFacesHOG(vector<CRectd >& o_regions, const Mat& intensity, vector<double>& confidences)
{
  dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();

  return DetectFacesHOG(o_regions, intensity, detector, confidences);

}

bool DetectFacesHOG(vector<CRectd >& o_regions, const Mat& intensity, dlib::frontal_face_detector& detector, vector<double>& o_confidences)
{

  Mat upsampled_intensity;

  double scaling = 1.3;

  resize(intensity, upsampled_intensity, Size((int)(intensity.cols * scaling), (int)(intensity.rows * scaling)));

  dlib::cv_image<uchar> cv_grayscale(upsampled_intensity);

  vector<dlib::full_detection> face_detections;
  detector(cv_grayscale, face_detections, -0.2);

  // Convert from int bounding box do a double one with corrections
  o_regions.resize(face_detections.size());
  o_confidences.resize(face_detections.size());

  for (size_t face = 0; face < o_regions.size(); ++face) {
    // CLM expect the bounding box to encompass from eyebrow to chin in y, and from cheeck outline to cheeck outline in x, so we need to compensate

    // The scalings were learned using the Face Detections on LFPW and Helen using ground truth and detections from the HOG detector

    // Move the face slightly to the right (as the width was made smaller)
    o_regions[face].x = (face_detections[face].rect.get_rect().tl_corner().x() + 0.0389 * face_detections[face].rect.get_rect().width()) / scaling;
    // Shift face down as OpenCV Haar Cascade detects the forehead as well, and we're not interested
    o_regions[face].y = (face_detections[face].rect.get_rect().tl_corner().y() + 0.1278 * face_detections[face].rect.get_rect().height()) / scaling;

    // Correct for scale
    o_regions[face].width = (face_detections[face].rect.get_rect().width() * 0.9611) / scaling;
    o_regions[face].height = (face_detections[face].rect.get_rect().height() * 0.9388) / scaling;

    o_confidences[face] = face_detections[face].detection_confidence;


  }

  return o_regions.size() > 0;
}

bool DetectSingleFaceHOG(CRectd& o_region, const Mat& intensity_img, dlib::frontal_face_detector& detector, double& confidence, Point preference)
{
  // The tracker can return multiple faces
  vector<CRectd > face_detections;
  vector<double> confidences;

  bool detect_success = DetectFacesHOG(face_detections, intensity_img, detector, confidences);

  if (detect_success) {

    bool use_preferred = (preference.x != -1) && (preference.y != -1);

    // keep the most confident one or the one closest to preference point if set
    double best_so_far;

    if (use_preferred) {
      best_so_far = sqrt((preference.x - (face_detections[0].width / 2 + face_detections[0].x)) * (preference.x - (face_detections[0].width / 2 + face_detections[0].x)) +
          (preference.y - (face_detections[0].height / 2 + face_detections[0].y)) * (preference.y - (face_detections[0].height / 2 + face_detections[0].y)));
    }
    else {
      best_so_far = confidences[0];
    }

    int bestIndex = 0;

    for (size_t i = 1; i < face_detections.size(); ++i) {

      double dist;
      bool better;

      if (use_preferred) {
        dist = sqrt((preference.x - (face_detections[0].width / 2 + face_detections[0].x)) * (preference.x - (face_detections[0].width / 2 + face_detections[0].x)) +
            (preference.y - (face_detections[0].height / 2 + face_detections[0].y)) * (preference.y - (face_detections[0].height / 2 + face_detections[0].y)));
        better = dist < best_so_far;
      }
      else {
        dist = confidences[i];
        better = dist > best_so_far;
      }

      // Pick a closest face
      if (better) {
        best_so_far = dist;
        bestIndex = i;
      }
    }

    o_region = face_detections[bestIndex];
    confidence = confidences[bestIndex];
  }
  else {
    // if not detected
    o_region = CRectd(0, 0, 0, 0);
    // A completely unreliable detection (shouldn't really matter what is returned here)
    confidence = -2;
  }

  return detect_success;
}
#endif

//============================================================================
// Matrix reading functionality
//============================================================================

// Reading in a matrix from a stream
void ReadMat(FILE* stream, Mat& output_mat)
{
  // Read in the number of rows, columns and the data type
  int row, col, type, i;

  fscanf(stream, "%d %d %d", &row, &col, &type);

  output_mat.create(row, col, type);

  switch (output_mat.depth()) {
  case CC_64F: {
    double* begin_it = (double*)output_mat.data;
    double* end_it = begin_it + row*col;

    while (begin_it != end_it) {
      double d=0;
      fscanf(stream, "%lf", &d);
      *begin_it++ = d;
    }
  }
  break;

  case CC_32F: {
    float* begin_it = (float*)output_mat.data;
    float* end_it = begin_it + row*col;
    
    while (begin_it != end_it) {
      double d=0;
      fscanf(stream, "%lf", &d);
      *begin_it++ = d;
    }
  }
  break;

  case CC_32S: {
    int* begin_it = (int*)output_mat.data;
    int* end_it = begin_it + row*col;
    
    while (begin_it != end_it) {
      int d=0;
      fscanf(stream, "%d", &d);
      *begin_it++ = d;
    }
  }
  break;

  case CC_8U: 
	  for (i = 0; i < row; ++i) {
		  int* begin_it = ((int*)output_mat.data)+i*output_mat.cols;
		  int* end_it = begin_it + col;
      
      while (begin_it != end_it) {
        int d=0;
        fscanf(stream, "%d", &d);
        *begin_it++ = d;
		  }
	  }
  break;

  default:
    printf("ERROR(%s,%d) : Unsupported Matrix type %d!\n", __FILE__, __LINE__, output_mat.depth());
    abort();
  }
}

void ReadMatBin1(FILE* stream, Mat& output_mat)
{
	// Read in the number of rows, columns and the data type
	int row, col, type;

	fread((char*)&row, 4, 1, stream);
	fread((char*)&col, 4, 1, stream);
	fread((char*)&type, 4, 1, stream);

	output_mat.create(row, col, type);
	int size = output_mat.rows * output_mat.cols * output_mat.elemSize();
	fread((char*)output_mat.data, size, 1, stream);
}

void ReadMatBin(FILE* stream, Mat& output_mat0, int outtype)
{
  // Read in the number of rows, columns and the data type
  int row, col, type;

  fread((char*)&row, 4, 1, stream);
  fread((char*)&col, 4, 1, stream);
  fread((char*)&type, 4, 1, stream);

  Mat output_mat(row, col, type);
  int size = output_mat.rows * output_mat.cols * output_mat.elemSize();
  fread((char*)output_mat.data, size, 1, stream);
  output_mat0 = convertTo(output_mat, outtype);
}

static int fpeek(FILE* pf) {
  int pos = ftell(pf);
  int ch = fgetc(pf);
  fseek(pf, pos, SEEK_BEG);
  return ch;
}
// Skipping lines that start with # (together with empty lines)
static void SkipComments(FILE* stream)
{
  char ch = fpeek(stream);
  char skipped[1024];
  while ((ch == '#' || ch == '\n' || ch == ' ' || ch == '\r')) {
    fgets(skipped, countof(skipped), stream);
    ch = fpeek(stream);
  }
}


