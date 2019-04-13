
# Hffman Tree - lpty的博客 - CSDN博客

2017年12月11日 10:55:43[lpty](https://me.csdn.net/sinat_33741547)阅读数：612标签：[机器学习																](https://so.csdn.net/so/search/s.do?q=机器学习&t=blog)[算法																](https://so.csdn.net/so/search/s.do?q=算法&t=blog)[二叉树																](https://so.csdn.net/so/search/s.do?q=二叉树&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=算法&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=机器学习&t=blog)个人分类：[机器学习																](https://blog.csdn.net/sinat_33741547/article/category/6482345)
[
																								](https://so.csdn.net/so/search/s.do?q=机器学习&t=blog)



## 一、概念
### 1、什么是Huffman Tree？
又称赫夫曼树、霍夫曼树、哈夫曼树、最优二叉树等，是一类带权路径长度最短的树。
### 2、路径与路径长度
从树中一个结点到另一个结点之间的分支构成这两个结点之间的路径，路径上的分支数目称做路径长度。
### 3、树的路径长度
树的路径长度是从树根到没一个结点的路径长度之和。
### 4、树的带权路径长度
树的带权路径长度为树中所有叶子节点的带权路径长度之和。
### 5、最优二叉树
带权路径长度最短的树称为最优二叉树或Huffman Tree。
### 6、如何构建Huffman Tree？
（1）根据给定的n个权值{w1，w2，...，wn}构建成n颗二叉树的集合F，其中每颗二叉树Ti中只有一个带权为wi的根节点，其左右子树为空。
（2）在F中选取两颗根节点的权值最小的树作为左右子树构造一颗新的二叉树，且置新的二叉树的根节点的权值为其左右子树上根节点的权值之和。
（3）在F中删除这两颗树，同时将新得到的二叉树加入F中。
（4）重复（2）和（3），直到F只含一颗树为止。
### 7、Huffman Tree的作用？
在解决某些判定问题时，可以得到最佳判定算法。Huffman Tree实际上使得权值最高的节点，在树的路径中越靠近根节点，即路径越短，这样越容易被搜索得到，加快速度。
## 二、实例
假设有一个字符串s=（A，B，C，D，E），其中每个字符出现的频数如下：
A：5
B：15
C：40
D：30
E：10
### 1、按每个字符出现频数，由小到大排序：
![](https://img-blog.csdn.net/20171211103914351?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzM3NDE1NDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

### 2、构造二叉树
**（1）第一步**
![](https://img-blog.csdn.net/20171211105023265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzM3NDE1NDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**（2）第二步**
![](https://img-blog.csdn.net/20171211105225033?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzM3NDE1NDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**（3）第三步**
![](https://img-blog.csdn.net/20171211105316513?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzM3NDE1NDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**（4）第四步**
![](https://img-blog.csdn.net/20171211105356721?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzM3NDE1NDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## 三、参考
1、《数据结构》(c语言版)  严蔚敏等

