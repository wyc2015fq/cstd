# RNN是怎么从单层网络一步步构造的 - 知乎
# 



作者：何之源

来源：[https://zhuanlan.zhihu.com/p/28054589](https://zhuanlan.zhihu.com/p/28054589)

收入《BAT机器学习面试1000题系列》

## **从单层网络谈起**

  在学习RNN之前，首先要了解一下最基本的单层网络，它的结构如图：
![](https://pic3.zhimg.com/v2-1d3d18e501027dafb7916958e466816a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='360'></svg>)
  输入是x，经过变换Wx+b和激活函数f得到输出y。相信大家对这个已经非常熟悉了。




## **经典的RNN结构（N vs N）**

##   在实际应用中，我们还会遇到很多序列形的数据：
![](https://pic1.zhimg.com/v2-8c1b09a112ad918cbf956bf0208759e0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='107'></svg>)
如：
- 自然语言处理问题。x1可以看做是第一个单词，x2可以看做是第二个单词，依次类推。
- 语音处理。此时，x1、x2、x3……是每帧的声音信号。
- 时间序列问题。例如每天的股票价格等等。

  序列形的数据就不太好用原始的神经网络处理了。为了建模序列问题，RNN引入了隐状态h（hidden state）的概念，h可以对序列形的数据提取特征，接着再转换为输出。先从h1的计算开始看：
![](https://pic2.zhimg.com/v2-211571a63180caea6d327dd2416c354d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='233'></svg>)
  图示中记号的含义是：
- 圆圈或方块表示的是向量。
- 一个箭头就表示对该向量做一次变换。如上图中h0和x1分别有一个箭头连接，就表示对h0和x1各做了一次变换。

  在很多论文中也会出现类似的记号，初学的时候很容易搞乱，但只要把握住以上两点，就可以比较轻松地理解图示背后的含义。

  h2的计算和h1类似。要注意的是，在计算时，每一步使用的参数U、W、b都是一样的，也就是说每个步骤的参数都是共享的，这是RNN的重要特点，一定要牢记。
![](https://pic2.zhimg.com/v2-4582b7b9c84dd6bfc304945941704425_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='201'></svg>)



  依次计算剩下来的（使用相同的参数U、W、b）：
![](https://pic4.zhimg.com/v2-99ff11a6da9ef7587164fc44ccff9ec3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='288'></svg>)



  我们这里为了方便起见，只画出序列长度为4的情况，实际上，这个计算过程可以无限地持续下去。

  我们目前的RNN还没有输出，得到输出值的方法就是直接通过h进行计算：
![](https://pic3.zhimg.com/v2-570d3caba7b7b7c2801d01a2f67c2376_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='396'></svg>)
  正如之前所说，一个箭头就表示对对应的向量做一次类似于f(Wx+b)的变换，这里的这个箭头就表示对h1进行一次变换，得到输出y1。

  剩下的输出类似进行（使用和y1同样的参数V和c）：
![](https://pic2.zhimg.com/v2-644d839d6c035e736134c85908aecab1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='473'></svg>)
  OK！大功告成！这就是最经典的RNN结构，我们像搭积木一样把它搭好了。它的输入是x1, x2, .....xn，输出为y1, y2, ...yn，也就是说，输入和输出序列必须要是等长的。

  由于这个限制的存在，经典RNN的适用范围比较小，但也有一些问题适合用经典的RNN结构建模，如：
- 计算视频中每一帧的分类标签。因为要对每一帧进行计算，因此输入和输出序列等长。
- 输入为字符，输出为下一个字符的概率。这就是著名的Char RNN（详细介绍请参考：The Unreasonable Effectiveness of Recurrent Neural Networks，Char RNN可以用来生成文章、诗歌，甚至是代码。此篇博客里有自动生成歌词的实验教程《基于torch学汪峰写歌词、聊天机器人、图像着色/生成、看图说话、字幕生成》[http://blog.csdn.net/v_july_v/article/details/52796239](https://link.zhihu.com/?target=http%3A//blog.csdn.net/v_july_v/article/details/52796239)）。

## **N VS****1**

  有的时候，我们要处理的问题输入是一个序列，输出是一个单独的值而不是序列，应该怎样建模呢？实际上，我们只在最后一个h上进行输出变换就可以了：
![](https://pic4.zhimg.com/v2-246608b5d7e02a697ccc666cb20c17fb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='460'></svg>)
  这种结构通常用来处理序列分类问题。如输入一段文字判别它所属的类别，输入一个句子判断其情感倾向，输入一段视频并判断它的类别等等。

## **1 VS N**

  输入不是序列而输出为序列的情况怎么处理？我们可以只在序列开始进行输入计算：
![](https://pic3.zhimg.com/v2-390646a46efd7f4c29d4ffba8b1bfa6e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='470'></svg>)



  还有一种结构是把输入信息X作为每个阶段的输入：
![](https://pic4.zhimg.com/v2-72b47aea268b2ae866bf8a23df190bf7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='468'></svg>)



  下图省略了一些X的圆圈，是一个等价表示：
![](https://pic2.zhimg.com/v2-8436e9e8a3573268ac3cd2699251b959_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='519'></svg>)



这种1 VS N的结构可以处理的问题有：
- 从图像生成文字（image caption），此时输入的X就是图像的特征，而输出的y序列就是一段句子
- 从类别生成语音或音乐等

## **N vs M**

  下面我们来介绍RNN最重要的一个变种：N vs M。这种结构又叫Encoder-Decoder模型，也可以称之为Seq2Seq模型。

  原始的N vs N RNN要求序列等长，然而我们遇到的大部分问题序列都是不等长的，如机器翻译中，源语言和目标语言的句子往往并没有相同的长度。

  为此，Encoder-Decoder结构先将输入数据编码成一个上下文向量c：
![](https://pic3.zhimg.com/v2-3d735e3ed223c86b6823f6e94936b902_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='391'></svg>)



  得到c有多种方式，最简单的方法就是把Encoder的最后一个隐状态赋值给c，还可以对最后的隐状态做一个变换得到c，也可以对所有的隐状态做变换。

  拿到c之后，就用另一个RNN网络对其进行解码，这部分RNN网络被称为Decoder。具体做法就是将c当做之前的初始状态h0输入到Decoder中：
![](https://pic1.zhimg.com/v2-a69cf71936549b4dd62468adf425c10c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='241'></svg>)



  还有一种做法是将c当做每一步的输入：
![](https://pic3.zhimg.com/v2-8d8e2a86f3c8c7bef806f6f8595c3a32_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='332'></svg>)



  由于这种Encoder-Decoder结构不限制输入和输出的序列长度，因此应用的范围非常广泛，比如：
- 机器翻译。Encoder-Decoder的最经典应用，事实上这一结构就是在机器翻译领域最先提出的
- 文本摘要。输入是一段文本序列，输出是这段文本序列的摘要序列。
- 阅读理解。将输入的文章和问题分别编码，再对其进行解码得到问题的答案。
- 语音识别。输入是语音信号序列，输出是文字序列。
[语音识别技术的前世今生 [CMU王赟博士主讲，参团99]​www.julyedu.com![图标](https://pic4.zhimg.com/v2-8682b14ca9f1b0ae61a58dac8c07ae4f_180x120.jpg)](https://link.zhihu.com/?target=http%3A//www.julyedu.com/course/getDetail/101)![](https://pic1.zhimg.com/v2-57988a3cd81465c1fcce4c48b887d0ec_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='750' height='1334'></svg>)



