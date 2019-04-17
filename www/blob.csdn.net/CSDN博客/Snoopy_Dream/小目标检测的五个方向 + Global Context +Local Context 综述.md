# 小目标检测的五个方向 + Global Context +Local Context 综述 - Snoopy_Dream - CSDN博客





2019年03月07日 22:18:48[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：157








首先推荐大家两篇目标检测综述论文,都是2018年11月的。

Recent Advances in Object Detection in the Age of Deep Convolutional Neural Networks

Deep Learning for Generic Object Detection: A Survey

[CVPR2019目标检测方法进展综述](https://mp.weixin.qq.com/s?__biz=MzUxNjcxMjQxNg==&mid=2247487882&idx=2&sn=86c21770d45bb7fe6e25132ddbe84297&chksm=f9a26305ced5ea139c2bd0931f60e2584016db40c688552bb673109015a34b4b2671c513eca8&mpshare=1&scene=23&srcid=0318ZoW7XNKQMDmCX4IGWOm9#rd)【另附一个2019的】

解决小目标问题的方法有很多，例如**:i)图像的缩放;ii)浅网络;iii)上下文信息;iv)超分辨率。补充还有针对小目标的图像增强，这个是最新的一篇论文中有提到。**

下面将讨论前四个方向

第一个——也是最琐碎的一个方向——是在检测前对图像进行缩放。但是，由于大图像变得太大，无法装入GPU进行训练，因此单纯的升级并不有效。ao等[2017]首先下采样图像，然后利用强化学习训练基于注意力的模型，动态搜索图像中感兴趣的区域。然后对选定的区域进行高分辨率的研究，并可用于预测较小的目标。这避免了对图像中每个像素进行同等关注分析的需要，节省了一些计算成本。一些论文[Dai等，2016b，2017年，Singh和Davis, 2018年]在目标检测上下文中训练时使用图像金字塔，而[Ren et al.， 2017]在测试时使用。

第二个方向是使用浅层网络。小物体更容易被接受场较小的探测器预测。较深的网络具有较大的接受域，容易丢失关于较粗层中较小对象的一些信息。Sommer等[2017b]提出了一种非常浅的网络，只有四个卷积层和三个完全连接的层，用于检测航空图像中的目标。当期望的实例类型很小时，这种类型的检测器非常有用。但是，如果预期的实例具有不同的大小，则效果更好



第三个方向是利用围绕小对象实例的**上下文**。Gidaris和Komodakis [2015]， Zhu等[2015b]使用上下文来提高性能，Chen等[2016a]则专门使用上下文来提高小对象的性能。他们使用上下文补丁对R-CNN进行了扩展，与区域建议网络生成的建议补丁并行。Zagoruyko等人[2016]将他们的方法与深度掩模对象建议相结合，使信息通过多条路径流动。



最后，正如Li等人[2017c]所提出的，最后一个方向是利**用生成对抗性网络选择性地提高小目标的分辨率。**

它的生成器学会了将小对象的不佳表示增强为超分辨对象，这些超分辨对象与真实的大对象非常相似，足以欺骗竞争的鉴别器。

引自：Recent Advances in Object Detection in the Age of Deep Convolutional Neural Networks

### Global Context +Local Context



![](https://img-blog.csdnimg.cn/20190307221137677.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)



![](https://img-blog.csdnimg.cn/20190307221151984.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==,size_16,color_FFFFFF,t_70)



图出自：Deep Learning for Generic Object Detection: A Survey



