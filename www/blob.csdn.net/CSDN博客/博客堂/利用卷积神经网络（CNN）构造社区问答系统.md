# 利用卷积神经网络（CNN）构造社区问答系统 - 博客堂 - CSDN博客





2017年06月13日 11:24:13[最小森林](https://me.csdn.net/u012052268)阅读数：710








 问答社区算是一类已经比较成熟的互联网应用了，国外的比如Quora、StackOverflow，国内的比如老派的百度知道，新一代的知乎，都算是代表性的社交类问答社区。问答社区本质上就是个人肉知识库，通过一段时间的积累，会累积相当多以<问题，答案>方式存在的知识。

![](https://img-blog.csdn.net/20151221204000978?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


除了这些通用的问答社区外还有很多垂直领域的问答社区，比如我们畅捷通的会计家园，就是拥有数百万财会人员的知识交流社区，财会人员可以在社区提出自己的一些工作和生活中的疑问，会有很多热心网友或领域专家帮你答疑解惑，会计家园长这个样子：

![](https://img-blog.csdn.net/20151221204048907?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


    这种问答社区往往有信息冗余的问题，就是说历史上已经有不少相同的问题以及答案，但是很多用户并不清楚这点，往往还会问出同样的问题，当然两个问题尽管是同一个问题，但是由于语言表达的灵活性，在字面上看起来可能问题还是有差别，就比如下面两个问题：

问题A：注册资本认缴制下实收资本的账务如何处理?

问题B：认缴制下成立的公司，一开始的账务处理是什么样的，实收资本要做吗？求解

为了能够增加信息的复用率，我们已经使用[自然语言](http://lib.csdn.net/base/nlp)处理、搜索技术以及一些[深度学习](http://lib.csdn.net/base/deeplearning)的技术做了问题推荐系统，在用户提问的时候就将语义相关的问题推荐出来，如果用户看到类似的问题直接看答案就行，所以用户提问的时候看到的这种交互界面：

![](https://img-blog.csdn.net/20151221204150408?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


    但是我们想更进一步，能否在用户问出问题后，直接把答案交给用户？说实话，这其实是[搜索引擎](http://lib.csdn.net/base/searchengine)的近乎终极目标，就是用户提出疑问，直接给出答案。目前搜索引擎的交互方式还是比较原始的，比如用户发出问题，然后人要在搜索结果里面再筛一遍，找到真正自己关心的答案，所以其实是技术+人工的方式。将来的搜索引擎交互方式应该是用户问问题，搜索引擎直接给答案，当然具体体现形式可以有多种，比如目前比较火的聊天[机器人](http://lib.csdn.net/base/robot)本质上就是在往这个目标走的一个中间形式，也就是说是这种方式：

![](https://img-blog.csdn.net/20151221204213832?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


    将来的话，如果全息投影技术普及后，应该就是科幻电影里常见的交互模式，那时候随时随地召唤出全息天使全天候为您服务，您不用担心雾霾天他有没胆出门的问题…..比如以这种附体方式：

![](https://img-blog.csdn.net/20151221204231429?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
嗯，上面是口味比较重的用户的选择，大多数用户可能会更喜欢这种附体方式：

![](https://img-blog.csdn.net/20151221204252040?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


闲话少叙，让我们言归正传并且紧张严肃起来。形式化地说，现在我们面临的是如下问题：

![](https://img-blog.csdn.net/20151221204334652?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




找到与**Qnew**语义相同的问题**Qi**后，将**Qi**对应的答案**Ai**推荐给用户，就完成了用户提出新问题后，直接告诉用户答案的任务。所以这个问题本质上是个问句Paraphrase问题，就是说判断两个句子是否语义等价的问题。

（读者小Y画外音**Qi**：请说人话！

作者小张回答**Ai**：请去京东买本初中数学教材！）



我们用Word Embedding加上卷积神经网络CNN来解决这个问题。CNN不必说了，目前在图像处理领域基本已经横扫，未来两年出现1000层CNN网络叠加起来解决应用问题也不必惊奇。Word
 Embedding更是深度学习在文本处理领域的技术基石。如果现在做应用不用这两样东西估计你出门不太好意思跟同行打招呼吧，为了面子上挂得住，咱得掏出这两把刷子刷刷，以证明咱确实拥有这两把刷子。

一种直观的思路会用两个CNN来解决这个问题，其[架构](http://lib.csdn.net/base/architecture)图如下：

![](https://img-blog.csdn.net/20151221204445517?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


就是说首先把两个要判断语义是否等价的句子转换为Word Embedding形式，作为整个神经网络的输入层，然后CNN1通过卷积层和池化层来抽取出一个句子的语义特征，CNN2抽取出另外一个句子的语义特征，之后两个CNN的池化层拼接起来作为后续三层神经网络的输入层，后续三层神经网络通过隐层对两组语义特征进行非线性变换，最后通过线性层分类输出，得出两个句子是语义相同（比如输出1）或者语义不同（比如输出0）的分类结果。

但是，我们想换种思路来做这个任务，能不能把输入层改造成真正的二维结构，就像一张图片那样，然后套上一个CNN来解决这个问题呢？让我们来试试，首先第一个问题是，给定两个句子SentenceA和SentenceB，如何把CNN的输入层改造成类似图片的二维结构？

在做之前，我们假设两个句子如下：（说明：这个例子只是为了方便画图和举例，真实的训练和[测试](http://lib.csdn.net/base/softwaretest)例子是会计家园的实际问题对，长度大约在10几个字到几十个字左右）

SentenceA：电脑多少钱？

SentenceB：计算机价格？






可以这么做：

Step1：把SentenceA切割成3-Gram表达形式，于是SentenceA变成如下形式

SetA={电脑多，脑多少，多少钱}




Step2：把SentenceB切割成3-Gram表达形式，于是SentenceB变成如下形式

SetB={计算机，算机价，机价格}




       Step3：把SetA的元素作为纵坐标，SetB的元素作为横坐标，将两个输入改造成二维结构，就像下面的图形：

![](https://img-blog.csdn.net/20151221204509888?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


Step4：那么这个矩阵格子里面的值怎么填呢？用横坐标和纵坐标对应的语言片段的语义相似性填充就可以。于是新问题又产生了，给了两个语言片段，比如“多少钱”和“机价格”，如何计算它们的语义相似性呢，请移步看Step5；




Step5：计算两个语言片段的语义相似性。

此时锣鼓点响起，我们的小杀器Word Embedding该粉墨登场了。首先可以用Word2Vec训练出每个汉字的Word
 Embedding，也就是其低维向量表示，一定程度上代表其包含的语义信息。那么3-GRAM包含了三个汉字，这3-GRAM的语义向量Word
 Embedding该怎么表示？可以简单粗暴地把其三个汉字的Word Embedding相应维度上的值累加求和即可，看上去霸王硬上弓包办婚姻，但是其实这是一种通常的做法，一般应用效果还可以。嗯，我们土豪界办事情通常就是这么任性。

这样两个3-GRAM片段对应的Word Embedding都有了，剩下的就简单了，它们两个的语义相似性直接用Cosine计算两个Word
 Embedding在语义空间的向量夹角就成，一般语义越相似，Cosine得分越大。




Step 6：有了Step5的锦囊妙计，就可以完形填空，填充矩阵中对应格子的值了，假设填充完图形如下：

![](https://img-blog.csdn.net/20151221204523737?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


那么类似图片的二维输入结构就完成了。这个矩阵代表什么含义呢？代表的是两个句子任意两个语言片段之间的语义相似性。



有了上面填充好的二维矩阵作为神经网络的输入层，那么后面就简单了，你就当做输入的是个图片，然后直接套上一层或者多层CNN，最后再加上一个全联接分类层就齐活了。改造完的神经网络结构如下：

![](https://img-blog.csdn.net/20151221204604358?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


敲定了网络结构，剩下的就是训练神经网络了。我们利用目前已经做好的问题推荐系统，通过人工找到语义相同表达不同的句子对作为训练集的正例，把一些语义相近但是不同的句子对作为训练集的负例，然后就可以训练这个基于Word Embedding和CNN的神经网络了。

通过实验我们发现，多层CNN并不能带来性能优势，所以最终仍然采用了一层CNN结构。然后用Torch
 7训练模型，调整超参数比如隐层神经元个数，卷积层filter的个数等，最终最优分类精度在90.36%左右，效果还不错，说明祭出CNN这个大杀器和Word
 Embedding这个小杀器还是有效的。当然这跟负例中两个句子对的语义相关性有一定关系，很明显负例句子对语义相关性越高，分类难度越大，后面我们还会不断增加分类难度对模型进行调整。



