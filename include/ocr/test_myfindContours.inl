
#define LONG int
#define WORD short
#define MAKELONG(a, b) ((LONG)(((WORD)(a)) | ((LONG)((WORD)(b))) << 16))
#define HIWORD(I) ( ( WORD ) ( ( ( LONG )( I ) >> 16) & 0xFFFF ) )
#define LOWORD(l) ((WORD)(((LONG)(l)) & 0xffff))

// 计算指定区域直方图峰值所在的范围
static int calcDistrib_Rel(const cv::Mat &image_gray, const cv::Rect &roi)
{
	int maxv = 0;
	int histogram[UCHAR_MAX + 1] = { 0 };

	// TODO: loop unrolling & optimization
	const size_t image_step = image_gray.step;
	const uchar *__restrict pdata = image_gray.data + roi.y * image_step;
	const int_fast32_t y_upper = roi.y + roi.height;
	const int_fast32_t x_upper = roi.x + roi.width;
	for (int_fast32_t i = roi.y; i < y_upper; ++i) {
		auto pline_data = pdata + roi.x;
		for (int_fast32_t j = roi.x; j < x_upper; ++j) {
			int_fast32_t m = *pline_data++;
			int_fast32_t v = ++histogram[m];
			if (v > histogram[maxv]) maxv = m;
		}
		pdata += image_step;
	}

	int mid = histogram[maxv] / 2;
	int low = maxv, high = maxv;
	while (low > 0 && histogram[--low] >= mid) {
	}
	while (high < UCHAR_MAX && histogram[++high] >= mid) {
	}

	return MAKELONG(low, high);
}

int select_max(int a, int b, int c, int d) {
	a = MAX(a, b);
	c = MAX(c, d);
	return MAX(a, c);
}

int select_min(int a, int b, int c, int d) {
	a = MIN(a, b);
	c = MIN(c, d);
	return MIN(a, c);
}

int mythreshold(cv::Mat r, cv::Mat& r_threshold) {
	// 假设身份证位于图像中部
	// 通过直方图计算上下左右和中间块区域的颜色区分度
	static constexpr int block_scale = 10;
	int block_width = r.cols / block_scale;
	int block_height = r.rows / block_scale;
	int e = calcDistrib_Rel(r, cv::Rect((r.cols - block_width) / 2 - 1, (r.rows - block_height) / 2 - 1, block_width, block_height));
	int a = calcDistrib_Rel(r, cv::Rect(0, 0, block_width, block_height));
	int m = HIWORD(a) <= LOWORD(e), n = LOWORD(a) >= HIWORD(e); // 背景颜色分布在前景前、后还是有重叠，从直方图很容易看出来
	int b = m || n ? calcDistrib_Rel(r, cv::Rect(r.cols - block_width - 1, 0, block_width, block_height)) : a;
	int o = (m && HIWORD(b) <= LOWORD(e)) || (n && LOWORD(b) >= HIWORD(e));
	int c = !o ? b : calcDistrib_Rel(r, cv::Rect(0, r.rows - block_height - 1, block_width, block_height));
	int q = (m && HIWORD(c) <= LOWORD(e)) || (n && LOWORD(c) >= HIWORD(e));
	int d = !q ? c : calcDistrib_Rel(r, cv::Rect(r.cols - block_width - 1, r.rows - block_height - 1, block_width, block_height));
	int s = (m && HIWORD(d) <= LOWORD(e)) || (n && LOWORD(d) >= HIWORD(e));
	// 上述m、n、o、q、s都是区域颜色是否重叠的标志，之所以这样做是为了快速判断输入图像能否使用当前方案，避免多余计算

	if (s) {
		int threshold = m ?
			select_max(HIWORD(a), HIWORD(b), HIWORD(c), HIWORD(d)) :
			select_min(LOWORD(a), LOWORD(b), LOWORD(c), LOWORD(d));
		threshold = m ?
			threshold + (LOWORD(e) - threshold) * 0.45 :
			HIWORD(e) + (threshold - HIWORD(e)) * 0.60; // 让阀值尽可能靠近背景
														//cv::Mat r_threshold;
		cv::threshold(r, r_threshold, threshold, UCHAR_MAX, m ? cv::THRESH_BINARY : cv::THRESH_BINARY_INV);
	}
	else {
		// 这里表明前、背景有颜色重叠，可能是背景颜色和身份证近似，或者光照污染，此时全局阀值二值化的结果肯定不好，会丢失轮廓，可以考虑自适应阀值+形态学滤波cv::MORPH_GRADIENT等来得到轮廓明显的图像，这里就不贴了。
		cv::threshold(r, r_threshold, 0, UCHAR_MAX, m ? cv::THRESH_BINARY : cv::THRESH_BINARY_INV);
	}
	return 0;
}
int myfindContours(cv::Mat& image_color) {
	cv::Mat mvs[3];
	cv::split(image_color, mvs);
	cv::Mat &r = mvs[0]; // roi

	cv::Mat r_threshold;
	mythreshold(r, r_threshold);
	std::vector< std::vector<cv::Point> > contours_list;
	std::vector<cv::Vec4i> hierarchy;
	// Since opencv 3.2 source image is not modified by this function
	cv::findContours(r_threshold, contours_list, hierarchy,
		cv::RetrievalModes::RETR_EXTERNAL, cv::ContourApproximationModes::CHAIN_APPROX_NONE);
	for (auto &contour : contours_list) {
		cv::Rect &&rect = cv::boundingRect(contour);
		if (rect.width > (r.cols / 2) && rect.height > (r.rows / 2)) {
			//cv::rectangle(image_color, rect, cv::Scalar(0, 255, 0), 5);
			std::vector<cv::Point2f> poly;
			cv::approxPolyDP(contour, poly, 2, true);
			for (uint32_t i = 0; i < poly.size() - 1; ++i) {
				cv::line(image_color, poly[i], poly[i + 1], cv::Scalar(0, 255, 0), 6);
			} //for
			cv::line(image_color, poly[poly.size() - 1], poly[0], cv::Scalar(0, 255, 0), 6);
			break;
		} //if
	}
	return 0;
}


int test_myfindContours() {
	std::vector<string> filenames;
	LoadTextFileList("E:/OCR_Line/demo_images/list.txt", filenames);
	int k;
	isdebug = true;
	for (k = 0; k < filenames.size(); ++k) {
		string fn = filenames[k];
		printf("%s\n", fn.c_str());
		cv::Mat srcImage = cv::imread(fn);
		if (srcImage.empty()) {
			continue;
		}
		if (srcImage.cols>600) {
			double t = 600.*1. / srcImage.cols;
			cv::resize(srcImage, srcImage, cv::Size(), t, t, cv::INTER_LANCZOS4);
		}
		myfindContours(srcImage);
		imshow("srcImage", srcImage);
		cv::waitKey(0);
	}
	return 0;
}
