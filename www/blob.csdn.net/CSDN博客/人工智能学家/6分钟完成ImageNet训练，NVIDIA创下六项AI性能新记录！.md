# 6分钟完成ImageNet训练，NVIDIA创下六项AI性能新记录！ - 人工智能学家 - CSDN博客
2019年02月25日 22:06:06[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：110
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBUAJRmSr5xFl5EttTDJ4kNIvubs2AxgCDTiatCY3iaN8tia4meCBHic7Is9icI5E4J5Xbe8CdostCDAo1w/640?wx_fmt=png)
来源：智东西
自AlphaGo引爆人工智能大潮以来，其中的大热门——机器学习，不仅在计算机视觉、语音处理等领域不断突破技术的瓶颈，而且开始快速普及到安防、金融、医疗等多个传统行业。从刷脸支付到个性化推荐，机器学习已经影响了人们生活的方方面面。
众所周知，统一规范的行业标准有助于推动良性竞争，从而催化底层技术升级和行业发展。然而，机器学习领域长期缺乏统一标准。
直到去年，硅谷AI芯片的一半大佬与以图灵奖得主为代表的学术大咖联手解决了这一问题——共同发布了全球首个用于测量和提高机器学习软硬件性能的国际基准测试MLPerf。MLPerf提供了神经网络常执行的7项任务，在谷歌、英特尔、NVIDIA均参与的首批测试结果中，NVIDIA在6项测试中拔得头筹。
以下为整理呈现的干货：
**![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/z7ZD1WagSLgFt8txemHtrKUiaFRQ3Fzr7pFR0j3pib9T80DoiaBMEzBnQicBhcziaGqvTSkT9cntnXNDibhGaHEg4AdA/640?wx_fmt=png)**
## **AI基准测试大丰收！NVIDIA摘得六项桂冠**
得益于算力的提升，机器学习在近几年突飞猛进地发展。训练和推理作为机器学习的两个重要环节，对算力提出了不同的需求。在训练领域，有GPU一家独大，在推理领域，则百花齐放各显神通。
为了解及其学习的对比评测问题，谷歌、百度、英特尔、NVIDIA、斯坦福等数十家产学界巨擘联合打造了一个全新高端基准测试套件——MLPerf基准测试，这也是世界上是第一个客观的机器学习性能评测标准，覆盖了物体识别、语言翻译等7类最常见的机器学习任务。
该基准测试针对不同任务设置了不同基准，并沿用了哈佛大学Fathom和斯坦福大学DAWNBench的一些评测思路，最终取达到目标质量的平均训练时间作为最终评估标准，同时保证足够的代表性和公正性。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBUAJRmSr5xFl5EttTDJ4kNIcX6o5H8ib9Zp9iaaQVXefXticLuvSZLRQA0kriaGJbQDicxs3ewiamxOe6pQ/640?wx_fmt=png)
首轮主流机器学习硬件平台的竞赛结果已经公布在MLPerf官网上。其中表现最为亮眼的当属NVIDIA，它不仅是唯一一家参与多达6项基准测试的公司，而且在6项测试结果中，均以遥遥领先的成绩拿下第一名。
NVIDIA硬件产品不仅在上图测试榜单中性能表现突出，而且相较其自身前几代产品也进步飞速。例如在图像分类项目中，NVIDIA主打的Tesla K80需要用25天完成任务，短短三年间，DGX集群把时间缩短到了6.3分钟，性能大幅提升。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBUAJRmSr5xFl5EttTDJ4kNIIaN3vFiaTXUxpmLzWrTWaewsd5wWOtmCMvibYoZ936QZ9cjQ95jcFb6A/640?wx_fmt=png)
**![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/z7ZD1WagSLgFt8txemHtrKUiaFRQ3Fzr7NHvWWGfxjWEk4ybhb9ddw81errHd3N5glhaSicHibO85fKNFGvEdxJ1w/640?wx_fmt=png)**
## **专为深度学习设计：DGX-1的两大创新黑科技**
在MLPerf大规模测试的物体检测和非递归翻译测试分项中，NVIDIA分别用时72.1分钟和2.7分钟，这些抢眼表现均归功于NVIDIA DGX-1集群。而DGX-1之所以在机器学习方面有着位居市场前列的性能，则源自NVIDIA专为深度学习设计的两大创新技术。
### **1、深度学习专用核心：Tensor核心**
Tensor核心是一种新兴张量处理核心，是NVIDIA专为深度学习应用而设计的专用ASIC单元，能在一个时钟周期内实现两个4×4矩阵的乘法和一个4×4矩阵的加法。它与Volta的其他微架构整合在一起，已支持Caffe、TensorFlow等越来越多的深度学习框架。
Tensor核心的矩阵乘累加计算是一种混合精度运算，NVIDIA提供两种方式来用Tensor核心实现矩阵乘法加速。一个是在CUDA编程里实现Tensor核心的调用，另一个是利用cuDNN 7.0、TensorRT3.0等现成库函数来调用Tensor核心。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBUAJRmSr5xFl5EttTDJ4kNIP17bVjkAgM62RfbwEL7jCoibQlF6JbNHw7iaSUg8d8Aupt5vxzGk0ubg/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBUAJRmSr5xFl5EttTDJ4kNIWWbfUVG8kic95dveo3gPuLwhbgYqYJ38a0ALTIricy1psyePyg5TjsPw/640?wx_fmt=png)
得益于Tensor核心的超强加速能力，Volta产品实现3倍于上一代产品的性能突破。Tensor核心在训练方面可提供高达12倍的TFLOPS峰值，在推理方面可提供6倍的TFLOPS峰值。
正是有这一超强核心的加持，NVIDIA陆续推出堪称深度学习核弹的NVIDIA Tesla V100加速器，以及性能媲美250台CPU服务器的超级计算机DGX-1。
### **2、超快数据交换：NVLink**
单节点本地内存不足的问题一直是阻碍大规模并行计算的一大难题，为了实现高效的数据传输，不仅需要有超强的GPU，还对GPU与GPU、CPU以及节点内其他设备间的数据交换通道带宽有着更高的要求。
在NVLink问世之前，GPU并行计算通过PCle总线来实现。彼时GPU通信双向带宽可达到32GB/s，而NVLink的出现彻底碾压了PCle。NVLink具有25GB/s的单向带宽、50GB/s的双向带宽。整合6条NVLink总线后，GPU的总带宽可以提速至300GB/s，比PCle 3.0足足快了将近10倍。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBUAJRmSr5xFl5EttTDJ4kNItzQokCW1CJmkhLvFgdDlwmXBtZkTGkicJricZAmNm0O8Pr6zpU40ibKbQ/640?wx_fmt=png)
NVLink是NVIDIA打造的世界上第一个灵活可扩展的高速GPU互连方案，采用NVIDIA全新高速信号互联技术（NVHS），通过差分对传输数据，速率高达25Gb/s，单个链路支持端点间高达50GB/s的双向带宽，多个链路整合在一起，就能实现处理器间更高的带宽。
以拥有6个NVLink链路的Tesla V100为例，采用NVLink总线的V100相比采用PCle总线，能将相同配置服务器的总体性能提高约30%。
**![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/z7ZD1WagSLgFt8txemHtrKUiaFRQ3Fzr7w2M9yjpArMeUKpITwBZXMbI4Qw6icuooFF0Bkq9ktyQzcdHf9hwcgxg/640?wx_fmt=png)**
## **从V100 GPU到DGX-1深度学习超级计算机**
Tensor 核心在被誉为“宇宙最快GPU”的Tesla V100 GPU加速器中体现，集合Tensor核心和NVLink两大创新技术于一体的正是超强算力的集大成者DGX-1深度学习超级计算机的核心组件。
### **1、V100 GPU加速器**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBUAJRmSr5xFl5EttTDJ4kNIbLaf4Gl4FQl6YzzUSQVgdZB1NUib5Uzicj3yZ7INNNEiaTHW4SiaCXapPA/640?wx_fmt=png)
Tesla V100 GPU加速器是NVIDIA性能强大的通用GPU，可同时实现AI训练、AI推理、HPC的加速。一个Tesla V100芯片采用12nm工艺设计，面积为815平方毫米，包含210亿颗晶体管，共有5120个CUDA核心和640个Tensor核心，并支持300GB/s双向带宽的NVLink2.0高速互联总线协议。单个V100 GPU 可提供高达100个CPU的性能。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBUAJRmSr5xFl5EttTDJ4kNI65hNtibH7YnysicRiclNJbnS2ShP1Y62cNdxjdNXsZ6m2hlI200WSdEAA/640?wx_fmt=png)
看中V100所能提供的业界顶级算力，美国橡树岭国家实验室的Summit超级计算机搭建了超过3400台Tesla V100 GPU服务器，其浮点计算性能较上一代超级计算机Titan提升约10倍。此外，日本国家先进工业科学和计算研究所（AIST）也在其数据中心部署4352颗V100 GPU，能实现37 petaFLOPS的双精度浮点计算性能。
Tesla V100已经如此能打，集成了8颗V100的DGX-1超级计算机更是成为加速深度学习应用开发的基础。
### **2、DGX-1深度学习超级计算机**
最后，我们来看看NVIDIA拿下6项MLPerf基准测试第一名的头号功臣DGX-1。它是一个高性能、全栈式深度学习超级计算机，内置8颗Tesla V100 GPU，混合精度算力可达1 petaFLOPS。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBUAJRmSr5xFl5EttTDJ4kNI37CE2dzYySHrJArZ6eaPpiad8Un8KiapFuIJvsLNGRvtm3Dq4j76kZvA/640?wx_fmt=png)
DGX-1不仅拥有超强的硬件配置，还提供全集成的NVIDIA DGX软件堆栈，能够提供比普通GPU服务器环境更强大的性能，实现深度学习研发环境快速部署。
如今，DGX-1已列入许多国内外科研机构的入手清单。以北京大学数字媒体所为例，他们利用DGX-1来加速深度学习系统模型训练的效率，尤其在视频行为识别和视频编解码等方面提升明显。
从制造业、癌症研究到自动驾驶，DGX-1在产业界的应用更加广泛。例如网易感知与智能中心就采用DGX-1，为网易全产业链的AI落地提供算力支撑；在金融行业，当下有几个主要的银行和保险公司也开始使用DGX-1快速处理数据，从而增加评估损失的准确率。
通过MLPerf基准测试公布的首轮结果，我们直观地看到了英特尔、谷歌、NVIDIA不同架构的硬件产品在多个机器学习领域的正面较量。其中，NVIDIA DGX-1深度学习超级计算机横扫六项AI性能第一，展现超强技术实力，强力推动AI研究的发展。
回溯到过去两年，NVIDIA GPU在一系列创新技术的加持下，实现了加速性能大爆发，以强大的浮点计算能力、更低成本和完整生态体系的优势，帮助越来越多的企业实现智能化升级。
尽管MLPerf基准测试还在完善中，它已经开始在机器学习领域为产学界提供高公信力的数据参考，从长远来看，将推动机器学习硬件良性竞争，催化技术的创新与突破。
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
