# PhysioBank简介 - 工作笔记 - CSDN博客





2012年01月12日 10:40:31[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：10073








PhysioBank简介

**PhysioNet**（[www.physionet.org）]()

PhysioNet提供免费的网站访问记录生理信号的数据库（PhysioBank）和相关的开源软件（PhysioToolkit）。每个月，全球约45,000访客使用PhysioNet，约4 TB的数据检索。

[**PhysioNetWorks**](https://physionet.org/users/)工作区用来PhysioNet的社区成员将在PhysioBank和PhysioToolkit公开时，完成在建工程。

PhysioBank：目前包括多参数的心肺功能，神经，和其他生物医学信号从各种主要的公众健康的影响，包括心脏猝死，充血性心脏衰竭，癫痫，步态失调，睡眠呼吸暂停的条件与健康受试者和患者数据库 ，并老化。提供一个拥有50个心电信号的数据库，共10000个心电信号数据，数据量高达700G！而大部分数据都是免费提供的，可以从网站上直接下载。

用户可以通过网址[http://physionet.org/cgi-bin/ATM](http://physionet.org/cgi-bin/ATM)，使用浏览器，在线地显示选定的心电数据，还可以将这些数据保存成文本的形式，或者发送到邮箱。

另外，用户可以通过网址[http://www.physionet.org/physiobank/database/](http://www.physionet.org/physiobank/database/)，来了解和查看各种数据库的入口。这些数据库大多是ECG有关的，也有步态数据库（Gait
 Databases）等一些非ECG的数据库，但数量不多。

每个ECG数据库都包括了一些记录（record），每个记录至少包含3个文件。后缀为.dat的是心电信号的数据文件（二进制格式），后缀为.atr的是心电信号的标注（annotation）文件，后缀为.hea的是心电信号的头（header）文件，该文件是一个文本文件，包含了心电信号数据文件的名字或者URL、存储格式、信号的类型和数量、采样频率等信息。

由于数据量较大，一些镜像站点仅仅收录了它的子集，称之为The PhysioBank Core Collection（PhysioBank核心集）。





**PhysioToolkit**：对于开发者而言，PhysioNet提供了一个开发包（WFDB Software Package），支持心电信号读取及分析（WFDB库），还有显示及标注（WAVE软件）。这个开发包都是开源（Open
 Source）的，且是跨平台的，可以在Windows、Linux、Unix、Mac OS等流行的操作系统平台上使用。





如果用户想开发自己的软件用于读取PhysioBank的数据，推荐使用WFDB库，该库支持C, C++, Fortran, Java, Matlab, Perl, Python等流行的编程语言。使用该库的好处在于：如果PhysioBank的数据格式或者读取方式发生变化的话，可以很容易地升级用户的程序。





更为有用的是：WFDB提供了评估心电信号分析算法（*Evaluation of ECG analyzers*）的工具，这些工具尊需ANSI/AAMI EC-38和EC-57标准。包括：bxb,
 ecgeval, epicmp, mxm, plotstm, rxr, sumstats等。这样的话，用户就可以评估自己的算法的效率了，而不用自行编制评估程序。

参见：[http://www.physionet.org/physiotools/software-index.shtml](http://www.physionet.org/physiotools/software-index.shtml)





详细的评估ECG分析算法或设备的内容。参见：

http://www.physionet.org/physiotools/wag/eval.htm





当然，如果你仍然执意要使用自己的程序来读取PhysioBank的数据，则在[*WFDB Applications Guide*](http://www.physionet.org/physiotools/wag/)中有关于数据文件格式的详细描述。





（以上内容大部分节选翻译自：An Introduction to the PhysioBank Archives，网址：http://www.physionet.org/physiobank/physiobank-intro.shtml）



