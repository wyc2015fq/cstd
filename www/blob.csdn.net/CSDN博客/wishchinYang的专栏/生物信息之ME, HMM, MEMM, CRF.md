# 生物信息之ME, HMM, MEMM, CRF - wishchinYang的专栏 - CSDN博客
2014年01月23日 13:59:51[wishchin](https://me.csdn.net/wishchin)阅读数：915
原文链接：[http://bbs.sciencenet.cn/home.php?mod=space&uid=260809&do=blog&id=573755](http://bbs.sciencenet.cn/home.php?mod=space&uid=260809&do=blog&id=573755)
注：有少量修改！如有疑问，请访问原作者。
做高端的生物信息理论离不开各种modeling 于是乎漫长的digest之路开始...
**一：最大熵模型 Maximum Entropy**
现从一个简单例子看起（不要把鸡蛋放在一个篮子里）：
比如华盛顿和维吉利亚都可以作人名和地名，而从语料中只知道p(人名)＝0.6，那么p(华盛顿＝人名)的概率为多少比较好呢？一个直观的想法就是p(华盛顿=人名)=0.3。为什么呢？这就是在满足已有证据的情况下不做任何其他假设，也就是熵最大，这就是最大熵模型的原理。
现在来看模型的定义：
首先，明确模型的目标：给定一个上下文x，估计p(y|x)
接着，从训练样本中我们可以得到一串标注过的样本(x_i, y_i)，其中x_i为上下文，y_i \in Y为类别
然后构造特征函数
f(x,y) = 1 如果x,y满足一些条件，比如x=记者*,y＝人名
            0 otherwise
注意x是一个上下文，是个向量，而不是单个词
(最大熵模型里的特征概念不同于模式识别里的特征，这里的特征即特征函数，通常是二值函数，也有直接定义成实数的，比如 jeon-sigir06里直接把f定义为KDE距离，不是很明白那样定义的好处。)
于是模型的约束就是对于所有的特征函数模型中的期望等于样本的期望，即
E_p(f) = E_{\tilde p}(f)
其中
E_p(f) = \sum_{x, y}p(x, y)f(x, y) = \sum_{x, y}p(x)p(y|x)f(x,y) \approx \sum_{x, y} \tilde p(x)p(y|x)f(x,y)
\tilde p(f) = \sum_{x, y} \tilde p(x, y)f(x, y),
并且对于任意的x：\sum_y p(y|x) = 1
而模型的熵为
H(p)=-\sum_{x,y} \tilde p(x) p(y|x) \log p(y|x)
在满足约束的情况下，使熵最大，于是问题即求
p* =\argmax_{p \in P} -\sum{x, y} p(y|x)\tilde p(x) \log p(y|x)
where P={p(y|x) | \all f_i : \sum_{x,y}p(y|x)\tilde p(x)f_i(x,y) = \sum_{x,y}\tilde p(x,y)f_i(x,y), \all x : \sum_y p(y|x) = 1}
可以证明，模型的最优解的形式为
p(y|x) = exp(\sum_i \lambda_i f_i(x,y)) / Zx
where Zx = \sum_y exp(\sum_i \lambda_i f_i(x,y))
**二：隐马尔可夫模型 Hidden Markov Model：**
马尔可夫模型实际上是个**有限状态机**，两两状态间有转移概率；
隐马尔可夫模型中状态不可见，我们只能看到输出序列，也就是每次状态转移会抛出个观测值；
当我们观察到观测序列后，要找到最佳的状态序列。
设O为观测值，x为隐变量，那么模型要找到让P(O)最大的最佳隐藏状态，而
P(O) = \sum_x P(O|X)P(X)
而其中
P(X)=p(x_1)p(x_{2..n}|x_1)
        =p(x_1)p(x_2|x_1)p(x_{3..n}|x_1,x_2)
        ……
根据x_i只与x_{i-1}相关的假设有
P(X)=p(x_1)p(x_2|x_1)p(x_3|x_2)……
而类似的
P(O|X)=p(o_1|x_{1..n})p(o_{2..n}|o_1x_{1..n})
            =p(o_1|x_{1..n})p(o_2|o_1x_{1..n})p(o_{3..n}|o_{1,2},x_{1..n})
            ……
根据o_i只与x_i有关的假设有
P(O|X)=p(o_1|x_1)p(o_2|x_2)……
合起来就是
P(O)=\sum_x p(x_1)p(x_2|x_1)p(o_1|x_1)p(x_3|x_2)p(o_2|x_2)……
定义向前变量\alpha_i(t)为t时刻以状态S_i结束时的总概率
\alpha_j(t)=\sum_{i=1}^N \alpha_ip(x_{t}=j|x_{t-1}=i)p(o_t=i|x_t=i)
定义向后变量\beta_i(t)为给定当前状态S_i和t时刻情况下观测序列中剩余部分的概率和
\beta_i(t)=\sum_{j=1}^N \p(x_{t}=j|x_{t+1}=i)p(o_{t}=i|x_{t}=i) \beta_j(t+1)
于是观测序列的概率为
P(O, X_t=i) = \alpha_i(t)\beta_i(t)
最佳状态可以由动态规划得到
模型参数可以由EM算法得到
**三：最大熵隐马 Maximum Entropy Markov Model**
HMM的缺点是根据观测序列决定状态序列，是用联合模型解决条件问题；另外，几乎不可能枚举所有所有可能的观测序列。
而MEMM解决了这些问题。
首先，MEMM和MM或HMM都有本质不同，MEMM估计的是P(S|O)，而MM估计的是P(S)，HMM估计的都是P(O)。
P(S|O)=P(s_1|O)P(s_{2..n}|s_1,O)
           =P(s_1|O)P(s_2|s_1,O)P(s_{3..n}|s_1,s_2,O)
          ……
然后根据假设有
P(S|O)=P(s_1|O)P(s_{2..n}|s_1,O)
           =P(s_1|o_1)P(s_2|s_1,o_2)P(s_{3..n}|s_1,s_2,o_3)
          ……
重新定义特征函数：
a=<b,r>
b是指示函数用于指示当前观测
r是状态值
f_a(o_t, S_t) = 1 if b(o_t) is true and s_t = r
于是约束变为
E_a = \sum_{k=1}^m_{s'}\sum_{s \in S}P(s|s', o_k)f_a(o_k, s) / m_s' = \sum_{k=1}^m_{s'} f_a(o_k, s_k) = F_a
这个目标函数和ME的目标函数实质是一样的
于是解的形式为
P(s|s', o)=exp(\sum_a \lambda_a f_a(o, s)) / Z(o, s')
然后依然采用HMM中的前向后向变量，寻找最佳序列
而实际上得到的序列是由计算
P(s|o) = P(s_0)P(s_1|s_0,o_0)P(s_2|s_1, o_1)……
得到
**四：条件随机场 Conditional Random Fields**
MEMM其实是用局部信息去优化全局，会有label bias的问题。比如rib和rob，有如下的状态设计：
  /r 1i - 2 \
0             b 3
  \r 4o - 5 /
如果训练集合里的ri多于ro，那么ro其实就被无视了……
所以要用全局优化全局，于是引入CRF，其实CRF就是ME扩展版，并不需要由MRF推出
p(y|x)\propto exp(\sum_i\lumbda_k f_k(y_{i-1}, y_i, x)+\sum_k \lumbda_kg_k(x_k, x))
其实这个定义并保持MRF性质：MRF中clique于clique是独立的
从这点意义上来看，Lafferty也满水的= =
虽然X，Y在图上不需要有相同的结构，甚至X都不需要有图的结构，目前通常G只是一条链G=(V={1,2, ..., m})，E={(i,i+1)}。
如果G是一棵树，它的团就是每条边和其上的点，这时候
p_\theta(y|x) = exp(\sun_{e\in E, k}\lambda_k f_k(e,y|_e, x)+\sum_{v \in V,k}\mu_k g_k(v, y|_v, x))
x是上下文，y是标注序列，y|_s是y中与子图S相连的部分
如果是最简单的first-order情况，g_k就相当于ME中的特征函数，f_k类似于MEMM中的特征函数，就是说用g_k来指示状态，用f_k来指示状态转移
从优化角度来说，这两类函数是一样的，可以简写为f_j(y_{i-1},y_i,x,i)，于是训练类似ME
当CRF是链状时，一个上下文x的标注可以利用矩阵高效计算
对于长度为n的y，定义n+1个|Y|*|Y|矩阵{M_i(x)|i=1..n+1}，其中
Y是类别个数
M_i(y', y|x) = exp(\sum_j \lambda_j f_j(y', y, x, i))
这个就是第i个矩阵i,j上的元素，表示在x下从y'转移到y的概率
于是有p(y|x, \lambda)=\multi_{i=1}^{n+1}M_i(y_{i-1},y_i|x) / Zx
Zx = \multi_{i=1}^{n+1}M_i(x)
Zx只是保证概率之和为1
原来已经有人开始做复杂的情况了……04年cvpr有篇用CRF做图像标注的。
看来易想到的idea一般都会有人做了……
John Lafferty, Andrew McCallum这两个人无比牛啊!! 几乎引领着CRF这一领域的发展：虽然ME不是1拉最先提出来的，但是1拉从96年就开始研究crf相关的东西，
这种牛就是每年N篇ICML+N篇NIPS……而且1看起来还是满ss的，套套看，kaka……Andrew McCallum从1999年开始做ME，那年与John Lafferty合作在IJCAI上发了篇用ME做文本分类的文章，2003年在ICML上发了篇Dynamic CRF，然后又把CRF应用在CV上，也是每年N篇ICML+N篇NIPS的人物，1虽然不如John Lafferty ss，但也还是很可爱的！发现牛人的圈子其实很小，一个领域领头人物也就那么几个……
原来HMM和MEMM,CRF估的东西就不一样，所以以前怎么推都推不出……= =
不过最近发现 conditional random fields (CRFs)
 应用于model RNA sequence–structure relationship ，predict
 post-translational modification sites (labels) in protein sequences 越来越广泛了
看来新PGM总有优势所在呀》》
先总结到这儿，欢迎大牛们指点，
