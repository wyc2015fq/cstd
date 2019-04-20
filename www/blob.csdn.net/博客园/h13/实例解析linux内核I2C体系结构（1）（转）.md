# 实例解析linux内核I2C体系结构（1）（转） - h13 - 博客园
**　本文转自：http://www.dzsc.com/data/html/2009-12-22/81040.html**
**作者：刘洪涛,**华清远见嵌入式学院讲师。
　　一、概述
　　谈到在[Linux](http://wiki.dzsc.com/info/4181.html)系统下编写I2C驱动，目前主要有两种方式，一种是把I2C设备当作一个普通的字符设备来处理，另一种是利用linux I2C驱动体系结构来完成。下面比较下这两种驱动。
　　第一种方法的好处（对应第二种方法的劣势）有：
　　●    思路比较直接，不需要花时间去了解linux内核中复杂的I2C子系统的操作方法。
　　第一种方法问题（对应第二种方法的好处）有：
　　●    要求工程师不仅要对I2C设备的操作熟悉，而且要熟悉I2C的[适配器](http://www.dzsc.com/product/searchfile/695.html)操作；
　　●    要求工程师对I2C的设备器及I2C的设备操作方法都比较熟悉，最重要的是写出的程序可移植性差；
　　●    对内核的资源无法直接使用。因为内核提供的所有I2C设备器及设备驱动都是基于I2C子系统的格式。I2C[适配器](http://wiki.dzsc.com/info/1732.html)的操作简单还好，如果遇到复杂的I2C适配器（如：基于PCI的I2C适配器），工作量就会大很多。
　　本文针对的对象是熟悉I2C协议，并且想使用linux内核子系统的开发人员。
　　网络和一些书籍上有介绍I2C子系统的源码结构。但发现很多开发人员看 了这些文章后，还是不清楚自己究竟该做些什么。究其原因还是没弄清楚I2C子系统为我们做了些什么，以及我们怎样利用I2C子系统。本文首先要解决是如何 利用现有内核支持的I2C适配器，完成对I2C设备的操作，然后再过度到适配器代码的编写。本文主要从解决问题的角度去写，不会涉及特别详细的代码跟踪。
　　二、I2C设备驱动程序编写
　　首先要明确适配器驱动的作用是让我们能够通过它发出符合I2C标准协议的时序。
　　在Linux内核源代码中的drivers/i2c/busses目录下包含着一些适配器的驱动。如[S3C2410](http://www.dzsc.com/icstock/597/S3C2410.html)的驱动i2c-[s3c2410](http://www.dzsc.com/icstock/597/S3C2410.html).c。当适配器加载到内核后，接下来的工作就要针对具体的设备编写设备驱动了。
　　编写I2C设备驱动也有两种方法。一种是利用系统给我们提供的i2c- dev.c来实现一个i2c适配器的设备文件。然后通过在应用层操作i2c适配器来控制i2c设备。另一种是为i2c设备，独立编写一个设备驱动。注意： 在后一种情况下，是不需要使用i2c-dev.c的。
　　1、利用i2c-dev.c操作适配器，进而控制i2c设备
　　i2c-dev.c并没有针对特定的设备而设计，只是提供了通用的read()、wri[te](http://www.dzsc.com/product/searchfile/10366.html)()和ioctl()等[接口](http://www.dzsc.com/product/searchfile/5540.html)，应用层可以借用这些接口访问挂接在适配器上的i2c设备的存储空间或[寄存器](http://www.dzsc.com/product/searchfile/19403.html)，并控制I2C设备的工作方式。
　　需要特别注意的是：i2c-dev.c的read()、write()方法都只适合于如下方式的数据格式（可查看内核相关源码）
![](http://www.dzsc.com/data/uploadfile/20091222134920793.jpg)
　　图1 单开始信号时序
　　所以不具有太强的通用性，如下面这种情况就不适用（通常出现在读目标时）。
![](http://www.dzsc.com/data/uploadfile/20091222134920104.jpg)
　　图2 多开始信号时序
　　而且read()、write()方法只适用用于适配器支持i2c算法的情况，如：
[ST](http://www.dzsc.com/data/iccompany/detail3.html)a[TI](http://www.dzsc.com/data/iccompany/detail2.html)c c[ON](http://www.dzsc.com/data/iccompany/detail12.html)st struct i2c_algorithm s3c24xx_i2c_algorithm = {
　　.master_xfer = s3c24xx_i2c_xfer,
　　.functionality = s3c24xx_i2c_func,
　　};
　　而不适合适配器只支持smbus算法的情况，如：
　　static const struct i2c_algorithm smbus_algorithm = {
　　.smbus_xfer = i801_access,
　　.functionality = i801_func,
　　};
　　基于上面几个原因，所以一般都不会使用i2c-dev.c的read()、write()方法。最常用的是ioctl()方法。ioctl()方法可以实现上面所有的情况（两种数据格式、以及I2C算法和smbus算法）。
　　针对i2c的算法，需要熟悉struct i2c_rdwr_ioctl_data 、struct i2c_msg。使用的命令是I2C_RDWR。
　　struct i2c_rdwr_ioctl_data {
　　struct i2c_msg __user *msgs; /* pointers to i2c_msgs */
　　__u32 nmsgs; /* [number](http://www.dzsc.com/icstock/207/NUMBER.html) of i2c_msgs */
　　};
　　struct i2c_msg {
　　_ _u16 addr; /* slave address */
　　_ _u16 flags; /* 标志（读、写） */
　　_ _u16 len; /* msg length */
　　_ _u8 *buf; /* pointer to msg data */
　　};
　　针对smbus算法，需要熟悉struct i2c_smbus_ioctl_data。使用的命令是I2C_SMBUS。对于smbus算法，不需要考虑“多开始信号时序”问题。
　　struct i2c_smbus_ioctl_data {
　　__u8 read_write; //读、写
　　__u8 command; //命令
　　__u32 size; //数据长度标识
　　union i2c_smbus_data __user *data; //数据
　　};
　　下面以一个实例讲解操作的具体过程。通过S3C2410操作[AT24C02](http://www.dzsc.com/stock-ic/AT24C02.html) e2prom。实现在AT24C02中任意位置的读、写功能。
　　首先在内核中已经包含了对s3c2410 中的i2c[控制器](http://www.dzsc.com/product/searchfile/568.html)驱动的支持。提供了i2c算法（非smbus类型的，所以后面的ioctl的命令是I2C_RDWR）
　　static const struct i2c_algorithm s3c24xx_i2c_algorithm = {
　　.master_xfer = s3c24xx_i2c_xfer,
　　.functionality = s3c24xx_i2c_func,
　　};
　　另外一方面需要确定为了实现对AT24C02 e2prom的操作，需要确定AT24C02的地址及读写访问时序。
　　●        AT24C02地址的确定
![](http://www.dzsc.com/data/uploadfile/20091222134920974.jpg)
　　原理图上将A2、A1、A0都接地了，所以地址是0x50。
　　●        AT24C02任意地址字节写的时序
![](http://www.dzsc.com/data/uploadfile/20091222134920542.jpg)
　　可见此时序符合前面提到的“单开始信号时序”
　　●        AT24C02任意地址字节读的时序
![](http://www.dzsc.com/data/uploadfile/20091222134920487.jpg)
　　可见此时序符合前面提到的“多开始信号时序”
　　下面开始具体代码的分析（代码在2.6.22内核上[测试](http://wiki.dzsc.com/info/3442.html)通过）：
　　/*i2c_test.c
　　* hongtao_liu <[lht@farsight.com.cn](mailto:lht@farsight.com.cn)>
　　*/
　　#define I2C_RETRIES 0x0701
　　#define I2C_TIMEOUT 0x0702
　　#define I2C_RDWR 0x0707
　　/*********定义struct i2c_rdwr_ioctl_data和struct i2c_msg，要和内核一致*******/
　　struct i2c_msg
　　{
　　unsigned [short](http://www.dzsc.com/icstock/293/SHORT.html) addr;
　　unsigned short flags;
　　#define I2C_M_TEN 0x0010
　　#define I2C_M_RD 0x0001
　　unsigned short len;
　　unsigned char *buf;
　　};
　　struct i2c_rdwr_ioctl_data
　　{
　　struct i2c_msg *msgs;
　　int nmsgs;
　　/* nmsgs这个数量决定了有多少开始信号，对于“单开始时序”，取1*/
　　};
　　/***********主程序***********/
　　int main()
　　{
　　int fd,ret;
　　struct i2c_rdwr_ioctl_data e2prom_data;
　　fd=open("/dev/i2c-0",O_RDWR);
　　/*
　　*/dev/i2c-0是在注册i2c-dev.c后产生的，代表一个可操作的适配器。如果不使用i2c-dev.c
　　*的方式，就没有，也不需要这个节点。
　　*/
　　if(fd<0)
　　{
　　p[error](http://www.dzsc.com/stock/E/ERROR.html)("open error");
　　}
　　e2prom_data.nmsgs=2;
　　/*
　　*因为操作时序中，最多是用到2个开始信号（字节读操作中），所以此将
　　*e2prom_data.nmsgs配置为2
　　*/
　　e2prom_data.msgs=(struct i2c_msg*)malloc(e2prom_data.nmsgs*sizeof(struct i2c_msg));
　　if(!e2prom_data.msgs)
　　{
　　perror("malloc error");
　　exit(1);
　　}
　　ioctl(fd,I2C_TIMEOUT,1);/*超时时间*/
　　ioctl(fd,I2C_RETRIES,2);/*重复次数*/
　　/***write data to e2prom**/
　　e2prom_data.nmsgs=1;
　　(e2prom_data.msgs[0]).len=2; //1个 e2prom 写入目标的地址和1个数据
　　(e2prom_data.msgs[0]).addr=0x50;//e2prom 设备地址
　　(e2prom_data.msgs[0]).flags=0; //write
　　(e2prom_data.msgs[0]).buf=(unsigned char*)malloc(2);
　　(e2prom_data.msgs[0]).buf[0]=0x10;// e2prom 写入目标的地址
　　(e2prom_data.msgs[0]).buf[1]=0x58;//the data to write
　　ret=ioctl(fd,I2C_RDWR,(unsigned long)&e2prom_data);
　　if(ret<0)
　　{
　　perror("ioctl error1");
　　}
　　sleep(1);
　　/******read data from e2prom*******/
　　e2prom_data.nmsgs=2;
　　(e2prom_data.msgs[0]).len=1; //e2prom 目标数据的地址
　　(e2prom_data.msgs[0]).addr=0x50; // e2prom 设备地址
　　(e2prom_data.msgs[0]).flags=0;//write
　　(e2prom_data.msgs[0]).buf[0]=0x10;//e2prom数据地址
　　(e2prom_data.msgs[1]).len=1;//读出的数据
　　(e2prom_data.msgs[1]).addr=0x50;// e2prom 设备地址
　　(e2prom_data.msgs[1]).flags=I2C_M_RD;//read
　　(e2prom_data.msgs[1]).buf=(unsigned char*)malloc(1);//存放返回值的地址。
　　(e2prom_data.msgs[1]).buf[0]=0;//初始化读缓冲
　　ret=ioctl(fd,I2C_RDWR,(unsigned long)&e2prom_data);
　　if(ret<0)
　　{
　　perror("ioctl error2");
　　}
　　printf("[buff](http://www.dzsc.com/stock/B/BUFF.html)[0]=%x\n",(e2prom_data.msgs[1]).buf[0]);
　　/***打印读出的值，没错的话，就应该是前面写的0x58了***/
[close](http://www.dzsc.com/stock-ic/CLOSE.html)(fd);
　　return 0;
　　}
　　以上讲述了一种比较常用的利用i2c-dev.c操作i2c设备的方法，这种方法可以说是在应用层完成了对具体i2c设备的驱动工作。
---------------------------------------------------------------------------------------------------------------------------------------------
在内核里写i2c设备驱动
　　（1）在内核里写i2c设备驱动的两种方式：
　　●    Probe方式（new style），如：
　　static struct i2c_driver pca953x_driver = {
　　.driver = {
　　.[name](http://www.dzsc.com/stock-ic/NAME.html) = "pca953x",
　　},
　　.probe = pca953x_probe,
　　.remove = pca953x_remove,
　　.id_table = pca953x_id,
　　};
　　●    Adapter方式（LEGACY），如：
　　static struct i2c_driver [pcf8575](http://www.dzsc.com/stock-ic/PCF8575.html)_driver = {
　　.driver = {
　　.owner = THIS_MODULE,
　　.n[AME](http://www.dzsc.com/data/iccompany/detail229.html) = "pcf8575",
　　},
　　.attach_adapter = pcf8575_attach_adapter,
　　.detach_client = pcf8575_detach_client,
　　};
　　（2）适配器驱动编写方法
　　（3）分享一些项目中遇到的问题
　　希望大家多提意见，多多交流。
“本文由华清远见http://www.embedu.org/index.htm提供”
