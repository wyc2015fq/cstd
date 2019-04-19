# Android S5PV210 camera驱动（tw9912）小结 - xqhrs232的专栏 - CSDN博客
2013年04月08日 11:25:57[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1044
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://blog.csdn.net/kickxxx/article/details/7714938](http://blog.csdn.net/kickxxx/article/details/7714938)
之前没接触过三星的开发板，更不了解S5PV210，TW9912这款AD芯片也是第一次接触，三星FAE又不给力（其实是没有），调试camera过程中，一会怀疑TW9912配置不对，一会怀疑三星FIMC驱动有问题，比较痛苦，今天终于调出了正确的图像，发贴和大家分享一下
**TW9912 配置**
TW9912这款AD芯片，TechWell FAE给了几组寄存器配置：
PAL in 656P out --- PAL做输入，输出为BT656的逐行方式
PAL in 656i out --- PAL做输入， 输出为BT656的隔行方式
NTSC in 656P out --- NTSC做输入，输出为BT656的逐行方式
NTSC in 656i out --- NTSC做输入，输出为BT656的隔行方式
考虑到后端FIMC对隔行的处理极其怪异（到现在我还没明白它输出的四个buffer后，怎么组合成一帧完整图像的），所以我选择了逐行输出，PAL输入(我的摄像头是PAL制式)，FAE给的PAL in 656P out配置是可以正常工作的
TechWell FAE给的配置寄存器值，是一次性逐个设置，顺序无所谓，这也是很神奇的地方啊，不过日后我还是要扩展一下，自动检测制式后，再设入相应的参数，这是后话。
在这插点跑题的话：说到逐行输出，我花了很多时间怀疑逐行BT656输出有问题，因为标准的BT656信号是不支持逐行的，我不知哪家油菜的厂商，扩展了标准的BT656，更牛逼的是，一个厂家产品输出逐行BT656信号，另外的厂家接收处理这个逐行BT656信号，他们居然配合的很好。网上有人讨论过这个逐行BT656是什么东西，就是BT656的F位总是0。
TW9912配置为逐行输出后，pixclk为54Mhz，相比隔行输出的pixclk=27Mhz，增大了一倍，因此帧数也增大了一倍由25->50，或者30->60。
pixclk频率有个大概的算法: 行数据 * 行数 * 帧率
PAL: (1440 + 280 + 4 + 4) * 625 * 25 = 27000000
NTSC: (1440 + 280 + 4 + 4) * 525 * 30 = 2721600
**FIMC驱动**
Samsung的camera后端是FIMC控制器，Samsung写了一个FIMC驱动框架，代码在drivers/media/video/samsung/fimc/下，fimc驱动框架我后面会分析
fimc的代码和mx51的camera驱动框架比起来，要简单清晰很多。但是还是有几个地方要修改一下，才能把camera驱动正确跑起来
1. 在arch/arm/mach-s5pv210/mach-smdkc210.c 中添加一个s3c_platform_camera设备
**[html]**[view
 plain](http://blog.csdn.net/kickxxx/article/details/7714938#)[copy](http://blog.csdn.net/kickxxx/article/details/7714938#)
- .id     = CAMERA_PAR_A,  
- .type       = CAM_TYPE_ITU,  
- .fmt        = ITU_656_YCBCR422_8BIT,  
- .order422   = CAM_ORDER422_8BIT_YCBYCR,  
- .i2c_busnum = 1,    
- .info       = &tw9912_i2c_info,  
- .pixelformat = V4L2_PIX_FMT_YUYV,  
- .srclk_name = "mout_mpll",  
- .clk_name   = "sclk_cam0",  
- .clk_rate   = 44000000,  
- .line_length = 1440,  
- .width      = 720,  
- .height     = 576,  
- .window     = {  
- .left   = 0,  
- .top    = 0,  
- .width  = 720,  
- .height = 576,  
- },  
- 
- /* Polarity */  
- .inv_pclk   = 0,  
- .inv_vsync  = 0,  
- .inv_href   = 0,  
- .inv_hsync  = 0,  
.id = CAMERA_PAR_A， 查看自己的原理图，看看使用哪个物理camera接口
FIMC共支持三个camera接口，其中两个为ITU类型，所谓ITU类型就是支持BT601或者BT656协议，另外一个是MIPI接口
.type = CAM_TYPE_ITU 这个毫无疑问了，我们使用的就是ITU接口类型
.srclk_name = "mout_mpll",
.clk_name = "sclk_cam0",
.clk_rate = 44000000
因为我使用的是外部晶振，不需要提供mclk，所以这几行实际没什么意义，但是如果你的camera需要FIMC提供mclk，那么要正确设置这个mclk
.line_length = 1440 这一行没什么用，随便设
.width = 720,
.height = 576,
PAL制式摄像头的标准宽高
2. 修改FIMC工作模式BT601->BT656模式
FIMC对于ITU接口，缺省的工作模式是BT601，需要改成BT656。我猜samsung的开发人员用的sensor只支持BT601，所以他们也仅验证了BT601
3. 修改DMA分配的对齐方式
DMA对齐问题折磨了我一个礼拜，因为缺省情况下，fimc驱动对DMA内存的分配对齐方式是没有要求的，而FIMC硬件在有些情况下需要DMA地址必须是4K对齐，如果你给了一个非4k对齐的地址如0x4abc5800，那么DMA是不会从这个地址开始写camera数据的，而是从0x4abc5000开始写数据，这就导致拍出来的图片发生了错位，如下图中间偏左的粗黑线。
黑线左边还有条细黑线，这个是TW9912的问题，调整VDELAY VACTIVE HDELAY HACTIVE几个参数就可以消掉，这个太搞笑了，也是调camera驱动过程中 一直怀疑TW9912有问题的原因
![](https://img-my.csdn.net/uploads/201207/06/1341551957_5223.png)
**测试**
用测试程序抓取图片，就可以验证了
