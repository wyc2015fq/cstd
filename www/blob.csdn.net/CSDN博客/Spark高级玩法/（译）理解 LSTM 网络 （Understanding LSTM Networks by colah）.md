# （译）理解 LSTM 网络 （Understanding LSTM Networks by colah） - Spark高级玩法 - CSDN博客
2018年04月22日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：145
**前言**：其实之前就已经用过 LSTM 了，是在深度学习框架 keras 上直接用的，但是到现在对LSTM详细的网络结构还是不了解，心里牵挂着难受呀！今天看了 tensorflow 文档上面推荐的这篇博文，看完这后，焕然大悟，对 LSTM 的结构理解基本上没有太大问题。此博文写得真真真好！！！为了帮助大家理解，也是怕日后自己对这些有遗忘的话可以迅速回想起来，所以打算对原文写个翻译。首先声明，由于本人水平有限，如有翻译不好或理解有误的多多指出！此外，本译文也不是和原文一字一句对应的，为了方便理解可能会做一些调整和修改。）
人们思考问题往往不是从零开始的。就好像你现在阅读这篇文章一样，你对每个词的理解都会依赖于你前面看到的一些词，而不是把你前面看的内容全部抛弃了，忘记了，再去理解这个单词。也就是说，人们的思维总是会有延续性的。
传统的神经网络是做不到这样的延续性（它们没办法保留对前文的理解），这似乎成了它们一个巨大的缺陷。举个例子，在观看影片中，你想办法去对每一帧画面上正在发生的事情做一个分类理解。目前还没有明确的办法利用传统的网络把对影片中前面发生的事件添加进来帮助理解后面的画面。
但是，循环神经网络可以做到。在RNNs的网络中，有一个循环的操作，使得它们能够保留之前学习到的内容。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFX3yUjVq9EGzNLT7JUibKMYpDS8EoVCItxt25ykgPAuqISE7qicdxLiaLasPSXV6ht2XvKyOUaWibPibQg/640?wx_fmt=png)
Fig1.  RNNs 网络结构 
在上图网络结构中，对于矩形块 A 的那部分，通过输入xt（t时刻的特征向量），它会输出一个结果ht（t时刻的状态或者输出）。网络中的循环结构使得某个时刻的状态能够传到下一个时刻。（译者注：因为当前时刻的状态会作为下一时刻输入的一部分）
这些循环的结构让 RNNs 看起来有些难以理解。但是，你稍微想一下就会发现，这似乎和普通的神经网络有不少相似之处呀。我们可以把 RNNs 看成是一个普通的网络做了多次复制后叠加在一起组成的。每一网络会把它的输出传递到下一个网络中。我们可以把 RNNs 在时间步上进行展开，就得到下图这样：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFX3yUjVq9EGzNLT7JUibKMYpXfk5utAEGhha01gFhibvrOacm0uyQv1K6HxibR2HGbxFNyVpTLXpykiag/640?wx_fmt=png)
fig2. RNNs 展开网络结构
从 RNNs 链状的结构很容易理解到它是和序列信息相关的。这种结构似乎生来就是为了解决序列相关问题的。
而且，它们的的确确非常管用！在最近的几年中，人们利用 RNNs 不可思议地解决了各种各样的问题：语音识别，语言模型，翻译，图像（添加）字幕，等等。关于RNNs在这些方面取得的惊人成功，我们可以看 Andrej Karpathy 的博客： The Unreasonable Effectiveness of Recurrent Neural Networks.
RNNs 能够取得这样的成功，主要还是 LSTMs 的使用。这是一种比较特殊的 RNNs，而且对于很多任务，它比普通的 RNNs 效果要好很多很多！基本上现在所使用的循环神经网络用的都是 LSTMs，这也正是本文后面所要解释的网络。
RNNs 的出现，主要是因为它们能够把以前的信息联系到现在，从而解决现在的问题。比如，利用前面的画面，能够帮助我们理解当前画面的内容。如果 RNNs 真的可以做到这个，那么它肯定是对我们的任务有帮助的。但是它真的可以 做到吗，恐怕还得看实际情况呀！
有时候，我们在处理当前任务的时候，只需要看一下比较近的一些信息。比如在一个语言模型中，我们要通过上文来预测一下个词可能会是什么，那么当我们看到“ the clouds are in the ?”时，不需要更多的信息，我们就能够自然而然的想到下一个词应该是“sky”。在这样的情况下，我们所要预测的内容和相关信息之间的间隔很小，这种情况下 RNNs 就能够利用过去的信息， 很容易的实现。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFX3yUjVq9EGzNLT7JUibKMYpmTCU4on8ydOgx6cDXdKIyewVqOQx26Wm6EsaLrOTWgo356FqWzTzXg/640?wx_fmt=png)
fig2. 短期依赖
但是，有些情况是需要更多的上下文信息。比如我们要预测“I grew up in France … (此处省略1万字)… I speak ?”这个预测的词应该是 Franch，但是我们是要通过很长很长之前提到的信息，才能做出这个正确的预测的呀，普通的 RNNs 很难做到这个。
随着预测信息和相关信息间的间隔增大， RNNs 很难去把它们关联起来了。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFX3yUjVq9EGzNLT7JUibKMYpfTMUsk9W2aOy8c4W7t8XTQXLsuNp4WoK8hZ2lDY9Asg3KjwO8m6gAA/640?wx_fmt=png)
fig3. 长期依赖
从理论上来讲，通过选择合适的参数，RNNs 确实是可以把这种长时期的依赖关系（“long-term dependencies”） 联系起来，并解决这类问题的。但遗憾的是在实际中， RNNs 无法解决这个问题。 Hochreiter (1991) [German] 和 Bengio, et al. (1994) 曾经对这个问题进行过深入的研究，发现 RNNs 的确很难解决这个问题。
但是非常幸运，LSTMs 能够帮我们解决这个问题。
长短期记忆网络（Long Short Term Memory networks） - 通常叫做 “LSTMs” —— 是 RNN 中一个特殊的类型。由Hochreiter & Schmidhuber (1997)提出，广受欢迎，之后也得到了很多人们的改进调整。LSTMs 被广泛地用于解决各类问题，并都取得了非常棒的效果。
明确来说，设计 LSTMs 主要是为了避免前面提到的 长时期依赖 （long-term dependency ）的问题。它们的本质就是能够记住很长时期内的信息，而且非常轻松就能做到。
所有循环神经网络结构都是由完全相同结构的（神经网络）模块进行复制而成的。在普通的RNNs 中，这个模块结构非常简单，比如仅是一个单一的 tanh 层。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFX3yUjVq9EGzNLT7JUibKMYpnQy9ibVSotZEVo5ia0iakQSpRVlWNG7aU4f7PjG82hMViads5ibayLj6V7g/640?wx_fmt=png)
fig4. 普通 RNNs 内部结构
LSTMs 也有类似的结构（译者注：唯一的区别就是中间部分）。但是它们不再只是用一个单一的 tanh 层，而是用了四个相互作用的层。 
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFX3yUjVq9EGzNLT7JUibKMYpEw5ic4v6Yo0Sjv8FfSgKVfBlnlsF17tXa38cblr7d8fJQ1m3wBszoJg/640?wx_fmt=png)
fig5. LSTM 内部结构
别担心，别让这个结构给吓着了，下面根据这个结构，我们把它解剖开，一步一步地来理解它（耐心看下去，你一定可以理解的）。现在，我们先来定义一下用到的符号： 
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFX3yUjVq9EGzNLT7JUibKMYp9444ic542EGuGJIInUQtpEm9sxSxUbvUsUQ12JmoTXmX7ls482ibYXPg/640?wx_fmt=png)
fig6. 符号说明
在网络结构图中，每条线都传递着一个向量，从一个节点中输出，然后输入到另一个节点中。粉红色的圆圈表示逐点操作，比如向量相加；黄色的矩形框表示的是一个神经网络层（就是很多个神经节点）；合并的线表示把两条线上所携带的向量进行合并（比如一个带 1ht−1,另一个带 xt , 那么合并后的输出就是1[ht−1,xt]）; 分开的线表示将线上传递的向量复制一份，传给两个地方。
## **3.1 LSTMs 的核心思想**
LSTMs 最关键的地方在于 cell（整个绿色的框就是一个 cell） 的状态 和 结构图上面的那条横穿的水平线。
cell 状态的传输就像一条传送带，向量从整个 cell 中穿过，只是做了少量的线性操作。这种结构能够很轻松地实现信息从整个 cell 中穿过而不做改变。（译者注：这样我们就可以实现了长时期的记忆保留了） 
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFX3yUjVq9EGzNLT7JUibKMYpyfdicPQwTnPNVA01fI0U11w6sKYhmppvj6kH8bQ2WzD2icrNv3GJ8HUQ/640?wx_fmt=png)
fig7.  传送带结构
若只有上面的那条水平线是没办法实现添加或者删除信息的。而是通过一种叫做 **门（gates）** 的结构来实现的。
**门** 可以实现选择性地让信息通过，主要是通过一个 sigmoid 的神经层 和一个逐点相乘的操作来实现的。 
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFX3yUjVq9EGzNLT7JUibKMYpoNBiaCWpjAruYk6fFfoKUiboX8l3VoZ8E38fuuibtQTxXJ0ZAicRKy4ouA/640?wx_fmt=png)
fig8.  门结构（sigmoid 层）
sigmoid 层输出（是一个向量）的每个元素都是一个在 0 和 1 之间的实数，表示让对应信息通过的权重（或者占比）。比如， 0 表示“不让任何信息通过”， 1 表示“让所有信息通过”。
每个 LSTM 有三个这样的门结构，来实现保护和控制信息。（译者注：分别是 “forget gate layer”, 遗忘门； “input gate layer”，传入门； “output gate layer”, 输出门）
## **3.2 逐步理解 LSTM**
（好了，终于来到最激动的时刻了）
### **3.2.1 遗忘门**
首先是 LSTM 要决定让那些信息继续通过这个 cell，这是通过一个叫做“forget gate layer ”的sigmoid 神经层来实现的。它的输入是1ht−1和xt，输出是一个数值都在 0，1 之间的向量（向量长度和 cell 的状态 1Ct−1 一样），表示让 1Ct−1 的各部分信息通过的比重。 0 表示“不让任何信息通过”， 1 表示“让所有信息通过”。
回到我们上面提到的语言模型中，我们要根据**所有**的上文信息来预测下一个词。这种情况下，每个 cell 的状态中都应该包含了当前主语的性别信息（保留信息），这样接下来我们才能够正确地使用代词。但是当我们又开始描述一个新的主语时，就应该把上文中的主语性别给忘了才对(忘记信息)。 
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFX3yUjVq9EGzNLT7JUibKMYpqd19xET3ySOPHhJEXyuSGU2om3zNQLY7NPFibMn0GRyKaSKZibK2bdGw/640?wx_fmt=png)
fig9.  遗忘门 (forget gates)
### **3.2.2 传入门**
下一步是决定让多少新的信息加入到 cell 状态 中来。实现这个需要包括两个 步骤：首先，一个叫做“input gate layer ”的 sigmoid 层决定哪些信息需要更新；一个 tanh 层生成一个向量，也就是备选的用来更新的内容，Ct~ 。在下一步，我们把这两部分联合起来，对 cell 的状态进行一个更新。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFX3yUjVq9EGzNLT7JUibKMYpNdP03EMEsxp2b6VAobnpkAn50Ual56aJ5h05yIMDpHuhuQzhtlib4UQ/640?wx_fmt=png)
fig10.  传入门 (input gates)
在我们的语言模型的例子中，我们想把新的主语性别信息添加到 cell 状态中，来替换掉老的状态信息。 
有了上述的结构，我们就能够更新 cell 状态了， 即把1Ct−1更新为 Ct。 从结构图中应该能一目了然， 首先我们把旧的状态 1Ct−1和ft相乘， 把一些不想保留的信息忘掉。然后加上it∗Ct~。这部分信息就是我们要添加的新内容。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFX3yUjVq9EGzNLT7JUibKMYpMIe7owRGKOViaiaj6duicIo5B86iaBUOX6YG0bxQRicu3hOtLxb2IlSuWgQ/640?wx_fmt=png)
fig11.  更新 cell 状态
### **3.2.3 输出门**
最后，我们需要来决定输出什么值了。这个输出主要是依赖于 cell 的状态Ct，但是又不仅仅依赖于 Ct，而是需要经过一个过滤的处理。首先，我们还是使用一个 sigmoid 层来（计算出）决定Ct中的哪部分信息会被输出。接着，我们把Ct通过一个 tanh 层（把数值都归到 -1 和 1 之间），然后把 tanh  层的输出和 sigmoid 层计算出来的权重相乘，这样就得到了最后输出的结果。
在语言模型例子中，假设我们的模型刚刚接触了一个代词，接下来可能要输出一个动词，这个输出可能就和代词的信息相关了。比如说，这个动词应该采用单数形式还是复数的形式，那么我们就得把刚学到的和代词相关的信息都加入到 cell 状态中来，才能够进行正确的预测。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFX3yUjVq9EGzNLT7JUibKMYp9ab0cETbg7phWTxLqLDPa8GKll82d5QaIuOdZ7rl7BE2S5o1Qro9Pg/640?wx_fmt=png)
fig12.  cell 输出
原文这部分介绍了 LSTM 的几个变种，还有这些变形的作用。在这里我就不再写了。有兴趣的可以直接阅读原文。
下面主要讲一下其中比较著名的变种 GRU（Gated Recurrent Unit ），这是由 Cho, et al. (2014) 提出。在 GRU 中，如 fig.13 所示，只有两个门：重置门（reset gate）和更新门（update gate）。同时在这个结构中，把细胞状态和隐藏状态进行了合并。最后模型比标准的 LSTM 结构要简单，而且这个结构后来也非常流行。 
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFX3yUjVq9EGzNLT7JUibKMYpoAY4Dib2dPXPbLCpIvrXBeSO01Ff9DJ8mibYj4X9lWCstgicwOe0weUuw/640?wx_fmt=png)
fig13.  GRU结构
其中， rt 表示重置门，zt 表示更新门。**重置门决定是否将之前的状态忘记。（作用相当于合并了 LSTM 中的遗忘门和传入门）**当 rt 趋于 0 的时候，前一个时刻的状态信息 1ht−1 会被忘掉，隐藏状态 ht~ 会被重置为当前输入的信息。**更新门决定是否要将隐藏状态更新为新的状态**ht~**（作用相当于 LSTM 中的输出门）** 。
和 LSTM 比较一下：
- 
(1) GRU 少一个门，同时少了细胞状态 Ct。
- 
(2) 在 LSTM 中，通过遗忘门和传入门控制信息的保留和传入；GRU 则通过重置门来控制是否要保留原来隐藏状态的信息，但是不再限制当前信息的传入。
- 
(3) 在 LSTM 中，虽然得到了新的细胞状态 Ct，但是还不能直接输出，而是需要经过一个过滤的处理： ht=ot∗tanh(Ct)； 同样，在 GRU 中, 虽然 (2) 中我们也得到了新的隐藏状态 ht~， 但是还不能直接输出，而是通过更新门来控制最后的输出： 11ht=(1−zt)∗ht−1+zt∗ht~ 。
**后记：**好了，到这里对一般形式的 LSTM 的结构讲解已经结束了，原文后面对 LSTM 的各种变形讲解也比较简单，在这里我就不再写了，有兴趣的可以直接阅读原文。上面我结合了原论文比较详细地介绍了一下 GRU，个人水平有限，难免也会出错。其实英语理解能力还行的话建议还是阅读原文比较好，有些东西翻译过来连我自己都不知道该怎么表达了，如果您觉得有什么地方不对，欢迎指出。翻译完后才发现网上已经有很多翻译的版本了，所以参考那些版本又做了一些调整，主要是参考了[译] 理解 LSTM 网络这篇文章。后面如果有时间的话，我应该会写个用 TensorFlow 来实现 LSTM 的例子，敬请期待，哈哈哈！
**推荐阅读：**
1，[【深度学习】③--神经网络细节与训练注意点](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484444&idx=1&sn=020832bbc1ab7c45522d2fd0ca6ad638&chksm=9f38e734a84f6e22d050e25ce4bbc9383b13b8f2c91c43248c7953bbe125ee842a1f323cf33a&scene=21#wechat_redirect)
2，[【深度学习】⑤--自然语言处理的相关应用](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484458&idx=1&sn=670207cb6fa965bb39ec18726ef03262&chksm=9f38e702a84f6e147fa59f71afcd42b72d874d84c1dac264bf02ddbe894c06597a8376d71450&scene=21#wechat_redirect)
3，[案例：Spark基于用户的协同过滤算法](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484291&idx=1&sn=4599b4e31c2190e363aa379a92794ace&chksm=9f38e0aba84f69bd5b78b48e31b3f5b3792ec40e2d25fdbe6bc735f9c98ceb4584462b08e439&scene=21#wechat_redirect)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640?wx_fmt=png)
