# HEVC与VP9编码效率对比 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月15日 20:05:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：49
个人分类：[视频质量评价																[视频编码](https://blog.csdn.net/leixiaohua1020/article/category/1638135)](https://blog.csdn.net/leixiaohua1020/article/category/1446693)









HEVC（High EfficiencyVideo Coding，高效率视频编码）是一种视频压缩标准，H.264/MPEG-4 AVC的继任者。目前正在由ISO/IEC MPEG和ITU-T VCEG开发中。为此目的MPEG与VCEG联合成立了一个JCT-VC（JointCollaborative Team on Video Coding）作为共同开发HEVC的团队。HEVC被认为不仅提升图像质量，同时也能达到H.264/MPEG-4 AVC两倍之压缩率（等同于同样画面质量下比特率减少了50%），可支持4K分辨率甚至到超高画质电视，最高分辨率可达到8192×4320（8K分辨率）。第一版的HEVC/H.265视频压缩标准在2013年4月13日被接受为国际电信联盟（ITU-T）的正式标准。



VP9是一个由Google开发的开放的视频压缩标准。VP9将是VP8的后继者。VP9的开发始于2011。VP9的目标之一是相同质量下相对于VP8可以减少50%的比特率。而另一个目标就是争取能在压缩效率上高于HEVC。2013年2月21日，第一个支持VP9解码技术的Google Chrome网页浏览器发布了。



HEVC和VP9都是下一代视频编码技术，到底孰优孰劣呢？DOOM9论坛上xooyoozoo对此进行了研究。





在这个测试中，HEVC和VP9的GOP长度是匹配的。



vp9编码器参数是WebM Guide中的“best quality”，但是改变了kf-max-dist以匹配GOP并且增加了tune=psnr。

x264设置为veryslow, psnr|ssim,matched keyint, open-gop, and crf.



以下是他的测试结果：

![](https://img-blog.csdn.net/20130915195818218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

总结的实验结果如图所示

![](https://img-blog.csdn.net/20130915194624968?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







从测试结果来看：VP9的编码效率在H.264之上，但是在HEVC之下

原帖地址：[http://forum.doom9.org/showthread.php?t=165839&page=3](http://forum.doom9.org/showthread.php?t=165839&page=3)





========补充==============

HEVC测试序列合集：[http://download.csdn.net/detail/leixiaohua1020/6843803](http://download.csdn.net/detail/leixiaohua1020/6843803)

VP9测试序列合集：[http://download.csdn.net/detail/leixiaohua1020/6843921](http://download.csdn.net/detail/leixiaohua1020/6843921)







