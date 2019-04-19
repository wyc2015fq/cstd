# opencv 中 最小二乘直线拟合fitline测试 - sinat_31425585的博客 - CSDN博客
2017年09月14日 22:08:15[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：497
最近使用opencv中自带的最小二乘直线拟合fitline接口，先测试一下：
测试代码如下：
```cpp
#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
using namespace cv;
using namespace std;
int main()
{
	vector<Point> pts;
	for (int i = 0; i < 5; i++)
	{
		int j = 2 * i;
		Point Point_t = Point(i, j);
		pts.push_back(Point_t);
	}
	Vec4f line_t;
	fitLine(Mat(pts), line_t, CV_DIST_L2, 0, 0.01, 0.01);
	cout << " " << line_t[0] << " " << line_t[1] << " " << line_t[2] << " " << line_t[3] << endl;
	return 0;
}
```
输出结果：
![](https://img-blog.csdn.net/20170914221022983?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzE0MjU1ODU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
可以看出来，fitline返还四个参数分别对应a*x+b*y+c = 0中a和b以及位于直线上一点的x,y坐标，测试完毕！
