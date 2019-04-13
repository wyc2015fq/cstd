
# openwrt 音频开发 - 嵌入式Linux - CSDN博客

2018年10月30日 19:30:19[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：266



## 1、Linux 音频架构图
音视频的好坏 直接影响 产品体验
![](https://pic4.zhimg.com/80/v2-bbf3b790d72623842650745dd7ad5e25_hd.jpg)
## 2、音频架构图层次说明
openWRT 采用 ALSA 层次图，如下
Application: 上层应用 主要调用alsa-lib 中的接口 实现业务逻辑。使用alsa-util中aplay,arecord,amixer,speaker-test进行相关测试。
HAL层 ： 移植alsa-lib 和 alsa-utils. 在openwrt中 feeds 自带了alsa-lib 1.1.01[[简介](https://link.zhihu.com/?target=https%3A//wiki.openwrt.org/doc/devel/feeds)]
kernel: 按照ALSA 驱动层进行适配。[[简介](https://link.zhihu.com/?target=https%3A//www.kernel.org/doc/html/v4.17/sound/kernel-api/writing-an-alsa-driver.html)]
## 3、Application层
openwrt中应用启动需要自定义启动脚本，放在目录/etc/init.d 目录下。
## 4、启动脚本顺序定义
05 defconfig //加载默认参数
10 boot //启动
39 usb // 加载usbfs
40 network // 设置网卡参数
45 firewall // 防火墙
50 dropbear // sshd server
50 cron // …
50 telnet // 如果没有修改root密码，则启动telnet server
60 dnsmasq // DHCP 和 DNS 服务端
95 done // …
96 led // 指示灯
97 watchdog // …
99 sysctl // 最后，进行必要的内核参数调整
## 5、启动脚本书写
[按照官方Wiki脚本,进行自定制](https://link.zhihu.com/?target=https%3A//wiki.openwrt.org/doc/techref/initscripts)。
## 6、HAL层
这一层不需要做太多改动，需要配置feeds选择自己需要的版本即可，
具体的接口查询，可以到[alsa-project](https://link.zhihu.com/?target=http%3A//www.alsa-project.org/main/index.php/Main_Page)查询。
## 7、kernel
根据ALSA驱动进行相关分析。
## machine
`/* SoC machine */
struct snd_soc_card {
      char *name;
      ...
      int (*probe)(struct platform_device *pdev);
      int (*remove)(struct platform_device *pdev);
      
      /* the pre and post PM functions are used to do any PM work before and
       * after the codec and DAIs do any PM work. */
      int (*suspend_pre)(struct platform_device *pdev, pm_message_t state);
      int (*suspend_post)(struct platform_device *pdev, pm_message_t state);
      int (*resume_pre)(struct platform_device *pdev);
      int (*resume_post)(struct platform_device *pdev);
      ...
      /* CPU <--> Codec DAI links  */
      struct snd_soc_dai_link *dai_link;
      int num_links;
      ...
};`probe/remove 可选，主要侦测machine。
suspend/resume 在codec，DAIs和DMA suspend resume 会相应的触发，也是可选的
machine DAI口配置，配置对应的结构体
`/*  digital audio interface glue - connects codec <--> CPU */
static struct snd_soc_dai_link corgi_dai = {
	.name = "WM8731",
	.stream_name = "WM8731",
	.cpu_dai_name = "pxa-is2-dai",
	.codec_dai_name = "wm8731-hifi",
	.platform_name = "pxa-pcm-audio",
	.codec_name = "wm8713-codec.0-001a",
	.init = corgi_wm8731_init,
	.ops = &corgi_ops,
};
/* audio machine driver */
static struct snd_soc_card snd_soc_corgi = {
      .name = "Corgi",
      .dai_link = &corgi_dai,
      .num_links = 1,
};`
## plarform
DMA 驱动，[Soc DAI 驱动](https://link.zhihu.com/?target=http%3A//www.alsa-project.org/~tiwai/writing-an-alsa-driver/)
`/* SoC audio ops */ 
struct snd_soc_ops { 
		int (*startup)(struct snd_pcm_substream *);
		void (*shutdown)(struct snd_pcm_substream *);
		int (*hw_params)(struct snd_pcm_substream *, struct snd_pcm_hw_params *); int (*hw_free)(struct snd_pcm_substream *);
		int (*prepare)(struct snd_pcm_substream *);
		int (*trigger)(struct snd_pcm_substream *, int); 
		};
//平台驱动通过DMA接口关联起来
struct snd_soc_platform_driver { 
		char *name;
		int (*probe)(struct platform_device *pdev);
		int (*remove)(struct platform_device *pdev);
		int (*suspend)(struct platform_device *pdev, struct snd_soc_cpu_dai *cpu_dai); 
		int (*resume)(struct platform_device *pdev, struct snd_soc_cpu_dai *cpu_dai);
	/* pcm creation and destruction */
		int (*pcm_new)(struct snd_card *, struct snd_soc_codec_dai *, struct snd_pcm *); 
		void (*pcm_free)(struct snd_pcm *);
	/*
	 * For platform caused delay reporting.
	 * Optional.
	 */ 
	 snd_pcm_sframes_t (*delay)(struct snd_pcm_substream *, struct snd_soc_dai *);
	/* platform stream ops */
		struct snd_pcm_ops *pcm_ops;
	};`
## codec
每个codec codec 驱动必须具备以下功能
codec DAI 与 PCM 配置
使用I2C 或者SPI控制IO
mixer audio 控制
codec 音频操作
DAPM 描述
DAPM 事件处理
可选
DAC mute处理
`// DAI PCM配置
static struct snd_soc_dai_ops wm8731_dai_ops = {
	.prepare	= wm8731_pcm_prepare,
	.hw_params	= wm8731_hw_params,
	.shutdown	= wm8731_shutdown,
	.digital_mute	= wm8731_mute,
	.set_sysclk	= wm8731_set_dai_sysclk,
	.set_fmt	= wm8731_set_dai_fmt, };
struct snd_soc_dai_driver wm8731_dai = { .name = "wm8731-hifi",
	.playback = { .stream_name = "Playback",
		.channels_min = 1,
		.channels_max = 2,
		.rates = WM8731_RATES,
		.formats = WM8731_FORMATS,},
	.capture = { .stream_name = "Capture",
		.channels_min = 1,
		.channels_max = 2,
		.rates = WM8731_RATES,
		.formats = WM8731_FORMATS,}, .ops = &wm8731_dai_ops,
	.symmetric_rates = 1, };`// i2c 控制 读写
`i2c_write
i2c_read`// mixer audio control
codec 说有mixer与control都定义在soc.h中
`#define SOC_SINGLE(xname, reg, shift, mask, invert)
定义单一控制器:-
  xname  控制名称e.g. "Playback Volume"
  reg = codec register
  shift = control bit(s) offset in register
  mask = control bit size(s) e.g. mask of 7 = 3 bits
  invert = the control is inverted 是否反转
其他宏包括： -
    #define SOC_DOUBLE （xname ，reg ，shift_left ，shift_right ，mask ，invert ）
立体声控制
    #define SOC_DOUBLE_R （xname ，reg_left ，reg_right ，shift ，mask ，invert ）
一个跨越2个寄存器的立体声控制
    #define SOC_ENUM_SINGLE （xreg ，xshift ，xmask ，xtexts ）
   xreg = 寄存器
   XSHIFT = 控制位（小号）偏移在寄存器 
   xmask = 控制位（小号）大小
   xtexts = 指向描述每个设置的字符串数组的指针`//audio 控制操作
`/* SoC audio ops */ 
struct snd_soc_ops { 
	int (*startup)(struct snd_pcm_substream *);
	void (*shutdown)(struct snd_pcm_substream *);
	int (*hw_params)(struct snd_pcm_substream *, struct snd_pcm_hw_params *); int (*hw_free (struct snd_pcm_substream *);
	int (*prepare)(struct snd_pcm_substream *); 
	};`//DAPM
//DAPM handler 参考
## 8、openWRT 音频应用
上面一节介绍了音频框架。在openWRT音频业务主要VOIP通过 ，上/下行通过模块如下：
![](https://pic4.zhimg.com/80/v2-5aaca5d8627347184a5908015b642d7c_hd.jpg)
## 9、调试技术
## 链路上应用层UDP截取文件
知道UDP 下的G711A 对应的buffer
`// 应用层将buffer写到文件 代码块
            static struct file *file_g711a= NULL;
            int writelen = 0；
            if (NULL == fp)
            {
                file_g711a= file_open("/test1", O_RDWR | O_CREAT, 0777);
                 if(file_g711a == NULL){
          			printf("g711a file  = NULL");
       	 			}else{
           			printf("g711a file open ok");
         			}   
            }
			if(file_g711a != NULL){
            	 writelen = fwrite(g711a_buffer, 1, sizeof(g711a_buffer), file_g711a);
            }
            printk("writelen:%d\n",writelen);`
## 10、链路上 alsa 截取文件
转换成PCM后，将对应的buffer 写道文件
`// 应用层将buffer写到文件 代码块
            static struct file *file_pcm= NULL;
            int writelen = 0；
            if (NULL == fp)
            {
                file_pcm= file_open("/test.pcm", O_RDWR | O_CREAT, 0777);
                 if(file_pcm == NULL){
          			printf("pcm file  = NULL");
       	 			}else{
           			printf("pcm file open ok");
         			}   
            }
			if(file_pcm != NULL){
            	 writelen = fwrite(pcm_buffer, 1, sizeof(pcm_buffer), file_pcm);
            }
            printk("writelen:%d\n",writelen);`
## 链路上kernel 截取文件
在PCM与DMA通信的函数中 将buffer 写到文件中
`// kernel 将buffer写到文件 代码块
            static struct file *fp = NULL;
            mm_segment_t fs;
            static loff_t pos = 0;
            printk("hello enter\n");
            if (NULL == fp)
            {
                fp = filp_open("/test.pcm", O_RDWR | O_CREAT, 0777);
                if (IS_ERR(fp))
                {
                    printk("create file error\n");
                    return -1;
                }
            }
            fs = get_fs();
            set_fs(KERNEL_DS);
            int writelen = vfs_write(fp, buf, size*4, &pos);
            pos += size*4;
            printk("writelen:%d pos:%d\n",writelen, pos);`
## 11、关注我们
觉得对你有用，关注我们的公众号【嵌入式Linux】

