# RMQ(Range minimum query) based LCA solution - 电影旅行敲代码 - CSDN博客
2018年10月21日 17:45:18[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：64
## 何为RMQ
在文章《[Tarjan’s off-line lowest common ancestors algorithm](https://blog.csdn.net/dashuniuniu/article/details/78634002)》我们用图形化的方式展示了Tarjan’s off-line LCA的求解过程，但是该文章有很多遗漏，例如下面的这些问题。在本篇文章中，我会介绍另外一种求解LCA的方法，然后尝试顺带回答列出的这些问题。
- 既然有了Leetcode 236中标准解法，为什么还需要Tarjan这种比较重的方法？
- 在那篇文章中，提到Tarjan方法的本质是并查集，但是那种说法并不严谨，并查集只是Tarjan实现途径
现阶段比较好的求解LCA的方式是基于RMQ的求解方法，本文章会着重介绍什么是RMQ以及常见的几种求解RMQ的方法，
*注：本篇文章完全按照TopCoder中的[此篇文章](https://www.topcoder.com/community/competitive-programming/tutorials/range-minimum-query-and-lowest-common-ancestor/)展开的，所以如果看过那篇文章就不用浪费时间本篇文章了*
RMQ，全称Range minimum query，用于查询一个数组中子数组的最值，这样一个看似简单的问题，却有很多值得玩味的地方。
> 
In computer science, a range minimum query (RMQ) solves the problem of finding the minimal value in a sub-array of an array of comparable objects.  [《Range minimum query》](https://en.wikipedia.org/wiki/Range_minimum_query)
### 朴素解法
例如，给定包含$N$个数的数组$A[N]$和$Q$个查询。每个查询的输入$(a, b)$都是一对整数，要求打印出$A[a]$到$A[b]$之间的最大值和最小值之差。例如，$N = 6$，$Q = 3$，一个输入样例是
*注：此问题来源于《[数据结构：线段树](https://zhuanlan.zhihu.com/p/30248914)》*
$A\ :\ 1\  7\ 3\ 4\ 2\ 5\ 6\ 9$
$query: 0\ 4$
$\qquad \quad  \ \ 3\ 5$
$\qquad \quad \ \  1\ 1$
比较直观的方法是先得到Max和Min，然后求差值，在数组没有发生变化的情况下，这种方法有很多资源的浪费，存在很多重复计算。例如我们在查询（$1$，$5$）之间Max和Min，可以顺手将子区间的最大值和最小值记录下来，使用一种Record Table来存储计算后的结果。
### Record Table
显而易见将所有可能的查询下标对（$a$，$b$）记录下来，需要$O(N^2)$的空间复杂度。求子数组最小值的记录表格如下所示：
||0|1|2|3|4|5|6|7|
|----|----|----|----|----|----|----|----|----|
|0|1|1|1|1|1|1|1|1|
|1||7|3|3|2|2|2|2|
|2|||3|3|2|2|2|2|
|3||||4|2|2|2|2|
|4|||||2|2|2|2|
|5||||||5|5|5|
|6|||||||6|6|
|7||||||||9|
该方法的复杂度如下所示：
- 前期准备工作，亦即计算该表格的时间复杂度为$O(N^2)$
- 查询复杂度为$O(1)$
- 空间复杂度为$O(N^2)$，
该方法的查询复杂度虽然很低，但是空间复杂度却比较高，那么是否可以对存储的表格进行精简？
可以使用动态规划来求解该表格，注意对`Table[i][i]`的赋值移动到双层loop中，但是那种做法没有下面这种形式高效，经过我在[quick-bench](http://quick-bench.com)上的测试，下面的这种形式比另外一种形式快1.6倍，测试结果见http://quick-bench.com/oTprU_S6yaNvqI2xpjtBemq9O4w。下面这种方式比较快的原因可能是C++中的**not pay for what you don’t use**，类似于copy-and-swap idiom相较于传统方式的优势。
```
#include <iostream>
#include <vector>
using TableType = std::vector<std::vector<int>>;
void solution(std::vector<int> &Array, TableType &Table) {
    size_t size = Array.size();
    for (int i = 0; i < size; ++i)
        Table[i][i] = Array[i];
    
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = i; j < size; ++j) {
            if (Table[i][j-1] < Array[j])
                Table[i][j] = Table[i][j-1];
            else
                Table[i][j] = Array[j];
        }
    }
}
int main() {
    std::vector<int> Vec{1, 7, 3, 4, 2, 5, 6, 9};
    TableType Table{Vec.size(), std::vector<int>(Vec.size(), 0)};
    solution(Vec, Table);
    return 0;
}
```
### block-based Table
### Sqrt-based Table
我们可以牺牲查询操作的效率，来得到更小的表格需要的存储空间。我们可以将$A[N]$分成几个chunks，存储各chunk的最小值，然后将某次查询经由这些chunk的最小值组合而成，由于我们至多可以将$A$分割成$N$个chunk，所以存储空间至多为$O(N)$。TopCoder直接将$A[N]$分割成了$sqrt(N)$个chunk，并没有解释缘由，GeekforGeeks中有一篇很好关于为什么常常将$Array$分割成$sqrt(N)$的讲解，见[Sqrt (or Square Root) Decomposition Technique | Set 1 (Introduction)](https://www.geeksforgeeks.org/sqrt-square-root-decomposition-technique-set-1-introduction/)。
> 
The key concept of this technique is to decompose given array into small chunks specifically of size sqrt(n).
我们以开头数组为例，选择将$Array$分割成不同的chrunk，如下图所示：
![chunks](https://img-blog.csdn.net/20181013220923983?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rhc2h1bml1bml1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
所以每次查询都可分为下面两种情况，查询的复杂度就可以通过下面两种情况中较大的复杂度决定。
- 查询跨越多个chunk
- 查询只局限在一个chunk中
那么分成几个chunk，才能使查询的最坏复杂度最小呢？答案是将长度为$N$的$Array$分为$sqrt(N)$个chunk时，worst case complexity最小。
#### Why sqrt is perfect?
假如我们将$WC(N, x)$定义为将长度为$N$的数组分割为$x$个chunk的复杂度，那么该函数如下所示：
$WC(N, x) = \begin{cases} N/x, \qquad {\rm if\ } N/x > x \\ x, \qquad \quad \ \ {\rm otherwise} \end{cases}$
当$x$取$sqrt(N)$时，$WC(N, x)$达到最小值，如下图所示，也就是$8/x$与$x$交点的位置。
![函数](https://img-blog.csdn.net/20181013223120318?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rhc2h1bml1bml1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
此时空间复杂度为$O(sqrt(N))$，查询复杂度为$O(sqrt(N))$，构建Table的复杂度是$O(N)$。
#### 泛华形式
### Sparse Table
*注：该小节的标题其实不是很合适，sparse table是一个很宽泛的概念，上一小节中的block-based table其实也可以算作这一小节中*
现如今针对RMQ中的sparse table就特指文章《The LCA Problem Revisited》中提出的sparse table的方法（注：也是该篇文章首次将LCA问题转换成RMQ问题求解的）。该方法首先也是基于**预先处理原数组，然后使用一个额外的Table存储指定query的值得方式**。
首先我们定义$M{_i}{_,}{_j}$，来表示子数组$A[i...i + 2^j-1]$的最小值的index（从这里可以看到当$j=0$时，表示就是A[i]这一个数组单元），如下图所示。任何关于子数组的query都可以通过两个$M{_i}{_,}{_j}$覆盖。例如$A[2...8]$就可以由$A[2...5]$，亦即$M{_2}{_,}{_2}$，和$A[5...8]$，亦即$M{_5}{_,}{_2}$覆盖。**所以求一个子数组最小值的问题就转化成为求两个预先存储好两个值的最小值的问题**。
![ST solution](https://img-blog.csdn.net/20181021120951782?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rhc2h1bml1bml1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
*注：该图摘于《Faster range minimum queries》*
由于对于数组中的元素$i{_i}{_h}$而言，都有$log_2n$个值要存储，所以
- 空间复杂度为 $O(n * log_2n)$
- 查询时间复杂度为$O(1)$
- 预处理复杂度，使用动态规划来计算的话，复杂度也是$O(n * log_2n)$
下面我们给出，这个$O(n * log_2n)$空间复杂度的动态规划算法。根据上面$M{_i}{_,}{_j}$的定义，转移方程如下：
$M{_i}{_,}{_j} = \begin{cases} M_{i, j-1} \qquad if A[M_{i,j}] <= A[M_{i + 2^{j-1}, j-1}] \\ M_{i+2^{j-1}, j-1}\end{cases} $
*注：示例数组是从下标1开始的*
例如我们要计算$M_{2, 2}$，首先计算得到$M_{2, 1} = 3$，$M_{4,1} = 5$，然后$A[3] < A[5]$，所以$M_{2,2} = M_{2,1} = 3$。
然后介绍一下，给定一个查询$RMQ_A(l, r)$，如何计算出能够覆盖该子数组$A[r...l]$的两个已经存储好的区间。例如我们想要求出$A[2...8]$的最小值，那么首先这个区间长度为$i_{th}...j_{th} = 6$的区间，是否能被一个$2^m$覆盖，如果不能的话，肯定能被两个$2^{m-1}$覆盖。所以，我们首先求$\lfloor log_2(j - i) \rfloor$，也就是求$log_2(j-i)$的下取整，假设这个值为k，那么分别
- 从$A[i]$开始取长度为$2^k$的子数组。也就是$M_{i, k}$
- 从$A[j - 2^k]$开始，长度为$2^k$的数组。也就是$M_{j-2^k,k}$
如下图所示。
![st](https://img-blog.csdn.net/20181021154403166?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rhc2h1bml1bml1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这个算法是2000年由《The LCA Problem Revisited》文章给出，但是并没有过多介绍关于该算法的其它问题，例如我最关心的核心问题
- **为什么使用以2为底，以3或者以4为底是否可行？**
#### Why 2?
首先需要注意的是，基于**Sparse Table**（狭义）的方式，对于数组$A[N]$而言，最少也要需要存储$N$个值，或者$N$个下标。如果存储$N$个值，在我们需要求$A[i...j]$最小值的时候，就需要遍历这N个值，此时就退化成了查询时间为$O(N)$的情况了。
在最开始的时候，我们使用$M_{i,j}$表示$A[i...i + 2^k - 1]$的最小值，此时的空间复杂度是$O(n * log_2n)$，如果我们不使用2作为底，而是用3作为底呢？**例如我们假设$M_{i,j}$表示$A[i...i+3^k-1]$的最小值，此时的空间复杂度为$O(n * log_3n)$，是否可行**？
底为3也是可行的，但无法在$O(1)$时间内完成一次查询。例如我们要取$A[i_{th}....j_{th}]$的最小值，与2为底的时候相同，我们同样要计算$k = \lfloor log_2(j - i) \rfloor$，只是在如下情况下需要不只一次比较。如下图所示，如果我们要取$A[4...11]$，那么取$k = 1$，此时无法完全覆盖$A[4...11]$，而取$k = 2$时，则超出了整个子数组。在我们取$k = 1$时，需要比较$A[M_{4, 1}]$，$M_{7,0}$，$A[M_{8,0}]$和$A[M_{9,1}]$四个值大小，4个值最少也需要三次比较。
![st3](https://img-blog.csdn.net/20181021165259417?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rhc2h1bml1bml1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 空间复杂度为 $O(n * log_3n)$
- 查询时间复杂度为 $O(1)$，无论如何都需要三次比较
- 预处理复杂度，使用动态规划来计算的话，为$O(n * log_3n)$
那么为什么底为2的时候查询时间复杂度为$O(1)$，底为3的时候最坏的查询时间复杂度就为$O(3)$呢？**假设底为$m$，当长度为$m^{k+1}$的子数组不能由两个$m^k$表示时，无论如何都需要其它值来补充**，最坏情况下需要进行关于$2*(m-1)$值的比较。
$m^{k+1} = m^k * (m - 1) + m-1$
![ST-m](https://img-blog.csdn.net/20181021173551648?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rhc2h1bml1bml1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
#### 泛化形式
下面我们提出狭义**sparse table**的泛华形式，取m为底，定义$M_{i, j}$为子数组$A[i...i+m^j-1 ]$的最小值，如果我们想要查询$RMQ_A(l. n)$，我们首先计算$k = \lfloor log_mn\rfloor$，然后取下面两个值：
- 子数组$A[i...i+m^k-1]$的最小值，也就是$M_{i,k}$
- 子数组$A[j-m^k+1...j]$的最小值，也就是$M_{j-m^k+1,k}$
然后再枚举这两个子数组无法覆盖的中间子数组，然后遍历这个子数组，$M_{i,k}以及M_{j-m^k+1,k}$得到最小值，这些值在最坏情况下有$(m-1) + 2$个，最少也需要m次比较。此时：
- 空间复杂度为 $O(n  * log_mn)$
- 查询时间复杂度为 $O(m)$
- 预处理的复杂度为$O(n*log_mn)$
注意对于数组$A[N]$而言，当取底为$N$时，就相当于将原有数组拷贝了一遍作为sparse table，此时空间复杂度为$O(N)$，而查询时的时间复杂度也为$O(N)$。
-------------------------------留坑----------------------------
### RMQ在信息论中的极限
### Segment Tree
## RMQ为什么能够解决LCA问题
### Euler Tour
## RMQ与Tarjan的区别
*ToDo: PAT(Patricia Tree)、Suffix-Tree*
[1]: [https://www.topcoder.com/community/competitive-programming/tutorials/range-minimum-query-and-lowest-common-ancestor/](https://www.topcoder.com/community/competitive-programming/tutorials/range-minimum-query-and-lowest-common-ancestor/)
[2]: [https://blog.csdn.net/dashuniuniu/article/details/78634002](https://blog.csdn.net/dashuniuniu/article/details/78634002)
[3]: [https://en.wikipedia.org/wiki/Range_minimum_query](https://en.wikipedia.org/wiki/Range_minimum_query)
[4]: [https://zhuanlan.zhihu.com/p/30248914](https://zhuanlan.zhihu.com/p/30248914)
[5]: [https://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom](https://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom)
[6]: [https://www.geeksforgeeks.org/range-minimum-query-for-static-array/](https://www.geeksforgeeks.org/range-minimum-query-for-static-array/)
[7]: [https://www.geeksforgeeks.org/sqrt-square-root-decomposition-technique-set-1-introduction/](https://www.geeksforgeeks.org/sqrt-square-root-decomposition-technique-set-1-introduction/)
[8]: [https://www.geeksforgeeks.org/range-minimum-query-for-static-array/](https://www.geeksforgeeks.org/range-minimum-query-for-static-array/)
[9]: The LCA Problem Revisited
