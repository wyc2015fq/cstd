# 【opencv3】视频压缩 ffmpeg - zj360202的专栏 - CSDN博客





2018年01月11日 14:50:32[zj360202](https://me.csdn.net/zj360202)阅读数：1864








视频压缩方式主要如下：

1. 压缩视频组成图片的尺寸

2. 还压缩算法和对应格式

3. 通过视频总比特率设置来改变

上面两种一般是可以通过opencv就可以实现的，

第一种：压缩比不高，1920x1080--->640x360也就压缩了三倍，但是牺牲太大

第二种：avi和mp4的差距非常大，avi的在80M左右，同样用mp4能到50M

第三种：通过ffmpeg设定，总比特率，压缩比非常OK，这个一般等同于ps保存的时候，提示保存图片质量是否好

ffmpeg.exe -i test.mp3 -b 500 test.flv 


-i 是输出

-b 是比特率

ffmpeg.exe下载地址
[https://ffmpeg.org/](https://ffmpeg.org/)

如果需要的话，可以通过python 调用ffmpeg完成代码设定






