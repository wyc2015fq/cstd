# 相机模型与标定（四）--opencv单目标定例子使用说明 - 工作笔记 - CSDN博客





2016年07月25日 10:44:15[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：8947
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









原文：

http://blog.csdn.net/t247555529/article/details/47836233

最近一个项目要进行相机的标定，作为一个菜鸟，瞎搞一下午才搞定，于是写篇博客记录下~

参考资料：(学习笔记)摄像机模型与标定——一次完成标定  http://www.xuebuyuan.com/1586576.html 




OpenCV sample目录下自带两个与相机标定的cpp文件即：calibration.cpp和calibration_artificial.cpp

calibration.cpp 是通过用户输入可选参数进行摄像机标定的程序。

calibration_artificial.cpp 是程序模拟模拟摄像机标定的过程，即程序创建棋盘然后自主标定。



通常我们有两种方式去标定，一个是实时的从摄像机获取拍摄到的棋盘进行标定，一个是已经获得了一些棋盘的图片来标定。

本文详细介绍第二种，即用已获得的棋盘图片的整个过程。

方法一：编译生成 .exe文件，然后在commond 窗口输入参数执行

**方法二：通过添加输入参数的代码，然后执行（可以单步调试）（推荐此方法）**



calibration.cpp 和 calibration_artificial.cpp 位置如图所示。此外，棋盘图片也在同一目录下


![](https://img-blog.csdn.net/20150821162239342?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20150821163037489?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





找到文件后，开始新建工程吧。不过在用以获得的图像来进行标定，应先通过OpenCV自带的imagelist_creator.cpp 产生一个xml或者yaml格式的图像列表，然后在使用下面的程序。

第一步，新建项目demo,添加刚才那个目录下的imagelist_creator.cpp 
 编译执行得到可执行文件 demo.exe

![](https://img-blog.csdn.net/20150821164627091?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20150821164744272?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





第二步，执行demo.exe 产生一个xml或者yaml格式的图像列表

static void help(char** av)

{

  cout << "\nThis creates a yaml or xml list of files from the command line args\n"

      "usage:\n./" << av[0] << " imagelist.yaml *.png\n"

      << "Try using different extensions.(e.g. yaml yml xml xml.gz etc...)\n"

      << "This will serialize this list of images or whatever with opencv's FileStorage framework" << endl;

}

输入命令：demo imagelist.yaml
  left01.jpg left02.jpg left03.jpg left04.jpg left05.jpg left06.jpg left07.jpg left08.jpg left09.jpg left010.jpg left11.jpg left12.jpg left13.jpg left14.jpg right01.jpg right02.jpg right03.jpg right04.jpg right05.jpg right06.jpg right07.jpg right08.jpg right09.jpg
 right10.jpg right11.jpg right12.jpg right13.jpg right14.jpg  。回车，生成imagelist.yaml

![](https://img-blog.csdn.net/20150821164854318?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





第三步，进行相机的标定

先把14张棋盘图片放到debug目录下，然后移除imagelist_creator.cpp 添加 calibration.cpp
 ，如图所示

![](https://img-blog.csdn.net/20150821165744616?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





编译执行，得到新的demo.exe ,进入cmd ，输入 demo -w 6 -h 9 imagelist.yaml  回车即可。如图所示，开始对每张图片进行角点检测

![](https://img-blog.csdn.net/20150821170024771?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


最后，得到一个 out_camera_data.yml 文件

![](https://img-blog.csdn.net/20150821170156694?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





内容如下：

%YAML:1.0

calibration_time: "08/21/15 16:54:26"

image_width: 640

image_height: 480

board_width: 6

board_height: 9

square_size: 1.

flags: 0

camera_matrix: !!opencv-matrix

   rows: 3

   cols: 3

   dt: d

   data: [ 5.3765976500165073e+002, 0., 3.4011155767874686e+002, 0.,

       5.3789341813113867e+002, 2.3694081464807104e+002, 0., 0., 1. ]

distortion_coefficients: !!opencv-matrix

   rows: 5

   cols: 1

   dt: d

   data: [ -2.7762353155161251e-001, 5.3976467600878486e-002,

       2.1257384355991209e-003, -3.9487502777272009e-004,

       4.8679847473271927e-002 ]

avg_reprojection_error: 4.4034956116049290e-001





此时，大功告成 =。= 

//-----------------------------------------------------------------------------------我是快乐的分割线----------------------------------------------------------------------------------//

//对于想自己单步调试的亲，补充第二种方法，即通过添加代码调试。

第一步：将图片及imagelist放到 D:\Workspace\demo\demo目录下

第二步: 添加代码

具体过程如图所示：

![](https://img-blog.csdn.net/20150823121408545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


在该目录下放入imagelist.yaml 和14张图片

![](https://img-blog.csdn.net/20150823121434652?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





添加代码

        argc = 6;
argv[0] = "demo.exe";
argv[1] = "-w";
argv[2] = "6";
argv[3] = "-h";
argv[4] = "9";
argv[5] = "imagelist.yaml";





![](https://img-blog.csdn.net/20150823121710405?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)








得到的结果是相同的哈，而且可以调试哈~（方法二补充于2015.8.23）





