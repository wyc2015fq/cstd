# 《linux设备驱动程序》——Linux设备模型 - 一世豁然的专栏 - CSDN博客





2014年11月28日 10:39:34[一世豁然](https://me.csdn.net/Explorer_day)阅读数：725标签：[Linux驱动																[kernel](https://so.csdn.net/so/search/s.do?q=kernel&t=blog)](https://so.csdn.net/so/search/s.do?q=Linux驱动&t=blog)
个人分类：[linux驱动](https://blog.csdn.net/Explorer_day/article/category/2652125)








一、概论

  1、2.6版内核对系统结构的一般性抽象描述。现在内核使用了该抽象支持了多种不同的任务，其中包括：

    1）、电源管理和系统关机。

    2）、与用户控件通信。

    3）、热插拔设备。

    4）、设备类型。

    5）、对象生命周期。




  2、对模型的大部分来说，Linux模型代码会处理好总线、设备和类等关系，而不把它们强加给驱动程序的作者。

    1）、模型隐藏在交互背后，与设备模型的直接交互通常由总线级逻辑和其他内核子系统来处理。

    2）、许多驱动程序的作者可完全忽略设备模型，并相信设备模型能处理好它所负责的事情。










二、kobject、kset和子系统

 一）、相关简介

  1、kobjsct是组成设备模型的基本结构。现在kobject结构能处理的任务以及它所支持的代码包括：

    1）、对象的引用计数。

    2）、sysfs表述。

    3）、数据结构关联。

    4）、热插事件拔处理。







 二）、kobject基础知识

  1、kobject是一种数据结构，它定义在<linux/kobject.h>中。在这个文件中，还包括了与kobjcet相关结构的声明，当然还有一个用于操作kobject对象的函数清单。




  2、嵌入的kobject

    1）、一个kobject对自身并不感兴趣，它存在的意义在于把高级对象连接到设备模型上。因此，内核代码很少去创建一个单独的kobject对象，相反，kobject用于控制对大型

               域相关对象的访问。为了达到此目的，kobject对象呗嵌入到其他结构中。

    2）、在C语言中不允许直接描述继承关系，因此使用了诸如在一个结构中嵌入另一个结构的技术。




  3、kobject的初始化

    1）、首先是将整个kobject设置为0，这通常使用memset函数。通常在对包含kobject的结构清零时，使用这种初始化方法。

    2）、之后调用以下函数，以便设置结构内部的一些成员；

           void  kobject_init(struct  kobject  *kobj);

    3）、仅仅调用kobject_init是不够的，kobject的使用必须至少设置kobject的名字，这实在sysfs入口中使用的名字

           int  kobject_set_name(struct  kobject  *kobj,  const  char  *format,  .  .  .  );

    4）、kobject的创建者需要直接或间接设置的成员有：ktype、kset和parent。




  4、对引用计数的操作

    1）、kobject的一个重要函数是为包含它的结构设置引用计数。只要对象的引用计数存在，对象（以及支持它的代码）就必须继续存在。底层控制kobject引用计数的函数有

          struct  kobject  *kobject_get(struct  kobject  *kobj);

          void  kobject_put(struct  kobject  *kobj);

         I、对kobject_get的成功调用将增加kobject的引用计数，并返回指向kobject的指针。

         II、当引用被释放时，调用kobject_put减少引用计数，并在可能的情况下释放该对象。

    2）、请注意，在许多情况下，在kobject中的引用计数不足以防止竟态的产生。




  5、release函数和kobject类型

    1）、通常，创建kobject的代码无法知道引用计数为0这种情况会发生什么。

    2）、引用计数不为创建kobject的代码所控制，因此当kobject的最后一个引用计数不再存在时，必须异步通知。通知是使用kobject的release方法实现的；

         void  my_object_release(struct  kobject  *kobi)

         {

               struct  my_object  *mine = container_of(kobj,  struct  my_object,  kobj);

               kfree(mine);


         }

    3）、有一个要点不能忽略：每个kobject都必须有一个release方法，并且kobject在该方法被调用前必须保持不变（处于稳定状态）。如果不能满足这些限制，代码中就会存

              在缺陷。

    4）、release函数并没有包含在kobject自身内，相反，它是与包含kobject的结构类型相关联的。一种称为kobj_type数据负责对该类型进行跟踪。

          struct  kobj_type {

               void  (*release) (struct  kobject  *);

               struct  sysfs_ops  *sysfs_ops;

               struct  attribute  **default_attrs;

          };

    5）、每个kobject都需要有一个相应的kobj_type结构。可以在两个不同的地方找到这个结构的指针。在kobject结构中包含了一个成员（称为ktype）保存了该指针。但是，如

              果kobject是kset的一个成员，kset会提供kobj_type指针。可以利用如下宏查找指定kobject的kobj_type指针。

            struct  kobj_type  *get_ktype(struct  kobject  *kobj);







 三）、kobject层次结构、kset和子系统

  1、通常，内核用kobject结构将各种对象连接起来组成一个分层的结构体系，从而与模型化的子系统相匹配。有两种独立的机制用于连接：parent和kset。

    1）、在kobject结构的parent成员中，保存了另外一个kobject结构的指针，这个结构表示了分层结构上一层的节点。

    2）、对parent指针最重要的用途是在sysfs分层结构中定位对象。




  2、kset

    1）、一个kset是嵌入相同类型结构的kobject集合。但kobj_type结构关心的是对象的类型，而kset结构关心的是对象的聚集和集合。

    2）、kset的主要功能包括：

        I、可以认为它是kobject的底层容器类。实际上，在每个kset内部，包含了自己kobject，并且用多种处理kobject的方法处理kset。需要注意的是，kset总是在sysfs中出

             现。

        II、一旦设置了kset并把它添加到系统中，将在sysyfs中创建一个目录。kobject不必再sysfs中表示，但是kset中的每一个kobject成员都将在sysfs得到表述。

    3）、创建一个对象，通常把一个kobject添加到kset中去。这有连个步骤：

        I、先把kobject的kset成员要指向目的kset。

        II、然后将kobject传递下面函数：

             int  kobject_add(struct  kobject  *kobj);

        III、另一个函数

             extern  int  kobject_register(struct  kobject  *kobj);

    4）、把kobject从kset删除，以清除引用；使用下面的函数达到这个目的：

            void  kobject_del(struct  kobject  *kobj);

       或 extern  int  kobject_unregister(struct  kobject  *kobj);

    5）、kset在一个标准的内核链表中保存了它的节点。在大多数情况下，所包含的kobject会在它们的parent成员中保存kset的指针。




  3、kset上的操作


    1）、lset拥有与kobject相似的初始化和设置接口。

         void   kset_init(struct  kset  *kset);

         int  kset_add(struct  kset  *kset);

         int  kset_register(struct  kset  *kset);

         void  kset_unregister(struct  kset  *kset);

    2）、为了管理kset的引用计数。

         struct  kset  *kset_get(struct  kset  *kset);

         void  kset_put(struct  kset  *kset);

    3）、一个kset也拥有名字，其保存在内嵌的kobject中。其可以用kobject_set_name()命名。

    4）、kset中也有一个指针（在ktype成员中）指向kobj_type结构，用来描述它所包含的kobject。该类型的使用优先于kobject中的ktype。




  4、子系统

    1）、子系统通常（但不一定）显示在sysfs分层结构中的顶层。内核中的子系统包括block_subsys、devices_subsys以及所有内核所知晓的用于各种总线的特定子系统。

    2）、一个子系统其实是对kset和一个信号量的封装。

    3）、每个kset都必须属于一个子系统。子系统的成员将帮助内核在分层结构中定位kset，但更重要的是，子系统的rwsem信号被用于访问内部的链表。在kset结构中，这种

              成员关系被表示为subsys指针。

    4）、使用下面的宏声明subsystem：

          decl_subsys(name,  struct  kobj_type  *type,  


                                   struct  kset_hotplug_ops  *hotplug_ops);

    5）、子系统拥有一个设置和销毁函数列表

         void  subsystem_init(struct  subsystem  *subsys);

         int  subsystem_register(struct  subsystem  *subsys);

         void  subsystem_unregister(struct  subsystem  *subsys);

         struct  sybsystem  *subsys_get(struct  subsystem  *subsys);

         void  subsys_put(struct  subsystem  *subsys);










三、低层sysfs操作


 一）、相关简介

  1、kobject是隐藏在sysfs虚拟文件系统后的机制，对于ysysfs中的每个目录，内核中都会存在一个对应的kobject。每一个kobject都输出一个或多个属性，它们在kobject的

        sysfs目录中表现为文件，其中的内容由内核生成。       




  2、kobiect相关知识。







 二）、默认属性

  1、当创建kobject的时候，都会给每个kobject一系列默认属性。这些属性保存在kobj_type结构中。

             struct  kobj_type {

               void  (*release) (struct  kobject  *);

               struct  sysfs_ops  *sysfs_ops;

               struct  attribute  **default_attrs;

             };

    1）、default_attrs成员保存了属性列表，用于创建该类型的每一个kobject，sysfs_ops提供了实现这些属性的方法。

            struct  attribute {

               char  *name;

               struct  module  *owner;

               mode_t  mode;

            };




  2、sysfs_ops成员实现了以上属性。

           struct  sysfs_ops {

               ssiae_t  (*show) (struct  kobject  *kobj,  struct  attrbute  *attr,  char  *buffer);

               ssiae_t  (*store) (struct  kobject  *kobj,  struct  attrbute  *attr,  const  char  *buffer,  size_t  size);

           };







 三）、非默认属性

  1、如果希望在attribute结构中添加新的属性，只需要填写一个attribute结构，并把它传递给下面的函数：

        int  systs_create_file(struct  kobject,  struct  attribute  *attr);




  2、注意那些对新属性调用同一show()和store()函数以实现操作的情况。再添加一个非默认属性前，应采取必要的步骤以保证这些函数知道如何利用这些属性。




  3、调用下面的函数删除属性：

        int  sysfs_remove_file(struct  kobject  *kobj,  struct  attribute  *attr);







 四）、二进制属性

  1、可以用bin_attribute结构描述二进制属性：

        struct  bin_attribute {

            struct  attribute  attr;

            size_t  size;

            ssize_t (*read) (struct  kobject  *kobj,  char  &buffer,

                                          loff_t  pos,  size_t  size);

            ssize_t (*write) (struct  kobject  *kobj,  char  *buffer,

                                          loff_t  pos,  size_t  size);

        };




  2、必须显式的创建二进制属性，也就是说，它们不能作为默认被设置。调用下面的函数可创建二进制属性：

         int  sysfs_create_bin_file(struct  kobject  *kobj,  struct  bin_attribute  *attr);




  3、使用下面的函数删除二进制属性

         int  sysfs_remove_bin_file(struct  kobject  *kobj,  struct  bin_attribute  *attr);







 五）、符号链接

  1、树形结构不能表示表示驱动程序及其所管理的设备之间的关系。为了表示这种关系还需要其他的指针，在sysfs中，通过符号链接实现了这个目的。




  2、在sysfs中创建符号链接时使用下面的函数：

        int  sysfs_create_link(struct  kobject  *kobj,  struct  kobject  *target,  char  *name);




  3、用下面的函数删除符号链接：

        int  sysfs_remove_link(struct  kobject  *kobj,  char  *name);










四、热插拔事件的产生

 一）、相关简介

  1、一个热插拔事件是从内核空间发送到用户控件的通知，它表明系统配置出现了变化。无论kobject被创建还是被删除，都会产生这种事件。




  2、热插拔事件会导致对/sbin/hotplug程序的调用，该程序通关过加载驱动程序，创建设备节点，挂载分区，或者其他正确的动作来相应这种事件。







 二）、热插拔操作

  1、对热插拔时间的实际控制，是由保存在kset_hotplug_ops结构中的函数完成的：

        struct  kset_hotplug_ops {

            int  (*filter) (struct  kset  *kset,  struct  kobject  *kobj);

           char  *(*name) (struct  kset  *kset,  struct  kobject  *kobj);

            int  (*hotplug) (struct  kset  *kset,  struct  kobject  *kobj,

                                       char  **envp,  int  num_envp,  char  *buffer,

                                       int  buffer_size);

        };




  2、可以在kset结构的hotplug_ops成员中发生指向这个结构的指针。如果kset中不包含一个指定的kobject，内核将在分层结构中进行搜索（通过parent指针），直到找到一个

        包含有kset的kobject为止，然后使用这个kset的热插拔操作。




  3、无论什么时候，当内核要为指定的kobject产生事件时，都要调用filter函数。如果filter返回0，将不产生事件。




  4、在调用用户空间的热插拔程序时，相关子系统的名字将作为唯一的参数传递给它。hotplug方法负责提供这个名字，它将返回一个适合传递给用户空间的字符串。




  5、任何热插拔脚本需要知道的信息将通过环境变量传递。hotplug方法会在调用脚本之前，提供添加环境变量的机会。










五、总线、设备和驱动程序

 一）、总线

  1、总线是处理器与一个或多个设备之间的通道。在设备模型中，所有的设备都通过总线相连。甚至是那些内部的虚拟“平台”总线。




  2、在Linux设备模型中用bus_type结构表示总线，它的定义包含在<linux/device.h>中。

        struct  bus_type {

            char  *name;

            struct  subsystem  subsys;

            struct  kset  drivers;

            struct  kset  devices;

            int  (*match) (struct  device  &dev,  struct  device_driver  *drv);

            struct  device  *(*add) (struct  device  * parent,  char  8  bus_id);

            int  (*hotplug) (struct  device  *dev,  char  **envp,  int  num_envp,  char  *buffer,  int  buffer_size);

            /*  省略了一些成员  */

         }；




  3、总线的注册

    1）、对于新的总线，我们必须调用bus_register进行注册。请注意，只有非常少的bus_type成员需要初始化，它们中的大多数都由设备模型核心所控制。

    2）、当有必要从系统中删除一个总线时，要使用bus_unregister函数。




  4、总线方法

    1）、int  (*match) (struct  device  *device,  struct  device_driver  *driver);

              当一个总线上的新设备或者新驱动被添加时，会一次货多次调用这个函数。

    2）、int  (*hotplug) (struct  device  *device,  char  **envp,  int  num_envp,  char  *buffer,  int  buffer_size);

              在为用户空间产生热插拔前，这个方法允许总线添加环境变量。




  5、对设备和驱动程序的迭代

    1）、为了操作注册到总线的每个设备，可使用。

               int  bus_for_each_dev(struct  bus_type  *bus,  struct  device  *start,

                                                         void  *data,  int  (*fn) (struct  device  *,  void  *));

    2）、相似的函数也可用于驱动程序的迭代上：

               int  bus_for_each_drv(struct  bus_type  *bus,  struct  device  *start,

                                                         void  *data,  int  (*fn) (struct  device  *,  void  *));




  6、总线属性

    1）、Linux设备总线层提供了添加属性的函数，bus_attribute类型在<linux/device.h>中定义：

              struct  bus_attribute {

                 struct  attribute  attr;

                 ssize_t  (*show) (struct  bus_type  *bus,  char  *buf);

                 ssize_t  (*store) (struct  bus_type  *bus,  const  char  *buf,  size_t  count);

               };


    2）、有可在编译时刻创建和初始化bus_attribute结构：

              BUS_ATTR(name,  mode,  show,  store);

    3）、创建属于总线的任何属性，都需要显式调用以下函数：

              int  bus_create_file(struct  bus_type  *bus,  struct  bus_attribute  *attr);

    4）、用下面的函数删除属性：

              void  bus_remove_file(struct  bus_type  *bus,  struct  bus_attribute  *attr);







 二）、设备


 1、在最底层，Linux系统中的每一个设备都用device结构的一个实例来表示：

       struct  device {

           struct  device  *parent;

           struct  kobject  kobj;

           char  bus_id[BUS_ID_SIZE];

           struct  bus_type  *bus;

           struct  device_driver  *driver;

           void  *driver_data;

           void  (*release) (struct  device  *dev);

           /*  省略了几个成员  */

        }；

    1）、struct  device  *parent：设备的“父设备”——指的是该设备所属的设备。

    2）、struct  kobject  kobj：表示设备并把它连接到结构体系中的kobject。

    3）、char  bus_id[BUS_ID_SIZE]：在总线上惟一标识该设备的字符串。

    4）、struct  bus_type  *bus：标识了该设备链接在何种类型的总线上。

    5）、struct  device_driver  *driver：管理该设备的驱动程序。

    6）、void  *driver_data：由设备驱动程序使用的私有数据成员。

    7）、void  (*release) (struct  device  *dev)：当指向设备的最后一个引用被删除时，内核调用该方法；它将从内嵌的kobject的release方法调用。




  2、设备注册

        int  device_register(struct  device  *dev);

        void  device_unregister(struct  device  *dev);




  3、设备属性

         struct  device_attribute {

             struct  attribute  attr;

             ssize_t  (*show) (struct  device  *dev,  char  *buf);

             ssize_t  (*store) (struct  device  *dev,  const  char  *buf,  size_t  count);

            };

    1）、可以在编译时刻用下面的宏构造这些attribute结构：

              DEVICE_ATTR(name,  mode,  show,  store);

    2）、可以用下面的两个函数实现对属性文件的实际处理：

              int  device_create_file(struct  device  *device,  struct  device_attribute  *entry);

              void  device_remove_file(struct  device  *dev,  struct  device_attribute  *attr);




  4、设备结构的嵌入

    1）、device结构中包含了设备模型核心用来模拟系统的信息。然而，大多数子系统记录了它们所拥有设备的其他信息，因此，单纯用device结构表示的设备是很少见的，二

              十通常把类似kobject这样的结构内嵌在设备的高层之中。

    2）、lddbus驱动程序和sculld驱动程序实例。




 三）、设备驱动程序

  1、设备模型跟踪所有系统所知道的设备。进行跟踪的主要原因是让驱动程序核心协调驱动程序与新设备之间的关系。一旦驱动程序是系统中的已知对象，就可能完成大量的工

         作。




  2、驱动程序由以下结构定义：

        struct  device_driver {

           char  *name;

           struct  bus_type  *bus;

           struct  kobject  kobj;

           struct  list_head  devices;

           int  (*probe) (struct  device  *dev);

           int  (*remove) (struct  device  *dev);

           void  (*shutdown) (struct  device  *dev);

        }；




  3、操作device_driver结构的函数形式：

        int  driver_register(struct  device_driver  *dev);

        void  driver_unregister(struct  device_driver  *dev);




  4、常用的属性结构是：

         struct  driver_attribute {

            struct  attribute  attr;

            ssize_t  (*show) (struct  device_driver  *drv,  char  *buf);

            ssize_t  (*store) (struct  device_driver  *drv,  const  char  *buf,  size_t  count);

         };

          DRIVER_ATTR(name,  mode,  show,  store);




  5、使用下面的函数创建属性文件：

        int  driver_create_file(struct  device_driver  *drv,  struct  drivere_attribute  *sttr);

         void  driver_remove_file(struct  device_driver  *drv,  struct  drivere_attribute  *sttr);




  6、驱动程序结构的嵌入








六、类

 一）、相关简介

  1、类是一个设备的高层视图，它抽象除了底层的实现细节。类允许用户kona进使用设备所提供的功能，而不关心设备是如何连接的，以及它们是如何工作的。




  2、几乎所有的类都显示在/sys/class目录中。




  3、类成员通常被上层代码所控制，而不需要来自驱动程序的明确支持。




  4、任何情况下，类子系统是向用户空间导出信息的最好办法。




  5、为了管理类，驱动程序核心道出了两个不同的接口。







 二）、class_simple接口

  1、第一步就是创建类本身

        struct  class_simple  *class_simple_create(struct  module  *owner,  char  *name);




  2、可以用下面函数销毁一个简单类：

        void  class_simple-destroy(struct  class_simple  *cs);




  3、创建一个简单类的真实目的是为它添加设备，可以使用下面的函数达到这一目的：

        struct  class_device  *class_simple_device_add(struct  class_simple  *cs,  dev_t  devnum,

                                                                                                  struct  device  *device,  const  char  *fmt,  .  .  .  );




  4、在插拔设备时，类会产生热插拔事件。如果驱动程序需要为用户控件处理程序添加环境变量的话，可以用下main的代码设置热插拔回调函数：

        int  class_simple_hotplug(struct  class_simple  *cs, 


                                                         int  (*hotplug) (struct  class_device  *dev, 


                                                                                    char  **envp,  int  num_envp,

                                                                                    char  *buffer,  int  buffer_size));




  5、当拔出设备时，使用下面的函数删除类入口：

        void  class_simple_device_remove(dev_t  dev);







 三）、完整的类接口

  1、管理类

    1）、用class结构的一个实例来定义类：

              struct  class {

                 char  *name;

                 struct  class_attribute  *class_attrs;

                 struct  class_device_attribute  *class_dev_attrs;

                 int  (*hotplug) (struct  class-device  *dev  ,char  **envp,

                                            int  num_envp,  char  *buffer,   int  buffer_size);

                 void  (*release) (struct  class_device  *dev);

                 void  (*class_release) (struct  class  *class);

               };

    2）、注册函数是：

              int  class_register(struct  class  *cls);

              void  class_unregister(struct  class  *cls);

    3）、处理属性接口函数：

             struct  class_attribute {

                struct  attribute  attr;

                ssize_t  (*show) (struct  class  *cls,  char  *buf);

                ssize_t  (*store) (struct  class  *cls,  const  char  *buf,  size_t  count);

              };

              CLASS_ATTR(name,  mode,  show,  store);

              int  class_create_file(struct  class  *cls,  const  struct  class_attribute  *attr);

              void  class_remove_file(struct  class  *cls,  const  struct  class_attribute  *attr);




  2、类设备

    1）、类存在的真正目的是，给作为类成员的各个设备提供一个容器。用class_device结构表示类的成员：

              struct  class_device {

                 struct  kobject  kobj;

                 struct  class  *class;

                 struct  device  *dev;

                 void  *class_data;

                 char  class_id[BUS_ID_SIZE];

               }；

    2）、常用的注册函数如下：

             int  class_device_register(struct  class_device  *cd);

             void  class__class_unregister(struct  class_device  *cd);

    3）、类设备接口还允许已经注册过的入口项改名：

             int  calss_device_rename (struct  class_device  *cd,  char  *new_name);

    4）、类设备入口具有属性：

             struct  class_device  attribute {

                struct  attribute  attr;

                ssize_t  (*show) (struct  class_device  *cls,  char  *buf);

                ssize_t  (*store) (struct  class_device  *cls,  const  char  *buf,  size_t  count);

              };

              CLASS__DEVICE_ATTR(name,  mode,  show,  store);

              int  class_create_file(struct  class_device  *cls,  const  struct  class_device_attribute  *attr);

              void  class_device_remove_file(struct  class_device  *cls,  const  struct  class_device_attribute  *attr);




  3、类接口


    1）、一个接口由下面的结构表达：

              struct  class_interface {

                 struct  class  *class;

                 int  (*add) (struct  class_device  *cd);

                 void  (*remove) (struct  class_device  *cd);

              };

    2）、可以用下面的函数注册和注销接口：

              int  class_interface_register(struct  class_interface  *intf);

              void  class_interface_unregister(struct  class_device  *cd);








七、各环节整合（略）










八、热插拔

 一）、相关简介

  1、有两个不同的角度来看待热插拔

    1）、从内核角度看，热插拔是硬件、内核、内核驱动程序之间的交互。

    2）、从用户的角度看，热插拔是在内核与用于之间，通过调用/sbin/hotplug程序的交互。当需要通知用户内核中发生了某些类型的热插拔事件时，内核才调用该函数。







 二）、动态设备

  1、随着某些设备的出现，要求Linux内核能够可靠的运行，而不管运行过程中在系统中添加或删除了什么硬件。其必须处理设备被毫无征兆地突然拔走的情况。




  2、每种不同的总线使用不同的方法处理设备的移除。




  3、热插拔并不仅仅局限在传统设备上，现在的Linux内核能正确处理添加、删除一些新型的硬件核心的“系统”设备。







 三）、/sbin/hotplug工具

  1、但用户向系统添加或者删除设备时，会产生热插拔事件。这会导致内核调用用户空间程序/sbin/hotplug。该程序是一个典型的bash脚本程序，知识将执行权传递给其他一

        系列放置在/etc/hotplug.d/目录中的程序。




  2、当kobject被创建或者删除时调用/sbin/hotplug。内核会使用具有一个参数的命令调用hotplug程序，该参数就是事件的名字。核心内内核及相关子系统也设置了一系列的环

        境变量用来描述所发生的事件。




  3、传递给/sbin/hotplug的命令行参数是热插拔时间的相关名称，由分派给kobject的kset来确定。可以调用kset的hotplug_ops结构中的name参数来设置这个名字。




  4、为/sbin/hotplug设置的默认环境变量是：

    1）、ACTION：根据操作是创建对象还是删除对象，设置字符串是“add”或者是“remove”。

    2）、DEVPTH：在sysfs文件系统中的目录路径，指向被创建的或者被删除的kobject。

    3）、SEQNUM：热插拔事件的序号。

    4）、subsystem：传递给命令行的字符串参数。




  5、IEEE1394(FireWire)

    1）、IEEE1394总线——也称之为“火线”上的任何设备，将/sbin/hotplug的参数名和SUBSYSTEM环境变量设置为ieee1394。

    2）、ieee1394子系统总是添加下面五个环境变量

      I、VENDOR_ID：IEEE1394涉笔的24位厂商ID。

      II、MODEL_ID：IEEE1394设备的24位型号ID。

      III、GUID：设备的64位GUID。

      IV、SPECIFIER_ID：指定设备协议所有者的24位ID。

      V、VERSION：指定设备协议版本号的值。




  6、网络

    1）、当网络设备在内核中注册和注销时，所有的网络设备都产生热插拔事件。

    2）、/sbin/hotplug程序将参数名和SUBSYSTEM变量设置为net，并且添加以下变量：

       I、INTERFACE：向内核注册或者注销的接口名称。




  7、PCI

    1）、在PCI总线上的所有设备将参数名和SUBSYSTEM环境变量设置为pci。

    2）、PCI子系统添加下面洗个环境变量：

       I、PCI_CLASS：以十六进制表示的PCI类号。

       II、PCI_ID以十六进制表示的PCI厂商和设备ID，按照vendor:device形式结合。

       III、PCI_SLOT_ID：PCI子系统厂商和子系统设备ID，按照subsys_vendor:subsys_device新式结合。

       IV、PCI_SLOT_NAME：内核给设备的PCI槽名字，它以domain:bus:slot:function的方式结合。




  8、输入

    1）、对于所有的输入设备，当设备从内核中添加或删除时，都要产生热插拔事件，/sbin参数和SUBSYSTEM环境变量都设置为input。

    2）、输入子系统都会添加下面的环境变量：

       I、PRODUCT：一个用十六进制表示，没有前置零的多值字符串列表值。

            如果设备支持，会有下面的环境变量：

            I、NAME：设备给出的输入设备名。

            II、PHYS：输入子系统分配给设备的物理地址。

            III、EV|KEY|REL|ABS|MSC|LED|SND|FF：舒适设备描述符，如果输入设备支持，将设置正确的值。




  9、USB

    1）、任何在USB总线上的设备，都将把参数名和SUBSYSTEM环境变量设置为usb。

    2）、USB子系统也总是添加下面的环境变量：

       I、PRODUCT：以idVendor/idProduct/bcdDevice格式表示的字符串，用来指定这些USB的设备相关成员。

       II、TYPE：以bDeviceClass/bDeviceSubCckass/bDeviceProtocol格式表示的字符串，用来指定这些USB的设备相关成员。

              如果bDeviceClass成员为0，则还要设置下面的环境变量：

       I、INTERFACE：以bInterfaceClass/bInterfaceSubCckass/bInterfaceProtocol格式表示的字符串，用来指定这些USB的设备相关成员。

           如果使用了内核编译选项CONFIG_USB_DEVICEFS，就会将usbfs文件系统编译进内核，则还需要设置下面的环境变量。

       I、DEVICE：显示设备在usbfs文件系统中位置的字符串。




  10、SCSI

    1）、当从内核中创建或删除任何SCSI设备时候，设备会产生一个热插拔事件。调用/sbin/plug程序的参数名以及SUBSYSTEM设置为scsi。

    2）、SCSI系统不再添加其他的环境变量。




  11、便携对接站

    1）、如果从正在运行的Liunx系统中添加或删除一个支持即插即用的便携对接站，会产生一个热插拔事件。

    2）、调用/sbin/hotplug程序的参数名称以及SUBSYSTEM环境变量设置为dock。除此之外，不会设置其他环境变量。




  12、S/390和zSeries

    1）、在S/390习题架构中，通道总线体系架构支持大量的硬件设备。当从Linux虚拟系统中添加或删除这些设备时，都能产生/sbin/hotplug事件。这些设备将/sbin/hotplug的

              参数名称以及SUBYSYTEM环境变量设置为dasd。除此之外，不会设置其他环境变量。







 四）、使用/sbin/hotplug

  1、当向内恶化添加或删除任何设备时，Linux内核将调用/sbin/hotplug，为此，用户空间存在许多有用的工具以便利用此特性。其中两个最有用的工具是Linux Hotplug脚本和

        udev。










九、处理固件

 一）、相关简介

  1、对于某些设备。必须先把固件下载到设备后，它才能正常工作。







 二）、内核固件接口

  1、使用固件接口：

        #include<linux/firmware.h>

        int  request_firmware(const  struct  firmware  **fw,  char  *name,  struct  device  *device);




  2、再把固件发送到设备设备后，需要使用下面的函数释放内核中的结构：

        void  release_firmware(struct  firmware  *fw);




  3、异步函数：

        void  request_firmware_nowait(struct  module  *module,  char  name,  struct  device  *device,

                                                                  void  *context,  void  (*cont) (const  struct  *fw,  void  context));







 三）、工作原理（略）




