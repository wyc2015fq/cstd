# misc_register函数 - h13 - 博客园
杂项设备（misc device）
杂项设备也是在嵌入式系统中用得比较多的一种设备驱动。在 Linux 内核的include\linux目录下有Miscdevice.h文件，要把自己定义的misc device从设备定义在这里。其实是因为这些字符设备不符合预先确定的字符设备范畴，所有这些设备采用主编号10 ，一起归于misc device，其实misc_register就是用主标号10调用register_chrdev()的。 
也就是说，misc设备其实也就是特殊的字符设备，可自动生成设备节点。 
字符设备(char device) 
使用register_chrdev(LED_MAJOR,DEVICE_NAME,&dev_fops)注册字符设备驱动程序时，如果有多个设备使用该函数注册驱动程序，LED_MAJOR不能相同，否则几个设备都无法注册(我已验证)。如果模块使用该方式注册并且 LED_MAJOR为0(自动分配主设备号 )，使用insmod命令加载模块时会在终端显示分配的主设备号和次设备号，在/dev目录下建立该节点，比如设备leds，如果加载该模块时分配的主设备号和次设备号为253和0，则建立节点:mknod leds c 253 0。使用register_chrdev (LED_MAJOR,DEVICE_NAME,&dev_fops)注册字符设备驱动程序时都要手动建立节点 ，否则在应用程序无法打开该设备。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/jemmy858585/archive/2010/11/21/6025148.aspx](http://blog.csdn.net/jemmy858585/archive/2010/11/21/6025148.aspx)
