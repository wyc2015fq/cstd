# linux input 子系统分析 - xqhrs232的专栏 - CSDN博客
2016年11月04日 10:03:55[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：274
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://blog.csdn.net/zhangxizhicn/article/details/6642062](http://blog.csdn.net/zhangxizhicn/article/details/6642062)
相关文章
1、[Linux如何查看与/dev/input目录下的event对应的设备](http://blog.csdn.net/myarrow/article/details/10582667)----[http://blog.csdn.net/myarrow/article/details/10582667](http://blog.csdn.net/myarrow/article/details/10582667)
先写一篇备忘，以后有机会再整理
[Linux](http://lib.csdn.net/base/linux) 
 Input 设备驱动
1认识和使用 input 事件：
1．1Linux input 驱动分类
Input驱动程序是Linux输入设备的驱动程序，分成游戏杆（joystick）、鼠标（mouse和mice）和事件设备（Event queue）3种驱动程序。其中事件驱动程序是目前通用的驱动程序，可支持键盘、鼠标、触摸屏等多种输入设备。
Input驱动程序的主设备号是13，驱动程序的设备号分配如下所示。
  joystick游戏杆：0～31 
  mouse鼠标：32～62    
  mice鼠标：63
 事件（Event）设备：64～95
1．2查看 input 驱动
cat /proc/bus/input/devices
例如显示以下信息：
I: Bus=0017 Vendor=0001 Product=0001 Version=0100
N: Name="Macintosh mouse button emulation"
P: Phys=
S: Sysfs=/devices/virtual/input/input0
U: Uniq=
H: Handlers=mouse0 event0
B: EV=7
B: KEY=70000 0 0 0 0 0 0 0 0
B: REL=3：
则说明 /dev/input/mouse0  和 /dev/input/event0 都对应着同一个鼠标设备。
2．理解Input 子系统的模型：
2．1  input 子系统概述
Linux input  子系统将一个输入设备的输入过程分成了设备驱动(input device driver)和事件驱动（input event driver）两个层。前者负责从底层硬件采集数据；后者负责与用户程序接口，将采集到的数据分发给不同的用户接口。通过这样的设计，将千差万别的设备统一到了为数不多的几种驱动接口上。同一种事件驱动可以用来处理多个同类设备；同一个设备也可以和多种事件驱动相衔接。
设备驱动并不创建文件节点，它只负责将采集到数据通过input.c中的函数input_event() 向上一层汇报；而各个事件驱动则分别将他们各自感兴趣的事件信息提取出来，通过文件节点，提供给用户。在这个过程中，input 子系统的核心负责着这两层的交互工作。并管理和维护着记录了他们各自信息的链表。具体过程如下图所示：
更进一步理解Input 子系统，需要理解以下4个对象：
Input_device , handler,  handle , client
Input_device:  代表着具体的输入设备，它直接从硬件中读取数据，并以事件的形式转发
Hanler:  代表接收某一类事件的上层接口，对应于一类事件设备文件
Handle : 用于将input_device 和  handler 连接起来的胶水，对应于某1个具体的设备文件。
Client:  对应于用户程序对文件的访问接口，每open一次事件驱动，就创建一个client.
Input 子系统维护者有两条重要的链表：
static LIST_HEAD(input_dev_list);
static LIST_HEAD(input_handler_list);
一条记录着系统中所有的输入设备，另一条记录着所有的事件驱动。输入设备是通过input_dev 表示，而事件驱动是通过handler 表示。每当一个新的设备（如键盘，鼠标等）或者一个新的事件驱动被系统加载（调用input_register_device()或 input_register_driver()），都会扫描整个连标，并调用函数input_match_device(struct
 input_handler *handler, struct input_dev *dev)  尝试配对工作。Input_handler-->id_table   记录了需要匹配的特征。
比如eventX事件驱动它的匹配特征存在于
static const struct input_device_id evdev_ids[] = {
            { .driver_info = 1 },       /* Matches all devices */
            { },                                /* Terminating zero entry */
};
就像代码中注释的一样，任何输入设备都可以和他匹配成功，也就是说任何输入设备都可以映射到/input/dev/eventX 下。 一旦匹配成功就会调用函数handler->connect(handler, dev, id);将具体的设备（input_dev）和事件驱动(handler)邦定到一起,形成一个handle，将handle 注册到它相关联的input_dev 设备的链表h_list中。设备编号在input_init()
 的时候通过调用创建。
2．2  文件节点的建立
Input 子系统在初始化时，首先调用register_chrdev(INPUT_MAJOR, "input", &input_fops); 为自己注册了256个字符设备节点。（这也代表着系统中最多可以存在256个输入设备）这256个设备会被分为8类，分别对应于数组input_table[8]中存放的8个 handler.
static struct input_handler *input_table[8];  // kernel/input/input.c
其中数第1个句柄管理此设备号0-31，第2个句柄管理设备号32-63。。。
每一个句柄，都可以用来实现一类事件驱动（但每类事件驱动最多只能管理32个设备）。
例如：/dev/input/eventX所表示的设备  和dev /input/mouseX 所表示的设备 就分别使用了最常见的两种事件驱动。以/dev/input/eventX  为例，他们都使用同一个event事件驱动，从对象的角度看，拥有同一个handler。而这个handler 所管理的设备，拥有次设备号64-95，每一个次设备号又可以对应到一个handle.
2．3 事件驱动注册
事件驱动的方法被抽象到特定的 handler结构体中，驱动在初始化时并通过调用input_register_handler（）把handler 注册的系统中。
input_register_handler（）具体完成3件事情：
1． 把handler 存放到input_table[]中
2． 把 handler 存放到input 核心的链表中
3． 扫描input核心链表上的device ,并试着匹配
2.4 设备驱动
具体设备是用input_dev 表示的，它通过函数input_register_device（）注册。
input_register_device（）主要做了两件事情：
1．  把 input_dev 存放到input 核心的链表中
2． 扫描input核心链表上的handler ,并试着匹配
2.5 匹配
Dev 和handler 是通过input_match_device(struct input_handler *handler, struct input_dev *dev)  尝试配的。每当查找到可以相互匹配的dev 和 handler 便回调handler->connect() ,生成一个handle结构体，并 通过handle  把某一个具体的dev 和另一个具体的handler被邦定到一起。最后handle
 通过函数input _register_handle()注册到dev 的链表中。以后dev 上有数据要发送时，便可以在dev 中搜索这个链表，找到handle,再调用handle->handler->event() 把数据传到了event 驱动中。
2．6数据传送
当有数据上传时，数据通过input_pass_event () 函数向handle 发送， 这个函数的核心内容如下：
list_for_each_entry_rcu(handle, &dev->h_list, d_node) {
            handler = handle->handler;
            handler->event(handle, type, code, value);
  }
根据input_dev 设备的h_list 链表查找与他相关的handle,然后调用event 处理方法，将数据传递给上层 的client。
2．7 clinet
每一个事件驱动（handler），可能对应多个文件节点(handle)，而每一个文件节点又可以有多个client, 底层的数据直接复制到上层的client中。对文件句柄的操作则实际上是对这些个client 中的数据进行操作。
以驱动evdev.c 为例，根据次设备号取值范围64-95。可以分别生成/input/event0 ,input/event1,一直到/input/event31总共32个设备文件。
每一个设备文件又可以同时对应多个client,当有多个应用程序同时调用设备文件时，他们会从不同的client 中取数据。每当应用程序调用open 时，便会调用evdev_open()创建一个client,并被与设备节点邦定  file->private_data = client;
设备被应用层当作文件操作时，通过struct evdev_client *client = file->private_data; 再将client 找到。直接读/写 client 中的数据。而当底层有数据要传过来时则会通过handler->event() 把数据复制到client 中，如evdev_event()通过client 链便，对每一个client 进行evdev_pass_event()调用。而在evdev_pass_event()中实现数据向client
 节点的复制。
