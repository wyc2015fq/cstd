# 老黄投下新核弹：英伟达十年力作图灵架构，实时光线追踪GPU - 知乎
# 



> 夏乙 发自 凹非寺
量子位 出品 | 公众号 QbitAI




“买得越多，省得越多。”

今天早上，英伟达CEO黄仁勋，在温哥华的SIGGRAPH会议上，又喊出了他的GPU发布宣言，扔下了一大堆全新核弹。

他发布了“英伟达十多年来在计算机图形领域最重要的创新”：图灵(Turing)架构。以及一系列基于图灵架构的GPU，包括Quadro RTX 8000、Quadro RTX 6000、Quadro RTX 5000。

另外，老黄还发布了CUDA 10、Quadro RTX Server。
![](https://pic1.zhimg.com/v2-13ec2b473e6006df617727a0eeb361e4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1000' height='460'></svg>)
**先看最直观的：新GPU。**

老黄说，这是全球首批支持即时光线追踪的GPU。

Quadro是英伟达的专业级显卡，主要面向设计、特效、科学可视化等视觉计算任务。新发布的这一系列GPU还支持AI、高级着色和仿真，

刚刚发布的Quadro RTX系列，除了支持实时光线追踪外，还支持AI、高级着色、仿真等任务。

详细来说：
- 新RT核心带来了对对象和环境的实时光线追踪，能创造物理上精确的阴影、反射、折射和全局照明。
- 图灵张量核心能加速深度神经网络的训练和推断。
- 图灵流多处理器（Streaming Multiprocessor）架构，具有4608个CUDA核心，每秒可以并行高达16万亿次浮点运算和16万亿次整数运算。
- 支持高级可编程着色技术，能提升复杂视觉效果。
- 首次使用三星16GB GDDR6内存，支持更复杂的设计、大规模建筑数据集、8K电影内容等。
- 通过英伟达NVLink将两个GPU高速连接起来，可以将内存容量扩展到96GB，数据传输速度达到每秒100GB。
- 支持USB Type-C和VirtualLink连接。
- 支持提升VR性能的新技术，包括可变速率着色、多试图渲染、VRWorks音频等。
![](https://pic4.zhimg.com/v2-df825abc86002f033ed7bedbbada13f3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='579'></svg>)
另外，英伟达今天还发布来了Quadro RTX服务器，将Quadro RTX GPU和Quadro Infinity软件结合起来。

这三款GPU的参数略有不同：
![](https://pic2.zhimg.com/v2-63f2fabeed46c524f813844a7b2670d5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='240'></svg>)
在这里，还出现了一个略为陌生的单位：GigaRays/sec，用来描述光线追踪性能。按字面理解，它的意思是每秒千兆光线。

这三款新GPU预计今年第四季度发售。
![](https://pic1.zhimg.com/v2-a39b6d894928d93e41485f6f0b2a9b1c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='573'></svg>)
**再看前边提到的新架构：图灵。**

图灵架构，被老黄称为“自2006年CUDA GPU发布以来的最大进步”，“英伟达十多年来在计算机图形领域最重要的创新”。图灵架构包含能为实时光线追踪提供硬件加速的RT核心，和为AI运算加速的全新张量核心。

RT核心（RT Core）是专门用于实时光线追踪的处理器，能对光线和声音在3D环境中的传播，进行加速计算，它的实时光线追踪能力提升到了Pascal架构的25倍。

张量核心（Tensor Core）是为深度学习训练和推断加速的处理器，支持每秒500万亿次张量运算。

两个核心结合起来，让图灵架构GPU模拟物理世界的能力提升到了Pascal架构的6倍。

图灵是英伟达的第18代GPU架构，聚积了10000工程年（相当于10000名工程师1年的工作）的努力。

上面提到的Quadro RTX系列三款产品，就是首批基于图灵架构的产品。
![](https://pic1.zhimg.com/v2-c0e5d9ac38c2a3518f981b8be7ab9fc4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1024' height='512'></svg>)
**再说说CUDA 10。**

CUDA大家应该都比较熟悉了，只要构建英伟达GPU加速的软件，就需要用到它。

与之前相比，CUDA 10具有以下新特性：
- 支持图灵架构。
- 新增性能优化库，在16-GPU系统中实现强大的FFT性能扩展。
- nvJPEG：用于混合JPEG处理的新库，单个和批量图像解码速度是原来2倍。
- CUDA / Graphics Interop：CUDA和图形API之间的新互操作性。
- 新的开发人员工具：用于跟踪，分析和调试CUDA应用程序的新Nsight产品系列工具。

CUDA 10与今天发布的其他新品同样是期货，不过英伟达说，加入开发者计划可以在能下载时第一时间通知你：

[NVIDIA Developer Program](https://link.zhihu.com/?target=https%3A//developer.nvidia.com/join)




— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


