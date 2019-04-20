# “芯痛”之下阿里苦心研发NPU AI芯片究竟哪款PU更厉害？ - 人工智能学家 - CSDN博客
2018年04月21日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：516
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWQxQ4uKLYxxuUmxdfYypkCZ1IkdjkALvNEJJsQuA5dNozrTFw5N8etjibG6skAXdM3OHVEiamOrBjQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
来源：OFweek人工智能网
4月19日，有消息称，阿里巴巴达摩院正在研发一款神经网络芯片——Ali-NPU，主要运用于图像视频分析、机器学习等AI推理计算。按照设计，这款芯片性能将是目前市面上主流CPU、GPU架构AI芯片的10倍，而制造成本和功耗仅为一半，其性价比超过40倍。
事实上，随着人工智能产业的发展，CPU、GPU、TPU、DPU、NPU、BPU……各种PU也开始爆发式出现。那么，究竟这些PU在性能和使用上有何异同，又有哪些优劣呢？
**CPU：计算力占据部分很小 擅长逻辑控制**
CPU是最为普遍，最为常见的中央处理器。主要包括运算器（ALU）和控制单元（CU），除此之外还包括若干寄存器、高速缓存器和它们之间通讯的数据、控制及状态的总线。依循冯诺依曼架构，CPU需要大量空间放置存储单元和控制逻辑，计算能力只占据很小的部分，更擅长逻辑控制。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWQxQ4uKLYxxuUmxdfYypkCvD13xy6Zl4DScaS7tE90tTqXfQN7ygRGBAkXjjd7WuI3JtDcfKJ6pA/640?wx_fmt=png)
CPU结构简化图
**GPU：计算单元数量众多 但无法单独使用**
GPU的诞生可以解决CPU在计算能力上的天然缺陷。采用数量众多的计算单元和超长的流水线，善于处理图像领域的运算加速。但GPU的缺陷也很明显，即无法单独工作，必须由CPU进行控制调用才能工作。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWQxQ4uKLYxxuUmxdfYypkCqh3Qlo1ibacLPTbFkEEQBaX5bpeKCcOC40aWiahAGAysPykZeZ4eIWHA/640?wx_fmt=png)
CPU、GPU微架构对比图
**TPU：高性能低功耗 然则开发周期长、转换成本高**
谷歌专门为 TensorFlow 深度学习框架定制的TPU，是一款专用于机器学习的芯片。TPU可以提供高吞吐量的低精度计算，用于模型的前向运算而不是模型训练，且能效更高。但它的缺陷主要是开发周期长、可配置性能有限，缺乏灵活性且转换成本高。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWQxQ4uKLYxxuUmxdfYypkCAOr5cicoOBWbVAgicyXFsLrXrwEVlmXqibRE1HQuJdSXVcc3enG5VM8icQ/640?wx_fmt=png)
**DPU：可实现快速开发与产品迭代**
国际上，Wave Computing最早提出DPU。在国内，DPU最早是由深鉴科技提出，是基于Xilinx可重构特性的FPGA芯片，设计专用深度学习处理单元，且可以抽象出定制化的指令集和编译器，从而实现快速的开发与产品迭代。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWQxQ4uKLYxxuUmxdfYypkCmTpZUgKPglAP7DYu0SPOumxfHGPDmXUu2zk2gSx01CEItfDGiagaLicA/640?wx_fmt=png)
深鉴“雨燕”DPU平台
**NPU：运行效率提升 不支持大样本训练**
NPU是神经网络处理器，在电路层模拟人类神经元和突触，并且用深度学习指令集直接处理大规模的神经元和突触，一条指令完成一组神经元的处理。相比于CPU和GPU的冯诺伊曼结构，NPU通过突触权重实现存储和计算一体化，从而提高运行效率。但NPU也有自身的缺陷，比如不支持对大量样本的训练。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWQxQ4uKLYxxuUmxdfYypkC7iaroWxRBuTwQjQ6uZpuRp1brtRoleV8WuIkWuhXs8JPmDZltwoFGzw/640?wx_fmt=png)
**BPU：比在CPU上用软件实现更为高效 不可再编程**
BPU是由地平线主导的嵌入式处理器架构。第一代是高斯架构，第二代是伯努利架构，第三代是贝叶斯架构。BPU主要是用来支撑深度神经网络,比在CPU上用软件实现更为高效。然而，BPU一旦生产，不可再编程，且必须在CPU控制下使用。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWQxQ4uKLYxxuUmxdfYypkChicjIFPwAz7jJucryV8sVicZEaIEa5sPUIzXrx0XJnkT4ibVyrpFfVHaQ/640?wx_fmt=png)
从CPU、GPU的市场来看，已经基本被英特尔、英伟达和AMD三分天下。而在ASIC框架下的TPU，只有谷歌的体量和实力才有开发专用加速的动力。
推出DPU的深鉴科技有清华和斯坦福双重学术背景，公司目前的两条发展路线是：以芯片技术为主的纯技术路线，以及基于技术的产品路线。其处理器做深度学习应用端，不做训练端。目前，其深度压缩技术可以将神经网络压缩数十倍而不影响精度，还可以使用芯片存储深度学习算法模型，减少内存读取次数，降低运行功耗。
去年底，地平线在创办两年后终于发布首款芯片——“征程”与“旭日”。目前，这两款处理器都属于嵌入式人工智能视觉芯片，分别面向智能驾驶和智能摄像头。2018年CES上，英特尔和地平线还发布了基于伯努利架构的新一代征程处理器，其发展路径图为：2018年，感知；2019年，建模；2020年，决策。
而因为与英特尔的合作，地平线不禁让市场联想到英特尔早前重金收购的Mobileye。在嵌入式人工智能领域，Mobileye是业界领头羊。地平线在英特尔的定位版图是否是中国版Mobileye？但其创始人余凯的抱负是，地平线是要做中国的英特尔。
相较而言，阿里在三家中最为热衷芯片布局，上述包括寒武纪、深鉴科技均有阿里参投。
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
