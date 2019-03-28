#ifndef __OCR_DETECT_BACK_HPP__
#define __OCR_DETECT_BACK_HPP__


//#include "debug.h"
#include "std/stddef_c.h"

#include <direct.h>
//#include "ui/window.inl"
#include "std/fileio_c.h"
#include "std/inifile.h"
//#include "cap.h"

//#include "img/imgio.inl"
//#include "std/imdraw.h"
//#include "std/drawing_c.h"
#include "std/gui_c.h"
#include "std/dir_c.h"
#include "std/log_c.h"
#include "std/objdetect.h"
#include "std/colorcvt.h"
#include "std/fileioex.h"
#include "face/face.h"

#include "opencvex/opencvex.hpp"
//#include "detect_idcard.hpp"
// idcard_back_20190326

uchar idcard_back_20190326[] = {
#include "face/cas/idcard_back_20190326.inl"
};


// face_detect

struct IdCardBackSplitRect {
	vector<RotatedRect> lines_rect;
	vector<RotatedRect> lines_ok;
	vector<Vec4i> lines;
	vector<Vec4i> linesxx;
	Mat color_edge;
	int run(const Mat& src) {
		int ret = 1;
		cv::Ptr<cv::MSER> mesr1 = cv::MSER::create(2, 10, 200, 0.2, 0.3);
		Mat gray;
		std::vector<cv::Rect> bboxes1;
		std::vector<cv::RotatedRect> vrr;
		std::vector<std::vector<cv::Point> > regContours;
		cv::cvtColor(src, gray, CV_BGR2GRAY);
		Rect aRect(0, 0, src.cols, src.rows);
		mesr1->detectRegions(gray, regContours, bboxes1);
		cv::Mat mserMapMat = cv::Mat::zeros(src.size(), CV_8UC1);
		Mat bw;
	}
};
struct IdCardBackSplitLine {
	//Rect rect[2];	int size;
	//vector<RotatedRect> lines_rect;
	vector<RotatedRect> lines_ok;
	//vector<Vec4i> lines;
	//vector<Vec4i> linesxx;
	Mat color_edge;
	int run(const Mat& mat, const Rect& rect) {
		int w = rect.width / 3;
		int h = rect.height / 2;
		Rect roi(rect.x + w*0.8, rect.y + h*1.1, w * 2, h);
		Mat src = mat(roi);
		double ss = 100. / src.rows;
		src = resize(src, ss);
		cv::Ptr<cv::MSER> mesr1 = cv::MSER::create(2, 20, 200, 0.2, 0.3);

		std::vector<cv::Rect> bboxes1;
		std::vector<cv::RotatedRect> vrr;
		std::vector<std::vector<cv::Point> > regContours;
		lines_ok.clear();
		Mat gry = mastbegray(src);

		mesr1->detectRegions(gry, regContours, bboxes1);
		cv::Mat mserMapMat = cv::Mat::zeros(src.size(), CV_8UC1);

		for (int i = (int)regContours.size() - 1; i >= 0; i--)
		{
			// 根据检测区域点生成mser+结果
			const std::vector<cv::Point>& p = regContours[i];
			Rect rc = bboxes1[i];
			//if (r.width > 30 || r.height > 30 || r.height<2 || r.width<2)        continue;
			RotatedRect r(Point2f(rc.x + rc.width / 2, rc.y + rc.height / 2), Size2f(rc.width, rc.height), 0);
			r = minAreaRect(p);
			//int k = 10;
			//if (rr.size.height < k || rr.size.width < k) { continue; }
			double aa = r.size.width*1. / r.size.height;
			double d = ptdist(r.center, Point(src.cols*0.8, src.rows*0.4))*1. / src.cols;
			int angle = mymodi(r.angle - 90, 90);
			if (aa>0.7 && r.size.width>5 && r.size.height>5) {
				RotatedRect rectPoint = minAreaRect(p);
				//Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
				//drawRotatedRect(color_edge, rectPoint, color, 1);
				vrr.push_back(r);
				for (int j = 0; j < (int)p.size(); j++)
				{
					cv::Point pt = p[j];
					//if (ptInRect(pt.x, pt.y, r))
					mserMapMat.at<unsigned char>(pt) = 255;
				}
			}
		}
		cv::cvtColor(mserMapMat, color_edge, CV_GRAY2BGR);
		vector<float> ph = project_H(mserMapMat, 10);
		//Mat ph = projectHistogram(mserMapMat, 1);
		//drawHist(color_edge, ph, 1);
		int xpos = 0;
		for (int i = 1; i < 50; ++i) {
			if (ph[i] <= ph[xpos]) {
				xpos = i;
			}
		}
		vector<RotatedRect> vec_rrect;
		vector<RotatedRect> bot_rrect;
		if (1) {
			vector<vector<Point> > contours;
			vector<Vec4i> hierarchy;
			findContours(mserMapMat.clone(), contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
			for (int i = 0; i < (int)contours.size(); i++) {
				cv::Rect r0 = cv::boundingRect(contours[i]);
				//Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
				//cv::rectangle(color_edge, r0, color, 1);
				cv::RotatedRect r = minAreaRect(contours[i]);
				r = curr(r);
				double aa = r.size.width*1. / r.size.height;
				if (r.center.x>xpos)
				{
					r.size.height = MIN(r.size.height, 20);
					r.size.width = MIN(r.size.width, 20);
					vec_rrect.push_back(r);
				}
			}
			drawRotatedRects(color_edge, vec_rrect, 1);
		}
		if (vec_rrect.size() < 3) { return 0; }
		vector<vector<RotatedRect>> rrects2;
		vector<int> labels;
		int nclasses = 0;
		nclasses = cv::partition(vec_rrect, labels, [](const RotatedRect& r1, const RotatedRect& r2) {
			double dy = fabs(r1.center.y - r2.center.y);
			double hd = MIN(r1.size.height, r2.size.height);
			return dy < hd;
		});
		if (nclasses >= 2) {
			vector<int> cnt;
			cnt.assign(nclasses, 0);
			for (int i = 0; i < labels.size(); ++i) {
				int j = labels[i];
				++cnt[j];
			}
			std::vector<int> index = argsort(cnt);
			vector<int> cnt2;
			for (int i = 0; i < 2; ++i) {
				int idx = index[i];
				int c = cnt[idx];
				if (c > 1) {
					vector<RotatedRect> rrects;
					for (int j = 0; j < vec_rrect.size(); ++j) {
						if (labels[j] == idx) {
							rrects.push_back(vec_rrect[j]);
						}
					}
					rrects2.push_back(rrects);
					cnt2.push_back(c);
					RotatedRect rr = minAreaRect(rrects);
					lines_ok.push_back(rr);
					//drawRotatedRects(color_edge, rrects, 1);
				}
			}
		}

		if (lines_ok.size() != 2) {
			return 0;
		}
		if (lines_ok[0].center.y > lines_ok[1].center.y) {
			std::swap(lines_ok[0], lines_ok[1]);
		}
		for (int i = 0; i < lines_ok.size(); ++i) {
			int h = lines_ok[i].size.height;
			if (h < 10 || h>40) {
				lines_ok.clear();
				return 0;
			}
		}
		if (1) {
			double ang = 0;
			double hh = 0;
			int angcnt = 0;
			for (int i = 0; i < 2; ++i) {
				if (lines_ok[i].size.width > 50) {
					ang += lines_ok[i].angle;
					hh += lines_ok[i].size.height;
					++angcnt;
				}
			}
			hh /= angcnt;
			ang /= angcnt;
			lines_ok[0].size.height = hh;
			lines_ok[1].size.height = hh;

			lines_ok[0].angle = ang;
			lines_ok[1].angle = ang;
			double s = sin(ang*CV_PI / 360);
			double c = cos(ang*CV_PI / 360);
			//double x1 = lines_ok[1].center.x + lines_ok[1].size.width*0.5;
			//double x11 = lines_ok[1].center.x + lines_ok[1].size.width;
			for (int i = 0; i < 2; ++i) {
				float w = lines_ok[i].size.width;
				lines_ok[i].size.width *= 1.4;
				lines_ok[i].center.x += (lines_ok[i].size.width-w)*0.4;
			}
		}

		if (0 && lines_ok.size()>1) {
			drawRotatedRects(color_edge, lines_ok, 1);
			drawRotatedRects(src, lines_ok, 1);

			Mat aa = getSubImage(src, lines_ok[0]);
			Mat bb = getSubImage(src, lines_ok[1]);

			imshow("aa", aa);
			imshow("bb", bb);
			imshow("color_edge", color_edge);
			imshow("src", src);
			waitKey(0);
		}
		double invss = 1. / ss;
		for (int i = 0; i < lines_ok.size(); ++i) {
			lines_ok[i] = scale(lines_ok[i], invss, invss);
			lines_ok[i].center.x += roi.x;
			lines_ok[i].center.y += roi.y;
		}
		if (0) {
			src = mat;
			drawRotatedRects(src, lines_ok, 1);
			rectangle(src, rect, CV_RGB(255, 0, 0), 1, 8, 0);
			imshow("src", src);
			waitKey(0);
		}
		return lines_ok.size();
	}
};

struct IdCardBackDetecter {
	HAARCASCADE* ca;
	IdCardBackDetecter() { ca = NULL; }
	std::vector<Rect> rect;
	IdCardBackSplitLine split_line;
	void init(const char* fn) {
		ca = cas_load(fn);
	}
	int run(cv::Mat& mat, std::vector<RotatedRect>& out, int rotid) {
		int n = 0;
		rect.clear();
		if (ca) {
			Mat gry = mastbegray(mat);
			int xywh[1000];
			double cs = 200.0 / gry.rows;
			int is_trans = 0;
			n = objdetect(ca, gry.rows, gry.cols, gry.data, gry.step1(),
				NULL, 0, cs, 1, 100, 1.1, 1,
				0.8, 8, xywh, 1000, is_trans);
			n = MIN(n, 1);
			for (int i = 0; i<n; ++i) {
				int* p = xywh + i * 4;
				Rect rc = Rect(p[0], p[1], p[2], p[3]);
				if (split_line.run(mat, rc)==2) {
					rect.push_back(rc);
					for (int j = 0; j < 2; ++j) {
						RotatedRect r = split_line.lines_ok[j];
						//RotatedRect rr(Point2f(r.x + r.w *0.5, r.y + r.h *0.5), Size2f(r.w, r.h), 0);
						r = rect_rotate90(r, Size(mat.cols, mat.rows), -rotid);
						out.push_back(r);
					}
					//drawRotatedRects(mat, split_line.lines_ok, 1);	imshow("mat", mat);	waitKey(0);
				}
			}
		}
		return rect.size();
	}
};


#endif // __OCR_DETECT_BACK_HPP__
