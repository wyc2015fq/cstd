# coursera-斯坦福-机器学习-吴恩达-第11周笔记-ORC系统 - 博客堂 - CSDN博客





2018年01月09日 22:28:41[最小森林](https://me.csdn.net/u012052268)阅读数：2350
所属专栏：[斯坦福-吴恩达《机器学习》之路](https://blog.csdn.net/column/details/18191.html)









# coursera-斯坦福-机器学习-吴恩达-第11周笔记-ORC系统



- [coursera-斯坦福-机器学习-吴恩达-第11周笔记-ORC系统](#coursera-斯坦福-机器学习-吴恩达-第11周笔记-orc系统)- [1图像ORC](#1图像orc)- [1问题描述](#11问题描述)
- [2 滑动窗sliding windows](#12-滑动窗sliding-windows)
- [3获取大量的图片](#13获取大量的图片)
- [4分析](#14分析)

- [2复习 quiz](#2复习-quiz)





## 1图像ORC

### 1.1问题描述

在这一段介绍一种 机器学习的应用实例 照片OCR技术。 我想介绍这部分内容的原因 主要有以下三个 ， 

1. 第一 我想向你展示 一个复杂的机器学习系统 是如何被组合起来的  

2. 第二 我想介绍一下 机器学习流水线（machine learning pipeline）的有关概念， 以及在决定下一步做什么时， 如何分配资源。  

3. 最后,通过介绍照片OCR问题 的机会来告诉你， 机器学习的诸多 有意思的想法和理念 。其中之一是如何将机器学习 应用到计算机视觉问题中， 第二是有关 人工数据合成（artificial data synthesis）的概念。
OCR技术 主要解决的问题是让计算机 读出照片中拍到的文字信息。

![image](https://img-blog.csdn.net/20150411222155597?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGlwaXNvcnJ5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**OCR pipeline**的意思主要是把一个ML系统分割为几个连续的部分，如下图：

![image](https://img-blog.csdn.net/20150411222210464?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGlwaXNvcnJ5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

如果你有一个工程师的团队 在完成同样类似的任务， 那么通常你可以让 不同的人来完成 不同的模块 ，所以我可以假设 文字检测这个模块 需要大概1到5个人 ，字符分割部分 需要另外1到5个人 ，字母识别部分 还需要另外1到5个人。

在复杂的机器学习系统中 流水线的概念 已经渗透到各种应用中 

### 1.2 滑动窗(sliding windows)

为了更好地介绍 图像的检测 ，我们从一个简单一点的例子开始， 我们先看这个探测行人的例子：

在行人检测中 你希望照一张相片 然后找出图像中 出现的行人。

![image](https://img-blog.csdn.net/20150411225629558?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGlwaXNvcnJ5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

这个问题似乎 比文字检测的问题更简单， 原因是 大部分的 行人都比较相似， 因此可以使用一个固定宽高比的 矩形来分离出你希望找到的行人。

![image](https://img-blog.csdn.net/20150411225825042?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGlwaXNvcnJ5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

我们要做的是 首先对这个图像取一小块长方形， 比如这是一个 82×36的图像块， 我们将这个图像块 ，通过我们训练得到的分类器 来确定 这个图像块中是不是有行人。 如果没问题的话， 我们的分类器 应该报告这个图像块 y=0 因为没有行人 。

做完这以后 ， 我们再向右滑动一点窗口 然后同样地 把图像块传入分类器， 你每次滑动窗口的 大小是一个参数 ，通常被称为 步长(step size) 。这样一个滑动的过程就叫做：滑动窗(sliding windows)。

接下来我们转向 文字识别的例子 ，让我们来看看 对于照片 OCR 流水线中 要检测出文字 需要怎样的步骤 。 

1. 第一步通过滑动窗在图像中找出有字母的部分，然后把他变白方便下一步提取。

![image](https://img-blog.csdn.net/20150411233025066?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGlwaXNvcnJ5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
- 分割字母，使用滑动窗在上一部提取的像素中提取分割的部分。

![image](https://img-blog.csdn.net/20150411233126870?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGlwaXNvcnJ5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![image](https://img-blog.csdn.net/20150411233131335?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGlwaXNvcnJ5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center) —–> ![image](https://img-blog.csdn.net/20150411233247428?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGlwaXNvcnJ5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

3. 第三步，字母分类。
![image](https://img-blog.csdn.net/20150411233318488?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGlwaXNvcnJ5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)分类哪个字母或哪个26个字符A到Z。

### 1.3获取大量的图片

Getting Lots of Data and Artificial Data获取大量数据和人工合成数据。

要想获得一个比较高效的 机器学习系统， 其中一种最可靠的办法是 选择一个低偏差的学习算法 ，然后用一个巨大的训练集来训练它 。但你从哪儿得到那么多的训练数据呢？ 其实在机器学习中 有一个很棒的想法 叫做“人工数据合成”（artificial data synthesis） 。 就是说用一个小的训练集 将它扩充为一个 大的训练集 ，这节课中 我们将对这两种方法进行介绍 。

为了介绍人工数据合成的概念 让我们还是用之前用过的 照片OCR流水线中 的字母识别问题， 我们输入一个图像数据 然后想识别出是什么字母 。

如果你想要获得 更多的训练样本 ，其中一种方法是你可以 采集同一个字符的不同种字体 ， 

然后将这些字符 加上不同的随机背景。 比如你可以取这个字母C， 然后把它粘贴到一个随机背景前面 。

![image](https://img-blog.csdn.net/20150412145553925?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGlwaXNvcnJ5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

因此通过使用合成的数据 ，你实际上已经获得了 无限的训练样本， 这就是人工数据合成 。

idea2: take an existing example and and introducing distortions通过引入扭曲合成数据 
![image](https://img-blog.csdn.net/20150412145610024?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGlwaXNvcnJ5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
注意：网格线覆盖图像只是为了说明的目的。

![image](https://img-blog.csdn.net/20150412150817672?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGlwaXNvcnJ5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

如果您面临机器学习问题，通常值得做两件事情： 

其中一个就是头脑清楚，通过学习曲线，可以得到更多的数据。 

其次，假设情况如此，请问：要获得十倍的创造数据将需要多少时间，但有时候，您可能会感到惊讶，原因可能是几天，几周甚至几天 ，这可以是一个很好的方式来给你的学习算法在性能上有巨大的提升。
### 1.4分析

Ceiling Analysis：What Part of the Pipeline to Work on Next 上限分析-接下来工作重心应放在pipeline哪个部分。

上限分析的想法：通过经历这样的分析，你试图找出什么是上升的潜力，改善这些组件的每一个，或者如果这些组件中的一个变得绝对完美，那么你可能获得多少 

对该系统的性能提出了一个上限。

![image](https://img-blog.csdn.net/20150412154533059?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGlwaXNvcnJ5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

首先写出总的项目精度为72%；然后手工把某一个模块设置为“全对”，看模型提升了多少，提升的多说明这个工作有用；提升的少，说明在这里做工作没什么太大意义。

另外一个例子:Another more complex ceiling analysis example人脸识别。 
![image](https://img-blog.csdn.net/20150412154559922?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGlwaXNvcnJ5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
看每一个小块的提升对总的精度提升的大小。 
![image](https://img-blog.csdn.net/20150412154537602?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcGlwaXNvcnJ5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## 2复习 quiz
- 
第 1 个问题 

Suppose you are running a sliding window detector to find text in images. Your input images are 1000x1000 pixels. You will run your sliding windows detector at two scales, 10x10 and 20x20 (i.e., you will run your classifier on lots of 10x10 patches to decide if they contain text or not; and also on lots of 20x20 patches), and you will “step” your detector by 2 pixels each time. About how many times will you end up running your classifier on a single 1000x1000 test set image?答案：B
- 1,000,000 
- 500,000
- 100,000
- 250,000

- 
第 2 个问题 Suppose that you just joined a product team that has been developing a machine learning application, using m=1,000 training examples. You discover that you have the option of hiring additional personnel to help collect and label data. You estimate that you would have to pay each of the labellers $10 per hour, and that each labeller can label 4 examples per minute. About how much will it cost to hire labellers to label 10,000 new training examples?答案：D
- $600 
- $250
- $10,000
- $400

- 
第 3 个问题What are the benefits of performing a ceiling analysis? Check all that apply. 答案：AB
- It can help indicate that certain components of a system might not be worth a significant amount of work improving, because even if it had perfect performance its impact on the overall system may be small.
- It helps us decide on allocation of resources in terms of which component in a machine learning pipeline to spend more effort on.
- It is a way of providing additional training data to the algorithm.
- If we have a low-performing component, the ceiling analysis can tell us if that component has a high bias problem or a high variance problem.

- 
Suppose you are building an object classifier, that takes as input an image, and recognizes that image as either containing a car (y=1) or not (y=0). For example, here are a positive example and a negative example: After carefully analyzing the performance of your algorithm, you conclude that you need more positive (y=1) training examples. Which of the following might be a good way to get additional positive examples?答案：A
- Apply translations, distortions, and rotations to the images already in your training set.
- Select two car images and average them to make a third example.
- Take a few images from your training set, and add random, gaussian noise to every pixel.
- Make two copies of each image in the training set; this immediately doubles your training set size.

- 
Suppose you have a PhotoOCR system, where you have the following pipeline: 

You have decided to perform a ceiling analysis on this system, and find the following:Which of the following statements are true?答案：
- If the text detection system was trained using gradient descent, running gradient descent for more iterations is unlikely to help much.
- If we conclude that the character recognition’s errors are mostly due to the character recognition system having high variance, then it may be worth significant effort obtaining additional training data for character recognition.
- We should dedicate significant effort to collecting additional training data for the text detection system.
- The least promising component to work on is the character recognition system, since it is already obtaining 100% accuracy.










