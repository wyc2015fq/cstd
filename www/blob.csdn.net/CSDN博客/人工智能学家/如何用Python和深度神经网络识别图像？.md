# 如何用Python和深度神经网络识别图像？ - 人工智能学家 - CSDN博客
2017年12月30日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：1344
*![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBVyKRdq62wSjJKt6FRZxGnv24OGzufLHtt5cWHB7PH1sWRMpaDU2WO5TMtCHnQLOcRBoVv7pWy7Lg/640?wx_fmt=png&wxfrom=5&wx_lazy=1)*
*来源：王树义科学网博客*
*概要：只需要10几行Python代码，你就能自己构建机器视觉模型，对图片做出准确辨识和分类。快来试试吧！*
只需要10几行Python代码，你就能自己构建机器视觉模型，对图片做出准确辨识和分类。快来试试吧！
# **视觉**
进化的作用，让人类对图像的处理非常高效。
这里，我给你展示一张照片。
![0?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBVyKRdq62wSjJKt6FRZxGnvg2JwFvhxN4NKIGvlnYpENch1GzN4fRq3pzib7MY43hsrI5bPwb6PyJQ/0?wx_fmt=jpeg)
如果我这样问你：
你能否分辨出图片中哪个是猫，哪个是狗？
你可能立即会觉得自己遭受到了莫大的侮辱。并且大声质问我：你觉得我智商有问题吗？！
息怒。
换一个问法：
你能否把自己分辨猫狗图片的方法，描述成严格的规则，教给计算机，以便让它替我们人类分辨成千上万张图片呢？
对大多数人来说，此时感受到的，就不是羞辱，而是压力了。
如果你是个有毅力的人，可能会尝试各种判别标准：图片某个位置的像素颜色、某个局部的边缘形状、某个水平位置的连续颜色长度……
你把这些描述告诉计算机，它果然就可以判断出左边的猫和右边的狗了。
问题是，计算机真的会分辨猫狗图片了吗？
我又拿出一张照片给你。
![0?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBVyKRdq62wSjJKt6FRZxGnv51cIvAKLMjVUct6UcIOrI53ONW8HiaA0vjyp6ZdC6tMblPcK251nq3g/0?wx_fmt=jpeg)
你会发现，几乎所有的规则定义，都需要改写。
当机器好不容易可以用近似投机取巧的方法正确分辨了这两张图片里面的动物时，我又拿出来一张新图片……
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBVyKRdq62wSjJKt6FRZxGnvwYVNvjIiaICbaIftB2RQPRrgbQlGBdoyVf9g2xsgIBCPmTSZMajqYsA/0?wx_fmt=png)
几个小时以后，你决定放弃。
别气馁。
人类没法把图片分辨的规则详细、具体而准确地描述给计算机，是不是意味着计算机不能辨识图片呢？
当然不是。
你早已听说过自动驾驶汽车的神奇吧？没有机器对图像的辨识，能做到吗？
你的好友可能（不止一次）给你演示如何用新买的iPhone X做面部识别解锁了吧？没有机器对图像的辨识，能做到吗？
![0?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBVyKRdq62wSjJKt6FRZxGnvVpQe3jiarf0QyYyVaKam881Ot0x1k9RBiaLvht0kgOp1JcxZBoUud8tw/0?wx_fmt=jpeg)
医学领域里，计算机对于科学影像（如X光片）的分析能力，已经超过有多年从业经验的医生了。没有机器对图像的辨识，能做到吗？
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBVyKRdq62wSjJKt6FRZxGnvnvoobhxm269CJ4JIRA0s4g68xqv62YeQaTzqY8bCrnYR33yh6axd6A/0?wx_fmt=png)
你可能一下子觉得有些迷茫了——这难道是奇迹？
不是。
计算机所做的，是学习。
通过学习足够数量的样本，机器可以从数据中自己构建模型。其中，可能涉及大量的判断准则。但是，人类不需要告诉机器任何一条。它是完全自己领悟和掌握的。
你可能会觉得很兴奋。
那么，下面我来告诉你一个更令你兴奋的消息——你自己也能很轻易地构建图片分类系统！
不信？请跟着我下面的介绍，来试试看。
# **数据**
咱们就不辨识猫和狗了，这个问题有点不够新鲜。
咱们来分辨机器猫，好不好？
对，我说的就是哆啦a梦。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBVyKRdq62wSjJKt6FRZxGnvKJ8tsQBDricYiaQeLhSu5oDeqK7icIvcsQSAM27EV1TG5ibxfDibqmbgliaA/0?wx_fmt=png)
把它和谁进行区分呢？
一提到机器人，我立刻就想起来了它。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBVyKRdq62wSjJKt6FRZxGnvFpamFgn4hH0bThoZPerjzha1r0Mu0YLuwiaKwiaTia9cicr7ticT6KI4mSw/0?wx_fmt=png)
对，机器人瓦力（WALLE）。
我给你准备好了119张哆啦a梦的照片，和80张瓦力的照片。图片已经上传到了这个Github项目。
请点击这个链接，下载压缩包。然后在本地解压。作为咱们的演示目录。
解压后，你会看到目录下有个image文件夹，其中包含两个子目录，分别是doraemon和walle。
![0?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBVyKRdq62wSjJKt6FRZxGnvDBT0GqhpRaw8plIIU2SIeFd3VqfJO01QL0oM5N3sKasiaSTrBUN3JNA/0?wx_fmt=jpeg)
打开其中doraemon的目录，我们看看都有哪些图片。
![0?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBVyKRdq62wSjJKt6FRZxGnvXs5u9dcu35G7BXX55XibJhlTVFia6xHVz7ODBLztpVyXnLU4u0niaIwaw/0?wx_fmt=jpeg)
可以看到，哆啦a梦的图片真是五花八门。各种场景、背景颜色、表情、动作、角度……不一而足。
这些图片，大小不一，长宽比例也各不相同。
我们再来看看瓦力，也是类似的状况。
![0?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBVyKRdq62wSjJKt6FRZxGnvMz4A3Eib3N66o7RnMOIh21ib7fWaqicTAiar7Zibszuvfj3lXLcpJGtcMTg/0?wx_fmt=jpeg)
数据已经有了，下面我们来准备一下环境配置。
# **环境**
我们使用Python集成运行环境Anaconda。
请到这个网址 下载最新版的Anaconda。下拉页面，找到下载位置。根据你目前使用的系统，网站会自动推荐给你适合的版本下载。我使用的是macOS，下载文件格式为pkg。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBVyKRdq62wSjJKt6FRZxGnvtmqAKS3I4SdmWAb1dAfs5KIWZXVudicndhGHxvanPibZjUdcqVKdEt0A/0?wx_fmt=png)
下载页面区左侧是Python 3.6版，右侧是2.7版。请选择2.7版本。
双击下载后的pkg文件，根据中文提示一步步安装即可。
![0?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBVyKRdq62wSjJKt6FRZxGnvE56ZbEibuFnp5qtaNOrqtPSoWnRqGvYyvWJgsSv7WYMtrZibU7IMZ8Sg/0?wx_fmt=jpeg)
安装好Anaconda后，我们需要安装TuriCreate。
请到你的“终端”（Linux, macOS）或者“命令提示符”（Windows）下面，进入咱们刚刚下载解压后的样例目录。
执行以下命令，我们来创建一个Anaconda虚拟环境，名字叫做turi。
conda create -n turi python=2.7 anaconda
然后，我们激活turi虚拟环境。
source activate turi
在这个环境中，我们安装最新版的TuriCreate。
pip install -U turicreate
安装完毕后，执行：
jupyter notebook
![0?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBVyKRdq62wSjJKt6FRZxGnv0mmLXWHYeTyVwrasUc6wbeMJuziaajKiaojL46s2t7djVtM38oBylwnQ/0?wx_fmt=jpeg)
这样就进入到了Jupyter笔记本环境。我们新建一个Python 2笔记本。
![0?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBVyKRdq62wSjJKt6FRZxGnvIjQCNQnOsCVM5sX7V0n7dQ1A75Aib8kiadcOicf9K0DFZfcn1e16HjH8Q/0?wx_fmt=jpeg)
这样就出现了一个空白笔记本。
![0?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBVyKRdq62wSjJKt6FRZxGnvzO3icLottsySfMqKtB3VmFx1rRaTicFVLiagNPCJefUFAtLia20icUtttGw/0?wx_fmt=jpeg)
点击左上角笔记本名称，修改为有意义的笔记本名“demo-python-image-classification”。
![0?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBVyKRdq62wSjJKt6FRZxGnvBZtfTZ8IgE27XArX4WicwDh8d9EsxpIKkSDIjng7DjrYNbWgoQq1UlQ/0?wx_fmt=jpeg)
准备工作完毕，下面我们就可以开始编写程序了。
# **代码**
首先，我们读入TuriCreate软件包。它是苹果并购来的机器学习框架，为开发者提供非常简便的数据分析与人工智能接口。
import turicreate as tc
我们指定图像所在的文件夹image。
img_folder = 'image'
前面介绍了，image下，有哆啦a梦和瓦力这两个文件夹。注意如果将来你需要辨别其他的图片（例如猫和狗），请把不同类别的图片也在image中分别存入不同的文件夹，这些文件夹的名称就是图片的类别名(cat和dog)。
然后，我们让TuriCreate读取所有的图像文件，并且存储到data数据框。
data = tc.image_analysis.load_images(img_folder, with_path=True)
这里可能会有错误信息。
Unsupported image format. Supported formats are JPEG and PNG   
 file: /Users/wsy/Dropbox/var/wsywork/learn/demo
本例中提示，有几个.DS_Store文件，TuriCreate不认识，无法当作图片来读取。
这些.DS_Store文件，是苹果macOS系统创建的隐藏文件，用来保存目录的自定义属性，例如图标位置或背景颜色。
我们忽略这些信息即可。
下面，我们来看看，data数据框里面都有什么。
data
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBVyKRdq62wSjJKt6FRZxGnv6EqZozh27e2ib75ial83eNPiavQtZ3uK5FwCCkejVCoeNYqibvMOTeCVOA/0?wx_fmt=png)
可以看到，data包含两列信息，第一列是图片的地址，第二列是图片的长宽描述。
因为我们使用了119张哆啦a梦图片，80张瓦力图片，所以总共的数据量是199条。数据读取完整性验证通过。
下面，我们需要让TuriCreate了解不同图片的标记（label）信息。也就是，一张图片到底是哆啦a梦，还是瓦力呢？
这就是为什么一开始，你就得把不同的图片分类保存到不同的文件夹下面。
此时，我们利用文件夹名称，来给图片打标记。
data['label'] = data['path'].apply(lambda path: 'doraemon'if'doraemon'in path else'walle')
这条语句，把doraemon目录下的图片，在data数据框里打标记为doraemon。反之就都视为瓦力（walle）。
我们来看看标记之后的data数据框。
data
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBVyKRdq62wSjJKt6FRZxGnva2kMM4QicjuQPovEvqJdc2xLJIQdJgIYibDUicwIibQLicOhMEkibybJXqzQ/0?wx_fmt=png)
可以看到，数据的条目数量（行数）是一致的，只是多出来了一个标记列（label），说明图片的类别。
我们把数据存储一下。
data.save('doraemon-walle.sframe')
这个存储动作，让我们保存到目前的数据处理结果。之后的分析，只需要读入这个sframe文件就可以了，不需要从头去跟文件夹打交道了。
从这个例子里，你可能看不出什么优势。但是想象一下，如果你的图片有好几个G，甚至几个T，每次做分析处理，都从头读取文件和打标记，就会非常耗时。
我们深入探索一下数据框。
TuriCreate提供了非常方便的explore()函数，帮助我们直观探索数据框信息。
data.explore()
这时候，TuriCreate会弹出一个页面，给我们展示数据框里面的内容。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBVyKRdq62wSjJKt6FRZxGnvcVNEcPdltxbqicAEjr7icuBABEMPnVBzwVFZqzPusYmlYyDXN4ibKf7iaQ/0?wx_fmt=png)
原先打印data数据框，我们只能看到图片的尺寸，此时却可以浏览图片的内容。
如果你觉得图片太小，没关系。把鼠标悬停在某张缩略图上面，就可以看到大图。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBVyKRdq62wSjJKt6FRZxGnvJTrCgcAUKdsMw3TpEjbz4XZ4WBmCUWBVEZAPLEvXhkbtDBQQiaAe0fg/0?wx_fmt=png)
数据框探索完毕。我们回到notebook下面，继续写代码。
这里我们让TuriCreate把data数据框分为训练集合和测试集合。
train_data, test_data = data.random_split(0.8, seed=2)
训练集合是用来让机器进行观察学习的。电脑会利用训练集合的数据自己建立模型。但是模型的效果（例如分类的准确程度）如何？我们需要用测试集来进行验证测试。
这就如同老师不应该把考试题目都拿来给学生做作业和练习一样。只有考学生没见过的题，才能区分学生是掌握了正确的解题方法，还是死记硬背了作业答案。
我们让TuriCreate把80%的数据分给了训练集，把剩余20%的数据拿到一边，等待测试。这里我设定了随机种子取值为2，这是为了保证数据拆分的一致性。以便重复验证我们的结果。
好了，下面我们让机器开始观察学习训练集中的每一个数据，并且尝试自己建立模型。
下面代码第一次执行的时候，需要等候一段时间。因为TuriCreate需要从苹果开发者官网上下载一些数据。这些数据大概100M左右。
需要的时长，依你和苹果服务器的连接速度而异。反正在我这儿，下载挺慢的。
好在只有第一次需要下载。之后的重复执行，会跳过下载步骤。
model = tc.image_classifier.create(train_data, target='label')
下载完毕后，你会看到TuriCreate的训练信息。
Resizing images...
Performing feature extraction on resized images...
Completed 168/168
PROGRESS: Creating a validation setfrom5percentof training data. This may take a while.
          You can set``validation_set=None``todisablevalidation tracking.
你会发现，TuriCreateh会帮助你把图片进行尺寸变换，并且自动抓取图片的特征。然后它会从训练集里面抽取5%的数据作为验证集，不断迭代寻找最优的参数配置，达到最佳模型。
这里可能会有一些警告信息，忽略就可以了。
当你看到下列信息的时候，意味着训练工作已经顺利完成了。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBVyKRdq62wSjJKt6FRZxGnvz56GGE3s43mlbmqUdJV3Pp3ZFJ9lHHroWlM4gt9YMbF0F5NeKD24Iw/0?wx_fmt=png)
可以看到，几个轮次下来，不论是训练的准确度，还是验证的准确度，都已经非常高了。
下面，我们用获得的图片分类模型，来对测试集做预测。
predictions = model.predict(test_data)
我们把预测的结果（一系列图片对应的标记序列）存入了predictions变量。
然后，我们让TuriCreate告诉我们，在测试集上，我们的模型表现如何。
先别急着往下看，猜猜结果正确率大概是多少？从0到1之间，猜测一个数字。
猜完后，请继续。
metrics = model.evaluate(test_data)
print(metrics['accuracy'])
这就是正确率的结果：
0.967741935484
我第一次看见的时候，震惊不已。
我们只用了100多个数据做了训练，居然就能在测试集（机器没有见过的图片数据）上，获得如此高的辨识准确度。
为了验证这不是准确率计算部分代码的失误，我们来实际看看预测结果。
predictions
这是打印出的预测标记序列：
dtype: str
Rows: 31
['doraemon', 'doraemon', 'doraemon', 'doraemon', 'walle', 'doraemon', 'walle', 'doraemon', 'walle', 'walle', 'doraemon', 'doraemon', 'doraemon', 'doraemon', 'doraemon', 'walle', 'doraemon', 'doraemon', 'walle', 'walle', 'doraemon', 'doraemon', 'walle', 'walle', 'walle', 'doraemon', 'doraemon', 'walle', 'walle', 'doraemon', 'walle']
再看看实际的标签。
test_data['label']
这是实际标记序列：
dtype: str
Rows: 31
['doraemon', 'doraemon', 'doraemon', 'doraemon', 'walle', 'doraemon', 'walle', 'walle', 'walle', 'walle', 'doraemon', 'doraemon', 'doraemon', 'doraemon', 'doraemon', 'walle', 'doraemon', 'doraemon', 'walle', 'walle', 'doraemon', 'doraemon', 'walle', 'walle', 'walle', 'doraemon', 'doraemon', 'walle', 'walle', 'doraemon', 'walle']
我们查找一下，到底哪些图片预测失误了。
你当然可以一个个对比着检查。但是如果你的测试集有成千上万的数据，这样做效率就会很低。
我们分析的方法，是首先找出预测标记序列（predictions）和原始标记序列（test_data['label']）之间有哪些不一致，然后在测试数据集里展示这些不一致的位置。
test_data[test_data['label'] != predictions]
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBVyKRdq62wSjJKt6FRZxGnvicXv3uCo38HDyWf3Zo7EmgtIwjIs6LyibZvoakkyGanft8NcjPMPvwUw/0?wx_fmt=png)
我们发现，在31个测试数据中，只有1处标记预测发生了失误。原始的标记是瓦力，我们的模型预测结果是哆啦a梦。
我们获得这个数据点对应的原始文件路径。
wrong_pred_img_path = test_data[predictions != test_data['label']][0]['path']
然后，我们把图像读取到img变量。
img = tc.Image(wrong_pred_img_path)
用TuriCreate提供的show()函数，我们查看一下这张图片的内容。
img.show()
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBVyKRdq62wSjJKt6FRZxGnvrYB4TO3PRBetlNQcPIvDR6rUokiaiaCUxEoPJDqUDM6o6MgPjgL0HhRw/0?wx_fmt=png)
因为深度学习的一个问题在于模型过于复杂，所以我们无法精确判别机器是怎么错误辨识这张图的。但是我们不难发现这张图片有些特征——除了瓦力以外，还有另外一个机器人。
如果你看过这部电影，应该知道两个机器人之间的关系。这里我们按下不表。问题在于，这个右上方的机器人圆头圆脑，看上去与棱角分明的瓦力差别很大。但是，别忘了，哆啦a梦也是圆头圆脑的。
# **原理**
按照上面一节的代码执行后，你应该已经了解如何构建自己的图片分类系统了。在没有任何原理知识的情况下，你研制的这个模型已经做得非常棒了。不是吗？
如果你对原理不感兴趣，请跳过这一部分，看“小结”。
如果你对知识喜欢刨根问底，那咱们来讲讲原理。
虽然不过写了10几行代码，但是你构建的模型却足够复杂和高大上。它就是传说中的卷积神经网络（Convolutional Neural Network, CNN）。
它是深度机器学习模型的一种。最为简单的卷积神经网络大概长这个样子：
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBVyKRdq62wSjJKt6FRZxGnvcFSmsWk0FxyyvcqKwFxO3jDsB1SMibLeLqMhnfgEiaJvub7p8Vd2AM8Q/0?wx_fmt=png)
最左边的，是输入层。也就是咱们输入的图片。本例中，是哆啦a梦和瓦力。
在计算机里，图片是按照不同颜色（RGB，即Red, Green, Blue）分层存储的。就像下面这个例子。
![0?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBVyKRdq62wSjJKt6FRZxGnvlDjWfCJozg0gHjEmiaBSwmj5wECnfW2vic2avzhsvZb0I8RR8btjLNbA/0?wx_fmt=jpeg)
根据分辨率不同，电脑会把每一层的图片存成某种大小的矩阵。对应某个行列位置，存的就是个数字而已。
下面这张动图，很形象地为你解释了这一过程。
![0?wx_fmt=gif](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_gif/f84kJBXzrBVyKRdq62wSjJKt6FRZxGnv8WBESaWyxiaV49pxXDJ8bggOYTP9DUBtB0zyvlNy1oLQAm6hicEEpusw/0?wx_fmt=gif)
这个过程，就是不断从一个矩阵上去寻找某种特征。这种特征可能是某个边缘的形状之类。
再下一层，叫做“池化层”（Pooling Layer）。这个翻译简直让人无语。我觉得翻译成“汇总层”或者“采样层”都要好许多。下文中，我们称其为“采样层”。
采样的目的，是避免让机器认为“必须在左上角的方格位置，有一个尖尖的边缘”。实际上，在一张图片里，我们要识别的对象可能发生位移。因此我们需要用汇总采样的方式模糊某个特征的位置，将其从“某个具体的点”，扩展成“某个区域”。
如果这样说，让你觉得不够直观，请参考下面这张动图。
![0?wx_fmt=gif](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_gif/f84kJBXzrBVyKRdq62wSjJKt6FRZxGnvoV0NzXKZZL5T9mcQfynsH5VaiaOd5wtbCLFdA9qeG2Rgwd0ujcQJicxQ/0?wx_fmt=gif)
这里使用的是“最大值采样”（Max-Pooling）。以原先的2x2范围作为一个分块，从中找到最大值，记录在新的结果矩阵里。
一个有用的规律是，随着层数不断向右推进，一般结果图像（其实正规地说，应该叫做矩阵）会变得越来越小，但是层数会变得越来越多。
只有这样，我们才能把图片中的规律信息抽取出来，并且尽量掌握足够多的模式。
回顾我们的示例图：
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBVyKRdq62wSjJKt6FRZxGnvcFSmsWk0FxyyvcqKwFxO3jDsB1SMibLeLqMhnfgEiaJvub7p8Vd2AM8Q/0?wx_fmt=png)
下一层叫做全连接层（Fully Connected Layer），它其实就是把上一层输出的若干个矩阵全部压缩到一维，变成一个长长的输出结果。
之后是输出层，对应的结果就是我们需要让机器掌握的分类。
如果只看最后两层，你会很容易把它跟之前学过的深度神经网络（Deep Neural Network, DNN）联系起来。
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBVyKRdq62wSjJKt6FRZxGnvCws6ujZcoRTdOg1mKZSnYfibIoYt9vqV4uicTSiaT2VUXssB7w1wjstpQ/0?wx_fmt=png)
既然我们已经有了深度神经网络，为什么还要如此费力去使用卷积层和采样层，导致模型如此复杂呢？
这里出于两个考虑：
首先是计算量。图片数据的输入量一般比较大，如果我们直接用若干深度神经层将其连接到输出层，则每一层的输入输出数量都很庞大，总计算量是难以想像的。
其次是模式特征的抓取。即便是使用非常庞大的计算量，深度神经网络对于图片模式的识别效果也未必尽如人意。因为它学习了太多噪声。而卷积层和采样层的引入，可以有效过滤掉噪声，突出图片中的模式对训练结果的影响。
你可能会想，咱们只编写了10几行代码而已，使用的卷积神经网络一定跟上图差不多，只有4、5层的样子吧？
不是这样的，你用的层数，有足足50层呢！
它的学名，叫做Resnet-50，是微软的研发成果，曾经在2015年，赢得过ILSRVC比赛。在ImageNet数据集上，它的分类辨识效果，已经超越人类。
![0?wx_fmt=jpeg](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBVyKRdq62wSjJKt6FRZxGnvhdvZFmVzH2a2DliautKGLglQjPRNBZXMwliagQnrOL87hdFP2WibDVPRA/0?wx_fmt=jpeg)
请看上图中最下面的那一个，就是它的大略样子。
足够深度，足够复杂吧。
# **小结**
通过本文，你已掌握了以下内容：
- 
如何在Anaconda虚拟环境下，安装苹果公司的机器学习框架TuriCreate。
- 
如何在TuriCreate中读入文件夹中的图片数据。并且利用文件夹的名称，给图片打上标记。
- 
如何在TuriCreate中训练深度神经网络，以分辨图片。
- 
如何利用测试数据集，检验图片分类的效果。并且找出分类错误的图片。
- 
卷积神经网络（Convolutional Neural Network, CNN）的基本构成和工作原理。
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。由互联网进化论作者，计算机博士刘锋与中国科学院虚拟经济与数据科学研究中心石勇、刘颖教授创建。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
