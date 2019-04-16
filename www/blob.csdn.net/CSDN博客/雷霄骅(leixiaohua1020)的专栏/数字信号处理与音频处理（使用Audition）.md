# 数字信号处理与音频处理（使用Audition） - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2014年05月13日 00:43:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：54
个人分类：[音频编码](https://blog.csdn.net/leixiaohua1020/article/category/1638137)










前一阵子因为考博学习需要，看了《数字信号处理》，之前一直不清除这门课的理论在哪里应用比较广泛。这次正巧用Audition处理了一段音频，猛然发现《数字信号处理》这门课还是非常有用的，在音频处理领域至少是这样。在此记录一下几步简单操作。



一．频谱

打开Audition，拖入一段音频。默认打开的是音频时域的波形图。波形图的横坐标是时间，纵坐标是采样值的大小。

![](https://img-blog.csdn.net/20140513003420250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

上面那个图其实就是音频文件中所有采样点连起来的一条线，不过由于采样点太密集了，所以根本看不出来采样点了。把波形图不停地放大，放大，再放大，就能看到一个个音频采样点了。

![](https://img-blog.csdn.net/20140513003430781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

把视图切换成频谱，可以查看频域波形。乍一看还感觉挺炫的。频域图中横坐标是时间，纵坐标是频率，而颜色越亮的地方代表位于该频率的分量值比较大。由于我打开的音频采样率是44.1kHz的，根据奈奎斯特抽样定律，频率的最大值不能超过44.1k/2=22.05kHz，所以纵坐标的最大值是22.05Khz。

PS：由于《爱的罗曼斯》这个曲子是吉他一下一下弹出来的。从频域图上我们可以看出，每隔一段时间就会有一个“竖条”，该位置显示出高频分量相对来说比较多，对应的就是吉他曲里弹吉他的那个音。

![](https://img-blog.csdn.net/20140513003438546?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

此外，还可以对这个频谱图进行配置。打开“编辑->首选参数->显示”，可以调整窗口函数和分辨率。

![](https://img-blog.csdn.net/20140513003455718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

比如说，如果把分辨率从512调整到16的话，频谱图就变成下面的模样了。可以看出，精度大为下降（貌似速度比较快）。

![](https://img-blog.csdn.net/20140513003504203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

勾选“视图->快捷栏->编组->解析”，可以添加一组解析频谱的按钮。单击“频率解析”按钮，可以查看某一时间点的频谱，如下图所示。

![](https://img-blog.csdn.net/20140513003512187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在这里，也可以调整FFT的大小。默认是1024。比如说改成32，效果如下。可以看出，精度大幅下降。

![](https://img-blog.csdn.net/20140513003521484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



二．FFT滤波器

在“效果->滤波与均衡->FFT滤波器”中，可以给音频添加FFT滤波器。

滤波器设置界面如下图所示。可以自己画滤波器，也可以选择预设的滤波器。比如说，选择“Telephone Receiver”的话，就是电话听筒效果的滤波器。此外也可以配置FFT大小，开窗函数等参数。

![](https://img-blog.csdn.net/20140513003533609?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



三．其他设置

在“编辑->调整采样率”中，可以调整音频的采样率（在这里是回放比率，并不修改文件）。可以通过该功能体会一下采样率对声音的影响。



![](https://img-blog.csdn.net/20140513003545640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在“编辑->转换采样类型”中，可以更改采样率，采样位数等信息（这里是会修改文件的）。可以通过该功能体会一下采样位数对声音的影响。

![](https://img-blog.csdn.net/20140513003551203?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)






