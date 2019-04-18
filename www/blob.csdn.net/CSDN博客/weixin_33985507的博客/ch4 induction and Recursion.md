# ch4 induction and Recursion - weixin_33985507的博客 - CSDN博客
2017年10月30日 19:25:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
## mathematical induction
#### 定义
**数学归纳法**（**Mathematical Induction**、**MI**、**ID**）是一种[数学证明](https://link.jianshu.com?t=https://zh.wikipedia.org/wiki/%E6%95%B0%E5%AD%A6%E8%AF%81%E6%98%8E)方法，通常被用于证明某个给定[命题](https://link.jianshu.com?t=https://zh.wikipedia.org/wiki/%E5%91%BD%E9%A2%98)在整个（或者局部）[自然数](https://link.jianshu.com?t=https://zh.wikipedia.org/wiki/%E8%87%AA%E7%84%B6%E6%95%B0)范围内成立。除了自然数以外，[广义](https://link.jianshu.com?t=https://zh.wikipedia.org/wiki/%E5%B9%BF%E4%B9%89)上的数学归纳法也可以用于证明一般[良基](https://link.jianshu.com?t=https://zh.wikipedia.org/wiki/%E8%89%AF%E5%9F%BA%E5%85%B3%E7%B3%BB)结构，例如：[集合论](https://link.jianshu.com?t=https://zh.wikipedia.org/wiki/%E9%9B%86%E5%90%88%E8%AE%BA)中的[树](https://link.jianshu.com?t=https://zh.wikipedia.org/w/index.php?title=%E6%A0%91_(%E9%9B%86%E5%90%88%E8%AE%BA)&action=edit&redlink=1)。这种广义的数学归纳法应用于[数学逻辑](https://link.jianshu.com?t=https://zh.wikipedia.org/wiki/%E6%95%B8%E5%AD%B8%E9%82%8F%E8%BC%AF)和[计算机科学](https://link.jianshu.com?t=https://zh.wikipedia.org/wiki/%E8%AE%A1%E7%AE%97%E6%9C%BA%E7%A7%91%E5%AD%A6)领域，称作[结构归纳法](https://link.jianshu.com?t=https://zh.wikipedia.org/wiki/%E7%BB%93%E6%9E%84%E5%BD%92%E7%BA%B3%E6%B3%95)。
虽然数学归纳法名字中有“归纳”，但是数学归纳法并非[不严谨](https://link.jianshu.com?t=https://zh.wikipedia.org/wiki/%E4%B8%A5%E8%B0%A8%E6%80%A7_(%E6%95%B0%E5%AD%A6))的[归纳推理法](https://link.jianshu.com?t=https://zh.wikipedia.org/wiki/%E5%BD%92%E7%BA%B3%E6%8E%A8%E7%90%86)，它属于完全[严谨](https://link.jianshu.com?t=https://zh.wikipedia.org/wiki/%E5%9A%B4%E8%AC%B9)的[演绎推理法](https://link.jianshu.com?t=https://zh.wikipedia.org/wiki/%E6%BC%94%E7%BB%8E%E6%8E%A8%E7%90%86)。事实上，所有[数学证明](https://link.jianshu.com?t=https://zh.wikipedia.org/wiki/%E6%95%B8%E5%AD%B8%E8%AD%89%E6%98%8E)都是演绎法。
## Strong Induction
与之相对的就是weak induction 。weak induction只利用了P(1)和P(n)来证明 P(n + 1)，而Strong Induction 则前提条件成了 P(begin)~P(n)都成立，则证明出P(
n +1)
我们在最一开始肯定会想weak induction不是已经说明了P（begin）～P（n）之间都成立了吗，Strong Induction和weak Induction之间有什么区别呢，其实我看了一些博客目前认为Strong induction和weak induction之间的区别主要是Strong inducton在证明递归成立的时候不止利用了p（n）而且利用了之前的项，我们可以选择去用之前的哪一项而不必要都用上，所以strong induction并不是证明的比weak induction更正确，而是说strong induction比 weak induction证明起来更简单，或者说能够利用的条件更多，使得很多用weak induction难以证明的用strong induction证明起来更加简单
这里给出一个例题（取自国外大神的博客）
![7498199-99c578aef3702032.png](https://upload-images.jianshu.io/upload_images/7498199-99c578aef3702032.png)
11.png
![7498199-45e4df0726d18b31.png](https://upload-images.jianshu.io/upload_images/7498199-45e4df0726d18b31.png)
12.png
