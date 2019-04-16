# 计量经济与时间序列_滞后算子和超前算子L的定义 - 时海涛|Thomas - 博客园




- [博客园](https://www.cnblogs.com/)
- [首页](https://www.cnblogs.com/noah0532/)
- [新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
- [联系](https://msg.cnblogs.com/send/%E6%97%B6%E6%B5%B7%E6%B6%9B%7CThomas)
- [管理](https://i.cnblogs.com/)
- [订阅](https://www.cnblogs.com/noah0532/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





# [计量经济与时间序列_滞后算子和超前算子L的定义](https://www.cnblogs.com/noah0532/p/8449991.html)





1.   为了使计算简单，引入滞后算子的概念：

2.   定义**LYt = Yt-1 ， L2Yt = Yt-2，... ， LsYt = Yt-s**。

3.   也就是把每一期具体滞后哪一期的k提到L的上方，来用一个Yt来标记具体属于哪一个滞后期。默认，Yt-1的上方为1，其实不用写。

4.   一定和一个滞后变量放在一起的，不能单独出现L。

5.   用滞后算子来表示比较方便一些，但是最后要带回去，表示具体的哪一个滞后期。

6   另外，我们把滞后算子的s标记，标记成负数，就为超前算子。比如**L-sYt = Yt+s**

**7   滞后算子的运算：**

**　　(1)   常数的滞后算子为常数：Lc = c**

**　　(2)   分配律也适合于滞后算子，即**

**　　　　(Li + Lj)yt = Liyt + Ljyt = yt-i + yt-j**

**　　(3)   结合律也适合滞后算子，即**

**　　　　LiLjyt = Li(Ljyt) = Liyt-1 = yt-i-j**

**8   反比特征方程，就是用L算子的形式表示多项式，与原方程的解互为倒数。在反比特方程中的|α| > 1 为稳定性。**

**　　1 - a1 - a2L2 - ... - apLp = 0**












