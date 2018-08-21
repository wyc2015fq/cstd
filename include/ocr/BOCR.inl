
#include <vector>

namespace BOCR {

	using namespace std;
	using namespace cv;
//按照X坐标排序
bool rect_rank_x(vector<Rect> &vec_rects) {
	Rect vec_temp;
	for (int l = 1; l < vec_rects.size(); l++) {
		for (int m = vec_rects.size() - 1; m >= l; m--) {
			if (vec_rects[m].x < vec_rects[m - 1].x) {
				vec_temp = vec_rects[m - 1];
				vec_rects[m - 1] = vec_rects[m];
				vec_rects[m] = vec_temp;
			}
		}
	}
	return true;
}
//按照X坐标排序
bool rect_rank_y(vector<Rect> &vec_rects) {
	Rect vec_temp;
	for (int l = 1; l < vec_rects.size(); l++) {
		for (int m = vec_rects.size() - 1; m >= l; m--) {
			if (vec_rects[m].y < vec_rects[m - 1].y) {
				vec_temp = vec_rects[m - 1];
				vec_rects[m - 1] = vec_rects[m];
				vec_rects[m] = vec_temp;
			}
		}
	}
	return true;
}



/*将rect上下合并
* 参数：vec_rects：输入的所有的rect集合;
*      vec_rects_out:输出的上下合并后的所有的rect集合;
*      x_dif：进行上下合并的x差值;y_dif：进行上下合并的y差值;
*      width:进行上下合并的width最大值;height:进行上下合并的height最大值;
width_rect:合并后的rect的width的值大于width_rect为满足条件
*/
bool rect_combine_uplow(vector<Rect> &vec_rects,
	vector<Rect>&vec_rects_out, int x_dif, int y_dif, int width, int height,
	int width_rect) {
	rect_rank_y(vec_rects);
	//将上下部分分裂的，合并
	int num_rect = vec_rects.size();
	for (int j = 0; j < num_rect; j++) {
		if (vec_rects[j].width > 0) {
			Rect r;
			for (int p = 0; p < num_rect; p++) {
				if ((vec_rects[p].width > 0) && (p > j || p < j)) {
					if ((((abs(vec_rects[p].x - vec_rects[j].x) < x_dif)
						|| (abs(
							vec_rects[p].x + vec_rects[p].width
							- vec_rects[j].x
							- vec_rects[j].width) < x_dif))
						&& ((abs(
							vec_rects[p].y
							- (vec_rects[j].y
								+ vec_rects[j].height))
							< y_dif)
							|| (abs(
								vec_rects[j].y
								- (vec_rects[p].y
									+ vec_rects[p].height))
								< y_dif))
						&& (vec_rects[p].height < height)
						&& (vec_rects[j].height < height)
						&& (vec_rects[p].width < width)
						&& (vec_rects[j].width < width))) {

						r.x = min(vec_rects[j].x, vec_rects[p].x);
						r.y = min(vec_rects[j].y, vec_rects[p].y);
						r.width = max(
							vec_rects[p].x + vec_rects[p].width
							- vec_rects[j].x,
							vec_rects[j].x + vec_rects[j].width
							- vec_rects[p].x);
						r.height = max(
							vec_rects[j].y + vec_rects[j].height
							- vec_rects[p].y,
							vec_rects[p].y + vec_rects[p].height
							- vec_rects[j].y);
						if (vec_rects[p].y < vec_rects[j].y) {
							vec_rects[p].width = 0;
							vec_rects[p].x = 0;
							vec_rects[p].height = 0;
							vec_rects[p].y = 0;
							vec_rects[j] = r;
						}
						else {
							vec_rects[j].width = 0;
							vec_rects[j].x = 0;
							vec_rects[j].height = 0;
							vec_rects[j].y = 0;
							vec_rects[p] = r;
						}

					}
				}
			}
		}
	}

	for (int j = 0; j < num_rect; j++) {
		if (vec_rects[j].width > width_rect) {
			vec_rects_out.push_back(vec_rects[j]);
		}
	}
	return true;
}

/*将rect左右合并
* 参数：
* show: 输入图像;
* vec_rects：输入的所有的rect集合;
* vec_rects_out: 输出的左右合并后的所有的rect集合;
* x_dif：进行左右合并的x差值;
* y_dif：进行左右合并的y差值;
* width: 进行左右合并的width最大值;
* height: 进行左右合并的height最大值;
* rate1：rect的长宽比最小值1;
* rate2: rect的长宽比最小值2;
* width_rect: 合并后的rect的width的值大于width_rect为满足条件
*/
bool rect_combine_leftright(Mat & show, vector<Rect> &vec_rects,
	vector<Rect>&vec_rects_out, int x_dif, int y_dif, int width, int height,
	double rate1, double rate2, int width_rect) {
	int num = vec_rects.size();
	for (int j = 0; j < num - 1; j++) {
		if (vec_rects[j].width > 0) {
			for (int q = j + 1; q < num; q++) {
				if (vec_rects[q].width > 0) {
					Rect r;
					if ((max(vec_rects[q].x - x_dif, 0)
						< min(vec_rects[j].x + vec_rects[j].width,
							show.cols))
						&& ((abs(vec_rects[q].y - vec_rects[j].y) < y_dif)
							|| (abs(
								min(
									vec_rects[q].y
									+ vec_rects[q].height,
									show.rows)
								- min(
									vec_rects[j].y
									+ vec_rects[j].height,
									show.rows)) < y_dif))
						&& (vec_rects[q].width < width)
						&& (vec_rects[j].width < width)
						&& (((vec_rects[q].height
							/ (double)vec_rects[q].width > rate1)
							&& (vec_rects[j].height
								/ (double)vec_rects[j].width
						> rate2))
							|| ((vec_rects[j].height
								/ (double)vec_rects[j].width
									> rate1)
								&& (vec_rects[q].height
									/ (double)vec_rects[q].width
						> rate2)))) {
						if ((vec_rects[j].x + vec_rects[j].width
						> show.cols / 10 * 8.5)
							&& (vec_rects[q].x > show.cols / 10 * 8.5)
							&& abs(vec_rects[j].width - vec_rects[q].width)
							< 4
							&& abs(
								vec_rects[j].height
								- vec_rects[q].height) < 3) {
							;
						}
						else {
							r.x = vec_rects[j].x;
							r.y = min(vec_rects[j].y, vec_rects[q].y);
							r.width = vec_rects[q].x + vec_rects[q].width
								- vec_rects[j].x;
							r.height = max(vec_rects[j].y + vec_rects[j].height,
								vec_rects[q].y + vec_rects[q].height) - r.y;
							vec_rects[q].width = 0;
							vec_rects[q].x = 0;
							vec_rects[j] = r;
						}
					}
				}
			}
		}
	}
	for (int j = 0; j < num; j++) {
		if (vec_rects[j].width > width_rect) {
			vec_rects_out.push_back(vec_rects[j]);
		}
	}
	return true;
}

Rect UnionRect(Rect r1, Rect r2) {
	int l = min(r1.x, r2.x);
	int r = max(r1.x+ r1.width, r2.x+r2.width);
	int t = min(r1.y, r2.y);
	int b = max(r1.y + r1.height, r2.y+ r2.height);
	return cv::Rect(l, t, r-l, b-t);
}

std::vector<Rect> rect_combine_by_index(const std::vector<Rect> &vec_rects, const std::vector<int> &index, std::vector<int>& cnt) {
	int i, j;
	int max_index = 0;
	std::vector<Rect> vec_rects_out;
	for (i = 0; i < index.size(); ++i) {
		max_index = std::max(max_index, index[i]);
	}
	vec_rects_out.resize(max_index + 1);
	cnt.resize(max_index + 1);
	for (i = 0; i < vec_rects_out.size(); ++i) {
		vec_rects_out[i] = cv::Rect(0, 0, 0, 0);
		cnt[i] = 0;
	}
	for (i = 0; i < index.size(); ++i) {
		j = index[i];
		++cnt[j];
		if (vec_rects_out[j].width>0) {
		  vec_rects_out[j] = UnionRect(vec_rects_out[j], vec_rects[i]);
		}
		else {
			vec_rects_out[j] = vec_rects[i];
		}
	}

	return vec_rects_out;
}

std::vector<Rect> getline(const std::vector<Rect> &vec_rects, const std::vector<int> &index, int id) {
	int i;
	std::vector<Rect> vec_rects_out;
	for (i = 0; i < index.size(); ++i) {
		if (id == index[i]) {
			vec_rects_out.push_back(vec_rects[i]);
		}
	}
	return vec_rects_out;
}

int rect_combine_in_line(std::vector<Rect>& vec_rects, std::vector<int>& index, int max_width) {
	int i, j = 0;
	rect_rank_x(vec_rects);
	index.resize(vec_rects.size());
	int x_cur = vec_rects[0].x;
	for (i = 0; i < index.size(); ++i) {
		Rect rect = vec_rects[i];
		int w = rect.x + rect.width - x_cur;
		if (w > max_width) {
			++j;
			x_cur = vec_rects[i].x;
		}
		index[i] = j;

	}
	++j;
	return j;
}


} // namespace BOCR
