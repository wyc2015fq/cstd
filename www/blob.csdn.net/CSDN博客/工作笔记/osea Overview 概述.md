# osea/ Overview 概述 - 工作笔记 - CSDN博客





2012年01月17日 14:16:52[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5881








2.0 Software Overview

软件概述



Our software implements the basic ECG analysis functions of beat detection and classification as C functions.

我们的软件用C语言实现基本的ECG分析功能------心拍检测和分类。



 This release includes three versions of our beat detector. Two are general-purpose beat detectors, where one represents a more efficient version with slightly different performance characteristics. The third beat detector is more computationally efficient,
 uses very little memory, and is embedded in a program that performs beat detection on a PIC processor. The beat detector functions are independent of the beat classification functions and may be used alone in applications that do not require beat classification.

 这个版本包含三个心拍检测器。有两个是通用心拍检测器，其中一个更加高效仅在性能特征上稍有不同。第三个心拍检测器计算上更加高效，占用很少的内存，嵌入 在PIC处理器上完成心拍检测 的程序上。心拍检测功能独立于心拍分类功能，可以单独使用。



2.1 File Listing

文件列表



All the files required for detecting and classifying beats and this documentation should be included in the file osea121.zip (Open Source ECG Analysis 1.21).

检测和分类心拍 需要所有的文件，这个文档包含在osea121.zip中（开源ECG分析1.21）。



The code is written in C and the files are .cpp files so that our complier (Turbo C++ V 4.5) treats them as C++ files (the only C++ feature we actually use is the // comment).

代码是用C语言编写的 .cpp 文件， 于是我们用的编译器（Turbo C++ V 4.5）将它们当做C++文件来处理（我们实际用到的C++特性仅仅是// 注释语法）。



We wrote the Picqrs.c to be compiled with the CCS C compiler for PIC processors. The following is a list of the files that make up the open source ECG analysis software release:

我们编写了Picqrs.c ，用CCS（code composer studio）C 编译器编译 并在PIC 处理器上应用。下面是构成 开源ECG分析软件的 文件列表：



QRS Detection Files:

QRS 检测文件：



Qrsdet.cpp Original QRS detection functions.    --------------------------------原始的QRS检测功能

Qrsdet2.cpp Simplified and improved QRS detector functions. -------------简化并改进的QRS检测功能

Qrsfilt.cpp Filter functions for QRS detection.   -------------------------------- QRS检测滤波

Qrsdet.h General parameters definitions associated with QRS detection.   -----------QRS检测相关的通用参数定义

Picqrs.c A PIC based QRS detector.                      ------------------------------------------ 基于PIC 的QRS检测器 


Beat Classification Functions:    

心拍分类功能：



Analbeat.cpp Functions for extracting beat features. ------------------------------------提取心拍特征

Analbeat.h Function prototypes associated with analbeat.cpp.    ----------------------analbeat.cpp 中函数原型

Bdac.cpp API function for beat detection and classification.  ---------------------------心拍检测和分类的API函数

Bdac.h General parameters associated with the beat classification software. -------心拍分类软件 通用参数

Classify.cpp Functions for classifying beats.       ------------------------------------------用于分类心拍 的函数

Match.cpp Functions for matching beats to previously detected beat types.  --------与先前检测到的心拍类型匹配

Match.h Function prototypes associated with match.cpp.      -----------------------------match.cpp 中函数原型

Noisechk.cpp Functions for detecting low frequency noise.  -----------------------------检测低频噪声

Postclas.cpp Functions associated with post classification.  ------------------------------ 延后分类功能

Rythmchk.cpp Functions for classifying R-to-R intervals.  ---------------------------------分类RR间期

Rythmchk.h Function prototypes associated with rythmchk.h.  ----------------------------rythmchk.cpp 中函数原型


Test programs:

测试程序：



Easytest.cpp Example program that creates MIT/BIH format annotation files from ECG data using the beat detection and classification software.

Easytest.cpp ----------利用心拍检测和分类软件生成 MIT/BIH格式的注释文件。



Bxb.cpp Example program that compares annotation files to evaluate beat detection and classification performance.

Bxb.cpp --------------比较注释文件 来评估心拍检测和分类性能



Documentation:

Osea3.pdf This document.   ------------------文档， 本文档Osea3.pdf。



Database I/O:            ----------------------数据库输入输出：

Wfdb.lib Library of database interface functions needed for easytest.cpp and bxb.cpp. ------ 数据库接口，easytest.cpp和bxb.cpp 用到。



Wfdb.h Header file for functions and structures in wfdb.lib   ---------- wfdb.lib 中函数和结构体声明

Ecgcodes.h ECG classification code definitions.   ------------------------ECG分类编码定义



2.2 File Dependencies

文件依赖关系



Fig. 1 illustrates the file dependencies for the beat detection and analysis files.

图1 演示了 心拍检测和分析文件的依赖关系



BDAC contains the function BeatDetectAndClassify, which serves as the interface between input data and beat detection and classification.

BDAC 中 BeatDetectAndClassify 函数  是输入数据和心拍检测和分类程序 接口。



BeatDetectAndClassify takes sample-by-sample data as input and passes it to the QRS detection functions in QRSDET or QRSDET2.

BeatDetectAndClassify 逐点的接收数据并 将其传给 QRS检测函数 QRSDET 或 QRSDET2。



 The QRS detection function uses filter functions in QRSFILT. 

QRS检测函数 使用了 QRSFILT 中的滤波函数。



When a beat is detected, BeatDetectAndClassify waits until the end of the beat, checks for low-frequency noise, and passes the beat, R-to-R interval, and noise level to the function Classify in the file CLASSIFY.

但一个心拍被检到，BeatDetectAndClassify  等待 这个心拍的结束， 检查低频噪声，传递这个心拍，RR 间期，噪声水平给CLASSIFY 中的Classify 函数。



Classify decides whether a beat is a normal beat or a ventricular beat. Classify uses functions in MATCH to compare new beats to templates of previously recorded beats.

Classify 决策一个心拍是正常拍 或 室性拍。Classify  利用MATCH 中函数 将新检测到的心拍和先前记录的心搏模板进行比较。



Functions in RYTHMCHK, classify the beats R-to-R interval as either a normal R-to-R interval, a premature R-to-R interval, or an R-to-R interval that represents a compensatory pause.

RYTHMCHK 中的函数，将RR间期分类成 正常RR间期，早搏RR间期，或 联律间期。



Functions in ANALBEAT determine the onset and end point of the QRS complex and the associated isoelectric level.

ANALBEAT 中函数确定QRS复合波的起点和终点，及相应的等电位水平。



Functions in POSTCLAS attempt to more accurately reclassify previous beats. 

POSTCLAS 中函数 试图更加准确地重新分类先前的心拍。



Classify uses this information on whether the beat matches previous beats, whether or not the R-to-R interval appears premature, whether the beat is narrow or wide, and whether this type of beat has previously appeared to be ventricular or normal to classify
 the beat. Classify returns its classification to BDAC which, in turn, returns the classification to the calling program.

Classify 利用一些信息：心拍是否和前一个心拍匹配，RR间期是否表现为早搏，心拍是狭窄或宽大，用来分类的 这种心拍类型先前表现为室性 还是 正常。 Classify 返回分类结果给BDAC，依次地，返回分类结果给调用程序。



<?xml:namespace prefix = v ns = "urn:schemas-microsoft-com:vml" /><?xml:namespace prefix = o ns = "urn:schemas-microsoft-com:office:office" />



依赖关系：

BDAC ----                     

               --------QRSDET or QRSDET2 

                              --------------QRSFILT 

               ---------NOISECHK 

               ---------CLASSIFY 

                              ---------------MATCH 

                              ---------------RYTHMCHK 

                              ----------------ANALBEAT 

                              ----------------POSTCLAS 



Fig. 1 Overview of ECG analysis software file and functional dependencies. 

ECG分析软件文件和功能依赖关系 简图



2.3 Beat Detection and Classification API ------------心拍检测和分类应用程序接口



There are only two functions required for using the beat detection and classification software, BeatDetectAndClassify and ResetBDAC.

使用拍检测和分类函数仅需要俩个函数，BeatDetectAndClassify 和 ResetBDAC。



 The first function provides the interface to the beat detection and classification software and the second allows the static variables required for beat detection and classification to be reset.

第一个函数提供拍检测和分类软件接口，第二个拍检测和分类函数需要的静态变量 重置。



If your application does not require beat classification, the function QRSDet can be used to interface directly to the QRS detection portion of the software.

如果你的应用程序不需要拍分类，QRSDet 函数可直接用来作 软件的QRS检测部分的接口。



The following are function definitions for BeatDetectAndClassify, ResetBDAC, and QRSDet. For details on using BeatDetectAndClassify, refer to easytest.cpp. We have included easytest.cpp in this software distribution as an example program that uses BeatDetectAndClassify
 to generate annotation files from MIT/BIH arrhythmia database records.

 接下来时BeatDetectAndClassify, ResetBDAC, QRSDet 函数定义，使用BeatDetectAndClassify的细节 参考easytest.cpp文件。例程easytest.cpp利用BeatDetectAndClassify 从MIT/BIH心律失常数据库记录 生成注释文件。



2.3.1 BeatDetectAndClassify



Syntax:    -----------语法：

int BeatDetectAndClassify(int ecgSample, int *beatType, int *beatMatch) ;  ------------函数原型

Description:  ---------------说明

BeatDetectAndClassify implements a beat detector and classifier.  ---------功能：实现一个心拍检测器和分类器 

ECG samples are passed into BeatDetectAndClassify one sample at a time with ecgSample. When a beat has been detected and classified the detection delay is returned and the beat classification is returned through the pointer *beatType.

ECG采样值 通过参数ecgSample 一次一个地传进函数BeatDetectAndClassify。当一个心拍已经被检测到并完成分类，返回检测延迟，拍分类结果通过指针变量*beatType 返回。



The index of the template that the beat was matched to is returned through the pointer *beatMatch (used for debugging).

该拍匹配的模板号通过指针变量*beatMatch返回(用于调试）。



The calling function must allocate space for beatType and beatMatch.-------------调用函数必须为beatType 和beatMatch分配空间。



Returns: ----------------返回值

BeatDetectAndClassify returns 0 if no new beat has been detected and classified.

 如果没有新的拍被检测并分类，BeatDetectAndClassify 返回0.

If a beat has been classified, BeatDetectAndClassify returns the number of samples since the approximate location of the R-wave.

如果一个拍被分类，BeatDetectAndClassify 返回 从这个R波大致位置到当前的样本点数量。

If a beat has been classified, *beatType is set to 1 (NORMAL), 5 (PVC), or 13 (UNKNOWN) to indicate the beat classification.

如果一个拍被分类，*beatType 被置为1（正常），5（PVC),13(未知）来表示拍类型。

The index of the previous beat template that the beat most closely matches is returned in *beatMatch.

这个心拍匹配的模板序号 在*beatMatch中返回。



2.3.2 ResetBDAC

Syntax:

void ResetBDAC(void) ; 

Description:


ResetBDAC resets static variables used in beat detection and classification. ---初始化检测和分类程序中使用的静态变量

ResetBDAC should be called before beginning to process a new ECG record with BeatDetectAndClassify.

用函数BeatDetectAndClassify 开始处理一个新的ECG记录之前 应调用ResetBDAC。



2.3.3 QRSDet

Syntax:

int QRSDet(int ecgSample, int init) ;      

Description:         



QRSDet implements a modified version of the QRS detection algorithm described in:

Hamilton, Tompkins, W. J., "Quantitative investigation of QRS detection rules using the MIT/BIH arrhythmia database", IEEE Trans. Biomed. Eng., BME-33, pp. 1158-1165, 1987.  ------------* 

QRSDet 实现了  论文 * 中描述的QRS检测算法，也有所修改。



Consecutive ECG samples are passed to QRSDet in ecgSample. 

 连续的ECG样本被通过参数ecgSample 传给QRSDet。

QRSDet contains a number of static variables that it uses to adapt to different ECG signals.

QRSDet 包含许多静态变量，用来适应不同的ECG信号。

These variables can be reset by passing a non-zero value in init.--------这些变量可通过在参数init中传入非0值被初始化。



Returns:

When a QRS complex is detected QRSDet returns the detection delay.

 当检测到一个QRS复合波时，QRSDet 返回检测延迟。



