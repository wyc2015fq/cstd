# 3D打印革命性升级！只要光照几十秒，完美雕像浮出水面丨Science - 知乎
# 



> 郭一璞 问耕 假装发自 凹非寺
量子位 出品 | 公众号 QbitAI

你见过的那些3D打印机都过时了。



![](https://pic1.zhimg.com/v2-9a8f3d5b4fd617bf40f68d34597b493c_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='320'></svg>)



传统的3D打印，像往蛋糕上挤奶油一般，照着目标的样子，一层一层挤出来。

不光动作慢，而且外表粗糙，产品表面总有横躺的一根一根的痕迹。



![](https://pic1.zhimg.com/v2-7c922651163a8352806440b6837efef4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='637' height='580'></svg>)



这实在太不完美了。

但现在，一份刚刚发表在Science上的研究提供了全新的3D打印技术：

将液体材料注入烧杯。



![](https://pic1.zhimg.com/v2-d0aa4f780949d78fb909b59f97bae5fc_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='501'></svg>)



以光为刀，简单照射。



![](https://pic2.zhimg.com/v2-01ef460845c3dfd836b91bbbed1213ad_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='375'></svg>)





![](https://pic1.zhimg.com/v2-b5e333a7462abf870487435748e24980_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='311'></svg>)



需要打印的物体就在水中出现。



![](https://pic2.zhimg.com/v2-93848c6f23189f5d0f59216380785fb9_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='311'></svg>)





![](https://pic2.zhimg.com/v2-39a731af6299d6889ab35803c6710085_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='736'></svg>)



而且，打印出来的思想者雕塑，整体圆润，表面光滑，全然不见那些参差的表面纹理。

简直就是《星际迷航》中的复制机啊！



![](https://pic2.zhimg.com/v2-d0cb44662869f45f1b5ee64b1e9ee3a9_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='320' height='240'></svg>)



## **凭空光刻**

而利用光聚合反应，能够合成任意的几何形状，开头出现的神奇一幕，就是通过这个原理最终完成了3D打印。

研究人员为此开发了一种计算轴向光刻（CAL）方法。

CAL制造系统能够选择性的固化容器内的光敏液体（甲基丙烯酸酯明胶水凝胶）。这个系统把一组二维图像，从不同的角度投射出来，这种多角度的曝光叠加，可以让光敏液体固化成所需要的几何形状。

以打印“思想者”雕像为例，简单说一下。



![](https://pic1.zhimg.com/v2-004383d7f2aa625ef478c2ae059aa8f0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='644'></svg>)



整套系统大体上就是下面这个样子。

光从一个商用的DLP投影仪里射出，投向一个中间持续旋转的系统，里面装有光敏液体。




![](https://pic3.zhimg.com/v2-ab5a5a2680626c319d265605e884eea6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='548'></svg>)





![](https://pic2.zhimg.com/v2-cffdfd5aa85721efeaaa618ddf229b1d_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='311'></svg>)



在不到一分钟的时间里，思想者就在液体里逐渐成型了。




![](https://pic1.zhimg.com/v2-b3dbed96c82b498eb22a1ca9e3846e78_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='355'></svg>)



这个系统的核心就是CAL算法。


还是以“思想者”为例，想要真正完成打印并不容易。首先得进行三维模型的重建，包括2D的傅里叶变换。然后还得针对光敏液体的剂量进行优化计划等等。



![](https://pic4.zhimg.com/v2-0dc8d4c8b4accdc14f400b749799110f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='824' height='1294'></svg>)



其中涉及的公式之一如下：




![](https://pic4.zhimg.com/v2-19c4c37096294a2d04f0b61b7402f14f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1052' height='100'></svg>)



这些参数包括材料充分凝固的临界剂量、树脂的光学吸收系数、容器的旋转速率径向投影强度、光敏材料的衰减等等。


## **快速灵活**

之前提到，普通的3D打印，主要是通过逐层堆叠一维或者二维单元的方式完成，但这种方法的速度有限，而且降低了表面质量，有可能导致机械性能的各向异性。

速度快是这套系统的优势之一，而且还能使用不同的材料完成打印。

在“思想者”这个案例中，在不同的精度和材料条件下，打印时间为30-120秒。这套系统的最高精度，目前可以达到0.3毫米。

能打印的东西种类很多。

比方复杂度很高的牙科模型、晶格结构。



![](https://pic1.zhimg.com/v2-665a9175045c76d945bc3c92dfc46f68_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='276'></svg>)



还有飞机模型、水晶球等……




![](https://pic3.zhimg.com/v2-59808f32f962f133f6a739ac50c88afe_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='570'></svg>)



打印出来的物体甚至可以具备弹性。




![](https://pic4.zhimg.com/v2-872cfb7a2be02b43786ce5181282a48f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='551'></svg>)



甚至还能在已有物体上打印，譬如为一个螺丝刀杆，打印一个刀把。




![](https://pic4.zhimg.com/v2-ce4d4a77b3635341c57ac378c7a5ac8f_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='311'></svg>)





![](https://pic3.zhimg.com/v2-6a53102e453c26e804852d9667e216ba_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='375'></svg>)



在打印过程中，还涉及氧气抑制、光场干涉、3D空间和2D投影之间的转换匹配等问题。不过好在CAL算法支持.stl文件，可以广泛支持此前3D打印的CAD模型。


与此前的Carbon3D相比，这套系统是一次性整体成型。而且在论文中作者表示，这套技术还能用于更大尺寸的3D打印。

如果你对细节感兴趣，可以细读一下发表在Science上的论文：

**Volumetric additive manufacturing via tomographic reconstruction**



![](https://pic1.zhimg.com/v2-0d251a017119ab35518d0858790927ec_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='336'></svg>)



## **作者团队**

这份研究的作者们，来自**UC伯克利**和美国**劳伦斯利佛摩国家实验室**。

三位共同一作，都从事材料、机械方面的研究。

**Brett E. Kelly**，UC伯克利机械工程系博士，他的导师正是这篇论文的通讯作者Hayden K. Taylor。同时，Kelly还是劳伦斯利佛摩国家实验室的增材制造研究员。

**Indrasen Bhattacharya**，UC伯克利应用科学技术博士，主要研究新型半导体光电器件、纳米激光器、硅光子集成、纳米光子学、计算光学成像、计算机断层扫描和成像等技术。

**Hossein Heidari**，也是UC伯克利机械工程系博士，Brett E. Kelly的同门师弟，研究纳米制造相关方向。

## **One More Thing**

马上要过年了，祝量子位的读者们，春节快乐，阖家幸福！

—**完**—

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态

戳右上角「+关注」获取最新资讯↗↗

如果喜欢，请分享or点赞吧~比心❤


