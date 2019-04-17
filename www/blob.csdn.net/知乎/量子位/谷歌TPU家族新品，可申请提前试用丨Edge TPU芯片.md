# 谷歌TPU家族新品，可申请提前试用丨Edge TPU芯片 - 知乎
# 



> 郭一璞 发自 凹非寺 
量子位 报道 | 公众号 QbitAI

昨晚，谷歌博客推送了他们的物联网软硬件新设备——**Edge TPU**硬件芯片，还有**Cloud IoT Edge**，将谷歌云AI功能扩展到网关和联网设备的软件堆栈。

**Edge**，也就是边缘设备，包含了连接到网络终端的各种各样的电子设备，比如网关、摄像头都属于这一类。通过新推出的两种产品，用户可以在云上构建和训练机器学习模型，通过Edge TPU硬件加速器的功能在Cloud IoT Edge所连接的设备上运行这些模型。
![](https://pic4.zhimg.com/v2-f792320b59a8a1528bfa6b6c215556f3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1000' height='620'></svg>)
## **Edge TPU**

Edge TPU是谷歌专为在边缘设备上运行TensorFlow Lite ML模型而设计的ASIC芯片，谷歌在博客中说，设计Edge TPU时专注优化了「每瓦性能」和「每美元性能」。

Edge TPU是云TPU的补充。在云端加速了机器学习训练之后，你还可以用Edge TPU在边缘设备上进行快速的机器学习推理。**让设备传感器不仅能采集数据，还能在本地实时做出智能的决策**。
![](https://pic3.zhimg.com/v2-86072c612fba9cfa5fc0e756c756ae46_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1000' height='640'></svg>)△ Edge TPU身材娇小
## **Cloud IoT Edge**

Cloud IoT Edge是个软件系统，可以将谷歌云的数据处理和机器学习功能扩展到网关、摄像头和终端设备上，有了它，用户就可以在Edge TPU或者基于GPU/CPU的加速器上，运行在谷歌云上训练好了的机器学习模型。

Cloud IoT Edge可以在Android或Linux设备上运行，关键组件包括：

· 一个运行时（runtime）：它用于至少有一个CPU的网关类设备，可以在边缘设备本地存储、转换、处理数据，并从中获得智能，同时，还能与云IoT平台的其余部分无缝互操作。

· Edge IoT Core：可以更安全地将设备连接到云，支持软件和固件更新，并通过Cloud IoT Core管理数据交换。

· 基于TensorFlow Lite的Edge ML运行时：它用预训练模型来实现本地机器学习推理，显着减少延迟，并增加边缘设备的多功能性。由于Edge ML运行时与TensorFlow Lite有接口连接，因此它可以在网关类设备、甚至终端设备（如摄像头）中的CPU，GPU、Edge TPU上，运行机器学习模型。

## **为企业每年节省数百万美元**

这套新产品主要面向企业端，借助实时预测功能提高安全性和可靠性。

比如，对工业生产来说，这套云端IoT系统可以帮助检测装配线上的异常情况，为可能出现的问题提前预警；

对零售业来说，这套系统可以针对线下门店的顾客行为进行针对性的推荐，提供优惠促销；

对智能汽车来说，可以提供防撞、交通路由和远离道路检测系统。

总之，对线下的产业是有一些帮助的。LG CNS首席技术官Shingyoon Hyun这样评价它：

「这套智能视觉检测解决方案能提升LG制造工厂安全和效率，借助谷歌云AI、谷歌云IoT Edge和Edge TPU，结合我们传统的MES系统和多年的经验，我们相信智能工厂将变得越来越智能和连接。我们一直渴望通过智能视觉检测系统创造一个更好的工作场所，提高产品质量，**每年节省数百万美元**。而谷歌云AI和物联网技术，加上我们LG CNS的专业知识，让这一切成为可能。」

## **套件大全&传送门**

## **开发套件**

为了让Edge TPU能快速开始开发和测试，谷歌提供了一个**开发套件**。
![](https://pic4.zhimg.com/v2-d7ef0e554864091e496c981f2a0353ff_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1000' height='810'></svg>)
该套件包括一个模块化系统（SOM），它结合了谷歌的Edge TPU，恩智浦CPU，Wi-Fi和Microchip的安全元件，将在今年10月提供给开发者。

不过，如果你想当上提前吃螃蟹的人，可以进入以下链接申请：
[http://t.cn/ReLmOSK](https://link.zhihu.com/?target=http%3A//t.cn/ReLmOSK)
（-_-||谷歌你懂得，需要自行越过山丘）

## **AIY Edge TPU开发板**

另外，谷歌还推出了两款AIY设备，其中一个就是下图这个**AIY Edge TPU**开发板：
![](https://pic3.zhimg.com/v2-728acde4aa15eaed891936f8ce0c24d6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='688'></svg>)
这块板子提供了对器件进行原型设计需要的所有外设连接——包括一个有40根引脚GPIO接头，可与各种电气元件集成。它还有可移动的模块化系统（SOM）子板，一旦准备好扩展，就可以直接集成到硬件中。

上参数：
![](https://pic1.zhimg.com/v2-4cb4d6603e6e41090609a10a6eb20f10_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1008' height='1924'></svg>)
## **AIY Edge TPU加速器**

另一个AIY设备则美观得多，**AIY Edge TPU加速器**：
![](https://pic1.zhimg.com/v2-b14eebb2dd23adbd4630f34e76d1e210_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='688'></svg>)
这是个带了一个type-C接口的USB设备，给系统添加Edge TPU协处理器，可以连到各种Linux系统上，加速机器学习运行，也支持安卓系统，在TensorFlow Lite框架下运行。

参数请查收：
![](https://pic4.zhimg.com/v2-d0a4dea85ecc8f3e59f8fc4d9152f313_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='928' height='730'></svg>)
这两款AIY设备将在今年秋天上市，很遗憾，不像上面那块板子那样可以提前申请。

了解更多，可以查看谷歌博客的介绍：

[https://www.blog.google/products/google-cloud/bringing-intelligence-to-the-edge-with-cloud-iot/](https://link.zhihu.com/?target=https%3A//www.blog.google/products/google-cloud/bringing-intelligence-to-the-edge-with-cloud-iot/)

还有Edge TPU的官方介绍：
[https://cloud.google.com/edge-tpu/](https://link.zhihu.com/?target=https%3A//cloud.google.com/edge-tpu/)

尝鲜愉快~

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


