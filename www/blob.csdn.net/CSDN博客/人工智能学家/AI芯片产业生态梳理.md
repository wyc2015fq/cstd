# AI芯片产业生态梳理 - 人工智能学家 - CSDN博客
2017年10月02日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：1276
![640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBVMjlaMkeTr6ypzs2m1gCqAicpD111wb5cQ06hh8ulQ5wwZHibiamTxdtv7jKNlAxghJ8xwqnaFL4hvA/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1)
*来源：  怪诞笔记      *
*概要：**AI芯片作为产业核心，也是技术要求和附加值最高的环节，在AI产业链中的产业价值和战略地位远远大于应用层创新。腾讯发布的《中美两国人工智能产业发展全面解读》报告显示，基础层的处理器/芯片企业数量来看，中国有14家，美国33家。本文将对这一领域产业生态做一个简单梳理。*
![640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/NAaKDV9nTRibxOpsMk1wSpt6BrAGcUaXOhSC7MJQUJ6ZdZ9zMfnIjhsljeQBxgszkpib2uz7wKykwqQcaawk3YzA/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1)
Inference环节指利用训练好的模型，使用新的数据去“推理”出各种结论，如视频监控设备通过后台的深度神经网络模型，判断一张抓拍到的人脸是否属于黑名单。虽然Inference的计算量相比Training少很多，但仍然涉及大量的矩阵运算。在推理环节，GPU、FPGA和ASIC都有很多应用价值。
从应用场景来看，可以分成“Cloud/DataCenter(云端)”和“Device/Embedded(设备端)”两大类。
在深度学习的Training阶段，由于对数据量及运算量需求巨大，单一处理器几乎不可能独立完成一个模型的训练过程，因此，Training环节目前只能在云端实现，在设备端做Training目前还不是很明确的需求。
在Inference阶段，由于目前训练出来的深度神经网络模型大多仍非常复杂，其推理过程仍然是计算密集型和存储密集型的，若部署到资源有限的终端用户设备上难度很大，因此，云端推理目前在人工智能应用中需求更为明显。GPU、FPGA、ASIC(Google TPU1.0/2.0)等都已应用于云端Inference环境。在设备端Inference领域，由于智能终端数量庞大且需求差异较大，如ADAS、VR等设备对实时性要求很高，推理过程不能交由云端完成，要求终端设备本身需要具备足够的推理计算能力，因此一些低功耗、低延迟、低成本的专用芯片也会有很大的市场需求。
按照上述两种分类，我们得出AI芯片分类象限如下图所示。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/NAaKDV9nTRibxOpsMk1wSpt6BrAGcUaXOvFib7yyGI1iakibhQNRyIzVXVFEjiazOI6spXfdD1Kwib77yOicUUrhteYFg/640?wx_fmt=jpeg)
除了按照功能场景划分外，AI芯片从技术架构发展来看，大致也可以分为四个类型：
1、通用类芯片，代表如GPU、FPGA；
2、基于FPGA的半定制化芯片，代表如深鉴科技DPU、百度XPU等；
3、全定制化ASIC芯片，代表如TPU、寒武纪 Cambricon-1A等；
4、类脑计算芯片，代表如IBM TrueNorth、westwell、高通Zeroth等。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/NAaKDV9nTRibxOpsMk1wSpt6BrAGcUaXO9INXh7X8UydSfDfYRxFnD516fJNuIcDBKxiasBEvtdsJ5HYJRZdBqicQ/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/NAaKDV9nTRibxOpsMk1wSpt6BrAGcUaXOElT2ibOd7QIo5CL3hiaAB1ySMZdmeWm9PHsusZAdVZic6M0ulpOJZGb1Q/640?wx_fmt=jpeg)
**（一）Training训练**
2007年以前，人工智能研究受限于当时算法、数据等因素，对于芯片并没有特别强烈的需求，通用的CPU芯片即可提供足够的计算能力。Andrew Ng和Jeff Dean打造的Google Brain项目，使用包含16000个CPU核的并行计算平台，训练超过10亿个神经元的深度神经网络。但CPU的串行结构并不适用于深度学习所需的海量数据运算需求，用CPU做深度学习训练效率很低，在早期使用深度学习算法进行语音识别的模型中，拥有429个神经元的输入层，整个网络拥有156M个参数，训练时间超过75天。
与CPU少量的逻辑运算单元相比，GPU整个就是一个庞大的计算矩阵，GPU具有数以千计的计算核心、可实现10-100倍应用吞吐量，而且它还支持对深度学习至关重要的并行计算能力，可以比传统处理器更加快速，大大加快了训练过程。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/NAaKDV9nTRibxOpsMk1wSpt6BrAGcUaXO7GFw2QiaAhH3wdNe2iaSThno7FT09fELPz3TZEz4Xx1ibibV79m0Xq1OSg/640?wx_fmt=jpeg)
从上图对比来看，在内部结构上，CPU中70%晶体管都是用来构建Cache(高速缓冲存储器)和一部分控制单元，负责逻辑运算的部分(ALU模块)并不多，指令执行是一条接一条的串行过程。GPU 由并行计算单元和控制单元以及存储单元构成，拥有大量的核(多达几千个)和大量的高速内存，擅长做类似图像处理的并行计算，以矩阵的分布式形式来实现计算。同CPU不同的是，GPU的计算单元明显增多，特别适合大规模并行计算。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/NAaKDV9nTRibxOpsMk1wSpt6BrAGcUaXOSOS4sps3fv8LLEaWspY79vWpyVj5HITd2nW3YWSZKaCVZBy80t7vbw/640?wx_fmt=jpeg)
在人工智能的通用计算GPU市场，NVIDIA现在一家独大。2010年NVIDIA就开始布局人工智能产品，2014年发布了新一代PASCAL GPU芯片架构，这是NVIDIA的第五代GPU架构，也是首个为深度学习而设计的GPU，它支持所有主流的深度学习计算框架。2016年上半年，NVIDIA又针对神经网络训练过程推出了基于PASCAL架构的TESLA P100芯片以及相应的超级计算机DGX-1。DGX-1包含TESLA P100 GPU加速器，采用NVLINK互联技术，软件堆栈包含主要深度学习框架、深度学习SDK、DIGITS GPU训练系统、驱动程序和CUDA，能够快速设计深度神经网络(DNN)，拥有高达170TFLOPS的半精度浮点运算能力，相当于250台传统服务器，可以将深度学习的训练速度加快75倍，将CPU性能提升56倍。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/NAaKDV9nTRibxOpsMk1wSpt6BrAGcUaXOEWYAnD2Esc0jfMXIlWpV8aJOXv7kvczeyXVFn2MY4S6MicsKtXSES8A/640?wx_fmt=jpeg)
Training市场目前能与NVIDIA竞争的就是Google。今年5月份Google发布了TPU 2.0，TPU(TensorProcessing Unit)是Google研发的一款针对深度学习加速的ASIC芯片，第一代TPU仅能用于推理，而目前发布的TPU 2.0既可以用于训练神经网络，又可以用于推理。据介绍，TPU2.0包括了四个芯片，每秒可处理180万亿次浮点运算。Google还找到一种方法，使用新的计算机网络将64个TPU组合到一起，升级为所谓的TPU Pods，可提供大约11500万亿次浮点运算能力。Google表示，公司新的深度学习翻译模型如果在32块性能最好的GPU上训练，需要一整天的时间，而八分之一个TPU Pod就能在6个小时内完成同样的任务。目前Google 并不直接出售TPU芯片，而是结合其开源深度学习框架TensorFlow为AI开发者提供TPU云加速的服务，以此发展TPU2的应用和生态，比如TPU2同时发布的TensorFlow Research Cloud (TFRC) 。
上述两家以外，传统CPU/GPU厂家Intel和AMD也在努力进入这Training市场，如Intel推出的Xeon Phi+Nervana方案，AMD的下一代VEGA架构GPU芯片等，但从目前市场进展来看很难对NVIDIA构成威胁。初创公司中，Graphcore 的IPU处理器(IntelligenceProcessing Unit)据介绍也同时支持Training和Inference。该IPU采用同构多核架构，有超过1000个独立的处理器；支持All-to-All的核间通信，采用BulkSynchronous Parallel的同步计算模型；采用大量片上Memory，不直接连接DRAM。
总之，对于云端的Training(也包括Inference)系统来说，业界比较一致的观点是竞争的核心不是在单一芯片的层面，而是整个软硬件生态的搭建。NVIDIA的CUDA+GPU、Google的TensorFlow+TPU2.0，巨头的竞争也才刚刚开始。
**（二）Inference On Cloud云端推理**
相对于Training市场上NVIDIA的一家独大，Inference市场竞争则更为分散。若像业界所说的深度学习市场占比(Training占5%，Inference占95%)，Inference市场竞争必然会更为激烈。
在云端推理环节，虽然GPU仍有应用，但并不是最优选择，更多的是采用异构计算方案(CPU/GPU +FPGA/ASIC)来完成云端推理任务。FPGA领域，四大厂商(Xilinx/Altera/Lattice/Microsemi)中的Xilinx和Altera（被Intel收购）在云端加速领域优势明显。Altera在2015年12月被Intel收购，随后推出了Xeon+FPGA的云端方案，同时与Azure、腾讯云、阿里云等均有合作；Xilinx则与IBM、百度云、AWS、腾讯云合作较深入，另外Xilinx还战略投资了国内AI芯片初创公司深鉴科技。目前来看，云端加速领域其他FPGA厂商与Xilinx和Altera还有很大差距。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/NAaKDV9nTRibxOpsMk1wSpt6BrAGcUaXO9vibCmxx75keNKe33UNBYggIchvc2X7ialHibn6niaU5XbNYaGMfibLT7EQ/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/NAaKDV9nTRibxOpsMk1wSpt6BrAGcUaXOKoj9JbrzZhuyAWHuGNJ2u38yVcY7JC9sxRMsesBM7rrY7bTx7D7Stw/640?wx_fmt=jpeg)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/NAaKDV9nTRibxOpsMk1wSpt6BrAGcUaXOTfwLhicicAkJichlYfjIjlwCjHMicr2ibBqrb6YM8bSg1znTkphzWpNJeyQ/640?wx_fmt=jpeg)
**1）智能手机**
华为9月初发布的麒麟970 AI芯片就搭载了神经网络处理器NPU(寒武纪IP)。麒麟970采用了TSMC 10nm工艺制程，拥有55亿个晶体管，功耗相比上一代芯片降低20%。CPU架构方面为4核A73+4核A53组成8核心，能耗同比上一代芯片得到20%的提升；GPU方面采用了12核Mali G72 MP12GPU，在图形处理以及能效两项关键指标方面分别提升20%和50%；NPU采用HiAI移动计算架构，在FP16下提供的运算性能可以达到1.92 TFLOPs，相比四个Cortex-A73核心，处理同样的AI任务，有大约50倍能效和25倍性能优势。
苹果最新发布的A11仿生芯片也搭载了神经网络单元。据介绍，A11仿生芯片有43亿个晶体管，采用TSMC 10纳米FinFET工艺制程。CPU采用了六核心设计，由2个高性能核心与4个高能效核心组成。相比A10 Fusion，其中两个性能核心的速度提升了25%，四个能效核心的速度提升了70%；GPU采用了苹果自主设计的三核心 GPU 图形处理单元，图形处理速度与上一代相比最高提升可达 30% 之多；神经网络引擎NPU采用双核设计，每秒运算次数最高可达 6000 亿次，主要用于胜任机器学习任务，能够识别人物、地点和物体等，能够分担 CPU 和 GPU 的任务，大幅提升芯片的运算效率。
另外，高通从 2014 年开始也公开了NPU的研发，并且在最新两代骁龙 8xx 芯片上都有所体现，例如骁龙 835 就集成了“骁龙神经处理引擎软件框架”，提供对定制神经网络层的支持，OEM 厂商和软件开发商都可以基于此打造自己的神经网络单元。ARM在今年所发布的 Cortex-A75 和 Cortex-A55中也融入了自家的AI 神经网络DynamIQ技术，据介绍，DynamIQ技术在未来 3-5 年内可实现比当前设备高50倍的AI性能，可将特定硬件加速器的反应速度提升10倍。总体来看，智能手机未来AI芯片的生态基本可以断定仍会掌握在传统SoC商手中。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/NAaKDV9nTRibxOpsMk1wSpt6BrAGcUaXOD2qERibSKdPEialqhMXUKAkJJPM6XcvMdNlDeFwFAB6BrJK4yvYo2zXQ/640?wx_fmt=jpeg)
**2）自动驾驶**
NVIDIA去年发布自动驾驶开发平台DRIVE PX2，基于16nm FinFET工艺，功耗高达250W，采用水冷散热设计；支持12路摄像头输入、激光定位、雷达和超声波传感器；CPU采用两颗新一代NVIDIA Tegra处理器，当中包括了8个A57核心和4个Denver核心；GPU采用新一代Pascal架构，单精度计算能力达到8TFlops，超越TITAN X，有后者10倍以上的深度学习计算能力。Intel收购的Mobileye、高通收购的NXP、英飞凌、瑞萨等汽车电子巨头也提供ADAS芯片和算法。初创公司中，地平线的深度学习处理器(BPU，BrainProcessor Unit)IP及其自研雨果(Hugo)平台也是重点面向自动驾驶领域。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/NAaKDV9nTRibxOpsMk1wSpt6BrAGcUaXO0tLicaCPVXNvBWJ5nzibCF5HmibC1FMgicfibruotHjDyvBzcQzQUmwHZibA/640?wx_fmt=jpeg)
**3）计算机视觉领域**
Intel收购的Movidius是主要的芯片提供商，大疆无人机、海康威视和大华股份的智能监控摄像头均使用了Movidius的Myriad系列芯片。目前国内做计算机视觉技术的公司中，商汤科技、Face++、云从、依图等，未来有可能随着其自身计算机视觉技术的积累渐深，部分公司向上游延伸去做CV芯片研发。另外，国内还有如人人智能、智芯原动等创业公司提供摄像头端的AI加速IP及芯片解决方案。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/NAaKDV9nTRibxOpsMk1wSpt6BrAGcUaXOTV7m74wGVQFDYF3FquGfItViarkw6zeoqmiczeoBqEBUEWb8fmic52Cmw/640?wx_fmt=jpeg)
**4）其他**
VR设备芯片的代表为微软为自身VR设备Hololens而研发的HPU芯片，这颗由台积电代工的芯片能同时处理来自5个摄像头、一个深度传感器以及运动传感器的数据，并具备计算机视觉的矩阵运算和CNN运算的加速功能；语音交互设备芯片方面，国内有启英泰伦以及云知声两家公司，其提供的芯片方案均内置了为语音识别而优化的深度神经网络加速方案，实现设备的语音离线识别；在泛IOT领域，NovuMind设计了一种仅使用3×3卷积过滤器的AI芯片，第一款芯片原型预计今年底推出，预计可实现耗能不超过5瓦进行15万亿次浮点运算，可以广泛应用于各类小型的互联网“边缘”设备。
**（四）新架构 - 类脑计算芯片**
“类脑芯片”是指参考人脑神经元结构和人脑感知认知方式来设计的芯片，其目标是开发出打破冯·诺依曼架构体系的芯片。这一领域目前仍处于探索阶段，如欧盟支持的SpiNNaker和BrainScaleS、斯坦福大学的Neurogrid、IBM公司的TrueNorth以及高通公司的Zeroth等；国内Westwell、清华大学、浙江大学、电子科技大学等也有相关研究。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/NAaKDV9nTRibxOpsMk1wSpt6BrAGcUaXO1bS0EONJyZm7syH3ugr8PkXLXW5CXo37FEMTUfk4l5SBf0iaqqZicM5w/640?wx_fmt=jpeg)
IBM的TrueNorth，2014年公布。在一颗芯片上集成了4096个内核，100万个神经元、2.56亿个可编程突触，使用了三星的28nm的工艺，共540万个晶体管；每秒可执行460亿次突触运算，总功耗为70mW，每平方厘米功耗20mW。IBM的最终目标就是希望建立一台包含100亿个神经元和100万亿个突触的计算机，这样的计算机要比人类大脑的功都强大10 倍，而功耗只有一千瓦，而且重量不到两升。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/NAaKDV9nTRibxOpsMk1wSpt6BrAGcUaXOkmg382ibtiaCy71HztThem97ggwk8RziaxZMA2CWRbrYBDExXD14qbfWQ/640?wx_fmt=jpeg)
国内AI初创公司西井科技Westwell是用FPGA模拟神经元以实现SNN的工作方式，有两款产品：
1、仿生类脑神经元芯片DeepSouth(深南)，第三代脉冲神经网络芯片SNN，基于STDP(spike-time-dependentplasticity)的算法构建完整的突触神经网络，由电路模拟真实生物神经元产生脉冲的仿生学芯片，通过动态分配的方法能模拟出高达5000万级别的“神经元”，功耗为传统芯片在同一任务下的几十分之一到几百分之一。
2、深度学习类脑神经元芯片DeepWell(深井)，处理模式识别问题的通用智能芯片，基于在线伪逆矩阵求解算法(OPIUM lite)对芯片中神经元间的连接权重进行学习和调整；拥12800万个神经元，通过专属指令集调整芯片中神经元资源的分配；学习与识别速度远远高于运行在通用硬件(如CPU, GPU)上的传统方法(如CNN)，且功耗更低。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/NAaKDV9nTRibxOpsMk1wSpt6BrAGcUaXOfeZc9Q3vnd6PiclUX4I0W5LgicfFiclBick9Ele8q5ha1PyMTTZ1d2wcRA/640?wx_fmt=jpeg)
**1）寒武纪科技&Cambricon 1A**
寒武纪科技成立于2016年，总部在北京，创始人是中科院计算所的陈天石、陈云霁兄弟，近期刚刚完成了一亿美元A轮融资，阿里巴巴创投、联想创投、国科投资、中科图灵、元禾原点、涌铧投资联合投资，成为全球AI芯片领域第一个独角兽初创公司。
寒武纪是全球第一个成功流片并拥有成熟产品的AI芯片公司，拥有终端AI处理器IP和云端高性能AI芯片两条产品线。2016年发布的寒武纪1A处理器(Cambricon-1A)是世界首款商用深度学习专用处理器，面向智能手机、安防监控、无人机、可穿戴设备以及智能驾驶等各类终端设备，在运行主流智能算法时性能功耗比全面超越传统处理器。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/NAaKDV9nTRibxOpsMk1wSpt6BrAGcUaXOJkdtppY1bib4zk5ph1pJgibT1x2jp7Piau833hY6khqZicDW49KoLhClVw/640?wx_fmt=jpeg)
**2）地平线机器人&BPU/盘古**
地平线机器人成立于2015年，总部在北京，创始人是前百度深度学习研究院负责人余凯。公司于去年中完成了A+轮融资，投资方包括了晨兴资本、高瓴资本、红杉资本、金沙江创投、线性资本、创新工场、真格基金、双湖投资、青云创投、祥峰投资、DST等。据介绍，公司近期即将完成B轮融资。
BPU(BrainProcessing Unit)是地平线机器人自主设计研发高效的人工智能处理器架构IP，支持ARM/GPU/FPGA/ASIC实现，专注于自动驾驶、人脸图像辨识等专用领域。2017年，地平线基于高斯架构的嵌入式人工智能解决方案将会在智能驾驶、智能生活、公共安防三个领域进行应用，第一代BPU芯片“盘古”目前已进入流片阶段，预计在今年下半年推出，能支持1080P的高清图像输入，每秒钟处理30帧，检测跟踪数百个目标。地平线的第一代BPU采用TSMC的40nm工艺，相对于传统CPU/GPU,能效可以提升2~3个数量级(100~1,000倍左右)。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/NAaKDV9nTRibxOpsMk1wSpt6BrAGcUaXOOLLhNNm8cMZbQeb3J6rR074Xhg4dtap1ibsB4ztGb33pGibKAnfeXQYg/640?wx_fmt=jpeg)
**3）深鉴科技&DPU**
深鉴科技成立于2016年，总部在北京。由清华大学与斯坦福大学的世界顶尖深度学习硬件研究者创立，今年初完成了A轮融资，投资方包括了联发科、赛灵思、金沙江创投、高榕资本、清华控股、方和资本等。
深鉴科技将其开发的基于FPGA的神经网络处理器称为DPU。到目前为止，深鉴公开发布了两款DPU：亚里士多德架构和笛卡尔架构，其中，亚里士多德架构是针对卷积神经网络CNN而设计；笛卡尔架构专为处理DNN/RNN网络而设计，可对经过结构压缩后的稀疏神经网络进行极致高效的硬件加速。相对于 Intel XeonCPU 与 Nvidia TitanX GPU，应用笛卡尔架构的处理器在计算速度上分别提高189倍与13倍，具有24000倍与3000倍更高能效。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/NAaKDV9nTRibxOpsMk1wSpt6BrAGcUaXOxumrGmHdDJBwQbZ6Dmhia2y8xEbY0RqvZJBvOzTtZt4D00PVEsPocqw/640?wx_fmt=jpeg)
**4）西井科技&DeepSouth/DeepWell**
公司成立于2015年，总部在上海。今年6月完成了A轮融资，投资方包括了复星同浩、源政投资、合力投资、十维资本、喔赢资本等。
西井科技是用FPGA模拟神经元以实现SNN的工作方式，有两款产品：
1、仿生类脑神经元芯片DeepSouth(深南)，第三代脉冲神经网络芯片SNN，基于STDP(spike-time-dependentplasticity)的算法构建完整的突触神经网络，由电路模拟真实生物神经元产生脉冲的仿生学芯片，通过动态分配的方法能模拟出高达5000万级别的“神经元”，功耗为传统芯片在同一任务下的几十分之一到几百分之一。
2、深度学习类脑神经元芯片DeepWell(深井)，处理模式识别问题的通用智能芯片，基于在线伪逆矩阵求解算法(OPIUM lite)对芯片中神经元间的连接权重进行学习和调整；拥12800万个神经元，通过专属指令集调整芯片中神经元资源的分配；学习与识别速度远远高于运行在通用硬件(如CPU, GPU)上的传统方法(如CNN)，且功耗更低。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/NAaKDV9nTRibxOpsMk1wSpt6BrAGcUaXOQibGc2EI3sE4LzDq41uqoJWGybxY5mt0tbrK3OBklBro4Aoia0pU7XdA/640?wx_fmt=jpeg)
**5）云飞励天&IPU**
公司成立于2014年，总部在深圳，由国家“千人计划”特聘专家陈宁和田第鸿博士联合创立，今年3月完成了A轮融资，投资方松禾资本、深投控、红秀盈信、山水从容投资、投控东海、真格基金等。
云天励飞提供视觉智能芯片和解决方案，专注于人工智能领域，以新型处理器、机器学习与大数据技术为核心。公司自主研发的处理器芯片IPU，采用了全新的面向视觉计算的处理器芯片架构，该技术将机器学习效能提升了两个数量级。公司在在深圳搭建的区域级天眼系统，实现了全球首创的“百万人群、秒级定位”，还被采纳为2016年杭州G20峰会和乌镇互联网大会的安全系统提供服务。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/NAaKDV9nTRibxOpsMk1wSpt6BrAGcUaXOXhXsIq4BiabvNJIicOcQicibiaHGFnWjVfez6pXGgLC7thwibcuF41vgnrWA/640?wx_fmt=jpeg)
**6）人人智能&FaceOS**
人人智能成立于2016年，是ARM OpenAI实验室核心合作企业。公司于去年底完成了ARM和英诺天使基金的天使轮融资，据报道目前正在启动新一轮融资。
人人智能提供一个基ARM的人脸识别核心芯片即模组方案，识别模组是独创的支持深度学习算法的嵌入式高性能ARM平台，支持外接摄像机从视频流检测和载取人脸照片等功能。据介绍，人人智能发布的“智能芯”是国内首个人脸识别硬件模组，尺寸仅为86mm*56mm*21mm，集成了人工智能操作系统FaceOS。通过将人工智能算法进行集成产品化，能够把产品的研发周期减少60%，成本降低50%。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/NAaKDV9nTRibxOpsMk1wSpt6BrAGcUaXOXeqkGN8MKfDm8jN2tm4aicjRjAoRn0WWNX4j6Jsw2Juice35PD3KH3Tw/640?wx_fmt=jpeg)
**7）启英泰伦&CI1006**
启英泰伦于2015年11月在成都成立，是一家语音识别芯片研发商，投资方包括了Roobo、汇声信息等。
启英泰伦的CI1006是基于ASIC架构的人工智能语音识别芯片，包含了脑神经网络处理硬件单元，能够完美支持DNN运算架构，进行高性能的数据并行计算，可极大的提高人工智能深度学习语音技术对大量数据的处理效率。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/NAaKDV9nTRibxOpsMk1wSpt6BrAGcUaXO5VFzhiaYSy8a8zT0niclb7Tqk9jDkpCicGTMMbesQ4AmtjibbxQlQK5wiaA/640?wx_fmt=jpeg)
**8）云知声&UniOne芯片**
云知声是一家智能语音识别技术公司，成立于2012年，总部位于北京。今年8月刚刚获得3亿人民币战略投资，其中部分资金将用于加大人工智能专用芯片UniOne的研发力度。
UniOne将内置DNN处理单元，兼容多麦克风、多操作系统，对任何的场景不做限制，无论是在智能的空调上、车载上或其他智能设备上都可以植入这个芯片，该芯片具有高集成度的，低功耗、低成本的优点。与此同时，公司还有IVM-M高性能嵌入式芯片，基于高通wifi模组，提供高性价比的物联网语音交互整体方案，主要应用在智能空调，厨电等职能家具产品上；基于Linux系统设计的Unitoy芯片可一站式解决儿童陪伴式机器人的唤醒、识别、设备互联能力。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/NAaKDV9nTRibxOpsMk1wSpt6BrAGcUaXO3eGWSicsS8M5dOr7VPUZffueHe4Bia1BCdicwdBOaumiaTSC4lsgmjm2CQ/640?wx_fmt=jpeg)
**9）百度&XPU**
百度2017年8月Hot Chips大会上发布了XPU，这是一款256核、基于FPGA的云计算加速芯片。合作伙伴是赛思灵(Xilinx)。XPU采用新一代 AI 处理架构，拥有GPU的通用性和FPGA的高效率和低能耗，对百度的深度学习平台PaddlePaddle做了高度的优化和加速。据介绍，XPU关注计算密集型、基于规则的多样化计算任务，希望提高效率和性能，并带来类似CPU的灵活性。但目前XPU有所欠缺的仍是可编程能力，而这也是涉及FPGA时普遍存在的问题。到目前为止，XPU尚未提供编译器。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/NAaKDV9nTRibxOpsMk1wSpt6BrAGcUaXOPPlLjftSzL8IrmombxrFrqAOUAruiawqXmSXP9FJ1EIDUPdfluMkgRw/640?wx_fmt=jpeg)
**10）NovuMind**
NovuMind成立于2015年，公司创始人是原百度异构计算小组负责人吴韧，在北京及硅谷设有办公室。公司于2017年初完成了A轮融资，投资方包括了真格基金、宽带资本、英诺天使基金、洪泰基金、臻云创投、极客帮创投等，据报道近期正在筹备新一轮融资。
NovuMind主要为智能为汽车、安防、医疗、金融等领域提供ASIC芯片，并提供训练模型的全栈式AI解决方案。与Nvidia GPU或Cadence DSP的通用深度学习芯片不同，NovuMind专注于开发一种“非常专用但非常高效地进行推理”的深度学习加速器芯片。NovuMind设计了一种仅使用3×3卷积过滤器的AI芯片，通过使用独特的张量处理架构(tensorprocessing architecture)直接对三维Tensor进行处理，新芯片将支持Tensorflow、Cafe和Torch模型。。NovuMind的第一个AI芯片(原型)预计会在17年圣诞节前推出。到明年2月份应用程序准备就绪，并能够在该芯片上实现耗能不超过5瓦进行15万亿次浮点运算。NovuMind的第二个芯片，耗能将不超过1瓦，计划在2018年中期面世。   
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/NAaKDV9nTRibxOpsMk1wSpt6BrAGcUaXOzm7Vg8Es3weyYxH6GAdjaicsU4a13ibmKq1IrsEpGhpTOQGGAHtFHXtQ/640?wx_fmt=jpeg)
**11）华为&麒麟970芯片**
麒麟970搭载的神经网络处理器NPU采用了寒武纪IP。麒麟970采用了TSMC 10nm工艺制程，拥有55亿个晶体管，功耗相比上一代芯片降低20%。CPU架构方面为4核A73+4核A53组成8核心，能耗同比上一代芯片得到20%的提升；GPU方面采用了12核Mali G72 MP12GPU，在图形处理以及能效两项关键指标方面分别提升20%和50%；NPU采用HiAI移动计算架构，在FP16下提供的运算性能可以达到1.92 TFLOPs，相比四个Cortex-A73核心，处理同样的AI任务，有大约50倍能效和25倍性能优势。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/NAaKDV9nTRibxOpsMk1wSpt6BrAGcUaXOolWqicaJibEmericYCNMrU15cXYRZ0iclB3tt4yiafyXedOZ5RmI5MT6Q7Q/640?wx_fmt=jpeg)
**12）中星微电子&NPU**
中星微2016年6月推出量产的NPU芯片“星光智能一号”，NPU采用了“数据驱动并行计算”的架构，单颗NPU(28nm)能耗仅为400mW，极大地提升了计算能力与功耗的比例，特别擅长处理视频、图像类的海量多媒体数据。每个NPU由四个NPU内核构成；每个NPU内核包括两个数据流处理器和一个长字处理器；每个数据流处理器由8个长字或16个短字的SIMD(单指令多数据)处理单元组成；每个NPU峰值可提供38Gops或76Gops短字的处理能力；支持通用的基于深度学习的神经网络层(ConvolutionLayer/Pooling Layer/Full Connection Layer/Activation Layer/Custom SpecificLayer)。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/NAaKDV9nTRibxOpsMk1wSpt6BrAGcUaXORAdQ6NIibOB2azgR7V3hKibsDCqxibVMSJICEG89m2qRQheFNiaSZ2JU1Q/640?wx_fmt=jpeg)
