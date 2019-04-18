# rtc驱动框架  与  新的字符设备驱动注册方式 - TaigaComplex求职中 - 博客园







# [rtc驱动框架  与  新的字符设备驱动注册方式](https://www.cnblogs.com/TaigaCon/archive/2012/11/29/2795124.html)





首先来了解新的字符设备驱动注册方式。

**对于Linux字符设备驱动注册方式**

**2.4及其之前的内核：**

　　　　　　　　　　　　register_chrdev(0,"hello",hello_fops);

　　　　　　　　　　　　该函数所注册的字符设备是以主设备号为单位的，一个主设备号只能对应一个fops，只能支持256个驱动

**2.4以后的内核　　：**

　　　　　　　　　　　　alloc_chrdev_region/register_chrdev_region　　/*　如果主设备号为0，用alloc，否则用register　　*/

　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　/*  主设备号与次设备号一起，用于建立新的设备号区域（devid） */

　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　/*　一个设备区域对应一个fops　　　　　　　　*/


　　　　　　　　　　　　cdev_init　　　　　　　　　　　　　　　　　　　/*　　把fops封装成cdev　　*/

　　　　　　　　　　　　cdev_add　　　　　　　　　　　　　　　　　　　/*　　建立cdev与设备号devid对应关系　　*/

　　　　　　　　　　　　可支持的主设备号有2的12次方个，次设备号有2的20次方个，即4G个驱动

　　后面的函数比register_chrdev所支持的字符设备驱动数目大大增多，因此推荐用后面的方法。



**rtc，即real-time clock，实时芯片时钟**

一般的计算机系统，当我们关机后，板子上的小电池能保证实时时钟的继续运行，所以开机后会继续显示正确的时间信息。

rtc驱动就是一个应用了新的字符设备驱动的一个实例。



本驱动基于s3c2440

rtc驱动起始于drivers\rtc\rtc-s3c.c

以下为驱动流程

![](https://pic002.cnblogs.com/images/2012/421096/2012112918172432.jpg)

1.先对对系统总线进行驱动注册，如果发现同名设备则调用probe函数

2.probe函数内部调用rtc_device_register，其中的参数有底层的操作函数结构体，用于操作rtc寄存器

3.rtc_device_register函数内部调用

　　　　　　　　　　　　　　　　rtc_dev_prepare(rtc)　　  //内部调用cdev_init，

　　　　　　　　　　　　　　　　rtc_dev_add_device(rtc)   //内部调用cdev_add

4.rtc内部有成员devid，在另外一个文件（driver/rtc/Rtc-dev.c）用alloc_chrdev_region进行初始化

** 其中rtc_device_register就是用于连接上层接口与底层硬件（操作函数）**



**rtc的open与ioctl调用过程：**



app: open("/dev/rtc0");
-------------------------------------------
kernel: sys_open
            　　　　rtc_dev_fops.open
            	　　　　　　　　rtc_dev_open
            	    　　　　　　　　　　　　// 根据次设备号找到以前用"rtc_device_register"注册的rtc_device
            		　　　　　　　　　　　　struct rtc_device *rtc = container_of(inode->i_cdev,struct rtc_device, char_dev);
            		　　　　　　　　　　　　const struct rtc_class_ops *ops = rtc->ops;
            		　　　　　　　　　　　　err = ops->open ? ops->open(rtc->dev.parent) : 0;
												　　　　　　　　　　　　　　　　　　　　　　　　　　　s3c_rtc_open


app:    ioctl(fd, RTC_RD_TIME,...)
-------------------------------------------
　　　　kernel: sys_ioctl
             　　　　　　　　rtc_dev_fops.ioctl
             	 　　　　　　　　　　　　rtc_dev_ioctl
             	 		　　　　　　　　　　　　　　　　　　struct rtc_device *rtc = file->private_data;
             	 		　　　　　　　　　　　　　　　　　　rtc_read_time(rtc, &tm);
             	 			　　　　　　　　　　　　　　　　　　　　　　err = rtc->ops->read_time(rtc->dev.parent, tm);
             	 						　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　s3c_rtc_gettime



**由于mini2440板子默认是没有设置rtc设备的，当驱动通过总线去寻找同名设备的时候会没法找到，因此驱动无法使用**

**所以，需要修改内核：**

首先　　

修改arch\arm\plat-s3c24xx\common-smdk.c
static struct platform_device __initdata *smdk_devs[] = {
	&s3c_device_nand,
	&smdk_led4,
	&smdk_led5,
	&smdk_led6,
	&smdk_led7,
改为(在数组smdk_devs里加上s3c_device_rtc):
static struct platform_device __initdata *smdk_devs[] = {
	&s3c_device_nand,
	&smdk_led4,
	&smdk_led5,
	&smdk_led6,
	&smdk_led7,
	&s3c_device_rtc,



然后　　make uImage

最后 　　使用新内核启动



**测试：**

ls /dev/rtc* -l

   date /* 显示系统时间 */
   date 112916302012.30 /* 设置系统时间 date [MMDDhhmm[[CC]YY][.ss]] */
   hwclock -w           /* 把系统时间写入RTC */

   短电,重启,执行date












