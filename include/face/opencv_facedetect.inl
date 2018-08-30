
#if 1

#define ACCESS_MASK ACCESS_MASK1
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#undef ACCESS_MASK

#ifdef _WIN64
#pragma comment(lib,"opencv_world320.lib")
#pragma comment(lib,"opencv_ts320.lib")
#pragma comment(lib,"strmiids.lib")
#else
#ifdef _DEBUG
#pragma comment(lib,"opencv_world300d.lib")
#else
#pragma comment(lib,"opencv_world300.lib")
#endif
#endif

#include "cstd.h"

using namespace cv;
static CascadeClassifier*  m_faceCascade = NULL;
std::vector<cv::Rect>   m_allFaces;
int opencv_facedetect(int h, int w, const unsigned char* img, int step, XRECT* out) {
	int cnt = 0;
	Mat frame = Mat(h, w, CV_8UC3);
	memcpy2d(frame.data, frame.step, img, step, h, w*3);
	const char* cascadeFilePath;
#define CASPATH "D:/dt/"
	cascadeFilePath = CASPATH"lbpcascade_frontalface.xml";
	cascadeFilePath = CASPATH"haarcascade_frontalface_default.xml";
	cascadeFilePath = CASPATH"haarcascade_frontalface_alt_tree.xml";
	cascadeFilePath = CASPATH "lbpcascade_frontalface_improved.xml";
	
	static const char* CASCADE_FILE = (cascadeFilePath);
	if (m_faceCascade == NULL) {
		m_faceCascade = new CascadeClassifier(CASCADE_FILE);
	}

	if (m_faceCascade->empty()) {
		printf("Error creating cascade classifier. Make sure the file\n%s exists.\n", cascadeFilePath);
	} else {
		m_faceCascade->detectMultiScale(frame, m_allFaces, 1.1, 3, 0,
			cv::Size(60, 60),
			cv::Size(frame.rows, frame.rows));
		cnt = m_allFaces.size();
		//imshow("frame", frame);
		//waitKey(-1);
		if (out) {
			for (int i = 0; i < cnt; ++i) {
				out[i].x = m_allFaces[i].x;
				out[i].y = m_allFaces[i].y;
				out[i].w = m_allFaces[i].width;
				out[i].h = m_allFaces[i].height;
			}
		}
	}
	//cnt = 1;
	return cnt;
}

#endif
