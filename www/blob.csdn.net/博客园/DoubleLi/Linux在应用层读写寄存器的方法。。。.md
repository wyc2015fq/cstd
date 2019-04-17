# Linux在应用层读写寄存器的方法。。。 - DoubleLi - 博客园






参考链接：[http://blog.csdn.net/liukang325/article/details/26601811](http://blog.csdn.net/liukang325/article/details/26601811)

/dev/mem:物理内存的全镜像。可以用来访问物理内存。由于应用运行都在用户空间，使用的是虚拟内存，不能直接访问物理地址空间，通过/dev/mem文件可以用来访问系统的全部寻址空间。

/dev/kmem:kernel看到的虚拟内存的全镜像。可以用来访问kernel的内容。

在开启MMU之前，你可以直接用物理地址访问。在开启MMU之后，你需要先将物理地址映射为虚拟地址，然后用虚拟地址访问。还是绝个简单的例子，比如某一个外设的寄存器物理地址为:

regaddr = 0x0200000，要读出/写入的值为value（unsigned int类型）；那么： 

  ①、在开启MMU之前： 

  读: 

  value = *(volatile unsigned int*)regaddr; 

  写： 

  *(volatile unsigned int*)regaddr = value; 

  ②、在开启MMU之后： 

  需要先用ioremap来做映射，如下 

  unsigned int virt_regaddr = 0; 

  virt_regaddr = ioremap (regaddr, sizeof (unsigned int)); 

  读： 

  value = *(volatile unsigned int *)virt_regaddr; 

  写： 

  *(volatile unsigned int*)virt_regaddr = value;  

  当然，如果不使用了，记得用iounmap()来回收资源。









