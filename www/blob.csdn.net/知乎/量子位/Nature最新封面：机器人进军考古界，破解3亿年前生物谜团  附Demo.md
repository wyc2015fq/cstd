# Nature最新封面：机器人进军考古界，破解3亿年前生物谜团 | 附Demo - 知乎
# 



> 乾明 发自 凹非寺
量子位 出品 | 公众号 QbitAI

最新一期Nature发布了，封面是一个原型为3亿年前生物化石的机器人。



![](https://pic4.zhimg.com/v2-d09c548733168fe5826a833c37d2615b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='903' height='1200'></svg>)



这篇论文将古生物学、生物力学、计算机模拟、活体动物演示等学科结合在一起，对3亿年前生物的行走状态进行了深入研究，并根据研究成果做成了机器人来进一步探索各种可能性。



![](https://pic1.zhimg.com/v2-5a41ebcf1f18992dcf0e7758f5754588_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='329'></svg>)



《连线》杂志发表评论称，这对于理解陆地上的运动是如何进化的有很大的影响。同时，这种利用机器人学等跨学科的研究方法， 也会对科学家研究各种灭绝动物的生存方式产生影响。


这个生物名叫Orobates，一个大型食草四足动物，是当前发现的、能够将完整的骨骼化石与足迹化石联系起来的最古老的生物。



![](https://pic1.zhimg.com/v2-a634619ce7101e8b1a80d3d4bd103b34_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='346'></svg>)



它诞生于大概3亿年前生物从海洋爬向陆地的时期，处于生物进化的关键交界处。对于研究来年两栖动物和哺乳动物来说，具有承上启下的作用。


## **怎么研究的？**

Orobates拥有完整的骨骼化石、相应的足迹化石以及行走轨迹的化石，让这项研究成为了可能。

基于对这些化石的综合分析，来自德国柏林洪堡大学和瑞士洛桑联邦理工学院的研究团队计算出了Orobates的数百种可能的步态：从较不先进的拖着肚子到在陆地行走，到像鳄鱼那样肚子悬空奔跑的更先进的姿势。



![](https://pic1.zhimg.com/v2-c8881939ccade19f7477fce504c4c2c8_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='359'></svg>)



此外，他们也对四种当前现存的两栖动物和爬行动物的步态进行了测量，分别是凯门鳄、鬣蜥、蝾螈和石龙子。

在这些生物行走时，对它们进行X光扫描，研究团队检查了这些动物如何直立行走的，行走时骨骼如何弯曲以及肘关节和肩关节弯曲程度。



![](https://pic1.zhimg.com/v2-03f4fd59e912f9690addb306cbe6e62c_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='270'></svg>)



基于对不同生物的骨骼解剖对比与相应行走轨迹分析，他们发现，Orobates的行走方式，更加偏向于直立行走的方式，而不是将肚皮拖在地上。



![](https://pic1.zhimg.com/v2-a50185c79e8ece278b2456401c38c340_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='485' height='685'></svg>)



从而得出结论称，直立行走这种相对先进的运动方式，演化时间比此前科学家预期的更早。


此外，研究团队还开发了一个仿真机器人“OroBOT”，来研究Orobates最有可能的行走方式，以及它们的合理性和有效性。



![](https://pic1.zhimg.com/v2-e48bf59a2930cf55a39bf67e642027e0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='567'></svg>)



## **有个Demo可以玩**

关于这项研究的数字模拟系统，已经做成了可以交互的Demo，任何人都可以去他们网站上去体验，设置不同的参数，观察这个大约3亿年前的生物行走步伐。

结果的呈现形式有两种。

一种是3D图像，代表着可能的步态。其中，点的颜色越蓝，得分就越高，越红得分就越底，也就代表着行走的步态更加笨拙。



![](https://pic2.zhimg.com/v2-d2593046531d59d581f31695aedf653d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='608'></svg>)



图中的大色块，代表着四种当前现存的两栖动物和爬行动物的数据，它们分别是凯门鳄、鬣蜥、蝾螈和石龙子。

左侧3D图像与动力学（Dynamic）指标相关，右侧与运动学（Kinematic）指标相关。

另一种是动图，直观呈现了不同的参数下Orobates的步态会如何变化。动图一共有3个，第一个是数字模拟的Orobates，第二个机器人OroBOT，第三个是基于化石重建的Orobates。



![](https://pic3.zhimg.com/v2-144f7ed73bb75ce868860cce2273a56e_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='130'></svg>)



动力学指标中，可以调整能量消耗程度、平衡度、精确度和地面反力，都会影响Orobates的步态得分。比如，能量消耗程度越大，步态得分就越低。




![](https://pic1.zhimg.com/v2-7b9bdb2ad85393129ad02fb74d7368e0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='609' height='1084'></svg>)



运动学指标用于调整前肢和后肢的骨骼碰撞程度，主要由骨骼间的软骨数量来衡量的，这对于Orobates来说是未知的。通过调整指标可以看出，如果骨骼碰撞频繁，步态就会更加敏捷。



![](https://pic4.zhimg.com/v2-3717c6736cd18df7fcf0c76b0180b0c3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='643' height='437'></svg>)



经过量子位的初步体验，这些指标的改变，只会影响左侧的3D图像。

能够影响其他3D图像和下方动图展示的指标位于下方。这些指标分别控制的是Orobates行走频率、脊柱弯曲程度、身体重量等等，都会直接影响Orobates的爬行方式。



![](https://pic2.zhimg.com/v2-3aa0394bc36926573ffa875ba11053ad_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='584' height='1228'></svg>)



此外，Demo的下方，还有四种当前现存的两栖动物和爬行动物的视频。


如果你整个Demo有兴趣，请收下传送门~

## **传送门**

Demo地址：
[https://biorob2.epfl.ch/pages/Orobates_interactive/​biorob2.epfl.ch](https://link.zhihu.com/?target=https%3A//biorob2.epfl.ch/pages/Orobates_interactive/)
论文地址（有付费墙）：
[https://www.nature.com/articles/s41586-018-0851-2​www.nature.com](https://link.zhihu.com/?target=https%3A//www.nature.com/articles/s41586-018-0851-2)
— **完** —
量子位 · QbitAI
վ'ᴗ' ի 追踪AI技术和产品新动态

[量子位​www.zhihu.com![图标](https://pic4.zhimg.com/v2-ca6e7ffc10a0d10edbae635cee82d007_ipico.jpg)](https://www.zhihu.com/org/liang-zi-wei-48)
欢迎大家关注我们，以及订阅[我们的知乎专栏](https://zhuanlan.zhihu.com/qbitai)


