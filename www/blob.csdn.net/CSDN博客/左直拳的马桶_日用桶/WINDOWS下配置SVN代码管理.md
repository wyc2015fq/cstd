# WINDOWS下配置SVN代码管理 - 左直拳的马桶_日用桶 - CSDN博客
2014年01月14日 22:43:33[左直拳](https://me.csdn.net/leftfist)阅读数：2172
个人分类：[应用](https://blog.csdn.net/leftfist/article/category/2778939)
服务器端使用 visualsvn server，客户端使用tortoiseSvn。
一、服务器端
1、首先，下载visualsvn server，安装到服务器。下载地址：
[http://www.visualsvn.com/server/download/](http://www.visualsvn.com/server/download/)
2、安装好后，打开
![](https://img-blog.csdn.net/20140113222034734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
其中，Repositories（库）就是代码库
3、新建代码库
![](https://img-blog.csdn.net/20140113222735500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20140113222801125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
注意：上图中的CheckBox如果选中，则在代码库StartKit下面会创建trunk、branches、tags三个子目录；不选中，则只创建空的代码库StartKit。不选中，点击 OK创建。
4、创建用户
在VisualSVN Server上创建用户，比如叫coder，然后右击代码库，属性，添加新建的用户，将read/write权限赋给coder。
5、设置SVN SERVER的访问形式及端口
右击 VisualSVN Server（如图），设置访问端口。默认是80，https。
![](https://img-blog.csdn.net/20140114223058687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
二、客户端
1、安装TortoiseSVN
2、签入源代码到SVN服务器
右击源代码目录，选Import，如图。通常源代码目录有许多并不需要提交的东西，如BIN目录，临时文件夹，等等，所以，import前，应该清洁源代码目录，保证里面的内容都是必须的。
![](https://img-blog.csdn.net/20140114223813734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20140114223900250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![](https://img-blog.csdn.net/20140114223358812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
3、签出源代码到本机
源代码签入服务器后还不够，还没有达到代码管理的目的。应该新建一个文件夹，checkout，将签入的代码签出到这个新文件夹，这个新文件夹才真正是可管理的源代码所在。
在本机创建文件夹StartKit，右键点击Checkout，弹出如下图的窗体：
![](https://img-blog.csdn.net/20140114224251562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
参考文章：
[http://www.jb51.net/article/17365.htm](http://www.jb51.net/article/17365.htm)
