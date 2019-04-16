# 视频客观质量评价工具：MSU Video Quality Measurement Tool - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月17日 14:57:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：30









MSU Video Quality Measurement Tool（msu vqmt）是莫斯科国立大学（Moscow State University）的Graphics and Media Lab制作的一款客观视频质量评价程序。它提供了多种全参考视频质量评价方法（对比两个视频）和无参考视频质量评价方法（分析一个视频）。

软件地址：[http://www.compression.ru/video/quality_measure/video_measurement_tool_en.html](http://www.compression.ru/video/quality_measure/video_measurement_tool_en.html)

软件工作方式就是通过计算原始视频和受损视频之间的参数，得到视频的质量。

视频客观质量评价方法介绍可参考：

[全参考客观视频质量评价方法 （MSE， PSNR，SSIM）](http://blog.csdn.net/leixiaohua1020/article/details/11729289)

[全参考视频质量评价方法（PSNR，SSIM）以及相关数据库](http://blog.csdn.net/leixiaohua1020/article/details/11694369)

![](https://img-blog.csdn.net/20130917145340703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

下面简单介绍一下MSU Video Quality Measurement Tool的使用。打开软件之后界面如图所示。首先（Step1）选择想要计算的两个序列。一个是原始序列，一个是受损序列。例如在这里选择了“src01_480x272.yuv”（原始序列）和“src01_480x272_0.025.yuv”（受损序列）。然后（Step2）选择客观质量评价算法。例如在这里选择的是SSIM。MSU VQMT支持相当丰富的视频客观质量评价算法，一般情况下都是足够使用的。然后（Step3）选择想要输出的数据。例如这里勾选了两项：Save CSV file，以及Save metric visualization video。CSV file是一个Excel表格，保存了算法的计算结果。metric visualization video是一个视频文件，可视化的呈现了算法的计算结果。最后，单击“Process”即可进行计算。

![](https://img-blog.csdn.net/20140503162119937?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

单击序列路径旁边的“Preview”，可以查看序列的内容。

![](https://img-blog.csdn.net/20140503162127250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

单击“Process”进行计算后，可以查看计算的结果，如图所示。图中列出了每一帧的计算结果。单击下方的“Show Frame”按钮，可以查看当前光标所在帧的内容。

![](https://img-blog.csdn.net/20140503162135171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

计算完成后输出的metric visualization video如下图所示。

![](https://img-blog.csdn.net/20140503162139859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




