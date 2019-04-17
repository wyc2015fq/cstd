# opencv打开监控录制的视频出现错误，解决办法！ - 勿在浮砂筑高台 - CSDN博客





2015年08月05日 13:58:07[勿在浮砂筑高台](https://me.csdn.net/luoshixian099)阅读数：2714标签：[打开视频错误																[opencv](https://so.csdn.net/so/search/s.do?q=opencv&t=blog)](https://so.csdn.net/so/search/s.do?q=打开视频错误&t=blog)
个人分类：[Image Processing](https://blog.csdn.net/luoshixian099/article/category/2734445)








**问题：**

之前没有遇到过此类问题，今天用opencv打开监控录制视频（.mp4）进行处理时，出现错误！ 
![这里写图片描述](https://img-blog.csdn.net/20150805124703121)

找到cap_ffmpeg_impl.hpp源文件,也没有看懂其原理，网上很多人说是路径出现错误，有人说是视频放在源程序下等，这些都不能解决问题。 

看似.mp4文件，应该有不同的编码方式（没研究过） 

其次只有迅雷看看能打开这类视频（期间还要自动下载解码器） 

于是想到转码，下载格式工厂也转不了！。。。。 
**解决办法：**

偶然发现迅雷看看有个功能，可以“截取与转码”！ 

对着打开的视频右键，选择“截取与转码”，再次保存为.mp4文件就可以了！




