# Seetaface编译与测试 - sinat_31425585的博客 - CSDN博客
2018年07月10日 10:58:08[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：259
    seetaFace人脸检测引擎是中科院计算所山世光研究员所带领人脸识别研究团队所研发并开源，主要提供了三个引擎：人脸检测（FaceDetection）、人脸对齐（FaceAlignment）、人脸识别（FaceDetection）。
    1、编译：
    具体编译方式，可以参考[seetafaceEngine](https://github.com/seetaface/SeetaFaceEngine)，里面自带一个配置文档，按照文档一步步来就好了，最后可以生成3个dll文件和3个lib文件：    
```
FaceIdentification.lib  FaceIdentification.dll
FaceAlignment.lib   FaceAlignment.dll
FaceDetection.lib    FaceDetection.dll
```
    2、配置：
    感觉什么东西都可以向opencv上靠，因此，首先，我将这六个库文件中，dll文件和lib文件分别放到opencv对应bin目录和lib目录：
![](https://img-blog.csdn.net/20180710104725342?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180710104850337?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
    然后，在include文件夹下新建seetaface文件夹：
![](https://img-blog.csdn.net/2018071010505739?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
 其次，将三个引擎中的include文件夹复制过来，并分别新建三个子文件夹用于区分三个引擎：
![](https://img-blog.csdn.net/20180710105334678?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
最后，就是配置，只需要像配置opencv一样，将三个include路径添加到配置文件中即可：
测试代码如下：
```cpp
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <face_detection.h>
#include <face_alignment.h>
#ifdef _WIN32
std::string DATA_DIR = ".//data//";
std::string MODEL_DIR = ".//model//";
#else
std::string DATA_DIR = ".//data//";
std::string MODEL_DIR = ".//model//";
#endif
int testImage()
{
	// Initialize face detection model
	seeta::FaceDetection detector(".//model//seeta_fd_frontal_v1.0.bin");
	detector.SetMinFaceSize(40);
	detector.SetScoreThresh(2.f);
	detector.SetImagePyramidScaleFactor(0.8f);
	detector.SetWindowStep(4, 4);
	// Initialize face alignment model 
	seeta::FaceAlignment point_detector((MODEL_DIR + "seeta_fa_v1.1.bin").c_str());
	cv::Mat gray = cv::imread((DATA_DIR + "image_0001.png").c_str(), 0);
	cv::Mat img = cv::imread((DATA_DIR + "image_0001.png").c_str(), 1);
	int pts_num = 5;
	int im_width = gray.cols;
	int im_height = gray.rows;
	seeta::ImageData image_data;
	image_data.data = gray.data;
	image_data.width = im_width;
	image_data.height = im_height;
	image_data.num_channels = 1;
	// Detect faces
	std::vector<seeta::FaceInfo> faces = detector.Detect(image_data);
	cv::Rect face_rect;
	int32_t face_num = static_cast<int32_t>(faces.size());
	for (int32_t i = 0; i < face_num; i++) {
		face_rect.x = faces[i].bbox.x;
		face_rect.y = faces[i].bbox.y;
		face_rect.width = faces[i].bbox.width;
		face_rect.height = faces[i].bbox.height;
		cv::rectangle(img, face_rect, cv::Scalar(255, 0, 0), 4, 8, 0);
	}
	// Detect 5 facial landmarks
	seeta::FacialLandmark points[5];
	point_detector.PointDetectLandmarks(image_data, faces[0], points);
	// Visualize the results
	//cv::rectangle(img, cv::Point(faces[0].bbox.x, faces[0].bbox.y), cv::Point(faces[0].bbox.x + faces[0].bbox.width - 1, faces[0].bbox.y + faces[0].bbox.height - 1), CV_RGB(255, 0, 0));
	for (int i = 0; i < pts_num; i++)
	{
		cv::circle(img, cv::Point(points[i].x, points[i].y), 5, cv::Scalar(0, 0, 255), CV_FILLED);
	}
	 
	cv::namedWindow("src", cv::WINDOW_AUTOSIZE);
	cv::imshow("src", img);
	cv::waitKey(0);
	
	return 0;
}
int main()
{
	testImage();
}
```
效果如下：
![](https://img-blog.csdn.net/20180710105752453?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMxNDI1NTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
