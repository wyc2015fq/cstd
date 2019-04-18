# 《Linux设备驱动程序》——USB驱动程序 - 一世豁然的专栏 - CSDN博客





2014年11月25日 21:26:33[一世豁然](https://me.csdn.net/Explorer_day)阅读数：779








一、概论

  1、从拓扑上来看，一个USB子系统并不是以总线的方式来不止的：踏实一颗由几个点对点的链接构造而成的树。这些链接是连接设备和集线器（hub）的四线电缆（地线、电

        源 线和两根信号线）。




  2、USB总线在技术层面是一个单主方式的实现，在此方式下，主机轮询各种不同的外围设备。此外USB另一个重要特性是它只担当设备和主控制器之间通信的角色，对它所

         发送的数据没有任何特殊的内容和结构上的要求。




  3、USB协议规范定义了一套任何特定类型的设备都可以遵循的标准。如果一个设备遵循该标准，就不需要一个特殊的驱动程序。




  4、Linux支持两种主要类型的USB驱动程序：宿主（host）系统上的驱动程序和设备（device）上的驱动程序。




  5、USB驱动程序概观。（P325  图13-1）










二、USB设备基础

 一）、端点

  1、USB通信最基本的形式是通过一个为端点的东西。USB端点只能往一个方向传输数据，从主机到设备（称为输出端点）或者从设备到主机（称为输入端点）。




  2、USB端点有四种不同的类型，分别具有不同的传送数据的方式：

    1）、控制：控制端点用来控制对USB设备不同部分的访问。

    2）、中断：每当USB宿主要求设备传输数据时，中断端点就以一个固定的速率来传送少量的数据。

    3）、批量：批量端点传输大量数据。

    4）、等时：等时端点同样可以传送大批量的数据，但数据是否到达是没有保证的。




  3、内核中用struct  usb_host_endpoint结构体来描述USB端点。该结构体在另一个名为struct  usb_endpoint_descriptor的结构体中包含了真正的端点信息：

    1）、bEndpointAddress：特定端点的USB地址。

    2）、bmAttributes：端点的类型。

    3）、wMaxPacketSize：该端点一次可以处理的最大字节数。

    4）、bInterval：如果端点是中断类型，该值是端点的间隔设置。







 二）、接口

  1、USB端点被捆绑为接口。USB接口只处理一种USB逻辑链接。一些USB设备具有多个接口。USB接口可以有其他的设置，这些适合接口的参数不同的选择。




  2、内核使用struct  usb_interface结构体来描述USB接口。USB核心把该结构体传递给USB驱动程序，之后由USB驱动程序来负责控制该结构体。

    1）、struct  usb_host_interface  *altsetting：一个接口结构体数组，包含了所有可能用于该接口的可选设置。

    2）、unsigned  num_altsetting：altsetting指针所指向的可选设置的数量。

    3）、struct  usb_host_interface  *cur_altsetting：指向altsetting数组内部的指针，表示该接口的当前活动设置。

    4）、int  minor：如果绑定到该接口的USB驱动程序使用USB主设备号，这个变量包含USB核心分配给该接口的次设备号。







 三）、配置

  1、USB接口本身被捆绑为配置。一个USB设备可以有多个配置，而且可以在配置之间切换以改变设备的状态。




  2、Linux使用struct  usb_host_config结构体来描述USB配置，使用struct  usb_devices结构体来描述整个USB设备。




  3、USB设备非常复杂，它由许多不同的逻辑单元组成。这些逻辑单元之间关系可以简单地描述如下：

    1）、设备通常具有一个或更多的配置。

    2）、配置经常具有一个或更多的接口。

    3）、接口通常具有一个或者更多的设置。

    4）、接口没有或者具有一个以上的端点。










三、USB和Sysfs

  1、无论是物理USB设备（用struct  usb_device表示）还是单独的USB接口（用struct  usb_interface表示），在sysfs中表示为单独的设备。




  2、内核如何分类USB设备

    1）、第一个USB设备是一个根集线器（root  hub）。所有的根集线器都由USB核心分配了一个独特的编号。

    2）、USB总线上的每个设备都以根集线器的编号作为其名字中的第一个号码。该号码随后是一个横杆字符和设备插入的端口号。

    3）、在后面是一个冒号加配置的编号加横杆随后是一个接口号。




  3、USB  sysfs设备命名方案为：

      根集线器 - 集线器端口号:配置 . 接口

    1）、对于两层的树：根集线器 - 集线器端口号- 集线器端口号:配置 . 接口




  4、sysfs并没有展示USB设备所有的不同部分，它只限于接口级别。










四、USB  urb

 一）、相关简介

  1、Linux内核中的USB代码通过一个称为urb的东西和所有的USB设备通信。这个请求快使用struct  urb结构体来描述。

    1）、urb被用来以一种异步的方式往/从特定的USB设备上的特定USB端点发送/接受数据。

    2）、USB设备驱动程序可能会为单个端点分配速度哦urb，也可能对许多不同的端点重用单个urb，这取决于驱动程序的需要。

    3）、设备中的每个端点都可以处理一个urb队列，所以多个urb可以在队列为空之前发送到同一端点。




  2、一个urb典型生命周期。







 二）、struct  urb

  1、struct  urb结构体中USB设备驱动程序需要关心的字段（P332-P338）。







 三）、创建和销毁urb

  1、struct  urb结构体不能在驱动程序中或者另一个结构体中静态创建。它必须使用以下函数创建：

        struct  urb  *usb_alloc_urb(int  iso_packets,  int  mem_flags);

    1）、当一个urb被创建之后，在它可以被USB核心使用之前必须被正确地初始化。




  2、驱动程序必须调用以下函数告诉USB核心驱动程序已经用完urb：

        void  usb_free_urb(struct  urb  *urb);







 四）、中断urb

  1、以下函数用来正确的初始化即将被发送到USB设备的中断端点的urb：

        void  usb_fill_int_urb(struct  urb  *urb,  struct  usb_device  *dev,

                                               unsigned  int  pipe,  void  *transfer_buffer,

                                               int  buffer_length,  usb_complete_t  complete,

                                               void  *context,  int  interval);




  2、函数参数。





 五）、批量urb


  1、批量urb的初始化所使用的函数如下：

        void  usb_fill_bulk_urb(struct  urb  *urb,  struct  usb_device  *dev,

                                                   unsigned  int  pipe,  void  *transfer_buffer,

                                                   int  buffer_length,  usb_complete_t  complete,

                                                   void  *context);




  2、函数参数。








 六）、控制urb

  1、控制urb的初始化方法用以下函数：

        void  usb_fill_control_urb(struct  urb  *urb,  struct  usb_device  *dev,

                                                       unsigned  int  pipe,  unsigned  char  *setup_packet,

                                                       void  *transfer_buffer, int  buffer_length, 


                                                       usb_complete_t  complete,void  *context);




  2、函数参数。








 七）、等时urb

  1、等时urb没有和中断、控制和批量urb类似的初始化函数。





 八）、提交urb

  1、一旦被USB驱动程序正确地创建和初始化之后，就可以提交到USB核心以发送到USB设备：

        int  usb_submit_urb(struct  urb  *urb,  int  mem_flags); 




  2、函数参数。







 九）、结束urb：结束回调处理例程


  1、如果usb_submit_urb成功，把对urb的控制权转交给USB核心，该函数返回0；否则，返回负的错误编号。如果函数调用成功，当urb结束的时候urb的结束处理例程正好被

        调用一次。当该函数被调用时，USB核心结束了对URB的处理，此刻对它的控制被返回给设备驱动程序。      





  2、只有三种结束urb和调用结束函数的情形。







 十）、取消urb

  1、应该调用以下两个函数来终止一个已经被提交到USB核心的urb：

        int  usb_kill_urb(struct  urb  *urb);

        int  usb_unlink_urb(struct  urb  *urb);                             










五、编写USB驱动程序

 一）、驱动程序支持那些设备

  1、struct  usb_device_id结构体提供了一系列不同类型的该驱动程序支持的USB设备。USB核心使用该列表来判断对于一个设备该使用哪一个驱动程序，热插拔脚本使用它来

        来确定当一个特定的设备插入到系统时该自动装载哪一个驱动程序。




  2、struct  usb_device_id结构体包括的字段（P343）。




  3、对于PCI设备，有许多用来初始化该结构体的宏（P344）。







 二）、注册USB驱动程序

  1、所有USB驱动程序都必须创建的主要结构体struct  usb_driver。该结构体必须由USB驱动程序来填写，包括许多回调函数和变量，它们像USB核心代码描述了USB驱动程

        序。（P344-P345）




  2、struct  usb_driver还包含了几个回调函数。（P345-P346）




  3、以struct  usb_driver指针为参数的usb_register_driver函数调用把struct  usb_driver注册到USB核心。




  4、当USB驱动程序将要被卸载时，需要把struct  usb_driver从内核中注销。通过调用usb_deregister_driver来完成该工作。








 三）、探测和断开的细节


  1、当一个设备被安装而USB核心认为该驱动程序应该处理时，探测函数被调用；探测函数应该检查传递给它的设备信息，确定驱动程序是否真的适合该设备。当驱动程序因

        为某种原因不应控制设备时，断开函数被调用，它可以做一些清理的工作。




  2、探测和断开回调函数都是在USB集线器内核线程的上下文中被调用的，因此在其中睡眠是合法的。



  3、在探测函数中，USN驱动程序应该初始化任何可能用于控制USN设备的局部结构体，它还应该把所需的任何设备相关信息保存到局部结构体中，因为在此时该工作是比较

        容易的。




  4、struct  usb_class-driver用于定义许多不同的参数，在注册一个设备号时USB驱动程序需要USB核心知道这些函数。（P349-P340）







 四）、提交和控制urb










六、不使用urb的usb传输

 一）、usb_bulk_msg

  1、usb_bulk_msg创建一个USB批量urb，把它发送到指定的设备，然后在返回调用者之前等待它的结束

           int  usb_bulk_msg(struct  usb_device  *usb_dev,  unsigned  int  pipe, 



                                               void  *data,  int  len,  int  &actual_length.  int  timeout);




  2、函数参数。




  3、不能在一个中断上下文或者在持有自旋锁的情况下调用usb_bulk_msg函数。同样，该函数不能被任何其他函数取消，因此使用它的时候要小心；确保驱动程序的断开函数

        了解足够的信息；在允许自身从内存中被卸载之前等待该调用的结束。







 二）、usb_control_msg

  1、除了允许驱动程序发送和接受USB控制消息之处，usb_control_msg函数的运作和usb_bulk_msg函数类似：

        int  usb_control_msg(struct  usb_device  *dev,  unsigned  int  pipe,

                                                 __u8  request,  __u8  requesttype,

                                                 __u16  value,  __16  index,

                                                 void  *data,  __u16  size,  int  timeout);




  2、函数参数。




  3、不能在一个中断上下文或者在持有自旋锁的情况下调用usb_bulk_msg函数。同样，该函数不能被任何其他函数取消，因此使用它的时候要小心；确保驱动程序的断开函数

        了解足够的信息；在允许自身从内存中被卸载之前等待该调用的结束。







 三）、其他USB数据函数

  1、usb_get_descriptor函数从指定的设备获取指定的USB描述符。

      int  usb_get_descriptor(struct  usb_device  *dev,  unsigned  char  type,

                                                  unsigned  char  index,  void  &duf,  int  size);

    1）、USB驱动程序可使用该函数来从struct  usb_device结构体中获取任何没有存在于已有的struct  usb_device和struct  usb_interface结构体中的设备描述符。

    2）、函数参数。




  2、usb_get_string(struct  usb_device  *dev,  unsigned  short  langid,

                                      unsigned  char  index,  void  *buf,  int  size);



