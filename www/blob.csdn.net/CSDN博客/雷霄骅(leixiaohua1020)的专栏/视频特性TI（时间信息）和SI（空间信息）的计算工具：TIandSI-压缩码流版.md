# 视频特性TI（时间信息）和SI（空间信息）的计算工具：TIandSI-压缩码流版 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2015年05月30日 18:55:24[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：4783










=====================================================

TI（时间信息）和SI（空间信息）计算工具文章列表：

[视频特性TI（时间信息）和SI（空间信息）的计算工具：TIandSI](http://blog.csdn.net/leixiaohua1020/article/details/9056443)

[视频特性TI（时间信息）和SI（空间信息）的计算工具：TIandSI-压缩码流版](http://blog.csdn.net/leixiaohua1020/article/details/46276915)

=====================================================


此前在文章《[视频特性TI（时间信息）和SI（空间信息）的计算工具：TIandSI](http://blog.csdn.net/leixiaohua1020/article/details/9056443)》中记录过两个基于YUV视频数据的TI和SI计算工具：一个图形界面工具和一个命令行工具。由于近期正在研究视频内容复杂度方面的东西，所以顺手将原来的TIandSI工具进行了升级，添加了另外两个支持压缩码流的TI和SI的工具，并将版本改为3.0。

![](https://img-blog.csdn.net/20150530182025499)

目前的TIandSI项目中一共包含4个项目：

> TIandSI : 计算YUV数据的TI和SI-图形界面版。
TIandSIcmd : 计算YUV数据的TI和SI-命令行版。
TIandSIpro : 计算压缩码流数据的TI和SI-图形界面版。
TIandSIprocmd : 计算压缩码流数据的TI和SI-命令行版。

其中后两个“pro”版用于计算压缩码流的TI和SI。

**项目主页**

> SourceForge：[https://sourceforge.net/projects/tiandsi/](https://sourceforge.net/projects/tiandsi/)Github：[https://github.com/leixiaohua1020/TIandSI](https://github.com/leixiaohua1020/TIandSI)开源中国：[http://git.oschina.net/leixiaohua1020/TIandSI](http://git.oschina.net/leixiaohua1020/TIandSI)

**CSDN下载**

> 
源代码：[http://download.csdn.net/detail/leixiaohua1020/8754819](http://download.csdn.net/detail/leixiaohua1020/8754819)

windows可执行程序：[http://download.csdn.net/detail/leixiaohua1020/8755215](http://download.csdn.net/detail/leixiaohua1020/8755215)






## 使用方法


### TIandSIprocmd
TIandSIprocmd是用于计算压缩码流的TI和SI的命令行工具。该工具有如下选项。

> *-i 输入文件-o 输出包含TI和SI的*.csv 文件 [默认是 {input name}.csv]-l 计算的帧数-g 图形化显示计算的中间图像 [可以是 'ti' 或者 'si']-n 中间图像显示的间隔 [默认是 5]-h 打开帮助*
下面举几条示例命令。

计算“test.avi”的TI和SI，结果保存在“test.csv”。


`TISIprogCmd -i test.avi`


计算“test.avi”的TI和SI，结果保存在“test_cal.csv”。


`TISIprogCmd -i test.avi -o test_cal.csv`

计算“test.avi”的TI和SI，计算过程中显示TI的中间图像。
`TISIprogCmd -i test.avi -g ti`
程序在运行过程中会在屏幕上打印出TI和SI，并将结果保存成“.csv”文件。程序运行截图如下所示。

![](https://img-blog.csdn.net/20150530183044477?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


显示的SI中间信息如下所示（Sobel滤波）。

![](https://img-blog.csdn.net/20150530183242407?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

显示的TI中间信息如下所示（帧差）。

![](https://img-blog.csdn.net/20150530183242320?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


### TIandSIpro

TIandSIprocmd是用于计算压缩码流的TI和SI的图形界面工具。该工具的界面如下所示。

![](https://img-blog.csdn.net/20150530183458470?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

该工具的使用可以分成三步：

> （1）选择视频文件
（2）点击“添加”将视频文件添加到“视频处理列表”
（3）点击“开始”进行计算

计算的过程中可以选择输出TI或者SI中间信息，如下图所示。

![](https://img-blog.csdn.net/20150530183404095?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


输入的视频为H264裸流，如下图所示。此外也支持mp4、mkv、avi等容器中的视频码流。

![](https://img-blog.csdn.net/20150530183712678?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

计算的结果保存成了“.csv”文件，TI和SI分成两列进行记录。


|TI|SI|
|----|----|
|5.091792|96.63046|
|4.006694|97.14991|
|4.047168|95.78278|
|4.016123|96.68823|
|5.111005|95.08357|
|4.308331|95.99482|
|4.231775|94.96137|
|4.243418|96.51315|
|4.777538|94.69438|
|4.63109|95.99242|
|4.442572|95.27241|
|5.958293|94.07044|
|4.802421|93.54437|
|3.626003|94.44892|
|67.47028|55.96951|
|4.859533|55.73122|
|2.576851|55.94511|
|3.537802|55.95005|
|3.136736|56.02485|
|2.776622|55.88079|
|5.21654|56.21251|
|4.816776|56.34703|
|2.113539|56.39521|
|2.463277|56.54902|
|3.58151|56.62169|
|4.281242|56.89031|
|2.611747|56.9627|
|2.134903|56.98027|
|2.935039|56.72913|
|2.607401|56.88178|
|1.745702|56.99831|
|2.356833|57.06902|
|3.137143|56.77847|
|3.540902|56.97581|
|2.897511|56.95166|
|2.958283|56.95023|
|3.18626|56.48377|
|3.212861|56.56799|
|3.140944|56.53749|
|3.214078|56.51278|
|3.40129|56.14477|
|3.60599|56.56562|
|4.518811|56.72331|
|7.05519|56.32037|
|6.418852|56.10862|
|3.521006|56.34639|
|3.380948|56.21924|
|4.811128|56.01251|
|5.344801|56.30534|
|5.112007|56.1002|
|4.340466|55.77702|
|4.603604|55.81942|
|5.183671|55.94553|
|5.25707|55.90999|
|6.184795|55.43008|
|6.549098|55.66671|
|7.301097|55.66413|
|6.839081|55.41397|
|6.012524|55.64191|
|6.21944|55.88689|
|7.074984|54.75292|
|7.934908|55.05619|
|6.990477|55.15948|
|6.806325|54.92125|
|7.230724|54.62891|
|5.788167|54.68493|
|5.61761|54.06684|
|5.728929|54.35502|
|5.711939|54.16241|
|5.219654|54.27668|
|5.367257|54.10954|
|5.890024|54.08825|
|7.249853|53.98721|
|6.442775|54.22794|
|6.254034|53.70972|
|6.627275|54.26056|
|6.446436|54.10505|
|6.364463|54.14791|
|6.606837|54.38265|
|7.350156|55.03591|
|6.401467|54.62808|
|4.030468|54.80471|
|4.21936|54.94787|
|3.748936|54.91839|
|3.503878|54.6622|
|4.081355|54.84961|
|3.576916|54.7182|
|3.589109|54.85014|
|3.072421|54.54525|
|3.071072|54.48639|
|3.125489|54.22224|
|3.183908|54.37459|
|3.93243|54.0887|
|TI_AVG|SI_AVG|
|5.308657|61.55853|






输入Excel之后可以得到以下结果。图中TI的峰值处出现了画面的突变。SI发生变化的地方视频的场景发生了变化。

![](https://img-blog.csdn.net/20150530184032795?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20150530184048286?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)






**雷霄骅leixiaohua1020@126.comhttp://blog.csdn.net/leixiaohua1020**










