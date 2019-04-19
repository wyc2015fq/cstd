# linux驱动中地址空间转换 - maopig的专栏 - CSDN博客
2017年01月19日 14:42:59[maopig](https://me.csdn.net/maopig)阅读数：241

在[Linux](http://lib.csdn.net/base/linux) kernel 中，物理地址是不能直接使用的，必须通过转换才可以。
转换分为两种， 静态和动态。
静态就是下面那种，不过，静态的地址转换，还需要在kernel 初始化的时候作映射。  
动态映射是使用 ioremap 函数 。
**[cpp]**[view
 plain](http://blog.csdn.net/cjsycyl/article/details/9212309#)[copy](http://blog.csdn.net/cjsycyl/article/details/9212309#)
- #define SL2312_GPIO_BASE  0x4D000000  
- 
- #define IO_ADDRESS(x)  (((X)&0x0fffffff)+(((x)>>4)&0x0f000000)+0xf0000000)   
- 
- #define GEMINI_GPIO_BASE1  IO_ADDRESS(SL2312_GPIO_BASE)   
linux 有内存保护机制，无论是内核态还是用户态。  
静态映射 ：是在物理地址上加一个偏移量， 所以虚拟地址是固定的。  
动态映射 ：系统会生成一个虚拟地址对应到物理地址上，虚拟地址不是固定的。
linux在初始化运行时会根据一个map_desc的数组中的物理地址进行静态映射，0x4D000000地址应该就是这样映射的。
静态映射的地址无须使用icoremap函数，只要知道在map_desc数组中物理地址与虚拟地址的移仿就可以了。  
至于驱动程序中关于物理地址是不是都要转换呢，这个是当然的，linux是运行在虚拟地址的。
任何程序不能访问物理地址，都必须转换为物理地址才能访问。只不过，在内核空间已经是转换好了的（如前所述），可以随便的访问。 
动态映射是由Linux内核统一管理，当使用kmalloc（分配在内核空间）和malloc函数申请时，内核会找到一块空余的物理内存，并在页表中动态地添加一项其与虚拟地址对应的表项。 
静态映射那应该就是LINUX在初始化阶段读取map_desc数组进行的地址映射，map_desc中包括了一对对的物理地址和虚拟地址的对应关系，这个可以根据硬件修改，其中的虚拟地址必须是内核空间的，例如，SDRAM的物理是0x30000000 那么可以将其映射为0xe0000000. 通过静态映射后的地址一般之后就不会更改了的，除非你想办法改掉。
注：以上均摘自论坛，仅供参考，不对的请指正，谢谢
