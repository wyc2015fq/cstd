# Linux下的device_create创建字符设备节点流程 - TaigaComplex求职中 - 博客园







# [Linux下的device_create创建字符设备节点流程](https://www.cnblogs.com/TaigaCon/archive/2012/12/06/2804307.html)





　　对于字符设备驱动，当file_operations注册好了之后，这只是实现了该驱动的内部功能，我们还要向用户提供操作接口，也就是在/dev/目录下创建字符设备节点，其中device_create函数就用于实现该功能。

　　创建设备节点是device_create的功能之一，下面主要分析该功能的主要流程。



以下基于Linux2.6.32.2

** device_create函数分析：**

device_create　　　　　　　　/*creates a device and registers it with sysfs*/

　　　　device_create_vargs

　　　　　　　　device_register

　　　　　　　　　　　　device_add

　　　　　　　　　　　　　　　　kobject_uevent(&dev->kobj, KOBJ_ADD);

　　　　　　　　　　　　　　　　　　　　kobject_uevent_env(kobj, action, NULL);　　　　/*send an uevent with environmental data*/

　　　　　　　　　　　　　　　　　　　　　　　　if (uevent_helper[0]){

　　　　　　　　　　　　　　　　　　　　　　　　...

　　　　　　　　　　　　　　　　　　　　　　　　call_usermodehelper(argv[0], argv,env->envp, UMH_WAIT_EXEC);}



其中kobject_uevent_env是实现该功能的主要函数

该函数功能分两部分：

**1.分配设置环境变量env：**

　　env = kzalloc(sizeof(struct kobj_uevent_env), GFP_KERNEL);　　　　　　//分配环境变量



　　add_uevent_var(env, "ACTION=%s", action_string);　　　　　　　　　　　//env->envp[0] = ACTION=add　　如果是删除设备的时候该值就是remove，

　　add_uevent_var(env, "DEVPATH=%s", devpath);　　　　　　　　　　　　  //env->envp[1] = DEVPATH=/devices/virtual/hello/hello0

　　add_uevent_var(env, "SUBSYSTEM=%s", subsystem);　　　　　　　　　　//env->envp[2] = SUBSYSTEM=hello



　　uevent_ops->uevent(kset, kobj, env);　　　　　　　　　　　　　　　　　　　//env->envp[3] = MAJOR=253

　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　//env->envp[4] = MINOR=0

　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　//env->envp[5] = DEVNAME=hello0



　　add_uevent_var(env, "SEQNUM=%llu", (unsigned long long)seq);　　　　　//env->envp[6] = SEQNUM=582

　　add_uevent_var(env, "HOME=/");　　　　　　　　　　　　　　　　　　　　　//env->envp[7] = HOME=/

　　add_uevent_var(env,"PATH=/sbin:/bin:/usr/sbin:/usr/bin");　　　　　　　　//env->envp[8] = PATH=/sbin:/bin:/usr/sbin:/usr/bin



**2.设置完环境变量后**

　　if (uevent_helper[0]){　　　　//如果有该参数（该参数实际上是mdev入口/sbin/mdev，因为在/etc/init.d/rcS中：echo /sbin/mdev > /proc/sys/kernel/hotplug）

　　　　　　　　　　　...

　　　　　　　　　　　call_usermodehelper(argv[0], argv,env->envp, UMH_WAIT_EXEC);}　　　　//调用mdev，然后mdev会根据先前设置的环境变量进行处理



**测试：**

如果在call_usermodehelper函数之前添加打印语句：

　　int i=0;

　　printk("Testing: uevent_helper = %s\n",uevent_helper);

 　　for(;env->envp[i];i++)
			　　　　printk("env->envp[%d] = %s\n",i,env->envp[i]);

就会在每次运行到函数kobject_uevent_env时都会打印uevent_helper跟环境变量

例子如下：

　　Testing : uevent_helper = /sbin/mdev

　　env->envp[0] = ACTION=add
　　env->envp[1] = DEVPATH=/devices/virtual/hello/hello0
　　env->envp[2] = SUBSYSTEM=hello
　　env->envp[3] = MAJOR=253
　　env->envp[4] = MINOR=0
　　env->envp[5] = DEVNAME=hello0
　　env->envp[6] = SEQNUM=582
　　env->envp[7] = HOME=/
　　env->envp[8] = PATH=/sbin:/bin:/usr/sbin:/usr/bin



**kobject_uevent_env函数最后调用了mdev，mdev的入口函数在busybox的mdev_main**

mdev_main

　　　　if (argv[1] && !strcmp(argv[1], "-s"))　　　　//先判断参数1是否为-s，如果为-s则表明mdev为开机执行的情况（mdev -s位于/etc/init.d/rcS中）

　　　　else

　　　　getenv　　　　/*提取各个环境变量*/

　　　　make_device　　/*根据action创建设备*/

　　　　　　　　/*对于设备，当我们创建其节点时，我们可以通过配置文件进行配置



　　　　　　　　 *该配置文件位于/etc/mdev.conf　　　　　　　　　　　　　　

　　　　　　　 　*            设备名称正则表达式用户id   组id         节点属性创建的设备节点路径 shell命令

　　　　　　　　 *配置方式为<device regex><uid>:<gid> <octal permissions> [=path][@|$|*<command>]

　　　　　　　　 *配置的帮助文件为busybox下的mdev.txt

　　　　　　　　 */

　　　　　　　　parser = config_open2("/etc/mdev.conf", fopen_for_read);　　　　/*打开/etc/mdev.conf文件*/

　　　　　　　　while (config_read(parser, tokens, 4, 3, "# \t", PARSE_NORMAL))　　/*分析mdev.conf文件内容，并执行相关操作command*/

　　　　　　　　{...}

　　　　　　　　mknod(device_name, mode | type, makedev(major, minor)) 　　　　/*调用mknod进行节点创建*/












