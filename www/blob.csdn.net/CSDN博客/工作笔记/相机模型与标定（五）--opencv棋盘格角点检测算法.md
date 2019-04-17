# 相机模型与标定（五）--opencv棋盘格角点检测算法 - 工作笔记 - CSDN博客





2016年07月25日 10:42:55[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：10762
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









原文：

http://blog.csdn.net/b5w2p0/article/details/18446961

很简单，作者写的差不多对，不高兴改了。。。

刚接触图像处理是从摄像机标定开始，一直好奇opencv程序中是怎么实现棋盘定位的。自己也曾用matlab写过摄像机标定的整个过程，在图像中检测出棋盘的位置是整个标定过程的第一步，但一直不稳定，不知道opencv中采用什么算法检测棋盘的位置。很可惜，网上基本上没有这方面的资料，在opencv原文件中的提示：The algorithms developed and implemented by Vezhnevets Vldimir    aka Dead Moroz (vvp@graphics.cs.msu.ru)
 See http://graphics.cs.msu.su/en/research/calibration/opencv.html for detailed information。但这个网站似乎打不开。最后终于在yahoo的邮件列表找到这个算法简单的介绍：


1. Dilation of the white squares by 1 pixel, so that the black squares don't touch (it might be the oter way, i'm not shure).

2. Thresholding with a value beneath the medium intensity 


3. Contour extraction and approximation to a rectangle. This extract the black (i suppose) squares.

4. selection of the inner corners by rejecting the points which don't belong to two extracted rectangles.


I noticed that this is not working very well for images taken using a camera with a flash, due to non uniform illumination.But i managed to extract the correct corners by running the contour extraction several times, using several different thresholds, even
 adaptive thresholding, different combinations of Dilate/Erode and rejecting duplicate rectangles prior to step 4.

当然，牛人觉得opencv中的算法还需要改进，我辈能够汲取opencv中营养，已十分欣喜。


粗略的懂些opencv源码编译的过程，就想看看opencv库中cvFindChessboardCorner()的源码。


工具：opencv 2.1、cmake、vs08


修改opencv安装文件中cvcalibinit.cpp文件，方便调试。把文件中


#pragmacomment(lib,"highgui210d.lib")


#defineDEBUG_CHESSBOARD


这两句的注释去掉。注意如果你用的不是highgui210.lib库，那就需要修改了。




#include". \\include\\opencv\\highgui.h"  


这句的路径改为你电脑中highgui.h所在的目录，主要原文件中这句是相对路径，如果不修该可能导致vs2008编译不通过。




现在来看其中的源码，源码真多，大致说明其过程-，-。


总的来说，棋盘的定位用还是图像处理中一些基本概念如直方图均衡化，图像的轮廓和棋盘图像的特点，这不像车牌定位，棋盘上的图像比车牌更有规律，只有黑白方格，棋盘所在背景也比较简单，等其他优点。


cvFindChessboardCorner()，中首先构造两个结构体：


struct CvCBQuad


{


    int count;      // Number of quad neighbors


    int group_idx;  // quad group ID


    int row,col;   // row and column of this quad


    bool ordered;   // true if corners/neighbors are ordered counter-clockwise


    float edge_len;// quad edge len, in pix^2


    // neighbors and corners are synced, i.e., neighbor 0 shares corner 0


    CvCBCorner *corners[4];// Coordinates of quad corners


    struct CvCBQuad *neighbors[4];// Pointers of quad neighbors


};


struct CvCBCorner


CvCBQuad用来存储棋盘上的方格信息，CvCBCorner用来存储棋盘方格的四个角点。CvCBQuad的neighbordors在程序中有重要的作用。




整个棋盘定位过程是一个循环过程，先对读入的棋盘图像直方图均衡化，接着自适应(取决于flag参数)二值化，再对二值化后的图像膨胀。为了定位的鲁棒性，自适应二值化和膨胀所采用核的大小不能是唯一的，故不断的循环用不同的参数用对棋盘图像处理，膨胀所采用核的大小逐渐变大。


在每次的循环过程都需要，经过以下步骤。


1、在二值化后图像外围画一白色的矩形框(方便轮廓提取)，然后进行轮廓提取cvFindContours。经过膨胀后的二值图像，每个黑色的方格已经被分开，轮廓提取后可以得到每个方格的轮廓，当然还有很多干扰轮廓。对轮廓进行多边形拟合cvApproxPoly，排除不是矩形的轮廓，利用矩形的其他性质，再排除一些干扰轮廓。这些工作主要由icvGenerateQuads函数完成。


2、寻找每个方格的相邻方格，并记相邻方格的个数，连同相邻方格的信息存在相应CvCBQuad结构体中。二值图像在膨胀后原本相邻的方格，分开了，原来相连部分有一个公共点，现在分开变成了两个点。找到相邻的方格之后，计算出原来的公共点，用公共点替代膨胀后分开的点。这主要由icvFindQuadNeighborhors函数完成。


3、对所有“方格”(包括被误判的)分类，分类的原则是类内所有方格是相邻的。由icvFindConnectedQuads函数完成。


4、根据已知所求的角点个数，判别每个类中方格是否为所求的棋盘方格，并对棋盘方格排序，即该方格位于哪行那列。在这个过程中，可以添加每类方格总缺少的方格，也可以删除每类方格中多余的方格。icvOrderFoundConnetedQuads函数完成该过程。


5、icvCleanFoundConnectedQuads函数、icvCheckQuadGroup函数根据已知棋盘的方格个数(由棋盘的角点数计算出来)确认方格位置及个数是否正确，并确定粗略强角点的位置(两个方格的相连位置)。icvCheckBoardMonotony再次检验棋盘方格是否提取正确。


6、以上如果有一步所有方格都不符合要求，则进入一个新的循环。若循环结束，还尚未找到符合要求的方格，则棋盘定位失败，退出函数。


最后，cvFindCornerSubpix()根据上步的强角点位置，确定强角点的精确位置。



