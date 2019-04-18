# 自信息, 信息熵, 互信息和K-L散度 - happyhorizon的算法天空 - CSDN博客
2017年09月08日 14:10:00[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：2686

# 香农-信息论领域的牛顿
香农一生发表的文章并不多，但是篇篇都是精品。
Amethematical theory of communication通信的数学理论
第一篇文章中提出了比特（bit）的概念。比特究竟测量的是什么呢？香农的回答是：用于测量信息的单位。在香农眼里，信息是和长度、重量这些物理量一样，是一种可以测量和规范的东西。由于对于通信系统而言，其传递的信息具有随机性，所以定量描述信息应基于随机事件。香农认为，任何信息都存在冗余，冗余的大小与信息中每个符号（数字、字母或者单词）的出现概率或者不确定性相关。
# 比特和自信息
通常，一个信号源发出什么符号是不确定的，衡量它可以根据其出现的概率来度量。概率大，出现的机会多，不确定性小；反之概率小，出现的机会少，不确定性大。在极限条件下，一个信号源只发出一种符号，即内容是确定的，概率为100%.但是接收方无法从接收信号中获得任何信息，即信息量为零。而反之，如果发送方和接收方约定，1代表二进制的0,2代表二进制的1，接收端可以通过接收到的信源符号获取一定的信息。
再次，较为不可能的时间具有更高的信息量。这个结合上一点很好理解。
最后，独立事件应该具有增量的信息。这一点有点和随机变量的独立性矛盾。每次独立地投掷硬币，正面或者反面的概率是一样的，但是每次独立事件带来的信息是会变化的，例如投掷硬币两次正面朝上传递的信息量，应该是一次正面朝上信息量的两倍。
为了满足上述三个性质，定义自信息（self-information）：
![png.latex?I(x)=-logP(x)](http://latex.codecogs.com/png.latex?I(x)=-logP(x))
式中的log表示自然对数， I(x)的单位是奈特（nats）。一奈特是以1/e的概率观测到一个事件时获得的信息量。如果用以2为底的对数，单位是比特（bit）或者香农（shannons）。
# 香农熵/信息熵
自信息只能处理单个的输出，信息熵则可以定量描述信息的大小。假设一个随机事件发生概率Pi的概率函数为f（Pi），该函数具有：
单调性：概率越大的事件，信息熵反而越小
非负性：f（pi）>=0
可加性：
事件X=x1,Y=y1同时发生，其发生的概率为
p(X=x1,Y=y1)=p(x1)p(y1)
而f满足：
f(p(X=x1,Y=y1))=f(p(x1))f(p(y1))
最后香农在文献[1]中从数学上证明了满足上述性质的函数具有唯一的形式，就是
![png.latex?H(x)=E_%7Bx~P%7D%5BI(x)%5D=-E_%7Bx~P%7D%5BlogP(x)%5D](http://latex.codecogs.com/png.latex?H(x)=E_%7Bx~P%7D%5BI(x)%5D=-E_%7Bx~P%7D%5BlogP(x)%5D)
离散形式为：
![png.latex?H=-K%5CSigma_%7Bi=1%7D%5En(p_i*log(p_i))](http://latex.codecogs.com/png.latex?H=-K%5CSigma_%7Bi=1%7D%5En(p_i*log(p_i)))
其中，K是一个正数。
这就是大名鼎鼎的信息熵（Informationentropy）/香农熵（Shannonentropy）。
从定义公式来看，香农熵可以理解为自信息的数学期望。那些接近确定性的分布，香农熵比较低，而越是接近平均分布的，香农熵比较高。这个和越不容易发生的事情信息越大这个基本思想是一致的。从这个角度看，信息可以看做是不确定性的衡量，而信息熵就是对这种不确定性的数学描述。
信息熵不仅定量衡量了信息的大小，并且为信息编码提供了理论上的最优值：使用的编码平均码长度的理论下界就是信息熵。或者说，信息熵就是数据压缩的极限。
当随机变量x是连续的，香农熵就被称为微分熵（differentialentropy）
# 互信息
要讲互信息，就必须从随机变量的独立性说起。如果两个随机变量X和Y满足：
P(X,Y)=P(X)P(Y)
则随机变量独立。其实，如果X，Y独立，也就是意味着已知X，将不会对Y的分布产生任何的影响，也就是说：
P(Y|X)=P(X,Y)/P(X)=P(X)P(Y)/P(X)=P(Y)
独立性反映了已知X的情况下，Y的分布是否会改变。独立性可以表示出两个随机变量之间是否有关系，但是不能刻画它们关系的大小。这时就有必要引入互信息（MutualInformation）。互信息定义为：
![png.latex?I(X;Y)=%5Cint_X%5Cint_Y%5Cfrac%7BP(X,Y)logP(X,Y)%7D%7BP(X)P(Y)%7DdXdY](http://latex.codecogs.com/png.latex?I(X;Y)=%5Cint_X%5Cint_Y%5Cfrac%7BP(X,Y)logP(X,Y)%7D%7BP(X)P(Y)%7DdXdY)
I(X;Y)表示由X的引入，使得Y的不确定性减小的量.（证明及推导详见2）
因而，如果X,Y的关系越密切，I(X;Y)越大，I(X;Y)的最大值是H(Y)
# K-L散度
互信息表明了两个随机变量的关系，特别是当一种随机变量引入时，另一个随机变量不确定性减小的程度。但是如何衡量两个随机变量分布是否相同呢？
对于同一个随机变量x，有两个单独的概率分布P(x)和Q(x)，我们可以用KL散度（Kullback-Leiblerdivergence）来衡量这两个分布之间的差异：
![Q(x))%5D=E_%7Bx~P%7D%5BlogP(x)-logQ(x)%5D](http://latex.codecogs.com/png.latex?D_%7BKL%7D(P%7C%7CQ)=E_%7Bx~P%7D%5Blog(P(x)/Q(x))%5D=E_%7Bx~P%7D%5BlogP(x)-logQ(x)%5D)
KL散度最重要的性质是非负性。对于离散型变量，当且仅当P和Q是相同的分布情况下KL散度为零。对于连续型随机变量，当且仅当P和Q是“几乎处处”（almosteverywhere）相同的，KL散度为零。虽然KL散度常被用来衡量两个分布之间的距离，但是KL散度并不是真正的距离，因为它是不对称的，这从它的定义很容易看出。
参考：
1.《深度学习》
2.[http://blog.csdn.net/lk7688535/article/details/52529610](http://blog.csdn.net/lk7688535/article/details/52529610)
3.[http://www.fuzihao.org/blog/2015/01/17/%E4%BA%92%E4%BF%A1%E6%81%AF%E7%9A%84%E7%90%86%E8%A7%A3/](http://www.fuzihao.org/blog/2015/01/17/%E4%BA%92%E4%BF%A1%E6%81%AF%E7%9A%84%E7%90%86%E8%A7%A3/)
4.[http://blog.csdn.net/pipisorry/article/details/51695283](http://blog.csdn.net/pipisorry/article/details/51695283)
