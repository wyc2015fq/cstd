# 最新手机芯片AI跑分出炉：高通、联发科力压华为 - 知乎
# 



> 晓查 发自 凹非寺
量子位 出品 | 公众号 QbitAI
![](https://pic2.zhimg.com/v2-ee514cc7fd5fa84f249ed23416d31065_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1077' height='671'></svg>)
手机AI哪家强？这不能仅仅听厂商的宣传。到底有没有一个AI的“安兔兔”来给手机跑个分，客观评价一下手机的AI性能呢？

去年，苏黎世联邦理工学院计算机视觉实验室联合各大手机芯片厂商，推出了手机AI的测试标准，广泛包含各种神经网络架构，尽量保证测试的全面性。

最近谷歌又为TensorFlow Lite带来更新，声称给手机芯片的AI表现带来了巨大提升。

那么经过一段时间的适配后，各家手机芯片AI性能排名如何？让我们一下来看看这份2月最新榜单吧：
![](https://pic4.zhimg.com/v2-93423c94ca04fb2307a9ce07badbe4ef_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='632'></svg>)
TensorFlow Lite更新成功让高通**骁龙855**超越联发科**Helio P90**，成为AI跑分最高手机芯片。华为**麒麟980**排名略有上升，但仍不及骁龙855之前的跑分，排在榜单第四位。


至于三星，他们好像还没开始重视，自研的Exynos芯片AI性能普遍较差，没有一款进入前十。

除了芯片榜单外，还有一份不同手机的AI排行榜，里面曝光了即将发布的三星S10跑分。
![](https://pic2.zhimg.com/v2-cbf6b98bb75f451f8041aa76bd6c7879_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='980' height='888'></svg>)



## **手机AI测试简介**

手机AI跑分测试包括9个计算机视觉AI任务，由9个不同的神经网络来运行。

主要包括以下几项：**目标识别、人脸识别、图像去模糊、图像超分辨率、图像语义分割、处理图像的内存极限等**。

其中目标识别分别在MobileNet和Inception两个神经网络上各运行一次，图像超分辨率分别在VGG-19和SRGAN上各运行一次。



![](https://pic4.zhimg.com/v2-fcc928808e151556cb737b4a21cb288f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='655'></svg>)
## **三星S10将成最强AI手机**

虽然三星自家的Exynos处理器AI性能不佳，但本月底即将发布的三星S10很可能要成为最强AI手机了。它的跑分接近21000，只有联想Z5 Pro GT性能与之接近。



![](https://pic3.zhimg.com/v2-f71e7878ec74b88b46def3cb428f7b3a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='608'></svg>)
另外，我们也能看出给手机“暴力”加内存的对AI性能的提升。vivo两款手机内存达到10GB后，使搭载骁龙845的它们也超越了华为Mate 20，进入手机AI性能榜单前十。

## **高通和华为分别强在哪里**

高通**骁龙855**无疑是目前最强的手机芯片之一。与华为芯片不同的是，骁龙855用GPU+DSP为AI提供硬件加速。

这种设计可以让高通可以不用NPU来加速AI计算，芯片面积更小。但是它的GPU无法完全用于运行神经网络，预计下一代手机芯片会加入NPU。



![](https://pic2.zhimg.com/v2-371b2b2d542a2bde3ea9359cbe9a54c9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='782'></svg>)



华为**麒麟980**是半年前推出的芯片，性能不及才推出不久的骁龙855也在情理之中。实际上它的浮点神经网络性能完全不输高通。

但是由于技术问题，麒麟980的NPU无法通过NNAPI加速整型运算，而是使用Arm NN驱动程序进行基于GPU的加速。在这种情况下，其性能低于竞争对手，最终导致导致跑分较低。

## **联发科为何能进前三**



![](https://pic1.zhimg.com/v2-1cf480bd36a1302b9d520884aba0f680_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='678' height='407'></svg>)



为什么联发科技的中端芯片P90能进前三，甚至AI性能能与骁龙855相媲美？

这是因为联发科采用了一款独立的AI芯片，该芯片基于其内部GPU设计，针对深度学习任务进行了大幅修改。虽然它AI性能很强，但是它的CPU性能比高通和华为旗舰低30%。

最后附上完整榜单地址：

芯片排行榜：

[AI-Benchmark​ai-benchmark.com](https://link.zhihu.com/?target=http%3A//ai-benchmark.com/ranking_processors.html)
手机排行榜：

[AI-Benchmark​ai-benchmark.com](https://link.zhihu.com/?target=http%3A//ai-benchmark.com/ranking_all.html)
— **完** —

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态

戳右上角「+关注」获取最新资讯↗↗

如果喜欢，请分享or点赞吧~比心❤


