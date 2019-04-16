# DVB数字电视系统简介（DVB-C，DVB-S，DVB-T） - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2015年02月21日 18:23:21[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：41116标签：[DVB																[DVB-C																[DVB-S																[DVB-T																[数字电视](https://so.csdn.net/so/search/s.do?q=数字电视&t=blog)
个人分类：[综合/IT与广电																[数字电视网络](https://blog.csdn.net/leixiaohua1020/article/category/2899497)](https://blog.csdn.net/leixiaohua1020/article/category/1645277)





前一段时间在《通信原理》期末的时候研究了一下DVB数字电视系统。视音频编解码这些技术都是属于“信源”的技术，而《通信原理》研究的范围正好是它的补集，属于“信道”方面的技术。由于自己不是做这方面的，所以信道方面的技术此前接触的比较少，因此打算把搜集的资料简单整理一下记录下来以备查阅。本文就打算记录一下DVB方面的知识。




## 1.简介

数字视频广播（英语：Digital Video Broadcasting，缩写：DVB），是由“DVB Project”维护的一系列为国际所承认的数字电视公开标准。“DVB Project”是一个由300多个成员组成的工业组织，它是由欧洲电信标准化组织（European Telecommunications Standards Institute, ETSI）、欧洲电子标准化组织（European Committee for Electrotechnical Standardization, CENELEC）和欧洲广播联盟（European Broadcasting Union, EBU）联合组成的“联合专家组”（Joint Technical Committee, JTC）发起的。DVB组织的徽标如图1所示。

![](https://img-blog.csdn.net/20150219162258913)

图1. DVB Project的LOGO

DVB标准现如今被应用于世界上大部分的国家和地区。图2显示了各国使用的数字电视地面广播标准。图中浅蓝色代表DVB-T标准，深蓝色代表DVB-T2标准，绿色代表ATSC标准，粉红红色代表ISDB-T标准，黄色代表DTMB标准（我国的标准）。从图中可以看出ATSC的使用国家主要分布在北美地区，ISDB-T的使用国家主要分布在南美地区，我国使用自主的DTMB标准，而剩下的地区全部采用了DVB-T标准。

![](https://img-blog.csdn.net/20150219162508035?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图2. 数字电视地面广播标准使用范围




## 2.EBU

DVB Project 最主要的一个支持者是欧洲广播联盟。欧洲广播联盟（European Broadcasting Union，EBU）简称欧广联，为欧洲与北非各广播电台与电视台之间的组织，成立于1950年2月12日。总部位于瑞士日内瓦。此外欧广联在北京、布鲁塞尔、伦敦、马德里、莫斯科、新加坡及华盛顿都设有办事机构。欧广联的徽标如图3所示。

![](https://img-blog.csdn.net/20150219162348628)

图3. EBU的LOGO

欧广联宗旨是维护成员在各个领域的利益，与其他广播组织建立联系，援助各种广播形式的发展，促进广播电视节目的交换，研究与广播有关的问题，保障对广播组织共同感兴趣的信息交换，努力确保各成员遵守国际协议的条款，协助成员组织参加任何种类的谈判，或应它们的要求代表它们去谈判。

欧广联是联合国教科文组织的咨询机构。只有国际电信联盟成员国中被权力当局授权经营一家全国性或在全国有重大影响的广播机构才能加入该组织。任何在欧洲广播区域以外提供电台或电视服务的国际电信联盟成员，都能申请成为欧广联的准成员。各国家准会员需缴纳年费以维持身分，否则将会撤销其准会员身份。需要注意的是被授予准成员身份的电视台并不能进入主成员。中国的中央电视台就在欧广联的准会员列表中。欧广联的成员国分布如图4所示，图中深蓝色国家是成员国，浅蓝色的国家是准成员。从图中可以看出，成员国分布在欧洲和北非地区，准成员国则分布在世界各地。

![](https://img-blog.csdn.net/20150219162520015?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


图4. EBU成员国和准成员国的分布

欧广联一直处在新媒体研究和开发的前沿阵地，逐步发展了很多新的广播电视系统：无线数据系统 (RDS)，数字音频广播(DAB)，数字视频广播(DVB)，高清晰电视(HDTV)等。




## 3.DVB框架结构

### 1)分层结构

DVB标准中描述的系统可以概括成图5所示的结构。根据所属的层次不同从上层到底层可以分为：音视频编码层，服务信息层，基带传输层，信道编码层，射频层。

![](https://img-blog.csdn.net/20150219162532589?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


图5. DVB分层结构

发送端按照箭头所指的方向完成多媒体数据的处理并且发送出去，而接收端则按照相反的方向完成数据的处理并送到终端显示屏/音频系统上显示画面。这些层次完成的工作可以概括如下：
- 音视频编码层：使用MPEG1、2等多项标准对模拟音视频信号进行采样和压缩。
- 服务信息层：使用DVB SI标准产生PSI、SI和EPG等信息服务。
- 基带传输层：使用ASI（异步串行）、SPI（同步并行）、SSI（同步串行）接口。
- 信道编码层：使用各种DVB-S、DVB-C、DVB-T信道编码。
- 射频层：使用卫星、CATV（有线电视网）、SFN（单频网）、Internet等进行信号传送。

### 2)发送端模型

发送端模型如图6所示。发送端的输入为图像/声音数据，输出为调制后的射频信号。输入的视音频数据经过音视频编码，复用，调制，发送4个步骤形成最终的信号。输入的视音频信号首先经过“Encoder”压缩编码为ES流；然后经过“Packetliser”打包为PES流之后，经过“Multiplexer”复用成TS流；接着经过“Modulation”调制成为射频信号，最后发送调制后的信号到不同的信道中。

![](https://img-blog.csdn.net/20150219162554819?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


图6. 发送端模型

### 3)接收端模型

接收端模型如图7所示。发送端的输入为射频信号，输出为图像/声音数据。接收端做的工作与发送端是完全相反的。输入的视音频数据经过接收，解调制，解复用，解码4个步骤完成信号的接收，并最终输出到终端的显示屏/音响设备中。

![](https://img-blog.csdn.net/20150219162629589?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


图7. 接收端模型




## 4.DVB系统组成

DVB系统按照信号传播的顺序可以分成前端系统，传输系统和终端系统。其中前端系统一般位于节目生产部门（例如电视台等部门），而终端系统一般用户设备中（例如机顶盒）。

### 1)前端系统

DVB系统中的前端系统，主要是指数字电视网络的信息源部分，是电视节目信息的交换中心, 属于整个数字有线电视系统的核心部分，常被业界誉为“心脏”。前端根据作用的不同可以分成转播数字前端和存储播出前端。转播数字前端通常包括复用器、实时编码器、扰码器、适配器、矩阵、网管等系统。存储播出前端通常包括节目存储、TS节目制作、媒体资产管理、视频服务器播出、媒体资产、EPG、证券信息、综合信息等各种应用系统。

### 2)传输系统

DVB系统中的传输系统，主要是指数字电视网络的信道部分。最常见的三种传输系统是DVB-C、DVB-S和DVB-T。DVB-C用于数字有线电视系统，DVB-S用于数字卫星电视系统，DVB-T用于数字地面电视广播系统。上述三种系统的徽标如图8所示。

![](https://img-blog.csdn.net/20150219162659323)


图8. DVB-C、DVB-S和DVB-T的LOGO

### 3)终端系统

DVB系统中的终端系统，主要是指数字电视网络的信宿部分。终端系统主要提供给数字电视的用户使用，因此产量很大而且售价便宜。比较常见的终端有机顶盒（STB）、接收卡、数字电视机、移动设备等。




## 5.DVB-S传输方式

本节简单介绍DVB-S系统的传输方式。

### 1)原理

DVB-S用于卫星信道。它的系统结构如图9所示。

![](https://img-blog.csdn.net/20150219162726825?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图9. DVB-S系统结构

卫星信道的特点是:可用频带宽、功率受限、干扰大、信噪比低。所以要求采用可靠性高的信号调制方式、强的信号纠错能力,对带宽要求不是特别高。QPSK是一种恒包络相位调制技术，使用QPSK将数字信息体现在相位上，只要相位不发生变化，幅度上的衰减和失真不会影响接收，抗雪衰、雨衰能力强。

### 2)调制

DVB-S采用QPSK调制器，对复用后的传输流进行信道编码，内码采用卷积码，外码采用RS分组码，再经过QPSK调制，在卫星11G/12GHz Ku频段广播发送数字电视。QPSK的调制方式如图10所示。

![](https://img-blog.csdn.net/20150219162749633?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图10. QPSK调制方式示意图

### 3)参数

DVB-S包含如下几个常见的参数。
- 下行频率：卫星上面每个转发器都有一个固定下行频率。下行频率的设定跟LNB进行混频之后得到接收机高频头的950MHz-2150MHz频段的高频信号。LNB工作在C波段时候混频频率是5150MHz，工作在Ku波段时候是13100MHz。所以C波段的下行频率在3000MHz-4200MHz之间；Ku波段下行频率在10950MHz-12150MHz之间。
- 符号率：QPSK的符号率就是I、Q单路相位信号的符号率，每符号携带2比特的信息。视频节目的QPSK的符号率一般在2MS-40MS之间。
- 极化方式：极化方式是卫星转发器下行的时候载波的偏振方向，作用是提高同频载波的利用率。常见的QPSK极化方式有垂直极化和水平极化，椭圆极化基本上不用于民用了。


## 6.DVB-C传输方式

本节简单介绍DVB-C系统传输方式。

### 1)原理

DVB-C用于有线信道。它的系统结构如图11所示。

![](https://img-blog.csdn.net/20150219162807760?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图11. DVB-C系统结构

有线信道的特点是:信噪比高、频带资源窄、存在回波和非线性失真。DVB-C采用带宽窄、频带利用率高、抗干扰能力较强的调制方式。同时,由于信道信噪比高,误码率较低,纠错能力要求不很高。因此,DVB-C的信道部分采用RS码和卷积码交织技术,正交幅度调制(QAM)。QAM可以说是QPSK的复杂化模型，当调制信号的相位和幅度都产生变化时候，就形成了相位星座图。

### 2)调制

DVB-C采用QAM调制器，对传输流采用16、64、128或者256QAM方式进行调制，用于通过有线电视（CATV）系统传送多路数字电视节目。QAM的调制方式如图12所示。

![](https://img-blog.csdn.net/20150219162830739?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

图12. QAM调制方式示意图

### 3)参数
DVB-C包含如下几个常见的参数。
- 载波频率：载波频率是使用传统模拟电视的频道资源的频率，传统模拟电视由51-858MHz，每个频道的频率固定。一般数字电视使用较高的频段，避开大部分模拟VSB信号的邻频干扰。
- 符号率：由于QAM方式的不同，每符号携带的比特数目不一样，QAM方式越大，同样符号率的码流的码率就越高。
- QAM方式：有16、32、64、128、256QAM方式，常用64QAM方式，符号率在6M-7M之间。
- 内编码方式：删余卷积码 1/2 , 2/3 , 3/4 , 5/6 , 6/7 或者7/8。


## 7.DVB-T传输方式

本节简单介绍DVB-T系统传输方式。

### 1)原理

DVB-T用于地面广播信道。它的系统结构如图9所示。

![](https://img-blog.csdn.net/20150219162833222?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


图13.DVB-T系统结构

地面广播的特点是:地形复杂、存在时变衰落和存在多径干扰、信噪比较低。DVB-T采用前向纠错(FEC)(包括内码交织、内码卷积编码、外码交织、外码RS编码)和能有效消除多径干扰的正交频分复用技术(COFDM)和格雷码映射4/16/64QAM调制等进行信道处理。然后在原来用于模拟的6MHZ、7MHZ和8MHZ的频带内发送数字电视节目。DVB-T发送的比特率是可变的。例如：在6MHZ频带可在3.7-23.8Mbit/s比特率之间进行选择; 在8MHZ频带可在4.9-31.7Mbit/s比特率之间进行选择以适应不同的接收环境（如移动接收应适当降低发送的码率）。

### 2)调制

DVB-T采用COFDM调制器，采用多载波（2000多个或者8000多个）正交复合调制方式，在一个COFDM数据帧中，所有载波上使用一样的格雷码映射的QPSK、16QAM或64QAM调制，调制方式最复杂。COFDM的调制方式如图14所示。

![](https://img-blog.csdn.net/20150219162907620)


图14. COFDM调制方式示意图

### 3)参数

DVB-T包含如下几个常见的参数。
- 载波频率：是使用传统模拟电视频道资源的频率，DVB-T一般由470-860MHz，每个频道的频率固定。使用高频段避开大部分模拟VSB信号的邻频干扰。
- 带宽模式：有三种带宽模式，分别时6MHz、7MHz和8MHz
- FFT模式：DVB-T有两种IFFT模式，分别是2K模式和8K模式，其中“2K”和“8K”表示子载波数目。一般移动接收使用2K模式，固定接收使用8K模式。
- 载波调制方式：在每个子载波上面分别有QPSK，16QAM和64QAM方式，可以由芯片自动识别。
- 内编码方式：删余卷积码 1/2 , 2/3 , 3/4 , 5/6 , 6/7或者7/8。
- 保护间隔设定：有四种保护间隔参数，分别是1/32, 1/16, 1/8, 1/4个符号周期。](https://so.csdn.net/so/search/s.do?q=DVB-T&t=blog)](https://so.csdn.net/so/search/s.do?q=DVB-S&t=blog)](https://so.csdn.net/so/search/s.do?q=DVB-C&t=blog)](https://so.csdn.net/so/search/s.do?q=DVB&t=blog)




