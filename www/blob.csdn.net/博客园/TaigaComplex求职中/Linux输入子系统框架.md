# Linux输入子系统框架 - TaigaComplex求职中 - 博客园







# [Linux输入子系统框架](https://www.cnblogs.com/TaigaCon/archive/2012/11/05/2756088.html)





Linux输入子系统源文件入口在drivers/input/input.c目录下

输入子系统分为两层：

**1.核心层：**

主要功能

　　注册主设备号

　　对于swi进入的open函数进行第一层处理，并通过次设备号选择handler进入第二层open，也就是真正的open所在的file_operation,并返回该file_opration的fd

　　提供input_register_device跟input_register_handler函数分别用于注册device跟handler

**2.handler和device层**

**handler：**

　　　　handler层是纯软件层，包含不同的解决方案，如键盘，鼠标，游戏手柄等，但是没有设计到硬件方面的操作

　　　　对于不同的解决方案，都包含一个名为input_handler的结构体，该结构体内含的主要成员如下

　　　　　　.id_table　　　一个存放该handler所支持的设备id的表（其实内部存放的是EV_xxx事件,用于判断device是否支持该事件）

　　　　　　.fops　　　　　该handler的file_operation

　　　　　　.connect　　　连接该handler跟所支持device的函数

　　　　　　.disconnect　　断开该连接

　　　　　　.event　　　　事件处理函数，让device调用

　　　　　　h_list　　　　也是一个链表，该链表保存着该handler到所支持的所有device的中间站：handle结构体的指针

**device：**

　　　　device是纯硬件操作层，包含不同的硬件接口处理，如gpio等

　　　　　　对于每种不同的具体硬件操作，都对应着不同的input_dev结构体

　　　　　　该结构体内部也包含着一个h_list



**input_handler_list和input_device_list**

　　　　对于handler和device，分别用链表input_handler_list和input_device_list进行维护，

　　　　当handler或者device增加或减少的时候，分别往这两链表增加或删除节点。



![](https://pic002.cnblogs.com/images/2012/421096/2012110522003085.jpg)





**代码流程：**

　　1.当外部应用程序需要调用输入子系统的open函数时，会先通过主设备号进入到核心层，然后通过次设备号进入handler层，再调用.fops内的open函数返回fd；

　　2.当外部应用程序需要调用输入子系统的read函数时，会通过返回的fd调用.fop内的read函数，然后休眠，等待被.event函数唤醒

　　3.当外部中断到达的时候，会先确定中断事件，然后用input_event上报事件，再通过h_list里面的所有handle调用对应的handler中的.event函数，对read进行唤醒，然后在read中返回（也就是当device有多个对应的handler的时候,input_event会向所有的handler上报事件）



　　4.当需要加入新的handler时，需要先构建handler结构体，然后调用input_register_handler对该handler进行注册

　　input_register_handler的内部实现：往input_handler_list加入新增的handler节点，然后对input_device_list的所有结点（也就是所有的device）进行遍历，通过.id_table查看该device是否支持该handler,对支持的device调用.connect,一一地构建input_handle结构体,连接handler跟device



　　5.当需要加入新的device时，需要先构建input_dev结构体，然后调用input_register_device对该input_dev进行注册

　　input_register_dev的内部实现：往input_device_list加入新增的device节点，然后对input_handler_list的所有结点（也就是所有的handler）进行遍历，通过handler　的.id_table查看该handler是否支持该device,对支持的device调用该handler的.connect,一一地构建input_handle结构体,连接handler跟device



　　在输入子系统框架下，我们一般的编写驱动也就是对device部分进行编写（分配input_dev并配置，驱动入口，出口，中断时进行中断判断，然后上报事件等），然后对该device的input_dev进行注册












