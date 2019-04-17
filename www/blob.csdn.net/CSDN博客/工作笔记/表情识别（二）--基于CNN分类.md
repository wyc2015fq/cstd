# 表情识别（二）--基于CNN分类 - 工作笔记 - CSDN博客





2018年04月25日 11:18:23[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：10978
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









### **说白了，就是个分类任务，但是纯粹的CNN分类，只是对传统方式的提升，本质思路没有改变，效果也不是很明显。**

### **转自：https://blog.csdn.net/walilk/article/details/58709611**

### **前言**

　　[机器学习] 实验笔记系列是以我在算法研究中的实验笔记资料为基础加以整理推出的。该系列内容涉及常见的机器学习算法理论以及常见的算法应用，每篇博客都会介绍实验相关的数据库，实验方法，实验结果，评价指标和相关技术目前的应用情况。 

　　本文主要整理自笔者在表情识别（emotion recognition）研究上的实验笔记资料，给出了表情识别常用的数据库，论文资料，识别方法，评价指标，以及笔者的实验笔记和实验结果。 

　　文章小节安排如下：

　　1）表情识别的意义

　　2）表情识别的应用

　　3）常用的数据库及比赛

　　4）实验-算法说明

　　5）实验-效果展示

　　6）结语 

### **一、表情识别的意义**

**1.1 什么是表情？**

　　我们天天都在展示自己的表情并且看到其他人的表情，那么表情到底是什么？

　　看看维基百科的定义： 
　　面部表情是面部肌肉的一个或多个动作或状态的结果。这些运动表达了个体对观察者的情绪状态。面部表情是非语言交际的一种形式。它是表达人类之间的社会信息的主要手段，不过也发生在大多数其他哺乳动物和其他一些动物物种中。

　　人类的面部表情至少有21种，除了常见的高兴、吃惊、悲伤、愤怒、厌恶和恐惧6种，还有惊喜（高兴＋吃惊）、悲愤（悲伤＋愤怒）等15种可被区分的复合表情。 

**1.2 表情的意义？**

表情是人类及其他动物从身体外观投射出的情绪指标，多数指面部肌肉及五官形成的状态，如笑容、怒目等。也包括身体整体表达出的身体语言。


　　参考：
[https://zh.wikipedia.org/wiki/%E9%9D%A2%E9%83%A8%E8%A1%A8%E6%83%85](https://zh.wikipedia.org/wiki/%E9%9D%A2%E9%83%A8%E8%A1%A8%E6%83%85)


　　简单来说，

　　面部表情是人体（形体）语言的一部分，是一种生理及心理的反应，通常用于传递情感。 

**1.3 表情的研究**

　　面部表情的研究始于 19 世纪，

　　1872年，达尔文在他著名的论著《人类和动物的表情（The Expression of the Emotions in Animals and Man，1872）》中就阐述了人的面部表情和动物的面部表情之间的联系和区别。 

　　1971年，Ekman 和 Friesen 研究了人类的 6 种基本表情 （即高兴、悲伤、惊讶、恐惧、愤怒、厌恶 ） ，并系统地建立了人脸表情图象库，细致的描述了每一种表情所对应的面部变化，包括眉毛、眼睛、眼睑、嘴唇等等是如何变化的。 

![表情的面部模式](https://img-blog.csdn.net/20170415120157590?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FsaWxr/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



　　1978年，Suwa等人提出了在图像序列中进行面部表情自动分析。

　　从20世纪90年代开始，由Mase和Pentland提出的光流法进行面部表情识别之后，自动面部表情识别进入了新的时期。 

**1.4 微表情**

　　随着研究的深入和应用的广泛，人们逐渐开始研究一种更细微的表情：微表情。

　　看维基百科的定义：

　　微表情是一种人类在试图隐藏某种情感时无意识做出的、短暂的面部表情。

　　微表情的持续时间仅为 1/25 秒至 1/5 秒，表达的是一个人试图压抑与隐藏的真正情感。

　　微表情的在自动谎言识别等众多领域有巨大的潜在应用价值，比如那部著名的电视剧《Lie to Me》，卡尔·莱特曼博士就是利用“脸部动作编码系统”（Facial Action Coding System）分析被观察者的肢体语言和微表情，进而向他们的客户（包括FBI等美国执法机构或联邦机构）提供被观测者是否撒谎等分析报告。 

　　随着科技进步和心理学的不断发展，对表情的研究越来越丰富，应用也越来越广泛。有兴趣的读者可以Google一下人类表情的研究历史，以及在心理学用的应用。 

### **二、表情识别的应用**

**2.1 表情识别的应用场景**

　　面部表情识别技术主要的应用领域包括人机交互、智能控制、安全、医疗、通信等领域。笔者目前接触过的两个领域， 
　　第一个是商场门店的顾客情绪分析。即通过摄像头捕获商场或门店的顾客画面，分析其面部表情，再进一步解读出客人的情绪信息，从而分析顾客在商场的体验满意度。 

　　第二个是人机交互。在笔者参与的一个教育辅助机器人项目中，负责视觉部分的研发工作，其中一项功能就是通过面部表情分析来判断机器人眼前的用户的情绪和心理。 

**2.2 在线应用接口**

　　微软提供了表情识别的API接口，并通过JSON返回识别结果，如下：
[Cognitive Services APIs - Emotion Recognization](https://www.microsoft.com/cognitive-services/#)

![微软表情识别API](https://img-blog.csdn.net/20170415120800192?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FsaWxr/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

　　Face++也提供了接口，并通过JSON返回识别结果，如下：
[人脸检测](https://www.faceplusplus.com.cn/face-detection/#demo)

![Face++的人脸检测API](https://img-blog.csdn.net/20170415120941851?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FsaWxr/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### **三、常用的数据库及比赛**

**3.1 表情识别常用数据库**

　　1）The Japanese Female FacialExpression (JAFFE) Database

　　发布时间：1998

　　详细说明：

　　The database contains 213 images of 7 facial expressions (6 basic facial expressions + 1 neutral) posed by 10 Japanese female models. Each image has been rated on 6 emotion adjectives by 60 Japanese subjects. The database was planned and assembled by Michael Lyons, Miyuki Kamachi, and Jiro Gyoba. We thank Reiko Kubota for her help as a research assistant. The photos were taken at the Psychology Department in Kyushu University.

　　表情：sad, happy, angry, disgust,surprise, fear, neutral.

　　地址：[http://www.kasrl.org/jaffe.html](http://www.kasrl.org/jaffe.html)

　　2）The Extended Cohn-Kanade Dataset(CK+) （这个数据库有人有吗？能共享下吗？）

　　发布时间：2010

　　详细说明：

　　The Cohn-Kanade AU-Coded Facial Expression Database is for research in automatic facial image analysis and synthesis and for perceptual studies. Cohn-Kanade is available in two versions and a third is in preparation.

　　Version 1, the initial release, includes 486 sequences from 97 posers. Each sequence begins with a neutral expression and proceeds to a peak expression. The peak expression for each sequence in fully FACS (Ekman, Friesen, & Hager, 2002; Ekman & Friesen, 1979) coded and given an emotion label. The emotion label refers to what expression was requested rather than what may actually have been performed. For a full description of CK, see (Kanade, Cohn, & Tian, 2000).For validated emotion labels, please use version 2, CK+, as described below. 

　　论文：P.Lucey, J. F. Cohn, T.Kanade, J. Saragih, Z. Ambadar, and I. Matthews, “TheExtended Cohn-KanadeDataset (CK+)_ A complete dataset for action unit andemotion-specifiedexpression,” inComputer Vision andPattern RecognitionWorkshops (CVPRW), 2010 IEEE Computer Society Conference on,2010, pp. 94-101.

　　地址：[http://www.pitt.edu/~emotion/ck-spread.htm](http://www.pitt.edu/~emotion/ck-spread.htm)

　　3）GEMEP-FERA 2011

　　发布时间：2011

　　详细说明：

　　该库是在 IEEE 的 Automatic Face & GestureRecognition and Workshops (FG 2011), 2011 IEEE International Conference on 上提供的一个数据库，如果要获取这个数据库，需要签署一个assignment，而且只有学术界可以免费使用。

　　论文：M.F. Valstar, M. Mehu, B.Jiang, M. Pantic, and K. Scherer, “Meta-Analysis ofthe First FacialExpression Recognition Challenge,”Systems,Man, andCybernetics, Part B: Cybernetics, IEEE Transactions on, vol. 42,pp. 966-979,2012.

　　地址：[https://gemep-db.sspnet.eu/](https://gemep-db.sspnet.eu/)

　　4）AFEW_4_0_EmotiW_2014

　　详细说明：

　　该数据库用作 ACM 2014 ICMI TheSecond Emotion Recognition In The Wild Challenge and Workshop。数据库中提供原始的video clips，都截取自一些电影，这些clips 都有明显的表情，这个数据库与前面的数据库的不同之处在于，这些表情图像是 in the wild, not inthe lab。所以一个比较困难的地方在于人脸的检测与提取。

　　论文：A.Dhall, R. Goecke, J. Joshi,M. Wagner, and T. Gedeon, “Emotion RecognitionIn The Wild Challenge2013,” inProceedings of the 15thACM on Internationalconference on multimodal interaction, 2013, pp.509-516.

　　地址：[https://cs.anu.edu.au/few/emotiw2014.html](https://cs.anu.edu.au/few/emotiw2014.html)

　　5）GENKI-4K

　　详细说明：

　　The MPLab GENKI Database is an expanding database of images containing faces spanning a wide range of illumination conditions, geographical locations, personal identity, and ethnicity. Each subsequent release contains all images from the previous release, and so is guaranteed to be backward compatible. The database of images is divided into overlapping subsets, each with its own labels and descriptions. For example, the GENKI-4K subset contains 4000 face images labeled as either “smiling” or “non-smiling” by human coders. The pose of the faces is approximately frontal as determined by our automatic face detector. The GENKI-SZSL subset contains 3500 images containing faces. They are labeled for the face location and size. The images are available for public use.

　　The current release of the GENKI database is GENKI-R2009a. It contains 7172 unique image files, which combine to form these subsets:

　　GENKI-4K: 4000 images, containing expression and head-pose labels.

　　GENKI-SZSL: 3500 images, containing face position and size labels.

　　论文：WhitehillJ, Littlewort G, Fasel I, et al. Toward practical smile detection[J]. PatternAnalysis and Machine Intelligence, IEEE Transactions on, 2009, 31(11):2106-2111.

　　地址：[http://mplab.ucsd.edu/wordpress/?page_id=398](http://mplab.ucsd.edu/wordpress/?page_id=398)

　　6）The UNBC-McMaster shoulder painexpression archive database

　　论文：Lucy,P., Cohn, J. F., Prkachin, K. M., Solomon, P., & Matthrews, I. (2011).Painful data: The UNBC-McMaster Shoulder Pain Expression Archive Database. IEEEInternational Conference on Automatic Face and Gesture Recognition (FG2011).

　　地址：[http://www.pitt.edu/~emotion/um-spread.htm](http://www.pitt.edu/~emotion/um-spread.htm)

**3.2 表情识别比赛**





　　1）The Third Emotion Recognition in the Wild Challenge

　　这是ACM International Conference on Multimodal Interaction (ICMI 2015)举办的一个表情识别的竞赛，每年都举办，感兴趣的可以参加一下。

[https://cs.anu.edu.au/few/emotiw2014.html](https://cs.anu.edu.au/few/emotiw2014.html)
[https://cs.anu.edu.au/few/emotiw2015.html](https://cs.anu.edu.au/few/emotiw2015.html)
[https://sites.google.com/site/emotiw2016/](https://sites.google.com/site/emotiw2016/)



    2)Challenges in Representation Learning: Facial Expression Recognition Challenge

　   https://www.kaggle.com/c/challenges-in-representation-learning-facial-expression-recognition-challenge

### **四、实验-算法说明**

**4.1 表情数据库**

　　Fer2013：Kaggle facial expression recognition challenge dataset

　　详细说明：

　　The training set consists of 28,709 examples. The public test set used for the leaderboard consists of 3,589 examples. The final test set, which was used to determine the winner of the competition, consists of another 3,589 examples.

　　表情分类：
0=Angry, 1=Disgust, 2=Fear, 3=Happy, 4=Sad, 5=Surprise, 6=Neutral

　　数据分布：　　

　　~/trainImg.zip　　　　　训练集，28709

　　~/privateTestImg.zip　　测试集，3589

　　~/publicTestImg.zip　　 测试集，3589


　　数据分布（训练集）：

　　Angry：3995

　　Disgust：436

　　Fear：4097

　　Happy：7215

　　Sad：4830

　　Surprise：3171

　　Neutral：4965


　　数据分布（fer2013_privatetest.csv）：

　　Angry：3995

　　Disgust：436

　　Fear：4097

　　Happy：7215

　　Sad：4830

　　Surprise：3171

　　Neutral：4965

**关于fer2013数据文件的使用：**

　　fer2013中的图片以“标签 图像向量”的形式保存在.csv文件中，即每一个列向量是一张图片，第一个数字是标签，剩下的数字是图片数据（将图片矩阵按列展开存储的），可以利用matlab的reshape函数将图片还原出来，图像尺寸是48x48大小。 

**4.3 参考论文**

　　论文：

　　Jeon, Jinwoo, et al. “A Real-time Facial Expression Recognizer using Deep Neural Network.” International Conference on Ubiquitous Information Management and Communication ACM, 2016:94.

　　基本方法：

　　采用深度神经网络算法，其网络结构是在AlexNet上修改得到，减少了3个卷积层，示意如下： 

![网络结构示意图](https://img-blog.csdn.net/20170415121714377?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FsaWxr/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



　　网络结构：


![网络结构详细描述](https://img-blog.csdn.net/20170415121739911?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FsaWxr/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

　　实验结果：

　　The average accuracy for all categories was 70.74%. Accuracy for the happy and surprise category was higherthan the others, but accuracy for the fear category was poor. 

　　混淆矩阵：


![论文的混淆矩阵](https://img-blog.csdn.net/20170415121844068?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FsaWxr/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



**4.3 实验环境**

　　深度学习框架：caffe

　　GPU卡：GeForce GTX TITAN Black 

**4.4 训练配置文件（solver.prototxt）**

　　net: “./train_val_rferdnn.prototxt”

　　test_iter: 1200

　　test_interval: 1000

　　base_lr: 0.001

　　lr_policy: “fixed”

　　display: 500

　　max_iter: 100000

　　momentum: 0.9

　　weight_decay: 0.0005

　　snapshot: 20000

　　snapshot_prefix: “./models/rt_exprec”

　　solver_mode: GPU 

**4.5 实验重现**

　　经过不断的调整参数和数据处理方法，笔者最后基本复现了论文中的实验结果，达到 70.2% 的准确率。 

　　具体如下：

　　average accuracy:

　　70.2%

　　confusion matrix:


![我实验复现的混淆矩阵](https://img-blog.csdn.net/20170415122019678?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FsaWxr/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**4.6 数据扩展方法**

　　论文《A Real-time Facial Expression Recognizer using Deep Neural Network》中采用的是随机crop出5个图像，说是可以 spatial invariance is induced。但文中的配图看起来并不像是随机采样的，如下：


![数据扩展方法](https://img-blog.csdn.net/20171026164208788?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FsaWxr/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

　　这里是直接按照四个角+中间进行采样的，不过这也可以看做是随机的一种吧。我在实验中也是这么采样的，然后再mirror一下，这样就将训练数据量扩大了10倍。 

**4.7 预测方法**

　　1）规则扩展测试图像，即crop测试图像四个corner区域+一个center区域，再做镜像（mirror），也就是每个原始样本扩展出10个crop_image；

　　2）得分融合：averaging method，即计算原始图像的10个扩展的预测结果的均值作为最终得分。 

　　测试集：fer2013_privatetest.csv（我记得应该是这个，还有一个，还有一个fer2013_publictest.csv我忘记测试了没有了(⊙﹏⊙)） 

**4.8 模型下载**

[http://pan.baidu.com/s/1jI1kwya](http://pan.baidu.com/s/1jI1kwya)，这是私有链接，需要下载的朋友请私信我。我希望各位同学能够自己琢磨算法、训练模型，这样才能有进步，而不是直接拿来用。 

**4.9 训练过程**

　　为了更好的展示实验，这里给出我当时的训练输出。不过我的硬盘里只找到了一部分输出和最终训练结果，非常抱歉！

**Iteration 0**

I0421 08:09:17.860702 27484 solver.cpp:341] Iteration 0, Testing net (#0)

I0421 08:10:26.924216 27484 solver.cpp:409] Test net output #0: accuracy = 0.142594

I0421 08:10:26.924386 27484 solver.cpp:409] Test net output #1: loss = 1.94273 (* 1 = 1.94273 loss)

I0421 08:10:27.081188 27484 solver.cpp:237] Iteration 0, loss = 1.94555

I0421 08:10:27.081243 27484 solver.cpp:253] Train net output #0: loss = 1.94555 (* 1 = 1.94555 loss)

I0421 08:10:27.081269 27484 sgd_solver.cpp:106] Iteration 0, lr = 0.001

I0421 08:11:52.943675 27484 solver.cpp:237] Iteration 500, loss = 1.81799

I0421 08:11:52.943876 27484 solver.cpp:253] Train net output #0: loss = 1.81799 (* 1 = 1.81799 loss)

I0421 08:11:52.943889 27484 sgd_solver.cpp:106] Iteration 500, lr = 0.001

I0421 08:13:20.381688 27484 solver.cpp:237] Iteration 1000, loss = 1.73778

I0421 08:13:20.381914 27484 solver.cpp:253] Train net output #0: loss = 1.73778 (* 1 = 1.73778 loss)

I0421 08:13:20.381924 27484 sgd_solver.cpp:106] Iteration 1000, lr = 0.001

**Iteration 5000**

I0421 08:24:59.778053 27484 solver.cpp:341] Iteration 5000, Testing net (#0)

I0421 08:26:16.136390 27484 solver.cpp:409] Test net output #0: accuracy = 0.562633

I0421 08:26:16.136633 27484 solver.cpp:409] Test net output #1: loss = 1.15914 (* 1 = 1.15914 loss)

I0421 08:26:16.282253 27484 solver.cpp:237] Iteration 5000, loss = 1.20592

I0421 08:26:16.282282 27484 solver.cpp:253] Train net output #0: loss = 1.20592 (* 1 = 1.20592 loss)

I0421 08:26:16.282294 27484 sgd_solver.cpp:106] Iteration 5000, lr = 0.001

I0421 08:27:43.646217 27484 solver.cpp:237] Iteration 5500, loss = 1.18084

I0421 08:27:43.646412 27484 solver.cpp:253] Train net output #0: loss = 1.18084 (* 1 = 1.18084 loss)

I0421 08:27:43.646425 27484 sgd_solver.cpp:106] Iteration 5500, lr = 0.001

**Iteration 10000**

0421 08:40:47.484722 27484 solver.cpp:341] Iteration 10000, Testing net (#0)

I0421 08:42:00.271831 27484 solver.cpp:409] Test net output #0: accuracy = 0.665208

I0421 08:42:00.272018 27484 solver.cpp:409] Test net output #1: loss = 0.891976 (* 1 = 0.891976 loss)

I0421 08:42:00.417796 27484 solver.cpp:237] Iteration 10000, loss = 0.875526

I0421 08:42:00.417830 27484 solver.cpp:253] Train net output #0: loss = 0.875526 (* 1 = 0.875526 loss)

I0421 08:42:00.417840 27484 sgd_solver.cpp:106] Iteration 10000, lr = 0.001

**Iteration 15000**

I0421 08:56:28.140434 27484 solver.cpp:341] Iteration 15000, Testing net (#0)

I0421 08:57:43.430665 27484 solver.cpp:409] Test net output #0: accuracy = 0.780628

I0421 08:57:43.430855 27484 solver.cpp:409] Test net output #1: loss = 0.612954 (* 1 = 0.612954 loss)

I0421 08:57:43.581197 27484 solver.cpp:237] Iteration 15000, loss = 0.656312

I0421 08:57:43.581219 27484 solver.cpp:253] Train net output #0: loss = 0.656312 (* 1 = 0.656312 loss)

I0421 08:57:43.581230 27484 sgd_solver.cpp:106] Iteration 15000, lr = 0.001

**Iteration 20000**

I0421 09:12:06.863005 27484 solver.cpp:341] Iteration 20000, Testing net (#0)

I0421 09:13:22.342000 27484 solver.cpp:409] Test net output #0: accuracy = 0.865189

I0421 09:13:22.342190 27484 solver.cpp:409] Test net output #1: loss = 0.389436 (* 1 = 0.389436 loss)

I0421 09:13:22.488214 27484 solver.cpp:237] Iteration 20000, loss = 0.539341

I0421 09:13:22.488237 27484 solver.cpp:253] Train net output #0: loss = 0.539341 (* 1 = 0.539341 loss)

I0421 09:13:22.488248 27484 sgd_solver.cpp:106] Iteration 20000, lr = 0.001

**Iteration 25000**

I0421 09:27:55.352700 27484 solver.cpp:341] Iteration 25000, Testing net (#0)

I0421 09:29:10.297168 27484 solver.cpp:409] Test net output #0: accuracy = 0.914303

I0421 09:29:10.297345 27484 solver.cpp:409] Test net output #1: loss = 0.256905 (* 1 = 0.256905 loss)

I0421 09:29:10.442956 27484 solver.cpp:237] Iteration 25000, loss = 0.365221

I0421 09:29:10.442978 27484 solver.cpp:253] Train net output #0: loss = 0.365221 (* 1 = 0.365221 loss)

I0421 09:29:10.442988 27484 sgd_solver.cpp:106] Iteration 25000, lr = 0.001

**Iteration 100000**
accuracy = 0.996794

loss = 0.010607 (* 1 = 0.010607 loss) 

### **五、实验-效果展示**

　　基于训练好的模型做了一些测试，效果如下：


![示例1](https://img-blog.csdn.net/20170415132018542?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FsaWxr/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![示例2](https://img-blog.csdn.net/20170415132234881?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FsaWxr/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



![示例3](https://img-blog.csdn.net/20170415132349453?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FsaWxr/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![示例4](https://img-blog.csdn.net/20170415132429570?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FsaWxr/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



![示例5](https://img-blog.csdn.net/20170415132459164?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FsaWxr/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![示例6](https://img-blog.csdn.net/20170415132512446?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FsaWxr/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



![示例7](https://img-blog.csdn.net/20170415132538252?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FsaWxr/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



![示例8](https://img-blog.csdn.net/20170415132547612?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FsaWxr/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### **六、结语**

　　现在人脸分析技术火遍大江南北，Microsoft，Face++，Linkface等都提供了表情识别技术，但从该技术的难度和现阶段的准确度来看，表情识别还只能用于那些对准确率要求不高的场景下，做一些辅助分析的工作。 

　　那么表情识别技术是否就不那么重要了呢？？？不是！如同笔者在文章开头所说，表情是人内心的直观反应，因此表情识别技术是人机交互技术的重要组件。未来机器人想要更懂人类更好的跟人类交互，那么表情识别技术是必不可少的。 

### **参考**

[维基百科-表情](https://zh.wikipedia.org/wiki/%E9%9D%A2%E9%83%A8%E8%A1%A8%E6%83%85)
[人脸表情识别常用的几个数据库](http://blog.csdn.net/matrix_space/article/details/25496815)
[Emotient 的表情识别技术的门槛是什么，有哪些应用场景？](https://www.zhihu.com/question/23003796)
[微软研究院大咖为你科普人脸表情识别技术](https://zhuanlan.zhihu.com/p/24483573)

参考论文：

Jeon, Jinwoo, et al. “A Real-time Facial Expression Recognizer using Deep Neural Network.” International Conference on Ubiquitous Information Management and Communication ACM, 2016:94.

其实FER2013测试级有很多标签错误，能做到70%不错了，看2013比赛的排名：

![](https://img-blog.csdn.net/2018051220015913)

之后就没有更新了。



## 2013-fer2013第一名71.2%：损失函数为L2-SVM的表情分类《Deep Learning using Linear Support Vector Machines》
- 将损失函数Softmax改为L2-SVM，相比L1-SVM,它具有可微，并且对误分类有更大的惩罚。
- SVM约束函数如右公式4。改写为无约束优化问题如公式5,即为L1-SVM的最初形式。公式6为L2-SVM的形式。

![](https://img-blog.csdn.net/2018052815032976)![](https://img-blog.csdn.net/20180528150351629)![](https://img-blog.csdn.net/20180528150357739)
- L2-svm通过 argtmax(wTx)t(tn∈{−1,+1})来判断x的类别。
- 在反向传播需要对其求导。如公式10为L1-SVM的，它是不可微。而公式11是L2-SVM的且是可微的。 且L2-SVM比L1-SVM效果好些。
- Softmax和L2-SVM在FER2013中的效果如图:

![](https://img-blog.csdn.net/20180528150405471)



