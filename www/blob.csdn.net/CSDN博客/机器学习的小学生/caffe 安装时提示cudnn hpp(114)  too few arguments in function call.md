
# caffe 安装时提示cudnn.hpp(114): too few arguments in function call - 机器学习的小学生 - CSDN博客


2017年06月22日 11:25:46[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：4476


**问：**
Try to build Caffe with GPU and MATLAB support.
However, when I use build_win.cmd to install, It can’t build and with the error :
“D:\caffe-windows\caffe-windows\scripts\build\ALL_BUILD.vcxproj” (default target) (1) ->
“D:\caffe-windows\caffe-windows\scripts\build\src\caffe\caffe.vcxproj” (default target) (3) ->
(CustomBuild target) ->
D:/caffe-windows/caffe-windows/include\caffe/util/cudnn.hpp(114): error : too few arguments in function call [D:\caff
e-windows\caffe-windows\scripts\build\src\caffe\caffe.vcxproj]
`205 Warning(s)
1 Error(s)`Please Help.
PS:
I use MATLAB R2016a, VS2015 and CUDA 8.0 with CuDNN 6.0.
**答：**
This has been fixed just recently (March 29 2017), pull the latest source commits.
The problem is due to CuDNN 6.0 adding the extra “type” parameter.
**解决方法：**
将cudnn-8.0-windows10-x64-v6.0.zip 换成cudnn-8.0-windows10-x64-v5.1.zip 。即将CuDNN6.0换成CuDNN5.1.
参考文献：
[https://groups.google.com/forum/\#!topic/caffe-users/o9683jAqrnY](https://groups.google.com/forum/#!topic/caffe-users/o9683jAqrnY)

