
# 赋能AI开源生态，Tengine成AI开发利器 - 喜欢打酱油的老鸟 - CSDN博客


2019年01月05日 10:37:04[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：115


[https://www.toutiao.com/a6642115769876349448/](https://www.toutiao.com/a6642115769876349448/)
2019-01-03 17:00:00
现阶段，边缘人工智能的主要挑战有成本、功耗、AI计算能力，以及软件生态构建等问题。芯片公司被迫花大量精力做上层开发环境和平台，应用/算法公司被迫做大量底层适配优化成为全栈AI公司。如何创造性解决这些问题，加速产业协作？
**嵌入式前端深度学习框架Tengine应运而生**
Tengine是由**OPEN**AI LAB 开发的一款轻量级模块化高性能神经网络推理引擎，专门针对Arm嵌入式设备优化，并且无需依赖第三方库，可跨平台使用支持Android，Liunx，并支持用GPU、DLA作为硬件加速计算资源异构加速。
Tengine支持各类常见卷积神经网络，包括SqueezeNet，MobileNet，AlexNet，ResNet等，支持层融合、8位量化等优化策略。并且通过调用针对不同CPU微构架优化的HCL库，将Arm CPU的性能充分挖掘出来。
![赋能AI开源生态，Tengine成AI开发利器](http://p3.pstatp.com/large/pgc-image/21a71d3c0fd14801b4f8ddf9645de464)
嵌入式前端深度学习框架Tengine
作为一款开放计算平台，Tengine的扩展能力非常强，除支持自带的HCL库以外，还支持业界常见的OpenBLAS, Arm Compute Library库。客户还可以很容易拓展自己的算子库，给用户自由的选择。
除此之外，Tengine还能适配芯片上GPU等各类硬件资源甚至是DLA硬件加速器。只需要简单的在Tengine上注册设备并挂载上驱动，就能充分利用芯片上的硬件资源提升计算能力。
![赋能AI开源生态，Tengine成AI开发利器](http://p9.pstatp.com/large/pgc-image/d02aaa122e60461d89560905e632d6af)
Tengine特点-嵌入式场景设计
目前，基于Rockchip RK3399的Rock960率先支持Tengine，RK3399 Cortex-A72 1.8GHz Tengine vs Caffe性能数据上，Tengine单核即超过Caffe双核的性能，硬件利用率大幅度提升。
![赋能AI开源生态，Tengine成AI开发利器](http://p1.pstatp.com/large/pgc-image/df5a04e7ae4b4f64b80ed19896748778)
Tengine特点-计算速度快，运行内存小，模型兼容性好
相比于Caffe，NCNN等其他AI框架，Tengine是为嵌入式设备终端推理量身定制的框架，计算速度快，运行内存小，模型兼容性好。Tengine运行ResNet50所需的物理内存为NCNN的三分之一，Caffe的二分之一。运行MobileNetv1的性能是Caffe的4.9倍，NCNN的2.8倍。
Tengine作为AI 应用框架，不仅对上层应用提供推理API接口，支持主流模型与格式，还在下层定义硬件接口，适配嵌入式平台各类计算模块，此外支持同时运行多个AI应用算法以及调用多个计算模块计算。
因此，Tengine的面世对AI开源生态具有里程碑的意义，可以预见会广泛应用于日益火爆的Face ID人脸门禁、语音识别、机器人多传感融合平台、边缘计算AI服务器等领域，将极大促进Arm生态下AI全领域的技术开发与产品应用的加速发展。
![赋能AI开源生态，Tengine成AI开发利器](http://p1.pstatp.com/large/pgc-image/5e85144236ad46338e0e647160e05ee0)
Tengine特点-适配嵌入式平台各类计算模块
![赋能AI开源生态，Tengine成AI开发利器](http://p9.pstatp.com/large/pgc-image/e9b30c8ea54d4c759f4de8b434f6f5d5)
Tengine应用场景丰富
关于**OPEN**AI LAB
**OPEN**AI LAB（开放智能实验室）于2016年12月成立，由Arm中国联合产业伙伴发起，致力于推动芯片、硬件、算法软件整个产业链的深度合作，加速人工智能产业化应用部署和应用场景边界拓展，为最终实现万物智能贡献力量。

