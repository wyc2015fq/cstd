# 转 alsa录音放音执行流程详解 - maopig的专栏 - CSDN博客
2013年03月01日 10:09:55[maopig](https://me.csdn.net/maopig)阅读数：2362
个人分类：[音频](https://blog.csdn.net/maopig/article/category/920338)

[](http://hi.baidu.com/)

前言：
    linux中，无论是oss还是alsa体系，录音和放音的数据流必须分析清楚。先分析alsa驱动层，然后关联到alsa库层和应用层。
链接分析：
    core/pcm_native.c文件中.mmap = snd_pcm_mmap调用snd_pcm_mmap_data(substream, file, area);进一步调用substream->ops->mmap(substream, area);根据soc/pxa/pxa3xx-pcm.c文件中.mmap = pxa3xx_pcm_mmap,可知dma_mmap_writecombine(, ,runtime->dma_addr,);函数被调用。上一章分析过soc/pxa/pxa3xx-pcm.c文件中pxa3xx_pcm_hw_params()函数会创建链表，根据dma_buff_phys
 = runtime->dma_addr;和dma_desc->dsadr = dma_buff_phys;可知runtime->dma_addr为dma内存端地址，且此地址由alsa库层传递进来。又根据dma_desc->dtadr = prtd->params->dev_addr和soc/pxa/pxa3xx-ac97.c文件中.dev_addr = __PREG(PCDR)，可知dma外设端地址为ac97控制器中fifo读写寄存器PCDR。至此，第一条链路建立完毕：FIFO通过DMA和内存交互。待续1
 。
    ac97接口或者i2s或者pcm接口可以将cpu和codec(wm9714/alc5620/alc5621)连接起来，配置好格式：pcm接口必须配置采样率、采样位数、通道数和传送格式；i2s接口必须配置采样率、采样位数、通道数和对齐方式；ac97接口比较灵活，可以认为cpu这端不用配置,只需要在codec端配置就行了。当然，电源、时钟、IO任何数字芯片都得配置。最后不能混淆数据接口和控制接口的慨念，i2s和pcm只能传输音频数据，访问codec的寄存器必须通过i2c等控制接口，ac97接口分时传输控制和数据。codec中的adc、dac通过ac97等接口同cpu的fifo交互数据。第二条链路建立完毕。待续2
 。
    alsa_lib源码中pcm.c文件中snd_pcm_readi(,buffer,size)调用pcm_local.h文件中_snd_pcm_readi(,buffer,size);进一步调用pcm->fast_ops->readi(pcm->fast_op_arg, buffer, size);根据pcm_hw.h文件中.readi = snd_pcm_hw_readi可知，ioctl(fd, SNDRV_PCM_IOCTL_READI_FRAMES, &xferi);被调用。内核中，根据/soc/pcm_native.c文件中.unlocked_ioctl
 = snd_pcm_capture_ioctl,可知snd_pcm_capture_ioctl1被调用，根据SNDRV_PCM_IOCTL_READI_FRAMES参数可知snd_pcm_lib_read(substream, xferi.buf, xferi.frames);被调用，最终snd_pcm_lib_read1(,,,,snd_pcm_lib_read_transfer)被调用。根据transfer被调用可知snd_pcm_lib_read_transfer被调用，然后调用copy_to_user(buf,
 hwbuf, frames_to_bytes(runtime, frames))，可知，将dma端内存的数据拷贝到alsa_lib提供的一个指针所指的内存，alsa库函数snd_pcm_readi、snd_pcm_writei实现了内存到内存的交互，或者近似地认为是内存到音频文件的交互。至此最后一条链路建立完毕。待续3 。
执行分析：
    录音：mic接到codec，经过adc变成数字信号，经过待续2中ac97等接口存储到cpu的fifo中，经过待续1中的dma传输存储到内存，经过待续3中alsa_lib中snd_pcm_readi接口传给录音软件，经过编码，进而形成音频文件。
    放音：播放软件将音频文件解码，并通过待续3中snd_pcm_writei接口逐渐传递到和dma相关的内存，经过待续2中dma传递给cpu的fifo，再经过ac97等接口传递给dac，最后传给连接在codec上的speaker。
心得：
    1.ac97数据传输颇复杂，分时复用，cpu端fifo和codec端adc/dac关系要对应好。比如，cpu端的pcm left fifo占用slot3，那么adc只有配置成slot3才能把数据传递给它，如果配置成slot6，那就传给cpu的mic in fifo了。录音单声道通常选择slot6，录音双声道通常两个adc分别选择slot3和slot4。
    2.wav音频文件大小计算：要测试录音是否丢祯，就必然要计算文件大小，通常的方法是：根据录音时间，用公式：录音时间(单位s)x采样率x(采样位数/8)x通道数。比如，录音时间5秒，采样率8kHz，位数16位，通道数1,那么5x8000x(16/8)x1=80k，实际的wav文件大小稍大于80k就对了。还有一种计算文件大小的方法：通常音频系统要用dma，也会用到dma中断，可以在dma中断中打印计数，次数xdma中断周期字节就行了。
    3.数据交换的大小问题：待续1中DMA传输必须和FIFO的特性匹配：若FIFO位宽是16位，深度是16，并且半满时向DMA发出请求(握手)，则链表式DMA必须配置成传输位宽16位，1次突发16字节，才能保证不丢失位数和数据个数。待续2中cpu端FIFO位数要和codec端adc/dac采样位数匹配，i2s/pcm接口可以配置成一样的值，比如16位，ac97接口复杂一点，cpu端不用配置，那么采样位数是多少呢?若cpu端fifo一个声道位宽16位，codec端adc/dac位宽18位，ac97通道20位，则传输到fifo端就被截取到有效的16位，整体采样位数16位，adc/dac的性能没有充分发挥而已。待续3中snd_pcm_readi、snd_pcm_writei函数第三个参数表示读写数据的大小，单位是祯，不是字节。双声道16位格式一祯大小为4字节。
