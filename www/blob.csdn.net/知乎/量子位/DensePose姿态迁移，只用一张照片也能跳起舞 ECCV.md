# DensePose姿态迁移，只用一张照片也能跳起舞|ECCV - 知乎
# 



> 栗子 发自 凹非寺 
量子位 出品 | 公众号 QbitAI
![](https://pic1.zhimg.com/v2-69077552e4f8f2cbf37b3e71b3198b5c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='192' height='382'></svg>)
怎样让一个**面朝镜头**、**静止不动**的妹子，跳起你为她选的舞蹈，把360度身姿全面呈现？
![](https://pic1.zhimg.com/v2-041fc4510862c5803932069cb82a694c_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='267' height='306'></svg>)
Facebook团队，把**负责感知**的多人姿势识别模型**DensePose**，与**负责生成**的**深度生成网络**结合起来。

不管是谁的感人姿势，都能附体到妹子身上，把她单一的静态，变成丰富的动态。
![](https://pic3.zhimg.com/v2-f22ab028c2f4d45119ce429b365dc76a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1030' height='520'></svg>)
这项研究成果，入选了**ECCV 2018**。

## **当然不能只有DensePose**

团队把**SMPL**多人姿态模型，跟**DensePose**结合到一起。这样一来，就可以用一个成熟的**表面模型**来理解一张图片。

这项研究，是用基于表面的**神经合成**，是在闭环里渲染一张图像，生成各种新姿势。
![](https://pic4.zhimg.com/v2-69fec1ca8c4896210fae332ddf5ef5c7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='680' height='438'></svg>)△左为源图像，中为源图姿势，右为目标姿势
照片中人需要学习的舞姿，来自**另一个人**的照片，或者视频截图。

DensePose系统，负责把两张照片关联起来。具体方法是，在一个公共表面UV坐标系 (common surface coordinates) 里，给两者之间做个**映射**。

但如果**单纯基于几何**来生成，又会因为DensePose采集数据不够准确，还有图像里的自我遮挡 (比如身体被手臂挡住) ，而显得不那么真实。
![](https://pic4.zhimg.com/v2-1f175746f7cf7c6d48b1e7ee230e61e3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='894' height='380'></svg>)△DensePose提取的质地 (左) vs 修复后的质地 (右)
那么，团队**处理遮挡**的方法是，在表面坐标系里，引入一个**图像修复** (Impainting) 网络。把这个网络的预测结果，和一个更传统的**前馈条件和成模型**预测结合起来。

这些预测是各自独立进行的，然后再用一个**细化模块**来优化预测结果。把**重构损失**、**对抗损失**和**感知损失**结合起来，**优势互补**，得出最终的生成效果。
![](https://pic3.zhimg.com/v2-5bd32838c18a60d25d9328cfab0e1c0e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='463'></svg>)
完整的网络结构，就如上图这般。

## **监督学习一下**

模型的监督学习过程，是这样的：
![](https://pic2.zhimg.com/v2-6c83c2b01fc34888b8ebb7ddf3a091f9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='428'></svg>)
从输入的源图像开始，先把它的每个像素对应到UV坐标系里。这一步是DensePose驱动的迁移网络完成的。

然后，负责**修复图像**的自编码器，就来**预测**照片中人的**不同角度**会是什么样子。这步预测，也是在扭曲的坐标系里完成的。

从右边开始，就是**生成目标**，同样要整合到UV坐标系中。再用**损失函数** 来处理 (上图红字部分) 把结果输入自编码器，帮助模型学习。

用同一人物 (同样装扮) 的**多个静态姿势**来作监督，替代了**360度**旋转的人体。

## **训练成果如何**

先来看一下，新加入的**图像修复步骤**，生成的效果：
![](https://pic1.zhimg.com/v2-a3e4ee3b92f17c69ca5f743f7b9acef0_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='179' height='323'></svg>)
把DensePose的质地纹路，修复一下，还是有明显效果的。

再来看一下多人视频什么样子：
![](https://pic4.zhimg.com/v2-71f74e4ff9965cf9d0aa6323f805b48b_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='488' height='324'></svg>)
虽然，脸部好像烧焦的样子，但已经很**鬼畜**了。在下不由得想起：
![](https://pic4.zhimg.com/v2-815fa571eb6ba238ef7e7245f4f96927_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='359'></svg>)
另外团队用**DeepFashion**数据集，对比了一下自家算法和其他同行。

结果是，**结构相似度** (Structural Similarity) ，以假乱真度 (Inception Score) 以及检测分 (Detection Score) 这三项指标，Facebook家的算法表现都超过了前辈。
![](https://pic4.zhimg.com/v2-49f7d5b0b51069884a72424574720777_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='615'></svg>)
各位请持续期待，DensePose更多鬼畜的应用吧。




论文传送门：
[https://arxiv.org/pdf/1809.01995.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1809.01995.pdf)




顺便一提，教人学跳舞的算法真的不少。

比如，伯克利[舞痴变舞王](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247502972%26idx%3D2%26sn%3D366ebe3b99262e2723aa7f4e854c51a4%26scene%3D21%23wechat_redirect)，优点是逼真，缺点是**无法实现多人共舞**：
![](https://pic2.zhimg.com/v2-b585050d97334c55c8c54dbcbf8629ad_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='393' height='450'></svg>)
 vs
![](https://pic2.zhimg.com/v2-82d9781fd4a10d58ab74ef09d7b541a1_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)
— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


