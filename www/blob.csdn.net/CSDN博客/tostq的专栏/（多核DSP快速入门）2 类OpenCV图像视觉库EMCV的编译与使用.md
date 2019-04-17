# （多核DSP快速入门）2.类OpenCV图像视觉库EMCV的编译与使用 - tostq的专栏 - CSDN博客





2016年04月25日 23:34:57[tostq](https://me.csdn.net/tostq)阅读数：7458
所属专栏：[多核DSP快速入门](https://blog.csdn.net/column/details/easymulticoredsp.html)











原创文章
转载请注册来源[http://blog.csdn.net/tostq](http://blog.csdn.net/tostq)


       EMCV即Embedded Computer Vision Library，是一个可在TI DM64x系列DSP上运行的计算机视觉库。其提供了跟OpenCV完全一致的函数接口（包括IplImage, CvMat，CvSeq等基本数据结构），通过EMCV，可以轻松的将OpenCV算法移植到DSP。

EMCV并不是一个官方库，其是由Shiqi Yu老师在2008年启动的开源项目，目前已经支持IplImage,
 CvMat，CvSeq等基本OpenCV数据结构，也可使用cvCreateImage等创建和释放图像，以及contour检测等。但是里面绝大多数函数只有接口却没有实现，所以这里值得注意的，EMCV并不能实现OpenCV的多数功能，不过我们可以利用EMCV里的数据结构。

此外，需要提一下的是，实现将OpenCV里的代码完全移植到DSP是可以的，目前已经有人这么做了！

本节主要参考以下几个文档


Shiqi Yu老师的EMCV：可在DSP上运行的OpenCV：[http://www.opencv.org.cn/forum.php?mod=viewthread&tid=3569&extra=page%3D1](http://www.opencv.org.cn/forum.php?mod=viewthread&tid=3569&extra=page%3D1)


在C6000系列DSP中使用EMCV ：[http://www.opencv.org.cn/forum.php?mod=viewthread&tid=4638](http://www.opencv.org.cn/forum.php?mod=viewthread&tid=4638)


学用emcv：[http://www.opencv.org.cn/forum.php?mod=viewthread&tid=7937](http://www.opencv.org.cn/forum.php?mod=viewthread&tid=7937)





**一、EMCV的下载**

由于EMCV是一个不断更新的开源项目，所以我们推荐利用subversion客户端获取源代码，这样的话，能及时得到最新的源代码。

我这里使用的是TortoiseSVN，安装了TortoiseSVN后，我们新建一个存在EMCV的文件夹，如D:/EMCV

进入文件夹，右击选择SVN Checkout... ，可以得到如下对话框：

![](https://img-blog.csdn.net/20160425233107252?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

我们在URL输入EMCV的代码网络地址，在Checkout directory中输入下载目录，然后点击OK，就可以自动完成安装了，下载完后的目录。以后如果有更新的话，TortoiseSVN会自动下载，完成代码更新。

![](https://img-blog.csdn.net/20160425233134889?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





**二、EMCV的编译**

（1）新建一个静态库项目

![](https://img-blog.csdn.net/20160425233200749?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（2）导入EMCV的源文件，这里的导入很简单，我们只需直接将源文件复制到项目文件夹中就可以了，CCS会自动将文件导入到项目中：

![](https://img-blog.csdn.net/20160425233217472?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


（3）编译EMCV，这里不需要链接命令文件.cmd，只需要直接点击小锤子进行Build就可以了，此时项目目录如下：

![](https://img-blog.csdn.net/20160425233257030?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


这里有些文件上有感叹号的标识，说明存在警告，emcv.lib是我们最终得到的静态链接库文件，使用的时候，只需要将其复制到项目文件夹就可以了。下一节，我们将介绍一个使用emcv.lib的例子。




例程下载：[https://github.com/tostq/EasyMulticoreDSP/tree/master/2.emcv](https://github.com/tostq/EasyMulticoreDSP/tree/master/2.emcv)





