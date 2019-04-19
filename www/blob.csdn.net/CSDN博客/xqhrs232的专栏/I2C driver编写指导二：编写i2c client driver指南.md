# I2C driver编写指导二：编写i2c client driver指南 - xqhrs232的专栏 - CSDN博客
2010年12月26日 10:18:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1627标签：[c																[struct																[command																[structure																[数据结构																[module](https://so.csdn.net/so/search/s.do?q=module&t=blog)](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)](https://so.csdn.net/so/search/s.do?q=structure&t=blog)](https://so.csdn.net/so/search/s.do?q=command&t=blog)](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://blog.csdn.net/guoshaobei/archive/2010/06/08/5656001.aspx](http://blog.csdn.net/guoshaobei/archive/2010/06/08/5656001.aspx)
译者：郭少悲
日期：2010/06/08
源文：linux-2.6/Documentation/i2c/writing-clients
这是一篇关于指导编写I2C/SMBus内核驱动的文档，使用Linux作为协议的host/master
(not slave)。
编写一个驱动，你需要做一些事情。有些是可选的，有些则需要你斟酌考虑。利用这篇文
档来指导你的工作，但是它并不是一个规则书 (rule book)！
General remarks
===============
为了保持内核尽可能的干净，最好的办法是在你使用的全局符号前加一个前缀，对那些需
要导出的符号来说尤为重要。在这篇文档里，我们使用'foo_'作为我们示例的前缀。
The driver structure
====================
通常，你需要实现一个简洁的驱动数据结构(driver structure)，利用它实例化所有的
clients。记住，一个驱动数据结构包括了通用的访问方法(routine)，一些你需要使用的
域必须显示的初始化，剩余的域则需要零初始化(zero-initialized)。一个client
srtucture包含了设备相关的信息，比如驱动模型设备节点和I2C地址。
static struct i2c_device_id foo_idtable[] = {
    { "foo", my_id_for_foo },
    { "bar", my_id_for_bar },
    { }
};
MODULE_DEVICE_TABLE(i2c, foo_idtable);
static struct i2c_driver foo_driver = {
    .driver = {
        .name    = "foo",
    },
    .id_table    = foo_ids,
    .probe        = foo_probe,
    .remove        = foo_remove,
    /* if device autodetection is needed: */
    .class        = I2C_CLASS_SOMETHING,
    .detect        = foo_detect,
    .address_list    = normal_i2c,
    .shutdown    = foo_shutdown,    /* optional */
    .suspend    = foo_suspend,    /* optional */
    .resume        = foo_resume,    /* optional */
    .command    = foo_command,    /* optional, deprecated */
}
name域是设备驱动的名字，不能包含空白符。它应当匹配模块的名字(如果driver被编译
为一个模块)，或者你也可以使用MODULE_ALIAS(本示例里为MODULE_ALIAS(foo))来为模块添加
一个别名。如果驱动名字不能匹配模块名，则模块不能被自动装载(hotplug/coldplug)。
其他的域是一些回调函数，在下面解释。
Extra client data
=================
每个client structure有一个特定的'data'域，可以指向你任意设定的数据结构。
你应当使用该域来保存设备相关的数据。
    /* store the value */
    void i2c_set_clientdata(struct i2c_client *client, void *data);
    /* retrieve the value */
    void *i2c_get_clientdata(const struct i2c_client *client);
注意从2.6.34开始，你不必在remove()里或者probe()失败的时候设置'data'域为NULL。
i2c-core将会在适当的时候处理它。
Accessing the client
====================
假定我们有一个有效的client structure。某些时候，我们需要从client里获取一些信息
，或者向client里写一些新的信息。
为此，我发现定义诸如foo_read和foo_write这样的函数很有必要。在一些case里，直接
调用i2c函数是比较简单的做法，但是许多芯片有自己相关的寄存器处理，因此有必要将
其封装成foo_read/foo_write这样的形式简洁的函数。
下面的函数是简单的示例，可作参考：
int foo_read_value(struct i2c_client *client, u8 reg)
{
    if (reg < 0x10)    /* byte-sized register */
        return i2c_smbus_read_byte_data(client, reg);
    else        /* word-sized register */
        return i2c_smbus_read_word_data(client, reg);
}
int foo_write_value(struct i2c_client *client, u8 reg, u16 value)
{
    if (reg == 0x10)    /* Impossible to write - driver error! */
        return -EINVAL;
    else if (reg < 0x10)    /* byte-sized register */
        return i2c_smbus_write_byte_data(client, reg, value);
    else            /* word-sized register */
        return i2c_smbus_write_word_data(client, reg, value);
}
Probing and attaching
=====================
Linux I2C stack最初的编写是为了支持PC主板上的硬件芯片监测，为此在里面嵌入了一
些倾向于SMBus协议(and PCs)的假定。其中的一个假定是绝大多数适配器和设备驱动支持用
于探测设备是否存在的SMBUS_QUICK协议。还有一种假定是设备和它们的驱动使用这种早
期的探测可以进行足够的配置。
当Linux和她的i2c协议栈被广泛的使用于嵌入式系统和复杂的组件诸如DVB adapter上时
，上述的假定就有了问题。i2c设备驱动在发出中断时，需要更多的（不同的）配置信息
；同样，驱动需要更多板级信息来处理那些不能被协议检测到的芯片寄存器。
Device/Driver Binding
---------------------
系统基础代码，典型的如板级相关的初始化代码或者启动固件，报告哪些i2c设备存在。
例如，在内核或者boot loader里会提供一张表，用于识别i2c设备，并将它们关联到合适
的板级配置信息上：IRQs, wiring artifact, chip tye等。这些信息用于为每一个i2c设
备创建i2c_client对象。
i2c设备使用绑定模型的方法和Linux的其他驱动一样：它们提供一个probe()方法来
绑定这些设备，提供一个remove()方法解除绑定。
    static int foo_probe(struct i2c_client *client,
                 const struct i2c_device_id *id);
    static int foo_remove(struct i2c_client *client);
记住i2c_driver并不创建这些client句柄(译者注：指的是foo_probe/foo_remvoe函数里
的client参数)。client句柄也许在foo_probe()里使用。如果foo_probe()报告返回成功
，则client句柄会被保存使用，直到foo_remove()被调用。绑定模型被绝大多数的Linux
drivers使用。
当id_table里的一个entry的name域与设备的名字匹配时，probe()函数被调用。通过传递
匹配的entry，使driver知道table里的哪一项被匹配。
Device Creation
---------------
如果你知道一个i2c设备如何被连接到一个给定的i2c总线上，你就能够实例化一个设备：
填充i2c_board_info structure的device address域和driver name域，然后调用
i2c_new_device()。这将创建一个设备，然后driver core将负责查找合适的驱动并调用该驱
动的probe()方法。如果一个驱动支持不同的设备类型，你可以通过type域来指定你需要
的类型。如果需要，你还可以指定IRQ和platform data。
有时你知道某个设备连接到一个给定的i2c总线上，但是你并不知道它使用的确切的地址
。这样的例子在TV adapters上发生，相同的驱动支持大量的有微小差别的模型，i2c设备
地址从一个模型到另一个模型不断变化。这种case下，你可以使用
i2c_new_probed_device()函数，它与i2c_new_device()类似，但是提供了额外的需要
probe的i2c地址表。一个设备在匹配表中的地址时被创建。如果你期望创建多个设备，仅
仅需要调用i2c_new_probed_device()多次即可。
对i2c_new_device()和i2c_new_probed_device()的调用典型的发生在i2c bus driver里
。你也许想保存返回的i2c_client，在之后使用。
Device Detection
----------------
有时你并不知道哪个i2c设备连接到哪个i2c总线上。举个例子，如PC上的SMBus总线进行
设备硬件监测。这种case下，你也许想让你的驱动自动检测所支持的设备。传统的驱动模
型会这样工作，新的标准驱动模型也可以这样做。
你仅仅需要定义一个detect回调函数，它会识别所支持的设备(返回0表示支持，否则返回
-ENODEV)；你还需要定义一个供探测的地址列表和一个设备类型，这样使那些仅匹配设备
类型的i2c总线被探测到。例如，对于一个自动监测硬件芯片的驱动将会设置它的class域为
I2C_CLASS_HWMON，只有那些class域为I2C_CLASS_HWMOND的适配器能够被驱动探测。记住
类匹配不成功不会阻碍一个给定的i2c适配器来使用该设备。它所阻碍的是自动匹配这个
功能；显示的实例化设备(尽管class不匹配)依然是可用的。
这种机制(detection)是可选的，并不适合所有的设备。你需要一些可靠的方式来识别所
支持的设备(典型的使用设备相关的用于识别的寄存器)，否则会发生紊乱检测。牢记i2c
协议并没有包含标准的方法来检测一个给定地址的芯片，也没有标准的方法来识别设备。
更糟糕的状况是，如果没有总线传输对应的原理图，同一种传输对于一种芯片而言是读操
作，对于另外一种芯片而言是写操作。基于以上原因，显示的设备实例化方式与自动检测
方式相比推荐前者。
Device Deletion
---------------
每个i2c设备，如果使用i2c_new_device()或者i2c_new_probed_device()创建，则可以通
过i2c_unregister_device()来销毁。如果你没有显示的调用它，i2c总线在销毁的时候会
自动调用该函数，因为在设备驱动模型里设备无法比它的父节点活得更长。
Initializing the driver
=======================
当你的内核启动，或者你的foo driver模块要被插入到内核。你必须做一些初始化。幸运
的是，仅仅注册一下驱动模块足够了。
static int __init foo_init(void)
{
    return i2c_add_driver(&foo_driver);
}
static void __exit foo_cleanup(void)
{
    i2c_del_driver(&foo_driver);
}
/* Substitute your own name and email address */
MODULE_AUTHOR("Frodo Looijaard <[frodol@dds.nl](mailto:frodol@dds.nl)>"
MODULE_DESCRIPTION("Driver for Barf Inc. Foo I2C devices");
/* a few non-GPL license types are also allowed */
MODULE_LICENSE("GPL");
module_init(foo_init);
module_exit(foo_cleanup);
注意有一些函数被标记为'__init'。这些函数在内核启动后（或者模块加载后）被删除。
同样的，被标记为'__exit'的函数如果被编入内核(built-in)，在编译的时候被编译器移
除，因为它们没有机会被调用。
Power Management
================
如果你的i2c设备在进入系统省电模式(low power state)时需要做一些特殊的处理--比如
将一个transceiver至於节电模式，或者触发一个系统唤醒机制--这些在suspen()方法里
实现。resume()方法完成suspend()方法的反向功能。
这些是标准驱动模型调用函数，在所有的驱动子系统里都如此工作。这些函数可以睡眠，
能够对被挂起的或者唤醒的设备进行i2c消息传输(因为它们的父节点i2c适配器是激活的
，IRQs也是使能的)。
System Shutdown
===============
如果你的I2c设备在系统关闭或者重启(包含kexec)需要做一些特殊处理--比如关掉某些器
件--使用shutdown()方法。
同样的，这个方法也是标准驱动模型函数调用，在所有的驱动子系统中均如此工作：它可
以睡眠，能够使用i2c messaging。
Command function
================
一个通用的ioctl-like回调函数，在驱动里被支持。你几乎不需要它，它是一个“过期”的
接口，因此新的设计中不应当使用它。
Sending and receiving
=====================
如果你想与你的设备通信，下面的函数API可以使用。参考<linux/i2c.h>。
如果你在plain i2c communicaiton和SMBus level communication之间选择一个，请选择
后者。所有的适配器都理解SMBus level commands，但是不是所有的适配器都理解plain
I2C!
Plain I2C communication
-----------------------
    int i2c_master_send(struct i2c_client *client, const char *buf,
                int count);
    int i2c_master_recv(struct i2c_client *client, char *buf, int count);
这些函数从一个client设备读或写一些字节的数据。client包含了i2c地址，所以你无须
另外指明。第二个参数包含了读/写的buffer，第三个参数包含了读/写的字节数(必须小于
buffer的长度，也应当小于64K(因为msg.len类型为u16))，返回值是读/写的实际字节数。
    int i2c_transfer(struct i2c_adapter *adap, struct i2c_msg *msg,
             int num);
这个函数发送一系列消息。每个消息可以是读，也可以是写，或者读写混合。一个
事务处理(transaction)是可以组合的：在事务处理中没有停止位发送。i2c_msg数据结构
包含了每条消息的client address，消息字节和消息本身。
你可以阅读'i2c-protocol'获取更多的i2c protocol信息。
SMBus communication
-------------------
    s32 i2c_smbus_xfer(struct i2c_adapter *adapter, u16 addr,
               unsigned short flags, char read_write, u8 command,
               int size, union i2c_smbus_data *data);
这是SMBus基础函数。下述的所有函数API基于它实现。不要直接使用这个基础函数！
    s32 i2c_smbus_read_byte(struct i2c_client *client);
    s32 i2c_smbus_write_byte(struct i2c_client *client, u8 value);
    s32 i2c_smbus_read_byte_data(struct i2c_client *client, u8 command);
    s32 i2c_smbus_write_byte_data(struct i2c_client *client,
                      u8 command, u8 value);
    s32 i2c_smbus_read_word_data(struct i2c_client *client, u8 command);
    s32 i2c_smbus_write_word_data(struct i2c_client *client,
                      u8 command, u16 value);
    s32 i2c_smbus_process_call(struct i2c_client *client,
                   u8 command, u16 value);
    s32 i2c_smbus_read_block_data(struct i2c_client *client,
                      u8 command, u8 *values);
    s32 i2c_smbus_write_block_data(struct i2c_client *client,
                       u8 command, u8 length, const u8 *values);
    s32 i2c_smbus_read_i2c_block_data(struct i2c_client *client,
                      u8 command, u8 length, u8 *values);
    s32 i2c_smbus_write_i2c_block_data(struct i2c_client *client,
                       u8 command, u8 length,
                       const u8 *values);
下面的函数从i2c-core里移除掉了，由于它们没有用户需求。
    s32 i2c_smbus_write_quick(struct i2c_client *client, u8 value);
    s32 i2c_smbus_block_process_call(struct i2c_client *client,
                     u8 command, u8 length, u8 *values);
所有的这些函数处理返回一个负值来表示出错。'write'处理返回0表示成功；'read'处理
返回读到的值，block transaction除外(它们返回的是读取的实际字节数)。block
buffers不能超过32字节。
你可以读文件'smbus-protocol'以获取关于SMBus protocol更多的信息。
General purpose routines
========================
下面是之前没有提到的通用函数。
    /* Return the adapter number for a specific adapter */
    int i2c_adapter_id(struct i2c_adapter *adap);
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/guoshaobei/archive/2010/06/08/5656001.aspx](http://blog.csdn.net/guoshaobei/archive/2010/06/08/5656001.aspx)

