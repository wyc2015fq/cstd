# Anderson's pointer analysis - 电影旅行敲代码 - CSDN博客
2017年12月04日 00:15:39[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：715
## 指针分析
> 
指针分析是一类特殊的数据流问题，它是其它静态程序分析的基础，但指针使用的灵活性导致了指针分析的复杂性，**实际上指针分析是一个不可判定问题，所以实际的指针分析算法都是近似且保守的，须在效率和精度之间进行折衷**。
指针分析研究的内容主要集中在分析精度和时空开销之间的取舍，精度方面，主要指**流敏感性（*flow-sensitivity*）**和**上下文敏感性（*context-sensitivity*）**，一般而言，流敏感分析方法的精度明显好于流不敏感的分析方法，在上下文敏感性上也有同样的特点。
流不敏感的指针分析普遍使用在开源或者产品级高级编译器中，其中主要有两类：**基于包含（*inclusion-based*）的指针分析**和**基于合并（*unification-based*）的指针分析**。
基于包含的指针分析是一种基于约束集（constraint set）求解的**流不敏感**的指针分析方法，该指针分析又称为基于子集（subset-based）的指针分析或者基于约束的（constraint-based）的指针分析，在指针分析领域后来也被称之为Anderson风格的指针分析。其算法的时间复杂度为O(n3)。
*注：上述内容引用自《陈聪明, 霍玮, 于洪涛,等. 基于包含的指针分析优化技术综述[J]. 计算机学报, 2011, 34(7):001224-1238.》*
## Anderson’s pointer analysis
Anderson的指针分析算法是一种基于约束（constraint-based）或者说基于子集（subset-based）的方法，根据程序中语句建立变量与变量，或者变量与内存之间的约束关系。Anderson的方法分为两步，**约束收集**（或者说约束生成，constraint generation）和**约束求解**（constraint resolution）。
### 约束生成
约束类型分为三种，基本约束，简单约束，复杂约束，并有如下四种具体形式。
|Constraint type|Assignment|Constraint|Meaning|
|----|----|----|----|
|Base|Referencing ( a = &b )|a ⊇ {b}|loc(b)∈pts(a)|
|Simple|Aliasing ( a = b )|a ⊇ b|pts(a) ⊇ ptrs(b)|
|Complex|Dereferencing read ( a = *b )|a ⊇ *b|∀v∈pts(b). pts(a) ⊇ pts(v)|
|Complex|Dereferencing write ( *a = b )|*a ⊇ b|∀v∈pts(a). pts(v) ⊇ pts(b)|
*注：上图中的 loc(b) 表示 b 的地址， pts(a) 表示 a 可能指向的内存的集合*
该方法按照以上四种约束形式收集程序中的约束，如下代码实例：
```
// code sample1
p = &a;
       // p  ⊇ {a}
q = &b;
       // p  ⊇ {a}, q  ⊇ {b}
*p = q;
       // p  ⊇ {a}, q  ⊇ {b}, *p = q
r = &c;
       // p  ⊇ {a}, q  ⊇ {b}, *p = q, r ⊇ {c}      
s = p;
       // p  ⊇ {a}, q  ⊇ {b}, *p = q, r ⊇ {c}, s ⊇ p
t = *p;
       // p  ⊇ {a}, q  ⊇ {b}, *p = q, r ⊇ {c}, s ⊇ p, t ⊇ *p
*s = r;
       // p  ⊇ {a}, q  ⊇ {b}, *p = q, r ⊇ {c}, s ⊇ p, t ⊇ *p, *s ⊇ r
```
### 约束图（constraint graph）
约束图 *G = <Var, E>*，图中的节点为变量或者抽象的内存区域，每一个节点都关联一个对应的point-to sets。 *a->b ∈ E*，当且仅当如下三种情况之一成立：
- *pts*( *a* ) ⊆ *pts*( *b* ) ：{ 基本约束 }
- *a* ∈ *pts*( *v* ) and *pts*( **v* ) ⊆ *pts*( *b* ) ： {复杂约束}
- *pts*( *a* ) ⊆ *pts*( **v* ) and *b* ∈ *pts*( *v* )：{复杂约束}
*注：约束图表示的是**约束关系，不是指向关系**，这一点要搞清楚*
约束图边的添加分为两种，
- 一是在生成约束信息的时候，根据 **基本约束** 直接添加的边
- 一是在约束求解的过程中，根据 **复杂约束** 进行“推导”添加的边
对于代码示例1，约束信息收集完时的约束图如图-1所示，该图称之为初始约束图，**基本约束**和**简单约束**是创建初始约束图的基础。初始约束图的创建分为如下三步：
- 首先为程序中的每个 变量建立一个节点
- 后根据基本约束标注节点的 指向集
- 每一个初始的简单约束建立一条有向边
![这里写图片描述](https://img-blog.csdn.net/20171203230037545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZGFzaHVuaXVuaXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
 图-1 Constraint Graph 
*注：图中的边也可以理解成 **source node 的 point-to information 需要传递到 destination node** ，例如图中的p->s，表示p指向的内存区域，s也应该指向。所以我们需要将p节点的point-to information传播到s节点。*
对于第二种边，我们不能从约束信息中直接获得，需要对complex constraints进行“推导”才能获得。complex constraint会产生新的base constraint。我们的最终目的不是获得约束图，而是在创建约束图的过程中将 *point-to information* 进行正确的“传递”。这些*point-to information* 按照下面的原则进行传播。
> 
**如果 *loc* ∈ *pts*( *a* ) 并 ( *a*->*b* ∈ *E*)，那么我们就可以得到( *loc* ∈ *pts*( *b* ))。**
### Anderson算法
采用的是一个工作队列算法（workque algorithm），该算法我直接从[[2]](https://www.seas.harvard.edu/courses/cs252/2011sp/slides/Lec06-PointerAnalysis.pdf)中摘抄过来，如图-2所示。
![Anderson's algorithm](https://img-blog.csdn.net/20171203212913298?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZGFzaHVuaXVuaXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
*注：更完善的算法见[1]*
上述算法中我们有两点需要注意：
- 边的添加。边的添加有两种情况，分别是***a* ∈ *pts*( *v* ) and *pts*( **v* ) ⊆ *pts*( *b* )** 和 ***pts*( *a* ) ⊆ *pts*( **v* ) and *b* ∈ *pts*( *v* )**，这两个都是**复杂约束**。
- point-to information的传播。point-to information才是我们最终欲得到的信息。
关于代码实现《[指针分析/Point-to Analysis/Reference Analysis](http://blog.csdn.net/majestyhao/article/details/49961585)》有一个比较初级的实现，这里我就不贴代码了，该博客也有关于算法的示意图。
我这里使用另外一个代码示例[[4]](https://www.youtube.com/watch?v=erIkdIwypbE&t=38s)来说明整个算法过程，如下所示。
```
// code sample 2
int i, j, k;
int *a = &i;
           // a ⊇ {i}
int *b = &k;
           // a ⊇ {i}, b ⊇ {k}
a = &j;
           // a ⊇ {i, j}, b ⊇ {k}
int **p = &a;
           // a ⊇ {i, j}, b ⊇ {k}, p ⊇ {a}
int **q = &b;
           // a ⊇ {i, j}, b ⊇ {k}, p ⊇ {a}, q ⊇ {b}
p = q;
           // a ⊇ {i, j}, b ⊇ {k}, p ⊇ {a}, q ⊇ {b}, p ⊇ q
int *c = *q;
           // a ⊇ {i, j}, b ⊇ {k}, p ⊇ {a}, q ⊇ {b}, p ⊇ q, c ⊇ *q
```
将Anderson`s algorithm应用到上述代码的整个过程如图-3所示。最后按照约束图中节点的point-to sets，按照指向关系，构建内存指向图。 
![Anderson's algorithm示意图1](https://img-blog.csdn.net/20171204000610770?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZGFzaHVuaXVuaXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![Anderson's algorithm示意图2](https://img-blog.csdn.net/20171204001343209?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZGFzaHVuaXVuaXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
图-3 代码示例2初始约束图
*注：最后一幅图中红色线条表示内存的指向关系*
当算法迭代终止的时候，我们根据约束图中节点的point-to sets，建立各个节点间的指向关系，如最后一幅图所示。
*未来工作*
*Anderson’s algorithm优化*
*llvm-2.4 Andersons.cpp源码分析*
*clang static analyzer中Anderson’s pointer analysis实现*
[1]. [陈聪明, 霍玮, 于洪涛,等. 基于包含的指针分析优化技术综述[J]. 计算机学报, 2011, 34(7):001224-1238.](http://xueshu.baidu.com/s?wd=paperuri:%28b0ede741642b1ff3e994c4b76f6b42aa%29&filter=sc_long_sign&sc_ks_para=q=%E5%9F%BA%E4%BA%8E%E5%8C%85%E5%90%AB%E7%9A%84%E6%8C%87%E9%92%88%E5%88%86%E6%9E%90%E4%BC%98%E5%8C%96%E6%8A%80%E6%9C%AF%E7%BB%BC%E8%BF%B0&tn=SE_baiduxueshu_c1gjeupa&ie=utf-8&sc_us=15287860218014106038)
[2]. [Pointer Analysis. CS252r Spring 2011](https://www.seas.harvard.edu/courses/cs252/2011sp/slides/Lec06-PointerAnalysis.pdf)
[3]. [Andersen L. Program analysis and specialization for the C programming language[J]. Addison-Wesley Series in Computer Science, 1994, 2(1):37 - 77.](https://www.cs.cornell.edu/courses/cs711/2005fa/papers/andersen-thesis94.pdf)
[4]. [指针分析/Point-to Analysis/Reference Analysis](http://blog.csdn.net/majestyhao/article/details/49961585)
[5]. [#63 Andersen’s Points to Analysis](https://www.youtube.com/watch?v=erIkdIwypbE&t=38s)
