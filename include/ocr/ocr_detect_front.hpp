

#ifndef __OCR_DETECT_FRONT_HPP__
#define __OCR_DETECT_FRONT_HPP__

//#include "debug.h"
#include "std/stddef_c.h"

//#include <direct.h>
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

#ifdef _DEBUG
#define SHOW 1
#else
#define SHOW 0
#endif



struct IdCardFrontSplitLine {
	//Rect rect[2];	int size;
	//vector<RotatedRect> lines_rect;
	vector<RotatedRect> lines_ok;
	//vector<Vec4i> lines;
	//vector<Vec4i> linesxx;
	Mat color_edge;
	int run(const Mat& mat, const Rect& rect) {
		//int w = rect.width / 3;
		//int h = rect.height / 2;
		Rect roi = rectExt(rect, rect.height*0.1, 0, mat.rows, mat.cols);
		Mat src = mat(roi);
		double ss = 200. / src.rows;
		src = resize(src, ss);
		cv::Ptr<cv::MSER> mesr1 = cv::MSER::create(2, 20, 200, 0.2, 0.3, 5);

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
			if (aa>0.7 && r.size.width>5 && r.size.height>5 && (r.center.x<src.cols*0.7 || r.center.y>src.rows*0.8)) {
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

		//SauvolaThresh(gry, mserMapMat, 0, Size(17, 17));
		NiblackSauvolaWolfJolion(gry, mserMapMat, WOLFJOLION, 11, 11, 0.2, 128);
		mserMapMat = 255 - mserMapMat;

		Mat element = getStructuringElement(MORPH_RECT, Size(5, 1)); //第一个参数MORPH_RECT表示矩形的卷积核，当然还可以选择椭圆形的、交叉型的
		Mat mserMapMat1;
		//膨胀操作
		dilate(mserMapMat, mserMapMat1, element);
		//imshow("mserMapMat1", mserMapMat1);

		cv::cvtColor(mserMapMat, color_edge, CV_GRAY2BGR);

		int xpos2 = mserMapMat.cols*0.5;
		int ypos = mserMapMat.rows - 50;
		if (1) {
			int xbeg = xpos2;
			int xend = mserMapMat.cols*0.8;
			Mat mserMapMat2 = mserMapMat1(Rect(xbeg, 0, xend - xbeg, ypos));
			vector<float> ph = project_H(mserMapMat2, 20);
			//Mat ph = projectHistogram(mserMapMat, 1);
			//drawHist(color_edge, ph, 1);
			int xpos_ = 0;
			for (int i = 1; i < ph.size(); ++i) {
				if (ph[i] < ph[xpos_]) {
					xpos_ = i;
				}
			}
			xpos2 = xpos_ + xbeg;
		}
		int xpos1 = mserMapMat.cols*0;
		if (1) {
			int xbeg = xpos1;
			int xend = mserMapMat.cols*0.3;
			Mat mserMapMat2 = mserMapMat(Rect(xbeg, 0, xend - xbeg, ypos));
			vector<float> ph = project_H(mserMapMat2, 20);
			//Mat ph = projectHistogram(mserMapMat, 1);
			//drawHist(color_edge, ph, 1);
			int xpos_ = ph.size()-1;
			int xbeg_ = 0;
			for (; xbeg_ < ph.size() && ph[xbeg_]<10; ++xbeg_);
			for (int i = xpos_ -1; i >xbeg_; --i) {
				if (ph[i] < ph[xpos_]) {
					xpos_ = i;
					if (ph[i] < 10) {
						break;
					}
				}
			}
			xpos1 = xpos_ + xbeg;
		}

		vector<RotatedRect> vec_rrect;
		vector<RotatedRect> bot_rrect;

		if (0) {
			Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
			morphologyEx(mserMapMat, mserMapMat, MORPH_OPEN, element);
			//morphologyEx(mserMapMat, mserMapMat, MORPH_CLOSE, element);
			//dilate(mserMapMat, mserMapMat, element);
		}
		if (1) {
			vector<vector<Point> > contours;
			vector<Vec4i> hierarchy;
			xpos2 += 20;
			findContours(mserMapMat.clone(), contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
			for (int i = 0; i < (int)contours.size(); i++) {
				cv::Rect r0 = cv::boundingRect(contours[i]);
				//Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
				//cv::rectangle(color_edge, r0, color, 1);
				cv::RotatedRect r = minAreaRect(contours[i]);
				r = curr(r);
				double aa = r.size.width*1. / r.size.height;
				if (r.size.height<20 && (r.size.width>4 || r.size.height>4) && ((r.center.x<xpos2) || r.center.y>ypos))
				{
					r.size.height = MIN(r.size.height, 20);
					r.size.width = MIN(r.size.width, 20);
					vec_rrect.push_back(r);
				}
			}
		}

		if (0) {
			drawRotatedRects(color_edge, vec_rrect, 1);
			imshow("color_edge", color_edge);
			imshow("src", src);
			waitKey(0);
		}
		if (vec_rrect.size() < 3) { return 0; }
		//sort(vec_rrect.begin(), vec_rrect.end(), [](const RotatedRect &r1, const RotatedRect &r2) { return r1.center.y < r2.center.y || r1.center.x < r2.center.x; });
		vector<vector<RotatedRect>> rrects2;
		vector<int> labels;
		int nclasses = 0;
		nclasses = cv::partition(vec_rrect, labels, [](const RotatedRect& r1, const RotatedRect& r2) {
			double dy = fabs(r1.center.y - r2.center.y);
			double hd = MIN(r1.size.height, r2.size.height);
			double hd2 = MAX(r1.size.height, r2.size.height);
			hd = (hd + hd2)*0.5;
			return dy < hd;
		});
		if (nclasses < 4) {
			return 0;
		}
		if (nclasses >= 2) {
			vector<int> cnt;
			cnt.assign(nclasses, 0);
			for (int i = 0; i < labels.size(); ++i) {
				int j = labels[i];
				++cnt[j];
			}
			std::vector<int> index = argsort(cnt);
			vector<int> cnt2;
			for (int i = 0; i < nclasses; ++i) {
				int idx = index[i];
				int c = cnt[idx];
				if (c > 1) {
					vector<RotatedRect> rrects;
					for (int j = 0; j < vec_rrect.size(); ++j) {
						if (labels[j] == idx) {
							rrects.push_back(vec_rrect[j]);
						}
					}
					RotatedRect rr = minAreaRect(rrects);
					if (rr.size.height > 5) {
						rrects2.push_back(rrects);
						cnt2.push_back(c);
						lines_ok.push_back(rr);
					}
					//drawRotatedRects(color_edge, rrects, 1);
				}
			}
		}

		if (lines_ok.size() < 4) {
			return 0;
		}

		if (0) {
			sort(lines_ok.begin(), lines_ok.end(), [](const RotatedRect &r1, const RotatedRect &r2) { return r1.center.y < r2.center.y; });

			if (lines_ok[lines_ok.size() - 1].size.width < 100) {
				lines_ok.pop_back();
				rrects2.pop_back();
			}
		}
		int maxid = -1;
		for (int i = 0; i < rrects2.size(); ++i) {
			sort(rrects2[i].begin(), rrects2[i].end(), [](const RotatedRect &r1, const RotatedRect &r2) { return r1.center.x < r2.center.x; });
			rrects2[i] = nms(rrects2[i]);
		}
		if (1) {
			double y = 0;
			for (int i = 0; i < rrects2.size(); ++i) {
				if (rrects2[i].size() > 10) {
					if (rrects2[i][0].center.y > y) {
						y = rrects2[i][0].center.y;
						maxid = i;
					}
				}
			}
			if (maxid < 0) { return 0; }
		}

		if (1) {
			RotatedRect rr = minAreaRect(rrects2[maxid]);
			Rect rc = rr.boundingRect();
			if (0) {
				int xbeg = 0;
				int xend = mserMapMat.cols*0.3;
				Mat mserMapMat2 = mserMapMat(rc);
				vector<float> ph = project_H(mserMapMat2, 20);
				int xpos_ = ph.size()*0.3;
				int xbeg_ = 0;
				for (; xbeg_ < xpos_; ++xbeg_);
				for (int i = xpos_ - 1; i > xbeg_; --i) {
					if (ph[i] < ph[xpos_]) {
						xpos_ = i;
					}
				}
				xpos1 = xpos_ + xbeg;
			}
			vector<RotatedRect>& rrects2_max = rrects2[maxid];
			int maxid2 = rrects2_max.size() - 20;
			int maxid3 = rrects2_max.size() - 15;
			if (maxid3 < 0) { return 0; }
			maxid2 = MAX(maxid2, -1);
			double tt1 = rc.x + rc.width*0.3;
			double tt2 = rc.x;
			double d = 0;
			if (maxid2 >= 0) {
				for (int i = maxid2; i < maxid3; ++i) {
					double d1 = rrects2_max[i + 1].center.x - rrects2_max[i].center.x;
					if (d1 > d) {
						maxid2 = i;
						d = d1;
						rrects2_max[i + 1].center.x - rrects2_max[i].center.x;
						tt2 = (rrects2_max[i + 1].center.x + rrects2_max[i].center.x)*0.5;
					}
				}
				int xpos_ = rrects2_max[maxid2 + 1].center.x - rrects2_max[maxid2 + 1].size.width*0.5 - rc.x;
				rr.size.width -= xpos_;
				rr.center.x += xpos_ / 2.;
			}
			lines_ok.clear();
			double tt = tt2 - rr.size.width*0.3;
			double tt11 = tt2 - rr.size.width*0.15;
			if (maxid2 >= 4) {
				//tt11 = rrects2_max[maxid2/2+1].center.x;
				//tt = rrects2_max[maxid2/2].center.x;
				tt = 0;
				tt11 = 0;
			}

			if (1) {
				vector<RotatedRect> vec_rrect;
				vector<int> labels;
				double xx = rr.center.x - rr.size.width*0.5;
				double yy = rr.center.y - rr.size.height*2;
				for (int i = 0; i < rrects2.size(); ++i) {
					if (i != maxid) {
						for (int j = 0; j < rrects2[i].size(); ++j) {
							RotatedRect& rr1 = rrects2[i][j];
							if (rr1.center.x < xx && rr1.center.y<yy) {
								vec_rrect.push_back(rr1);
							}
						}
					}
				}
				int nclasses = 0;
				nclasses = cv::partition(vec_rrect, labels, [](const RotatedRect& r1, const RotatedRect& r2) {
					double dy = fabs(r1.center.y - r2.center.y);
					double dx = fabs(r1.center.x - r2.center.x);
					double hd = MIN(r1.size.height, r2.size.height);
					double hd2 = MAX(r1.size.height, r2.size.height);
					return dy > hd2 && dx<5;
				});
				vector<RotatedRect> vlines_ok;
				vector<Rect> vlines_okrc;
				if (nclasses>1){
					vector<int> cnt;
					cnt.assign(nclasses, 0);
					for (int i = 0; i < labels.size(); ++i) {
						int j = labels[i];
						++cnt[j];
					}
					std::vector<int> index = argsort(cnt);
					vector<int> cnt2;
					for (int i = 0; i < nclasses; ++i) {
						int idx = index[i];
						int c = cnt[idx];
						if (c > 3) {
							vector<RotatedRect> rrects;
							for (int j = 0; j < vec_rrect.size(); ++j) {
								if (labels[j] == idx) {
									rrects.push_back(vec_rrect[j]);
								}
							}
							RotatedRect rr1 = minAreaRect(rrects);
							vlines_ok.push_back(rr1);
							vlines_okrc.push_back(rr1.boundingRect());
							//drawRotatedRects(color_edge, rrects, 1);
						}
					}
				}
				if (vlines_ok.size() == 0) {
					return 0;
				}
				float maxy = 0;
				for (int i = 0; i < vlines_okrc.size(); ++i) {
					int y = vlines_okrc[i].y + vlines_okrc[i].height;
					maxy = MAX(maxy, y);
				}
				double tttt = 10000;
				for (int i = 0; i < vlines_ok.size(); ++i) {
					int y = vlines_okrc[i].y + vlines_okrc[i].height;
					if (y > maxy*0.9) {
						Rect rc = vlines_ok[i].boundingRect();
						tttt = MIN(tttt, rc.x);
					}
				}

				if (0) {
					drawRotatedRects(color_edge, vlines_ok, 1);
					imshow("color_edge_v", color_edge);
					//waitKey(0);
				}

				tt = tttt;
				tt11 = tttt;
			}
			//ASSERT(rrects2.size()== lines_ok.size());
			for (int i = 0; i < rrects2.size(); ++i) {
				vector<RotatedRect> vrr1;
				for (int j = 0; j < rrects2[i].size(); ++j) {
					const RotatedRect& rr1 = rrects2[i][j];
					if (rr1.center.x > tt) {
						if (rr1.center.x>tt11 || j == (rrects2[i].size() - 1) || ((rrects2[i][j + 1].center.x - rr1.center.x) < (rr1.size.height)))	{
							if (i!= maxid && j < (rrects2[i].size() - 1)) {
								double tt = rrects2[i][j + 1].center.x - rr1.center.x;
								tt = rrects2[i][j + 1].center.x - rr1.center.x;
							}
							if (rr1.center.x < (rr.center.x - 20) || (vrr1.size() > 0 && (rr1.center.x - vrr1.back().center.x) < 30)) {
								if (rr1.center.x < rr.center.x+10) {
									int asdf = 0;
									vrr1.push_back(rr1);
								}
							}
						}
					}
				}
				if (vrr1.size() > 0) {
					RotatedRect rr1 = minAreaRect(vrr1);
					//lines_ok[i] = rr1;
					if (rr1.size.height > 5) {
						if ((rr1.size.width*rr1.size.width) > 400 || (rr.center.y - rr1.center.y) < 40) {
							lines_ok.push_back(rr1);
						}
					}
				}
			}

			sort(lines_ok.begin(), lines_ok.end(), [](const RotatedRect &r1, const RotatedRect &r2) { return r1.center.y < r2.center.y; });
			lines_ok[lines_ok.size() - 1] = rr;

		}
		if (lines_ok.size() < 4) { return 0; }
		for (int i = 0; i < lines_ok.size(); ++i) {
			float a = fabs(lines_ok[i].angle);
			if (a > 30) {
				lines_ok[i].angle = 0;
			}
		}
		if (1) {
			vector<float> vecleft;
			for (int i = 0; i < lines_ok.size() - 1; ++i) {
				float left = lines_ok[i].center.x - lines_ok[i].size.width*0.5;
				vecleft.push_back(left);
			}
			vecleft.push_back(vecleft.back());
			for (int i = 1; i < vecleft.size() - 1; ++i) {
				vecleft[i] = MIN(vecleft[i], vecleft[i + 1]);
				vecleft[i] = MIN(vecleft[i], vecleft[i - 1]);
			}
			vecleft[0] = vecleft[1];
			vecleft[vecleft.size() - 1] = vecleft[vecleft.size() - 2];

			for (int i = 0; i < lines_ok.size() - 1; ++i) {
				float left1 = vecleft[i];
				float left = lines_ok[i].center.x - lines_ok[i].size.width*0.5;
				float tt = left - left1;
				lines_ok[i].size.width += tt;
				lines_ok[i].center.x -= tt*0.5;
			}
		}
		if (1) {
			// split
			RotatedRect rr = lines_ok.back();
			vector<RotatedRect> lines_ok2;
			for (int i = 0; i < lines_ok.size()-1; ++i) {
				RotatedRect r1 = lines_ok[i];
				if (r1.size.height > rr.size.height * 2) {
					double tt = (r1.size.height - rr.size.height)*0.5;
					RotatedRect r2 = r1;
					r2.size.height = r1.size.height = rr.size.height;
					r1.center.y -= tt;
					r2.center.y += tt;
					lines_ok2.push_back(r1);
					lines_ok2.push_back(r2);
				}
				else {
					if (rr.center.y - r1.center.y > 20) {
						lines_ok2.push_back(r1);
					}
				}
			}
			lines_ok2.push_back(rr);
			lines_ok = lines_ok2;
			sort(lines_ok.begin(), lines_ok.end(), [](const RotatedRect &r1, const RotatedRect &r2) { return r1.center.y < r2.center.y; });
		}
		if (0) {
			drawRotatedRects(color_edge, lines_ok, 1);
			imshow("color_edge", color_edge);
			imshow("src", src);
			waitKey(0);
		}

		if (1) {
			// combine
			RotatedRect rr = lines_ok.back();
			rr.size.height = MIN(rr.size.height, 13);
			if (rr.center.y > 190) {
				double dy = rr.center.y - 190;
				rr.size.height += dy;
				rr.center.y += dy*0.5;
			}
			vector<RotatedRect> lines_ok2;
			for (int i = 0; i < lines_ok.size() - 1; ++i) {
				double dy = lines_ok[i + 1].center.y - lines_ok[i].center.y;
				double dx = lines_ok[i + 1].center.x - lines_ok[i].center.x;
				RotatedRect r1 = lines_ok[i];
				if (r1.size.height<10 && dy<rr.size.height*1.5 && dx<10) {
					r1.size.height = dy*1.5;
					r1.center.y = (lines_ok[i + 1].center.y + lines_ok[i].center.y)*0.5;
					lines_ok2.push_back(r1);
					++i;
				}
				else {
					lines_ok2.push_back(r1);
				}
			}
			lines_ok2.push_back(rr);
			lines_ok = lines_ok2;
		}
		if (1) {
			// insert 
			RotatedRect rr = lines_ok.back();
			vector<RotatedRect> lines_ok2;
			for (int i = 0; i < lines_ok.size() - 1; ++i) {
				double dy = lines_ok[i + 1].center.y - lines_ok[i].center.y;
				RotatedRect r1 = lines_ok[i];
				if (i<2 && dy>rr.size.height*4) {
					RotatedRect r2 = lines_ok[i + 1];
					r2.center.y = (lines_ok[i + 1].center.y + lines_ok[i].center.y)*0.5;
					lines_ok2.push_back(r1);
					lines_ok2.push_back(r2);
				}
				else {
					lines_ok2.push_back(r1);
				}
			}
			lines_ok2.push_back(rr);
			lines_ok = lines_ok2;
		}
		if (1) {
			RotatedRect rr = lines_ok.back();
			lines_ok[0].size.height = rr.size.height*1.3;
		}
		if (1) {
			Rect r = lines_ok[1].boundingRect();
			r = curr(r);
			int off1 = MIN(r.height * 5, r.width - r.height);
			off1 = MAX(0, off1);
			r.x += off1;
			r.width -= off1;
			r = curr(r);
			Mat gray = mserMapMat(r);
			vector<float> ph = project_H(gray, 10);
			int nnn = ph.size();
			int xpos = nnn-1;
			//int begpos = gray.rows * 4;
			for (int i = nnn-1; i >0; --i) {
				if (ph[i] < ph[xpos]) {
					xpos = i;
				}
			}
			xpos += off1;
			lines_ok[1].size.width -= xpos;
			lines_ok[1].center.x += xpos*0.5;
			//imshow("gray", gray); waitKey(0);
		}

		if (0) {
			drawRotatedRects(color_edge, lines_ok, 1);
			imshow("color_edge", color_edge);
			imshow("src", src);
			waitKey(0);
		}

		if (0 && lines_ok.size()>1) {
			drawRotatedRects(color_edge, lines_ok, 1);
			drawRotatedRects(src, lines_ok, 1);

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
			imshow("color_edge", color_edge);
			imshow("src", src);
			waitKey(0);
		}
		return lines_ok.size();
	}
};


struct IdCardFrontDetecter {
	HAARCASCADE* ca;
	IdCardFrontDetecter() { ca = NULL; }
	std::vector<vector<RotatedRect> > rect;
	IdCardFrontSplitLine split_line;
	void init(const char* fn) {
		if (NULL == ca) {
			ca = cas_load(fn);
		}
	}
	int run(cv::Mat& mat, std::vector<vector<RotatedRect> >& rect, int rotid) {
		int n = 0;
		if (ca) {
			Mat gry = mastbegray(mat);
			int xywh[1000];
			double cs = 100.0 / gry.rows;
			int is_trans = 0;
			n = objdetect(ca, gry.rows, gry.cols, gry.data, gry.step1(),
				NULL, 0, cs, 1, 100, 1.1, 1,
				0.8, 8, xywh, 1000, is_trans);
			n = MIN(n, 1);
			for (int i = 0; i<n; ++i) {
				int* p = xywh + i * 4;
				Rect rc = Rect(p[0], p[1], p[2], p[3]);
				Size size(mat.cols, mat.rows);
				vector<RotatedRect> one;
				one.push_back(r2rr(rc, size, rotid));
				if (split_line.run(mat, rc)>4) {
					for (int j = 0; j < split_line.lines_ok.size(); ++j) {
						RotatedRect r = split_line.lines_ok[j];
						//RotatedRect rr(Point2f(r.x + r.w *0.5, r.y + r.h *0.5), Size2f(r.w, r.h), 0);
						r = rect_rotate90(r, size, -rotid);
						one.push_back(r);
					}
					//drawRotatedRects(mat, split_line.lines_ok, 1);	imshow("mat", mat);	waitKey(0);
				}
				rect.push_back(one);
			}
		}
		return rect.size();
	}
};



#endif // __OCR_DETECT_FRONT_HPP__
