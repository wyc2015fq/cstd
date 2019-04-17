# SimRank--基于结构的相似度度量方法学习笔记 - 蓁蓁尔的博客 - CSDN博客





2016年07月18日 17:24:50[蓁蓁尔](https://me.csdn.net/u013527419)阅读数：3502








详见：Glen Jeh 和 Jennifer Widom 的论文SimRank: A Measure of Structural-Context Similarity∗

## 一、简介
- 目前主要有两大类相似性度量方法： 

    (1) 基于内容(content-based)的特定领域(domain-specific)度量方法，如匹配文本相似度，计算项集合的重叠区域等； 

    (2) 基于链接（对象间的关系）的方法，如PageRank、SimRank和PageSim等。最近的研究表明，第二类方法度量出的对象间相似性更加符合人的直觉判断。- Simrank的基本思想是：如果两个实体相似，那么跟它们相关的实体应该也相似。比如在图一中如果a和c相似，那么A和B应该也相似。
- SimRank的特点：完全基于结构信息，且可以计算图中任意两个节点间的相似度。 
![这里写图片描述](https://img-blog.csdn.net/20160718172159150)

                    图一
## 二、基本公式
- 在普通的同构网络中： 
![这里写图片描述](https://img-blog.csdn.net/20160718162502321)

  其中，s(a,b)是节点a和b的相似度Ii(a)Ii(a)表示a的第i个in-neighbor。参数c是个阻尼系数，它的含义可以这么理解：假如I(a)=I(b)={A}，按照上式计算出sim(a,b)=c*sim(A,A)=c，很明显，c应该大于0小于1，所以c∈(0,1)，论文中c都设为了0.8。还有一种理解是说，两个节点间相隔的节点越多，相似度衰减的越厉害。- 
在二分网络中，如图一所示： 
![这里写图片描述](https://img-blog.csdn.net/20160718163051095)

理解：- 
A和B之间的相似度等于A,B出度间相似度的平均值。 

a和b之间的相似度等于a,b入度间相似度的平均值。

- 
如果A,B分别代表顾客A和顾客B，a,b,c代表三件商品的话。买家A和B的相似度等于他们购买的物品之间相似度的平均值，物品a和b的相似度是购买它们的买家之间相似度的平均值。


## 三、朴素计算方法（矩阵计算）

![这里写图片描述](https://img-blog.csdn.net/20160718163343707)

利用上面的公式即可进行simrank的编码实现，编程实现部分本文不考虑，如果读者感兴趣可以参考博客[http://www.cnblogs.com/zhangchaoyang/articles/4575809.html](http://www.cnblogs.com/zhangchaoyang/articles/4575809.html)
## 四、理解simrank的物理意义（Random Surfer-Pairs Model）
- expected distance，即为两节点间步数的平均值 
![这里写图片描述](https://img-blog.csdn.net/20160718164101366)

u为出发点，v为终点且不为中间节点。引入图二帮助理解，如图中所示，从u节点到节点v有2条路径，公式中t即代表路径，例如t=w1–w2–w3–w4，为u到v的一条路径，l(t)代表路径t的长度，即为t中的步数，此处若t=w1–w2–w3–w4，则l(t)为3。p[t]为选择路径t的概率，它的值为1/o(wi)的乘积，出度的倒数的乘积。———sigma（概率*步数）= 两节点间步数的平均值。 
![这里写图片描述](https://img-blog.csdn.net/20160718164304988)

                              图二- Expected Meeting Distance（EMD） 
![这里写图片描述](https://img-blog.csdn.net/20160718165310027)

Note：分别以相同的速度从a，b节点同时出发，x点为终点，即最后在x节点相遇。 
****存在路径无限长问题**
![图三](https://img-blog.csdn.net/20160718165537090)

     图三 

如在图三（a）中，任意的a，b两点永远不会相遇，路径l(t)无限长，导致相似度无限大。 

针对上面的问题： 

1）参考pagerank方法，给它一个跳出循环的机会，给一个小概率可以跳到循环外某一随机点上。 

2）本文中采用了函数映射的方法，将l(t)映射为指数函数![这里写图片描述](https://img-blog.csdn.net/20160718170141153)并且指数函数可以满足题意。两点相遇所需要的平均步数越大，两点间相似度越小，最小为0；两点间相遇需要的步数为0时，即两点为同一点时，两点间相似度最大，值为1。 
![这里写图片描述](https://img-blog.csdn.net/20160718170323531)
**——所以函数映射以后，引出Expected-f Meeting Distance的概念**- Expected-f Meeting Distance 

define s’(a, b), the similarity between a and b in G based on expected-f meeting distance, as 
![这里写图片描述](https://img-blog.csdn.net/20160718170734004)

其中，c是（0,1）间的常数。 

有了上面的概念以后，我们可以计算： 
![这里写图片描述](https://img-blog.csdn.net/20160718173423435)- Equivalence to SimRank，有了上面这个概念，接下来推导相似迭代形式的s‘(a,b),最后得到实际上：s‘(a,b) = s(a,b)，s(a,b)为上面提到的同构网络中的基本公式。推导过程如下，最终可以得 
![这里写图片描述](https://img-blog.csdn.net/20160718171455953)- **Theorem.** The SimRank score, with parameter C, between two nodes is their expected-f  

meeting distance traveling back-edges, for f(z) =![这里写图片描述](https://img-blog.csdn.net/20160718171634315). 

两节点间simrank相似度的值即为此两节点间期望-f相遇距离，也就是（随机漫步所有的 

可能性）从这两个节点出发到相遇时所需要的步数的平均数。
## 五、回顾总结：

1）simrank的基本思想和主要特点 

2）simrank的基本公式 

3）理解Simrank的物理意义Theorem.














