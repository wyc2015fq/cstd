# 统计学假设检验中 p 值的含义具体是什么

701 人赞同了该回答

讲概率、论统计，肯定要从抛硬币说起啊，这才是正确打开姿势嘛。

## **1 什么是假设检验？**

你说你的硬币是公平的，也就是“花”和“字”出现的概率是差不多的。

然后，你想和我打赌，作为一个资深的理智赌徒，我怎能听信你的一面之词，我提出要检查下你的硬币到底是不是公平的，万一是两面“花”怎么办？电影里面不是经常出现这样的桥段？

![img](https://pic2.zhimg.com/50/v2-aafc4dddb4fd9e7a47fb222841fdf06d_hd.jpg)

你神色紧张，死活不让我检查，后来我们提出了折衷的方案，抛几次硬币，看看结果是不是公平的。

总共扔了两次，都是“花”朝上，虽然几率是 ![0.5\times 0.5=0.25](https://www.zhihu.com/equation?tex=0.5%5Ctimes+0.5%3D0.25) ，但是也正常，继续扔。

总共扔了四次，也都是“花”朝上，几率是 ![0.5^4=0.0625](https://www.zhihu.com/equation?tex=0.5%5E4%3D0.0625) ，感觉有点不正常，但是万一是运气呢？继续扔。

总共扔了十次，也都是“花”朝上，那我就认为很可能你这枚硬币不是公平的。

这就是**假设检验**：

- 你提出假设：说你的硬币是公平的
- 我提出要检验你的假设：扔十次，看实验的结果是不是和你的假设相符

## **2 P值**

为了完成假设检验，需要先定义一个概念：P值。我们这里就来解释什么是P值？

根据上面的描述，这里假设检验的思路就是：

- 假设：硬币是公平的
- 检验：认为假设是成立的，然后扔十次，看结果与假设是否相符

反复扔硬币应该符合二项分布（这就不解释了），也就是：

![X\sim B(n,\mu )\\](https://www.zhihu.com/equation?tex=X%5Csim+B%28n%2C%5Cmu+%29%5C%5C)

其中， ![n](https://www.zhihu.com/equation?tex=n) 代表扔硬币的次数， ![\mu ](https://www.zhihu.com/equation?tex=%5Cmu+) 代表“花”朝上的概率。

在我们认为硬币是公平的前提下，扔10次硬币应该符合以下分布：

![X\sim B(10, 0.5)\\](https://www.zhihu.com/equation?tex=X%5Csim+B%2810%2C+0.5%29%5C%5C)

下图表示的就是，假如硬币是公平的情况下的分布图：

![img](https://pic2.zhimg.com/80/v2-47479aec1d4e16520c23c699e81b9d95_hd.jpg)

我扔了十次之后得到的结果是，有八次正面：

![img](https://pic3.zhimg.com/80/v2-6952e115ff5143671e12f0b54e62bc5e_hd.jpg)

这个时候有个数学大佬出来定义了一个称为 ![P](https://www.zhihu.com/equation?tex=P) 值（p-value)的概念：

![img](https://pic2.zhimg.com/80/v2-c2906801d63296c04ed1d9cce09599e4_hd.jpg)罗纳德·艾尔默·费希尔爵士（1890－1962）

把八次正面的概率，与更极端的九次正面、十次正面的概率加起来：

![img](https://pic3.zhimg.com/80/v2-879ff755c31b54a249d60277e297ee4a_hd.jpg)

得到的就是（单侧P值）：

![\text {p-value}=P(8\leq X\leq 10)=0.05\\](https://www.zhihu.com/equation?tex=%5Ctext+%7Bp-value%7D%3DP%288%5Cleq+X%5Cleq+10%29%3D0.05%5C%5C)

其实，出现两次正面、一次正面、零次正面的概率也是很极端的：

![img](https://pic2.zhimg.com/80/v2-f3e94c81ba5e5e9b460628d245735bc9_hd.jpg)

所以（双侧P值）：

![\text {p-value}=P(0\leq X\leq 2)+P(8\leq X\leq 10)=0.1\\](https://www.zhihu.com/equation?tex=%5Ctext+%7Bp-value%7D%3DP%280%5Cleq+X%5Cleq+2%29%2BP%288%5Cleq+X%5Cleq+10%29%3D0.1%5C%5C)

### **2.1 为什么要把更极端的情况加起来？**

根据扔硬币这个例子，可能你会觉得，我知道八次正面出现不正常就行了，干嘛要把九次、十次加起来？

我觉得有这么一个现实原因，比如我要扔1000次硬币来测试假设是否正确。

扔1000次硬币用二项分布来计算很麻烦，根据中心极限定理，我们知道，可以用正态分布来近似：

![img](https://pic2.zhimg.com/80/v2-a3570c77ffb6c1459cf84ed22d89992d_hd.jpg)

比如，我扔了1000次，得到了530次正面，用正态分布来计算就比较简单。

但是，对于正态分布，我没有办法算单点的概率（连续分布单点概率为0），我只能取一个区间来算极限，所以就取530、以及更极端的点组成的区间：

![img](https://pic4.zhimg.com/80/v2-1edee362d8d7c580de010ee7dca8fc6b_hd.jpg)

我上面只取了单侧P值，说明下：

- 取单侧还是双侧，取决于你的应用
- 什么叫做更极端的点，也取决于你的应用

### **3 显著水平**

总共扔10次硬币，那么是出现7次正面之后，可以认为“硬币是不公平的”，还是9次正面之后我才能确认“硬币是不公平的”，这是一个较为主观的标准。

我们一般认为

![\text {p-value}\leq 0.05\\](https://www.zhihu.com/equation?tex=%5Ctext+%7Bp-value%7D%5Cleq+0.05%5C%5C)

就可以认为假设是不正确的。

0.05这个标准就是**显著水平**，当然选择多少作为显著水平也是主观的。

比如，上面的扔硬币的例子，如果取单侧P值，那么根据我们的计算，如果扔10次出现9次正面：

![\text {p-value}=P(9\leq X\leq 10)=0.01\leq 0.05\\](https://www.zhihu.com/equation?tex=%5Ctext+%7Bp-value%7D%3DP%289%5Cleq+X%5Cleq+10%29%3D0.01%5Cleq+0.05%5C%5C)

表示出来如下图所示：

![img](https://pic1.zhimg.com/80/v2-ba3719724c3c2daf135c3006779f3f6f_hd.jpg)

我们可以认为刚开始的假设错的很“显著”，也就是“硬币是不公平的”。

如果扔10次出现出现8次正面：

![\text {p-value}=P(8\leq X\leq 10)=0.05\leq 0.05\\](https://www.zhihu.com/equation?tex=%5Ctext+%7Bp-value%7D%3DP%288%5Cleq+X%5Cleq+10%29%3D0.05%5Cleq+0.05%5C%5C)

呃，这个和我们的显著水平是一样的啊，我们也可以拒绝假设，只是没有那么“显著”了。

### **4 与置信区间的关系**

知识要联系起来看，理解更深刻。

置信区间，目的是根据样本构造一个区间，然后希望这个区间可以把真值包含进去，但是并不知道这个真值是多少？具体可以参考 [如何理解 95% 置信区间？](https://www.zhihu.com/question/26419030/answer/274472266)

而假设检验，则是假设真值是多少，然后检验这个假设是否可能为真。

之所以觉得它们有关系，大概是因为它们都提到了0.05。

它们之间的关系也简单，如果我们提出来的假设 ![\mu _0](https://www.zhihu.com/equation?tex=%5Cmu+_0) 在样本 ![\bar{x}](https://www.zhihu.com/equation?tex=%5Cbar%7Bx%7D) 的置信区间内，就可以通过测试：

![img](https://pic1.zhimg.com/80/v2-6704ace6b06b1ebe0356ca5c23bdae18_hd.jpg)

反之，就不能通过：

![img](https://pic3.zhimg.com/80/v2-64103ea87cbd4d54e046bc164832032d_hd.jpg)



[发布于 2017-12-25](https://www.zhihu.com/question/23149768/answer/282842210)