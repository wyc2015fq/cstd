
# linux安装dlib，关键点检测 - 小花生的博客 - CSDN博客


2018年12月16日 21:32:24[Peanut_范](https://me.csdn.net/u013841196)阅读数：230



###### Linux下安装dlib19.6 Face Landmark Detection
**1、下载dlib**
下载dlib-19.6.zip
**2、 C++编译dlib**
从官网下载的dlib文件如下：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181216212217637.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
**从dlib的根目录执行下面语句即可：**
`>>首先进入dlib的根目录下
>>再执行如下语句：
cd examples  #进入dlib下的examples文件夹
mkdir build  #新建build文件夹，存放cmake编译后的执行文件
cd build     #进入新建好的build文件夹
cmake ..     #cmake编译examples整个文件夹
cmake --build . --config Release`**这只是编译好了examples下的dlib，要想编译整个dlib库，则执行以下代码即可：**
`>>进入dlib根目录下
mkdir buildcd build
cmake ..
make release=1`**Dlib 关键点检测代码实现：**
`#include <dlib/opencv.h>
#include <opencv2/opencv.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
 
using namespace dlib;
using namespace std;
 
int main()
{
	try
	{
		// Load face detection and pose estimation models.
		frontal_face_detector detector = get_frontal_face_detector();
		shape_predictor pose_model;
		deserialize("shape_predictor_68_face_landmarks.dat") >> pose_model;
 
		// Grab a frame
		string path = "XZQ.jpg";
		cv::Mat temp = cv::imread(path);
		
		cv_image<bgr_pixel> cimg(temp);
		// Detect faces 
		std::vector<rectangle> faces = detector(cimg);
		// Find the pose of each face.
		std::vector<full_object_detection> shapes;
		for (unsigned long i = 0; i < faces.size(); ++i)
			shapes.push_back(pose_model(cimg, faces[i]));
		if (!shapes.empty()) {
			for (int i = 0; i < 68; i++) {
				circle(temp, cvPoint(shapes[0].part(i).x(), shapes[0].part(i).y()), 3, cv::Scalar(0, 0, 255), -1);
				//	shapes[0].part(i).x();//68个
			}
		}
		//imshow("Dlib特征点", temp);
		cv::imwrite("saveXZQ_landmark.jpg", temp);
        //cv::waitKey(0); 
		
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
	}
}`**参考CMakeList.txt文件：**
`cmake_minimum_required(VERSION 2.8.4)  #版本最小为2.8.4
PROJECT(dlib_facedetector)   #设置工程名
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2 -DDLIB_JPEG_SUPPORT")
IF(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything")
ELSEIF(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
ENDIF()

# #INCLUDE OPENCV
# include_directories(/home/local/include/opencv/)
# include_directories(/home/local/include/opencv2/)
# include_directories(/home/local/include/)
# link_libraries(opencv_core)
# link_libraries(opencv_highgui)
# link_libraries(opencv_imgproc)
#INCLUDE OPENCV
FIND_PACKAGE(OpenCV REQUIRED)
INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
message(STATUS "Opencv include dir found at ${OpenCV_INCLUDE_DIRS}")

#包含头文件
INCLUDE_DIRECTORIES(/dlib-19.6) #dlib根目录地址
LINK_DIRECTORIES(/dlib-19.6/build/dlib/) #dlib编译后bulid下dlib地址
#生产类似于.exe的可执行文件
ADD_EXECUTABLE(dlib_detector dilib_landmark.cpp)
#链接库
TARGET_LINK_LIBRARIES(dlib_detector dlib ${OpenCV_LIBS})
#TARGET_LINK_LIBRARIES(dlib_detector libjpeg.so)`**编译执行，编译代码如下：**
`>>在自己建的工程文件夹下新建build文件夹
mkdir buildcd build
cmake ..
make -j8 #用make也行`**然后再执行.cpp文件，执行代码如下：**
`>>在build文件夹下继续执行一行代码即可显示结果：
./dlib_detector`**可能出现的问题：**
make出现/usr/bin/ld: 找不到 -lopencv_dep_cudart错误时候
**解决方法：**在cmake时候加上参数即可。
cmake -D CUDA_USE_STATIC_CUDA_RUNTIME=OFF .
**测试结果：**
![在这里插入图片描述](https://img-blog.csdnimg.cn/201812162128187.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
参考：[https://blog.csdn.net/qq_17448289/article/details/56494998](https://blog.csdn.net/qq_17448289/article/details/56494998)
---

###### 注：博众家之所长，集群英之荟萃。

