# 对称群与正曲率—Karsten Grove's symmetry program - 知乎
# 


写在开头的话：这篇文章是我这阵子看的一些数学论文的一个小结。**看懂这篇文章需要黎曼几何和李群理论的基础知识，慎入**。
事情的起因是上世纪80年代，项武义和他的学生Kleiner证明了如下一个定理：

定理（Hsiang-Kleiner）如果一个单连通紧4维黎曼流形M^4带正截面曲率sec>0，且M上有一个S^1的等距有效作用（isometric effective action）——换句话说，M的isometry group包含S^1作为一个子群＝M的isometry group的维数是正的＝M上有一个非零的Killing vector field——这个条件也被称为M具有连续对称性（continuous symmetry）。则：M^4同胚于S^4或者CP^2。

注1：Kleiner在他的PhD thesis里只证到了同胚，后来Grove和Wilking把结论加强为equivariant diffeomorphism，也就是搞清楚了M的diffeomorphism type（就是正常的，没有exotic）和群作用的具体方式（实际上就是线性作用）。不过Grove他们的证明用到了Ricci flow和Poincare猜想的解，比Kleiner最初的证明复杂得多。Kleiner原来的证明是通过Donaldson-Freedman理论把证明约化为估计M的欧拉示性数的上界（<=3），然后再约化到M的S^1-fixed point set的欧拉示性数上界，然后用群作用的一些结论，对fixed point set的拓扑进行分类讨论，其间用到了Toponogov comparison定理。具体证明见[https://www.math.upenn.edu/~wziller/math661/LectureNotesLee.pdf](https://link.zhihu.com/?target=https%3A//www.math.upenn.edu/~wziller/math661/LectureNotesLee.pdf)［1］ 的第4小节。

注2: 如果有人熟悉正曲率和Hopf conjeture的话，马上能推导出如下结论：如果S^2 * S^2上有度量满足sec>0, 那么这个度量只有“离散对称性”，也就是它的等距变换群必定是有限群——因为S^2 * S^2不同胚于S^4或CP^2，根据上述定理立得。这就告诉我们，如果要找Hopf conjecture的例子，也就是S^2 * S^2上sec>0的度量的例子，必须去找那些“非常不对称”的度量，而且李群理论在这里也起不到什么作用，因为这里只有有限群作用，有限群的李代数是平凡的。



然后Karsten Grove受此定理启发，和Searle一起研究了更一般的torus action，并得到如下maximal rank theorem  ［2］:

Theorem. Let M be an n-dimensional, closed, connected Riemannian manifold with
positive sectional curvature. Then


(i) symrank(M) <=  [(n + 1)/2].
Moreover,


(ii) Equality holds in (i) only  if M is diffeomorphic to either a sphere, a real or complex
projective space, or a lens space. 

所谓 symrank就是M的isometry group G的rank,也就是G的maximal torus的维数。这个定理说一个带正截面曲率流形的symrank不超过[(n+1)/2]，也就是差不多是M自己维数的一半，[]是高斯取整。并且在取到等号的时候有如(ii)所示的rigidity result. 后来Wilking继续研究了当symrank小于[(n+1)/2]的情况，得到了如下1/2-maximal rank theorem [3]: 

For n ≥ 10, if a closed simply connected positively curved n-manifold M with symmetry rank (i.e. the rank of its isometry group) at least n/4 + 1, then M is homeomorphic to a sphere or a quaternionic projective space or homotopic to a complex projective space (Wilking 2002). 

Wilking证明上述定理的这篇发在Acta Math的论文（参考文献［3］）我尝试着读过，读不太懂，很多拓扑的东西我都不太熟。。不过有意思的是他用到了code theory的一些组合性质的结论，用来估计torus里面某些involutions的数目。

后来，Grove在他的一篇综述性文章( [4] )里提出了如下“指导性原则”，我直接摘录原文：


“Our guiding principle can be expressed as


**Classify or determine the structure of positively/nonnegatively curved manifoldswith large isometry groups.**



By itself this opens a vast and interesting area of which we have so far only seen the beginning.
In addition, partial answers can help solve problems in which initially no isometries are present
(see section 4). Even more importantly perhaps, when pushed to the limit, the above principle
provides a systematic search for new and interesting examples (see section 3).


Let us mention some simple examples of what “large” isometry group G could mean:


• Big dim G ≥ 0, i.e., large degree of symmetry relative to M 

• Big rank G ≥ 0, i.e., large symmetry rank relative to M
• Small dim M ≥ 0, relative to G
• Small dim M/G ≥ 0, i.e., low cohomogeneity

• Small dimM/G−dim Fix(M,G) ≥1,i.e.,low fixed point cohomogeneity

For finite groups G, where all these notion make G small, other notions such as the order |G| of
G, or the minimal number of generators for G or similar invariants for say abelian subgroups of
G can be used to express largeness. ”

大意是说，我们希望寻找一类带正／非负截面曲率的黎曼流形，使得他们的isometry group(相对流形的维数)非常“大”。而这里的"大"，有上面带• 的好几种不同的理解方式, 我就不一一翻译了。**对 对称性非常好、对称群非常大的带正／非负截面曲率的黎曼流形的研究（与分类），从此就被称为Grove's symmetry program**。

列举一下沿着这个方向得到一些结果：

maximal rank theorem和Wilking改进的1/2-maximal rank实际上就是沿着第2个•的结果。对于 最后两个•，也就是low cohomogeneity和low fixed point cohomogeneity，有著名的classification of homogeneous spaces of positive curvature（这个很多人都有贡献，这方面综述文章可以看我老板写的[https://www.math.upenn.edu/~wziller/papers/SurveyMexico.pdf](https://link.zhihu.com/?target=https%3A//www.math.upenn.edu/~wziller/papers/SurveyMexico.pdf)），然后对positively curved cohomogeneity one manifolds的研究，我老板说他和别人合作进行了部分分类，但是还有一些例子不知道到底带不带正曲率，也许可以看看这篇综述文章：[https://www.math.upenn.edu/~wziller/papers/GeometryofCohOneExamples.pdf](https://link.zhihu.com/?target=https%3A//www.math.upenn.edu/~wziller/papers/GeometryofCohOneExamples.pdf)。

对于fixed point homogeneous space的分类，Grove和Searle给出了带正曲率的完全分类，见［7］。

然后再说个中国人的工作。方复全老师2002年写了一篇文章，声称分类了带SU(2)对称的正曲率单连通6维流形(参考文献[8])——某种意义上可以看成Hsiang Kleiner定理的推广：


Theorem A. Let M be a simply connected compact 6-manifold of positive sectional curvature.
If the identity component of the isometry group of M contains a simple Lie subgroup (which contains either a SU(2) or SO(3) ). Then
M =S^6,S^2 ×S^4,CP^3,SU(3)/T^2 or SU(3)//T^2. 

然而，我老板跟我说，**这篇文章的证明是不完全的，因为对群作用进行分类讨论的时候遗漏了一些情形**。。他希望我看看这篇文章能不能填补里面的漏洞，不过我拓扑学得不行，很多地方都看不太明白。。


最后小结一下：Grove's symmetry program是正／非负曲率几何的一个方向。正曲率几何这个大领域，我个人觉得还是很难做得动的。你如果能找到一个新的正截面曲率流形（新的拓扑型哦，不是已经知道的流形上找新的正曲率度量）的例子，估计起码是能发JDG的工作；如果你有一个系统性的方法，能够找到一大堆新的正截面曲率流形，那应该是能发Annals的工作。。我觉得这个领域也是古老的“几何联系拓扑”的思想的延伸：Gauss-Bonnet公式告诉你曲面如果带正曲率，那么它的亏格为0. 那么我们把这种想法推广到高维，看看高维流形如果带正曲率会对它的拓扑带来什么限制——结果发现这个限制好像还挺强的，因为已知的（严格）正截面曲率流形的例子并没有多少。。而Grove's symmetry program的含义呢，在于如果额外告诉你这个正曲率流形还非常对称，你能不能对所有这样的流形进行分类，或者至少say something about their topology。这个方向有很多很多很多工作，我这篇文章列举了一些看上去比较重要的，也是我目前在看、在思考（能不能推广）的一些结果。

今天早上突然想要写这么一篇文章，写得有点零乱，见谅。。


参考文献：

［1］[https://www.math.upenn.edu/~wziller/math661/LectureNotesLee.pdf](https://link.zhihu.com/?target=https%3A//www.math.upenn.edu/~wziller/math661/LectureNotesLee.pdf)

［2］Grove, Searle, Positively Curved Manifolds with Maximal Symmetric Rank, Journal of Pure and Advanced Algebra 91 (1994) 137-142.

[3] Wilking, Torus actions on manifolds
of positive sectional curvature, Acta Math., 191 (2003), 259-297.

[4] Grove, GEOMETRY OF AND VIA SYMMETRIES.

［5］[https://www.math.upenn.edu/~wziller/papers/SurveyMexico.pdf](https://link.zhihu.com/?target=https%3A//www.math.upenn.edu/~wziller/papers/SurveyMexico.pdf)

［6］[https://www.math.upenn.edu/~wziller/papers/GeometryofCohOneExamples.pdf](https://link.zhihu.com/?target=https%3A//www.math.upenn.edu/~wziller/papers/GeometryofCohOneExamples.pdf)

［7］Grove, Searle，DIFFERENTIAL TOPOLOGICAL RESTRICTIONS
CURVATURE AND        SYMMETRY ，JDG 47 1997 530-.

[8] Fuquan Fang, Positively curved 6-manifolds with simple symmetry groups, Anais da Academia Brasileira de Ciências (2002) 74(4): 589–597






?? ?????? ?????? 


