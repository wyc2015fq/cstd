# 视频主观质量评价工具：MSU Perceptual Video Quality tool - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2014年05月04日 00:29:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：46
个人分类：[视频质量评价](https://blog.csdn.net/leixiaohua1020/article/category/1446693)










MSU Perceptual Video Quality tool是莫斯科国立大学（Moscow State University）的Graphics and Media Lab制作的一款视频主观评价工具，使用起来比较方便。最近这段时间赶上毕设需要大量的主观评价试验，使用了一阵子这个软件，感觉还是挺方便的，记录一下它的使用方法。

软件地址：[http://www.compression.ru/video/quality_measure/perceptual_video_quality_tool_en.html](http://www.compression.ru/video/quality_measure/perceptual_video_quality_tool_en.html)



## **1.软件的特性**

**（1）6种主观评价方法（其中有4种来自于ITU-R BT.500标准）。包括：**

lDSIS（Double Stimulus Impairment Scale，双激励损伤量表）

lDSCQS（Double Stimulus Continuous Quality Scale，双激励连续质量量表）type I and type II

lSCACJ（Stimulus Comparison Adjectival CategoricalJudgement）

lEBU SAMVIQ（Subjective Assessment Method for Video Quality evaluation）

lMSUCQE（MSU Continuous Quality Evaluation）

注：主观评价方法详细介绍参考：[视频主观质量评价方法](http://blog.csdn.net/leixiaohua1020/article/details/11730313)

**（1）3种统计结果的方法**

**（2）支持.avi文件和.avs文件**

**（3）结果保存为csv格式文件**



## **2.使用方法**

软件分为2个部分：

"MSU Perceptual Video Quality - task manager"：任务管理器，用于实验设计人员设置实验。

"MSU Perceptual Video Quality - player"：播放器，用被测人员参与试验。

一般情况下先使用task manager设置实验，然后使用player进行实验。整体使用流程如下：

（a）选择原始序列（一般为YUV，RGB或者其他非压缩格式）

（b）选择受损序列（可以是YUV等非压缩格式，也可以是H264这样的压缩码流）

（c）选择测试使用的主观评价方法（即怎样对比原始序列和受损序列）

（d）寻找实验人员，做实验

（e）统计实验结果



下面分别介绍两个软件。

### **（1）MSU Perceptual Video Quality - task manager**

软件运行后的截图如图所示。对于新用户，首先给task取一个名称。注意，本软件对中文支持有问题，因此有些名称的task是无法保存的。因此起名字的时候需要使用英文，并且尝试能否保存。后文关于此问题有详细的叙述。

然后，单击任务列表下方的“Add file”添加原始序列和参考序列。添加完成后，所有序列会列在任务列表中。此时需要选择一个序列作为参考序列（即原始序列）。单击列表中序列后，再单击“Set task reference”即可。

最后，在软件的左下方选择主观评价的实验方法。设置完成后，单击“Save task”，保存任务文件就可以了。



![](https://img-blog.csdn.net/20140503165950125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### **（2）MSU Perceptual Video Quality - player**

软件运行后，首先输入测试人员的姓名。然后选择任务文件（*.tsk）进行主观评价。

![](https://img-blog.csdn.net/20140503165955562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

下面简单展示以一下几种常用的主观评价方法。

### **（a）DSIS（Double Stimulus Impairment Scale，双激励损伤量表）**

最基础的视频主观质量评价方法。受测者每次观看原始参考视频和受损视频组成的“视频对”，并且每次总是先观看原始参考视频，然后观看受损视频。评分采用5分制失真测度。下图分别显示了观看参考视频和受损视频时候的截图。由图可见，软件的下方还包含了控制视频播放的各个按钮。

![](https://img-blog.csdn.net/20140503170002671?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20140503170012468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

每看完一组“视频对”之后，会弹出对话框要求对受损视频进行打分。如果没看清楚，可以单击“Watch Again”。打分采用5分制，如图所示。

![](https://img-blog.csdn.net/20140503170026828?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### **（b）DSCQS（Double Stimulus Continuous Quality Scale，双激励连续质量量表）type I**

受测者每次观看原始参考视频和失真视频组成的“视频对”，但与DSIS不同的是，原始参考视频和失真视频的显示顺序是随机的，并且受测者对每个“视频对”中两幅视频的质量都进行打分。需要注意的是DSCQS本身要求打分取值是连续的，但是本软件目前实现的是5分制。

软件中DSCQS有2种。其中type I是每次显示一个视频，测试人员可以在视频播放的任意时刻，按键盘上的“Tab”键在原始序列和受损序列之间切换，用于查看两个序列之间的区别。

![](https://img-blog.csdn.net/20140503170031937?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

播放完成之后，需要对两个序列都进行打分，如图所示。

![](https://img-blog.csdn.net/20140503170038390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### **（c）DSCQS（Double Stimulus Continuous Quality Scale，双激励连续质量量表）type II**

DSCQS的type II是每次同时显示两个视频，其他方面和DSCQS I是一样的。如图所示。

![](https://img-blog.csdn.net/20140503170046000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## **3.其它需要注意的地方**

### **（1）实验环境**

主观评价需要一定的实验环境，例如灯光这些条件。因此做主观评价的时候，需要先选定合适的实验环境。

![](https://img-blog.csdn.net/20140503170053046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### **（2）批量实验**

当需要做多个实验的时候（即有多个tsk文件），可以在软件目录下新建一个批处理.bat文件，添加以下内容

"MSU perceptual video quality player.exe" "c:\tasks\task1.tsk" "c:\tasks\task2.tsk" "c:\tasks\task3.tsk"

执行该批处理文件，测试人员即可按照task1.tsk，task2.tsk，task3.tsk的顺序进行实验。

### **（3）色盲**

有色盲的测试人员可能会干扰到实验结果。本软件提供了监测色盲的功能。在软件目录下新建一个批处理.bat文件，添加以下内容

"MSU perceptual video quality player.exe" -c

执行该批处理文件即可检查测试人员是否有色盲。截图如下所示。

![](https://img-blog.csdn.net/20140503170101484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## **4.使用中可能的问题**

以下是自己使用过程中出现的问题，总结一二。

### **（1）task manager中task的命名问题**

本软件对中文支持有问题，因此起名字的时候需要使用英文。但是尽管使用了英文，有的时候依然不能保存。这是一个比较棘手的问题，经过多次尝试，总结出了一些经验。

保存的时候，任务名称为“asd”的时候，可能弹出下面对话框：

![](https://img-blog.csdn.net/20140503170106671?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

后面的问号（有时候是乱码）是不合法的字符。但是实际上我们并没有输入这个字符。我的觉得可能是保存任务名的内存没有清0导致的，因此，后面有几个乱码字符，我们就在任务名后面添加几个字符，这样就可以覆盖掉非法字符了。例如我们把“asdf”改为“asdff”，发现就可以保存了（注意：有的时候可能要反反复复好几次，但是最后肯定能保存）。



### **（2）task manager中无法添加AVI文件。**

本软件只支持AVI封装格式的视频文件。所以无论什么测试视频都要转换为AVI封装格式才能被软件识别。注意本软件的测试序列并不限于原始数据序列（YUV，RGB等），也可以使用压缩编码序列（h264，mpeg4等）。

在这里可以使用FFmpeg对测试序列进行封装。注意一定要使用“-vcodeccopy”参数，这样才能保证在不改变编码方式的情况下封装视频数据，否则会导致视频重新编码从而影响到视频的质量。下面举几个例子。

H264码流封装成AVI：


`ffmpeg -i src11_480x272.264 -vcodec copy src11_480x272.avi`




YUV420P数据封装成AVI：



```cpp
ffmpeg -s 480x272 -i src11_480x272.yuv -vcodec copy src11_480x272.avi
```





### **（3）player播放AVI文件失败。**

本软件中的播放器是基于DirectShow的，所以需要安装相应的解码器才能保证可以播放AVI文件中的码流。在这里介绍一个最简单而且通用的方法：下载安装一个《终极解码》软件。《终极解码》附带了大量的解码器，足够用了。当然，安装其它解码器软件也可以解决问题。






