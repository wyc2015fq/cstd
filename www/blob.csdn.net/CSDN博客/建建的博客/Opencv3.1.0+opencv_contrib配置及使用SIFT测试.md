# Opencv3.1.0+opencv_contrib配置及使用SIFT测试 - 建建的博客 - CSDN博客
2018年09月20日 11:08:14[纪建](https://me.csdn.net/u013898698)阅读数：449
因为需要用到一些比较新的跟踪算法，这两天装了opencv3.1并配置了opencv_contrib，并使用了SIFT算法测试是否配置成功。 
**1.opencv3.1安装与配置**
这里不多言，不熟悉的可以参考浅墨的博客：[http://blog.csdn.net/poem_qianmo/article/details/19809337](http://blog.csdn.net/poem_qianmo/article/details/19809337)
**2.opencv_contrib安装与配置**
从opencv3以来，一些比较新的功能都挪到了“opencv_contrib”库里。配置这个库需要重新编译opencv，关于此部分可以参考教程：[http://blog.csdn.net/linshuhe1/article/details/51221015](http://blog.csdn.net/linshuhe1/article/details/51221015)
关于此教程需要补充两点：**A,**使用cmake编译的过程中经常会失败，因为国内网络问题ippicv_windows_20151201.zip 文件下载失败导致，可以直接从这里下载：[http://download.csdn.net/detail/qjj2857/9495013](http://download.csdn.net/detail/qjj2857/9495013)**B.**教程最后配置包含目录、库目录时没有提及添加环境变量，这里也是同样需要的。还有一切配置完成后别忘了重启电脑哟。 
**3.写个程序测试一下配置是否成功吧**
opencv3.1中SIFT匹配是在opencv_contrib库中的，这里我们就用它来做一个简单的测试。 
参考： 
1. cv::xfeatures2d::SIFT Class Reference：[http://docs.opencv.org/3.1.0/d5/d3c/classcv_1_1xfeatures2d_1_1SIFT.html#gsc.tab=0](http://docs.opencv.org/3.1.0/d5/d3c/classcv_1_1xfeatures2d_1_1SIFT.html#gsc.tab=0)
2. OpenCV3.1 xfeatures2d::SIFT 使用：[http://blog.csdn.net/lijiang1991/article/details/50855279](http://blog.csdn.net/lijiang1991/article/details/50855279)
程序：
```
#include <iostream>
#include <opencv2/opencv.hpp>  //头文件
#include <opencv2/xfeatures2d.hpp>
using namespace cv;  //包含cv命名空间
using namespace std;
int main()
{
    //Create SIFT class pointer
    Ptr<Feature2D> f2d = xfeatures2d::SIFT::create();
    //读入图片
    Mat img_1 = imread("1.jpg");
    Mat img_2 = imread("2.jpg");
    //Detect the keypoints
    vector<KeyPoint> keypoints_1, keypoints_2;
    f2d->detect(img_1, keypoints_1);
    f2d->detect(img_2, keypoints_2);
    //Calculate descriptors (feature vectors)
    Mat descriptors_1, descriptors_2;
    f2d->compute(img_1, keypoints_1, descriptors_1);
    f2d->compute(img_2, keypoints_2, descriptors_2);    
    //Matching descriptor vector using BFMatcher
    BFMatcher matcher;
    vector<DMatch> matches;
    matcher.match(descriptors_1, descriptors_2, matches);
    //绘制匹配出的关键点
    Mat img_matches;
    drawMatches(img_1, keypoints_1, img_2, keypoints_2, matches, img_matches);
    imshow("【match图】", img_matches);
    //等待任意按键按下
    waitKey(0);
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
原始图片： 
![这里写图片描述](https://img-blog.csdn.net/20160811141502761)
![这里写图片描述](https://img-blog.csdn.net/20160811141512964)
匹配结果： 
![这里写图片描述](https://img-blog.csdn.net/20160811140733948)
*———————————-2016/8/12———————————*
1.关于Ubuntu下opencv3.1及opencv_contrib的安装与配置可参考： 
官网：[Installation in Linux](http://docs.opencv.org/3.1.0/d7/d9f/tutorial_linux_install.html)
[http://www.cnblogs.com/asmer-stone/p/5089764.html](http://www.cnblogs.com/asmer-stone/p/5089764.html)
上博文中有两点需要注意： 
A.按上文参考所述，第3步build文件夹需建在~/opencv/opencv文件夹中；且cmake时按照作者示例OPENCV_EXTRA_MODULES_PATH=~/opencv/opencv_contrib/modules ,注意”<>”需要去掉；末尾的`..` 表示opencv源码在上一级目录中。当然如果你了解cmake的使用方法`cmake [optional] <opencv source directory>`, 可以任意设置文件夹目录。 
B.与在windows下相同，cmake时会因为“ippicv_linux_20151201.tgz 无法下载”而导致失败。我们可以从[http://download.csdn.net/download/lx928525166/9479919](http://download.csdn.net/download/lx928525166/9479919)下载，并放入相应文件夹中。 
2. 好，现在假设你已经安装配置好了。由于在windows下我们习惯了用一个IDE来编程，这里在Ubuntu下我选择使用eclipse来作为编程环境，下边简单说一下怎么在eclipse中配置opencv。 
首先参考官网： [http://docs.opencv.org/3.1.0/d7/d16/tutorial_linux_eclipse.html](http://docs.opencv.org/3.1.0/d7/d16/tutorial_linux_eclipse.html) 你就应该能配置的差不多了，或者其他类似的吧网上一大堆。 
但是中间可能会出现一些小问题，我个人配置的时候出现了两个小问题： 
A. 错误 undefined reference to symbol ‘_ZN2cv6imreadERKNS_6StringEi’ ，参考：[http://answers.opencv.org/question/46755/first-example-code-error/](http://answers.opencv.org/question/46755/first-example-code-error/)
B. 错误 error while loading shared libraries: libopencv_core.so.3.0: cannot open shared object file: No such file or directory ，参考：[http://stackoverflow.com/questions/27907343/error-while-loading-shared-libraries-libopencv-core-so-3-0](http://stackoverflow.com/questions/27907343/error-while-loading-shared-libraries-libopencv-core-so-3-0)
3. 所有配置均完成后，在上述windows下的代码可以在这里直接运行。见下图： 
![这里写图片描述](https://img-blog.csdn.net/20160812161558436)
4.关于在ubuntu下运行其他samples程序。这里以cpp为例，直接找到opencv/samples/cpp/example_cmake，这里有一个示例已经提供了Makefile文件，make一下即可生成可执行文件。其他cpp示例文件类似。
