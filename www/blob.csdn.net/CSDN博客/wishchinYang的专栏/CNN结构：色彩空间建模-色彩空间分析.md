# CNN结构：色彩空间建模-色彩空间分析 - wishchinYang的专栏 - CSDN博客
2017年11月21日 18:12:31[wishchin](https://me.csdn.net/wishchin)阅读数：349
个人分类：[艺术/图像评价																[数学/工具																[图像特征																[ANN/DNN/纤维丛																[AI/ML																[StyleAI](https://blog.csdn.net/wishchin/article/category/7372752)](https://blog.csdn.net/wishchin/article/category/6985098)](https://blog.csdn.net/wishchin/article/category/3109825)](https://blog.csdn.net/wishchin/article/category/1860001)](https://blog.csdn.net/wishchin/article/category/1777763)](https://blog.csdn.net/wishchin/article/category/1649519)
         原文： [色彩空间基础](https://zhuanlan.zhihu.com/p/24214731)
        好一个NB的知乎专栏：[色彩空间基础](https://zhuanlan.zhihu.com/p/24214731)
**第一章：色彩空间基础**
        关于色彩分析，引出了专门的数学基础。整个过程给出了完备的数学阐述，虽然没有试验数据，论述的相当精彩。        
![](https://img-blog.csdn.net/20171121190159997?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       摘抄出一段：  上述色品图摘抄于原文
       自然界本身是没有「颜色」这个属性的，只有对不同波长光线的反射率/透过率，到达人眼中的，显然是一个连续的光谱分布函数。数学上，这是一个无穷维的函数空间（巴拿赫空间）。而人眼内的三种视锥细胞，它们的感光特性曲线相当于是在这个无穷维的函数空间中建立了三个基底。任何一个光谱分布进来，三种视锥细胞被激发。由于色视觉响应的线性性，这一过程相当于光谱分布函数与三个基底做内积，或者说，「投影」到这三个基底上。                
        从这个观点看，人类的色视觉，是相当于在自然界所有颜色的无穷维函数空间中取了一个三维的投影。这个三维空间的基底，既可以是视锥细胞的感光特性曲线（我们的大脑就用的是这套），当然也可以是选取三种颜色的光进行组合（CIE RGB 空间），甚至还可以是用实际中不存在的「光线」进行组合 （CIE XYZ 空间）。既然这几个空间实际上是同一个线性空间，只不过由于选择了不同的基底而有不同的表达形式，那么根据线性代数的结论，这几个空间的表述形式之间，只需要通过矩阵乘法就可以完成转换，这是完全的线性变换。
** 第二章**：[色彩表示与空间转换](https://zhuanlan.zhihu.com/p/24281841)
![](https://img-blog.csdn.net/20171121190432491?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
        感光细胞对不同颜色的感光曲线
         可以看到 L 型视锥细胞与 M 型视锥细胞的感光曲线差别很小，实际上这两种视锥细胞起源于一次基因变异，在这之前人类可都是红绿色盲呢，多亏这个基因变异，让人类可以看到更加多彩的世界——这又是一个庞大的话题了，就此打住。
        总之，大自然的这千千万万种颜色，在人类的眼里看到，最后传送到大脑里的信号，就只有这三种视锥细胞的电信号而已。根据这三种电信号的强弱，大脑解读成了不同的颜色。这就是三原色理论的生物学依据。
        不仅如此，人类眼睛对不同颜色光线混合的反应还是 **线性** 的。根据 [格拉斯曼定律（Grassmann's Law）](http://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Grassmann%2527s_law_%28optics%29)，两束不同颜色的光 ![C_1](http://www.zhihu.com/equation?tex=C_1) 和![C_2](http://www.zhihu.com/equation?tex=C_2)，假设某个视锥细胞对他们的反应分别是 ![r_1](http://www.zhihu.com/equation?tex=r_1) 和![r_2](http://www.zhihu.com/equation?tex=r_2)，现在将他们按照一个比例混合，得到第三种颜色![C_3 = \alpha C_1 + \beta C_2](http://www.zhihu.com/equation?tex=C_3+%3D+%5Calpha+C_1+%2B+%5Cbeta+C_2)，那么视锥细胞对这个混合颜色的反应也将是前两个反应的线性叠加![r_3 = \alpha r_1 + \beta r_2](http://www.zhihu.com/equation?tex=r_3+%3D+%5Calpha+r_1+%2B+%0A%5Cbeta+r_2)。
**格拉斯曼定律**是一个实验规律，并没有物理或者生物学上的依据。然而这个规律大大简化了我们对人类彩色视觉系统的建模，并且给我们使用线性代数理论分析人类彩色视觉系统提供了一个前提和基础。
![](https://img-blog.csdn.net/20171121190453464?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20171121190508075?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
                   CIE -RGB空间曲线                                                                                        CIE-XYZ空间曲线
为了消除部分CIE-RGB空间 负数坐标，我们变换到 CIE XYZ 空间，满足一些约束条件：
- 所有坐标都是正的保持等能点（equal energy point）作为白色
- 使得新的 Y 坐标能够代表明度，也就是使得新的 Y 坐标等于视觉的明度响应
- 使得新的 Z 坐标在红光端保持为 0
- 使得所有色彩尽可能充满新的空间
在这些约束条件下，CIE 委员会设计了这两个空间之间最初的转换矩阵：
![\left[\begin{array}{c}    X \\ Y \\ Z  \end{array}\right] =   \left[\begin{array}{ccc}        2.7688 &   1.7517 &   1.1301 \\        1.0000 &   4.5906 &   0.0601 \\         0     &   0.0565 &   5.5942  \end{array}\right]  \left[\begin{array}{c}    R \\ G \\ B  \end{array}\right]](http://www.zhihu.com/equation?tex=%5Cleft%5B%5Cbegin%7Barray%7D%7Bc%7D%0A++++X+%5C%5C+Y+%5C%5C+Z%0A++%5Cend%7Barray%7D%5Cright%5D+%3D+%0A++%5Cleft%5B%5Cbegin%7Barray%7D%7Bccc%7D%0A++++++++2.7688+%26+++1.7517+%26+++1.1301+%5C%5C%0A++++++++1.0000+%26+++4.5906+%26+++0.0601+%5C%5C%0A+++++++++0+++++%26+++0.0565+%26+++5.5942%0A++%5Cend%7Barray%7D%5Cright%5D%0A++%5Cleft%5B%5Cbegin%7Barray%7D%7Bc%7D%0A++++R+%5C%5C+G+%5C%5C+B%0A++%5Cend%7Barray%7D%5Cright%5D)
以及反变换的矩阵
![\left[\begin{array}{c}    R \\ G \\ B  \end{array}\right] =   \left[\begin{array}{ccc}        0.4185 &  -0.1587 &  -0.0828 \\       -0.0912 &   0.2524 &   0.0157 \\        0.0009 &  -0.0025 &   0.1786  \end{array}\right]  \left[\begin{array}{c}    X \\ Y \\ Z  \end{array}\right]](http://www.zhihu.com/equation?tex=%5Cleft%5B%5Cbegin%7Barray%7D%7Bc%7D%0A++++R+%5C%5C+G+%5C%5C+B%0A++%5Cend%7Barray%7D%5Cright%5D+%3D+%0A++%5Cleft%5B%5Cbegin%7Barray%7D%7Bccc%7D%0A++++++++0.4185+%26++-0.1587+%26++-0.0828+%5C%5C%0A+++++++-0.0912+%26+++0.2524+%26+++0.0157+%5C%5C%0A++++++++0.0009+%26++-0.0025+%26+++0.1786%0A++%5Cend%7Barray%7D%5Cright%5D%0A++%5Cleft%5B%5Cbegin%7Barray%7D%7Bc%7D%0A++++X+%5C%5C+Y+%5C%5C+Z%0A++%5Cend%7Barray%7D%5Cright%5D)
虽然最开始是从 CIE RGB 转换到 CIE XYZ 空间的，但之后由于历史原因和技术原因，使得 CIE XYZ 空间更为广泛接受，逐渐作为更常用的转换空间。在最新的官方资料中，只保留了 XYZ 空间的色匹配函数，已经没有 RGB 空间的色匹配函数了。
![](https://img-blog.csdn.net/20171121190525358?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
                             CIE-XYZ空间色彩的正交性质：各个颜色在CIE-XYZ空间中的分布
        这个曲线毕竟在三维空间中，是一条极度扭曲的曲线，不论是用来展示还是用于辅助计算都不方便。我们可以将这条曲线投影到 ![x+y+z=1](http://www.zhihu.com/equation?tex=x%2By%2Bz%3D1) 的平面上（上图中的黄色平面）进行「归一化」，在这个归一化的平面上，光谱色曲线就是一条形状像舌头的曲线，如上图所示位于黄色平面上的弯曲的曲线。根据格拉斯曼定律，所有的颜色都是由纯光谱色混合而来的，经过投影后只能是光谱色的「锥组合」，都会落在这个曲线范围内。这个舌头形状的图，就叫「色品图（Chromaticity Diagram）」，所有纯光谱色，构成色品图的边界。
         各个颜色区间的转换公式，参考原文：[色彩表示与空间转换](https://zhuanlan.zhihu.com/p/24281841)
**第三章**： [RGB是色彩空间唯一的正交基吗](https://www.zhihu.com/question/24886171)? 第一个回答                       
        在大量实验基础上，人们得到了格拉斯曼定律 [Grassmann's law (optics)](https://link.zhihu.com/?target=http%3A//en.wikipedia.org/wiki/Grassmann%2527s_law_%28optics%29) 根据这个定律，颜色空间是满足线性性质的。幺元很简单就是数字1。那么构成线性空间还需要几个基本的条件：零元和加法逆元。
零元很简单就是全黑，加法逆元是不好理解的，因为光线只能叠加，不能相减。也就是说在色彩空间里面不存在“减法”。但是在实验中可以定义一个变相的“减法”。
实验中比较两个颜色是否相等，是用两个颜色的光分别照射两块用不透光隔板隔开的白色物体，如果人眼不能分辨差别，那么认为两个颜色相等。比如说一个颜色 C1 可以由 R1, G1, B1 三种颜色的光混合而成，是什么意思呢，意思是用 C1 颜色的光照射一个白板的左边，用 R1, G1, B1 三种颜色照射白板的右边，中间用不透光隔板隔开，结果人眼看不出中间的分界线，认为两边是一样的，于是我们认为 C1 = R1+G1+B1
        在这个意义上，我们的“减法”，比如 C1-C2=C3 这样的形式，可以转化成 C1=C2+C3，也就是左边照射 C1 的光线，右边照射 C3 的光线，然后把 C2 的光线照射到右边去，这样就在实验意义上有了“减法”的手段。
王赟的答案中提到
> 以人类为例，人类感受到的三原色的强度，可以理解成光的能量谱（光谱的模方）与上面图中的敏感性曲线的**内积**。
实际上确实如此，上面的格拉斯曼定律就是这个所谓“内积”的具体表现。在 CIE 定义的色彩空间中，通过“色匹配函数”来表现纯光谱色的内积分量 [CIE 1931 color space](https://link.zhihu.com/?target=http%3A//en.wikipedia.org/wiki/CIE_color_space%23Definition_of_the_CIE_XYZ_color_space)，在这个 wiki 页面中大家可以看到色匹配函数是有小于零的值的，这就是通过上面那样的“减法”定义出来的。
![](https://pic3.zhimg.com/50/afdc9d40be5ae389e583d4df0ea3c102_hd.jpg)当然，为了在实际中使用，我们普通的 RGB 空间是将上述的 CIE 1973 色彩空间做了线性变换，得到一个所有分量都大于零的色彩空间。既然是线性变换，那么得到的 RGB 空间仍然是一个线性的。         ![](https://img-blog.csdn.net/20171121191554621?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
当然，为了在实际中使用，我们普通的 RGB 空间是将上述的 CIE 1973 色彩空间做了线性变换，得到一个所有分量都大于零的色彩空间。既然是线性变换，那么得到的 RGB 空间仍然是一个线性的。
我在另一个答案 [为什么红色和紫色波长相差最大，但看起来却是相近的？](http://www.zhihu.com/question/21072321/answer/17107328?group_id=762566651) 里提到过
> 那么这些不同波长的光混合之后能呈现怎样的颜色呢？根据实验，我们假定人眼在颜色响应方面是线性的（[Grassmann's law (optics)](https://link.zhihu.com/?target=http%3A//en.wikipedia.org/wiki/Grassmann%2527s_law_%28optics%29)），所以这些光混合之后所能呈现的颜色，就是这条曲线「内部」所代表的空间（更严谨一点叫做「凸组合」）。所以就有了舌形图下部的那一条直线。也就是说，这条直线上的颜色，都不是单纯的光谱色，都是混合出来的。
而我们的显示器，则是在这个空间内部，选了三个点，来「围住」尽可能大的面积。在这个三角形内部的颜色都是显示器可以显示的，而这个三角形外部的颜色则是显示器显示不了的。
![](https://pic3.zhimg.com/50/c95e7ed805b01c7fa1c4a43876f2f23a_hd.jpg)![](https://img-blog.csdn.net/20171121191612736?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
          这里要强调一下是“凸组合”，因为光线没有办法相减，上面实验中的“减法”也不过是一种变通，因此只能是凸组合（也就是围出来的凸多边形，不能到达凸多边形以外的地方）
        可见，只要在这个色度图里面任意取若干点，就能组成由这几个顶点组成的凸多边形内部所有的颜色了。之所以采用RGB三种颜色，从这张图上也可以看出来，由这三种颜色围出来的三角形能包含更多的颜色而已。
         当然，为了实现更多的颜色，现在已经有很多四色的激光和OLED显示器了，色彩表现比三色的要丰富很多。
