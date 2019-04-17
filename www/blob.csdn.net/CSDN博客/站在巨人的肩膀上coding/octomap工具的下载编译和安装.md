# octomap工具的下载编译和安装 - 站在巨人的肩膀上coding - CSDN博客





2018年04月09日 15:59:55[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：443








        做SLAM的童鞋都知道，ORBSLAM2得到的三维点云不能直接拿来做导航，那我们怎么去实现导航呢？

这就要提到我们今天要介绍的嘉宾了：octomap。

    科普帖：知乎高博的回答https://www.zhihu.com/question/39029941

                    高博的实验博客：https://www.cnblogs.com/gaoxiang12/p/5041142.html

    那么，既然有了这么全的资料，为什么还要花时间整理出一份博客说明呢？因为。。。。。因为我TM遇到了问题，编译不通过，网上基本都是没有遇到什么问题！！！我的错误在这里：

![](https://img-blog.csdn.net/20180409161028340)


**解决方法：**

**本质问题在于链接octovis的库的时候出现问题，也就是说需要先单独编译这个库**

**先装驱动：sudo apt-get install libqt4-dev qt4-qmake libqglviewer-dev**

注意看的话octomap下面有两个文件夹octomap和octovis。我们需要单独的去编译安装这两个工具，我们先来安装octomap:

cd octomap/octomap

mkdir build

cd build

cmake ..






生成好配置文件之后，我们就开始make了

make




make完还没完，还需要安装

sudo make install(默认安装在根目录下，需要给管理员权限)

![](https://img-blog.csdn.net/20180409161352435)




这样，我们的octomap模块就装好了。接下来是octovis模块，方法同上

cd  octomap/octovis

mkdir build 


cd build

cmake ..



没有错误产生，接下来make && sudo  make instal 


![](https://img-blog.csdn.net/20180409161159302)


然后我们再去dynamicEDT3D文件夹执行上述相似操作，编译安装就可以了。（如果担心未完全安装，可以去octomap根目录下建立build文件夹，然后编译安装）


至此，我们就大功告成啦！！可以愉快的使用octomap查看三维点云图了。博主也等不及赶紧试一波：

运行命令：


![](https://img-blog.csdn.net/20180409161138329)


得到的效果：


![](https://img-blog.csdn.net/20180409161310384)



那么问题又来了，怎么全是蓝色的呢？为什么不是彩色的呢？

在设置里面打开未占有的显示，就可以看到未占有的绿色表示：

![](https://img-blog.csdn.net/20180409161324796)







