
# 人脸识别 | AI产品经理需要了解的CV通识（二） - 喜欢打酱油的老鸟 - CSDN博客


2018年07月29日 08:35:01[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：136标签：[AI																](https://so.csdn.net/so/search/s.do?q=AI&t=blog)[产品经理																](https://so.csdn.net/so/search/s.do?q=产品经理&t=blog)[人脸识别																](https://so.csdn.net/so/search/s.do?q=人脸识别&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=产品经理&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=AI&t=blog)个人分类：[人工智能																](https://blog.csdn.net/weixin_42137700/article/category/7820233)
[
																								](https://so.csdn.net/so/search/s.do?q=AI&t=blog)


*前言：本文作者@jasmine，是我们“AI产品经理大本营”成员，下面是她分享的第4篇文章，欢迎更多有兴趣“主动输出”的朋友们一起加入、共同进步：）*
![0?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/TS56qMjSfEhqlRAqZemxRV5kGdrrXHg0FPX81eIpsBPR9Kusb6Lsq6rrUKy9w3fVDwEsxnCfKeibOicEiaHoCUgicQ/0?wx_fmt=jpeg)
最近整理了CV方向的一些产品基础知识，我上一篇文章《看AI产品经理如何介绍“计算机视觉”（基于实战经验和案例）》算是这个系列的第一篇；本文是本系列下的第二篇，主要针对**人脸识别**进行梳理。后续还会有**多目标跟踪****、OCR**等方向的内容。希望大家能从这个系列收获到更多的CV干货：）
**人脸识别（Face Recognition）****：对图像中的人脸进行****检测、识别和跟踪****。**
1
**技术流程详解**
**技术流程：**
![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/TS56qMjSfEhqlRAqZemxRV5kGdrrXHg0xZD4zx24wGUNfdhMiaNctHFGtRp1ObpmzX0CzhU4iauC9wC6ttNvUDiaw/0?wx_fmt=png)
**1、****人脸采集**
人脸采集主要关注以下因素——
1）**图像大小**：**人脸图像过小会影响识别效果，人脸图像过大会影响识别速度。**非专业人脸识别摄像头常见规定的最小识别人脸像素为**60*60**或**100*100**以上。在规定的图像大小内，算法更容易提升准确率和召回率。图像大小反映在实际应用场景就是**人脸离摄像头的距离**。
2）**图像分辨率**：越低的图像分辨率越难识别。图像大小综合图像分辨率，直接影响摄像头识别距离。现4K摄像头看清人脸的最远距离是**10米，7K**摄像头是**20米**。
3）**光照环境**：过曝或过暗的光照环境都会影响人脸识别效果。可以从摄像头自带的功能补光或滤光平衡光照影响，也可以利用算法模型优化图像光线。
![0?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/TS56qMjSfEhqlRAqZemxRV5kGdrrXHg0ggzvdmn9AzgramIOynYILdoZA7Tk0gCga1YZl9fXe5zLgnhicJIPib7Q/0?wx_fmt=jpeg)
4）**模糊程度**：实际场景主要着力解决**运动模糊**，人脸相对于摄像头的移动经常会产生运动模糊。部分摄像头有抗模糊的功能，而在成本有限的情况下，考虑通过算法模型优化此问题。
![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/TS56qMjSfEhqlRAqZemxRV5kGdrrXHg0kcCjWK21fuvnKeU3HLWEBWzXWG69foFqhRKJkuMETHuXJBkPe8SOHA/0?wx_fmt=png)
5）**遮挡程度**：五官无遮挡、脸部边缘清晰的图像为最佳。而在实际场景中，很多人脸都会被帽子、眼镜、口罩等遮挡物遮挡，**这部分数据需要根据算法要求决定是否留用训练。**
6）**采集角度**：人脸相对于摄像头角度为正脸最佳。但实际场景中往往很难抓拍正脸。因此算法模型需训练包含左右侧人脸、上下侧人脸的数据。工业施工上摄像头安置的角度，需满足**人脸与摄像头构成的角度在算法识别范围内**的要求。
**2、****人脸检测（Face Detection）**
1）**人脸位置检测**
![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/TS56qMjSfEhqlRAqZemxRV5kGdrrXHg0u05xKN0GSYm3z7pAl1wOycOSsydpWYMrd1YIMS8JFmLCRGhr7VaibQg/0?wx_fmt=png)
目的是在图像中准确标定出**人脸的位置和大小**。
**2）****人脸关键点检测****（****人脸对齐）**
![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/TS56qMjSfEhqlRAqZemxRV5kGdrrXHg0wQMuULWDA0JD3JEaOt6CMvzxlObHrBuyzvgxZqvoocalMHH1N1DiaVw/0?wx_fmt=png)
目的是自动估计人脸图片上脸部特征点的坐标
一般可以使用Openface中的Dlib模型实现人脸检测，利用OpenCV库对数据进行处理。最近人脸检测算法模型的流派包括三类及其之间的组合：**viola-jones框架（性能一般速度尚可，适合移动端、嵌入式上使用），dpm（速度较慢），cnn（性能不错）**。
人脸检测关注以下指标：
检测率：识别正确的人脸/图中所有的人脸。检测率越高，代表检测模型效果越好。
误检率：识别错误的人脸/识别出来的人脸。误检率越低，代表检测模型效果越好。
漏检率：未识别出来的人脸/图中所有的人脸。漏检率越低，代表检测模型效果越好。
速度：从采集图像完成到人脸检测完成的时间。时间约短，检测模型效果越好。

举一个实际例子说明：
在摄像头某张抓拍图像中，一共有7张人脸，算法检测出4张人脸，其中3张是真实人脸，1张是把路标误识为人脸。
在这个实际案例中：检测率=3/7  误检率=1/4  漏检率=（7-3）/7
**3、****图像预处理**
图像预处理的目的是**消除图像中无关的信息，尽可能去除或者减少光照、成像系统、外部环境等对图像的干扰，使它具有的特征能够在图像中明显地表现出来。**主要过程包括人脸图像的光**线补偿、灰度变换、直方图均衡化、归一化、几何校正、滤波以及锐化等**。
1）**灰度化**：将彩色图像转化为灰色图像的过程。
![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/TS56qMjSfEhqlRAqZemxRV5kGdrrXHg08othU1je4LGG8bwKGEOIFc4rT4jjDmMmXzxjiasTcYgCzeg4c0ic18icA/0?wx_fmt=png)
2）**直方图均衡化**：直方图描述了一副图像的灰度级内容。直方图均衡化主要目的是为了**提高对比度和灰度色调的变化，使图像更加清晰。**
![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/TS56qMjSfEhqlRAqZemxRV5kGdrrXHg0a4koc7DlnZIQE5ZPVCGEaHXnUCKGnbgoL7WtdZOdCrpqtV7Tc6ld4Q/0?wx_fmt=png)
3）**归一化**：通过一系列变换，将待处理的原始图像转换成相应的唯一标准形式（该标准形式图像对平移、旋转、缩放等仿射变换具有不变特性），使得图像可以**抵抗几何变化的攻击**。例如用放大缩小图像、减少图片由于光照不均匀造成的干扰等。
4）中值滤波：将图片进行平滑操作以**消除噪声**。
![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/TS56qMjSfEhqlRAqZemxRV5kGdrrXHg0VPvsWwk5fTuYPaNfOicVMXwm871dThtqHjjwQbJKfIkAoZicgCvTKFmQ/0?wx_fmt=png)
预处理的效果直接影响到人脸识别率。可以通过不同方法，相同算法、相同训练模式评估模型效果，选择最佳的预处理方式。
4、**人脸特征提取**
传统的人脸识别模型一般都是在SVM等分类器分类后，通过CNN提取特征。
但随着深度学习的发展，人脸识别有更多端到端的框架选择。这里简单介绍一下三种近两年基于深度学习人脸识别的方法：**Deep face、Deep ID、FaceNet**
**1）Deep face**
a.Deep face 与其他方法最大的不同在于，DeepFace在训练神经网络前，使用了基于3D模型人脸对齐的方法。
![0?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/TS56qMjSfEjYdFpuvjAHavD3nPgETguf7AZal2x8YQsgkjjvyPw7k1t6E9mqG7xeDb8OsMLmlBE8KFM2Fsok9Q/0?wx_fmt=jpeg)
b.使用深度神经网络学习特征
![0?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/TS56qMjSfEjYdFpuvjAHavD3nPgETgufRRGNm3JbDeWTCgoVQYDpbXjonrJ7nDQwYM5dMiag6a5Mws6tBgcDoGg/0?wx_fmt=jpeg)
c.该模型在LFW数据集上取得了0.9735准确率
2）**DeepID**
DeepID有三代：DeepID、DeepID2、DeepID2+。
其中**DeepID2+对遮挡有很好的鲁棒性。**有两种方式对人脸进行多种尺度的遮挡，第一种是从下往上进行遮挡，从10%-70%。第二种是不同大小的黑块随机放，黑块的大小从10×10到70×70。
![0?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/TS56qMjSfEjYdFpuvjAHavD3nPgETgufapoKOx1EVFvk6X84Sicouiaia6F5zw1An6XKdGfR396YprrWRUrdHZZLw/0?wx_fmt=jpeg)
结论是：遮挡在20%以内，块大小在30*30以下，Deep2+的输出向量的验证正确率几乎不变。
3）**FaceNet**
直接将人脸图像映射到欧几里得空间，**空间的距离代表了人脸图像的相似性**。该方法基于深度卷积神经网络。FaceNet得到人脸最终表示后不用像DeepID那样需要再训练模型进行分类，直接计算距离，效率很高。
它在LFW数据集上使用了两种模式：
a.直接取LFW图片的中间部分进行训练，
b.使用额外的人脸对齐工具，准确率为0.9963，超过DeepID。
当然基于深度学习的人脸识别方法还有很多种，比如baidu的方法等等。对这块有兴趣的朋友可以看看这篇文章《基于深度学习的人脸识别技术综述》http://t.cn/RJeczWj
**5、****人脸匹配与识别**
**将提取的人脸特征数据与数据库中储存的特征模板进行搜索匹配，设定一个最佳的相似度阈值，当相似度超过该阈值，则输出匹配后的结果。**
最佳相似度阈值的决定需要考虑到输出结果的**正确率和数量**。假设1000张样本图片里，共600张正样本。相似度为0.9的图片一共100张，其中正样本为99张。虽然0.9阈值的正确率很高，为99/100；但是0.9阈值正确输出的数量确很少，只有99/600。这样很容易发生漏识的情况。
有的同学可能很快就通过这个场景联想到**精确率**和**召回率**的概念，是的，这也是需要应用精确率和召回率的实际评估场景之一。这里再解释一下精确率和召回率：
1）**精确率（precision）**：识别为正确的样本数/识别出来的样本数=99/100
2）**召回率（recall）**：识别为正确的样本数/所有样本中正确的数=99/600
![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/TS56qMjSfEhqlRAqZemxRV5kGdrrXHg0vv7vZwKSUvye1hZVSBYDjgxombTOIO6WjxnYDliaF8ZCqLLCfib6icwIA/0?wx_fmt=png)
*(图片by@mousever )*
只有当选择的阈值实现：a.阈值以上结果的精确率极高；b.阈值以上结果的召回率极高。这个阈值才是在该算法模型下最佳阈值。
除精确率和召回率值得关注外，**误报率（False Alarm）**也是非常重要的指标。家用家用机器人/摄像头，可能会常开人脸检测，误报会非常影响用户体验（比如，没有异常情况但频繁报警提示）。因此误报率会是这类场景的重要指标。
另外在这一个识别的步骤中，产品还需要确定具体输出的需求点是**人脸认证**还是**人脸识别**——
1）**人脸认证**：**一对一**的匹配过程，解决“**这是否为某人**”的问题。系统需先找出已储存的对象信息，再将待认证的信息与前者对比核对，从而判定接受或拒绝待认证的身份。
常见的应用场景：例如**移动支付认证、安全性身份核对**等。
产品在系统设计的逻辑上，需要先考虑调取已储存对象信息的先验条件。通过界面/语音的提示，使得待认证者预先知道自己正处于被核对的过程中，且已预先了解拟核对对象的身份。
![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/TS56qMjSfEhqlRAqZemxRV5kGdrrXHg0dJNDAv0WJvsLuCfHtmdAtLiapTPjOgWvXuHUic8VgOAzOutSz2I0tMPA/0?wx_fmt=png)
2）**人脸比对**：**一对多**的匹配过程，解决“**这是谁**”的问题。系统将待识别人脸图像的特征值与数据库中存储的所有已知身份的对象的特征值进行比较，来确定待识别者的身份。
常见的应用场景：**人脸开门、人脸检索**等。
产品在本需求的设计上，需要考虑系统的**实时性**和**架构方案**的不同对产品逻辑的影响。例如人脸开门常见架构是跨互联网和局域网的，人脸检索常见架构是在某一个局域网（互联网）下的。
**a.人脸开门等跨网方案需要关注的因素**
远程算法更新：远程算法更新必然会造成本地局域网功能暂时性无法使用。因此远程算法更新的频率、时间、更新效果都需要产品在更新前精确评估。
增删改人脸数据与本地数据的同步：本地局域网和互联网是无法直接交互的，因此用户在互联网一旦对人脸数据库进行增删改的操作，下发程序的稳定性和及时性都需要重点关注。
硬件环境：本地存储空间的大小和GPU直接影响到本地识别的速度。服务器的稳定性影响到功能地正常使用。
守护程序：断电等外置情况意外情况发生又被处理完善后，程序能自动恢复正常。

**b.人脸检索等某一局域网（互联网）方案需要关注的因素**
![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/TS56qMjSfEhqlRAqZemxRV5kGdrrXHg0QW8KORZdT6ibOUMwJgM3SFtia5SEo9Uic2k0P815dJfCma8PAVrhv7Rbg/0?wx_fmt=png)
速度：除了算法识别需要消耗一定时间外，该局域网下的网速会影响到识别结果输出的速度。
数据库架构：通过检索结果关联结构化数据。
阈值的可配置性：在界面设置阈值功能，从产品层面输入阈值后，改变相对应的结果输出。
输出结果排序：根据相似度排序或结构化数据排序内容地抉择
云服务的稳定性。

2
**与人脸识别相关的其他CV内容**
**1、****人脸表情识别****（Face expression recognition 简称FER）**
![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/TS56qMjSfEhqlRAqZemxRV5kGdrrXHg0mX5Hr0Gko72pnXP4dbuaJbyn3xEIdPvnMspOv3icyW6utq05cLyEJtw/0?wx_fmt=png)
人脸表情识别的一般步骤：图片获取、图像预处理（主要有图片归一化、人脸检测与定位）、特征提取及**表情分类**。
现普遍认为人类主要有六种基本情感：**愤怒（anger）、高兴（happiness）、悲伤（sadness）、惊讶（surprise）、厌恶（disgust）、恐惧（fear）**。而大多数表情识别是基于这六种情感及其拓展情绪实现的，目前主要困难点是：
a.**表情的精细化程度划分**：每种情绪最微弱的表现是否需要被分类。分类的界限需要产品给出评估规则。
b.**表情类别的多样化**：是否还需要补充其他类别的情绪，六种情绪在一些场景下远不能变现人类的真实情绪。因此除了基本表情识别外，还有**精细表情识别、混合表情识别、非基本表情识别**等细致领域的研究。
c.**缺少鲁棒性**
**2、****人脸性别识别**
性别分类是一个典型的二类问题，人脸性别分类问题需要解决的两个关键问题是**人脸特征提取和分类器的选择**。
人脸性别识别其实仅能识别到人脸外貌更偏向于女性还是男性，很难对女生男相、男生女相进行正确判断。产品需制定合理的性别划分规则。
**3、****人脸年龄识别**
![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/TS56qMjSfEhqlRAqZemxRV5kGdrrXHg0qyYRkMgPrYfic6J4SweefhwxL2erLJzV2FWFp3LKYQwwaBJ79fzTwpw/0?wx_fmt=png)
年龄识别的难度在于单人的不同年龄段识别和多人的不同年龄段识别，**人脸年龄识别常和人脸识别进行组合识别，能更正确的判断在一定年限内“是否是一个人”的问题**。
识别年龄无变化的人脸用分类即可，而对年龄变化的人脸识别方法是通过**年龄模拟**，将测试图像和查询库中的图像变换到某一共同的年年龄，从而去除年龄不同的影响，使识别在年龄相同的人脸图像进行。
除了以上内容，还有**是否戴眼镜、头发长度、肤色**等等方向，产品可根据需求对算法提出不同的组合类别。值得强调的是，随着行业的深入，我们会发现单纯的人脸识别并不能满足实际应用场景的需求，比如在安防行业或者金融领域，**活体技术**（如何验证目前的人是真人而不是照片或视频）和**防欺诈技术**才是戳中用户痛点的、更本质的关键。人脸识别技术和多个技术相互结合，是现在场景中运用得比较广泛的方案。
3
**补充说明**
**1、测试环境说明**
例如：
CPU：Intel(R) Core(TM) i7-4790 CPU @ 3.60 GHz
内存：8GB
系统：Ubuntu 14.04 x86_64/Windows 7 SP1 64bit
GCC版本：4.8.2
注：以上测试环境是PC（也可选手机上的测试环境）。
**2、需要说明测试集是什么，或者图片要求**（比如“图片包含人脸大小应超过96*96像素）
a.经典人脸身份识别测试集LFW（LFW benchmark http://vis-www.cs.umass.edu/lfw/ ）共包含13233 张图片 5749 种不同身份；世界记录99.7%。
b.CK+一个是人脸表情数据集（CK+，Extended Cohn-Kanade Dataset， http://www.pitt.edu/~emotion/ck-spread.htm ），包含固定表情和自发表情，包含123个人的593个表情序列。每个序列的目标表情被FACS编码，同时添加了已验证的情感标签（生气、厌恶、害怕、快乐、悲伤、惊讶）。
**3、需要说明“****有效距离，左右角度，上下角度，速度****”等参数值（范围）**
注：这和“**部署的灵活性**”相关——由于不同客户不同场景的需求不同，所以技术方的人脸检测模块，一般可以通过调整参数得到N种亚型，以适应不同应用场景(光照、角度、有效距离、速度) 下对运算量和有效检测距离的需求。
4、需要知道的是，**很多公司刷榜的数据，更多是PR效果**
﻿UCLA的@朱松纯 教授 在文章《浅谈人工智能：现状、任务、构架与统一 | 正本清源》里就提到：
*咱们刷榜比打乒乓球还厉害，刷榜变成咱们AI研究的“国球”。所谓刷榜，一般是下载了人家的代码，改进、调整、搭建更大模块，这样速度快。我曾经访问一家技术很牛的中国公司（不是搞视觉的），那个公司的研发主管非常骄傲，说他们刷榜总是赢，美国一流大学都不在话下。我听得不耐烦了，**我说人家就是两个学生在那里弄，你们这么大个团队在这里刷，你代码里面基本没有算法是你自己的。如果人家之前不公布代码，你们根本没法玩。很多公司就拿这种刷榜的结果宣传自己超过了世界一流水平**。*
4
**人脸识别的现状**
**1、实验室效果和现实效果对比**
1）实验室训练数据和实际抓拍数据
![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/TS56qMjSfEhqlRAqZemxRV5kGdrrXHg0ibIzq7eawOFUXZn7TEIxM9THoU8MrkKzYAQ3BZqErKMakQDq6AGouNw/0?wx_fmt=png)
**大多数情况下，实际抓拍图像质量远低于训练图像质量。**由于训练数据普遍是由用户经过手机app/web采集上来的图片，所以光照、角度、模糊程度都可以由产品控制，但是实际抓拍图片质量是和实际场景中的光线、摄像头分辨率等等因素相关，照片质量比较难把握。
在上图的情况下，即使算法模型可以达到很高的精确度，但由于抓怕图片质量不好的原因，实际效果会大打折扣甚至不可用。
这种情况的解决办法是：
对摄像头进行补光/滤光
将摄像头固定在角度合适的位置
算法在识别前进行图像预处理

2）训练时的标准和实际应用的标准
**大多数情况下，实际应用的标准会远高于训练标准。**例如，人脸识别实验室的标准是通过**正脸**数据训练出模型，能识别正确人脸就可以。而实际情况可能**没有正脸数据**，对训练提出了更高的要求。
3）训练效果和现实效果
**大多数情况下，实际效果会远低于训练效果。**现在市面上CV公司都是说自己的训练效果在99%以上（无限接近于100%），但这不等于实际应用的效果就是99%。工业上场景复杂的人脸应用（类似识别黑名单这种1:N的人脸比对）正确率在90%以上就已经是表现得很好的算法模型。
**2、算法漏洞常被利用破解人脸识别系统**
人脸识别常常被质疑是否安全性不够高，很多人脸识别的产品都能被破解。比如长得比较像的人可能会被误认为同一人；用录入人员的照片或者视频也能通过人脸系统。
![0?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/TS56qMjSfEhqlRAqZemxRV5kGdrrXHg0SQtcQOeateq6FVKI0h0Copqsbk3AY3khXQIZ8l8RvssUeBMrIRTTKA/0?wx_fmt=png)
而现在安全性能比较高的系统一般是**人脸识别技术和其他生物识别技术相结合**，例如人脸识别+指纹识别；人脸识别+虹膜识别等等。



