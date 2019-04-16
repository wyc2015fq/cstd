# OpenCV3.1中读写图像与读写像素 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2016年09月13日 11:05:27[gloomyfish](https://me.csdn.net/jia20003)阅读数：3778








**OpenCV3.1中读写图像与读写像素**


一：读图像，显示到窗口



从本地目录读写一张RGB图像到内存对象Mat中并把它显示到指定窗口。

相关函数：

- imread 加载图像文件

- imshow 显示图像

- namedWindow 创建窗口

相关代码：

```cpp
#include<opencv2\opencv.hpp>
#include<highgui.h>
using namespace cv;
int main(int argc, char** argv)
{
	// read image
	Mat picture = imread("test.jpg");
	// create window
	namedWindow("My Test", CV_WINDOW_AUTOSIZE);
	// display
	imshow("My Test", picture);

	// 关闭
	waitKey(0);
	destroyWindow("My Test");
	destroyWindow("My Gray Test");
	return 0;
}
```

运行结果：
![](https://img-blog.csdn.net/20160913105546708)


**二：写图像文件到本地**

将加载以后的RGB图像转换为灰度图像，然后保存到本地的目录路径。

相关函数：

- cvtColor 将图像从RGB彩色图像转换为灰度图像

-imwrite 将内存中Mat对象图像保存为本地图像文件。

相关代码：

```cpp
#include<opencv2\opencv.hpp>
#include<highgui.h>
using namespace cv;
int main(int argc, char** argv)
{
	// read image
	Mat picture = imread("test.jpg");

	// 转换为灰度图像
	Mat gray_image;
	cvtColor(picture, gray_image, COLOR_BGR2GRAY);
	imwrite("D:/gloomyfish/graytest.jpg", gray_image);

	// create windows
	namedWindow("My Test", CV_WINDOW_AUTOSIZE);
	namedWindow("My Gray Test", CV_WINDOW_AUTOSIZE);

	// display image
	imshow("My Test", picture);
	imshow("My Gray Test", gray_image);

	// 关闭
	waitKey(0);
	destroyWindow("My Test");
	destroyWindow("My Gray Test");
	return 0;
}
```

运行结果：会把看到这张灰度图像保存到D盘gloomyfish文件夹下：

![](https://img-blog.csdn.net/20160913105718068)


**三：读写像素**

从Mat中读出每个像素值，与255取反值然后重新写入到Mat中，并在窗口中显示结果。首先要获取通道数据，检测到图像是ＲＧＢ三通道还是单通道图像，然后获取Mat中的像素块指针，从上到下，从左到右访问每个像素即可。灰度图像Mat中像素数据存储
![](https://img-blog.csdn.net/20160913105812694)



Mat中彩色RGB图像数据存储：
![](https://img-blog.csdn.net/20160913105853085)

相关函数：

Mat.copyTo 把图像的数据从当前的Mat对象拷贝到一个新的Mat对象中去

Mat.channels 返回图像的通道数

相关代码：

```cpp
#include<opencv2\opencv.hpp>
#include<highgui.h>
using namespace cv;
int main(int argc, char** argv)
{
	// read image
	Mat image = imread("test.jpg");

	// 对图像进行所有像素用 （255- 像素值）
	Mat invertImage;
	image.copyTo(invertImage);

	// 获取图像宽、高
	int channels = image.channels();
	int rows = image.rows;
	int cols = image.cols * channels;
	if (image.isContinuous()) {
		cols *= rows;         
		rows = 1;
	}

	// 每个像素点的每个通道255取反
	uchar* p1;
	uchar* p2;
	for (int row = 0; row < rows; row++) {
		p1 = image.ptr<uchar>(row);// 获取像素指针
		p2 = invertImage.ptr<uchar>(row);
		for (int col = 0; col < cols; col++) {
			*p2 = 255 - *p1; // 取反
			p2++;
			p1++;
		}
	}

	// create windows
	namedWindow("My Test", CV_WINDOW_AUTOSIZE);
	namedWindow("My Invert Image", CV_WINDOW_AUTOSIZE);

	// display image
	imshow("My Test", image);
	imshow("My Invert Image", invertImage);

	// 关闭
	waitKey(0);
	destroyWindow("My Test");
	destroyWindow("My Invert Image");
	return 0;
}
```

显示取反之后图像:
![](https://img-blog.csdn.net/20160913110031854)
**欢迎继续关注本人博客，加入图像处理QQ群讨论学习！**





