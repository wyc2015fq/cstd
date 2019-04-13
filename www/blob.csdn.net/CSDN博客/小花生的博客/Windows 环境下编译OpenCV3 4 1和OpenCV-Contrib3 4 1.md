
# Windows 环境下编译OpenCV3.4.1和OpenCV-Contrib3.4.1 - 小花生的博客 - CSDN博客


2018年12月16日 20:45:12[Peanut_范](https://me.csdn.net/u013841196)阅读数：440



###### Windows 环境下编译OpenCV3.4.1和OpenCV-Contrib3.4.1
## 1.测试环境
Windows7
Visual Studio 2015
OpenCV3.4.1+OpenCV-Contrib3.4.1
OpenCV3.4.1：[https://github.com/opencv/opencv/releases](https://github.com/opencv/opencv/releases)
OpenCV-Contrib3.4.1：[https://github.com/opencv/opencv_contrib/releases/tag/3.4.1](https://github.com/opencv/opencv_contrib/releases/tag/3.4.1)
**注意**：版本一定要一一对应！
Windows安装还需要cmake：[https://cmake.org/download/](https://cmake.org/download/)
## 2.为什么要安装OpenCV_contrib？
其中包括了人脸检测、识别、关键点检测、深层神经网络等内容
## 3.用CMake编译OpenCV3.4.1和OpenCV-Contrib3.4.1
1）.首先打开cmake，where is the source code 选择opencv文件夹中source的目录，e.g.
D:/opencv3.4.1/opencv/sources，需要根据实际情况更改目录。
2）.where to build the binaries 选择需要存放生成文件的目录，e.g.
D:/opencv3.4.1/opencv/new_build
这里要注意的是选择的目录最好还在opencv文件夹中，但不能是opencv文件夹中原有的目录，最好是纯英文。
3）.点击configure，第一次使用会弹出对话框，选择对应的vs版本即可，其他的不用管。e.g. vs2015
64位，选择的是vs2015 Win64，之后点确认，程序开始运行，运行完成后会在最下方显示configuration done.
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181216201513992.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
4）.在cmake搜索框中搜索OPENCV_EXTRA_MODULES_PATH，将其值选择为opencv_contrib文件夹中的modules文件夹，e.g.D:\opencv3.4.1\opencv_contrib-3.4.1\modules。同时要保证WITH_CUDA未被勾选。最后再一次点击configure，成功之后依然会在最下边显示configuration done.
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181216201804312.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181216201824127.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
5）.点击generate。然后直接在cmake面板点击open project打开项目。至此Cmake中的操作全部完成，可以关掉它了。
[
](https://img-blog.csdnimg.cn/20181216201804312.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)注：new_build文件夹里面多了好多文件：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181216202016913.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
3.用VS2015生成解决方案
1）打开项目文件
找到new_build文件夹里的OpenCV.sln文件，并双击，打开后如图：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181216202106801.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
2）点击生成解决方案：
[
](https://img-blog.csdnimg.cn/20181216202106801.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)（1）选择Debug x64 调试环境：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181216202440554.png)
（2）直接按快捷键
Ctrl + Shift + B
或者：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181216202528694.png)
等待生成，完成之后会出现如下情况：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181216202600466.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
3）生成安装文件
（1）找到CMakeTargets中的INATALL
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181216202709132.png)
（2）对INSTALL单击右键–>仅用于项目–>仅生成 INSTALL
完成后new_build文件夹里多了一个install文件夹：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181216202819861.png)
至此呢，所有的编译以及生成工作就完成了。
[
](https://img-blog.csdnimg.cn/20181216202819861.png)4）可能遇到的问题：
在用 VS2015 编译 Debug 版的 openCV 源码时，最后一步生成 INSTALL，碰到了下面问题：
LNK110：无法打开文件“python36_d.lib”
这个是 Anaconda 中 pyconfig.h 的配置问题
解决方法：
在对应的 pyconfig.h 文件中做出如下修改：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181216203048277.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018121620311066.png)
4.VS2015配置opencv环境
1）配置环境变量
在Path中加入：D:\opencv3.4.1\install\x64\vc14\bin
2）包含目录：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181216203407465.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
3）库目录：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181216203438729.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
4）附加依赖项：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181216203827164.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTM4NDExOTY=,size_16,color_FFFFFF,t_70)
import os
\#可使用脚本，把lib库中的’.lib’打印出来，复制即可。
if __name__ == '__main__':
    path = r"D:\opencv3.4.1\install\x64\vc14\lib"
    content = os.listdir(path)
    for temp in content:
        if '.lib' in temp:
            print(temp)5）复制有关动态库至系统目录下
将D:\opencv3.4.1\install\x64\vc14\bin下的dll文件复制至 C:\Windows\System32 和 C:\Windows\SysWOW64 目录下。
参考：
[https://blog.csdn.net/weixin_42012977/article/details/82992962](https://blog.csdn.net/weixin_42012977/article/details/82992962)
[https://blog.csdn.net/Chris_zhangrx/article/details/78947526](https://blog.csdn.net/Chris_zhangrx/article/details/78947526)
[https://blog.csdn.net/liu798675179/article/details/51259505](https://blog.csdn.net/liu798675179/article/details/51259505)
[https://github.com/amusi/opencv-facial-landmark-detection](https://github.com/amusi/opencv-facial-landmark-detection)
---
注：博众家之所长，集群英之荟萃。

