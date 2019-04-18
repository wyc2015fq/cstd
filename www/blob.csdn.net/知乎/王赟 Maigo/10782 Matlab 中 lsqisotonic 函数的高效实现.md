# 10782 Matlab 中 lsqisotonic 函数的高效实现 - 知乎
# 

　　上个月发现，我一直在使用的盗版 Matlab 2010a 就要过期了。于是我从学校下载了正版的 2017b。升级后，我发现统计工具包里的 lsqisotonic 函数跟原来一样，仍然使用了一种低效的实现，而我在 2012 年就发现了这个问题，并写了一个更高效的版本。这篇专栏就来分享一下我的实现。

　　本文的第一部分介绍 lsqisotonic 这个函数的背景 —— multi-dimensional scaling，第二部分讨论这个函数本身的实现。对背景不感兴趣的读者，可以直接跳到第二部分，因为这个函数本身的功能，可以归纳成一道普通的算法题。

## 一、背景：Multi-dimensional Scaling

.　　[Multi-dimensional scaling](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Multidimensional_scaling) 是一种数据可视化的方法。它不太容易翻译成中文，主要是因为 scaling 这个词的用法比较奇怪。维基百科给出的中文翻译是「[多维标度](https://link.zhihu.com/?target=https%3A//zh.wikipedia.org/zh-cn/%25E5%25A4%259A%25E7%25BB%25B4%25E6%25A0%2587%25E5%25BA%25A6)」，其实挺不知所云的，甚至都不能体现出这是一个动名词。而日文翻译叫「[多次元尺度構成法](https://link.zhihu.com/?target=https%3A//ja.wikipedia.org/wiki/%25E5%25A4%259A%25E6%25AC%25A1%25E5%2585%2583%25E5%25B0%25BA%25E5%25BA%25A6%25E6%25A7%258B%25E6%2588%2590%25E6%25B3%2595)」，我觉得可以把二者融合一下，译作「多维尺度构成法」。在本文中，我把这种方法简称为 MDS。Matlab 中统计工具包里的 [mdscale](https://link.zhihu.com/?target=https%3A//www.mathworks.com/help/stats/mdscale.html) 函数，就是用来做 MDS 的。

　　MDS 并不是目前最流行的数据可视化方法，最流行的应该是机器学习大佬 Hinton 开创的 [t-SNE](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/T-distributed_stochastic_neighbor_embedding)。在这个回答中，我用 MDS 和 t-SNE 两种方法对我 2012 年的人人好友关系进行了可视化，并比较了它们的效果：[王赟 Maigo：有没有那种方式可以将高维数据进行可视化？比如保持数据结构不变将高维数据映射到低维空间？](https://www.zhihu.com/question/37189447/answer/74759345)下图就是用 MDS 可视化的结果：
![](https://pic3.zhimg.com/v2-833f817911f592f3273868ffdeb581be_b.jpg)
　　MDS 的输入，是 ![n](https://www.zhihu.com/equation?tex=n) 个对象两两之间的**差异度**（dissimilarities），共 ![n(n-1)/2](https://www.zhihu.com/equation?tex=n%28n-1%29%2F2) 个数值。如果已知的是相似度（similarities），则可以通过一个单调递减的函数转换成差异度。记第 ![i,j](https://www.zhihu.com/equation?tex=i%2Cj) 个对象之间的差异度为 ![\delta_{ij}](https://www.zhihu.com/equation?tex=%5Cdelta_%7Bij%7D) 。MDS 要做的事情，是在一个给定维数（通常为二维或三维）的空间中找一组点 ![X = \{X_1, \ldots, X_n\}](https://www.zhihu.com/equation?tex=X+%3D+%5C%7BX_1%2C+%5Cldots%2C+X_n%5C%7D) 来代表这些对象，使得第 ![i,j](https://www.zhihu.com/equation?tex=i%2Cj) 两个点之间的距离 ![d_{ij}(X) = ||X_i - X_j||](https://www.zhihu.com/equation?tex=d_%7Bij%7D%28X%29+%3D+%7C%7CX_i+-+X_j%7C%7C) 尽可能接近给定的差异度 ![\delta_{ij}](https://www.zhihu.com/equation?tex=%5Cdelta_%7Bij%7D) 。具体来说，是要最小化如下的目标函数，这个函数称为**压力**（stress）：

> ![\sigma(X) = \sum_{i<j} w_{ij}(d_{ij}(X) - \delta_{ij})^2](https://www.zhihu.com/equation?tex=%5Csigma%28X%29+%3D+%5Csum_%7Bi%3Cj%7D+w_%7Bij%7D%28d_%7Bij%7D%28X%29+-+%5Cdelta_%7Bij%7D%29%5E2)

其中 ![w_{ij}](https://www.zhihu.com/equation?tex=w_%7Bij%7D) 是点对 ![(i,j)](https://www.zhihu.com/equation?tex=%28i%2Cj%29) 的权重。一般来说，所有权重都取为 1；如果输入数据不全，某一组差异度 ![\delta_{ij}](https://www.zhihu.com/equation?tex=%5Cdelta_%7Bij%7D) 没有测量到，那么可以通过设置 ![w_{ij} = 0](https://www.zhihu.com/equation?tex=w_%7Bij%7D+%3D+0) 来把这个点对排除掉。当然，如果认为某些点对的差异度比另一些点对更重要，也可以给每一个点对赋予不同的权重。

　　求使得压力最小化的点集 ![ X](https://www.zhihu.com/equation?tex=+X) 的方法有很多。比如梯度下降法就可以使用；Matlab 中 mdscale 函数使用的是一种共轭梯度法。除此之外，[Modern Multidimensional Scaling](https://link.zhihu.com/?target=http%3A//www.springer.com/us/book/9780387251509) 一书的第 8 章还介绍了一种称为 [SMACOF](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Stress_majorization) 的迭代算法，它与机器学习中常见的 [EM 算法](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Expectation%25E2%2580%2593maximization_algorithm)有相似之处，二者都是 [MM 算法](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/MM_algorithm)的特例。不过求点集 ![ X](https://www.zhihu.com/equation?tex=+X) 的算法不是本文讨论的重点，所以我就不继续展开了。

　　在实际问题中，差异度不一定是定比数据，而有可能只是定序的（参见：[王赟 Maigo：华裔数学家陶哲轩IQ230，是智商100聪明程度的几倍？](https://www.zhihu.com/question/40102238/answer/85313038)），即差异度的数值并无意义，而只有它们之间的大小关系有意义。这种情形的 MDS，称为 non-metric MDS。上文中的压力函数依赖于差异度的具体数值，在 non-metric MDS 中再使用这种压力函数，就显得不合理了。于是就有了下面这种新的压力函数：

> ![\sigma(X, \hat{d}) = \sum_{i<j} w_{ij}(d_{ij}(X) - \hat{d}_{ij})^2](https://www.zhihu.com/equation?tex=%5Csigma%28X%2C+%5Chat%7Bd%7D%29+%3D+%5Csum_%7Bi%3Cj%7D+w_%7Bij%7D%28d_%7Bij%7D%28X%29+-+%5Chat%7Bd%7D_%7Bij%7D%29%5E2)

其中 ![\hat{d}_{ij}](https://www.zhihu.com/equation?tex=%5Chat%7Bd%7D_%7Bij%7D) 是差异度经过变换 ![\hat{d}_{ij} = f(\delta_{ij})](https://www.zhihu.com/equation?tex=%5Chat%7Bd%7D_%7Bij%7D+%3D+f%28%5Cdelta_%7Bij%7D%29) 的结果。变换 ![f](https://www.zhihu.com/equation?tex=f) 仅需要满足单调性，称为**单调回归变换**（[isotonic regression](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Isotonic_regression)）；它的作用就是说明差异度的数值并不重要，重要的只有大小关系。要最小化这个压力函数，一方面需要求出一组点的坐标 ![X](https://www.zhihu.com/equation?tex=X) ，另一方面还要求出一个变换 ![f](https://www.zhihu.com/equation?tex=f) ，形成了一个「鸡生蛋，蛋生鸡」的问题。这种问题一般也是通过迭代算法来解决的，即不停重复下面的步骤：
- 固定 ![\hat{d}_{ij}](https://www.zhihu.com/equation?tex=%5Chat%7Bd%7D_%7Bij%7D) ，求使得压力最小化的 ![X](https://www.zhihu.com/equation?tex=X) 。事实上这一步并不需要使得压力「最小化」，只要能让它减小就行了。这一步可以使用梯度下降法、共轭梯度法、SMACOF 等任意一种方法，且只需迭代一次。
- 固定 ![X](https://www.zhihu.com/equation?tex=X) ，求使得压力最小化的单调回归变换 ![f](https://www.zhihu.com/equation?tex=f) 。这一步同样只要让压力减小就行了，不过让压力最小化也不困难。这一步，就是由本文的主角 —— lsqisotonic 函数来实现的。

　　lsqisotonic 这个函数的名字中，lsq 是 least squares（最小二乘）的意思，指的是压力函数的形式；isotonic 则说明函数用来求解最优的单调回归变换。为了下文讨论方便，我把函数的功能再提炼一下。我们把所有的差异度 ![\delta_{ij}](https://www.zhihu.com/equation?tex=%5Cdelta_%7Bij%7D) 从小到大排序，得到 ![x_1, \ldots, x_N](https://www.zhihu.com/equation?tex=x_1%2C+%5Cldots%2C+x_N) ，其中 ![N = n(n-1)/2](https://www.zhihu.com/equation?tex=N+%3D+n%28n-1%29%2F2) 是点对的数目。与 ![x_k = \delta_{ij}](https://www.zhihu.com/equation?tex=x_k+%3D+%5Cdelta_%7Bij%7D) 对应的那个点对在空间中的距离 ![d_{ij}(X)](https://www.zhihu.com/equation?tex=d_%7Bij%7D%28X%29) 记作 ![y_k](https://www.zhihu.com/equation?tex=y_k) ，其权重记作 ![w_k](https://www.zhihu.com/equation?tex=w_k) 。lsqisotonic 函数要求的是变换后的差异度 ![f(x_1), \ldots, f(x_N)](https://www.zhihu.com/equation?tex=f%28x_1%29%2C+%5Cldots%2C+f%28x_N%29) ，把它们记作 ![\hat{y}_1, \ldots, \hat{y}_N](https://www.zhihu.com/equation?tex=%5Chat%7By%7D_1%2C+%5Cldots%2C+%5Chat%7By%7D_N) 。它们需要满足单调性： ![\hat{y}_1 \le \hat{y}_2 \le \ldots \le \hat{y}_N](https://www.zhihu.com/equation?tex=%5Chat%7By%7D_1+%5Cle+%5Chat%7By%7D_2+%5Cle+%5Cldots+%5Cle+%5Chat%7By%7D_N) ，并且最小化目标函数 ![\sigma = \sum_{k=1}^N w_k(y_k - \hat{y}_k)^2](https://www.zhihu.com/equation?tex=%5Csigma+%3D+%5Csum_%7Bk%3D1%7D%5EN+w_k%28y_k+-+%5Chat%7By%7D_k%29%5E2) 。注意，提炼后的函数的输入其实只有 ![y_1, \ldots, y_N](https://www.zhihu.com/equation?tex=y_1%2C+%5Cldots%2C+y_N) 和 ![w_1, \ldots, w_N](https://www.zhihu.com/equation?tex=w_1%2C+%5Cldots%2C+w_N) ； ![x_1, \ldots, x_N](https://www.zhihu.com/equation?tex=x_1%2C+%5Cldots%2C+x_N) 的具体数值是没有用的，它们唯一的作用就是指定了 ![y_1, \ldots, y_N](https://www.zhihu.com/equation?tex=y_1%2C+%5Cldots%2C+y_N) 和 ![w_1, \ldots, w_N](https://www.zhihu.com/equation?tex=w_1%2C+%5Cldots%2C+w_N) 的顺序。

## 二、lsqisotonic 函数的实现

　　如果你跳过了第一部分，那么从此往下看，也不会有任何问题 ^_^

　　lsqisotonic 函数的作用是求解最优的「单调回归变换」。它的输入是一个序列 ![y = \{y_1, \ldots, y_N\}](https://www.zhihu.com/equation?tex=y+%3D+%5C%7By_1%2C+%5Cldots%2C+y_N%5C%7D) 和一组权重 ![w = \{w_1, \ldots, w_N\}](https://www.zhihu.com/equation?tex=w+%3D+%5C%7Bw_1%2C+%5Cldots%2C+w_N%5C%7D) 。序列 ![y](https://www.zhihu.com/equation?tex=y) 不一定是单调递增的，现在我们要求一个单调递增的序列 ![\hat{y}_1 \le \hat{y}_2 \le \ldots \le \hat{y}_N](https://www.zhihu.com/equation?tex=%5Chat%7By%7D_1+%5Cle+%5Chat%7By%7D_2+%5Cle+%5Cldots+%5Cle+%5Chat%7By%7D_N) ，让它跟输入序列 ![y](https://www.zhihu.com/equation?tex=y) 尽可能接近。「接近」的具体标准是最小化压力函数 ![\sigma = \sum_{k=1}^N w_k(y_k - \hat{y}_k)^2](https://www.zhihu.com/equation?tex=%5Csigma+%3D+%5Csum_%7Bk%3D1%7D%5EN+w_k%28y_k+-+%5Chat%7By%7D_k%29%5E2) 。为讨论简洁，认为所有权重均为正。

　　先来看一个具体的例子。设输入序列为 ![y = \{1, 4, 3, 5, 3, 1, 7, 5\}](https://www.zhihu.com/equation?tex=y+%3D+%5C%7B1%2C+4%2C+3%2C+5%2C+3%2C+1%2C+7%2C+5%5C%7D) ，所有元素权重均为 1。输入序列不单调递增，我们需要用最小的力把它「掰」成单调递增的，即把所有的下降段「掰平」。

　　首先看「4, 3」这个下降段。现在要把它掰平，那么把两个数都掰成多少能使得压力最小呢？不难发现，答案应该是 4 和 3 的平均数，即 3.5。如果这两个数有不同的权重，那么使得压力最小的，就应该是它们的加权平均数（证明留给读者）。

　　按照这种思路，可以把输入序列中三个下降段「4, 3」「5, 3, 1」「7, 5」分别掰成 3.5、3、6，得到 ![1, 3.5, 3.5, 3, 3, 3, 6, 6](https://www.zhihu.com/equation?tex=1%2C+3.5%2C+3.5%2C+3%2C+3%2C+3%2C+6%2C+6) 。这就结束了吗？并没有 —— 因为 3.5 和 3 这两个段落又违反单调性了。此时，就要把 3.5 和 3 这两个段落整体掰平。3.5 段落的总权重为 2，3 段落的总权重为 3，所以掰平的结果应该是加权平均数 3.2。此时得到的序列为 ![1, 3.2, 3.2, 3.2, 3.2, 3.2, 6, 6](https://www.zhihu.com/equation?tex=1%2C+3.2%2C+3.2%2C+3.2%2C+3.2%2C+3.2%2C+6%2C+6) ，满足单调性，所以这就是要求的 ![\hat{y}_1, \ldots, \hat{y}_8](https://www.zhihu.com/equation?tex=%5Chat%7By%7D_1%2C+%5Cldots%2C+%5Chat%7By%7D_8) 。

　　上面逐渐把下降段「掰平」的过程用图象表示如下，蓝点为输入序列，红点及红线为所求的单调序列。
![](https://pic1.zhimg.com/v2-cff89fd46c5d6faf1a91e1d371781ab8_b.jpg)
　　Matlab 自带的 lsqisotonic 函数，就是这样求解最优单调回归变换的。它不断地在序列中寻找下降段，并把下降段掰成整体的加权平均数，直到序列单调递增为止。其代码的核心部分如下：

```matlab
yhat = y;                               % 用输入序列初始化输出序列
block = :length(y);                    % block(i) 表示第 i 个元素属于第几个段落
                                        % 初始时每个元素独立成段
while true
    diffs = diff(yhat);                 % 求所有相邻元素之差
    if all(diffs >= ), break; end      % 若已满足单调性，退出
    idx = cumsum([; (diffs > )]);     % 找出序列中所有的下降段，并依次编号
                                        % 例如，若输入为 1,4,3,5,3,1,7,5
                                        % 则编号结果为   1,2,2,3,3,3,4,4
    sumyhat = accumarray(idx, w.*yhat); % 计算每段元素的加权和
    w = accumarray(idx, w);             % 计算每段元素的总权重
    yhat = sumyhat ./ w;                % 求出每段元素的加权平均数
    block = idx(block);                 % 更新每个元素所属的段落编号
end
yhat = yhat(block);                     % 构建输出序列
```

　　这段代码使用了一些 Matlab 特有的操作（比如 [cumsum](https://link.zhihu.com/?target=https%3A//www.mathworks.com/help/matlab/ref/cumsum.html)、[accumarray](https://link.zhihu.com/?target=https%3A//www.mathworks.com/help/matlab/ref/accumarray.html)），可能比较难理解。理解的关键在于，在迭代过程中，yhat 并不是记录了完整的序列，而是对序列中每一个水平段落，只记录一个值。上文所举例子的执行过程如下表所示，它会帮助你理解。
![](https://pic3.zhimg.com/v2-90a04e98fa40114bffc720681c563882_b.jpg)
　　上面的实现方式有什么问题呢？当然是复杂度啦！不难看出，每次迭代的时间复杂度为 ![O(N)](https://www.zhihu.com/equation?tex=O%28N%29) ，而迭代次数的上限也是 ![O(N)](https://www.zhihu.com/equation?tex=O%28N%29) ，所以总复杂度为 ![O(N^2)](https://www.zhihu.com/equation?tex=O%28N%5E2%29) 。下面这个例子可以达到复杂度的上限：输入序列 ![y = \{10000, 1, 2, 3, 4, 5\}](https://www.zhihu.com/equation?tex=y+%3D+%5C%7B10000%2C+1%2C+2%2C+3%2C+4%2C+5%5C%7D) ，权重 ![w = \{10000, 1, 1, 1, 1, 1\}](https://www.zhihu.com/equation?tex=w+%3D+%5C%7B10000%2C+1%2C+1%2C+1%2C+1%2C+1%5C%7D) 。这个例子的精髓在于，序列有且仅有前两个元素组成下降段，并且因为第一个元素 10000 的权重很大，把前两个元素取加权平均合并后，序列第一段的值依然会很大。这个巨大的值会在每次迭代中吃且仅吃掉后面的一个元素，导致迭代次数达到 ![N](https://www.zhihu.com/equation?tex=N) 。

> **下面这段话是写给看过第一部分的读者的：**
　　注意在 MDS 的背景下，![N](https://www.zhihu.com/equation?tex=N) 是点对的个数，它与数据点数目 ![n](https://www.zhihu.com/equation?tex=n) 的关系是 ![N = n(n-1)/2](https://www.zhihu.com/equation?tex=N+%3D+n%28n-1%29%2F2) 。也就是说，Matlab 自带的 lsqisotonic 函数的时间复杂度，是吓人的 ![O(n^4)](https://www.zhihu.com/equation?tex=O%28n%5E4%29) ！我用来可视化的人人好友有 1000 多名，难怪 lsqisotonic 会卡死了。

　　事实上，「反复合并下降段」这个过程，完全可以用 ![O(N)](https://www.zhihu.com/equation?tex=O%28N%29) 的时间复杂度来实现。我们从左到右扫描序列的每一个元素，并用一个栈来维护已经扫描的部分「掰平」后的各个水平段落。当扫描到一个新的元素的时候，先把它作为一个单独的段落压入栈顶，然后反复查看栈顶的两个段落，如果它们违反了单调性，就把它们合并。这种实现的代码如下：

```matlab
yhat = y; N = length(y);                 % 用输入序列初始化输出序列
bstart = zeros(,N); bend = zeros(,N);  % 栈：bstart(i), bend(i) 记录第 i 段的起止位置
                                         % 此外 yhat 和 w 也兼用作栈，
                                         % yhat(i) 与 w(i) 表示第 i 段的值和总权重
b = ;                                   % 栈顶指针
for i = :N                              % 依次扫描每个元素
    b = b + ;                           % 由此往下三行：新元素作为单独的段落入栈
    yhat(b) = yhat(i); w(b) = w(i);
    bstart(b) = i; bend(b) = i;
    while b >  && yhat(b) < yhat(b-)   % 栈顶两个段落违反单调性
        yhat(b-) = (yhat(b-) * w(b-) + yhat(b) * w(b)) / (w(b-) + w(b));
        w(b-) = w(b-) + w(b);
        bend(b-) = bend(b);
        b = b - ;                       % 由此往上四行：栈顶两个段落取加权平均合并
    end
end
block = zeros(,N);
for i = :b
    block(bstart(i) : bend(i)) = i;      % 由栈中信息反推出输出序列的每个元素位于第几段
end
yhat = yhat(block);                      % 构建输出序列
```

　　这段代码的主体循环没有用到 Matlab 的黑科技，比较好懂，所以样例数据的执行过程我就不写了。

　　我的实现的时间复杂度为 ![O(N)](https://www.zhihu.com/equation?tex=O%28N%29) 。虽然有时一个元素入栈会引发连锁式的段落合并，但考虑算法的整个执行过程，一共会有 ![N](https://www.zhihu.com/equation?tex=N) 个元素入栈，最多有 ![N-1](https://www.zhihu.com/equation?tex=N-1) 次段落合并，所以复杂度为 ![O(N)](https://www.zhihu.com/equation?tex=O%28N%29) 。那么 Matlab 自带实现慢在哪儿了呢？仍然考虑极端输入 ![y = \{10000, 1, 2, 3, 4, 5\}](https://www.zhihu.com/equation?tex=y+%3D+%5C%7B10000%2C+1%2C+2%2C+3%2C+4%2C+5%5C%7D) ，![w = \{10000, 1, 1, 1, 1, 1\}](https://www.zhihu.com/equation?tex=w+%3D+%5C%7B10000%2C+1%2C+1%2C+1%2C+1%2C+1%5C%7D) 。在迭代过程中，序列的尾部始终是单调递增的，但 Matlab 的实现在每次迭代中都徒劳无功地在序列的尾部检查是否有下降段。这就是它慢的原因。

## 三、附记

　　我实现的 lsqisotonic 函数，可以从 [Mathworks File Exchange](https://link.zhihu.com/?target=https%3A//www.mathworks.com/matlabcentral/fileexchange/64933-lsqisotonic-x-y-w-) 上下载。这个函数位于 Matlab 安装目录下的 toolbox\stats\stats\private 子目录，可以用我的版本替代原有版本。

　　对 MDS 感兴趣的读者，推荐阅读 [Modern Multidimensional Scaling](https://link.zhihu.com/?target=http%3A//www.springer.com/us/book/9780387251509) 一书。其中第 8、9 章介绍的就是本文讨论的 non-metric MDS。第 12 章介绍了 metric MDS 的另一种情形 classical MDS，它最小化的目标函数并不是 stress，而是另一种称为 strain 的目标函数；其优点是求解过程不是迭代的，而是可以一步到位。Classical MDS 在 Matlab 中由 [cmdscale](https://link.zhihu.com/?target=https%3A//www.mathworks.com/help/stats/cmdscale.html) 函数实现。

