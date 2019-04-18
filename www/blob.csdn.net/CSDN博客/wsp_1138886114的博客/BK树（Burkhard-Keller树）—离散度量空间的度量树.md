# BK树（Burkhard-Keller树）—离散度量空间的度量树 - wsp_1138886114的博客 - CSDN博客





2018年09月09日 17:29:28[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：437
所属专栏：[深度学习—神经网络](https://blog.csdn.net/column/details/27368.html)











- - - - - [一、BK树定义](#一bk树定义)
- [二、BK树构造就过程](#二bk树构造就过程)
- [三、BK树查询相似词：](#三bk树查询相似词)








##### 一、BK树定义

BK树，是一种基于树的数据结构，被设计于快速查找近似字符串匹配，比方说拼写纠错，或模糊查找，当搜索”aeek”时能返回”seek”和”peek”。 

在定义BK树之前，我们需要预先定义一些操作。为了索引和搜索字典，我们需要一种比较字符串的方法。编辑距离（ Levenshtein Distance）是一种标准的方法，它用来表示经过插入、删除和替换操作从一个字符串转换到另外一个字符串的最小操作步数。其它字符串函数也同样可接受（比如将调换作为原子操作），只要能满足以下一些条件。

现在我们观察下编辑距离：构造一个度量空间（Metric Space），该空间内任何关系满足以下三条基本条件：

```
令d(x,y)表示字符串x到y的Levenshtein距离，那么显然：
d(x,y) = 0       当且仅当 x=y （Levenshtein距离为0 <==> 字符串相等）
d(x,y) = d(y,x) （从x变到y的最少步数就是从y变到x的最少步数）
d(x,y) + d(y,z) >= d(x,z) （从x变到z所需的步数不会超过x先变成y再变成z的步数）
```

上述条件中的最后一条被叫做三角不等式（Triangle Inequality）。三角不等式表明x到z的路径不可能长于另一个中间点的任何路径（从x到y再到z）。看下三角形，你不可能从一点到另外一点的两侧再画出一条比它更短的边来。

编辑距离符合基于以上三条所构造的度量空间。请注意，有其它更为普遍的空间，比如欧几里得空间（Euclidian Space），编辑距离不是欧几里得的。既然我们了解了编辑距离（或者其它类似的字符串距离函数）所表达的度量的空间，再来看下Burkhard和Keller所观察到的关键结论。

假设现在我们有两个参数:  

1. query表示我们搜索的字符串 

2. n为待查找的字符串与query距离满足要求的最大距离 

3. 我们可以拿任意字符串A来跟query进行比较，计算距离为d，因为我们知道三角不等式是成立的，则满足与query距离在n范围内的另一个字符转B，其与A的距离最大为d+n，最小为d-n。 
推论如下： 


$\begin{align}\bf{d}(\bf{query}, B) + d(B, A) \geqslant d(query, A)& \Leftrightarrow \bf{d}(query, B) + d(A,B) \geqslant d \tag{1}\\&\Rightarrow \bf{d}(A,B) \geqslant d - d(query, B) \geqslant d - n \tag{2} \\  \end{align}$



$\begin{align}\bf{d}(A, B) \leqslant d(A,query) + d(query, B) & \Leftrightarrow \bf{d}(A, B) \leqslant d + d(query, B) \leqslant d + n  \tag{3} \\ &\Rightarrow \bf{d}(A,B) \geqslant d(query, B) - d(query, A) \tag{4}\\&\Rightarrow \bf{d}(A,B) \geqslant 1 - d \geqslant 0  (query\neq B) \tag{5}\end{align}$

所以， $\min\{1, \bf{d} - n\} \leqslant d(A,B) \leqslant d + n$，这是更为完整的结论。

##### 二、BK树构造就过程

首先我们随便找一个单词作为根（比如GAME）。 

 以后插入一个单词时首先计算单词与根的Levenshtein距离：如果这个距离值是该节点处头一次出现，建立一个新的儿子节点；否则沿着对应的边递归下去。 

 例如，我们插入单词FAME，它与GAME的距离为1，于是新建一个儿子，连一条标号为1的边；  

 下一次插入GAIN，算得它与GAME的距离为2，于是放在编号为2的边下。再下次我们插入GATE，它与GAME距离为1，于是沿着那条编号为1的边下去，递归地插入到FAME所在子树；  

 GATE与FAME的距离为2，于是把GATE放在FAME节点下，边的编号为2。 
![这里写图片描述](https://img-blog.csdn.net/20180909170907468?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


每个节点有任意个子节点，每条边有个值表示编辑距离。所有子节点到父节点的边上标注n表示编辑距离恰好为n。比如，我们有棵树父节点是”book”和两个子节点”rook”和”nooks”，”book”到”rook”的边标号1，”book”到”nooks”的边上标号2。

从字典里构造好树后，无论何时你想插入新单词时，计算该单词与根节点的编辑距离，并且查找数值为d(neweord, root)的边。递归得与各子节点进行比较，直到没有子节点，你就可以创建新的子节点并将新单词保存在那。比如，插入”boon”到刚才上述例子的树中，我们先检查根节点，查找d(“book”, “boon”) = 1的边，然后检查标号为1的边的子节点，得到单词”rook”。我们再计算距离d(“rook”, “boon”)=2，则将新单词插在”rook”之后，边标号为2。

##### 三、BK树查询相似词：

计算单词与根节点的编辑距离d，然后递归查找每个子节点标号为d-n到d+n（包含）的边。假如被检查的节点与搜索单词的距离d小于n，则返回该节点并继续查询。

BK树是多路查找树，并且是不规则的（但通常是平衡的）。试验表明，1个查询的搜索距离不会超过树的5-8%，并且2个错误查询的搜索距离不会超过树的17-25%，这可比检查每个节点改进了一大步啊！需要注意的是，如果要进行精确查找，也可以非常有效地通过简单地将n设置为0进行。  

![这里写图片描述](https://img-blog.csdn.net/20180909192143594?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)






