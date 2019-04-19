# AVR BOOTLOADER 实例开发笔记 - xqhrs232的专栏 - CSDN博客
2019年03月27日 16:19:47[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：38
个人分类：[ARM/STM32/LPC17/单片机技术](https://blog.csdn.net/xqhrs232/article/category/939440)
原文地址::[https://blog.csdn.net/knowmuch/article/details/38367663](https://blog.csdn.net/knowmuch/article/details/38367663)
相关文章
1、AVR之BOOTLOADER技术详解----[https://blog.csdn.net/zhenhua10/article/details/6442412](https://blog.csdn.net/zhenhua10/article/details/6442412)
2、ATMEL系列MCU在线升级bootloader固件源码及操作过程----[http://www.51hei.com/bbs/dpj-55428-1.html](http://www.51hei.com/bbs/dpj-55428-1.html)
3、AVR的bootloader的设置步骤教程及注意事项----[http://m.elecfans.com/article/680055.html](http://m.elecfans.com/article/680055.html)
4、AVR通用Bootloader折腾记----[http://www.eeworld.com.cn/mcu/article_2017020633654.html](http://www.eeworld.com.cn/mcu/article_2017020633654.html)
5、avr bootloader学习----[https://blog.csdn.net/wandersky0822/article/details/79416971](https://blog.csdn.net/wandersky0822/article/details/79416971)
6、AVR通用Bootloader----[https://blog.csdn.net/ppp2006/article/details/24982713](https://blog.csdn.net/ppp2006/article/details/24982713)
7、AVR的bootloader的设置----[https://blog.csdn.net/qing000/article/details/78930222](https://blog.csdn.net/qing000/article/details/78930222)
AVR BOOTLOADER 实例开发笔记
写了一个基于Atmega8 的Bootloader应用实例，其间遇到了一些问题，下面把过程和问题叙述如下，供朋友们参考。工程实例的代码在http://download.csdn.net/detail/knowmuch/7712209，也包含了一个C#的配套烧录程序，欢迎有兴趣的朋友们下载。
1、 准备一个目标程序（被加载的应用程序），一般环境编译生成的结果是Hex文件，而写进Flash的是Bin文件。二者转换其实很简单，去掉Hex中的类型、长度、校验等，只留下数据依次排列就好了。
a)        Hex文件格式：以ASCII格式记录，“：”表示一行开始，第1、2个字符表示此行数据长度。第3-6字符表示数据存储的起始地址，第7、8字符表示类型，“00”数据记录、“01”文件结束、“02”扩展段地址、“04”扩展线性地址，其他格式这里不涉及，不作详述。第9字符开始为数据。最后两个字符是效验码（与前面的值相加为0）。比如说，每个文件的最后一行肯定是：00 0000 01 FF。
b)       Bin文件格式：没有其他信息，只有要烧录的命令与操作数。
c)       先写一个很小的闪灯程序，编译出Hex，再把Bin提取出来。
d)       目标程序大小不要超过RWW区大小，BootLoad程序不要超过NRWW区大小。呵呵，废话，这是自然啦。
2、 程序代码定位，Bootloader程序是运行在NRWW区（也就是BootLoader区）的，这个区的大小和首地址通过熔丝设置（别忘了把复位指针也定向到NRWW区开始）。设置后，要把Bootloader程序的开始也定位到那里。
a)        在avrstudio项目设置的Memory setting中添加一个段(段名记住要加个点在前面)。地址是bootloader首地址的一半(这是由于计算单位不同，一个是字节、一个是字)。也可以用保留段名“.text”,这样所有的内容都将默认到text段去。
b)       代码地址指定：在函数声明之前加一个宏指令，比如:
__attribute__((section(".BootLoad")))void ABCfuntion（void）{……}。
3、 BootLoader程序本身倒比较简单，就是通过数据接口（一般是串口，其实也可以是其它接口，比如有些AVRUSB系统）把bin文件传进来，再用SPM指令把数据写进FLASH中，最后跳转到应用程序开始的地址去执行就行了。我在网上看到的示例都实现了Xmodem传输控制，这样的目的一是保证数据无误，二是不用开发烧录软件。其实烧录软件也很简单的，校验也可以用其它方式（比如回传）实现。为了更好的说明BootLoader机制本身，这里没有进行校验，传输控制也很简单，就是一问一答式的一页一页（Atmega8是64字节）传。实验证明这样的差错率实际上也很小，不过这和你的接口电路等等也有关系。
4、 FLASH的操作，WinAvr提供了一组宏，在boot.h文件中。这组宏其实是将内联汇编嵌入到代码中。比如说下面就是向Flash中写入一页数据的宏定义，仔细看看也挺好理解的（内联汇编的问题，这里暂不涉及，有兴趣可以参考一下WinAVR\doc\avr-libc\avr-libc-user-manual目录下的inline_asm.html.html）：
5、 数据写入Flash的过程就是上述宏的一系列调用，这些宏的名称很直观。当然也可以自己用汇编来实现咯，有兴趣的话：
a)        把要写的页擦干净。使用boot_page_erase.
b)       把这一页的数据写入缓冲区，使用boot_page_fill，注意一次是写一个字。
c)       写满缓冲区后，使用boot_page_write，就可以将数据写入Flash了。
d)       操作过程中，要确保操作完成在进行下面的操作，使用boot_spm_busy_wait。
e)        下面是写一页的代码。
6、 关于中断，当我们把Reset向量熔丝位设置到BootLoader时，其实所有的中断向量都被设置到了BootLoader开始，要在应用程序中正常使用中断，就必须在跳转到应用程序之前把IVSEL位设置为0。
不同的型号这一位在不同的寄存器，查下手册就知道了。AVR对于一些关键的位有种保护机制，就是写这一位之前，必须先写修改使能位（对于IVSEL来说是IVCE）。写的时序是先给使能位写1，在随后的4个时钟周期内写目标位，同时使能位要写0。过了4个周期，使能位会自动置零，再写就没用了。
这和有些共用地址的寄存器选择位（比如Atmega8的UCSRC、UBRRH的URSEL位）看起来有点像，但实际上是两码事。还是要仔细看看手册。
7、 最后是跳转到目标程序的开头，Atmega8的指令集中没有jmp（绝对跳转）指令，所以使用相对跳转。
8、 上面的工作完成后，按说就可以了，如果你没有遇到下面的问题，就可以不看了，因为我感觉这个问题并不典型。
a)        突然发现有一些程序在烧录后不能正常执行，只会一直执行bootloader区，重新上电也不行。比较了烧录后的程序，完全与目标程序一致。后来发现改变某些熔丝位后可以正常执行，比如将boot指针改为0x0000后再改回BootLoader首部，程序就一直正常了。但再次烧录就又不行了。
c)       把烧录函数中的操作一个个注掉，然后手工合成了同时包含目标程序（因为注掉烧录函数的一些操作后就不能正常烧录目标程序了）和bootloader程序的hex文件，最后发现是boot_page_write的原因。因为把它注掉后，即使执行了其它操作，也不会有问题。
d)       分析了一下，应该是写Flash时改变了一些东西，没有改回来的，而熔丝位的改变可以将它改回来。
e)        最后发现与加密位有关，在写了flash后，不论加密位是什么内容，也不论往里写什么内容（当然得保证执行时CPU起码可以读取应用程序代码），只要执行一下boot_lock_bits_set，都能解决这个问题。
