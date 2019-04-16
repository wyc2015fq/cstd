# 开源视频质量评价工具： Evalvid - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月16日 12:35:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：45
个人分类：[视频质量评价](https://blog.csdn.net/leixiaohua1020/article/category/1446693)













Evalvid是一个对在真实或模拟的网络里传输的视频进行质量评价的框架和工具集。除了底层网络的QoS参数的测量，如丢包率，延迟，抖动，Evalvid还提供标准的视频质量评价算法如PSNR和SSIM。它视频编码方面支持H.264，MPEG-4和H.263。音频编码方面支持AAC。Evalvid是开源的，由jirka klaue编写。

出品Evalvid的机构：TKN - the Telecommunication Networks Group，是柏林工业大学的电气工程和计算机科学学院。

![](https://img-blog.csdn.net/20140731194332777)





## 

Evalvid中的PSNR工具用于计算原始序列与受损序列之间的PSNR或者SSIM。

PS：虽然名字叫“PSNR”，实际上是可以计算SSIM的，在命令行后面添加参数“ssim”即可。

命令格式：


```
psnr x y <YUV format> <src.yuv> <dst.yuv> [multiplex] [ssim]
x             视频宽度
y             视频宽度
YUV format    420, 422
src.yuv      原始序列
dst.yuv      受损序列
[multiplex]   可选
[ssim]       可选:计算SSIM而不是PSNR
```


使用示例（YUV格式：宽高为480x272，采样格式YUV420P，受损视频为src01_480x272_0.100.yuv，原始视频src01_480x272.yuv）：

计算PSNR：

`psnr 480 272 420 src01_480x272_0.100.yuv src01_480x272.yuv > ref_psnr.txt`

计算SSIM：

`psnr 480 272 420 src01_480x272_0.100.yuv src01_480x272.yuv ssim > ref_ssim.txt`

计算PSNR截图：

![](https://img-blog.csdn.net/20140731193054117?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

计算SSIM截图：

![](https://img-blog.csdn.net/20140731193104756?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

计算结果可以导入Excel进行处理。

PSNR结果：

![](https://img-blog.csdn.net/20140731193741605?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

SSIM结果：

![](https://img-blog.csdn.net/20140731193753367?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

网址：[http://www2.tkn.tu-berlin.de/research/evalvid/](http://www2.tkn.tu-berlin.de/research/evalvid/)

下载：[http://download.csdn.net/detail/leixiaohua1020/6374037](http://download.csdn.net/detail/leixiaohua1020/6374037)





