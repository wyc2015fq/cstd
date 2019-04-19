# C++ 实现两个向量之间的夹角 - sinat_31425585的博客 - CSDN博客
2017年07月15日 09:29:01[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：7118
        具体原理可以参考另外一篇博客：[点击打开链接](http://blog.csdn.net/csxiaoshui/article/details/73498340)，实现思想就是，通过计算两个向量的斜率角，然后相减，就得到了夹角，好了，直接上代码！
```cpp
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <math.h>
using namespace cv;
using namespace std;
// 以pt1为基准
float getAngelOfTwoVector(Point2f &pt1, Point2f &pt2, Point2f &c)
{
	float theta = atan2(pt1.x - c.x, pt1.y - c.y) - atan2(pt2.x - c.x, pt2.y - c.y);
	if (theta > CV_PI)
		theta -= 2 * CV_PI;
	if (theta < -CV_PI)
		theta += 2 * CV_PI;
	theta = theta * 180.0 / CV_PI;
	return theta;
}
void main()
{
	Point2f c(0, 0);
	Point2f pt1(0, -1);
	Point2f pt2(-1, 0);
	float theta = getAngelOfTwoVector(pt1, pt2, c);
	cout << "theta: " << theta << endl;
}
```
打完收工！
            
