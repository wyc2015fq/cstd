# MATLAB 读取AVI视频 Unable to locate decompressor to decompress video stream - 家家的专栏 - CSDN博客





2011年02月22日 11:05:00[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：15140








MATLAB 读取AVI视频出现错误： Unable to locate decompressor to decompress video stream。

错误如下：


|mov=aviread('1.avi');??? Unable to locate decompressor to decompress video streamError in ==> aviread at 63    X = readavi(info.Filename,-1);|
|----|

**解决方案有2个：**

1，下载相应的解码器，对AVI文件进行解码，再读取；文件格式和对应的解码器可通过一下语句：

aviinfo('1.avi')



ans = 

              Filename: 'cctv052x2004080516x01638.avi'
              FileSize: 125272
           FileModDate: '20-Jun-2005 08:38:30'
             NumFrames: 53
       FramesPerSecond: 10
                 Width: 320
                Height: 240
             ImageType: 'truecolor'
      VideoCompression: 'MPG4'
               Quality: 4.2950e+007
    NumColormapEntries: 0



可以看出，本avi文件可以通过下载安装MPG4 Video compression 解码器,matlab中可以正常运行，并没有错误报告，但是读出来的视频文件大部分像素是， 出现大幅黑色的。。。。

不知道为什么，可以解码出视频文件的大小，帧数目等信息，不知道为什么是黑色的。。。郁闷。。无奈之下，暂时选择第二种解决方法。。



下载各种视频解码器的地址：[http://www.moviecodec.com/videocodecs.html](http://www.moviecodec.com/videocodecs.html)



[](http://www.zdh1909.com/html/matlab/9004.html)

2、另一种方法是采用一些视频转换软件，将实验用的AVI文件转成MATLAB支持的格式。

例如采用winAVI 9.0这个软件，选择编码格式ZJMedia Uncompress RGB24，转换成新文件后再实验则可通过。



















