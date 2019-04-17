# 手势识别（三）--ChaLearn Gesture Challenge数据说明 - 工作笔记 - CSDN博客





2016年12月07日 09:02:16[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：7256
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









原文：http://www.cnblogs.com/tornadomeet

另外一个博主根据上文修改过：http://blog.csdn.net/wm_1991/article/details/52577623

ChaLearn Gesture Challenge挑战赛是手势识别中比较新的(2011年开始的)一个挑战赛，属于[机器学习](http://lib.csdn.net/base/2)挑战赛中的一个，其初衷是进行One-Shot
 learning的挑战，当然也不局限在此。官网为：[http://gesture.chalearn.org/](http://gesture.chalearn.org/)  ,2012年分别主办了两轮比赛，一个是在CVPR2012中进行，另一个是在ICPR2012中进行。关于本次比赛的一些规则，以及已经完成比赛的作者的识别方法的一些细节在官网上都能查看得到。大家有兴趣13年也可以参加下。因为在日常生活中会遇到各种各样的手势，如果对这些手势的识别能取得很大成功的话，那么离AI又更近一步了，下面这张图是生


活中常见产生手势的场所：

![](http://images.cnitblog.com/blog/381513/201301/10112735-2eb2d82cfd4d457c866da2e6bb5e7663.png)


　　本文主要是简单介绍下关于这个挑战赛的[数据库](http://lib.csdn.net/base/mysql)，数据库的英文名称为ChaLearn Gesture Data，简称CGD，或者CGD2011。数据库可以从[http://gesture.chalearn.org/data/cgd2011](http://gesture.chalearn.org/data/cgd2011)这里下载，如果下载压缩版的则大小约5G，否则无压缩版的则达到30G。考虑到即使压缩过后，其实也不会对识别结果有太大的影响，且本人电脑承受能力有限，所以我这里下载了个压缩版的。


　　当然了，下载下来后的数据库都是视频格式，分为深度图像视频和彩色图像视频，因为在录制视频的时候是用Kinect录制的。由此可见，这些手势识别既可以能有深度信息又可以利用色彩信息，这对搞深度数据的行为识别的人算是一个很好的公共数据库了。


　　下面是这个数据库的一些特征：

**1.** 共有30个左右的手势单词，也就是说有30个基本的手势动作。

**2.** 视频数据分为500个batch，每个batch含有94个视频，视频分为2种，每种各47个。如果视频名称为M_x.avi的话，则表示是RGB图像的视频，否则名称为K_x.avi表示的是深度图像的视频。每个视频有可能有多个基本手势动作，一般是1~5个，且每个batch最终是100个手势，很明显这中间有重复的手势(因为总共才有30个手势单词)。因此，最终有50000=500*100个手势。每个batch如下图显示：

![](http://images.cnitblog.com/blog/381513/201301/10112834-913c5c8dda7b48c6af4660ce4932ae53.png)


　　当然了，其中还包含了2个csv文件，用来描述训练和[测试](http://lib.csdn.net/base/softwaretest)数据用的（具体应用还没弄过）。数据库中每一个batch都是对应一个人的手势，且每一个batch都只有8~15种手势，虽然说有47个视频共100个手势，但是这是One-shot
 learning，所以训练数据的标签也是只有8~15个，其它的都是测试数据的标签（每一个batch中的每一种手势都只能用一个视频来训练）。

**3.**视频数据的录制是由20个人完成的。取出一个人的某一个手势的深度图和RGB图，显示如下：

![](http://images.cnitblog.com/blog/381513/201301/10112912-d07d2b31e060423d9f8e7e7b843e35bd.png)![](http://images.cnitblog.com/blog/381513/201301/10112920-c3ede62f314348b59b478069b7e1bdfd.png)

**4.** 视频中每一张图片的大小为240*320.

**5.** 视频的帧率是每秒10帧。

**6.** 这500个batch又分为2种，即”devel”和”valid”,其中”devel”有480个，”valid”有20个。”devel”表示可用于挑战赛开发(development)的数据库，这些数据提供了训练数据和测试数据的标注。”valid”表示用于验证(validation)的，只提供了训练数据的标签。测试结果需要在网络上提交。

**7.**深度图像是以灰阶的形式显示出来的，其计算公式如下所示：


　　f(x)=(x-mini)/(maxi-mini)，max为最大距离，min为最小距离。X是当前点的距离，f(x)为深度图像显示时的灰度值。

**8.**深度图像也是有不同精度的，因为录制数据时的软件版本不同。这里分为3个精度，0表示一般（Mediocre），1表示比较好(Good),2表示非常好(Very Good).
 比如说前面几个batch的精度显示如下（num表示序号，acc表示精度， miss表示漏洞的文件）：

![](http://images.cnitblog.com/blog/381513/201301/10112934-10bd2eb3fab043ccbaa04b34fc7e4432.png)


　　如果大家是研究手势识别的话，特别是基于Kinet深度信息的，可以一起交流下，挑战下这个数据库.


官网上不仅提供了数据库的下载以及一些比赛规则，还提供了丰富的sample，这些sample中包含了很多图片，视频的处理，比如说深度图像和RGB图像的去噪，梯度计算，采样；视频特征的提取，这些特征包括STIP，MHI，MEI，HOG，HOF，PCA等；一些手势识别的算法，比如DTW；头部检测，骨骼检测，背景消除等等。在官网[http://gesture.chalearn.org/data/sample-code](http://gesture.chalearn.org/data/sample-code)上可以下载到最新版的code，解压后有4个.m文件，这4个文件就是提供的sample，分别为browse.m(用来浏览和播放视频数据的，有GUI，类似一个简单的播放器)，examples.m(该文件中包含了26个sample，sample的内容如上面所讲)，main.m(该文件提供了一个principal
 motion的识别算法，有整个训练和识别的全部代码，效果还不错)，prepare_final_resu(主要是用来参加挑战赛提交验证代码的格式统一用的)。


关于该数据库的挑战赛在2012年主办了2场，其比赛结果依次分别在CCPRW2012和ICPRW2012中公布。这2轮比赛过程中官网[http://gesture.chalearn.org/](http://gesture.chalearn.org/)中都有对参加比赛的队伍所使用的方法等从感性上做了个调查，并且公布了其调查报告。从这些报告中我们可以参考到做手势识别的大概流程，每个流程中常见的方法是什么，我想这点对初步进入该领域的人来说应该有很大的启发作用。下面就官网发布的第二轮比赛调查[Method
 survey](https://docs.google.com/a/chalearn.org/viewer?a=v&pid=sites&srcid=Y2hhbGVhcm4ub3JnfGdlc3R1cmVjaGFsbGVuZ2V8Z3g6MmU1ZjQzNjFlZGUxNjhjZg&pli=1)文章来简单分析下。


　　从文章中可以看出识别过程主要分为2大部分：预处理及数据表达、识别。


　　其中预处理及数据表达又可以细分为很多，比如说图像增强和滤波，这有时候也是需要的，因为识别的视频数据中有不少噪声。文章对比赛获得前8名的方法分析了下，其结果如下：

![](http://images.cnitblog.com/blog/381513/201302/26124148-a4b5e45107fe4f15b31b3a773e0889d5.png)


　　其中的方法也有不少，比如说去噪，均衡白化，对比度增强，频域变化，背景消除等。特征提取各队伍使用的方法分布如下：

![](http://images.cnitblog.com/blog/381513/201302/26124205-6c849607c6994143a72ec3b1c5430367.png)


　　其中包括了常见的HOG/HOF描述等。


　　当然了，特征最终提取出来还是要经过维数约简的，如下:

![](http://images.cnitblog.com/blog/381513/201302/26124220-4889ee8a179d4452b5f096c4f564d4fd.png)


　　在识别过程中，有可能会使用到时间分割，如下：

![](http://images.cnitblog.com/blog/381513/201302/26124233-35446396dfd04f04b807f4fd5f62029a.png)


　　很多分割方法和识别是结合在一起的。




　　当然了，识别过程中比较重要的是手势的表达，这些队伍使用的方法如下：

![](http://images.cnitblog.com/blog/381513/201302/26124258-441297b7a70c4627a5f010ef3774c201.png)


　　其中基于可变长度的向量表示最多。




　　在分类时的相似度测量，使用的方法分布如下：

![](http://images.cnitblog.com/blog/381513/201302/26124312-20238cff0340480ca6c8a88f44411d73.png)


　　最后面就是使用的分类器了，如下所示：

![](http://images.cnitblog.com/blog/381513/201302/26124324-9f3807aa28204bd9a9b8383448fd21fc.png)


　　可以看出在这些队伍中，用得最多的分类器是最近邻。




　　现在总结下手势识别系统的大概流程可以如下（当然了，并不是每个步骤都需要的）：

***去噪预处理——》时间分割——》特征提取——》维数约简——》特征表示——》分类器设计（可能用到相似度测量）.***

CGD数据库是2011年建立的，且在2012年利用该数据库进行了两轮One shot learning的挑战赛，从官网上给的CGD数据库可以推断出组织方是花费了很大的精力来收集这些数据的，不仅而此，组织方还给出了很多sample代码以及一些对应数据采集界面，方便大家共同扩大该数据库。CGD数据库目前有50000多个关于手势视频，共500个batch，每个batch都是同一个人在相同环境下的手势动作，只是手势内容不同而已。且每个batch都分为训练部分和测试部分。训练部分的每个视频只包含一个手势，而测试视频则包含1~5个手势。且两者都有标注对应的视频是哪个手势。在看过one
 shot learning的一些文章前脑袋里会有2个疑惑。

1. 该CGD数据库只适应于做One shot learning吗？

　　答案肯定不是。不过在目前的CGD库的每个batch中的训练部分中，一个手势只对应一个视频，且每个视频只有一个手势。测试部分则可以有多个手势（意味着需要进行不同手势的时间维分割），不同的batch之间只是人和环境不同，且手势类别的标注是独立的，也就是说batch01和batch02中标注为1的那个手势其实不一定是同一个手势。这个也非常容易理解，因为该数据库目前是为了one
 shot learning而标注的，一个手势只能用一个视频来学习。所以CGD的50000个手势大部分都没有用到，其实只用到了其中的30个视频作为训练即可，其它的都可以用来做测试。因此，在目前的数据库和目前的标注下，确实只能用于做one shot learning，如果想用到其它需要大量数据训练的领域的话（比如deep learning, supervised learning等）就需要重新标注这些手势视频，使它们的标注在不同的batch中保持一致。



2. 2012年的这两轮one shot learning比赛算法流程大致是什么呢？不是说只能用一个数据来训练么，那为什么官方的分析[ChaLearn
 Gesture Challenge_4：one shot learning比赛结果简单分析](http://www.cnblogs.com/tornadomeet/archive/2013/02/26/2933358.html)中还有分类器这一栏呢？

　　首先因为one shot learning确实只能使用一个样本来训练，所以一般的分类器都会失效。比如说svm，adaboost，random tree等。因为这些类器的设计需要大量的正样本，同时也需要不少负样本，所以就目前只有一个样本的情况，是不可能训练出这些分类器参数的。并且该挑战赛其实是一个多分类问题，那就更需要样本了。因此这里的分类器我们要理解为测试样本的特征和训练样本中的每个样本特征（因为N个手势对应N个样本）进行相似度比较，找出最相似的那个样本手势类别为最终的分类结果。从这些比赛作者提供的论文可以看出，它们大部分都使用最近邻法，这就很容易理解了。

　　现在来看看one shot learning比赛的大致算法流程：首先肯定是提取出每个batch中训练样本的特征（虽然每个手势只有一个视频），这个特征一般用个向量表示。然后把测试视频中手势的特征向量也提取出来，由于一个测试视频有可能含有多个手势，所以必须先把这些手势视频区域给分割出来，这个分割的准确非常影响最终的评价结果（其实就是相似度测量），因为后面是用编辑距离来评价的，如果分割出的手势个数出现错误，或者位置出现错误，则这个编辑距离会变得很大。从比赛统计结果发现，参赛队伍一般使用candidate
 cuts的方法来进行分割的。

参考资料:

[http://gesture.chalearn.org/](http://gesture.chalearn.org/)

[Method
 survey](https://docs.google.com/a/chalearn.org/viewer?a=v&pid=sites&srcid=Y2hhbGVhcm4ub3JnfGdlc3R1cmVjaGFsbGVuZ2V8Z3g6MmU1ZjQzNjFlZGUxNjhjZg&pli=1)






