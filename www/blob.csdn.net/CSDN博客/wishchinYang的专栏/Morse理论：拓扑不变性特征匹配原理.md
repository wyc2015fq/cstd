# Morse理论：拓扑不变性特征匹配原理 - wishchinYang的专栏 - CSDN博客
2016年12月21日 13:26:11[wishchin](https://me.csdn.net/wishchin)阅读数：1055
设计精美的宽基线双目相机镇文
![](https://img-blog.csdn.net/20161221131949228?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
Mo’ersi lilun
莫尔斯理论(卷名：数学)
Morse theory
　　微分拓扑的一个重要分支。通常是指两部分内容：一部分是微分流形上可微函数的莫尔斯理论，即临界点理论；另一部分是变分问题的莫尔斯理论，即大范围变分法。确切地说，假设*ƒ*是*n*维微分流形*M*上的实值可微函数，*ƒ*的临界点*p*是指梯度向量场grad*ƒ*的零点，即在局部坐标下使得的点。*ƒ*的全部临界点的性态与流形*M*本身的拓扑结构有密切的关系，探索这些关系就是临界点理论的主要任务。例如，著名的莫尔斯不等式就是这样一种关系：
*M*0≥*R*0
*M*1-*M*0≥*R*1-*R*0，
                ……
*Mk*-*Mk*-1+…±*M*0≥*Rk*-*Rk*-1+…±*R*0，
                ……
*Mn*-*M*n-1+…±*M*0=*Rn*-*R*n-1+…±*R*0，
式中*Rk*是*n*维闭流形*M*的*k*维模2贝蒂数，即同调群*hk*(*M*，*Z*2)的秩，*Mk*是*M*上非退化函数*ƒ*的指数为*k*的临界点的个数。这里说*ƒ*是非退化函数，是指*ƒ*的任何临界点*p*均非退化，即在局部坐标下*ƒ*在*p*处的黑塞矩阵![](https://img-blog.csdn.net/20161221132305870?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center) 
 之秩为*n*；这个矩阵的负特征值的个数称为临界点*p*的指数。莫尔斯不等式是H.M.莫尔斯本人在20世纪20年代建立的基本结果，后来有了远为一般的结果。
![](https://img-blog.csdn.net/20161221132340323?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20161221132506483?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
例如，考虑图1[](http://202.204.27.227:918/temp/trs_tmp1430_15971.htm)中环面*M*关于水平切面*V*的高度函数*ƒ*:*M*→*R*，其中*p*，*q*，*r*，*s*是*ƒ*的四个非退化临界点，其指数分别为0，1，1，2，因为可以适当选择局部坐标，使得在*p*的邻近*ƒ*=*ƒ*(*p*)+*x2*+*y2*(旋转抛物面)，在*q*的邻近*ƒ*=*ƒ*(*q*)-*x2*+*y2*(鞍面)，在*r*的邻近*ƒ*=*ƒ*(*r*)-*x2*+*y2*（鞍面），在*s*的邻近*ƒ*=*ƒ*(*s*)-*x2*-*y2*(旋转抛物面)。命不难看出，当*α*由小变大经过各个临界值时，*M*α的同伦型发生表[](http://202.204.27.227:918/temp/trs_tmp1433_31321.htm)中所列的变化。
　　可见，当*α*从小变大经过指数为*λ*的临界点时，*M*α的同伦型变化相当于粘上一个*λ*维胞腔，从而整个环面*M*的同伦型相当于由一个
 0维胞腔、两个一维胞腔以及一个二维胞腔组成的CW复形，这样就把*M*的同伦型与*ƒ*的临界点的性态联系起来了。如果把这个事实推广到一般情形就是：
临界点理论的基本定理　命*M*是微分流形，*ƒ*:*M*→***B***是非退化函数，并且任何*M*α都是紧致集。于是，每个*M*α都具有一个有限CW复形的同伦型，从而整个*M*具有一个至多是可数的CW复形的同伦型：对于指数为*λ*的每个临界点，这个复形有一个*λ*维胞腔。
　　临界点理论的应用中最完美的是对测地线问题的应用，这就是变分学的莫尔斯理论。例如，考虑完备黎曼流形*M*上两个固定端点*p*和*q*之间的测地线问题，即是使弧长为极小的变分问题：
式中*ω*:[0，1]→*M*表示*M*上的逐段光滑道路，*ω*(0)=*p*，*ω*(1)=*q*；这个变分问题的泛极线就是所谓测地线。于是，从*p*到*q*的所有光滑测地线的性态与流形*M*的拓扑结构之间是否有什么关系，这就是大范围变分学要研究的主要问题，可以应用临界点理论的框架得到相似的结果。命*Ω*=*Ω* (*M*；*p*，*q*)表示*M*上从*p*到*q*所有逐段光滑道路组成的空间，具有尺度拓扑。
式中ρ 
表示*M*上由黎曼尺度导出的距离函数；表示*ω*上的弧长。
大范围变分学基本定理　命*M*是完备黎曼流形，*p*，*q*∈*M*沿任何测地线不共轭，则*Ω*(*M*；*p*，*q*)具有可数CW复形的同伦型：对于从*p*到*q*每条指数为*λ*的测地线，这个复形有一个*λ*维胞腔。
　　随着拓扑学的发展，莫尔斯理论本身也有很大的飞跃。例如，由于临界点定义为梯度向量场grad*ƒ*的零点，自然可以考虑*n*维闭流形*M*上一般向量场*X*的零点与*M*的拓扑结构之间的关系，即*M*上的动力系统
![](https://img-blog.csdn.net/20161221132120838?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
的奇点与*M*的拓扑结构的关系。S.斯梅尔在某些假设下得到了形式相同的莫尔斯不等式，不过这时*Mk*=*αk*+*bk*+*b*k+1，*αk*表示向量场*X*的*k*型零点的个数，*bk*表示*k*型闭轨线的条数。斯梅尔正是在这个基础上完成了他关于高维庞加莱猜想的卓越工作，这是微分拓扑学的重大成就之一。其次，由于测地线问题是一维变分问题，本来是无限维的空间*Ω*才能化为有限维流形应用临界点理论来处理。但一般的多维变分问题就无法做到这一点，因而要求发展无限维流形上的临界点理论，直接处理相应的无限维空间*Ω*，从而把原来的两个方面统一起来。
参考书目
J.Milnor 著，江嘉禾译：Morse理论，《数学译林》，北京，1980～1981。(J.
 Milnor，*Morse Theory，Ann. Math. Studies，*Princeton
 Univ. Press，Princeton，1963.)
H.赛弗尔、W.施雷法著，江嘉禾译:《大范围变分学》，上海科学技术出版社，上海，1963。(H.Seifert
 und W.Threlfall，*variationsrechnung im Grossen，*Chelsea Pub.Co.，1948.)
S.Smale， Morse Inequalities for a  Dynamical Systems，*Bull. Amer. Math.Soc.，*Vol.
 66， pp.43～49，1960.
R.S.Palais， Morse Theory on Hibert Manifolds，*Topology，*Vol.2，pp.
 299～340，1963.
R.S.Palais and S.Smale，A Generalized MorseTheory，*Bull. Amer. Math. Soc.，*Vol.70，*pp.*165*～*172，1964.
江嘉禾
