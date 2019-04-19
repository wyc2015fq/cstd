# vs2015编译EasyDarwin开源流媒体服务器Linux版本调研 - xcyl的口袋 - CSDN博客
2016年07月06日 23:28:57[xcyl](https://me.csdn.net/cai6811376)阅读数：1240
之前InfoQ的一篇文章提到用vs2015可以将C++项目直接编译成Linux的可执行文件。[Writing C++ for Linux on Visual Studio](https://www.infoq.com/news/2016/06/linux-cpp-vs2015)。
于是我使用vs2015安装最新Visual C++ for Linux Development插件。[下载地址](https://visualstudiogallery.msdn.microsoft.com/725025cf-7067-45c2-8d01-1e0fd359ae6e)。
安装完插件vs的新建中多了“Linux”的新建项
![new](https://img-blog.csdn.net/20160706231119426)
选择Empty Project (Linux)
创建完成后，添加main.cpp，写Hello World测试：）
右键项目生成弹出配置对话框，通过ssh连接目标Linux（注：Linux需要安装g++）
![li](https://img-blog.csdn.net/20160706231756828)
点击Connect开始生成
![ii](https://img-blog.csdn.net/20160706232007508)
进入目标Linux的连接用户目录，目录下有个projects，这个目录下就有我们的项目目录。
![lin](https://img-blog.csdn.net/20160706232510228)
进入项目目录下的bin
![bin](https://img-blog.csdn.net/20160706232652795)
运行，成功！！
![out](https://img-blog.csdn.net/20160706232752390)
## 获取更多信息
邮件：[support@easydarwin.org](mailto:support@easydarwin.org)
WEB：[www.EasyDarwin.org](http://www.easydarwin.org)
QQ交流群：288214068
Copyright © EasyDarwin.org 2012-2016
![EasyDarwin](http://www.easydarwin.org/skin/easydarwin/images/wx_qrcode.jpg)
