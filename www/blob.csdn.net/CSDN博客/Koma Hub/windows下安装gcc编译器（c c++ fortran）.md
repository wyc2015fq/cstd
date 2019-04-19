# windows下安装gcc编译器（c/c++/fortran） - Koma Hub - CSDN博客
2018年01月08日 14:59:35[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：15912
个人分类：[Windows																[C/C++](https://blog.csdn.net/Rong_Toa/article/category/7156199)](https://blog.csdn.net/Rong_Toa/article/category/7388572)
首先下载[MinGW - Minimalist GNU for Windows](https://sourceforge.net/projects/mingw/files/MinGW/)
下载结束后双击，点击下一步即可，
然后勾选要安装的组件：
![](https://img-blog.csdn.net/20180108144448735?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
点击应用：
![](https://img-blog.csdn.net/20180108144514707?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
开始安装（这一步要经过漫长的下载过程，请耐心等待）：
![](https://img-blog.csdn.net/20180108144539882?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
修改环境变量：
右击此电脑》属性》高级系统设置》环境变量
![](https://img-blog.csdn.net/20180108145342673?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
将D:\MinGW\bin添加到PATH里；
将D:\MinGW\include添加到INCLUDE里；
将D:\MinGW\lib添加到lib里；
点击确定即可；
按win+x》运行》输入cmd，然后输入gcc -v
![](https://img-blog.csdn.net/20180108145833889?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvUm9uZ19Ub2E=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
安装完成！
