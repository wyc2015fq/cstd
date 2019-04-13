
# Windows下安装dlib19.6，关键点检测 - 小花生的博客 - CSDN博客


2018年12月16日 22:09:33[Peanut_范](https://me.csdn.net/u013841196)阅读数：72



###### Windows下安装dlib19.6 Face Landmark Detection
### 1.测试环境
Windows10
Visual Studio 2015
dlib-19.6
Windows安装还需要cmake：[https://cmake.org/download/](https://cmake.org/download/)
### 2.用CMake编译dlib-19.6
1）.首先打开cmake，where is the source code 选择dlib-19.6文件夹中的dlib目录，e.g. D:\dlib\dlib-19.6\dlib，需要根据实际情况更改目录。
2）.where to build the binaries 选择需要存放生成文件的目录，e.g. D:\dlib\build，最好是纯英文。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181216214629559.png)
3）.点击configure，第一次使用会弹出对话框，选择对应的vs版本即可，其他的不用管。e.g. vs2015 64位，选择的是vs2015 Win64，之后点确认，程序开始运行，运行完成后会在最下方显示configuration done.
4）.点击generate。然后直接在cmake面板点击open project打开项目。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181216214742389.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
3.用VS2015生成解决方案
属性配置:
1）在C/C++ =》常规=》附加包含目录下添加如下的文件夹的路径(自动配置)
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018121621481671.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
2）配置属性→ C/C++ →预处理器 中的预处理器定义（自动配置）
DLIB_JPEG_SUPPORT
DLIB_PNG_SUPPORT
DLIB_JPEG_STATIC![在这里插入图片描述](https://img-blog.csdnimg.cn/20181216214939645.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
3）完成上述操作后。重新编译项目生成dlib.lib文件。如图分别为debug和release版本的情况。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181216215004190.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181216215034376.png)
[
](https://img-blog.csdnimg.cn/20181216215004190.png)4.验证dlib.lib是否正确
[
](https://img-blog.csdnimg.cn/20181216215004190.png)VS2015，新建win32控制台应用程序。把examples中的face_landmark_detection_ex.cpp内容复制即可。
需要修改的属性：
1）配置属性 C/C++ → 常规中的附加包含目录
添加dlib源码所在目录和源码目录/dlib/external中zlib、libpng和libjpeg总计四个文件目录。
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018121621510457.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
2）在C/C++ →预处理器→ 预处理定义 中添加：
DLIB_PNG_SUPPORT
DLIB_JPEG_SUPPORT![在这里插入图片描述](https://img-blog.csdnimg.cn/20181216215139537.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
3）在链接器→常规→附加库目录，添加编译好的64位Debug版本的dlib.lib的路径
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181216215204442.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
4）把.\dlib-19.6\dlib\all这个路径下的source.cpp文件加入到工程中(以添加现有项的方式加入)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181216215233910.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
5）把.\dlib-19.6\dlib\external这个路径下的libjpeg、libpng、zlib里面的文件都添加到资源文件。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181216215301601.png)
6）编译工程，即可完成。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181216215328210.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
可能遇到的问题：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181216215355372.png)
解决方法：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181216215416344.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
关键点参考代码：
\#include <dlib/image_processing/frontal_face_detector.h>
\#include <dlib/image_processing/render_face_detections.h>
\#include <dlib/image_processing.h>
\#include <dlib/gui_widgets.h>
\#include <dlib/image_io.h>
\#include <iostream>
using namespace dlib;
using namespace std;
// ----------------------------------------------------------------------------------------
int main(int argc, char** argv)
{
	try
	{
		// This example takes in a shape model file and then a list of images to
		// process.  We will take these filenames in as command line arguments.
		// Dlib comes with example images in the examples/faces folder so give
		// those as arguments to this program.
		if (argc == 1)
		{
			cout << "Call this program like this:" << endl;
			cout << "./face_landmark_detection_ex shape_predictor_68_face_landmarks.dat faces/*.jpg" << endl;
			cout << "\nYou can get the shape_predictor_68_face_landmarks.dat file from:\n";
			cout << "http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2" << endl;
			return 0;
		}
		// We need a face detector.  We will use this to get bounding boxes for
		// each face in an image.
		frontal_face_detector detector = get_frontal_face_detector();
		// And we also need a shape_predictor.  This is the tool that will predict face
		// landmark positions given an image and face bounding box.  Here we are just
		// loading the model from the shape_predictor_68_face_landmarks.dat file you gave
		// as a command line argument.
		shape_predictor sp;
		deserialize(argv[1]) >> sp;
		image_window win, win_faces;
		// Loop over all the images provided on the command line.
		for (int i = 2; i < argc; ++i)
		{
			cout << "processing image " << argv[i] << endl;
			array2d<rgb_pixel> img;
			load_image(img, argv[i]);
			// Make the image larger so we can detect small faces.
			pyramid_up(img);
			// Now tell the face detector to give us a list of bounding boxes
			// around all the faces in the image.
			std::vector<rectangle> dets = detector(img);
			cout << "Number of faces detected: " << dets.size() << endl;
			// Now we will go ask the shape_predictor to tell us the pose of
			// each face we detected.
			std::vector<full_object_detection> shapes;
			for (unsigned long j = 0; j < dets.size(); ++j)
			{
				full_object_detection shape = sp(img, dets[j]);
				cout << "number of parts: " << shape.num_parts() << endl;
				cout << "pixel position of first part:  " << shape.part(0) << endl;
				cout << "pixel position of second part: " << shape.part(1) << endl;
				// You get the idea, you can get all the face part locations if
				// you want them.  Here we just store them in shapes so we can
				// put them on the screen.
				shapes.push_back(shape);
			}
			// Now let's view our face poses on the screen.
			win.clear_overlay();
			win.set_image(img);
			win.add_overlay(render_face_detections(shapes));
			// We can also extract copies of each face that are cropped, rotated upright,
			// and scaled to a standard size as shown here:
			dlib::array<array2d<rgb_pixel> > face_chips;
			extract_image_chips(img, get_face_chip_details(shapes), face_chips);
			win_faces.set_image(tile_images(face_chips));
			cout << "Hit enter to process the next image..." << endl;
			cin.get();
		}
	}
	catch (exception& e)
	{
		cout << "\nexception thrown!" << endl;
		cout << e.what() << endl;
	}
}注：
需要先下载关键点检测文件：
[http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2](http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2)
在调试中填入命令参数，e.g.：shape_predictor_68_face_landmarks.dat XZQ.jpg
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181216215517720.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
输出结果：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018121621555375.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
参考：
[https://www.cnblogs.com/www-caiyin-com/p/7484390.html](https://www.cnblogs.com/www-caiyin-com/p/7484390.html)
[https://blog.csdn.net/dawnfox/article/details/77282246](https://blog.csdn.net/dawnfox/article/details/77282246)
---
注：博众家之所长，集群英之荟萃。

