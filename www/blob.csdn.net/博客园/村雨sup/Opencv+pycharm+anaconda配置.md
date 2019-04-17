# Opencv+pycharm+anaconda配置 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





最简便的方法，没有之一

首先下载annconda：（不要从官网下，还要FQ，还要慢）

https://mirrors.tuna.tsinghua.edu.cn/anaconda/archive/

请注意下载annconda2，不要下载3，opencv支持py2.7，如果你硬要找py3.6的，就会很麻烦。

然后从pycharm中选择annconda2自带的py2的编译器



![](https://images2018.cnblogs.com/blog/1196023/201803/1196023-20180308224552745-95441678.png)

然后同样的方法下载OpenCV

点击下载的opencv-2.4.12.exe，一路next下去，例如本人安装到E盘根目录下。安装完成后，将E:\opencv2_4_12\build\python\2.7\x64下的cv2.pyd拷贝到    C:\Anaconda2\Lib\site-packages       下面。

等着pycharm加载完成就好了

试试import cv2 吧













