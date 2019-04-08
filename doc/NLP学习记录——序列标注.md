# NLP学习记录——序列标注

2018年02月18日 22:39:04 [浪漫主义AI](https://me.csdn.net/qq_37171771) 阅读数：1655



## tagging problem

即序列标注问题。 
给定一个词序列作为输入：the dog saw a cat。 
要求输出其词性序列：D N V D N （D for a determiner, N for noun, and V for verb）。 
有时输出序列会是这种形式：the/D dog/N saw/V a/D cat/N。 
其中有两个重要具体分支任务：part-of-speech(POS) tagging和named-entity recognition.

#### POS tagging

INPUT: 
Profits soared at Boeing Co., easily topping forecasts on Wall Street, as their CEO Alan Mulally announced first quarter results. 
OUTPUT: 
Profits/N soared/V at/P Boeing/N Co./N ,/, easily/ADV topping/V forecasts/N on/P Wall/N Street/N ,/, as/P their/POSS CEO/N Alan/N Mulally/N announced/V first/ADJ quarter/N results/N ./. 
KEY: 
N = Noun 
V = Verb 
P = Preposition 
Adv = Adverb 
Adj = Adjective 
… 
pos tagging是NLP领域的基础问题之一，在很多领域发挥着重要作用。 
pos tagging的一个难点在于歧义——许多单词可以是不同的pos。上例中的profits是名词，但是在其它地方可能是动词。想到高中政治中学到“人是社会中的人”，同样，“单词是语句中的单词”，单纯从单词本身出发去解决问题是很难的，而考虑上下文后会更容易一些，能够削弱歧义的影响。 
另一个难点在于很多单词出现的频率很低，导致比较难训练。这一点在词向量这一概念出现之后就不再是很严重的问题了，因为就算很多单词很少出现，但是其词向量会跟近义词接近。

#### named entity recognition

例： 
INPUT: Profits soared at Boeing Co., easily topping forecasts on Wall Street, as their CEO Alan Mulally announced first quarter results. 
OUTPUT: Profits soared at [Company Boeing Co.], easily topping forecasts on [Location Wall Street], as their CEO [Person Alan Mulally] announced first quarter results. 
该任务就是要从语句中找出命名实体来，如人名、地名、公司名等。 
实际上，在处理此类任务时，通常会对每一个单词预测一个label： 
INPUT: Profits soared at Boeing Co., easily topping forecasts on Wall Street, as their CEO Alan Mulally announced first quarter results. 
OUTPUT: Profits/NA soared/NA at/NA Boeing/SC Co./CC ,/NA easily/NA topping/NA forecasts/NA on/NA Wall/SL Street/CL ,/NA as/NA their/NA CEO/NA Alan/SP Mulally/CP announced/NA first/NA quarter/NA results/NA ./NA 
KEY: 
NA = No entity 
SC = Start Company 
CC = Continue Company 
SL = Start Location 
CL = Continue Location 
… 
然后对这个半成品输出进行整理就可以得到完整的命名实体。

## generative model

接下来要介绍利用监督学习处理pos tagging的方法，具体为隐马尔可夫模型，在此之前先来介绍生成模型。 
给定训练集：(x1,y1)…(xm,ym)，每一个训练集包括输入x和标注y，我们的任务是寻找一个函数f，使得对于给定的输入x得到尽可能准确的输出y——y=f(x)。 
基于条件概率的判别模型： 
![这里写图片描述](https://img-blog.csdn.net/20180218190403155?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzcxNzE3NzE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70) 
而生成模型是基于联合概率p(x,y)的，并且我们常常会分解联合概率： 
p(x,y)=p(y)p(x|y) 
然后分别对p(y)和p(x|y)进行估计。这样做只是因为求解过程通常会比较便利。 
应用贝叶斯公式，有： 
![这里写图片描述](https://img-blog.csdn.net/2018021819313311?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzcxNzE3NzE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70) 
于是 
![这里写图片描述](https://img-blog.csdn.net/20180218194428297?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzcxNzE3NzE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70) 
这个过程就是，先以概率p(y)选择某种输出y，然后在概率分布p(x|y)上生成x。 
然后让我们把生成模型应用到tagging上。 
假定有限词库V和所有可能tag的集合K，由V中单词组成输入序列x1,x2…xn，由V中单词和对应的K中tag组成输出序列y1,y2…yn，两个序列组成一个序列对，定义所有的序列对的集合为S，那么，generative tagging model即为： 
1.对于S中所有的序列对： 
![这里写图片描述](https://img-blog.csdn.net/20180218195853781?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzcxNzE3NzE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70) 
2.![这里写图片描述](https://img-blog.csdn.net/20180218195918663?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzcxNzE3NzE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70) 
3.![这里写图片描述](https://img-blog.csdn.net/20180218200213603?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzcxNzE3NzE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70) 
可以看到，generative tagging model实际上就是落在S上的一个概率分布，下面介绍求解这个分布的一种方法。

## Trigram Hidden Markov Models

首先定义两个参数： 
q(s|u,v)：在观测到tags u,v之后，下一个tag是s的概率。 
e(x|s)：观测到tag s后，对应位置的输入是x的概率。 
则有： 
![这里写图片描述](https://img-blog.csdn.net/20180218202911448?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzcxNzE3NzE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70) 
其中y(n+1)=STOP，y0=y(-1)=*。这就是trigram HMM模型。 
例如，输入序列为”the dog laughs”，输出tag序列为”D N V STOP”，则： 
![这里写图片描述](https://img-blog.csdn.net/20180218203600707?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzcxNzE3NzE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70) 
其中四个q的乘积即为观测到输出序列为”D N V STOP”的概率，即为p(y)。 
三个e的乘积为输出序列为”D N V STOP”时，输入序列为”the dog laughs”的概率，即为p(x|y)。 
那么，以上模型是怎么来的呢？ 
我们将模型的核心问题写成： 
![这里写图片描述](https://img-blog.csdn.net/20180218205400577?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzcxNzE3NzE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70) 
首先， 
![这里写图片描述](https://img-blog.csdn.net/2018021820545432?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzcxNzE3NzE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70) 
对于前一项。与语言模型中的trigram类似，假设某位置的Y只与前两个Y有关： 
![这里写图片描述](https://img-blog.csdn.net/20180218205614487?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzcxNzE3NzE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70) 
再做一个假设，某位置的X只与序列位置对应的Y有关，则有： 
![这里写图片描述](https://img-blog.csdn.net/20180218210044611?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzcxNzE3NzE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70) 
与语言模型中的概率计算方法类似，依然采取频率估计概率的方法。 
在训练集中，定义c(u,v)为tag序列u v出现的次数，c(u,v,s)为tag序列u v s出现的次数，c(s)为tag s出现的次数，c(s>x)为tag s 与单词x配对的次数，那么： 
![这里写图片描述](https://img-blog.csdn.net/20180218210948390?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzcxNzE3NzE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70) 
具体计算过程中还可以利用一些平滑方法提高准确率。 
最后还有一个问题是如何求得 
![这里写图片描述](https://img-blog.csdn.net/20180218211227373?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzcxNzE3NzE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70) 
课上讲了一种算法叫做Viterbi Algorithm，个人认为没有必要细究，跳过。

## named entity recognition using nerual natwork

以下为cs224n assignment3内容。 
首先考虑基于基于窗口预测的NER： 
![这里写图片描述](https://img-blog.csdn.net/20180218214804636?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzcxNzE3NzE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70) 
设置窗口大小为w，即选取预测中心词加上上下各w个单词的窗口作为输入，输出为命名实体标识。上例中w=1 
这是一个很简单的任务，可直接使用一个简单神经网络去做，并使用cross entropy损失函数： 
![这里写图片描述](https://img-blog.csdn.net/20180218215214944?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzcxNzE3NzE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70) 
也可以使用RNN来做NER： 
![这里写图片描述](https://img-blog.csdn.net/20180218215325969?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzcxNzE3NzE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70) 
每一时刻的输出都是该时刻输入的命名实体标识。 
若同样使用cross entropy损失函数： 
![这里写图片描述](https://img-blog.csdn.net/20180218215453876?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzcxNzE3NzE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70) 
![这里写图片描述](https://img-blog.csdn.net/20180218215514970?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzcxNzE3NzE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70) 
为改善性能，还可以更新cell，换为GRU或者LSTM。 
emmmm。。。显然，用神经网络做要无脑得多。。