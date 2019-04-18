# StyleAI：色调、感情色彩量化、色彩交流API-PCCS颜色体系 - wishchinYang的专栏 - CSDN博客
2018年09月20日 14:29:42[wishchin](https://me.csdn.net/wishchin)阅读数：486
个人分类：[StyleAI																[推荐/Rank系统																[艺术/图像评价																[图像特征](https://blog.csdn.net/wishchin/article/category/1860001)](https://blog.csdn.net/wishchin/article/category/1649519)](https://blog.csdn.net/wishchin/article/category/7149402)](https://blog.csdn.net/wishchin/article/category/7372752)
        参考文章：[日本色研配色體系PCCS](http://mepopedia.com/forum/read.php?981,20217)。[https://en.wikipedia.org/wiki/Color。](https://en.wikipedia.org/wiki/Color)
       1965年前后人们通过生理学实验验证了Thomas Young的假设，在眼睛中的确存在三种不同类型的锥体。看看这个巴黎大学的知乎回答：[为何人眼选择了390-700nm作为自身的感光范围](https://www.zhihu.com/question/62330761/answer/316074733)？下图：人眼和蜂鸟的感光分布.
![](https://img-blog.csdn.net/20180920150942317?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dpc2hjaGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
        鸟的四种感色细胞，感受波段的比较. [为何人感受的是可见光波段](https://www.zhihu.com/question/67377797/answer/255552215)?这是一个你说的黑是什么黑的故事。
![](https://img-blog.csdn.net/20180920150825786?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dpc2hjaGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
       拥有正常视力的人类的眼睛有三种能感知光的圆锥细胞，它们在短波 (“S”, 420 nm – 440 nm)，中波 (“M”, 530 nm – 540 nm)，长波 (“L”, 560 nm – 580 nm) 三处有最敏感的峰值。这些圆锥细胞成为人类在中等或高度充足的光线中感知颜色的基础；在非常暗的光线中，颜色的视觉感知会减少，在低亮度下，单色“夜晚视力”报告员，我们把它命名为“rod cells”，变得更为敏感。因此，三个参数对应三种圆锥细胞的刺激的水平，原则上描述了所有人类对颜色的感觉。通过分别对三种类型的圆锥细胞的光谱感知渲染三种高效的刺激的数值，来衡量总的光能量频谱；这三个值组成一个光谱的颜色对象的三色定义。这三个参数，”S”, “M”, and “L”，使用了一个三维空间的命名”LMS color space”，是量化人类颜色视觉的许多颜色空间之一。
        基于人类视觉的感知颜色总域分析，基于三感光细胞RGB色彩分析和任意线性和非线性变换都能完备地描述整个色彩域。三种原色RGB为三原色，位于感光峰值区域；而不断进化的HSV标准以数学标准化清晰和更接近于人类直觉线性表达，使用的最为广泛。 而在一些色彩空间中，包括LMS and XYZ spaces，主要被使用的颜色不是真正的感知上的颜色，他们不能被任何光谱生成。
### 色彩交流
色彩交流，从想法到成品或环境是一条相连结的完整的链。这条链子连结着颜色本身（即设计师的感觉）和生产，包括：
         A — 创造性的感觉、概念；
           B — 对现实中环境、物质的视觉分析；
             C — 计划和选择 — 建立最终的颜色概念 — 说明；
                   D — 说明所选的颜色；
                      E — 进行生产，对所选颜色进行控制。
要满足A和B必须有一套建立在人们如何观察和体验色彩的[心理学基础](https://baike.baidu.com/item/%E5%BF%83%E7%90%86%E5%AD%A6%E5%9F%BA%E7%A1%80)上的系统。
要满足B、C、D，要求这套系统中实用的、专业的所选色系配有具不同质地和大小的实样。
要满足E要求在视觉系统、工业生产和控制检测中有某种技术性的联系，需要有一种高质量的色彩标准，能适应各行各业进行准确的色彩交流和配色。
         目前市场上的系统或色卡大多只能满足D和E，都是些检测系统或颜色配色系统，或没有任何说明的色卡。而 NCS系统能满足从A到E的各条，这是参与这个项目的研究者们一直追求的目标。
### 色彩体系
        色彩体系为工业色彩体系，不只是把色彩离散地划分为有限个段和群，同时标准企业也附带了配色颜料以及标准混比方式，并非所有的色彩体系枚举的色彩是视觉上均匀或者数学上均匀的。
        NCS色卡可以通过颜色编号判断颜色的基本属性，如：黑度、彩度、白度以及色相。NCS色卡编号描述的是色彩的视觉属性，与颜料配方及光学参数等无关。而使用HSV分析的潘通色卡和一些NCS色卡公司，给出了标准色卡，同时给出了配色方法以及油墨配方。
       孟塞尔是第一个把色调、明度和色度分离成为感知均匀和独立的尺度，并且是第一个系统地在三维空间中表达颜色的关系。孟塞尔的系统，尤其是其后的再标记法，是基于严格的人类受试者测量的视觉反应，使之具有坚实的实验科学依据。基于人类的视觉感知，孟塞尔的系统熬过了其他现代色彩模式的挑战，尽管在某些领域其地位已被某些特殊用途的模型取代了，如CIELAB（*L*a*b**）和CIECAM02。孟塞尔的系统目前仍然是最广泛使用的系统。
       今天我们看到的孟赛尔系统是孟赛尔公司和OSA（美国光学学会）合作、努力多年之后的修订版。
****       正因为如此，Munsell颜色系统在科学性上，几乎已经没什么好挑剔的。是一个高度成熟、完备的颜色系统。所以它是目前商业、工业、科学领域非常重要的一个颜色体系，尤其美国常见。****
        但是，在艺术性上，它还可以再进一步：
****——我们该如何利用Munsell颜色体系来研究配色规律呢？PCCS的主要着力点就在这里。****
### ******PCCS颜色体系********-色调******
      PCCS(Practical Color-ordinate System)色彩体系，是日本色彩研究所于1964年发布的颜色体系。从它的名字就能看出来，PCCS的主要着眼点，就在于“用”——怎么用，颜色才和谐、好看、有商业价值。
        从色度学色彩量化的上，“色调”概念的意义不大，却有美学的独到之处。
        PCCS主要是以色彩调和为目的的色彩体系。明度和彩度在这里结合成为色调，PCCS就是用色调和色相这两个系统来表示色彩调和的基本色彩体系。PCCS体系同样具有色彩三属性：色相、明度、彩度。
　　 PCCS体系中将色彩分为24个色相、17个明度色阶和9个彩度等级，再将色彩群外观色的基本倾向分为12个色调。多数配色教学中使用这个体系。
![](https://img-blog.csdn.net/20180920151903743?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dpc2hjaGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180920142030862?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dpc2hjaGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### ******PCCS-调色公式******
      PCCS色彩体系直接使用了Munsell色彩体系的HSV概念，配色公式来源于这个配色球，小日本划分的还好啊
![](https://img-blog.csdn.net/20180920142233889?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dpc2hjaGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
