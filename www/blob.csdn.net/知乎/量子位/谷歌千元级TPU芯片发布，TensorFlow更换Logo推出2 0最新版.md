# 谷歌千元级TPU芯片发布，TensorFlow更换Logo推出2.0最新版 - 知乎
# 



> 栗子 晓查 发自 凹非寺 
量子位 报道 | 公众号 QbitAI

今天凌晨，谷歌一年一度的TensorFlow开发者大会在加州举行。明明应该是一场软件盛会，却被生生开出了硬件发布会的味道。

谷歌在会上发布了两款AI硬件：售价仅千元搭载着TPU的开发板；可以为Linux机器学习推理加速的计算棒。它俩取代软件成了本次发布会的主角。



![](https://pic2.zhimg.com/v2-aa063e0245d624d850ae0d9f20002711_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='930' height='523'></svg>)



当然还有照例升级的TensorFlow 2.0 Alpha版，版本号提升的同时，Logo也变成时下流行的扁平化设计。




![](https://pic4.zhimg.com/v2-7161698fb169dc180b442f9fcf9294a7_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='425' height='228'></svg>)



在移动设备运行AI模型越来越主流的今天，用于部署在边缘设备上的TensorFlow Lite终于迎来了1.0版。


下面就让我们一起来回顾这场发布会的亮点吧。

## **发布会主角Coral**

Coral开发板（Coral Dev Board）是一款售价150美元（折合人民币约1000元）的小型计算机，具有可拆卸的模块化系统和一个定制的TPU芯片，类似于树莓派。



![](https://pic2.zhimg.com/v2-e2b0eac1110df5184722e5610cb9f851_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1000' height='562'></svg>)



Coral中的Edge-TPU尺寸大约只有一枚硬币的1/4，拥有1GB的LPDDR4内存和8GB的eMMC存储，安装Mendel版Linux或者Android，可以进行本地的离线运算。




![](https://pic4.zhimg.com/v2-f270be60177e4f2422ca72438a114bbf_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='611'></svg>)



它不训练机器学习模型，而只是用TensorFlow Lite进行推理预测，因此比全栈框架更节能。


Coral能够以每秒30帧的速度在高分辨率视频上运行深度前馈神经网络，或者以每秒超过100帧的速度运行MobileNet V2这样的单一模型。



![](https://pic2.zhimg.com/v2-b23db0865636ad34d0fe985c654de76d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='488'></svg>)



这位妹子现场用Coral展示了一个有趣的图像分类应用。只需要开发板加摄像头，再加几个按钮而已。

右边的四个按钮，各自代表一个类别。

给AI看一只橘子，连按十几下黄色按钮。

再看到橘子的时候，黄灯就会亮起。



![](https://pic1.zhimg.com/v2-7d63946adefd38f35e3138313fb4a6f0_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='277'></svg>)



再给AI看TF的标志，也是橙色的，连按十几下红色按钮。

然后，再看到标志的时候，红灯就会亮起。



![](https://pic4.zhimg.com/v2-e1a8e9cb163c4caafd3f978a7281b16b_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='367' height='278'></svg>)



这时候，就算再把橘子放到摄像头前，AI也不会被迷惑，毫不犹豫地点亮黄灯。

同时，谷歌还发布了一款Coral USB加速器，体内同样包含一颗Edge TPU，可以在任何64位ARM或x86平台的Debian Linux上运行。




![](https://pic2.zhimg.com/v2-b0f9711b1ae4d9ad4d9b265fa53b4e31_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1000' height='562'></svg>)



Coral USB加速器售价75美元，可以加速树莓派和Linux系统的机器学习推理。


谷歌不是第一个发布此类产品的厂商，英特尔早在几年前就发布过USB神经网络加速计算棒，但是神经计算棒只支持Caffe，而Coral支持Caffe、TensorFlow和ONNX。

由于PyTorch能转成ONNX，所以Coral实际上也能支持PyTorch。

连竞争对手Facebook的大神Yann LeCun，也在推特上给出一个好评：“Nice feature rundown at Hackaday”。



![](https://pic3.zhimg.com/v2-64998b415d3e37d333baae65657d0a2a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='593'></svg>)



此外还有一款售价25美元、500万像素的相机配件。


前两款硬件已经可以去官网订购，和查看详细技术文档：
[https://coral.withgoogle.com/](https://link.zhihu.com/?target=https%3A//coral.withgoogle.com/)

## **TF 2.0 Alpha**



![](https://pic4.zhimg.com/v2-01a2888cf5334f5ebc17e09b1b63816f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='589'></svg>)



TensorFlow团队表达了对Keras更深的爱。


用tf.keras高层API，可以有效简化TF的使用。

团队说，从前tf.keras的设定用来做小模型的，要扩大规模怎么办？

估计器 (Estimators) 就是强大的工具。

在2.0里，tf.keras已经整合了Estimators的能量：



![](https://pic1.zhimg.com/v2-b08cac5d2cd9ef49892261722fb659a0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='824' height='520'></svg>)



如此，便不用在一个简单API和一个可扩展API之间，再做多余的选择。


死磕Keras，扔掉其他API，就是为了减少重复。

除此之外，就是Eager Execution变成了默认设定。有了它，除了执行起来更加快捷，debug也容易一些：

“可以使用 Python 调试程序检查变量、层及梯度等对象。”

## **快速上手**

如果想要丝滑入门TF 2.0的Alpha版，可前往TensorFlow新鲜设计的友好网站，那里有教程和指南：

[https://www.tensorflow.org/alpha](https://link.zhihu.com/?target=https%3A//www.tensorflow.org/alpha)



![](https://pic2.zhimg.com/v2-f005da24b68511897fa6222313e013ad_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='573'></svg>)



团队温馨建议，先观察一下“Hello World”示例，分为**初学者版**和**老司机版**：

> 初学者版，用的是Keras Sequential API，最简单的入门大法；
老司机版，展示了怎样用命令式来写正向传播、怎样用GradientTape来写自定义训练loop，以及怎样用tf.function一行代码自动编译。

然后，再去读*Effective TensorFlow 2.0*等等指南。

不止这些，还有AutoGraph指南，代码升级指南，以及其他Keras相关指南，一应俱全。

## **还有新课**

随着TF 2.0 Alpha版一同发布的，还有两门深度学习课程，**零基础**可食用。

其中一门，**吴恩达**老师参与了课程开发，叫做**针对人工智能、机器学习和深度学习的TensorFlow入门**：



![](https://pic3.zhimg.com/v2-e342bed6439eea4c1d22251cb565ed8a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='740'></svg>)



这是一套实践课程，会教你在TensorFlow里面搭建神经网络、训练自己的计算机视觉网络，再利用卷积来改善网络。


课程分四周。

> 第一周，了解一种编程新范式。
第二周，入门计算机视觉。
第三周，用CNN增强计算机视觉。
第四周，给网络喂食真实世界的图像。

另一门，是优达学城的免费课，叫做**针对深度学习的TensorFlow入门**。



![](https://pic3.zhimg.com/v2-9abe0c55d04f11ffbae409789f5fb2ca_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1072' height='800'></svg>)



第一节课还在了解课程大纲，第三节课就已经要训练自己的模型了：




![](https://pic4.zhimg.com/v2-5548fb807ed1233abedb76499959c1ef_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='529'></svg>)



现在，前四节课已经上线。第五节待续中。


## **给手机用的TF Lite**

介绍完TF 2.0后，谷歌TensorFlow Lite的工程师Raziel Alvarez上台，TF Lite 1.0版正式上线。

TensorFlow Lite是一个为移动和嵌入式设备提供的跨平台解决方案。谷歌希望让TensorFlow能够运行在更多设备上。

除了PC和服务器以外，我们的生活中还有像手机、智能音箱、智能手表等设备需要用到机器学习模型，而它们都是无法运行TensorFlow的。

要让这些设备运行TensorFlow需要面临以下挑战：计算力不足、存储空间有限、电池限制。



![](https://pic2.zhimg.com/v2-818b1f168c384f20eb7043bdde440809_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='611'></svg>)



必须要有个轻量级（Lite）框架能将机器学习模型部署在移动和IoT设备上的。


TensorFlow Lite就是为此而诞生，它于2017年5月在谷歌I/O开发者大会上首次推出，目前它已经部署到超过20亿台设备中，主要是通过以下一些软件，包括谷歌官方应用和两家来自中国的应用——爱奇艺、网易。



![](https://pic3.zhimg.com/v2-6d51bed70c61ab23fb54674876dee90a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='657'></svg>)



接着谷歌邀请了来自中国的网易机器学习工程师Lin Huijie介绍了TensorFlow Lite在“有道”中的应用。


Lin Huijie说，网易用它实现了30~40%的图片翻译加速。



![](https://pic4.zhimg.com/v2-9627c593bd119a703cce20a648762c23_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='752'></svg>)



谷歌表示为移动设备部署TF Lite非常方便，只需用TensorFlow打包好模型，再用TF Lite转换器将之转化为TF Lite模型。


经过TF Lite的优化后，设备在CPU上的性能达到原来的1.9倍，在Edge TPU上的性能最高提升了62倍。



![](https://pic4.zhimg.com/v2-c8fe2671cd014239130bae07faeb5817_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='610'></svg>)



## **其他**

除了以上几款硬件和软件外，谷歌今天还发布了TensorFlow Federated，和TensorFlow Privacy。

TensorFlow Federated是一个开源框架，用来训练来自不同位置数据的AI模型。TensorFlow Privacy能让开发人员更容易训练具有强大隐私保障的AI模型。

另外在发布会上亮相的有：TensorFlow.js 1.0；Swift for TensorFlow 0.2。

— **完** —

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态

戳右上角「+关注」获取最新资讯↗↗

如果喜欢，请分享or点赞吧~比心❤


