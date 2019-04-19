# Linux下IIS音频驱动程序分析(基于S3C2410+ UDA1341) - xqhrs232的专栏 - CSDN博客
2011年12月12日 22:13:32[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1469
原文地址::[http://os.chinaunix.net/a2009/0901/997/000000997053.shtml](http://os.chinaunix.net/a2009/0901/997/000000997053.shtml)
armlinux学习笔记－－IIS音频驱动程序分析[z]
    2008-07-18 19:53:51 
    原文:http://www.icdew.com/viewthread.php?tid=284
    //*******************************************************
    //* 2007.7.5
    //*******************************************************
     Linux 下的IIS 音频驱动程序主要都在/kernel/drivers/sound/s3c2410-uda1341.c 文件中。 在音频驱动程序中有2个比较重要的结构体： typedef struct {
    int size; /* buffer size */
    char *start; /* point to actual buffer */（内存虚拟地址起始地址）
    dma_addr_t dma_addr; /* physical buffer address */（内存物理地址起始地址）
    struct semaphore sem; /* down before touching the buffer */
    int master; /* owner for buffer allocation, contain size when true */（内存大小）
    } audio_buf_t; typedef struct {
    audio_buf_t *buffers; /* pointer to audio buffer structures */
    audio_buf_t *buf; /* current buffer used by read/write */
    u_int buf_idx; /* index for the pointer above */
    u_int fragsize; /* fragment i.e. buffer size */（音频缓冲区片大小）
    u_int nbfrags; /* nbr of fragments */（音频缓冲区片数量）
    dmach_t dma_ch; /* DMA channel (channel2 for audio) */
    } audio_stream_t; 这是一个管理多缓冲区的结构体，结构体audio_stream_t 为音频流数据组成了一个环形缓冲区。（audio_buf_t *buffers 同触摸屏驱动中struct TS_DEV 结构中的TS_RET buf[MAX_TS_BUF] 意义一样，都为环形缓冲区）用audio_buf_t 来管理一段内存，在用audio_stream_t 来管理N 个audio_buf_t。 音频驱动的file_operations 结构定义如下：
    static struct file_operations smdk2410_audio_fops = {
    llseek: smdk2410_audio_llseek,
    write: smdk2410_audio_write,
    read: smdk2410_audio_read,
    poll: smdk2410_audio_poll,
    ioctl: smdk2410_audio_ioctl,
    open: smdk2410_audio_open,
    release: smdk2410_audio_release
    }; static struct file_operations smdk2410_mixer_fops = {
    ioctl: smdk2410_mixer_ioctl,
    open: smdk2410_mixer_open,
    release: smdk2410_mixer_release
    }; 这里定义了两种类型设备的file_operations 结构，前者是DSP 设备，后者是混频器设备。 
    ------------------------------------------------------------------------
     和往常一样，先来看一下加载驱动模块时的初始化函数：
    int __init s3c2410_uda1341_init(void)
    该函数首先会初始化I/O 和UDA1341 芯片，然后申请2个DMA 通道用于音频传输。 local_irq_save(flags); 调用该宏函数来保存IRQ 中断使能状态，并禁止IRQ 中断。 在/kernel/include/asm-arm/system.h 文件中：
    /* For spinlocks etc */
    #define local_irq_save(x) __save_flags_cli(x)
    #define local_irq_restore(x) __restore_flags(x) 在/kernel/include/asm-arm/proc-armo/system.h 文件中：
    /*
    * Save the current interrupt enable state & disable IRQs
    */
    #define __save_flags_cli(x) \
    do { \
     unsigned long temp; \
     __asm__ __volatile__( \
    " mov %0, pc @ save_flags_cli\n" \
    " orr %1, %0, #0x08000000\n" \
    " and %0, %0, #0x0c000000\n" \
    " teqp %1, #0\n" \
     : "=r" (x), "=r" (temp) \
     : \
     : "memory"); \
    } while (0) 最后用ARM 汇编指令实现了保存IRQ 和FIQ 的中断使能状态，并禁止IRQ 中断。 /*
    * restore saved IRQ & FIQ state
    */
    #define __restore_flags(x) \
    do { \
     unsigned long temp; \
     __asm__ __volatile__( \
    " mov %0, pc @ restore_flags\n" \
    " bic %0, %0, #0x0c000000\n" \
    " orr %0, %0, %1\n" \
    " teqp %0, #0\n" \
     : "=&r" (temp) \
     : "r" (x) \
     : "memory"); \
    } while (0) 最后用ARM 汇编指令实现了恢复IRQ 和FIQ 的中断使能状态。 
    /* GPB 4: L3CLOCK, OUTPUT */
    set_gpio_ctrl(GPIO_L3CLOCK);
    /* GPB 3: L3#, OUTPUT */
    set_gpio_ctrl(GPIO_L3#);
    /* GPB 2: L3MODE, OUTPUT */
    set_gpio_ctrl(GPIO_L3MODE); /* GPE 3: I2SSDI */
    set_gpio_ctrl(GPIO_E3 | GPIO_PULLUP_EN | GPIO_MODE_I2SSDI);
    /* GPE 0: I2SLRCK */
    set_gpio_ctrl(GPIO_E0 | GPIO_PULLUP_EN | GPIO_MODE_I2SSDI);
    /* GPE 1: I2SSCLK */
    set_gpio_ctrl(GPIO_E1 | GPIO_PULLUP_EN | GPIO_MODE_I2SSCLK);
    /* GPE 2: CDCLK */
    set_gpio_ctrl(GPIO_E2 | GPIO_PULLUP_EN | GPIO_MODE_CDCLK);
    /* GPE 4: I2SSDO */
    set_gpio_ctrl(GPIO_E4 | GPIO_PULLUP_EN | GPIO_MODE_I2SSDO); 
     接下来马上设置与UDA1341 芯片相关GPIO 引脚。这里首先将GPB4，GPB3，GPB2 这3个GPIO 引脚设置为输出模式，参考原理图后，得知这3个引脚分别连接UDA1341 芯片的L3CLOCK，L3#，L3MODE 这3个引脚，作为这3个信号的输入。 在/kernel/drivers/sound/s3c2410-uda1341.c 文件中：
    #define GPIO_L3CLOCK (GPIO_MODE_OUT | GPIO_PULLUP_DIS | GPIO_B4)
    #define GPIO_L3# (GPIO_MODE_OUT | GPIO_PULLUP_DIS | GPIO_B3)
    #define GPIO_L3MODE (GPIO_MODE_OUT | GPIO_PULLUP_DIS | GPIO_B2)
     然后继续设置与IIS 控制器输出信号相关GPIO 引脚。将GPE0～GPE4 这5个引脚设置为IIS 接口的信号模式。需要通过配置GPECON 寄存器来设定该端口管脚的输出模式，对应位如下： [9:8] [7:6] [5:4] [3:2] [1:0]
    GPE4 GPE3 GPE2 GPE1 GPE0
    参考S3C2410 芯片#sheet 的I/O口章节，都要设为10（二进制）。 local_irq_restore(flags); 设置完GPIO 口的工作模式，就可以前面已经分析过的local_irq_restore 宏函数来恢复IRQ 和FIQ 的中断使能状态。 init_uda1341(); 这里调用了init_uda1341 函数来初始化UDA1341 芯片，该函数会在后面说明。 output_stream.dma_ch = DMA_CH2; if (audio_init_dma(&output_stream,
 "UDA1341 out")) {
     audio_clear_dma(&output_stream);
     printk( KERN_WARNING AUDIO_NAME_VERBOSE 
     ": unable to get DMA channels\n" );
     return -EBUSY;
    } input_stream.dma_ch = DMA_CH1; if (audio_init_dma(&input_stream, "UDA1341 in")) {
     audio_clear_dma(&input_stream);
     printk( KERN_WARNING AUDIO_NAME_VERBOSE
     ": unable to get DMA channels\n" );
     return -EBUSY;
     }
     在全局变量中定义了，两个audio_stream_t 结构的变量，分别是output_stream 和input_stream，一个作为输出音频缓冲区，一个作为输入音频缓冲区。
     将输出音频缓冲区的DMA 通道设为通道2，输入音频缓冲区的DMA 通道设为通道1。 在/kernel/include/asm-arm/arch-s3c2410/dma.h 文件中：
    #define DMA_CH0 0
    #define DMA_CH1 1
    #define DMA_CH2 2
    #define DMA_CH3 3 通过查阅S3C2410 芯片#sheet 中的DMA 章节，知道该芯片共有4个DMA 通道，DMA 控制器的每个通道可以从4个DMA 源中选择一个DMA 请求源。其中，通道1具有IIS 输入源，而通道2具有IIS 输出和输入源。所以要以全双工模式进行音频数据传输的话，只有将输出音频缓冲区的设为DMA 通道2，输入音频缓冲区设为DMA 通道1。
     接着调用2次audio_init_dma 函数来分别对输出和输入音频缓冲区的DMA 通道进行初始化设置。该函数比较简单，定义如下：
    static int __init audio_init_dma(audio_stream_t * s, char *desc)
    {
    if(s->dma_ch == DMA_CH2)
     return s3c2410_request_dma("I2SSDO", s->dma_ch, audio_dmaout_done_callback, NULL);
    else if(s->dma_ch == DMA_CH1)
     return s3c2410_request_dma("I2SSDI", s->dma_ch, NULL ,audio_dmain_done_callback);
    else
     return 1;
    } 这个函数其实就是对DMA 的通道号进行判断，然后调用了s3c2410_request_dma 函数来向内核申请一个DMA 通道。 在/kernel/arch/arm/mach-s3c2410/dma.c 文件中：
    int s3c2410_request_dma(const char *device_id, dmach_t channel,
     dma_callback_t write_cb, dma_callback_t read_cb) 在该函数中会分配DMA 通道，并申请DMA 中断，即当DMA 传输结束时，会响应中断请求，调用回调函数。这里的参数中，device_id 为设备id 号，用字符串来表示；channel 为DMA 通道号，将前面定义的通道号1，2传入；write_cb 和read_cb 分别指向DMA 发送和读取结束时调用的函数，即DMA 传输结束时调用的回调函数。
    在该函数中有： 
    err = request_irq(dma->irq, dma_irq_handler, 0 * SA_INTERRUPT,
     device_id, (void *)dma); 即申请了一个DMA 的中断号，中断处理子程序为dma_irq_handler 函数，然后：
    dma->write.callback = write_cb;
    dma->read.callback = read_cb; 将读写DMA 中断的两个回调函数指针传入。 在/kernel/arch/arm/mach-s3c2410/dma.c 文件中：
    static void dma_irq_handler(int irq, void *dev_id, struct pt_regs *regs)
    {
    s3c2410_dma_t *dma = (s3c2410_dma_t *)dev_id; DPRINTK(__FUNCTION__"\n"); s3c2410_dma_done(dma);
    } 在中断处理子程序中，调用了s3c2410_dma_done 函数，该函数定义如下：
    static inline void s3c2410_dma_done(s3c2410_dma_t *dma)
    {
    dma_buf_t *buf = dma->curr;
    dma_callback_t callback; if (buf->write) callback = dma->write.callback;
    else callback = dma->read.callback; #ifdef HOOK_LOST_INT
    stop_dma_timer();
    #endif
    DPRINTK("IRQ: b=%#x st=%ld\n", (int)buf->id, (long)dma->regs->DSTAT);
    if (callback)
     callback(buf->id, buf->size);
    kfree(buf);
    dma->active = 0;
    process_dma(dma);
    } 最后在s3c2410_dma_done 函数中，通过callback 函数指针调用了DMA 发送和读取的回调函数。 DMA 写入和读取的两个回调函数audio_dmaout_done_callback，audio_dmain_done_callback 会在后面说明。其中DMA 写入为音频输出，DMA 读取为音频输入。
     在调用audio_init_dma 函数来对输出和输入音频缓冲区的DMA 通道进行初始化设置时，如果返回失败，则会调用audio_clear_dma 函数来释放已申请的DMA 通道。在audio_clear_dma 函数中直接调用了s3c2410_free_dma 函数来进行动作。 在/kernel/arch/arm/mach-s3c2410/dma.c 文件中：
    void s3c2410_free_dma(dmach_t channel) 该函数中释放了已申请的DMA 通道，并调用了free_irq 函数来释放已分配的DMA 发送和读取结束的中断号。 audio_dev_dsp = register_sound_dsp(&smdk2410_audio_fops, -1);
    audio_dev_mixer = register_sound_mixer(&smdk2410_mixer_fops, -1); 在驱动模块的初始化函数最后调用了register_sound_dsp，和register_sound_mixer 两个函数来分别注册驱动设备，前者注册为DSP 设备，后者注册为混频器设备。 在/kernel/drivers/sound/sound_core.c 文件中：
    /**
    * register_sound_dsp - register a DSP device
    * @fops: File operations for the driver
    * @dev: Unit number to allocate
    *
    * Allocate a DSP device. Unit is the number of the DSP requested.
    * Pass -1 to request the next free DSP unit. On success the allocated
    * number is returned, on failure a negative error code is returned.
    *
    * This function allocates both the audio and dsp device entries together
    * and will always allocate them as a matching pair - eg dsp3/audio3
    */ int register_sound_dsp(struct file_operations *fops, int dev) /**
    * register_sound_mixer - register a mixer device
    * @fops: File operations for the driver
    * @dev: Unit number to allocate
    *
    * Allocate a mixer device. Unit is the number of the mixer requested.
    * Pass -1 to request the next free mixer unit. On success the allocated
    * number is returned, on failure a negative error code is returned.
    */ int register_sound_mixer(struct file_operations *fops, int dev) 这两个函数的参数一样，fops 为传给内核的file_operations 结构中的接口函数，dev 为分配的设备序号，设为-1 表示由内核自动分配一个空闲的序号。
    ------------------------------------------------------------------------
     紧接着就来看一下init_uda1341 这个初始化UDA1341 芯片的函数：
    static void init_uda1341(void) uda1341_volume = 62 - ((DEF_VOLUME * 61) / 100);
    uda1341_boost = 0;
     uda_sampling = #2_DEEMP_NONE;
    uda_sampling &= ~(#2_MUTE); 首先上来就是设定几个待会儿配置要用的参数。参考UDA1341 芯片#sheet 后，可以知道uda1341_volume 参数的含义，62 表示音量设置表中有效音量的总档数，61 表示音量总共有61 档，DEF_VOLUME％表示所要调的音量的百分比大小，这样61*DEF_VOLUME％所得出的就是所要调的音量是音量总档数的第几档，由于音量设置表中列出值的是按衰减量递增的，所以刚才得到的音量档数需要在总档数下衰减多少才能得到呢？显然只要将音量总档数减去所要调到的音量档数即可，即62-61*DEF_VOLUME％。
 local_irq_save(flags); 同先前一样，调用该宏函数来保存IRQ 中断使能状态，并禁止IRQ 中断。 write_gpio_bit(GPIO_L3MODE, 1);
    write_gpio_bit(GPIO_L3CLOCK, 1); 调用write_gpio_bit 宏函数，将GPIO 相应的引脚设为高电平或低电平。这里是把GPIO_L3MODE 和GPIO_L3CLOCK 这两个引脚设为高电平。 local_irq_restore(flags); 同先前一样，调用该宏函数来恢复IRQ 和FIQ 的中断使能状态。
    //*******************************************************
    //* 2007.7.6
    //******************************************************* uda1341_l3_address(UDA1341_REG_STATUS);
     uda1341_l3_#(STAT0_SC_384FS | STAT0_IF_MSB); // set 384 system clock, MSB
     uda1341_l3_#(STAT1 | STAT1_DAC_GAIN | STAT1_ADC_GAIN | STAT1_ADC_ON | STAT1_DAC_ON); 下面就调用了uda1341_l3_address 函数和uda1341_l3_# 函数来对UDA1341 芯片进行配置。在看了UDA1341 芯片的#sheet 后知道了，原来S3C2410 与UDA1341 的通信就是通过L3CLOCK，L3#，L3MODE 这3个引脚，通信时序由GPIO 口编程控制，有点类似于SPI 接口时序。这两个函数会在后面进行说明。
     其中uda1341_l3_address 函数是L3 接口操作模式的地址模式，这里用00010110（二进制）（参考了UDA1341 芯片的#sheet 得知D7～D2 为设备地址，默认UDA1341TS 的设备地址为000101，而D1～D0 为数据传输的类型）参数设置为寄存器状态地址。uda1341_l3_# 函数是L3 接口操作模式的数据传输模式，这里先用00011000（二进制）参数将系统时钟设置为384fs，数据输入格式设置为MSB 模式，然后用11100011（二进制）参数将DAC
 和ADC 的获取开关都设为6dB，将DAC 和ADC 电源控制都设为打开。 uda1341_l3_address(UDA1341_REG_#0);
    uda1341_l3_#(#0 |#0_VOLUME(uda1341_volume)); // maximum volume
    uda1341_l3_#(#1 |#1_BASS(uda1341_boost)| #1_TREBLE(0));
     uda1341_l3_#(uda_sampling); /* --;;*/ 
    uda1341_l3_#(EXTADDR(EXT2));
    uda1341_l3_#(EXT#(EXT2_MIC_GAIN(0x6)) | EXT2_MIXMODE_CH1); 再次调用uda1341_l3_address 函数，用00010100（二进制）参数设置为直接地址寄存器模式。接着分5次调用uda1341_l3_# 函数来进行配置，第一次用uda1341_volume 参数的值23（十进制）将音量大小设置为总音量的65％；第二次用01000000（二进制）参数将低音推进设置为0，高音设置为0；第三次用00000000（二进制）参数又将音量调到衰减0dB，即调到最大（不理解为什么）；最后两次要一起看，先用11000010（二进制）参数将EA2～EA0
 设为010（二进制）进入设置特定功能的外部地址，然后用11111001（二进制）参数将ED4～ED0 设为11001（二进制）将MIC 的灵敏度设为+27dB，将混频器模式设为选择通道1输入（这时通道2输入关闭）。
     【其实这里的“uda1341_l3_#(uda_sampling); /* --;;*/”，这句话应该是不正确的，不是准备再将音量调到最大。应该改为：
    uda_l3_#(#2 | uda_sampling);
    即用10000000（二进制）参数设置静音关闭和高低音模式为flat 模式（高低音增益都为0dB）等。】 
    ------------------------------------------------------------------------
     马上来看一下uda1341_l3_address 和uda1341_l3_# 这两个具体控制GPIO 口时序来传输数据的函数。首先看uda1341_l3_address 函数：
    static void uda1341_l3_address(u8 #) local_irq_save(flags); 在对GPIO 口设置或操作前总要先调用该宏函数来保存IRQ 中断使能状态，并禁止IRQ 中断。 write_gpio_bit(GPIO_L3MODE, 0);
    write_gpio_bit(GPIO_L3#, 0);
    write_gpio_bit(GPIO_L3CLOCK, 1); 分别将GPIO_L3MODE 引脚设为低电平，将GPIO_L3# 引脚设为低电平，将GPIO_L3CLOCK 引脚设为高电平。根据UDA1341 芯片#sheet 里的时序图，把GPIO_L3MODE 引脚设为低电平，就是地址模式。 udelay(1); 调用udelay 函数来短暂延时1us。在驱动程序中用udelay 函数来延时微秒级时间，mdelay 函数来延时毫秒级时间，而在应用程序中用usleep 函数来延时微秒级时间，sleep
 函数来延时毫秒级时间。 在/kernel/include/asm-arm/delay.h 文件中：
    /*
    * division by multiplication: you don't have to worry about
    * loss of precision.
    *
    * Use only for very small delays ( linux/delay.h 文件中：
    #ifdef notdef
    #define mdelay(n) (\
    {unsigned long msec=(n); while (msec--) udelay(1000);})
    #else
    #define mdelay(n) (\
    (__builtin_constant_p(n) && (n)>= 1;
    } 接下来就是将一个字节一位一位通过GPIO 口发送出去的循环结构，从该字节的最低位（D0）开始发送。若D0 为1，则设置GPIO_L3# 引脚为高电平，否则为低电平。同时需要控制GPIO_L3CLOCK 引脚的时钟信号，数据会在时钟的上升沿写入UDA1341 芯片，所以需要在时钟引脚为低电平时准备好要传送的数据，然后再将时钟设为高电平。在设置时钟和数据引脚之间用udelay 函数进行短暂延时1us。 write_gpio_bit(GPIO_L3MODE, 1);
    udelay(1); 在地址模式下数据传送完成后，则设置GPIO_L3MODE 引脚为高电平，准备进入数据传输模式，并短暂延时1us。 local_irq_restore(flags); 最后调用该宏函数来恢复IRQ 和FIQ 的中断使能状态。
    ------------------------------------------------------------------------
     接着来看uda1341_l3_# 函数：
    static void uda1341_l3_#(u8 #) local_irq_save(flags); 同样首先要调用该宏函数来保存IRQ 中断使能状态，并禁止IRQ 中断。 write_gpio_bit(GPIO_L3MODE, 1);
    udelay(1); write_gpio_bit(GPIO_L3MODE, 0);
    udelay(1);
    write_gpio_bit(GPIO_L3MODE, 1); 在进入数据传输模式前，先要将GPIO_L3MODE 信号需要有一个低电平的脉冲，所以依次设置该引脚为高电平，低电平，高电平，这样就进入了数据传输模式。 for (i = 0; i >= 1;
    } 接下来的这个步骤和uda1341_l3_address 函数一样，一位一位将数据发送出去。 write_gpio_bit(GPIO_L3MODE, 1);
    write_gpio_bit(GPIO_L3MODE, 0);
    udelay(1);
    write_gpio_bit(GPIO_L3MODE, 1); 最后，GPIO_L3MODE 信号同样需要有一个低电平的脉冲才表示数据传输模式结束，所以再次设置该引脚为高电平，低电平，高电平。 local_irq_restore(flags); 完成后，调用该宏函数来恢复IRQ 和FIQ 的中断使能状态。
    ------------------------------------------------------------------------
     看一下卸载驱动模块时调用的函数：
    void __exit s3c2410_uda1341_exit(void) 这个函数就比较简单了。 unregister_sound_dsp(audio_dev_dsp);
    unregister_sound_mixer(audio_dev_mixer); 首先调用unregister_sound_dsp 和unregister_sound_mixer 这两个函数来分别注销原先注册的DSP 设备和混频器设备。 在/kernel/drivers/sound/sound_core.c 文件中：
    /**
    * unregister_sound_dsp - unregister a DSP device
    * @unit: unit number to allocate
    *
    * Release a sound device that was allocated with register_sound_dsp().
    * The unit passed is the return value from the register function.
    *
    * Both of the allocated units are released together automatically.
    */ void unregister_sound_dsp(int unit) /**
    * unregister_sound_mixer - unregister a mixer
    * @unit: unit number to allocate
    *
    * Release a sound device that was allocated with register_sound_mixer().
    * The unit passed is the return value from the register function.
    */ void unregister_sound_mixer(int unit) 这两个函数的参数一样，为刚才调用注册函数时返回的内核所分配的设备序号。 audio_clear_dma(&output_stream);
    audio_clear_dma(&input_stream); /* input */ 分两次调用audio_clear_dma 函数来分别释放已申请的音频输入和音频输出的DMA 通道。
    ------------------------------------------------------------------------
     继续来看一下打开设备文件的接口函数，这里先看针对DSP 设备文件的函数：
    static int smdk2410_audio_open(struct inode *inode, struct file *file) if ((file->f_flags & O_ACCMODE) == O_RDONLY) {
     if (audio_rd_refcount || audio_wr_refcount)
     return -EBUSY;
     audio_rd_refcount++;
    } else if ((file->f_flags & O_ACCMODE) == O_WRONLY) {
     if (audio_wr_refcount)
     return -EBUSY;
     audio_wr_refcount++;
    } else if ((file->f_flags & O_ACCMODE) == O_RDWR) {
     if (audio_rd_refcount || audio_wr_refcount)
     return -EBUSY;
     audio_rd_refcount++;
     audio_wr_refcount++;
    } else
     return -EINVAL; 首先上来就是一大段条件判断，主要就是判断file->f_flags 这个表示设备文件的打开方式是读取，写入，还是可读写。用audio_rd_refcount 和audio_wr_refcount 这两个变量来设置类似于信号量一样的读写占位标志（要写设备的话，只要没有用写方式打开过设备即可；要读的话，则需要该设备同时没有用读或写方式打开过），只要打开过设备文件，相应的方式标志就会加一。 if (cold) {
     audio_rate = AUDIO_RATE_DEFAULT;
     audio_channels = AUDIO_CHANNELS_DEFAULT;
     audio_fragsize = AUDIO_FRAGSIZE_DEFAULT;
     audio_nbfrags = AUDIO_NBFRAGS_DEFAULT; 在audio_rd_refcount 和audio_wr_refcount 这两个变量都为0 的时候才进入这一步，即对已经打开过的设备文件不进行下面的操作。
     这里先设置一下待会儿要配置到IIS 相关寄存器中的变量。 if ((file->f_mode & FMODE_WRITE)){
     init_s3c2410_iis_bus_tx();
     audio_clear_buf(&output_stream);
     }
     if ((file->f_mode & FMODE_READ)){
     init_s3c2410_iis_bus_rx();
     audio_clear_buf(&input_stream);
     }
    } 从file->f_mode 中判断文件是否可读可写，根据设备文件的打开模式，分别调用了init_s3c2410_iis_bus_tx 和init_s3c2410_iis_bus_rx 函数来进行对IIS 总线读写的初始化配置，在这两个函数中对S3C2410 芯片的IIS 相关寄存器进行了相应的配置，会在后面说明。然后又调用了audio_clear_buf 函数来分别对音频输入和输出两个DMA 缓冲区进行了清空，该函数也会在后面说明。
     因为读写操作控制必须用f_mode 来进行判断，所以这里要根据f_mode 为可读或可写的标识来进行读写模式的硬件设置。而read，write 函数不需要检查f_mode 因为读写权限的检查是由内核在调用他们之前进行的。 MOD_INC_USE_COUNT; 最后调用MOD_INC_USE_COUNT; 来对设备文件计数器加一计数，并返回。
    ------------------------------------------------------------------------
     下面马上来看一下init_s3c2410_iis_bus_tx 和init_s3c2410_iis_bus_rx 这两个函数，首先是init_s3c2410_iis_bus_tx 函数：
    static void init_s3c2410_iis_bus_tx(void) IISCON = 0;
     IISMOD = 0;
     IISFIFOC = 0; 首先初始化IIS 控制寄存器，IIS 模式寄存器和IIS FIFO 控制寄存器都为0。 /* 44 KHz , 384fs */
    IISPSR = (IISPSR_A(iispsr_value(S_CLOCK_FREQ, 44100)) 
     | IISPSR_B(iispsr_value(S_CLOCK_FREQ, 44100))); 设置IIS 预分频寄存器，其中调用了iispsr_value 函数来计算预分频值，该函数会在后面说明。
     参考S3C2410 芯片#sheet 中关于IIS 总线接口的章节，具体设置参数如下：
    IISPSR_A(iispsr_value(S_CLOCK_FREQ, 44100)) = IISPSR_A(iispsr_value(384, 44100)) = (一个0～31 之间的值)
    //*******************************************************
    //* 2007.7.9
    //*******************************************************
    IISCON = (IISCON_TX_DMA /* Transmit DMA service request */
     |IISCON_RX_IDLE /* Receive Channel idle */
     |IISCON_PRESCALE); /* IIS Prescaler Enable */ 设置IIS 控制寄存器，参考S3C2410 芯片#sheet 中关于IIS 总线接口的章节，具体设置参数如下：
    IISCON_TX_DMA = 1命令，设为1 表示接收通道空闲
    IISCON_PRESCALE = 1系统输出时钟为384fs 和音频采样频率fs为44.1KHz 的情况下，所需要的预分频值，并返回。
    ------------------------------------------------------------------------
     接下来init_s3c2410_iis_bus_rx 函数与前面的init_s3c2410_iis_bus_tx 函数形式上也差不多：
    static void init_s3c2410_iis_bus_rx(void)
    IISCON = 0;
     IISMOD = 0;
     IISFIFOC = 0;
     首先初始化IIS 控制寄存器，IIS 模式寄存器和IIS FIFO 控制寄存器都为0。
     /* 44 KHz , 384fs */
     IISPSR = (IISPSR_A(iispsr_value(S_CLOCK_FREQ, 44100))
     | IISPSR_B(iispsr_value(S_CLOCK_FREQ, 44100)));
     设置IIS 预分频寄存器，参考S3C2410 芯片#sheet 中关于IIS 总线接口的章节，具体设置参数如下：
    IISPSR_A(iispsr_value(S_CLOCK_FREQ, 44100)) = IISPSR_A(iispsr_value(384, 44100)) = (一个0～31 之间的值)dma_ch);
     调用该函数来刷新所指定的DMA 通道缓冲区。
    在/kernel/arch/arm/mach-s3c2410/dma.c 文件中：
    int s3c2410_dma_flush_all(dmach_t channel)
    这个函数会释放所指定的DMA 通道对应的内存缓冲区。
    if (s->buffers) {
     int frag;
     for (frag = 0; frag nbfrags; frag++) {
     if (!s->buffers[frag].master)
     continue;
     consistent_free(s->buffers[frag].start,
     s->buffers[frag].master,
     s->buffers[frag].dma_addr);
     }
     kfree(s->buffers);
     s->buffers = NULL;
    }
     接下来判断，如果环形缓冲区不为空，通过调用consistent_free 函数来释放环形缓冲区中的s->nbfrags 个buffer 所分配的内存空间，其中s->buffers[frag].master 表示buffer 所分配的内存大小。最后调用kfree 函数，将整个s->buffers 指针所指的已分配的内存释放掉，并将它设为空指针。
    在/kernel/arch/arm/mm/consistent.c 文件中：
    /*
    * free a page as defined by the above mapping. We expressly forbid
    * calling this from interrupt context.
    */
    void consistent_free(void *vaddr, size_t size, dma_addr_t handle)
    该函数的参数vaddr 为指向内存虚拟地址起始地址的指针，size 为要释放的内存大小，handle 为所分配的内存物理地址的起始地址。
    s->buf_idx = 0;
    s->buf = NULL;
     最后将环形缓冲区buffer 索引号和当前buf 指针都清空，返回。
    ------------------------------------------------------------------------
     下面来看一下，DMA 写入和读取的两个回调函数audio_dmaout_done_callback，audio_dmain_done_callback，当DMA 写入或读取完成就会产生中断，并调用这两个中断处理函数。在分析这两个函数之前，需要重新了解一下这两个函数被调用的过程以及传入参数的意义。
     从前面对申请DMA 通道函数的分析中，可以知道DMA 写入和读取的中断处理函数是在s3c2410_dma_done 函数中被调用的，而s3c2410_dma_done 函数又是在真正的DMA 中断处理函数dma_irq_handler 中被调用的。
    在/kernel/arch/arm/mach-s3c2410/dma.c 文件中：
    static void dma_irq_handler(int irq, void *dev_id, struct pt_regs *regs)
    {
    s3c2410_dma_t *dma = (s3c2410_dma_t *)dev_id;
    DPRINTK(__FUNCTION__"\n");
    s3c2410_dma_done(dma);
    }
    在该函数中，首先定义了一个s3c2410_dma_t 结构的指针变量指向中断处理程序的参数dev_id，然后将它再作为参数传入s3c2410_dma_done 函数中。
     接着在s3c2410_dma_done 函数中做如下操作：
    static inline void s3c2410_dma_done(s3c2410_dma_t *dma)
    {
    dma_buf_t *buf = dma->curr;
    dma_callback_t callback;
    if (buf->write) callback = dma->write.callback;
    else callback = dma->read.callback;
    #ifdef HOOK_LOST_INT
    stop_dma_timer();
    #endif
    DPRINTK("IRQ: b=%#x st=%ld\n", (int)buf->id, (long)dma->regs->DSTAT);
    if (callback)
     callback(buf->id, buf->size);
    kfree(buf);
    dma->active = 0;
    process_dma(dma);
    }
    在该函数中又定义了一个dma_buf_t 结构的指针变量，指向了参数中的dma->curr，即指向当前DMA 缓冲区的指针。
    在/kernel/arch/arm/mach-s3c2410/dma.h 文件中：
    /* DMA buffer struct */
    typedef struct dma_buf_s {
    int size; /* buffer size */
    dma_addr_t dma_start; /* starting DMA address */
    int ref; /* number of DMA references */
    void *id; /* to identify buffer from outside */
    int write; /* 1: buf to write , 0: but to read */
    struct dma_buf_s *next; /* next buf to process */
    } dma_buf_t;
    /* DMA channel structure */
    typedef struct {
    dmach_t channel;
    unsigned int in_use; /* Device is allocated */
    const char *device_id; /* Device name */
    dma_buf_t *head; /* where to insert buffers */
    dma_buf_t *tail; /* where to remove buffers */
    dma_buf_t *curr; /* buffer currently DMA'ed */
    unsigned long queue_count; /* number of buffers in the queue */
    int active; /* 1 if DMA is actually processing # */
    dma_regs_t *regs; /* points to appropriate DMA registers */
    int irq; /* IRQ used by the channel */
    dma_device_t write; /* to write */
    dma_device_t read; /* to read */
    } s3c2410_dma_t;
     然后根据buf->write 这个DMA 读写标志来对callback 函数指针进行设置，是指向写DMA 函数dma->write.callback，还是读DMA 函数dma->read.callback。最后在调用该函数指针所指的函数时将buf->id，buf->size 这两个值作为参数传入，即是原来定义在dma_irq_handler 函数中的dma 变量的dma->curr->id 和dma->curr->size，分别表示当前DMA 缓冲区的id 号和缓冲区大小。
     现在可以先来看一下DMA 写入中断处理函数audio_dmaout_done_callback：
    static void audio_dmaout_done_callback(void *buf_id, int size)
    audio_buf_t *b = (audio_buf_t *) buf_id;
     在该函数中首先就定义了一个audio_buf_t 结构的指针变量，并指向传入的参数。
