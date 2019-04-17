# 英伟达CUDA 10终于开放下载了，“是最强大的GPU加速应用开发平台” - 知乎
# 



> 栗子 发自 凹非寺
量子位 出品 | 公众号 QbitAI
![](https://pic3.zhimg.com/v2-25e8b146a7fbe4890d65ae6675293b0a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='326'></svg>)
英伟达的**CUDA 10**工具包，终于可以下载了。

与9字头的前辈相比，10.0是第一次为**图灵GPU**定制的版本。

英伟达新近发布的GeForce RTX 20系列和Quadro RTX系列显卡，以及新一代GPU Tesla T4，都是基于图灵架构。

官网的评价是，要编写**GPU加速**的应用程序，CUDA 10是**最强大的开发平台**。

## **10.0是重大更新**

CUDA工具包，都是为开发**GPU加速**的应用程序而存在。

而针对**图灵GPU**定制的10.0，主要发生了以下变化：




**图灵架构**

10.0支持图灵架构，包括最新的**Tesla T4 GPU** (适用于超大规模的数据中心等等) 。




**CUDA平台**
![](https://pic2.zhimg.com/v2-b2e0476d13187d7e1b851b2bb7984e3d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='135' height='212'></svg>)
CUDA Graphs，是新的异步任务图像 (Task-Graph) 编程模型，让内核启动和执行更高效。

另外，CUDA和图形API之间的互操作性 (Interoperability) 增强了。




**CUDA库**
![](https://pic4.zhimg.com/v2-3518b1161b1b24de9528cc731f1e068f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='381'></svg>)
许多库的性能得到了优化。比如，做快速傅里叶变换(FFT)、线性代数以及矩阵乘法用的那些库。


**开发者工具**
![](https://pic1.zhimg.com/v2-6f946e237a95fffffdffd6d44abd41f0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='536'></svg>)
增加了Nsight系列产品，用来追踪、分析、Debug。




**操作系统**

除此之外，CUDA 10支持的**操作系统**和**编译器**，也比以前更多了。

操作系统：
![](https://pic2.zhimg.com/v2-43e2bf97d778e84b67bb3c45ef6988e9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='586' height='322'></svg>)
编译器：
![](https://pic2.zhimg.com/v2-dfac44d46f1f367503cb307d549958b1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='574' height='408'></svg>)
## **CUDA库性能大幅提升**

**cuFFT 10.0**

做快速傅里叶变换 (FFT) 的库，**16 GPU**时最高可以达到**17 teraFlops** (3D FFT, size 1024) 。
![](https://pic4.zhimg.com/v2-f29f5d50f299d76482d590e1c9a75f07_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='569' height='449'></svg>)
**cuBLAS 10.0**

做矩阵运算的库，包含了图灵优化的混合精度 (Mixed-Precision) 。做矩阵乘法 (GEMM) 时，最高可以达到**90 teraFlops**。
![](https://pic4.zhimg.com/v2-bb9b25e43e12b3deb3d9cc79c05910fb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='668' height='403'></svg>)
**cuSOLVER 10.0**

线性代数用的库，在对称的特征值求解 (eigensolver) 中，速度最多提升**4倍**。
![](https://pic3.zhimg.com/v2-f736c49787c2be22ebc7aa0fb87fdb92_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='516' height='403'></svg>)
## **下载，下载**

9月27日，在SIGGRAPH大会上正式发布的一个多月之后，CUDA 10的下载入口姗姗来迟。

英伟达发了一篇超长博客，事无巨细地描述了这次进化。

GPU应用开发者的各位，请速前去下载。




下载传送门：
[https://developer.nvidia.com/cuda-downloads](https://link.zhihu.com/?target=https%3A//developer.nvidia.com/cuda-downloads)

博客传送门：
[https://devblogs.nvidia.com/cuda-10-features-revealed/](https://link.zhihu.com/?target=https%3A//devblogs.nvidia.com/cuda-10-features-revealed/)




— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态




