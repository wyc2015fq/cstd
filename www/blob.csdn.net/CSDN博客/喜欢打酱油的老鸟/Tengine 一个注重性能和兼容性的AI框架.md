
# Tengine 一个注重性能和兼容性的AI框架 - 喜欢打酱油的老鸟 - CSDN博客


2019年01月11日 00:00:46[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：222


[https://www.toutiao.com/a6643578312285225479/](https://www.toutiao.com/a6643578312285225479/)
2019-01-07 17:01:00
前段时间，Tensorflow官方提供的本地编译的方式在Arm嵌入式设备运行Tensorflow Lite，我在11月中旬，使用本地编译的方式编译二进制的Label_image, 对RK3288 以及树莓派上做了性能的测试。
![Tengine 一个注重性能和兼容性的AI框架](http://p3.pstatp.com/large/pgc-image/8748c38223b146cf8adef1bc13be87c0)
A17与A53 MobilenNet V1(fp) 性能对比
从性能测试的结果来看，物美价平的树莓派其实性能并不弱。但是今天我想介绍一个我新发现并让我眼前一亮的框架**Tengine**.
Tengine 显著的优点是**性能和兼容性**，使用Tengine框架后运行性能可以得到大幅度的提升。
下图为MobileNet V1_1.0_224 浮点型模型单张图片分类在单核A53的性能对比。我们可以看到Tengine 有显著的优势。
![Tengine 一个注重性能和兼容性的AI框架](http://p1.pstatp.com/large/pgc-image/d37260580efe4b84a9364577e04b3914)
MobileNet V1_1.0_224 浮点型模型单张图片分类在单核A53的性能对比
Tengine 框架有着非常好的模型兼容性，支持直接加载caffe/mxnet/tensorflow模型文件，而不需要事先转换，而且用户仅需编译就可以利用Tengine的加速Caffe和Tensorflow性能。
Tengine 不仅可以让分类网络的性能大大提升，连大家众所周知的MobileNet-SSD 网络也能提升它的性能。很多人都觉得树莓派的性能不足以hold住检测网络，但是如果用Tengine框架就可以做到。
即使是开源版是每一帧的检测**耗时仅为 286.136ms**，足以看出Tengine的性能非常强大。
![Tengine 一个注重性能和兼容性的AI框架](http://p1.pstatp.com/large/pgc-image/aa57afbc0bc64f50a206244e2f614cce)
开源版上每一帧的检测耗时仅为 286.136ms
![Tengine 一个注重性能和兼容性的AI框架](http://p9.pstatp.com/large/pgc-image/3f66a54703564296a8ae0ec2940868e5)
GPU用半浮点精度float16的检测结果准确无误
有兴趣的朋友可以看一下Tengine GitHub的主页，最近还推出了跑MobileNet分类网络的Android App， 手机端就可以使用AI 程序。
https://github.com/OAID/Tengine/?tt010701
https://github.com/OAID/Tengine-app/?tt010701
关于**OPEN**AI LAB
**OPEN**AI LAB（开放智能实验室）于2016年12月成立，由Arm中国联合产业伙伴发起，致力于推动芯片、硬件、算法软件整个产业链的深度合作，加速人工智能产业化应用部署和应用场景边界拓展，为最终实现万物智能贡献力量。

