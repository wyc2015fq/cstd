
# ubuntu安装openpose - 郭云飞的专栏 - CSDN博客


2019年01月16日 12:21:54[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：188


**一、使用Git直接下载openpose**
`git clone https://github.com/CMU-Perceptual-Computing-Lab/openpose.git`
`二、下载各种模型文件（比较大，可能需要几分钟到几个小时不等，看网速了）`
进入openpose所在目录
cd models
sh ./getModels.sh（这个脚本就是用来下载各种已训练的模型的）
**三、编译Caffe（GPU版）**
注意，在下载openpose时自带了caffe，位置在：openpose/3rdparty/caffe，
必须编译自带的这个caffe，用其他的版本可能会导致各种不兼容问题。
编译Caffe的具体过程，请参考其它文档，总之此时编译成功Caffe，在之后编译openpose时
会省心的很。
**四、利用cmake-gui生成build文件**
如果没有安装cmake-gui工具，请先安装。安装方法很简单，百度一下。
在openpose目录下创建build文件夹：sudo mkdir build
在终端执行cmake-gui，在弹出界面中如下操作：
![](https://img-blog.csdnimg.cn/20190116115635569.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2d1b3l1bmZlaTIw,size_16,color_FFFFFF,t_70)

Configure完毕后，配置Caffe路径并Generate：
![](https://img-blog.csdnimg.cn/2019011612050173.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2d1b3l1bmZlaTIw,size_16,color_FFFFFF,t_70)
Generate之一步一般不会出错，完事后会在信息栏里输出“Generate done”字样。
**五、开始编译openpose**
cd build
sudo make -j8
**六、编译openpose时可能遇到的问题**
提示找不到caffe/proto/caffe.pb.h，导致编译失败。解决：
在openpose/3rdparty/caffe/目录下，终端操作：
caffe$ protoc src/caffe/proto/caffe.proto --cpp_out=.
caffe$ mkdir include/caffe/proto
caffe$ mv src/caffe/proto/caffe.pb.h include/caffe/proto/
**七、测试**
链接usb摄像头测试：
./build/examples/openpose/openpose.bin --face --hand


