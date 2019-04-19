# 手把手教你写Linux I2C设备驱动 - maopig的专栏 - CSDN博客
2012年06月14日 23:15:20[maopig](https://me.csdn.net/maopig)阅读数：1186标签：[c																[linux																[module																[linux内核																[buffer																[null](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=buffer&t=blog)](https://so.csdn.net/so/search/s.do?q=linux内核&t=blog)](https://so.csdn.net/so/search/s.do?q=module&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)
个人分类：[u-boot/驱动开发/linux](https://blog.csdn.net/maopig/article/category/930963)
                
 手把手教你写Linux I2C设备驱动
标签：[Linux](http://blog.51cto.com/tagindex.php?keyword=Linux)[设备](http://blog.51cto.com/tagindex.php?keyword=%C9%E8%B1%B8)[驱动](http://blog.51cto.com/tagindex.php?keyword=%C7%FD%B6%AF)[详解](http://blog.51cto.com/tagindex.php?keyword=%CF%EA%BD%E2)[i2c](http://blog.51cto.com/tagindex.php?keyword=i2c)
原创作品，允许转载，转载时请务必以超链接形式标明文章 [原始出处](http://ticktick.blog.51cto.com/823160/760020) 、作者信息和本声明。否则将追究法律责任。[http://ticktick.blog.51cto.com/823160/760020](http://ticktick.blog.51cto.com/823160/760020)
    Linux I2C驱动是嵌入式Linux驱动开发人员经常需要编写的一种驱动，因为凡是系统中使用到的I2C设备，几乎都需要编写相应的I2C驱动去配置和控制它，例如 RTC实时时钟芯片、音视频采集芯片、音视频输出芯片、EEROM芯片、AD/DA转换芯片等等。
    Linux I2C驱动涉及的知识点还是挺多的，主要分为Linux I2C的总线驱动（I2C BUS Driver）和设备驱动（I2C Clients Driver），本文主要关注如何快速地完成一个具体的I2C设备驱动（I2C Clients Driver）。关于Linux I2C驱动的整体架构、核心原理等可以在网上搜索其他相关文章学习。
    注意：本系列文章的I2C设备驱动是基于Linux 2.6.18内核。
    本文主要参考了Linux内核源码目录下的 ./Documentation/i2c/writing-clients 文档。以手头的一款视频采集芯片TVP5158为驱动目标，编写Linux I2C设备驱动。
**1.   i2c_driver结构体对象**
    每一个I2C设备驱动，必须首先创造一个i2c_driver结构体对象，该结构体包含了I2C设备探测和注销的一些基本方法和信息，示例如下：
- staticstruct i2c_driver tvp5158_i2c_driver = {  
-         .driver = {  
-             .name = "tvp5158_i2c_driver",  
-         },  
-         .attach_adapter = &tvp5158_attach_adapter,  
-         .detach_client  = &tvp5158_detach_client,  
-         .command        = NULL,  
- };  
    其中，name字段标识本驱动的名称（不要超过31个字符），attach_adapter和detach_client字段为函数指针，这两个函数在I2C设备注册的时候会自动调用，需要自己实现这两个函数，后面将详细讲述。
2.   **i2c_client 结构体对象**
    上面定义的i2c_driver对象，抽象为一个i2c的驱动模型，提供对i2C设备的探测和注销方法，而i2c_client结构体则是代表着一个具体的i2c设备，该结构体有一个data指针，可以指向任何私有的设备数据，在复杂点的驱动中可能会用到。示例如下：   
- struct tvp5158_obj{        
- struct i2c_client client;        
- int users; // how many users using the driver   
- };  
- 
- struct tvp5158_obj* g_tvp5158_obj;  
    其中，users为示例，用户可以自己在tvp5158_obj这个结构体里面添加感兴趣的字段，但是i2c_client字段不可少。具体用法后面再详细讲。
**3.   设备注册及探测功能**
    这一步很关键，按照标准的要求来写，则Linux系统会自动调用相关的代码去探测你的I2C设备，并且添加到系统的I2C设备列表中以供后面访问。
    我们知道，每一个I2C设备芯片，都通过硬件连接设定好了该设备的I2C设备地址。因此，I2C设备的探测一般是靠设备地址来完成的。那么，首先要在驱动代码中声明你要探测的I2C设备地址列表，以及一个宏。示例如下：
- static unsigned short normal_i2c[] = {  
-         0xbc >> 1,  
-         0xbe >> 1,  
-         I2C_CLIENT_END  
- };  
- I2C_CLIENT_INSMOD;  
    normal_i2c 数组包含了你需要探测的I2C设备地址列表，并且必须以I2C_CLIENT_END作为结尾，注意，上述代码中的0xbc和0xbe是我在硬件上为我的tvp5158分配的地址，硬件上我支持通过跳线将该地址设置为 0xbc 或者 0xbe，所以把这两个地址均写入到探测列表中，让系统进行探测。如果你的I2C设备的地址是固定的，那么，这里可以只写你自己的I2C设备地址，注意必须向右移位1。
    宏 I2C_CLIENT_INSMOD 的作用网上有许多文章进行了详细的讲解，这里我就不详细描述了，记得加上就行，我们重点关注实现。
    下一步就应该编写第1步中的两个回调函数，一个用于注册设备，一个用于注销设备。探测函数示例如下：
- staticint tvp5158_attach_adapter(struct i2c_adapter *adapter)  
- {  
- return i2c_probe(adapter, &addr_data, &tvp5158_detect_client);  
- } 
    这个回调函数系统会自动调用，我们只需要按照上述代码形式写好就行，这里调用了系统的I2C设备探测函数，i2c_probe()，第三个参数为具体的设备探测回调函数，系统会在探测设备的时候调用这个函数，需要自己实现。示例如下：
- staticint tvp5158_detect_client(struct i2c_adapter *adapter,int address,int kind)  
- {  
- struct tvp5158_obj *pObj;  
- int err = 0;  
- 
-     printk(KERN_INFO "I2C: tvp5158_detect_client at address %x ...\n", address);  
- 
- if( g_tvp5158_obj != NULL  ) {  
- //already allocated,inc user count, and return the allocated handle 
-         g_tvp5158_obj->users++;  
-         return 0;  
-     }  
- 
- /* alloc obj */
-     pObj = kmalloc(sizeof(struct tvp5158_obj), GFP_KERNEL);  
- if (pObj==0){  
- return -ENOMEM;  
-     }  
-     memset(pObj, 0, sizeof(struct tvp5158_obj));  
-     pObj->client.addr    = address;  
-     pObj->client.adapter = adapter;  
-     pObj->client.driver  = &tvp5158_i2c_driver;  
-     pObj->client.flags   = I2C_CLIENT_ALLOW_USE;  
-     pObj->users++;  
- 
- /* attach i2c client to sys i2c clients list */
- if((err = i2c_attach_client(&pObj->client))){  
-         printk( KERN_ERR "I2C: ERROR: i2c_attach_client fail! address=%x\n",address);  
- return err;  
-     }  
- 
- // store the pObj 
-     g_tvp5158_obj = pObj;  
- 
-     printk( KERN_ERR "I2C: i2c_attach_client ok! address=%x\n",address);  
- 
- return 0;  
- } 
    到此为止，探测并且注册设备的代码已经完成，以后对该  I2C 设备的访问均可以通过 g_tvp5158_obj 这个全局的指针进行了。
**4.    注销I2C设备 **
    同理，设备注销的回调函数也会自动被系统调用，只需要按照模板写好设备注销代码，示例如下：    
- staticint tvp5158_detach_client(struct i2c_client *client)  
- {  
- int err;  
- 
- if( ! client->adapter ){  
- return -ENODEV;  
-     }  
- 
- if( (err = i2c_detach_client(client)) ) {  
-         printk( KERN_ERR "Client deregistration failed (address=%x), client not detached.\n", client->addr);  
- return err;  
-     }  
- 
-     client->adapter = NULL;  
- 
- if( g_tvp5158_obj ){  
-         kfree(g_tvp5158_obj);  
-     }  
- 
- return 0;  
- } 
    到此为止，设备的注册和注销代码已经全部完成，下面要做的就是提供读写I2C设备的方法。
** 5.   I2C设备的读写       **
    对I2C设备的读写，Linux系统提供了多种接口，可以在内核的 i2c.h 中找到，这里简单介绍其中的两种接口。
   【接口一】：
- externint i2c_master_send(struct i2c_client *,constchar* ,int);  
- 
- externint i2c_master_recv(struct i2c_client *,char* ,int);  
    第一个参数是 i2c_client 对象指针，第二个参数是要传输的数据buffer指针，第三个参数为buffer的大小。
   【接口二】：
- externint i2c_transfer(struct i2c_adapter *adap, struct i2c_msg *msg, int num); 
    这个接口支持一次向I2C设备发送多个消息，每一个消息可以是读也可以是写，读或者写以及读写的目标地址（寄存器地址）均包含在msg消息参数里面。
    这些接口仅仅是最底层的读写方法，关于具体怎么与I2C设备交互，比如具体怎么读芯片的某个特定寄存器的值，这需要看具体的芯片手册，每个I2C芯片都会有具体的I2C寄存器读写时序图。因此，为了在驱动中提供更好的访问接口，还需要根据具体的时序要求对这些读写函数进行进一步封装，这些内容将在后面的文章中讲述。
**6.  模块初始化及其他**
    下一步就是整个模块的初始化代码和逆初始化代码，以及模块声明了。    
- staticint __init tvp5158_i2c_init(void) 
- { 
-     g_tvp5158_obj = NULL; 
- 
- return i2c_add_driver(&tvp5158_i2c_driver); 
- } 
- 
- staticvoid __exit tvp5158_i2c_exit(void) 
- { 
-     i2c_del_driver(&tvp5158_i2c_driver); 
- } 
- 
- module_init(tvp5158_i2c_init); 
- module_exit(tvp5158_i2c_exit); 
- 
- MODULE_DESCRIPTION("TVP5158 i2c driver"); 
- MODULE_AUTHOR("Lujun @hust"); 
- MODULE_LICENSE("GPL"); 
    在初始化的代码里面，添加本模块的 i2c driver 对象，在逆初始化代码里面，删除本模块的 i2c driver 对象。
**7.   总结**
    到此为止，算是从应用的角度把编写一个I2C的设备驱动代码讲完了，很多原理性的东西我都没有具体分析（其实我也了解的不深），以后会慢慢更深入地学习和了解，文中有什么讲述不正确的地方，欢迎留言或者来信[lujun.hust@gmail.com](mailto:lujun.hust@gmail.com)交流。
    读到最后，大家可能还有一个疑问，这个驱动写完了怎么在用户空间（应用层）去使用它呢？由于本文不想把代码弄得太多太复杂，怕提高理解的难度，所以就没有讲，其实要想在用户空间使用该I2C设备驱动，则还需要借助字符设备驱动来完成，即为这个I2C设备驱动封装一层字符设备驱动，这样，用户空间就可以通过对字符设备驱动的访问来访问I2C设备，这个方法我会在后面的文章中讲述。
