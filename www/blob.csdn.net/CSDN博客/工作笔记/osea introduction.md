# osea/ introduction - 工作笔记 - CSDN博客





2012年01月17日 14:15:54[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5411









1.0 Introduction

Computer analysis of ECG signals is common today. 

ECG信号的计算机分析现在已经很常见。



Devices on the market that analyze ECGs, such as patient monitors, stress test systems, and Holter analysis systems, do a good job of detecting beats and classifying arrhythmias.


市场上分析ECG信号的仪器，例如 病人监护仪，压力测试系统，Holter 分析系统，检测心拍和分类心律失常的工作都做的非常好。



However, new companies are constantly emerging and applying new technologies, such as PDAs, in an effort to make smaller and cheaper ECG analysis systems.


然而，新的公司不断涌现 并 应用新的技术，例如掌上电脑，努力研制 更小更便宜的ECG分析系统。



Other companies are using ECG gating to improve imaging of the heart. 

其它一些公司 利用 ECG门控技术  改善心脏 成像。



Each new company that produces a product that requires ECG analysis must implement their own ECG analysis software, duplicating the efforts of every other company.


新公司的产品如果需要 ECG 分析功能， 他们必须实现自己的ECG分析软件，重复其它公司的工作。



Similarly, researchers who need ECG analysis to explore new diagnostic methods, such as the analysis of heart rate variability, must also implement their own versions of basic ECG analysis functions.


类似的，开发新的诊断方法的研究人员 也需要ECG分析，例如心率变异性的分析，也必须实现自己的基本的ECG分析功能。



Thirty years of research on computer analysis of ECG signals has produced a great many methods for detecting beats and classifying ECG signals, but there is still a significant effort required to go from theory to implementation.

心电信号计算机分析的研究 已有30年，已经产生很多检测心拍和分类ECG 信号的方法，但是从理论到实现仍需要非常大的努力。



In an effort to reduce this industry and research wide duplication of effort, E.P. Limited, with support from the National Heart Lung and Blood Institute (NHLBI) of the National Institutes of Health (NIH), has been developing and is now releasing open source
 software for ECG analysis. 

为减少 企业和研究者的 重复工作，E.P.Limited 公司， 在 国家卫生研究院 下设的 国家心肺和血液研究所的 资助下，开发并发布了开源ECG分析软件。



We have developed C functions that implement the most basic ECG analysis operations, detection and classification of individual beats.


我们已经开发了 实现最基本的ECG分析操作的 C 程序，检测和分类单个的心拍。



By building on this open source software we hope that new companies will be able to bring reliable systems to market more quickly, and researchers will be able to spend more time exploring new diagnostic techniques rather than implementing beat detectors.

通过建立这个开源软件 我们希望新的公司能够更快地向市场投放可靠系统，研究人员能够花费更多时间开发新诊断技术而不是去实现心拍检测。



This is our fifth software release and includes C implementations of three variations of our beat detector and a C implementation of a beat classifier.


这是我们第五次发行软件，包含三个 C 语言实现的心拍检测器和一个C 语言实现的心拍分类器。



The beat classifier and two of the beat detector implementations can easily be adapted for different sample rates.


心拍分类器和两个心拍检测器能容易的修改以适应不同的采样率。



One beat detector has been written for implementation on processors with limited available memory. Finally, we have included some programs to facilitate the testing of beat detection and beat classification software.

一个心拍检测器可用在内存空间有限的处理器上。 最后，包含一些用于测试心拍检测和分类的程序。



In releasing this software as open source code, we hope that others will make improvements on it and make those improvements available. To be realistic, we do not expect that companies will make an effort to release modifications that they make to this software.

发布这个开源软件，我们希望其他人能改进它并将改进结果供大家使用。事实上，我们没期望公司能发布他们对这个软件的改进结果。



 On the other hand, we believe that this software presents significant opportunities for students and others in an academic environment to build on our work and make the results of their work available as open source code. With this in mind, we would like
 to suggest areas where our algorithms might be improved or extended. These areas represent opportunities for course projects, undergraduate theses, or Masters theses.

另一方面，我们相信会有学生和在学术环境中的其它人愿意在我们工作的基础上改进这个软件，并将自己的工作结果开源。基于这种想法，我们愿意指出我们的算法可以改进或扩展的一些地方。这些地方可作为课程项目，本科论文或硕士论文。



Some possible improvements or extensions of the beat detection algorithm are:

心拍检测算法可能的改进或扩展是：



• The QRS detector only uses a single channel of information (on the MIT/BIH database the second channel is often too noisy to be of much help), can it be made more robust if it uses two or more ECG channels as input?

  QRS 检测器仅利用单通道信息（MIT/BIH 数据库中第二个通道常常是 因太大噪声而没太大帮助），如果使用两个或多个ECG通道作为输入能够使软件更加健壮么？



• The QRS detector has not been tested in noisier environments. Can performance be improved with noisier signals (as might occur during exercise or stress testing) by making stronger assumptions about rhythm?

QRS 检测器没有在噪声更大的环境中测试过。通过对节律作更强的假设能够在信号干扰更强时改进QRS检测器性能么 ？ 



• As is, the QRS detector has a variable, sometimes long, delay. How well can a QRS detector perform if the delay is restricted to be 0.25 s or less?

QRS检测器有可变的、有时候很长的延时。如果延时被限制为不超过0.25 秒，QRS检测器的性能表现如何？



• The QRS detector filters were designed to be efficient, requiring only shift and add operations rather than multiplications of any precision. If processing power is not a significant problem, are there better filters that use less restricted filter coefficients?


QRS检测滤波以高效为目标进行的设计，仅需要移位和加法运算，不需要乘法运算。如果处理能力不是大问题，有没有更好的不那么限制滤波器系数的滤波器？



Though the code for the QRS detection software has been developed and improved over the course of roughly 15 years, the beat classification software has only been under serious development for a little more than one year.


虽然QRS 检测软件的代码开发和改进大约有15年的历程，心拍分类软件连续开发时间仅一年多。



The performance of the classification software is as good as or better than the performance reported for other algorithms on the same data [1-5], but it is still “tuned” to the data from the MIT/BIH and AHA databases.

分类软件与其它算法在相同测试数据上的性能表现一样好甚至会更好些，但它仍然是按MIT/BIH和AHA 数据来优化的。  （--------训练集和 测试集一样，这是局限性）



 The following are some areas where the algorithm could be improved:

下面是心拍分类算法可以被改进的一些地方：



• Significant improvements could be made in the software’s ability to recognize and compensate for noise.

 软件识别和补偿噪声的能力可以做很大的改进



• The algorithm only uses a limited set of features to recognize whether or not a beat is ventricular and completely ignores the T-wave region of the beat. A more complete feature set might allow more accurate classification of ventricular beats.

算法仅使用有限的特征来识别一个心拍是否是室性的而完全忽略T波区域。更加完整的特征集可能会更准确的分类室性心拍。



• The algorithm only uses information from a single channel of ECG data.

算法仅使用单通道ECG 数据的信息。



• The algorithm has only been tested on half hour records. Longer records will tend to use up the available beat templates with minor beat variations and noisy beats. We do not know how this will affect performance on a long record.

算法仅用半小时记录进行过测试。分析更长的带有小的心拍变异和噪声心拍的记录将趋向于用尽可用的心拍模板。我们不知道这将如何影响在长记录中的性能表现。



• The algorithm does not recognize Ventricular Fibrillation or Flutter (as on 207)

算法不识别室颤和室扑（207 记录中存在）



Further development of this software requires that the developer continually test the software against as large a database of ECG data as possible. It is not of much use to improve the performance on one record if it requires sacrificing performance on all
 the others.

这个软件的进一步开发需要开发者连续的用尽可能大的ECG数据测试这个软件。如果需要牺牲所有其他记录上的性能来提高某个记录上的性能是没有多大用处的。



 Luckily, it has never been easier or less expensive to test ECG analysis software on MIT/BIH data and data from supplemental databases. ECG data and open source software for accessing this data are available on line at
[www.physionet.org](http://www.physionet.org/). 

真幸运，从来没有比  用MIT/BIH 数据和补充数据库中数据 测试ECG分析软件 更容易和更便宜的事。ECG数据和访问该数据的开源软件可在[www.physionet.org](http://www.physionet.org/) 网站获取。



In this release, we have provided two example programs (easytest and bxb) to facilitate testing beat detection and classification software with MIT/BIH formatted data.

 这个发行版中，我们提供了两个例程（easytest 和 bxb）便于用 MIT/BIH 格式数据测试心拍检测和分类软件。



The following documentation provides an overview of the software included in this release and documents the general theory of operation for the various software components. If anything seems unclear you can always refer directly to the source code. We would
 appreciate any improvements to, comments about, corrections to, or suggestions regarding this documentation or software. We will make an effort to test and incorporate all future improvements in the distribution software.

 接下来文档提供本发行版本中软件概述和各软件组成部分的一般操作理论。如果哪里看起来不清晰可以直接参考源代码。我们欢迎对这个文档或软件所做的改进，评价，更正，建议。我们将努力去测试并在发布的软件中将所有的改进包含进来。




