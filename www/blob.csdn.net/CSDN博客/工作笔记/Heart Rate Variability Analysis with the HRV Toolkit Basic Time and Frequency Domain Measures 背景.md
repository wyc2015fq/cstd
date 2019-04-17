# Heart Rate Variability Analysis with the HRV Toolkit: Basic Time and Frequency Domain Measures/背景 - 工作笔记 - CSDN博客





2012年01月18日 14:57:33[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：7770标签：[domain																[basic																[intervals																[statistics																[matlab																[工具](https://so.csdn.net/so/search/s.do?q=工具&t=blog)
个人分类：[信号处理](https://blog.csdn.net/App_12062011/article/category/1063270)





Heart Rate Variability Analysis with the HRV Toolkit:
Basic Time and Frequency Domain Measures

 利用HRV工具包 做心率变异性分析：

**Background: Joseph E. Mietus, B.S. and Ary L. Goldberger, M.D.Software and related material: Joseph E. Mietus, B.S.Margret and H.A. Rey Institute for Nonlinear Dynamics in Physiology and MedicineDivision of Interdisciplinary Medicine and Biotechnology and Division of CardiologyBeth Israel Deaconess Medical Center/Harvard Medical School, Boston, MA**



## I. Background          -------背景


Heart rate variability (HRV) analysis attempts to assess cardiac autonomic regulation through quantification of sinus rhythm variability. The sinus rhythm times series is derived from the QRS to QRS
 (RR) interval sequence of the electrocardiogram (ECG), by extracting only normal sinus to normal sinus (NN) interbeat intervals. Relatively high frequency variations in sinus rhythm reflect parasympathetic (vagal) modulation, and slower variations reflect
 a combination of both parasympathetic and sympathetic modulation and non-autonomic factors [1-5].

心率变异性（HRV）分析试图 通过窦性节律变异的定量分析 评估心脏自主调节功能。窦性节律时间序列从ECG 的QRS到QRS（RR）间期序列 推导出来，只提取出正常窦性到正常窦性（NN） 相邻心拍间期。窦性节律中相对高频变化反映副交感神经（迷走神经）调节作用，低频变化反映副交感神经和交感神经调节功能 与非自主 因素 综合作用。



Traditional heart rate variability (HRV) measures are usually divided into two broad categories: time domain measures and frequency domain measures [3,4]. The time domain heart rate variability statistics
 commonly calculated are defined in[Table 1](http://www.physionet.org/tutorials/hrv-toolkit/#table1). Note, however, that computing[pNNx](http://www.physionet.org/physiotools/pNNx/)with
 x < 50 ms in both long- and short-term recordings may provide a more robust index of fluctuations due to vagal tone than the standard pNN50 statistic [6]. Commonly used frequency domain measures are defined in[Table
 2](http://www.physionet.org/tutorials/hrv-toolkit/#table2).

传统的心率变异性测量值 常常被分为两大类：时域测量和频域测量。时域心率变异性统计常用的参数见表1。然而，注意到在长程和短程记录中计算pNNx ，x < 50ms, 可以提供比 标准的pNN50指标 更具鲁棒性的 归因于迷走神经张力的波动性指标。常用的频域测量指标定义见 表2。



The low frequency band (0.04 - 0.15 Hz) includes physiologic oscillations associated with baroreceptor reflexes and the high frequency band (0.15 - 0.40 Hz) encompasses respiratory sinus arrhythmia.
 The powers in these bands has been used to provide indexes of autonomic function. Such measures must be interpreted with caution, however. As noted, oscillations in the "low" frequency bands appear to be mediated by parasympathetic and sympathetic components,
 while the "high" frequency power is mediated exclusively by the vagus.

低频带（0.04—0.15Hz）包含 与压力感受器反射有关的 生理振荡， 高频带（0.15—0.40Hz） 涉及呼吸性窦性心律失常。这些频段的功率用来提供自主神经功能的指标。然而，这些测量值必须小心解释。如前所述，“低”频带的振荡似乎受副交感神经和交感神经成分调节，而“高”频功率仅受迷走神经调节。



Traditionally, frequency domain measures are calculated by resampling the original NN interval series and then applying the fast Fourier transform or autoregressive spectral estimation (the maximum entropy
 method). This resampling, however, can cause an attenuation in the high frequency components. If discontinuities exist in the NN interval series, either because of the presence of abnormal beats or because of gaps or extreme noise in the original ECG recording,
 traditional approaches require either discarding the data or guesswork to estimate the locations of missing normal beats. To eliminate the need for evenly sampled data required by Fourier or maximum entropy methods, frequency domain spectra can be calculated
 using the[Lomb periodogram](http://www.physionet.org/physiotools/lomb/lomb.html)for unevenly sampled data [7,8] (the method used in this toolkit).

传统上，频域测量指标的计算是通过对原始NN间期序列重采样，然后应用快速傅里叶变换或自回归谱估计（最大熵方法）。然而，重采样会导致高频成分的衰减。如果在NN间期序列中不连续性存在，要么因为异常心拍的存在，要么因为原始ECG记录中存在间断 或是 过度的噪声，传统的方法要求要么放弃这些数据 要么估计出缺失的正常心拍的位置。为了去掉均匀采样过程（傅里叶变换或最大熵方法要求），频域谱可以利用Lomb周期图来计算，适用于非均匀采样数据（工具包中使用了该方法）。



Although the long term (24-hour) statistics of SDANN, SDNNIDX and ULF power can be calculated for shorter data lengths, they will become increasingly unreliable. For short-term data (less than 15 minutes
 in length), only the time domain measures of AVNN, SDNN, rMSSD and pNN50 and the frequency domain measures of total power, VLF power, HF power and LF/HF ratio should be used.

A number of the HRV measures are highly correlated with each other. These include SDNN, SDANN, total power and ULF power; SDNNIDX, VLF power and LF power; and rMSSD, pNN50 and HF power. The LF/HF ratio
 does not correlate strongly with any other HRV measures [4].

虽然长程（24小时）统计量SDANN,SDNNIDX 和ULF功率对于较短的数据长度也能进行计算，它们会变得越来越不可靠。对于短期数据（长度小于15分钟），只有时域指标AVNN, SDNN, rMSSD, pNN50 和频域指标 总功率，VLF功率，HF功率，LF/HF比可以使用。许多HRV测量指标彼此高度相关。包括SDNN, SDANN, 总功率和ULF功率；SDNNIDX,
 VLF功率和LF功率；rMSSD, pNN50 和HF功率。LF/HF比值和任何其他HRV指标没有强的相关性。



Heart rate variability (HRV) has been widely applied in basic and clinical research studies. Its clinical application is very limited at present, however. These limitations are due to lack of standardization
 of methodology and application to different non-comparable subsets of subjects, as well as to the confounding effects of age, gender, drugs, health status, and chronobiologic variations, among others. Furthermore, outliers due to ectopy and artifact can have
 major effects on computed HRV values. In elderly subjects, especially, a spuriously high value of certain measures may be due to the effects of "erratic supraventricular rhythm" [9] due to subtle atrial ectopy, wandering atrial pacemaker, or sinus node conduction
 abnormalities. Additional information on heart rate dynamics and analysis techniques, including non-linear and complexity based measures, can be found in the[HRV
 2006 course notes](http://www.physionet.org/events/hrv-2006/course-materials.html)and elsewhere on PhysioNet (see for example:[Detrended Fluctuation Analysis](http://www.physionet.org/physiotools/dfa/),[Multiscale
 Entropy Analysis](http://www.physionet.org/physiotools/mse/tutorial/), and[Information-Based Similarity](http://www.physionet.org/physiotools/ibs/), among others).

HRV 已经广泛应用于基础与临床研究。然而，它的临床应用目前非常有限。这些限制归因于 对不同的不具有可比性的受试者的子集 研究方法和应用缺少标准化，还由于年龄，性别，药物，健康状况，时间生物学差异等等 的混合影响。特别地，在老年受试者中，一些测量指标的不合逻辑的高值可能是由于 细微的心房异位，游走性心房起搏，或窦房结传导异常 导致的“不稳定室上性节律”的影响。关于心率动力学和分析技术的其他信息，包括非线性和基于复杂性的度量，可以在HRV 2006 课程笔记
 和Physionet 的其他地方找到（见示例：去趋势波动分析，多尺度熵分析 和 信息为基础的相似性，等等）。



PhysioNet's HRV Toolkit, available here, is a rigorously validated package of open source software for HRV analysis, including visualization of NN interval time series, automated outlier removal, and
 calculation of the basic time- and frequency-domain HRV statistics widely used in the literature, including all of those listed in the tables below.

Several other high-quality, freely available HRV toolkits may also be of interest to researchers; links to them are provided at the end of this page.

这里提供的 PhysioNet的心率变异性工具包，它是一个经严格验证的HRV分析开源软件包，包括NN间期时间系列的可视化，自动去除异常值，计算文献中广泛使用的基本的时域和频域 HRV统计指标，包括下表中列出的所有指标。其他几个高品质的，免费提供的心率变异性工具包也可能对研究人员们感兴趣；在本页结束位置提供了它们的链接。



**Table 1:Commonly used time-domain measures**       表1：常用时域指标
|AVNN*|Average of all NN intervals         ---------AVNN，所有NN间期的平均值|
|----|----|
|SDNN*|Standard deviation of all NN intervals     -----------所有NN间期的标准差|
|SDANN|Standard deviation of the averages of NN intervals in all 5-minute segments of a 24-hour recording  -------24小时记录的所有5分钟片段中NN间期平均值的标准差|
|SDNNIDX|Mean of the standard deviations of NN intervals in all 5-minute segments of a 24-hour recording  ----------一个24小时记录的所有5分钟片段中NN间期标准差的平均值|
|rMSSD*|Square root of the mean of the squares of differences between adjacent NN intervals  ------------相邻NN间期差值序列平方后平均 再开方|
|pNN50*|Percentage of differences between adjacent NN intervals that are greater than 50 ms; a member of the larger pNNx family [6]           ---------相邻NN间期差别超过50ms的百分比；庞大的pNNx指标家族的一员。|
* Short-term HRV statistics   ----------带星号的指标 表示短程HRV统计指标







**[Table 2:](/) Commonly used frequency-domain measures**  -------表2：常用频域指标

|TOTPWR*|Total spectral power of all NN intervals up to 0.04 Hz      --------所有NN间期的直到0.04 Hz的总的谱能量|
|----|----|
|ULF|Total spectral power of all NN intervals up to 0.003 Hz --------所有NN间期的直到0.003 Hz的总的谱能量|
|VLF*|Total spectral power of all NN intervals between 0.003 and 0.04 Hz   --------所有NN间期的0.003—0.04 Hz的总的谱能量|
|LF*|Total spectral power of all NN intervals between 0.04 and 0.15 Hz.  --------所有NN间期的0.04—0.15 Hz 的总的谱能量|
|HF*|Total spectral power of all NN intervals between 0.15 and 0.4 Hz   --------所有NN间期的0.15—0.4 Hz的总的谱能量|
|LF/HF*|Ratio of low to high frequency power    ---------低频和高频能量的比|


* Short-term HRV statistics (VLF = spectral power between 0 and 0.04 Hz.)   --------带星号的指标 表示短程HRV统计指标（VLF = 0—0.04Hz 的谱能量）



### Selected References:        ---------选择参考：

[1] Wolf MM, Varigos GA, Hunt D, Sloman JG. Sinus arrhythmia in acute myocardial infarction. Med J Aust 1978;2:52-53.


[2] Kleiger RE, Miller JP, Bigger JT, Moss AJ, and the Multicenter Post-Infarction Research Group. Decreased heart rate variability and its association with increased mortality after acute myocardial infarction. Am J Cardiol 1987;59:256-262.


[3] Task Force of the European Society of Cardiology and the North American Society of Pacing and Electrophysiology. Heart rate variability: Standards of measurement, physiological interpretation, and clinical use. Circulation 1996;
 93:1043-1065. 

[4] Mietus JE. [Time domain measures: from variance to pNNx](http://www.physionet.org/events/hrv-2006/mietus-1.pdf). http://physionet.org/events/hrv-2006/mietus-1.pdf


[5] Parati G, Mancia G, Di Rienzo M, Castiglioni P, Taylor JA, Studinger P. Point-Counterpoint: Cardiovascular variability is/is not an index of autonomic control of circulation. J Appl Physiol 2006; 101: 676 - 682.


[6] Mietus JE, Peng C-K, Henry I, Goldsmith RL, Goldberger AL. The pNNx-files: Reexamining a widely-used heart rate variability measure. Heart 2002;88:378-380.


[7] Press WH, Teukolsky SA, Vetterling WT, Flannery BP. Numerical Recipes in C: The Art of Scientific Computing, 2nd ed. Cambridge Univ. Press, 1992, pp. 575-584.


[8] Moody GB. [Spectral analysis of heart rate without resampling](http://www.physionet.org/physiotools/lomb/lomb.html). Computers in Cardiology 1993; 715-718.


[9] Stein PK, Yanez D, Domitrovich PP, Gottdiener J, Chaves P, Kronmal R, Rautaharju P. Heart rate variability is confounded by the presence of erratic sinus rhythm. Computers in Cardiology 2002; 669-72.




## II. Obtaining the HRV Toolkit  -------------获得HRV工具包

### The HRV Toolkit: Contents         -----------HRV工具包： 组成

The HRV Toolkit consists of a [Usages](http://www.physionet.org/tutorials/hrv-toolkit/HRV.src/Usages) file, a [Makefile](http://www.physionet.org/tutorials/hrv-toolkit/HRV.src/Makefile), and the following programs: 


HRV工具包包含一个Usages 文件，一个Makefile，和下面的程序：  
|[plt_rrs](http://www.physionet.org/tutorials/hrv-toolkit/HRV.src/plt_rrs)|a shell script for plotting RR/NN interval series  --------绘制RR/NN间期序列的shell脚本|
|----|----|
|[get_hrv](http://www.physionet.org/tutorials/hrv-toolkit/HRV.src/get_hrv)|a shell script for calculating time and frequency domain HRV statistics  -----计算时域和频域统计量的shell脚本|
|*Scripts above use the programs below, and others from the [WFDB](http://www.physionet.org/physiotools/wfdb.shtml) and[plt](http://www.physionet.org/physiotools/plt) packages**上面的脚本使用了下面的程序，及WFDB 和plt 软件包的其他程序*| |
|[rrlist.c](http://www.physionet.org/tutorials/hrv-toolkit/HRV.src/rrlist.c)|C code for extracting an RR interval list from an annotation file   ------从注释文件提取RR间期列表的C代码|
|[filt.c](http://www.physionet.org/tutorials/hrv-toolkit/HRV.src/filt.c)|C code for filtering RR/NN intervals     ---------过滤RR/NN间期的C代码|
|[filtnn.c](http://www.physionet.org/tutorials/hrv-toolkit/HRV.src/filtnn.c)|C code for filtering RR/NN intervals       过滤RR/NN间期的C代码|
|[statnn.c](http://www.physionet.org/tutorials/hrv-toolkit/HRV.src/statnn.c)|C code for calculating time domain statistics     --------计算时域统计量的C代码|
|[pwr.c](http://www.physionet.org/tutorials/hrv-toolkit/HRV.src/pwr.c)|C code for calculating power in up to 10 frequency bands from a power spectrum ---计算10个频段的功率|
|[seconds.c](http://www.physionet.org/tutorials/hrv-toolkit/HRV.src/seconds.c)|C code for converting hh:mm:ss to seconds  ----------将hh:mm:ss 表示的时间表示成秒数|
|[hours.c](http://www.physionet.org/tutorials/hrv-toolkit/HRV.src/hours.c)|C code for converting seconds to hh:mm:ss    --------将秒表示的时间转换成hh:mm:ss 格式|

plt_rrs and get_hrv are the main scripts used in calculating HRV as illustrated below. These two scripts call the various C programs to accomplish their calculations.
 The C programs can be used separately and their usage and various options can be found in
[Usages](http://www.physionet.org/tutorials/hrv-toolkit/Usages), or by running any of these programs with the
-h option. 

plt_rrs 和get_hrv 是主要的脚本向下面演示的那样计算HRV指标。这两个脚本调用各种C程序完成计算。C程序可以单独使用，别且它们的用法和各种选项可以在Usages文件中找到，或者通过运行这些程序，带有-h选项查看帮助信息。


### Downloading and Installing the HRV Toolkit      -------下载和安装HRV工具包





Installing the HRV toolkit is easy:   -------安装HRV工具包是容易的： 
- On Windows, install the free Cygwin software first; see our [tutorial](http://www.physionet.org/physiotools/cygwin/) for details. Cygwin includes the gcc C compiler and all other utilities needed to build and run the components of the HRV Toolkit on Windows. Start a Cygwin (terminal emulator) window and use it
 for all of the remaining steps. -------在Windows上，首先安装免费Cygwin软件；详见我们的教程。Cygwin包含gcc C编译器和Windows上编译和运行HRV工具包组件 所需的全部其他工具。

- Install the free [WFDB](http://www.physionet.org/physiotools/wfdb.shtml) and
[plt](http://www.physionet.org/physiotools/plt/) software packages.  ------安装WFDB和plt 软件包。

- The HRV toolkit is available as a [tarball of sources](http://www.physionet.org/tutorials/hrv-toolkit/HRV.src.tar.gz) (for all platforms) or as tarballs of prebuilt binaries for
[GNU/Linux (x86)](http://www.physionet.org/tutorials/hrv-toolkit/HRV-i386-linux.tar.gz),
[Mac OS X (x86)](http://www.physionet.org/tutorials/hrv-toolkit/HRV-i386-Darwin.tar.gz),
[Solaris (SPARC)](http://www.physionet.org/tutorials/hrv-toolkit/HRV-sol-8.tar.gz), or
[Windows/Cygwin](http://www.physionet.org/tutorials/hrv-toolkit/HRV-i686-CYGWIN_NT-5.1.tar.gz). Download the version of your choice.    ------HRV工具包提供的方式 有源代码的压缩包（对所有平台） 或预编译二进制压缩包，用于 GNU / Linux（X86），Mac OS X（X86），Solaris（SPARC），或Windows / Cygwin。下载你选择的版本。

- Unpack the HRV toolkit tarball you downloaded. (See the PhysioNet FAQ for information on
[unpacking tarballs](http://www.physionet.org/faq.shtml#tar-gz).)  -----------解压你下载的HRV 压缩工具包（见PhysioNet 常问问题回答中关于解压缩的信息）。

- If you downloaded the sources, enter the source directory (HRV.src) and compile and install the toolkit by typing:     ---------如果你下载了源代码，进入源代码目录（HRV.src) ，输入make install 编译和安装：
    make install

If you downloaded the binaries, move the contents of the HRV directory into some directory in your
PATH (or add the HRV directory to your
PATH). The binaries require the same additional packages as the source distribution (WFDB,
plt, and (on Windows) Cygwin).  --------如果你下载了二进制包，将HRV目录中的内容移动到PATH中的某个目录（或将HRV目录添加到PATH中）。二进制包同样需要其他的 以源码形式发布的软件包（WFDB，plt，Windows上的Cygwin）。

## III. Using the HRV toolkit -----------使用HRV软件包

### User interface         ----------用户界面





The HRV toolkit does not include a graphical user interface. Its components are command-line tools that must be run from a terminal window (under MS-Windows, a Cygwin window) or by a shell script. (Even
plt_rrs must be started from the command line or a script, although it produces graphical output.)


HRV工具包不包含图形用户界面。它的组成部分是命令行工具，必须从终端窗口来运行（在MS-Windows下，用Cygwin窗口） 或通过shell脚本。（即使plt_rrs 必须从命令行或脚本启动，虽然它产生图形输出。）

### Input data format       ---------输入数据格式

Both plt_rrs (for plotting the RR/NN interval series) and
get_hrv (for calculating the HRV statistics) can take as input either a
[PhysioBank-compatible beat annotation file](http://www.physionet.org/faq.shtml#annotations) or a text file containing an RR interval list. RR interval lists can be in any of four formats:   -----------plt_rrs(用来绘制RR/NN间期序列）和get_hrv（用来计算HRV统计指标）能够支持两种输入方式，或者是PhysioBank
 兼容心拍注释文件 或者是包含RR间期列表的text 文件。RR间期列表可以用下面四种格式中的任何一个： 
- 3 columns (T, RR, A)    -------三列 
- 2 columns (RR, A)   ----------两列 
- 2 columns (T, RR)  ---------两列 
- 1 column (RR)   -----------一列





where T is the time of occurrence of the beginning of the RR interval, RR is the duration of the RR interval, and A is a beat label. Normal sinus beats are labeled
N.   -------其中T表示该RR间期开始出现的时间（ ----     第一个QRS时间），RR表示RR间期时限，A表示心拍标识。正常窦性心拍标记为N。



Although T is assumed to be expressed in seconds by default, the -I option of
plt_rrs and get_hrv (see below) can be used if the RR interval list contains T values expressed as clock time (hh:mm:ss.xxx), hours (hh.xxxxxxx) or minutes (mm.xxxxx).
 Similarly, although RR is assumed to be expressed in seconds, intervals in milliseconds can be input by using the `-m' option. (See details below or in
[Usages](http://www.physionet.org/tutorials/hrv-toolkit/Usages)). 

虽然T默认情况下用秒来表示，如果RR间期列表包含的T值是钟表时间表示形式hh:mm:ss.xxx，小时表示形式hh.xxxxxxx，或者分钟表示形式mm.xxxxx，运行plt_rrs 和get_hrv 时可使用选项 -I 。类似地，虽然RR设定为用秒表示，但是毫秒表示的间期也可以输入，通过使用-m 选项。（见下面详细介绍或参考Usages文件）。




Beat annotation files are available for most of the PhysioBank records that include ECGs. For information about record and annotation conventions see the
[PhysioNet FAQ](http://www.physionet.org/faq.shtml#file-types). If you wish to study a recording for which no beat annotation file or RR interval listing is available, you may be able to
[create an annotation file](http://www.physionet.org/faq.shtml#creating-annotations) using software from the
[WFDB](http://www.physionet.org/physiotools/wfdb.shtml) software package. Additional information on RR intervals, heart rate, and HRV can be found at the
[RR/HR/HRV Howto](http://www.physionet.org/tutorials/hrv/). 

PhysioBank 包含ECG的大多数记录都带有心拍注释文件。关于记录和注释转换的信息见PhysioNet 常问问题回答。如果你想研究一个记录，记录中没带心拍注释文件或RR间期列表，你可以利用WFDB软件包中的软件生成一个注释文件。关于RR间期，心率，HRV 的其他信息可以在题为 [RR/HR/HRV Howto](http://www.physionet.org/tutorials/hrv/) 的网页中找到。


#### Input used in this tutorial  ---------本教程中使用的输入        





The examples below read the ecg annotations from record
chf03 of the [BIDMC Congestive Heart Failure Database](http://www.physionet.org/physiobank/chfdb/). To reproduce the results shown below, it is not necessary to download this annotation file, because applications that use the WFDB library (including those in the HRV Toolkit that read annotation files) can locate
 and read the copy from the PhysioNet web server if no local copy exists. In order to do so successfully, it is necessary to specify the path to the record from the PhysioBank archive directory within the record name, as shown in these examples (use
chfdb/chf03 rather than simply chf03). Examples that illustrate input from RR interval lists assume that the input file is named
chf03.rr, and that it is in the current (local) directory.


 下面的例子使用的是 BIDMIC 充血性心衰数据库的记录chf03的ecg注释。要重复生成下面显示的结果，没有必要下载这个注释文件，因为使用WFDB库的应用程序（包括HRV工具包中那些读取注释文件的程序），如果不存在本地的副本，能够定位和读取PhysioNet网络服务器上的副本。要达到该目的，如这些例子中所演示的那样，必须在记录名中指定该记录在PhysioBank存档目录中的路径（使用chf/chf03,而不简单的用chf03)。那些演示以RR间期列表作为输入的例子  假定输入文件名是chf03.rr,
 并且在当前（本地）目录中。



接下来是：

plt_rrs: plotting the RR/NN interval time series

get_hrv: calculating the HRV statistics

这两个脚本的用法。

## IV. Representative measurements of HRV   ---------HRV典型值

Table 3 gives representative values of HRV measurements, obtained using the HRV toolkit from the 72 subjects in the
[MIT-BIH Normal Sinus Rhythm Database](http://www.physionet.org/physiobank/database/nsrdb/) and the
[Normal Sinus Rhythm RR Interval Database](http://www.physionet.org/physiobank/database/nsr2db/). These representative values (comparable to those previously reported in healthy subjects) are not, however, intended as a standard normative database.


表3 给出了HRV测量典型值，从MIT-BIH 正常窦性节律数据库和正常窦性节律RR间期数据库72个对象 利用HRV工具包 计算出来。这些典型值与 先前对健康受试者的 研究报告相近，但是，没有打算将它们作为标准规范性的数据库。


**Table 3: HRV in 24-hour NN interval time series from 72 ostensibly healthy subjects**

(35 males, 37 females, ages 20-76 years, mean age 55)

|Measurement|Mean ± SD|
|----|----|
|AVNN (msec)|787.7 ± 79.2|
|SDNN (msec)|136.5 ± 33.4|
|SDANN (msec)|127.2 ± 35.7|
|SDNNIDX (msec)|51.2 ± 14.2|
|rMSSD (msec)|27.9 ± 12.3|
|pNN20 (%)|34.2 ± 13.7|
|pNN50 (%)|7.5 ± 7.6|
|TOTPWR (msec2)|21490 ± 11577|
|ULF PWR (msec2)|18128 ± 10109|
|VLF PWR (msec2)|1900 ± 1056|
|LF PWR (msec2)|961 ± 722|
|HF PWR (msec2)|501 ± 847|
|LF/HF ratio|2.7 ± 1.3|







## Links to other HRV toolkits   ------------------其他HRV工具包链接

Several other free HRV toolkits exist. Among them are:   --------存在其他几个免费HRV工具包。其中包括：

- KUBIOS-HRV ([http://kubios.uku.fi/](http://kubios.uku.fi/))          --------软件名

Authors: JP Niskanen and colleagues at Kuopio University, Finland  -------作者，芬兰


KUBIOS-HRV 2.0 is available for Windows and Linux. It is a standalone Matlab application (only the Matlab runtime compiler and libraries, both included in the package, are needed in order to use it). KUBIOS-HRV includes a well-written 53-page user guide
 with detailed descriptions of all of the algorithms used, mathematical derivations where needed, and an extensive bibliography.   ------------------KUBIOS-HRV 2.0 可在Windows和Linux上使用。它是独立的Matlab应用程序（仅需要Matlab 运行编译器和库，它们都包含在软件包中）。KUBIOS-HRV 包含写得很好的53页的用户指南，详细描述所用的全部算法，必要的数学推导，广泛的参考书。

- 
The package includes automated and interactive methods for outlier identification and removal; a smoothness-based detrending algorithm for preprocessing time series; derivation of SDNN, SDSD, RMSSD, pNN50, HRV triangular index, and TINN; FFT and AR power
 spectrum estimates, VLF, LF, HF, and LF/HF ratio; and Poincaré plots, ApEn, SampEn, DFA, correlation dimension, and recurrence plots. Inputs are RR interval lists in text format.  ------------软件包包含自动的和交互的识别和剔除异常值的方法；处理时间序列以平滑化为基础的去趋势算法；SDNN ，SDSD, RMSSD, pNN50,
 HRV 三角指数，TINN  的推导；FFT 和 AR功率谱估计，VLF, LF,HF,LF/HF比；庞加莱散点图，近似商，样本熵，去趋势波动分析，相关维数，递归图。输入是文本格式的RR间期列表。

- 
KUBIOS-HRV 2.0 and its user guide were released in 2002, and it was described in detail in: ----文献


> Juha-Pekka Niskanen, Mika P Tarvainen, Perttu O Ranta-aho, Pasi A Karjalainen, "[Software for advanced HRV analysis](http://it.uku.fi/biosignal/pdf/HRVdeprep.pdf)." Computer Methods and Programs in Biomedicine
**76**(1):73-81 (2004 Oct; DOI: 10.1016/j.cmpb.2004.03.004) 

- R-HRV ([http://cran.r-project.org/web/packages/RHRV/](http://cran.r-project.org/web/packages/RHRV/))    ------软件名

Authors: M Lado, A Mendez, L Rodriguez-Linares, X Vila (Univ. of Vigo, Spain) ------西班牙


R-HRV is freely available in source form (for all platforms) and as prebuilt binaries for Mac OS X and Windows, together with a 13-page user guide. It requires R (a very widely used open-source statistics package). R-HRV includes functions for importing
 PhysioBank (WFDB) annotation files, automated outlier handling, and frequency domain HRV analysis.   -------R-HRV  免费提供适用于所有平台的源代码和适用于Mac OS X 和Windows 的预编译二进制包，附带有13页的用户指南。它需要R软件包（一个使用非常广泛的开源统计软件包）。R-HRV 包含的功能有 导入PhysioBank（WFDB)注释文件，自动处理异常值，频域HRV分析。

- 
R-HRV was described in:        ------------文献描述 

> L. Rodriguez-Linares, X. Vila, A. Mendez, M. Lado, D. Olivieri, "[RHRV: An R-based software package for heart rate variability analysis of ECG recordings](http://trevinca.ei.uvigo.es/~anton/publications/cisti2008.pdf)."
 3rd Iberian Conference in Systems and Information Technologies (CISTI 2008), 21-23 June 2008.


- Software for Heart Rate Variability ([http://www.macalester.edu/~kaplan/hrv/doc/](http://www.macalester.edu/~kaplan/hrv/doc/))   ---HRV软件

Authors: Danny Kaplan and Phil Staffin (Macalester College)     ---作者，麦卡利斯特学院 


This software, written as Matlab m-code, includes routines for outlier handling, SDNN, RMSSD, a generalization of pNN50, spectral analysis using FFT (VLF, LF, HF, LF/HF), ApEn, and DFA. Inputs are RR interval lists in text format.















 这个软件 ，用Matlab m-code 编写，包含的程序有 异常值处理，SDNN, RMSSD, pNN50 的推广，使用FFT的谱分析（VLF,LF,HF,LF/HF), 近似熵，去趋势波动分析。输入是文本格式的RR间期列表。](https://so.csdn.net/so/search/s.do?q=matlab&t=blog)](https://so.csdn.net/so/search/s.do?q=statistics&t=blog)](https://so.csdn.net/so/search/s.do?q=intervals&t=blog)](https://so.csdn.net/so/search/s.do?q=basic&t=blog)](https://so.csdn.net/so/search/s.do?q=domain&t=blog)




