# RNN_LSTM网络 - wsp_1138886114的博客 - CSDN博客





2018年09月05日 17:04:54[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：104









- - - [一、RNN（Recurrent Neural Networks）](#一rnnrecurrent-neural-networks)- - [长期依赖（Long-Term Dependencies）问题](#长期依赖long-term-dependencies问题)
- [RNN种类](#rnn种类)


- [二、LSTM 网络](#二lstm-网络)- - [2.1 LSTM 的核心思想](#21-lstm-的核心思想)
- [2.2 逐步理解 LSTM](#22-逐步理解-lstm)







### 一、RNN（Recurrent Neural Networks）

人类并不是每时每刻都从一片空白的大脑开始他们的思考。在你阅读这篇文章时候，你都是基于自己已经拥有的对先前所见词的理解来推断当前词的真实含义。我们不会将所有的东西都全部丢弃，然后用空白的大脑进行思考。我们的思想拥有持久性。 

传统的神经网络并不能做到这点，看起来也像是一种巨大的弊端。RNN 解决了这个问题。RNN 是包含循环的网络，允许信息的持久化。  
![这里写图片描述](https://img-blog.csdn.net/20180807102751673?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

RNN 在语音识别，语言建模，翻译，图片描述等问题上已经取得一定成功 
##### 长期依赖（Long-Term Dependencies）问题

RNN 的关键点之一就是他们可以用来连接先前的信息到当前的任务上。 

例如使用过去的视频段来推测对当前段的理解。但是 CNN 还有很多依赖因素。  
![这里写图片描述](https://img-blog.csdn.net/20180807105152884?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

假设我们试着去预测“I grew up in France… I speak fluent French”最后的词。当前的信息建议下一个词可能是一种语言的名字，需要先前提到的离当前位置很远的 France 的上下文的。相关信息和当前预测位置之间的间隔不断增大时，RNN 会丧失学习到连接如此远的信息的能力。  
![这里写图片描述](https://img-blog.csdn.net/20180807111458565?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在理论上，RNN 绝对可以处理这样的 长期依赖 问题，但是训练 RNN 变得非常困难。  

幸运的是，LSTM 并没有这个问题！ 
##### RNN种类

![这里写图片描述](https://img-blog.csdn.net/20180809152102703?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 二、LSTM 网络

经常接触LSTM，GRU,RNN这些模型，对于LSTM的印象只是知道它用来解决梯度消失梯度爆炸问题，对于长距离的句子的学习效果不好的问题，而且大概知道里面是加了一些参数，加了门单元来选择忘记和记住一些信息。 
- 
**忘记门**： 

作用对象：细胞状态 

作用：**将细胞状态中的信息选择性的遗忘**

让我们回到语言模型的例子中来基于已经看到的预测下一个词。在这个问题中，细胞状态可能包含当前主语的类别，因此正确的代词可以被选择出来。当我们看到新的主语，我们希望忘记旧的主语。 

例如，他今天有事，所以我。。。当处理到‘’我‘’的时候选择性的忘记前面的’他’，或者说减小这个词对后面词的作用。- 
**输入层门**： 

作用对象：细胞状态 

作用：**将新的信息选择性的记录到细胞状态中**

在我们语言模型的例子中，我们希望增加新的主语的类别到细胞状态中，来替代旧的需要忘记的主语。 

例如：他今天有事，所以我。。。。当处理到‘’我‘’这个词的时候，就会把主语我更新到细胞中去。- 
**输出层门**： 

作用对象：隐层ht 

在语言模型的例子中，因为他就看到了一个 代词，可能需要输出与一个 动词 相关的信息。例如，可能输出是否代词是单数还是负数，这样如果是动词的话，我们也知道动词需要进行的词形变化。 

例如：上面的例子，当处理到‘’我‘’这个词的时候，可以预测下一个词，是动词的可能性较大，而且是第一人称。 

会把前面的信息保存到隐层中去。
Gated Recurrent Unit (GRU)就是lstm的一个变态，这是由 Cho, et al. (2014) 提出。它将忘记门和输入门合成了一个单一的 更新门。同样还混合了细胞状态和隐藏状态，和其他一些改动。最终的模型比标准的 LSTM 模型要简单，也是非常流行的变体。 

Long Short Term 网络—— 一般就叫做 LSTM ——是一种 RNN 特殊的类型，可以学习长期依赖信息。LSTM 由Hochreiter & Schmidhuber (1997)提出，并在近期被Alex Graves进行了改良和推广。 

LSTM 通过刻意的设计来避免长期依赖问题。记住长期的信息在实践中是 LSTM 的默认行为，而非需要付出很大代价才能获得的能力！ 

所有 RNN 都具有一种重复神经网络模块的链式的形式。在标准的 RNN 中，这个重复的模块只有一个非常简单的结构，例如一个 tanh 层 
![这里写图片描述](https://img-blog.csdn.net/20180807113131356?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
LSTM 同样是这样的结构，但是重复的模块拥有一个不同的结构。不同于 单一神经网络层，这里是有四个，以一种非常特殊的方式进行交互。  
![这里写图片描述](https://img-blog.csdn.net/20180807113419941?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

LSTM 中的图标： 
![这里写图片描述](https://img-blog.csdn.net/20180807132137613?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
##### 2.1 LSTM 的核心思想

LSTM 的关键就是细胞状态，水平线在图上方贯穿运行。 

细胞状态类似于传送带。直接在整个链上运行，只有一些少量的线性交互。信息在上面流传保持不变会很容易。  
![这里写图片描述](https://img-blog.csdn.net/2018080713471586?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

LSTM 有通过精心设计的称作为“门”的结构来去除或者增加信息到细胞状态的能力。门是一种让信息选择式通过的方法。他们包含一个 sigmoid 神经网络层和一个 pointwise 乘法操作。(如上右图)  

Sigmoid 层输出 0 到 1 之间的数值，描述每个部分有多少量可以通过。0 代表“不许任何量通过”，1 就指“允许任意量通过”！LSTM 拥有三个门，来保护和控制细胞状态。 
##### 2.2 逐步理解 LSTM

**第一步:从细胞状态中丢弃什么信息，由忘记门层完成。**

 该门会读取 $h_{t-1} $和 $x_t$，输出一个在 0 到 1 之间的数值给每个在细胞状态 $C_{t-1}$ 中的数字。1 表示“完全保留”，0 表示“完全舍弃”。 

让我们回到语言模型的例子中来基于已经看到的预测下一个词。在这个问题中，细胞状态可能包含当前主语的性别，因此正确的代词可以被选择出来。  

当我们看到新的主语，我们希望忘记旧的主语。 
![这里写图片描述](https://img-blog.csdn.net/20180807142228493?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

下一步是确定什么样的新信息被存放在细胞状态中。这里包含两个部分。  

第一，sigmoid 层称 “输入门层” 决定什么值我们将要更新。  

然后，一个 tanh 层创建一个新的候选值向量，$\tilde{C}_t$，会被加入到状态中。下一步，我们会讲这两个信息来产生对状态的更新。 

在我们语言模型的例子中，我们希望增加新的主语的性别到细胞状态中，来替代旧的需要忘记的主语  
![这里写图片描述](https://img-blog.csdn.net/20180807143100510?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
现在是更新旧细胞状态的时间了，$C_{t-1}$ 更新为$ C_t$。前面的步骤已经决定了将会做什么，我们现在就是实际去完成。 

我们把旧状态与 $f_t$ 相乘，丢弃掉我们确定需要丢弃的信息。接着加上 $i_t * \tilde{C}_t$。这就是新的候选值，根据我们决定更新每个状态的程度进行变化。 

在语言模型的例子中，这就是我们实际根据前面确定的目标，丢弃旧代词的性别信息并添加新的信息的地方。  
![这里写图片描述](https://img-blog.csdn.net/20180807143438703?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

最终，我们需要确定输出什么值。这个输出将会基于我们的细胞状态，但是也是一个过滤后的版本。首先，我们运行一个 sigmoid 层来确定细胞状态的哪个部分将输出出去。接着，我们把细胞状态通过 tanh 进行处理（得到一个在 -1 到 1 之间的值）并将它和 sigmoid 门的输出相乘，最终我们仅仅会输出我们确定输出的那部分。 

在语言模型的例子中，因为他就看到了一个 代词，可能需要输出与一个 动词 相关的信息。例如，可能输出是否代词是单数还是负数，这样如果是动词的话，我们也知道动词需要进行的词形变化。  
![这里写图片描述](https://img-blog.csdn.net/20180807144957952?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)















