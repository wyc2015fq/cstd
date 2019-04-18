# NVIDIA各个领域芯片现阶段的性能和适应范围 - wishchinYang的专栏 - CSDN博客
2018年03月16日 11:49:24[wishchin](https://me.csdn.net/wishchin)阅读数：1274
       NVIDIA作为老牌显卡厂商，在AI领域深耕多年。功夫不负有心人，一朝AI火，NVIDIA大爆发，NVIDIA每年送给科研院所和高校的大量显卡，大力推广Phy*six*和CUDA，终于钓了产业的大鱼。
       由弱到强理一下NVIDIA的现有产品线，在AMD锐龙发力之后，NVIDIA已经取代Intel，称为新的牙膏厂。
       在与人工智能与机器人有关的平台上，NVIDAI产品线性能由低到高依次为Tegra X系列、GeForce 系列、DRIVER系列、Titan 系列、TESLA系列。
       参考：[NVIDIA-GPU的浮点计算](http://blog.csdn.net/haima1998/article/details/78250143)能力
       Linux系统是广泛应用的OS，可以以各种形态存在，甚至可以裁剪到么有MU的微型计算机上。现有支持较为完备的流行的计算机OS主要为CentOS和ubuntu，Fedora在消费级商业化上做的还是差了一点。
**一、SHIELD掌机系统**
完整规格  **SHIELD掌机系统** 是基于TX1-CPU的集成平台
处理器    NVIDIA® **Tegra® X1 处理器**，搭载 256 核 GPU 和 3 GB RAM
视频功能  支持 4K HDR 
以 60 FPS 的帧速率播放分辨率高达 4K 的 HDR 内容 (H.265/HEVC) 
以 60 FPS 的帧速率播放分辨率高达 4K 的内容（VP8、VP9、H.264、MPEG1/2） 
以 60 FPS 的帧速率播放分辨率高达 1080p 的内容（H.263、MJPEG、MPEG4、WMV9/VC1）
通过 HDMI 提供 Dolby Atmos 和 DTS-X 环绕立体声透传音效 
通过 HDMI 和 USB 播放高达 24 位/192 kHz 的高清音频内容 
通过 USB 播放高达 24 位/192 kHz 的高清音频内容 
操作系统 Android 7.0
软件更新 直接由 NVIDIA 提供 SHIELD 软件更新
**二、嵌入式平台的Jetson**
         现有的平台为[TX2和TX1](http://www.nvidia.cn/object/autonomous-robots-cn.html)，提供 开发套件....
         参考[CPU两倍于820](https://baijiahao.baidu.com/s?id=1561274449482192&wfr=spider&for=pc)处理器
         基于TX1和TX2，NVIDIA为移动机器人系统提供了不同层级的混搭，面对开发者的开发板、和集成视觉系统的工业套件。
### 模块技术规格
||Jetson TX2|Jetson TX1|
|----|----|----|
|GPU|NVIDIA Pascal™、256 颗 CUDA 核心|NVIDIA Maxwell™、256 颗 CUDA 核心|
|CPU|HMP Dual Denver 2/2 MB L2 + Quad ARM® A57/2 MB L2|Quad ARM® A57/2 MB L2|
|视频|4K x 2K 60 Hz 编码 (HEVC)4K x 2K 60 Hz 解码（12 位支持）|4K x 2K 30 Hz 编码 (HEVC)4K x 2K 60 Hz 解码（10 位支持）|
|内存|8 GB 128 位 LPDDR458.3 GB/s|4 GB 64 位 LPDDR425.6 GB/s|
|显示器|2 个 DSI 接口、2 个 DP 1.2 接口/HDMI 2.0 接口/eDP 1.4 接口|2 个 DSI 接口、1 个 eDP 1.4 接口/DP 1.2 接口/HDMI 接口|
|CSI|超级多 6 个摄像头（2 通道CSI2 D-PHY 1.2（每个通道 2.5 Gbps）|超级多 6 个摄像头（2 通道）CSI2 D-PHY 1.1（每个通道 1.5 Gbps）|
|PCIE|Gen 2 | 1x4 + 1x1 或 2x1 + 1x2|Gen 2 | 1x4 + 1x1|
|数据存储|32 GB eMMC、SDIO、SATA|16 GB eMMC、SDIO、SATA|
|其他|CAN、UART、SPI、I2C、I2S、GPIO|UART、SPI、I2C、I2S、GPIO|
|USB|USB 3.0 + USB 2.0| |
|连接|1 千兆以太网、802.11ac WLAN、蓝牙| |
|机械|50 毫米 x 87 毫米（400 针兼容板对板连接器）| |
支持平台：
         基于TX2 的平台可运行ubuntu系统。
        NVIDIA为此计算平台贡献了软件开发套件：https://developer.nvidia.com/embedded/downloads#?tx=$software,l4t-tk1。
性能：
**FLOPS**：TX1上，配置是Tegra X1+4G LPDDR4，GPU是256个CUDA的Maxwell，TX2则升级为Tegra Parker处理器，也就是16nm 6核CPU（2x丹佛+4xA57），所以GPU部分也升级为Pascal架构，同样256个CUDA，浮点性能**1.5 TeraFLOPS**，也就是提高了50%。
        Jetson TX2 延续了tegra系列体积小巧、高度集成的特性，大小仅相当于一张信用卡。与前一代Jetson TX1相比，TX2能提供两倍的功效。GPU、CPU都进行了升级，其中GPU变成了Pascal 架构（16 nm工艺）。这意味着Jetson 系列进入了 Pascal 架构时代。内存、存储都增加了一倍，提供了 8G 内存、32G 固态存储器。支持802.11ac WLAN和蓝牙。
       功耗TDP：15W
购买：
【注1】你想申请TX2，你需要拥有有效且公认的大学电子邮件地址就可以了。笔者就不在这具体购买流程请咨询代购商。 
【注2】TX2在某宝上也能购买，当然笔者走的是教育优惠。
**三、消费级计算平台的GeForce**
           面向专业图形工作站应用的**Quadro系列**，硬件核心和GeForce是相同的。
         GeForce平台为消费级显卡，主要用途依然是3D渲染。消费用途主要是玩游戏和PS，工业用途做2维和三维图像处理。NVIDAI把Phy*six*和CUDA普及到GeForce平台，GeForce平台也开始了另一新的功能，兼职做科学计算。
         GeForce平台产品眼花缭乱，玩游戏的人群比做机器人的人群覆盖面更广，需求也更多样化。以现有最新的帕斯卡构架的消费级显卡来说，从低端的GT1030 到高端的GTX1080TI，参数性能价格已有天壤之别。
        GeForce GT 1030显卡基于GP108-300核心，拥有384个CUDA处理器，基础核心频率1227MHz，boost频率1468MHz。使用2GB GDDR5显存，显存位宽64-bit，带宽48GB/s。支持最新的DX12，OpenGL4.5等特性，TDP仅30W。价格最低不到500块。
         GeForceGTX1080拥有2560个[CUDA](https://baike.baidu.com/item/CUDA)处理器，核心频率1607MHz，boost频率1733MHz，等效[显存](https://baike.baidu.com/item/%E6%98%BE%E5%AD%98)频率10GHz。显卡位宽为256bit，带宽320GB/s。公版显卡最高温度94℃，采用单8pin供电，TDP180w。
         GeForceGTX1080强于GeForceGTX980SLI，甚至比GeForceGTXTitanX还要强，大概超过TitanX2成。单卡*浮点*运算次数上,*GTX1080Ti*（显存8GB）达到**11.5T FLOPs**。兵荒马乱的年月，GTX1080Ti已经涨到了6000+。
         因为CUDA的存在，大量的科研计算使用GeForce卡做并行计算，现在AI最流行CUDA做深度学习。
         TDP：30W-300W。移动计算机平台TDP低至15w。
**四、为自动驾驶开发的Tesla系列**
        NVIDIA Driver系列为自动驾驶提供一整套计算解决方案想，现有平台有 NVIDIA DRIVER PX、NVIDIA DXG1、NVIDIA DRIVER IX。
**        Driver PX : [https://www.nvidia.com/zh-cn/self-driving-cars/drive-px/](https://www.nvidia.com/zh-cn/self-driving-cars/drive-px/)**
      可扩展架构适用于各种配置。从功率为 10 瓦的被动冷却型移动处理器，到双移动处理器加双独立 GPU 的多芯片配置（可提供每秒 24 万亿次的深度学习运算），都在此架构的支持范围内。您可并行使用多个 DRIVE PX 平台来实现完全自动驾驶。
       DRIVER PX Pegasus AI 计算机拥有高达 **320 TOPS**的深度学习计算能力。「Pegasus」之运算能力达到320 TOPS(Trillion Operations Per Second)，超越其前代平台「Drive PX 2」之运算能力高达10倍。 此运算能力主要来自于4颗处理器-2颗为以[*NVIDIA*](http://www.eepw.com.cn/news/listbylabel/label/NVIDIA)目前最新GPU架构「Volta」为核心的SoC「Xavier」、以及另外2颗为车用机械视觉与深度学习所准备的专用GPU。
       DRIVE PX Xavier 能够提供** 20 TOPS** 的高性能，而功耗仅有**20 瓦**。单个 Xavier AI 处理器中就包含了 70 亿个晶体管，能够取代目前配备多个移动 SoC 和两个独立 GPU 的 DRIVE PX，且功耗大幅下降。Xavier为低功耗自动驾驶平台。
**NVIDIA DRIVER IX **系列在硬件基础上集成了一些与自动驾驶有关的软件功能，如基于深度学习的面部识别、视线追踪、和对话功能。
      CES2018展会上，百度的“阿波罗”(Apollo)自动驾驶项目将使用英伟达的Drive Xavier自动驾驶计算平台。百度的阿波罗项目致力于通过与一系列汽车制造商、供应商以及科技公司合作，为自动驾驶汽车打造一个开放平台。
**五、GPU加速计算的Tesla系列**
**Tesla**本是第一代产品的架构名称，后来演变成了这个系列产品的名称了。在NVIDIA的产品列表上，被定义为数据中心产品，产品线为TESLA、DXG、DXG-Station、HGX。
        现在NVIDIA官方显示的产品有TESLA-P40和TESLA-P6。P40浮点运算为**47T FLOPS**。TESLA-P40的成品价格为**4W+**（显存24GB）。
        黄仁勋在GTC2017 上发布了TESLA V100和他的 VoltaGPU。仔细读一下文章，有不少知识：英伟达[Volta架构：为深度学习而生](http://www.yxtvg.com/toutiao/5007404/20170511A05LZM00.html)的Tensor Core。当然，TESLA的价格吗，**15万$**，100万呢....
        Tesla V100 可以提供业界领先的浮点和整型计算性能。峰值计算速度（基于 GPU Boost 时钟频率）：
双精度浮点（FP64）运算性能：7.5 TFLOP/s；
单精度（FP32）运算性能：15 TFLOP/s;
混合精度矩阵乘法和累加：**120 Tensor TFLOP**/s。
        新的 Tensor Core 是 Volta GV100 最重要的特征，有助于提高训练神经网络所需的性能。Tesla V100 的 Tensor Core 能够为训练、推理应用的提供**120 Tensor TFLOPS**。
**Tensor核**
..................
**NVIDIA TITAN，**英伟达联合创始人兼CEO黄仁勋在NIPS2017全球神经信息处理系统大会上突然发布了全新的TITAN显卡，NVIDIA TITAN V！新显卡基于新一代的Volta架构打造，完整的GV100核心，峰值浮点性能高达**110TFlops**，相当于TITAN Xp 12Tflops的9倍。Titan也可以面对消费者，相对于TESLA V100，便宜了不少，只要3000$，也要**2万块**。
         消费级旗舰显卡基于12nm GV100核心，内建5120颗CUDA核心，640颗Tensor核心（用于AI专门计算和加速），核心主频1200MHz，加速频率1455MHz。搭配12GB HBM2显存，位宽3072bit，总带宽653GB/s。性能方面，**单精度浮点15 TFLOPS**，此前公布时给出的110T仅仅是用于深度学习的Tensor内核性能，所以这样话，**比TITAN Xp提高了24%左右。**
         TITAN V使用Volta构架，使用了Tensor核，和适合微型的企业和研究机构做深度学习。[https://www.nvidia.com/zh-cn/titan/titan-v/](https://www.nvidia.com/zh-cn/titan/titan-v/)。
**DGX-1** 定位为个人计算中心，价格为5万$。参考：[https://www.nvidia.com/zh-cn/data-center/dgx-systems/](https://www.nvidia.com/zh-cn/data-center/dgx-systems/)，系统CPU使用的是8块TESLA V100，128GB显存。提供960 TFLOPS（GPU FP16）性能 。
...................
         差不多结束了，当然据说百度谷歌都有一次性集成两万张TESLA显卡的习惯。
