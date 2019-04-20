# 深度解读：深度学习在IoT大数据和流分析中的应用 - 人工智能学家 - CSDN博客
2018年02月28日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：4128
![640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/fYsJGst9n2SNQYGuBAPTQL6HGKMDFKamLuibibvPYicsoaRQiaAXibeoa7uPiclJS3gWMK85Yy12alUDQZAR7vUJOtqw/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1)
*来源：网络大数据（ID:raincent_com）*
*摘要：这篇论文对于使用深度学习来改进IoT领域的数据分析和学习方法进行了详细的综述。*
在物联网时代，大量的感知器每天都在收集并产生着涉及各个领域的数据。
由于商业和生活质量提升方面的诉求，应用物联网(IoT)技术对大数据流进行分析是十分有价值的研究方向。
这篇论文对于使用深度学习来改进IoT领域的数据分析和学习方法进行了详细的综述。从机器学习视角，作者将处理IoT数据的方法分为IoT大数据分析和IoT流数据分析。论文对目前不同的深度学习方法进行了总结，并详细讨论了使用深度学习方法对IoT数据进行分析的优势，以及未来面临的挑战。
**论文贡献**
为了更好的在IoT领域内应用深度学习方法，作者分析了IoT数据的关键特征和主要问题。
作者对于目前最先进的深度学习方法及其在物联网领域对于大数据和流数据的应用进行了详细的总结。
作者对于目前应用了深度学习方法的大量IoT应用进行了介绍，并且对不同类型的深度神经网络在各种IoT领域的应用进行了概括和对比。
强调了深度学习与物联网应用成功结合所面临的挑战和未来的研究方向。
**论文结构**
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/fYsJGst9n2SNQYGuBAPTQL6HGKMDFKamib1ucbvSdCAchBeKmS56mHv2ib6By5wichU04sCYMRh8OF1xTr0l8woAw/640?wx_fmt=jpeg)
**物联网数据特征及分析要求**
> 
IoT快速流数据
目前流数据分析都是基于数据并行计算或增量处理的框架，尽管这些技术减少了从流数据分析框架返回响应的时间延迟，对于IoT应用的严格时间要求，它们并不是最佳方案。
IoT需要在数据源附近的平台(甚至是IoT设备自身)上进行快速流数据分析，以达到实时或近实时性的要求，传统的流数据分析方法则面临着计算、存储以及数据源能量方面的局限和挑战。
> 
IoT大数据
IoT大数据具有“6V”特点：
容量(Volume)： 数据量是将数据集视为大数据、或传统的大规模/超大数据的一个决定性因素，使用物联网设备产生的数据量比以前要多得多，明显符合这一特点。
速度(Velocity)： 物联网大数据产生和处理速率要足够高，以支持实时大数据的可用性。鉴于这种高数据率，也证明了需要先进的工具和技术分析才能有效地运作。
多样性(Variety)： 一般来说，大数据有不同的形式和类型。这可能包括结构化的、半结构化的和非结构化的数据。各种各样的数据类型可以通过物联网产生，如文本、音频、视频、传感器数据等等。
真实性(Veracity)： 真实性是指质量，一致性，和数据的可信性，有真实性的数据才能进行准确的分析。这一点对于物联网来说尤其重要，特别是那些群体感知数据。
易变性(Variability)： 这个属性是指数据流的速率不同。由于物联网应用的性质，不同的数据生成组件可能会有不一致的数据流。此外，在特定时间，一个数据源的数据加载速率可能不同。例如，利用物联网传感器的停车服务应用在高峰期的数据加载会达到峰值。
价值(Value)： 价值是指大数据转化成为有用的信息和内容，为组织带来竞争优势。数据的价值的高度不仅仅取决于对数据的处理过程或服务，还取决于对待数据的方式。
数据流处理的主要障碍是缺少能部署在系统边缘，甚至是IoT设备上的框架或算法。当采用深度学习方法时，也要折衷考虑运行在系统边缘的网络的深度和性能。
**深度学习**
与其他传统机器学习方法相比，深度学习结构在近几年受到越来越广泛的关注。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/fYsJGst9n2SNQYGuBAPTQL6HGKMDFKamP77nepURvOmaBEUUErytRWeRyYYVH9Uiarfk0sYHRWhXx7fAZb8fQrg/640?wx_fmt=jpeg)
*Google Trend显示近几年对深度学习的关注呈上升趋势*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/fYsJGst9n2SNQYGuBAPTQL6HGKMDFKam2bt74HjddAAueh6ibQN4smsEqrDL4BtdQOTaHNsgKUEHgRWL0t6fdtw/640?wx_fmt=jpeg)
*深度学习整体训练机制*
> 
结构
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/fYsJGst9n2SNQYGuBAPTQL6HGKMDFKamwoa6v0xmdMeD9Pa1xb9gb6AP0nU3icE1gYVLGakvdZGAYBXccDic6FMQ/640?wx_fmt=jpeg)
*深度学习模型总结*
1)卷积神经网络(Convolutional Neural Networks，CNN)
CNN的核心结构是卷积层，有一系列可学习的参数，称作滤波器。训练过程中，滤波器在全图按照卷积顺序进行移动，计算输入和滤波器的乘积，得到该滤波器的特征图。CNN的另一个结构是池化层，将输入划分成不重叠的区域，然后用每个区域的最大值作为输出。CNN的最后一个结构是ReLU激活函数层，既可以缩短训练时间，也能避免影响网络的泛化能力。
CNN和DNN的主要区别在于CNN具有局部相连、权值共享的特性，因此在视觉任务中具有独特的优越性，并且降低了网络的复杂性。
2)循环神经网络(Recurrent Neural Networks，RNN)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/fYsJGst9n2SNQYGuBAPTQL6HGKMDFKamSm4wEwg9LFbY0Kux4ia1xEHticgBic96HP6Aib36dBAibftk69ibGLtcF7tA/640?wx_fmt=jpeg)
*循环神经网络结构图*
RNN主要适用于输入为序列(例如语音和文本)或时间序列的数据(传感器数据)。RNN的输入既包括当前样例，也包括之前观察的样例。也就是说，时间为t-1时RNN的输出会影响时间为t的输出。RNN的每个神经元都有一个反馈环，将当前的输出作为下一步的输入。该结构可以解释为RNN的每个神经元都有一个内部存储，保留了用之前输入进行计算得到的信息。
3)长短时记忆(Long Short Term Memory，LSTM)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/fYsJGst9n2SNQYGuBAPTQL6HGKMDFKampmCAEictHw1J5ZSibUVPQ9cGbCLAByibtpnj4n98Xt0QsAV7MRaTXvoKA/640?wx_fmt=jpeg)
*LSTM记忆单元结构*
LSTM是RNN的一种扩展。LSTM中，每个神经元除了有反馈环这一储存信息的机制，还有用于控制神经元信息通过的“遗忘门”、“输入层门”及“输出层门”，防止不相关的信息造成的扰动。
4)自动编码器(Autoencoders，AE)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/fYsJGst9n2SNQYGuBAPTQL6HGKMDFKamMRp9sRKvmcItS0tsmu5gmr09PVtplLjIiaR2YVibEaHTI3TAnWuWS4OA/640?wx_fmt=jpeg)
*自编码器网络结构*
AE的输入层和输出层由一个或多个隐层相连接，其输入和输出神经元数量相同。该网络的目标是通过用最简单的方式将输入变换到输出，以重建输入信息。
5)变分自动编码器(Variational Autoencoders，VAE)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/fYsJGst9n2SNQYGuBAPTQL6HGKMDFKamu00S0wvGS7dLbwKicNDuPich3GxH0icugeicMYaSLMSEFphEIXFickXsNhQ/640?wx_fmt=jpeg)
*变分自动编码器结构*
VAE对数据结构的假设并不强，是较为流行的生成模型框架。它很适用于IoT解决方案，因为IoT数据呈现的多样性，以及标记数据的缺失。模型由两个子网络组成：一个生成样例，一个进行假设推理。
6)生成对抗网络(Generative Adversarial Networks，GAN)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/fYsJGst9n2SNQYGuBAPTQL6HGKMDFKamCff7U6szPxZ9APCukMe0Bp3kZc1S2KnKjPmRQFHLN9eAWbs9icUfGgA/640?wx_fmt=jpeg)
*生成对抗网络概念图*
GAN由两个神经网络组成，一个生成网络，一个判别网络，共同工作来产生合成的、高质量数据。生成器根据数据在训练数据集中的分布生成新数据，判别器学习判别真实数据和生成器生成的假数据。GAN的目标函数是基于极大极小博弈的，一个网络要最大化目标函数，而另一个要最小化目标函数。
7)受限玻尔兹曼机(Restricted Boltzmann Machine，RBM)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/fYsJGst9n2SNQYGuBAPTQL6HGKMDFKamFaUFjibYcxB3SoEia0xNbp5cIaaDqIoTl7bPfbVrRr4BxSf5Yic6hmEIg/640?wx_fmt=jpeg)
*受限玻尔兹曼机结构*
RBM是一种随机神经网络，由两层组成，一层是包含输入的可见层，一层是含有隐变量的隐藏层。RBM中的限制是指同一层的任意两个神经元互不相连。除此之外，偏置单元与所有的可见层和隐藏层单元都相连。
8)深度信念网络(Deep Belief Network，DBN)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/fYsJGst9n2SNQYGuBAPTQL6HGKMDFKamPXEEics5XjJvtiaXf6cH5PM6HX6fXngP1Q6pMISHC60ReTWxibGPu74iaQ/640?wx_fmt=jpeg)
*深度信念网络结构图*
虚线表示特征提取通道，实现表示生成通道。DBN是一种生成神经网络，由一个可见层可几个隐层组成。可以提取训练数据的多层表示，并且对输入数据进行重构。DBN的训练过程是逐层训练，将每一层视作一个RBM，在前一层的基础上进行训练。这样的机制使DBN成为深度学习中有效且快速的网络之一。
9)阶梯网络(Ladder Network)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/fYsJGst9n2SNQYGuBAPTQL6HGKMDFKamRnicM5S8gex6ianwQPhbAtRyLf7kYf7XsYcay0JNScWsJzibOREEorxbA/640?wx_fmt=jpeg)
*两层阶梯网络*
阶梯网络在无监督和半监督学习任务中达到了先进的水平。阶梯网络由两个编码器和一个解码器组成。编码器作为网络的有监督部分，解码器进行无监督学习。训练目标是最小化有监督部分和无监督网络的损失和。
> 
快速实时深度学习结构
使用深度学习模型对数据流进行快速实时的处理仍在起步阶段。
早期工作：
【1】是对超限学习机（Extreme learning machine，ELM）的扩展——OS-ELM，将一个实时序列学习算法应用到单隐层的前馈神经网络。
【2】Ren等人提出的Faster-RCNN在图片中的目标检测中达到了接近实时的速度。他们的目标检测框架的运行时间为5-17fps。然而对于图像处理任务，真正的实时效果需要系统的处理和分析时间达到30fps或更高。
【3】Redmon等人提出了YOLO，将目标检测的速度提高到45fps，以及更小版本的YOLO，速度更是达到了155fps，已经适用于智能相机。
> 
深度学习与其他方法结合
1）深度增强学习（Deep Reinforcement Learning）
深度增强学习是将增强学习和深度神经网络相结合的产物。其目标是创建能自主学习的个体（agent），通过建立成功的交互过程以获得长期的最大正反馈（reward）。
当环境（environment）可由大量状态表示时，传统的增强学习方法稍显不足，而深度神经网络则弥补了这一点。在IoT领域，【4】使用深度增强学习实现了半监督条件下智能校园环境中的定位。
2）迁移学习与深度模型（Transfer Learning with Deep Models）
迁移学习主要应用在域适应和多任务学习的领域。迁移学习对于许多难以收集训练数据的IoT应用来说都是一个可用的解决方案。
例如训练一个通过智能手机的低功耗蓝牙和Wifi fingerpringting的定位系统，同一时间，在同一地点的RSSI值（Received Signal Strength Indication接收的信号强度指示）对于不同的平台来说可能不同。
如果我们对一个平台训练了一个模型，该模型可以迁移到其他平台，而不需要对新平台再收集训练数据。
3）深度学习与在线学习算法
由于IoT的应用产生的数据流会上传到云平台来分析，在线机器学习算法的角色变得越来越重要，因为训练模型需要随数据的增加而更新。
> 
框架
近几年，随着深度学习在各个领域的应用热潮，各种深度学习框架也应运而生。
Tensorflow： Tensorflow是机器学习系统的开源库，可以使用多种深度神经网络。
Tensorflow使用图表示来建立神经网络模型。开发人员也在使用TensorBoard，能可视化神经网络模型，并且观测学习过程，包括参数更新。
Torch： Torch是一个机器学习开源框架，包含大量深度学习算法，可用于深度神经网络模型的简单开发。它基于Lua语言开发，是训练深度学习算法的轻量级快速框架。支持在CPU和GPU上开发机器学习模型，并且提供了训练深度神经网络的并行计算库。
Caffe： Caffe是一个深度学习算法和参考模型集的开源框架。基于C++，支持CUDA进行GPU运算，并且提供Python和Matlab接口。Caffe通过配置文件定义模型，而不需要在源代码中定义参数，将模型表示和实现分开。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/fYsJGst9n2SNQYGuBAPTQL6HGKMDFKamtuk3dAeTOHmw9w623x3XKgxzI64tb2WRJFZ0DD6vNv2WCcTrEjjn3g/640?wx_fmt=jpeg)*深度学习框架对比*
**深度学习在IoT领域的应用**
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/fYsJGst9n2SNQYGuBAPTQL6HGKMDFKamTyI1uIkrNoOUvxQsGUBOM2mnwOpeXCYOIangQrqx2RtGLyNtFGNUUg/640?wx_fmt=jpeg)
IoT应用和基础服务。
> 
基础服务
1）图像识别：
IoT的一大部分应用场景中，输入深度学习的数据是图片或视频。每天，每个人都在用手机的高清摄像头拍摄者图片和视频，除此之外，家居、校园或工厂也在使用智能摄像头。所以，图像识别、分类、目标检测是这类设备的基础应用。
2）语音识别
随着智能手机和可穿戴设备的普及，语音识别也成了人们和自己的设备互动的一种自然而方便的方式。Price等人【5】搭建了一个专用的低功耗深度学习芯片，用于自动语音识别。这种特制芯片的能量消耗要比目前手机上运行的语音识别工具的能量消耗低100倍。
3）室内定位
室内定位在IoT领域有许多应用，例如智能家居、智能校园、或智能医院。例如DeepFi系统，在线下训练阶段，通过深度学习用之前储存的WiFi通道状态信息数据来训练网络权重，在线上定位阶段通过fingerpringting来测定用户位置。
4）生理和心理状态检测
IoT与深度学习的结合也应用在了检测各种生理或心理状态中，例如姿态、活动和情绪。许多IoT应用都在交付的服务中整合了人体姿态估计或活动识别模块，例如智能家居、智能汽车、XBox、健康、运动等等。
5）安全和隐私
安全和隐私是所有IoT领域应用所关注的一个重要问题。事实上，系统功能的有效性取决于是否能保护机器学习工具和处理过程不受攻击。虚假数据注入（False Data Injection，FDI）是数据驱动系统的一种常见攻击类型。
He等人【6】提出用条件DBN从历史数据中提取FDI特征，然后利用这些特征进行实时攻击检测。作为物联网数据和应用程序的一大贡献者，智能手机也面临着黑客攻击的威胁。
Yuan等人【7】提出用深度学习框架来鉴别安卓应用中的恶意软件，准确率达到了96.5%。深度机器学习方法的安全性和隐私保护是能否在IoT领域应用的最重要因素。
Shokri等人【8】提出了一种解决分布式学习的深度学习模型隐私保护问题的方法。
> 
应用
1）智能家居：
智能家居的概念涉及广泛的基于IoT的应用，它有助于提高家庭的能源使用和效率，以及居住者的便利性、生产力和生活质量。如今，家电可以与互联网连接，提供智能服务。例如微软和 Liebherr的一个合作项目，对从冰箱内收集的信息应用了Cortana 深度学习。这些分析和预测可以帮助家庭更好地控制他们的家庭用品和开支，并结合其他外部数据，可用于监测和预测健康趋势。
2）智慧城市：
智能城市服务跨越多个物联网领域，如交通、能源、农业等。智慧城市的一个重要问题是预测群体移动模式，Song等人【9】开发了基于深度神经网络模型的系统，在城市级别实现了这一目标。Liang等人【10】基于RNN模型搭建了实时群体密度预测系统，利用移动手机用户的通信数据对交通站的群体密度进行预测。废物管理和垃圾分类也是智慧城市的一个相关任务，可以通过基于视觉分类任务的CNN模型来实现自动化。Amato等人【11】基于智能相机和深度CNN开发了检测停车场的使用中和空闲车位的系统。
3）能源：
消费者与智能电网之间的双向通信是IoT大数据的来源。能源供应商希望学习当地的能源消费模式、预测需求，并根据实时分析做出适当的决定。在智能电网方面，从太阳能、风能或其他类型的自然可持续能源中预测电力是一个活跃的研究领域，深度学习在这一领域的许多应用中越来越多地被使用。
4）智能交通系统：
来自智能交通系统（ITS）的数据是大数据的另一个数据源。Ma等人【12】采用RBM和RNN结构设计了一个交通网络分析系统，模型输入是参与该系统的出租车GPS数据。该系统通过一小时内的累积数据预测交通拥堵的准确率高达88%。ITS也带动了交通标志检测和识别的发展，这一技术在自动驾驶、辅助驾驶系统中都有很重要的应用。除此之外，许多初创公司应用深度学习来完善自动驾驶汽车系统的检测行人、交通标志、路障等任务。
5）医疗和健康：
IoT结合深度学习也在为个人和组织提供医疗和健康方案中得到应用。例如，开发基于移动应用程序的精确测量饮食摄入量的解决方案，可以帮助提升个人健康和幸福感。Liu等人【13】采用CNN开发了识别食物图片和相关信息的系统。用深度学习对医学图片进行分类和分析是医疗领域的研究热点。Pereira等人【14】通过CNN识别手写图片来鉴定早期帕金森症。除此之外，深度学习与IoT的结合在声音异常检测、乳腺血管疾病检测中也得到了应用
6）农业：
生产健康作物和发展有效的种植方式是健康社会和可持续环境的要求。使用深度神经网络进行植物病害识别是一个可行的解决方案。深度学习也被用于遥感，进行土地和作物的检测与分类。研究显示，使用CNN进行作物识别准确率达到了85%，相比于MLP或随机森林有很大提高。自动耕作中的预测和检测任务也应用了深度学习。
7）教育：
IoT和深度学习的结合有助于提高教育系统的效率。移动设备可以收集学生的数据，深度分析方法可以用来预测和解释学生的进步和成就。增强现实技术结合可穿戴设备和移动设备也是深度学习在这一领域的潜在应用，激发学生的兴趣，让教育学习方法更有效。此外，深度学习可以用于个性化推荐模块，向教育者推荐更多相关内容。利用深度学习对大型开放式网络课程数据（MOOC）进行分析，可以帮助学生更好的学习。除此之外，利用CNN监测教室占用率是深度学习在教育方面的另一个应用。
8）工业：
对于工业部门来说，IoT和网络物理系统（CPS）是推动制造技术迈向智能制造（工业4.0）的核心要素。工业中的广泛应用均可以受益于深度学习模型的引入。通过将装配线中生产车辆的图像及其注释都输入深度学习系统，可以利用AlexNet、GoogLeNet等网络实现视觉检测。
9）政府：
许多涉及市政的各种任务需要精确的分析和预测。【15】利用美国地质调查局网络的历史数据训练LSTM网络，可进行地震预测。【16】利用极端气候的图片训练CNN，进行极端气候事件探测。此外，城市的基础设施，如道路、供水管道等的损害检测，是IoT和深度学习可以为政府提供便利的另一个领域。
10）运动和娱乐：
运动分析近年来发展迅速，为团队或运动员带来了竞争优势。【17】提出了深度学习方法打造智能篮球场。【18】采用RNN识别NBA比赛中的球员违规。【19】结合了可穿戴设备传感数据和CNN进行排球运动员活动识别。【20】采用层级结构的LSTM模型研究排球队的整体活动。
11）零售：
随着移动设备的普及，网上购物的人数大大增加了。最近出现了通过视觉搜索技术向产品图像检索的转变。CNN一直用于服装和时尚市场的视觉搜索，帮助你在网店中找到在电影中看到的或在街上看到的商品。IoT结合深度学习可以搭建视觉购物辅助系统，包括智能眼镜、手套和购物车，目的是帮助视障人士购物。此外，智能购物车的开发可以实现实时自结账的功能。
> 
IoT设备上的深度学习
深度学习在语音和视频方面的成功为IoT的基础服务打下了良好的基础，如何将它们的模型和方法部署在资源受限的设备上成了IoT领域的一个重要研究方向。到目前为止，深度学习方法难以应用于IoT和资源受限设备，因为它们需要大量的资源来运行，如处理器、电池能量和存储器。
幸运的是，近期研究显示，深度神经网络的许多参数是冗余的，有时也不需要大量的隐层。有效的去除这些参数或层可以减少网络的复杂度，同时对输出不会有太大的影响。
> 
方法和技术
1）网络压缩
在资源受限设备上应用深度神经网络的方法之一是网络压缩，将密集的网络转化为一个稀疏的网络。主要局限性在于，它不足以支持所有类型的网络。它只适用于具有这种稀疏性的特定网络模型。另外，修剪多余的和不重要的参数或神经元，是在资源受限的设备上运行深度神经网络的另一个重要途径。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/fYsJGst9n2SNQYGuBAPTQL6HGKMDFKamDF6w8EMpYQ5UO2uBn3zH1a5r7u26FrETynbQThGXzYh1ArN78vjFRA/640?wx_fmt=jpeg)
*深度神经网络剪枝整体概念图*
2）近似计算：
近似计算是实现在IoT设备上部署机器学习工具的另一种方法，并有助于主机设备的节能。在许多IoT应用中，机器学习的输出不一定是精确的，而是在可接受的范围内提供所需的质量。实际上，将深度学习模型与近似计算相结合，可以为资源受限设备提供更有效的深度学习模型。
3）加速器：
设计特定的硬件和电路来优化IoT设备中深度学习模型的能量效率和内存占用是另一个活跃的研究方向。目前已有工作为DNN和CNN设计加速器，并且应用Post-CMOS技术进行电子自旋加速。
4）微处理器：
除了之前所提方法，开发具有强深度学习能力的小尺寸处理器也是研究热点。微处理器的设计尺寸在一立方毫米的范围内，可以用电池驱动，进行深度神经网络分析只消耗大约300毫瓦。通过这种技术，许多对时间要求较高的IoT应用程序可以在设备上执行决策，而不是将数据发送到高性能计算机，等待它们的响应。
> 
IoT的雾和云中心深度学习
最近，人们提出了雾计算，使计算和分析更接近终端用户和设备，而不是仅仅停留在云计算上。实验表明，通过对雾计算节点进行数据分析，可以避免向遥远的云节点传输大量原始数据，从而提高整体性能。还可以在一定程度上进行实时分析，因为雾计算在本地，靠近数据源。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/fYsJGst9n2SNQYGuBAPTQL6HGKMDFKam5cnviaQWribg2Hmbia7nXZtzG2vic1A8g9XWVaaOqKDlOulYblglfZW8ag/640?wx_fmt=jpeg)*一些用于在雾或云上使用深度学习和服务的IoT领域的产品*
> 
技术和平台
尽管在雾计算架构上引入了深度学习分析，云计算仍然是许多无法在雾计算中处理的IoT应用的唯一可行的解决方案。因此，设计的可扩展的和高性能的云中心的DNN模型和算法，对大量的IoT数据进行分析，仍然是一个重要的研究领域。
除了在云平台上托管可扩展的深度学习模型基础设施的进步，还需要研究使深度学习模型通过API访问的机制和方法，以便容易地集成到IoT应用程序中。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/fYsJGst9n2SNQYGuBAPTQL6HGKMDFKam7QExA92BtH7ITnYicsJzudOFxvp54UkTMDEvzdNXhDtawzIiaXGgqvhw/640?wx_fmt=jpeg)
*在云平台中作为服务的深度学习模型*
> 
挑战
在雾计算节点上进行深度学习分析时，也会面临一些挑战：
深度学习服务发现：设备需要通过深度学习分析的某种扩展服务发现协议，来识别适当的分析提供者的来源。
深度学习模型和任务分布：在雾节点之间划分深度学习模型和任务的执行，以及在可用节点之间优化数据流分配，对于时间敏感的应用程序是至关重要的。
设计因素：研究如何雾计算环境的设计因素，以及在这种环境中部署深度学习模型如何影响分析服务的质量是很有必要的。
移动端：在设计终端辅助的深度学习分析系统时，需要考虑移动端计算环境的动态性，因为移动设备可能会加入或离开系统。
**深度学习带来的IoT挑战，以及未来的研究方向**
> 
挑战
1）缺少大型IoT数据集：
缺乏可用的实际IoT应用大数据集将深度学习模型引入IoT的一个主要障碍，因为深度学习需要更多的数据来实现更高的精度。此外，更多的数据也可以防止模型过度拟合。
2）预处理：
许多深度学习方法需要对数据进行预处理以产生更好的结果，对于IoT应用，预处理会更复杂，因为系统处理的是来自不同数据源的数据，可能有多种格式和分布，而且还可能有数据丢失。
3）安全和隐私：
确保数据安全和隐私是许多IoT应用的一个主要问题，因为IoT大数据将通过互联网进行分析，因此世界各地都有可能看得到。此外，深度学习训练模型也容易受到恶意攻击，如虚假数据注入或对抗性样本输入，其中IoT系统的许多功能或非功能性要求可能无法得到保证。
4）IoT大数据”6V“特性：
Volume（数据量）对于深度学习模型的时间消耗和结构复杂性提出了很大的挑战。并且数据量巨大也带来了包括噪声和未标注数据的挑战。
Variety（多样性）带来了管理不同数据源之间冲突的挑战。在数据源没有冲突的情况下，深度学习能够有效处理异质数据。
Velocity（速率）带来了高速处理和分析数据的要求，增强深度学习的在线学习和序列学习的技术仍需进一步研究。
Veracity（可信度），当输入数据不是来自可信的数据源时，IoT的大数据分析则是无用的。
Variability（可变性），IoT大数据的流速可变性对在线分析提出了挑战。
Value（价值），企业经理采用大数据的一个主要挑战是，他们不清楚如何使用大数据分析来获得价值，并改善他们的业务。
5）IoT设备上的深度学习：
在IoT设备上开发深度学习是一个新的挑战，要考虑在资源受限的设备上处理深度神经网络的需求。
6）深度学习局限：
尽管深度学习模型在许多应用中显示出令人印象深刻的结果，它仍然有局限性。研究发现，深度网络会将无法识别的图片分类到熟悉的种类中。并且深度神经网络的回归能力有待增强。
> 
未来研究方向
1）IoT移动数据：
IoT数据的一大部分来自移动设备。研究利用移动大数据与深度学习方法相结合的有效方式，可以为IoT提供更好的服务，特别是在智慧城市场景中。
2）结合环境信息：
单靠IoT的 传感数据不能理解环境的情况。因此，IoT数据需要与其他数据源融合，即环境信息，以补充对环境的理解。
3）IoT分析的在线资源供应：
基于雾和云计算的深度学习快速数据分析部署需要在线配置雾或云资源来承载数据流。由于IoT数据的流特性，无法提前知道数据序列的容量。因此，我们需要一种新的基于当前数据流的算法，并且不依赖于数据流的先验知识。
4）半监督分析框架：
为半监督学习而设计的先进的机器学习算法非常适合于智慧城市系统，可以使用少量的训练数据集训练模型，然后使用大量未标记数据来提高模型的准确性。
5）可靠的IoT分析：
深度学习方法可以通过分析大量的信息物理系统（CPS）和IoT系统的日志，以识别和预测可能受到攻击的系统的薄弱点。这将有助于系统防止或从故障中恢复，从而提高CPS和IoT系统的可靠性水平。
6）自组织通信网络：
由于IoT设备的数量庞大，配置和维护他们的基本物理M2M通信和网络变得越来越难。虽然大量的网络节点及其相互关系对传统的机器学习方法是一个挑战，但它为深度学习体系结构提供了一个机会，通过提供自配置、自优化、自修复和自负载平衡等一系列的自我服务足以证明它们在这一领域的能力。
7）新兴IoT应用：
无人机：无人机被用于许多实时图像分析任务，如监视、搜索和救援行动，以及基础设施检查。这些设备的采用面临包括路由、节约能源、避免私人区域和避障等挑战。深度学习对于该领域的预测和决策任务有很大的影响，可以推动无人机达到最佳性能。
虚拟/增强现实：虚拟/增强现实是受益于IoT和深度学习的另一个应用领域。增强现实可以用于提供诸如目标跟踪、行为识别、图像分类和对象识别这样的服务。增强现实可以极大地影响如教育，博物馆，智能车等几大领域。
**结论**
深度学习和IoT近年来受到研究人员和商业领域的广泛关注，这两项技术对我们的生活、城市和世界都产生了积极的影响。IoT和深度学习构成了一个数据生产者-消费者链，其中IoT生成由深度学习模型分析的原始数据，深度学习模型产生高层次的分析，反馈给IoT系统，以微调和改进服务。
查看论文原文：Deep Learning for IoT Big Data and Streaming Analytics: A Survey
参考文献
【1】N.-Y. Liang, G.-B. Huang, P. Saratchandran, and N. Sundararajan, “A fast and accurate online sequential learning algorithm for feedforward networks,” IEEE Transactions on neural networks, vol. 17, no. 6, pp. 1411–1423, 2006.
【2】S. Ren, K. He, R. Girshick, and J. Sun, “Faster R-CNN: Towards realtime object detection with region proposal networks,” IEEE transactions on pattern analysis and machine intelligence, vol. 39, no. 6, pp. 1137–1149, 2017.
【3】J. Redmon, S. Divvala, R. Girshick, and A. Farhadi, “You only look once: Unified, real-time object detection,” in Proceedings of the IEEE Conference on Computer Vision and Pattern Recognition, 2016, pp. 779–788.
【4】M. Mohammadi, A. Al-Fuqaha, M. Guizani, and J.-S. Oh, “Semisupervised deep reinforcement learning in support of IoT and smart city services,” IEEE Internet of Things Journal, vol. PP, no. 99, pp. 1–12, 2017.
【5】M. Price, J. Glass, and A. Chandrakasan, “A scalable speech recognizer with deep-neural-network acoustic models and voice-activated power gating,” in Proceedings of the IEEE ISSCC2017, 2017.
【6】Y. He, G. J. Mendis, and J. Wei, “Real-time detection of false data injection attacks in smart grid: A deep learning-based intelligent mechanism,” IEEE Transactions on Smart Grid, 2017.
【7】 Z. Yuan, Y. Lu, Z. Wang, and Y. Xue, “Droid-sec: deep learning in android malware detection,” in ACM SIGCOMM Computer Communication Review, vol. 44, no. 4. ACM, 2014, pp. 371–372.
【8】R. Shokri and V. Shmatikov, “Privacy-preserving deep learning,” in Proceedings of the 22nd ACM SIGSAC conference on computer and communications security. ACM, 2015, pp. 1310–1321.
【9】X. Song, H. Kanasugi, and R. Shibasaki, “Deeptransport: Prediction and simulation of human mobility and transportation mode at a citywide level.” IJCAI, 2016.
【10】V. C. Liang, R. T. Ma, W. S. Ng, L. Wang, M. Winslett, H. Wu, S. Ying, and Z. Zhang, “Mercury: Metro density prediction with recurrent neural network on streaming cdr data,” in Data Engineering (ICDE), 2016 IEEE 32nd International Conference on. IEEE, 2016, pp. 1374–1377.
【11】G. Amato, F. Carrara, F. Falchi, C. Gennaro, C. Meghini, and C. Vairo, “Deep learning for decentralized parking lot occupancy detection,” Expert Systems with Applications, 2017.
【12】X. Ma, H. Yu, Y. Wang, and Y. Wang, “Large-scale transportation network congestion evolution prediction using deep learning theory,” PloS one, vol. 10, no. 3, p. e0119044, 2015.
【13】C. Liu, Y. Cao, Y. Luo, G. Chen, V. Vokkarane, and Y. Ma, “Deepfood: Deep learning-based food image recognition for computer-aided dietary assessment,” in International Conference on Smart Homes and Health Telematics. Springer, 2016, pp. 37–48.
【14】C. R. Pereira, D. R. Pereira, J. P. Papa, G. H. Rosa, and X.-S. Yang, “Convolutional neural networks applied for parkinson’s disease identification,” in Machine Learning for Health Informatics. Springer, 2016, pp. 377–390.
【15】Q. Wang, Y. Guo, L. Yu, and P. Li, “Earthquake prediction based on spatio-temporal data mining: An lstm network approach,” IEEE Transactions on Emerging Topics in Computing, 2017.
【16】Y. Liu, E. Racah, J. Correa, A. Khosrowshahi, D. Lavers, K. Kunkel, M. Wehner, and W. Collins, “Application of deep convolutional neural networks for detecting extreme weather in climate datasets,” Int’l Conf. on Advances in Big Data Analytics, 2016.
【17】W. Liu, J. Liu, X. Gu, K. Liu, X. Dai, and H. Ma, “Deep learning based intelligent basketball arena with energy image,” in International Conference on Multimedia Modeling. Springer, 2017, pp. 601–613.
【18】K.-C. Wang and R. Zemel, “classifying nba offensive plays using neural networks,” in Proc. MIT SLOAN Sports Analytics Conf, 2016.
【19】T. Kautz, B. H. Groh, J. Hannink, U. Jensen, H. Strubberg, and B. M. Eskofier, “Activity recognition in beach volleyball using a deep convolutional neural network,” Data Mining and Knowledge Discovery, pp. 1–28, 2017.
【20】M. S. Ibrahim, S. Muralidharan, Z. Deng, A. Vahdat, and G. Mori, “A hierarchical deep temporal model for group activity recognition,” in Proceedings of the IEEE Conference on Computer Vision and Pattern Recognition, 2016, pp. 1971–1980.
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
