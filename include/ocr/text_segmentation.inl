#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <algorithm> 
using namespace cv;
using namespace std;


#define V_PROJECT 'V'
#define H_PROJECT 'H'

typedef struct
{
	int begin;
	int end;

}char_range_t;

void show_projection(vector<int>& pos, int mode)
{
	vector<int>::iterator max = std::max_element(std::begin(pos), std::end(pos)); //求最大值
	if (mode == H_PROJECT)
	{
		int height = pos.size();
		int width = *max;
		Mat project = Mat::zeros(height, width, CV_8UC1);
		for (int i = 0; i < project.rows; i++)
		{
			for (int j = 0; j < pos[i]; j++)
			{
				project.at<uchar>(i, j) = 255;
			}
		}
		cvNamedWindow("horizational projection", 0);
		imshow("horizational projection", project);

	}
	else if (mode == V_PROJECT)
	{
		int height = *max;
		int width = pos.size();
		Mat project = Mat::zeros(height, width, CV_8UC1);
		for (int i = 0; i < project.cols; i++)
		{
			for (int j = project.rows - 1; j >= project.rows - pos[i]; j--)
			{
				//std::cout << "j:" << j << "i:" << i << std::endl;
				project.at<uchar>(j, i) = 255;
			}
		}

		imshow("vertical projection", project);
	}

	//waitKey();
}

//获取文本的投影用于分割字符(垂直，水平)
int GetTextProjection(const Mat &src, vector<int>& pos, int mode)
{
	mode = toupper(mode);
	if (mode == V_PROJECT)
	{
		pos.resize(src.cols);
		for (int j = 0; j < src.cols; j++) {
			pos[j] = 0;
		}
		for (int i = 0; i < src.rows; i++)
		{
			const uchar* p = src.ptr<const uchar>(i);
			for (int j = 0; j < src.cols; j++)
			{
				if (p[j] == 0)
				{
					pos[j]++;
				}
			}
		}

		show_projection(pos, V_PROJECT);
	}
	else if (mode == H_PROJECT)
	{
		pos.resize(src.rows);
		for (int j = 0; j < src.rows; j++) {
			pos[j] = 0;
		}
		for (int i = 0; i < src.cols; i++)
		{

			for (int j = 0; j < src.rows; j++)
			{
				if (src.at<uchar>(j, i) == 0)
				{
					pos[j]++;
				}
			}
		}
		show_projection(pos, H_PROJECT);

	}	

	return 0;
}

//获取每个分割字符的范围，min_thresh：波峰的最小幅度，min_range：两个波峰的最小间隔
int GetPeekRange(vector<int> &vertical_pos, vector<char_range_t> &peek_range, int min_thresh = 2, int min_range = 10)
{
	int begin = 0;
	int end = 0;
	for (int i = 0; i < vertical_pos.size(); i++)
	{

		if (vertical_pos[i] > min_thresh && begin == 0)
		{
			begin = i;
		}
		else if (vertical_pos[i] > min_thresh && begin != 0)
		{
			continue;
		}
		else if (vertical_pos[i] < min_thresh && begin != 0)
		{
			end = i;
			if (end - begin >= min_range)
			{
				char_range_t tmp;
				tmp.begin = begin;
				tmp.end = end;
				peek_range.push_back(tmp);
				begin = 0;
				end = 0;
			}

		}
		else if (vertical_pos[i] < min_thresh || begin == 0)
		{
			continue;
		}
		else
		{
			//printf("raise error!\n");
		}
	}

	return 0;
}

inline void save_cut(const Mat& img, int id)
{
	char name[128] = { 0 };
	sprintf(name, "./save_cut/%d.jpg", id);
	imwrite(name, img);
}

//切割字符
int CutChar(Mat &img, const vector<char_range_t>& v_peek_range, const vector<char_range_t>& h_peek_range, vector<Mat>& chars_set)
{
	static int count = 0;
	int norm_width = img.rows;  //因为汉字都是类正方形的，所以我们定了norm_width就是汉字的高度
	Mat show_img = img.clone();
	cvtColor(show_img, show_img, CV_GRAY2BGR);
	for (int i = 0; i < v_peek_range.size(); i++)
	{
		int char_gap = v_peek_range[i].end - v_peek_range[i].begin;
		//if (char_gap <= (int)(norm_width*1.2) && char_gap >= (int)(norm_width*0.8))
		{
			int x = v_peek_range[i].begin - 2>0 ? v_peek_range[i].begin - 2 : 0;
			int width = char_gap + 4 <= img.rows ? char_gap : img.rows;
			Rect r(x, 0, width, img.rows);			
			rectangle(show_img, r, Scalar(255, 0, 0), 1);
			Mat single_char = img(r).clone();
			chars_set.push_back(single_char);
			save_cut(single_char, count);
			count++;
		}
	}

	imshow("cut", show_img);
	waitKey();

	return 0;
}

Mat cut_one_line(const Mat& src,int begin,int end)
{
	Mat line = src(Rect(0,begin,src.cols,end-begin)).clone();
	return line;
}


vector<Mat> CutSingleChar(Mat& img)
{
	Mat show = img.clone();
	cvtColor(show, show, CV_GRAY2BGR);
	threshold(img, img, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	imshow("binary", img);
	vector<int> horizion_pos(img.rows, 0);
	vector<char_range_t> h_peek_range;
	GetTextProjection(img, horizion_pos, H_PROJECT);
	GetPeekRange(horizion_pos, h_peek_range, 2, 10);

#if 1

	/*将每一文本行切割*/
	vector<Mat> lines_set;
	//vector<Mat> lines_set_show;
	for (int i = 0; i < h_peek_range.size(); i++)
	{
		Mat line = cut_one_line(img, h_peek_range[i].begin, h_peek_range[i].end);
		lines_set.push_back(line);		
		//Mat line_show = show(Rect(0, h_peek_range[i].begin, show.cols, h_peek_range[i].end - h_peek_range[i].begin));
		//lines_set_show.push_back(line_show);
	}

	vector<Mat> chars_set;
	for (int i = 0; i < lines_set.size(); i++)
	{
		Mat line = lines_set[i];
		//Mat line2 = lines_set_show[i];
		imshow("raw line", line);
		vector<int> vertical_pos(line.cols, 0);
		vector<char_range_t> v_peek_range;
		GetTextProjection(line, vertical_pos, V_PROJECT);
		GetPeekRange(vertical_pos, v_peek_range);
		CutChar(line, v_peek_range, h_peek_range, chars_set);
		//CutChar(line2, v_peek_range, h_peek_range, chars_set);
	}
#endif

	//imshow("line_show", show);
	//imwrite("show.png", show);
	return chars_set;
}

#define CLAMP(x, a, b)   ((x)<(a) ? (a) : (x)>(b) ? (b) : (x))
#include <algorithm>

int getMinPos1(const vector<int>& pos, int k, int range) {
	int n = pos.size();
	k = (int)CLAMP((k), 0, n-1);
	int b = (int)CLAMP((k - range), 0, n-1);
	int e = (int)CLAMP((k + range), 0, n-1);
	int minv = pos[k];
	int mini = k;
	for (int i = k; i < e; ++i) {
		if (pos[i] < minv) {
			mini = i;
			minv = pos[i];
		}
	}
	for (int i = k; i > b; --i) {
		if (pos[i] < minv) {
			mini = i;
			minv = pos[i];
		}
	}
	return mini;
}

#include <algorithm>

bool pair_less(pair<int, int>& a, pair<int, int>& b) {
	return a.second < b.second;
};
int getMinPos(const vector<int>& pos, int k, int range) {
	int n = pos.size();
	k = (int)CLAMP((k), 0, n - 1);
	int b = (int)CLAMP((k - range), 0, n - 1);
	int e = (int)CLAMP((k + range), 0, n - 1);
	int minv = pos[k];
	int s = 0, mini = k;
  minv = pos[(b + e)/2];
  for (int i = b; i < e; ++i) {
    if (pos[i] < minv) {
      minv = pos[i];
    }
  }
	n = 0;
	minv += 5;
	for (int i = b; i < e; ++i) {
		if (pos[i] <= minv) {
			s += i;
			++n;
		}
	}
	mini = s / n;
	return mini;
}
Range getRange(const vector<int>& pos, double thd) {
  int b = 0, e = pos.size();
  int minv = pos[(b + e) / 2];
  int maxv = pos[(b + e) / 2];
  int thdv = minv;
  for (int i = b; i < e; ++i) {
    if (pos[i] < minv) {
      minv = pos[i];
    }
    else if (pos[i] > maxv) {
      maxv = pos[i];
    }
  }
  thdv = minv + int(thd*(maxv - minv));
  for (; b < e && pos[b]<thdv; ++b);
  for (; b < e && pos[e-1]<thdv; --e);
  return Range(b, e);
}

vector<Rect> CutBlock(const Mat& src, const char* str) {
	vector<Rect> out;
  vector<int> pos;
  vector<int> pos1;
  Mat img;
  Rect r(0, 0, src.cols, src.rows);
	threshold(src, img, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
  cv::imshow("bin", img);
  double thd = 0.05;
  GetTextProjection(img(r), pos, 'v');
  Range range1 = getRange(pos, thd);
  GetTextProjection(img(r), pos, 'h');
  Range range2 = getRange(pos, thd);
  r = Rect(range1.start, range2.start, range1.size(), range2.size());
  cv::imshow("bin2", img(r));
  //waitKey();
	for (; *str;) {
		int n, c = *str++;
    int w = r.width, h = r.height;
		if (*str == 0) { assert(0); }
		n = (*str++) - '0';
		GetTextProjection(img(r), pos, c);
		int mini = n*pos.size() / 10;
		int range = pos.size() / 10;
		mini = getMinPos(pos, mini, range);
		switch (c) {
    case 'V':
      out.push_back(Rect(r.x, r.y, mini, h));
      r = Rect(r.x+mini, r.y, w - mini, h);
      break;
    case 'v':
      out.push_back(Rect(r.x+mini, r.y, w - mini, h));
      r = Rect(r.x, r.y, mini, h);
      break;
    case 'H':
      out.push_back(Rect(r.x, r.y, w, mini));
      r = Rect(r.x, r.y+mini, w, h - mini);
      break;
    case 'h':
      out.push_back(Rect(r.x, r.y+mini, w, h - mini));
      r = Rect(r.x, r.y, w, mini);
      break;
    default:
      assert(0);
			break;
		}
	}
	out.push_back(r);
	return out;
}

int LoadTextFileList(const string& testfile, std::vector<string>& imgs) {
	FILE* input = NULL;
	input = fopen(testfile.c_str(), "rb");
	if (input) {
		char buf[1024];
		char fname[256];
		for (; fgets(buf, 1024, input) > 0; ) {
			sscanf(buf, "%s", fname);
			imgs.push_back(fname);
		}
		fclose(input);
	}
	return imgs.size();
}

#include "test_rotation_correction.inl"

int text_segmentation()
{
	//test_rotation_correction();
	if (1) {
		std::vector<string> flist;
		LoadTextFileList("E:/OCR_Line/demo_images/list.txt", flist);
		for (int j = 0; j < flist.size(); ++j) {
			Mat img = imread(flist[j], 0);
			//resize(img, img, Size(), 2, 2, INTER_LANCZOS4);
      resize(img, img, Size(800, 600));
      imshow("src", img);
			vector<Rect> chars_set = CutBlock(img, "h7v7");
			//vector<Mat> chars_set = CutSingleChar(img);
			//waitKey();

			for (int i = 0; i < chars_set.size(); i++)
			{
				char buf[256];
				snprintf(buf, 256, "chars_set %d", i);
        Rect r = chars_set[i];
				cv::imshow(buf, img(r));
			}

			waitKey();
		}
		return 0;
	}
	const char* ch = "D:/code/pudn/ocr/Id_recognition/id_card9.jpg";
	ch = "E:/OCR_Line/demo_images/002.jpg";
	Mat img = imread(ch, 0);
	imshow("src", img);
	//resize(img, img, Size(), 2, 2, INTER_LANCZOS4);
	vector<Rect> chars_set = CutBlock(img, "h7v7");
	//vector<Mat> chars_set = CutSingleChar(img);
	//waitKey();

	for (int i = 0; i < chars_set.size(); i++)
	{
		char buf[256];
		snprintf(buf, 256, "chars_set %d", i);
		cv::imshow(buf, img(chars_set[i]));
	}

	waitKey();
	return 0;
}