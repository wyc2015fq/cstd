# 一文看懂谷歌的AI芯片布局，边缘端TPU将大发神威 - 人工智能学家 - CSDN博客
2018年11月19日 23:17:52[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：117
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBXTv17KraKfDDOuav2hgMDAVN5gVX7Zh58vuSuhr7uDNOtDjHQpbMYUs923EibDg45OhSXrVwEmwDg/640?wx_fmt=png)
来源：新电子
2018年7月Google在其云端服务年会Google Cloud Next上正式发表其边缘(Edge)技术，与另两家国际公有云服务大厂Amazon/AWS、Microsoft Azure相比，Google对于边缘技术已属较晚表态、较晚布局者，但其技术主张却与前两业者有所不同。
**Google AI布局逐渐走向边缘**
除了同样提倡基础的物联网闸道器(IoT Gateway)软件Edge IoT Core、人工智慧/机器学习(AI/ML)软件Edge ML外，还针对人工智慧/机器学习推出专属的加速运算芯片，称为Google Edge TPU(图1)，成为此次盛会一大焦点。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/ghsnibD4zwPvA9Kjv25HnZeH0v1hns7h1kz6nsb3QH9eKBS7AprLVicxWKkIWknQfzEvMj507gpBkdzYQ1AKIzVg/640?wx_fmt=png)
图1 Google发表仅有1美分铜板面积不到的人工智慧加速运算芯片Edge TPU。
资料来源：Google官网
在Google发表Edge TPU前已发表过Cloud TPU芯片，首次发表是在Google另一个更全面、更盛大的例行年会Google I/O 2016上。Cloud TPU顾名思义用于云端机房，而TPU是TensorFlow Processing Unit的缩写，言下之意是针对TensorFlow而设计的硬件加速运算器，TensorFlow则是Google于2015年11月提出的人工智慧框架，是目前诸多人工智慧框架中的一大主流，其他知名的框架如Caffe/Caffe 2、Apache MXnet等。
目前人工智慧框架百花齐放，其他常见的亦有Keras、PyTorch、CNTK、DL4J、Theano、Torch7、Paddle、DSSTNE、tiny-dnn、Chainer、neon、ONNX、BigDL、DyNet、brainstorm、CoreML等。若以简单譬喻而言，人工智慧的开发撰写如同文书撰写，人工智慧框架就如同记事本、Word等文书处理器，功效在于协助与便利开发撰写。
Google自行开发设计的Cloud TPU仅用于自家云端机房，且已对多种Google官方云端服务带来加速效果，例如Google街景图服务的文字处理、Google相簿的照片分析、乃至Google搜寻引擎服务等。Google Cloud TPU也改版快速，2016年首次发表后2017年推出第二代，2018年推出第三代芯片(图2)。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/ghsnibD4zwPvA9Kjv25HnZeH0v1hns7h1ico17NE9Q8kRibc2rwSB7n79ZNYCiaYcStdPYwwpmlXMYh5BLybenGp0g/640?wx_fmt=png)
图2 Google连续三年在Google I/O上揭露自研的Cloud TPU新技术动向。
资料来源：Google官网
不过，Google之后对Cloud TPU的技术态度似有变化。2018年2月宣布可申请租用TPU运算力，如同Google Cloud Platform(GCP)的公有云服务般，依据运算芯片的使用时间计费，每小时6.5美元(至2018年9月已降至4.5美元) ，与GCP的CPU租用服务相比相当昂贵，GCP的CPU租用服务，以台湾彰化滨海工业区的机房(不同位置的机房费率不同)而言，标准型计价约在0.055至5.28美元间，且8种计费费率中有5种低于1美元/小时。
TPU租用费亦同样高于GPU租用，GCP的NVIDIA GPU租用费率约在0.49至2.48美元间，视规格等级而异(Tesla K80/P100/V100)。Google Cloud TPU虽可租用，但Google是否愿意单独销售Cloud TPU给系统商，让系统商制造及销售TPU运算系统，仍待进一步观察。
在Google推出云端用的Cloud TPU后，让人未预料的是Google也针对边缘提出专属的TPU芯片，然在此前仍有些征兆，即2017年11月Google提出轻量版的TensorFlow Lite(某种程度取代此前的TensorFlow Mobile)，使电力有限的行动装置也能支援TensorFlow，2018年推出的Edge TPU芯片即是以执行TensorFlow Lite为主，而非原宗的TensorFlow。
Google Edge装置内的作业系统为Linux或Android Things，而后执行Google Edge IoT Core基础功能软件、Google Edge ML人工智慧软件，并可选用配置Google Edge TPU。
**![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/ghsnibD4zwPvpwMG9PAr1SX5PabmdraKRVo0fIbZian5SOI7ribgYvxLKIc5iby6micckKr0Jl61S1jrRLichJ4h8z1A/640?wx_fmt=png)**
**Google Edge软硬件架构概观**
图3左侧为物联网感测器，右侧为Google云端系统及服务。另外Edge TPU也支援Android Neural Networks 神经网路应用程式介面(API)，简称NNAPI(图4)。NNAPI于在2017年12月Android 8.1释出时一同提出，NNAPI可视为TensorFlow Lite、Caffe2等框架的基础层。由此可知Edge TPU所支援呼应并加速的软件，于2017年便已先行到位。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/ghsnibD4zwPvA9Kjv25HnZeH0v1hns7h1smMpKWLsIkJQoD8Vdo9OanfZNTEhY1PdRFIaT7IqXlfR0avelToHng/640?wx_fmt=png)
图3 Google Edge装置软硬件架构图 资料来源：Google官网
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/ghsnibD4zwPvA9Kjv25HnZeH0v1hns7h1DvpiaT5BZic6NWbfw9bFfXWMB4XETP9DxX8EExXdYNiaCicrhMfiaSbicg0w/640?wx_fmt=png)
图4 Google Android NNAPI系统架构图，NNAPI可透过硬件抽象层与驱动程式，运用GPU、特定处理器或数位讯号处理器(DSP)等，使人工智慧运算加速。资料来源：Google官网
与Cloud TPU不同的是，Edge TPU估将由Google销售给系统商，再由系统商配置于前端装置内，包含感测器节点、装置或闸道器内，Edge TPU不是自用或租用而是销售。
**![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/ghsnibD4zwPvpwMG9PAr1SX5PabmdraKRVo0fIbZian5SOI7ribgYvxLKIc5iby6micckKr0Jl61S1jrRLichJ4h8z1A/640?wx_fmt=png)**
**Edge TPU技术轮廓**
虽然Google对Cloud TPU、Edge TPU的技术资讯揭露均不多，但仍有若干资讯可推测其背后意向与考量。
首先是Google很明白Edge定位的系统运算力有限，所以Edge TPU的运算任务仅在于执行已训练完成的人工智慧模型，即推测运算、推算工作(Inference，今日多译成「推论」)，真正要大量耗费运算力的模型训练(Training)，依然由充沛运算力的系统事先进行，如工作站、伺服器、云端等，透过CPU/GPU/TPU进行训练。
其次，Edge TPU强调可同时执行处理多组每秒30张高清晰度画质的人工智慧判别运算，显示Edge TPU将用于视讯影像类的人工智慧应用，且为即时判别(30FPS)。
更进一步的说明，Edge TPU只负责人工智慧的加速判别、加速推算，仅为加速器、辅助处理器的角色。因此，必须与系统的主控芯片沟通联系，这方面Edge TPU提供了两种介接的方式，一是PCI Express介面，另一是USB介面。两种介面均适合嵌入式设计，然PCI Express传输率较高，可避免传输瓶颈，而USB介面较可能定位在后装型运用，即前端装置已经存在，但仍可透过USB连接Edge TPU，带来加速效果。
也由于须与Edge装置整合，因此Edge TPU设计之初已尽可能减少功耗，虽然Google官方并未正式揭露，但已表态将与Google合作的台湾工控系统商也表示，其典型功耗(Thermal Design Power, TDP)仅在1.8瓦，很明显只要现成芯片封装即可散热，几乎可不加散热片，更不需要马达风扇，便可让Edge TPU正常运作。
至于Edge TPU支援的运算格式则为int8、int16，即8位元整数、16位元整数的人工智慧模型推算，但无法进行更高位元数的整数，或者是浮点数的运算，如16位元浮点数(FP16)。
Google一发表Edge TPU即有合作伙伴与应用的揭露，如南韩乐金(LG)将用于产线制造上；另外纽西兰、澳洲、英国的Smart Parking公司(顾名思义是与智慧停车相关的方案商)也表态采用，Smart Parking不单采用Edge TPU，其后端系统也大量采用GCP服务；还有Xee公司将Edge TPU用于汽车驾驶辅助上，对影像与雷达资料进行研判，而后给予驾驶潜在危险警告，如路况变差、轮胎过度磨损等。目前所知Google将Edge TPU诉求于三个目标，即制造、零售、汽车。
**![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/ghsnibD4zwPvpwMG9PAr1SX5PabmdraKRVo0fIbZian5SOI7ribgYvxLKIc5iby6micckKr0Jl61S1jrRLichJ4h8z1A/640?wx_fmt=png)**
**Movidius与Edge TPU的瑜亮情结**
在Google尚未推出Edge TPU芯片前，其实Google已有使用前端的人工智慧加速芯片，2011年Google购并Motorola Mobility，而后于2014年将Motorola Mobility售给Lenovo，但Google仍保留下2个原属于Motorola Mobility的研究专案，而未移转给Lenovo，一是模组化手机专案Ara，另一是扩增实境技术专案Tango(此专案于2018年3月结束，改由ARCore技术接手)。
Google在2014年对外揭露Tango研究，此专案所发展的平板、手机等行动装置，即配置了Movidius公司的VPU芯片，VPU即Vision Processing Unit之意，更具体而言是视讯类型的人工智慧运算、推算加速芯片。
Movidius自身发展第一代VPU，而后在与Google合作Tango计画时则为第二代VPU，称为Myriad 2(芯片编号MA2150/MA2450，差别在于MA2150最高连接1Gbit记忆体，MA2450可至4Gbit)。不仅Google采用，包含多轴无人机大厂大疆亦用于无人机上。之后2016年英特尔(Intel)购并Movidius，接手后发展出第三代VPU，称为Myriad X(芯片编号MA2085/MA2485，MA2085封装内无记忆体，MA2485内含4Gbit记忆体)。
Movidius在被英特尔收购后，Google依然钟爱Movidius技术。2017年底Google推出AIY Vision Kit的开发套件，该套件内仍可见MA2450芯片的踪影。AIY Vision Kit是Google用来推行其影像人工智慧技术的评估套件，以树莓派电脑(Raspberry Pi, RPi)为基础提供扩充延伸的硬件配件，即可摸索与评估Google的影像人工智慧技术。
而所谓的AIY是Google自创的复合字，是以人工智慧(AI)与DIY(Do-It-Yourself自己动手做)二字叠合而成。事实上Google在推出Vision Kit的同时也有推出Voice Kit，可供摸索评估Google的语音人工智慧技术，但语音的人工智慧运算其运算负荷并不吃重，不需要专属加速芯片。
从2014年的Tango到2017年底的AIY Vision Kit，Google均以第二代Movidius芯片为主，因此理论上顺其发展，Google即便在Edge环节有其技术主张，认为可配置人工智慧硬件加速芯片，应也会属意Movidius，而非自行研发Edge TPU。
不过，最终Google提出了Edge TPU，因此不得不推测，英特尔购并Movidius后，Google可能认为后续新发展并不完全合乎期待，过往Movidius为小型独立企业时，可能提供Google高度支援，然英特尔可能对Movidius技术资产的后续延伸与新走向有不同的想法。
对此可若干比较第三代Movidius Myriad X与Edge TPU，前者支援FP16的16位元浮点数推测运算及8位元整数推算，而Edge TPU如前述仅支援8位元整数及16位元整数推算；Movidius Myriad系列仍可能顾及潜在市场机会最大化，虽未广泛支援多种AI框架，但至少支援两种现阶段主流框架，即TensorFlow与Caffe。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/ghsnibD4zwPvA9Kjv25HnZeH0v1hns7h13nfY4LejFAicQ6zT3QE4LzO0HU6H8UOLyluVecktGH7icE8icdLichJ6DQ/640?wx_fmt=png)
图5 Google在Tango专案与AIY Vision套件上均使用Movidius MA2450芯片。资料来源：Google官网
由此而论，Google可能对TensorFlow Lite寄予厚望，因而有专属设计的加速芯片Edge TPU，Edge TPU可能一直维持比Movidius Myriad系列更低规(无浮点数)、更专精(只支援TensorFlow Lite )的技术发展定位，更高阶的需求仍会选用Movidius Myriad系列，两者高低并用并行。
或者Google未来只属意自家Edge TPU，只在云端外推行Edge TPU，并一直维持低阶定位，不往更高阶发展，或Edge TPU仅是首发，未来将持续发展更高阶的Edge TPU，如此将与英特尔Movidius竞争，甚在效能规格大幅强化后而与NVIDIA Xavier竞争。
认为Edge TPU将与Movidius竞争的另一支持，在于Edge TPU也将推出USB随身碟型态的开发/运用套件，并同样使用AIY之名推展。早于Movidius未被英特尔收购前，Movidius即有提供USB随身碟型态的人工智慧评估/运用套件(图6)，英特尔收购Movidius套件产品仍持续，套件称为NCS(Neural Compute Stick)。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/ghsnibD4zwPvA9Kjv25HnZeH0v1hns7h1XQIUkHXF3KRVEAS7gnG9ATUglZxGn9YAqDT2erCzIzjd6hnDfr4ia6w/640?wx_fmt=png)
图6英特尔、Google均以PCI Express介面开发板与USB介面随身碟型加速器，来推行其人工智慧加速芯片。资料来源：英特尔、Google
另一需考量的是，虽然Google在技术布局上已逐渐同时注重软硬件的均衡与呼应，自身对于硬件发展的主导与涉入也日深，但就过往经验而言Google的硬件技术策略仍经常摇摆，甚在短期内放弃，因此Edge TPU仍可能不是长期技术策略中的一环，而是一个技术尝试，特别是Cloud TPU、Edge TPU均为加速作用的辅助处理器，为选择性使用，必要时仍可由其他芯片实现相同运算效果，或在英特尔、NVIDIA等芯片商给予更多技术发展承诺后，仍可能停止自有芯片发展路线。
其他产业推测也包含，Google Edge TPU的技术授权来自于Movidius，有可能初始使用Movidius技术与架构之后独立发展，也可能持续向英特尔Movidius取得授权，而始终与英特尔Movidius主力销售芯片保持区隔与技术落差时间。
**![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/ghsnibD4zwPvpwMG9PAr1SX5PabmdraKRVo0fIbZian5SOI7ribgYvxLKIc5iby6micckKr0Jl61S1jrRLichJ4h8z1A/640?wx_fmt=png)**
**Edge TPU后续策略、动向推测**
展望未来，Edge TPU由于将提供给系统商，估计日后揭露的技术资讯将较Cloud TPU为多，Cloud TPU由于目前只在Google机房端配置，至多提供远端租用，因此可以不揭露更多技术资讯，事实上Google对于第三代Cloud TPU所揭露的资讯，已明显少于前二代。
倘若Edge TPU推展顺遂，支持的系统伙伴日增，则可激励Google更快速发展新版Edge TPU。若推展不如预期，则Google也可能自行推出官方版的Edge闸道器、Edge装置，作为产业示范，或自始至终不投入官方版示范，直接停止Edge TPU后续发展。
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
