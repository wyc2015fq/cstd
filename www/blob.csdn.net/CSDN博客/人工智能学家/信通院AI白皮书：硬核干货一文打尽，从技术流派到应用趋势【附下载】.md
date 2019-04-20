# 信通院AI白皮书：硬核干货一文打尽，从技术流派到应用趋势【附下载】 - 人工智能学家 - CSDN博客
2018年09月09日 21:03:28[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：193
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBWYoTm3yKSWYRkF4ycVyks0trSO5Z5bb2bdSxGqK2nduZBIjVChsHaG7lN4JRHEgcPecVsdZJiaiaBQ/640?wx_fmt=jpeg)
来源：智东西
摘要：从产业发展的角度，分析AI技术现状、问题以及趋势，盘点智能语音、语义理解、计算机视觉等相关应用。
自2016年AlphaGo击败李世石之后，人工智能（AI）这个再度翻红的科技热词已经在争议声中走过了两年多。这两年里，从一键美颜、刷脸开机，到编辑快讯、演唱会抓逃犯，人工智能正在突破次元壁，落地现实。
人工智能的战略重要性不仅吸引了科技巨头和资本的疯狂投资，养活了一众创企，更是得到了各国顶层支持。
我国继去年7月发布了《新一代人工智能发展规划》，11月公布了首批国家人工智能开放创新平台名单之后，工信部本周三又公示了2018年人工智能与实体经济深度融合创新项目名单（覆盖106个项目）。
本期的智能内参，我们推荐来自中国信息通信研究院和中国人工智能产业发展联盟的人工智能发展白皮书（技术架构篇），从产业发展的角度，分析技术现状、问题以及趋势，盘点智能语音、语义理解、计算机视觉等相关应用。
整理呈现的干货：
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBVmmiaE0s8SXhWAv40NsNEctQOO5cZorf1uDx7YEI2babwUJ5Rk9XUc6OmQplr54tkSHwboSEdXmDA/640?wx_fmt=jpeg)![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBVmmiaE0s8SXhWAv40NsNEctfhUjqSv9f6l3snCic8ulDJXXyVPTEZN7icEIIaEINRSSxOUxsE3GIxvA/640?wx_fmt=jpeg)![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBVmmiaE0s8SXhWAv40NsNEctCcUDwB4K5icehtGFQl2dgfkg4cuC1qljXyG7srRoWkp5q96oqZHXmiaQ/640?wx_fmt=jpeg)
**![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/z7ZD1WagSLgFt8txemHtrKUiaFRQ3Fzr7pFR0j3pib9T80DoiaBMEzBnQicBhcziaGqvTSkT9cntnXNDibhGaHEg4AdA/640?wx_fmt=png)**
## **AI技术流派**
让机器实现人的智能，一直是人工智能学者不断追求的目标，不同学科背景或应用领域的学者，从不同角度，用不同的方法，沿着不同的途径对智能进行了探索。其中，符号主义、连接主义和行为主义是人工智能发展历史上的三大技术流派。
### **符号主义**
符号主义又称为逻辑主义 ，在人工智能早期一直占据主导地位。
该学派认为人工智能源于数学逻辑，其实质是模拟人的抽象逻辑思维，用符号描述人类的认知过程。早期的研究思路是通过基本的推断步骤寻求完全解，出现了逻辑理论家和几何定理证明器等。
上世纪 70 年代出现了大量的专家系统，结合了领域知识和逻辑推断，使得人工智能进入了工程应用。PC 机的出现以及专家系统高昂的成本，使符号学派在人工智能领域的主导地位逐渐被连接主义取代。
### **连接主义**
连接主义又称为仿生学派 ，当前占据主导地位。该学派认为人工智能源于仿生学，应以工程技术手段模拟人脑神经系统的结构和功能。
连接主义最早可追溯到 1943 年麦卡洛克和皮茨创立的脑模型，由于受理论模型、生物原型和技术条件的限制，在 20 世纪 70 年代陷入低潮。
直到 1982 年霍普菲尔特提出的 Hopfield 神经网络模型和 1986年鲁梅尔哈特等人提出的反向传播算法，使得神经网络的理论研究取得了突破。
2006 年，连接主义的领军者 Hinton 提出了深度学习算法，使神经网络的能力大大提高。2012 年，使用深度学习技术的 AlexNet模型在 ImageNet 竞赛中获得冠军。
### **行为主义**
行为主义又称为进化主义，近年来随着 AlphaGo 取得的突破而受到广泛关注。
该学派认为人工智能源于控制论，智能行为的基础是“感知—行动”的反应机制，所以智能无需知识表示，无需推断。智能只是在与环境交互作用中表现出来，需要具有不同的行为模块与环境交互，以此来产生复杂的行为。
在人工智能的发展过程中，符号主义、连接主义和行为主义等流派不仅先后在各自领域取得了成果，各学派也逐渐走向了相互借鉴和融合发展的道路。特别是在行为主义思想中引入连接主义的技术，从而诞生了深度强化学习技术，成为 AlphaGo 战胜李世石背后最重要的技术手段。
**![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/z7ZD1WagSLgFt8txemHtrKUiaFRQ3Fzr7NHvWWGfxjWEk4ybhb9ddw81errHd3N5glhaSicHibO85fKNFGvEdxJ1w/640?wx_fmt=png)**
## **深度学习一招翻红**
可以说，本轮人工智能的发展，是在大数据环境和计算能力大幅提升的基础上，由深度学习带动的。
深度学习全称深度神经网络，本质上是多层次的人工神经网络算法，即从结构上模拟人脑的运行机制，从最基本的单元上模拟了人类大脑的运行机制。目前，深度学习已经在计算机视觉、语音识别、自然语言理解等领域取得了突破（详情参考第209期智东西内参）。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/z7ZD1WagSLiaicBynIsXPUhicgQlYo8Y6gCnnr8UH2KLzCn0wN3kLVeovPfnxWc9ePUElnEDvgic4LRFzT18WIn4Eg/640?wx_fmt=png)▲深度学习已在多领域实现突破
深度学习分为训练(training)和推断(inference)两个环节。训练需要海量数据输入，训练出一个复杂的深度神经网络模型。推断指利用训练好的模型，使用待判断的数据去“推断”得出各种结论。
大数据时代的到来，图形处理器（GPU）等各种更加强大的计算设备的发展，使得深度学习可以充分利用海量数据（标注数据、弱标注数据或无标注数据），自动地学习到抽象的知识表达，即把原始数据浓缩成某种知识。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/z7ZD1WagSLiaicBynIsXPUhicgQlYo8Y6gCjyl631qePub3wT6fbG2AUdQVWAkKUSibY8Dv1G2EMpCK0hADHzG6xGA/640?wx_fmt=png)
▲基于深度学习的人工智能技术架构
当前，基于深度学习的人工智能算法主要依托计算机技术体系架构实现，深度学习算法通过封装至软件框架的方式供开发者使用。
软件框架是整个技术体系的核心，实现对人工智能算法的封装，数据的调用以及计算资源的调度使用 。为提升算法实现的效率，其编译器及底层硬件技术也进行了功能优化。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBVmmiaE0s8SXhWAv40NsNEctqFehv8m7ME3ib2Be3gOFyKFw1F6dXc2EAySq6A1aYYbt5rKOvrCJp3A/640?wx_fmt=png)
▲深度学习技术体系概述
人工智能算法的设计逻辑可以从“学什么”（表征所需完成任务的函数模型）、“怎么学”（通过不断缩小函数模型结果与真实结果误差来达到学习目的）和“做什么”（回归、分类和聚类三类基本任务）三个维度进行概括。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBVmmiaE0s8SXhWAv40NsNEct361AeEYeo0JS7WLZVsjQwtBWFRJf52LlLxG8VbnTDHic3C6K5NeTgKQ/640?wx_fmt=png)
▲人工智能主要算法分类
近年来，随着AI算法在多领域的突破，相关算法的理论性研究持续加强，新算法如胶囊网络、生成对抗网络、迁移学习等，也被不断提出。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBVmmiaE0s8SXhWAv40NsNEctC946Rh4rTCDyqpbq5r0P32iaWo1P2ib73WOjt865FhCZaMMkq9CNVEDg/640?wx_fmt=jpeg)▲人工智能新算法
下文将概述围绕深度学习的五方面技术现状和发展趋势。
### **大佬开源造生态**
如前所述，软件框架是整个技术体系的核心，是算法的工程实现。当前，人工智能基础性算法已经较为成熟，各大厂商纷纷发力建设算法模型工具库，并将其封装为软件框架，供开发者使用。
企业的软件框架实现有闭源和开源两种形式：苹果公司等少数企业选择闭源方式开发软件框架，目的是打造技术壁垒；目前业内巨头基本都是基于自身技术体系的训练及推断软件框架，将开源深度学习软件框架作为打造开发及使用生态核心的核心。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBVmmiaE0s8SXhWAv40NsNEctibOuYkDc2Y8ShYT2icPyicD5nj4fFZEqcXcrXgiakR5ozzn51iaicVMnJIEA/640?wx_fmt=jpeg)▲人工智能开源平台对比（援引招商证券）
总体来说开源软件框架在模型库建设及调用功能方面具有相当共性，但同时又各具特点。业界目前主要有深度学习训练软件框架（TensorFlow、MXNet等）和推断软件框架（Caffe2go等）两大类别。
当前开源软件框架的技术发展呈现出以下几方面的特点：
1、谷歌与其他公司间持续竞争。巨头公司在技术上将积极探寻包括模型互换，模型迁移等技术联合，以对抗谷歌公司。例如脸书（Facebook）和微软已经合作开发了一个可互换的人工智能软件框架解决方案。
2、开源软件框架在向统一和标准化方向发展。随着人工智能应用的爆发，开发人员在不同平台上创建模型及部署模型的需求愈发强烈，在各类软件框架间的模型迁移互换技术研发已经成为重点。
3、更高级的 API 逐渐占据主导地位。以 Keras 为例，它是建立在TensorFlow、Theano、CNTK、MXNet 和 Gluon 上运行的高级开源神经网络库，以其高级 API 易用性而得到了广泛的使用。
4、模型的集群并发计算成为业界研究热点。当前人工智能网络对于单计算节点的算力要求过高，但当前主流开源软件框架对于模型分割进行计算并没有实现，而这个问题也将随着应用场景的不断丰富而不断引起重视，成为开源软件框架下一个核心竞争点。
### **编译器解决适配问题**
在实际工程应用中，人工智能算法可选择多种软件框架实现，训练和开发人工智能模型也可有多种硬件选项，这就开发者带来了不小的挑战。
原因一是可移植性问题，各个软件框架的底层实现技术不同，导致在不同软件框架下开发的模型之间相互转换存在困难；二是适应性问题，软件框架开发者和计算芯片厂商需要确保软件框架和底层计算芯片之间良好的适配性。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBVmmiaE0s8SXhWAv40NsNEctLGwaanbp6dkGSBsMThjgMAEK8iaVmg3QylO6fF35rEoZJvq3OrsWfDg/640?wx_fmt=png)
▲编译器解决软硬件适配问题
解决以上两个挑战的关键技术之一就是深度神经网络模型编译器，它在传统编译器功能基础上，通过扩充面向深度学习网络模型计算的专属功能，以解决深度学习模型部署到多种设备时可能存在的适应性和可移植性问题。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBVmmiaE0s8SXhWAv40NsNEctw0Bu9JRPYBDhVXD1rEWtcicGMcufrWKgZiaKzfyJmmaKK0VOR4z5h0pQ/640?wx_fmt=png)
▲深度学习的两大表示规范
深度学习网络模型的表示规范分为两大阵营。
第一阵营是 Open Neural Network Exchange（ONNX，开放神经网络交换），是一个用于表示深度学习模型的标准，可使模型在不同软件框架之间进行转移。ONNX 由微软和 Facebook 联合发布，该系统支持的软件框架目前主要包括 Caffe2，PyTorch，Cognitive Toolkit 和 MXNet，而谷歌的TensorFlow 并没有被包含在内。
第二阵营是 Neural Network Exchange Format（NNEF，神经网络交换格式），是由 Khronos Group 主导的跨厂商神经网络文件格式，计划支持包括 Torch, Caffe, TensorFlow, 等几乎所有人工智能软件框架的模型格式转换，目前已经有 30 多家计算芯片企业参与其中。
### **芯片提供算力保障**
现有深度神经网络需要用更短的时间、更低功耗完成计算，这就给深度学习计算芯片提出了更高的要求：一是计算芯片和存储间海量数据通信需求，包括缓存（Cache）和片上存储（Memory）要大，计算单元和存储之间的数据交互带宽要大；二是专用计算能力的提升，解决对卷积、残差网络、全连接等计算类型的大量计算需求，同时降低功耗。
总的来说，AI 计算芯片的发展过程可以总结为一直在围绕如何有效解决存储与计算单元的提升这两个问题而展开，成本问题则作为一个舵手控制着最终的航向。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBVmmiaE0s8SXhWAv40NsNEcttFG8yYMiawOVrb7vSnxcmYAg4qynr0vRtKt1OrE68cR7r6awEH7GS9g/640?wx_fmt=png)
▲AI 计算芯片分类
在深度学习训练环节，除了使用 CPU 或 GPU （深度学习训练的首选）进行运算外，现场可编程门阵列（FPGA）以及专用集成电路（ASIC）也发挥了重大作用；而用于终端推断的计算芯片主要以 ASIC 为主（详情可参考第208期智东西内参）。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBVmmiaE0s8SXhWAv40NsNEct1yrc1lkQCCdyCouRqnKrVRNuicwcl4ic3xmMNNsAnbShnnaxGroNpeDA/640?wx_fmt=jpeg)▲典型AI芯片商一览
### **数据：人工智能的基础资源**
基于深度学习的人工智能技术，核心在于通过计算找寻数据中的规律，运用该规律对具体任务进行预测和决断。
源数据需要进行采集、标注等处理后才能够使用，标注的数据形成相应数据集。业务类型主要包括数据采集、数据处理、数据存储以及数据交易等环节。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBVmmiaE0s8SXhWAv40NsNEctXv40P02icnpQ9WACAHk5ibjVEq9f7OfEicaSopAL1B2ZhEiawy5e2R2CiaA/640?wx_fmt=png)
▲人工智能数据集的参与主体
当前，人工智能基础数据类型主要包括语音语言类（包括声音、文字、语言学规则）、图像识别类（包括自然物体、自然环境、人造物体、生物特征等）以及视频识别类三个大类，从世界范围来看，数据服务商总部主要分布在美国、欧洲等发达国家。但其数据处理人员则大多数分布在第三世界国家；我国语音、图像类资源企业机构正处于快速发展阶段，为产业发展增添了动力。
### 计算和服务平台的快速崛起
深度学习使用GPU计算具有优异表现，催生了各类GPU服务器，带动了 GPU 服务器的快速发展；同时，也带动了以服务的形式提供人工智能所需要的能力，如深度学习计算类的计算平台，以及语音识别，人脸识别等服务，这也成为人工智能企业打造生态的重要抓手。
一方面，服务器厂商相继推出了专为 AI 而设计的、搭载 GPU 的， 应用于视频编解码、深度学习、科学计算等多种场景的服务器，为 AI 云场景对弹性配置能力予以优化，以增强 PCI-E 拓扑和数量配比的弹性，增加适配多种软件框架的运算需求，支持 AI 模型的线下训练和线上推理两类场景。
另一方面，为了解决企业自行搭建 AI 能力时遇到的资金、技术和运维管理等方面困难，人工智能企业纷纷以平台类服务和软件 API 形式的服务等形式，提供 AI 所需要的计算资源、平台资源以及基础应用能力。
**![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/z7ZD1WagSLgFt8txemHtrKUiaFRQ3Fzr7w2M9yjpArMeUKpITwBZXMbI4Qw6icuooFF0Bkq9ktyQzcdHf9hwcgxg/640?wx_fmt=png)**
## **科幻落地 未来已来**
目前随着深度学习算法工程化实现效率的提升和成本的逐渐降低，一些基础应用技术逐渐成熟，如智能语音，自然语言处理和计算机视觉等，并形成相应的产业化能力和各种成熟的商业化落地。同时，业界也开始探索深度学习在艺术创作、路径优化、生物信息学相关技术中的实现与应用，并已经取得了瞩目的成果。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBVmmiaE0s8SXhWAv40NsNEctGo9J9awNepWsF9TetWOO2JwtYxAMpUus6S2dxogZiaP4cGfC34NXVtQ/640?wx_fmt=png)
▲基础应用架构图
### **智能语音**
按机器在其中所发挥作用的不同，分为语音合成技术、语音识别技术、语音评测技术等。智能语音技术会成为未来人机交互的新方式，将从多个应用形态成为未来人机交互的主要方式。
已经面世的智能语音技术应用有智能音箱（智能家庭设备的入口）、个人智能语音助手（个性化应用整合），以及以 API 形式提供的智能语音服务，覆盖了智能客服、教育/口语评测、医疗/电子病历、金融/业务办理、安防、法律、个人手机、自动驾驶及辅助驾驶、传统家电、智能家居等领域的应用。
### **计算机视觉**
一般来讲，计算机视觉主要分为图像分类、目标检测、目标跟踪和图像分割四大基本任务。
目前，计算机视觉识别这一人工智能基础应用技术部分已达商业化应用水平，被用于身份识别、医学辅助诊断、自动驾驶等场景。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBVmmiaE0s8SXhWAv40NsNEcticTBSYJ0FnBooAkMziaY7lsNXHffI9IrwjFZDVT4j1zmZDBPmZyEQWQw/640?wx_fmt=png)
▲计算机视觉的三大热点
在政策引导、技术创新、资本追逐以及消费需求的驱动下，基于深度学习的计算机视觉应用不断落地成熟，并出现了三大热点应用方向：人脸识别、视频结构化、姿态识别。
### **自然语言处理**
自然语言处理（NLP）是研究计算机处理人类语言的一门技术，是机器理解并解释人类写作与说话方式的能力，也是人工智能最初发展的切入点和目前大家关注的焦点。
自然语言处理的主要步骤包括分词、词法分析、语法分析、语义分析等，其应用方向主要有文本分类和聚类、信息检索和过滤、信息抽取、问答系统、机器翻译等方向。
**![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/z7ZD1WagSLgFt8txemHtrKUiaFRQ3Fzr7LzPRgPuKzUXaU1wEsibPVq4fADWG94UP8o92sdWwlcPnbqyEcQozTWQ/640?wx_fmt=png)**
## **展望AI的五大趋势**
人工智能这座矿还远没有挖完，还有一箩筐的问题等待解决。
一方面，深度学习算法模型存在可靠性及不可解释性问题，因此存在产生不可控结果的隐患；另一方面，当前的数据环境不够完善，存在着流通不畅、数据质量良莠不齐和关键数据集缺失等问题。
此外，推断软件框架质量参差不齐，制约了业务开展，编译器缺乏统一的中间表示层标准，云、侧端AI芯片的市场格局有待形成。
基于技术和产业的发展现状，信通院总结出了以下五大趋势：
**1、迁移学习的研究及应用将成为重要方向。**
迁移学习由于侧重对深度学习中知识迁移、参数迁移等技术的研究，能够有效提升深度学习模型复用性，同时对于深度学习模型解释也提供了一种方法，能够针对深度学习算法模型可靠性及不可解释性问题提供理论工具。
**2、深度学习训练软件框架将逐渐趋同，开源推断软件框架将迎来发展黄金期。**
随着人工智能应用在生产生活中的不断深入融合，对于推断软件框架功能及性能的需求将逐渐爆发，催生大量相关工具及开源推断软件框架，降低人工智能应用部署门槛。
**3、中间表示层之争将愈演愈烈。**
以计算模型为核心的深度学习应用，由于跨软件框架体系开发及部署需要投入大量资源，因此模型底层表示的统一将是业界的亟需，未来中间表示层将成为相关企业的重点。
**4、AI 计算芯片朝云侧和终端侧方向发展。**
从云侧计算芯片来看，目前 GPU 占据主导市场，以 TPU 为代表的 ASIC 只用在巨头的闭环生态，未来 GPU、TPU 等计算芯片将成为支撑人工智能运算的主力器件，既存在竞争又长期共存，一定程度可相互配合；FPGA 有望在数据中心中以 CPU+FPGA 形式作为有效补充。从终端侧计算芯片来看，这类芯片将面向功耗、延时、算力、特定模型、使用场景等特定需求，朝着不同发展。
**5、行业巨头以服务平台为核心打造生态链。**
对于国内外的云服务和人工智能巨头，如亚马逊、微软，阿里云、腾讯云、科大讯飞、旷视科技等企业，将围绕各自应用，与设备商、系统集成商、独立软件开发商等联合，为政府，企业等垂直领域提供一站式服务，共同打造基于服务平台的生态系统。
小编认为，上一波的企业数字化为深度学习的产业化落地提供了初步引导，随后，人工智能相关的大数据、云服务、芯片、算法产业和市场格局逐渐成熟。在智能语音、计算机视觉、自然语言理解等细分技术的发展下，智能医疗、智能金融、智能汽车等“AI+”项目掀起了一波创投热。无疑，AI泡沫确实存在，但AI曙光势必降临。
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
