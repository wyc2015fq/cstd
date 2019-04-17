# 《Linux内核设计与实现》——设备与模块 - 一世豁然的专栏 - CSDN博客





2014年12月01日 10:19:58[一世豁然](https://me.csdn.net/Explorer_day)阅读数：730








一、设备类型

  1、在Linux以及所有Unix系统中，设备被分为以下三类：

    1）、块设备。

    2）、字符设备。

    3）、网络设备。




  2、块设备通常缩写blkdev，它是可寻址的，寻址以块为单位，块大小随设备不同而不同；块设备通常支持重定位操作，也就是对数据的随机访问。




  3、字符设备通常缩写为cdev，它是不可寻址的，仅提供数据的流式访问，就是一个个字符或者一个个字节。字符设备通过称为“字符设备节点”的特殊文件来访问的，与块设备

        不同，应用程序通过直接访问设备节点与字符设备交互。




  4、网络设备最常见的类型有时也以以太网设备来称呼，它提供了对网络的访问，这是通过一个物理适配器和一个特定的协议进行的。网络设备不是通过设备来访问的，而是通

        过 套接字API这样的特殊借口来访问的。




  5、Linux还提供了不少其他设备类型，但都是针对单个任务，而非通用的。一个特列是“杂项设备”，通常简写miscdev。




  6、并不是所有设备驱动都表示为物理设备，有些设备驱动是虚拟的，仅提供访问内核功能而已，我们称之为“伪设备”。










二、模块

 一）、Hello， World

  1、模块的所有初始化函数必须符合下面的形式：

        int  my_init(void);




  2、退出函数必须符合下列形式：

        void  my_exit(void);




  3、MODULE_LICENSE()宏用于指定模块的版权。MODILE_AUTHOR()宏和MODULE_DESCRIPTION宏指定了代码作者和模块的简要描述，它们完全是用作信息记录目的。







 二）、构造模块

  1、构造模块的第一步是决定在哪里管理模块源码。

    1）、可以把模块源码加入到内核源代码树中，或者是作为一个补丁或者是把你的代码合并到正是的内核代码树中。

    2）、另一种可行的方式是在内核源代码之外维护和构建你的模块源码。




  2、放在内核源代码中

    1）、当决定把模块放入内核源代码中，下一步要清楚你的模块应在内核源代码树中处于何处。设备驱动程序存放在内核源码树根目录下/driver的子目录下，在其内部，设备

              驱动文件被进一步按照类别、类型或特殊驱动程序等更有序地组织起来。

    2）、实例：Fish  Master  XL  3000。




  3、放在内核代码外







 三）、安装模块

  1、下面的构建命令用来安装编译的模块到合适的目录下：

         make  module_install







 四）、产生模块依赖性

  1、若想产生模块依赖性，root用户可运行命令：

         depmod

        为了更快执行的更新操作，那么只为新模块生成依赖信息，而不是生成所有的依赖关系：

        depmod  -A







 五）、载入模块

  1、载入模块可以用最简单的**insmod命令**，insmod程序不执行任何依赖性分析或进一步的错误检查：

        insmod  module.ko





  2、类似的，卸载一个模块，可使用rmmod命令：

         rmmod  module




  3、modprode提供了模块依赖性分析，错误只能检查、错误报告以及许多其他功能和选项。

        modprode  module  [ module  parameters ]

        modprode命令也可用来从内核中卸载模块

        modprode  -r  modules







 六）、管理配置项

  1、如果建立了一个新目录，而且也希望kconfig文件存在于该目录中的话，则应该在一个已存在的kconfig文件将它引入。




   2、Kconfig文件解析

    1）、第一行定义了该选项所代表的配置目标。

    2）、第二行声明了选项类型为tristate，也就是说可以编译成内核（Y），也可作为模块编译（M），或者干脆不编译它（N）。

    3）、第三行指定了该选项的默认选项。

    4）、Help指令的目的是为该编译选项提供帮助文档。




  3、Select指令——只要是其指定的，它就会强行将被指定的选项打开。

    1）、如果select和depends同时指定多个选项，那就需要通过&&指令来进行多选。




  4、tristate和bool选项往往会结合if指令一起使用，这表示某个选项取决于另一个配置选项。如果条件不满足，配置选项不但会被禁止，甚至不会显示在公知工具中。







 七）、模块参数

  1、定义一个模块参数可通过过宏module_param()完成：

        module_oaram(name,  type,  pwerm);




  2、有可能模块的外部名称不同于它对应的内部变量名称，这是就应该使用宏：

        module_param_named(name,  variable,  type,  perm);




  3、如果需要，也可使用内核直接拷贝字符串到指定的字符串数组。

        module_param_string(name,  string,  len,  perm);




  4、可接受逗号分隔的参数序列，这些参数可通过以下宏存储在C数组中：

        module_param_array(name,  type,  nump,  perm);




  5、可以将内部参数数组命令区别于外部参数：

        module_param_array_named(name,  array,  type,  nump,  perm);




  6、可使用MODULE_PARM_DESC()描述你的参数。







 八）、导出符号表

  1、在内核中，导出内核函数需要使用特殊的指针：EXPORT_SYMBOL()和EXPORT_SYMBOL_GPL()。




  2、导出的内核函数可以被模块调用，而未导出的函数模块则无法被调用。










三、设备模型

 一）、kobject

  1、设备模型的核心部分就是kobject，它由**struct  kobject结构体**表示，定义在头文件<linux/kobject.h>中。

        struct  kobject {

            const  char  *name;

            struct  list_head  entry;

            struct  kobject  *parent;

            struct  kset  *kset;

            struct  kobj_type  *ktype;

            struct  sysfs_dirent  *sd;

            struct  kref  kref;

            unsigned  int  state_initialized:1;

            unsigned  int  state_in_sysfs:1;

            unsigned  int  state_add_uevent_sent:1;

            unsigned  int  state_remove_uevent_sent:1;

            unsigned  int  uevent_suppress:1;

         };




  2、kobject通常是嵌入其他结构中时，其单独意义不大。







 二）、ktype

  1、kobject被关联到一种特殊的类型，即ktype。ktype由kobject结构体表示，定义在头文件<linux/kobject>中。

         struct  kobj_type  {

             void  (*release)(struct  kobject  *);

             const  struct  sysfs_ops  *sysfs_ops;

             struct  attribute  **default_attrs;

         };

    1）、ktype的存在是为了秒sohu一组kobject所具有的普遍性。







 三）、kset

  1、kobject的kset指针指向相应的kset集合。kset集合由kset结构体表示，定义于头文件<linux/kobject.h>中。

        struct  kset {

            struct  list_head  list;

            spinlock_t  list_lock;

            struct  kobject  kobj;

            struct  kset_uevent_ops  *uevent_ops;

        };




  2、kset是kobject对象的集合体。







 四）、kobject、ktype和kset的相互关系

  1、kobject与一个特别的ktype对象关联，ktype由struct  kobj_type结构体表示，在kobject中ktype字段指向该对象。ktype定义了一些kobject相关的默认特性：析构行为（反析

        构行为）、sysfs行为（sysfs操作表）以及别的一些默认属性。




  2、kobject又归入了称作kset的集合，kset集合由struct  kset结构体表示。kset提供了两个功能。

    1）、第一，其中嵌入的kobject作为kobject组的基类。

    2）、kset将相关的kobject集合在一起。







 五）、管理和操作kobject

  1、使用kobject的第一步需要先来声明和初始化。

        void  kobject_init(struct  kobject  *kobj,  struct  kobj_type  *ktype);




  2、创建一个新的kobject

        struct  kobject  *kobject_create(void);







 六）、引用计数

  1、递增和递减引用计数

    1）、添加一个引用计数可以通过一下函数：

               struct  kobject  *  kobject_get(struct  kobject  *kobj);

    2）、减少一个引用计数可以通过以下函数：

               void  kobject_put(struct  kobject  *kobject);




  2、kref

    1）、在引用计数系统的内部，会发现kobject的引用计数是通过kref结构体实现的，该结构都定义在文件<linux/kref.h>中：

              struct  kref  {

                  atomic_t  refcount;

              };

    2）、可通过kref_init()初始化struct  kref结构：

              void  kref_init(struct  kref  *kref)

              {

                   atomic_set(&kref->recount,  1);


              }

    3）、要获得对kref的引用，需要调用kref_get()函数，这个函数声明在<linux/kref.h>中：

              void  kref_get(struct  kref  *kref)

              {

                   WARN_ON(!atomic_read(&kref->refcount));

                   atomic_inc(&kref->refcount);


              }

    4）、减少对kref的引用，调用声明在<linux/kref.h>中的函数kref_put();

              int  kref_put(struct  kref  *kref, (void  (*release)  (struct  kref  *kref))

              {

                   WARN_ON(release  ==  NULL);

                   WARN_ON(release  ==(void  (*)  (struct  kref  *)) kfree);

                   if(atomic_dec_and_test(&kref->refcount)) {

                           release(kref);

                           return  1;

                    };


              }










四、sysfs

 一）、相关简介

  1、sysfs文件系统是一个处于内存中的虚拟文件系统，它为我们提供了kobject对象层次结构视图。帮助用户能以一个简单文件系统的方式来观察系统中各种设备的拓扑接口。

        借助属性对象，kobject可以用导出文件的方式，将内核变量提供给用户读取或写入（可选）。




  2、挂载玉/sys目录下的sysfs文件系统的局部视图。（P289  图17-2）

    1）、sysfs的根目录包含了至少是个目录：block、bus、class、dev、devices、firmware、fs、kernel、module和power。







 二）、sysfs中添加和删除kobject

  1、仅仅初始化kobject是不能自动将其导出到sysfs中的，想要把kobject导入sysfs，需要用到函数kobject_add()：

         int  kobject_add(struct  kobject  *kobj,  struct  kobject  *parent,  const  char  *fmt,  .  .  .  );




  2、辅助函数kobject_create_and_add()把kobject_add和kobject_create()所做的工作放在一个函数中：

        struct  kobject  *kobject_create_and_add(const  char  *name,  struct  kobject  *parent);




  3、从sysfs中删除一个kobject对应的文件目录，需要使用函数kobject_del()：

        void  kobject_del(struct  kobject  *kobj);







 三）、向sysfs中添加文件

  1、默认属性

    1）、默认属性的描述使用struct  attribute结构体：

              struct  attribute  {

                  const  char  *name;

                  struct  module  *owner;

                  mode_t  mode;

              };

    2）、struct  sysfs_ops结构体描述了如是用默认属性：

              struct  sysfs_ops  {

                  ssize_t  (*show)  (struct  kobject  *kobj,  struct  attribute  *sttr,  char  *buffer);

                  ssize_t  (*store)  (struct  kobject  *kobj,  struct  attribute  *attr,  const  char  *buffer,  size_t  size);

              };




  2、创建新属性

    1）、有些kobject实例希望（甚至必须）有自己的属性——也就是通用属性没包含那些需要的数据或函数。为此，内核能在默认集合智商，在添加新属性：

              int  sysfs_create_file(struct  kobject  *kobj,  const  struct  attribute  *attr);

    2）、除了添加文件外，还有可能需要创建符号链接。

              int  sysfs_create_link(struct  kobject  *kobj,  struct  kobject  *target,  char  *name);




  3、删除新属性

    1）、删除一个属性需通过函数sysfs_remove_file()完成

              void  sysfs_remove_file(struct  kobject  *kobj,  const  struct  attribute  *attr);

    2）、删除由sysfs_create_link()创建的符号链接可通过函数sysfs_remove_link()函数：

              void  sysfs_remove_link(struct  kobject   *kobj,  char  *name);




  4、sysfs约定

    1）、首先，开发者应该保证每个文件只导出一个值，该值应该是文本形式而且映射为简单C类型。

    2）、其次，在sysfs中要以一个清晰的层次组织数据。

    3）、最后，记住sysfs提供内核到用户空间的服务，这多少有些用户的ABJ的作用。







 四）、内核事件层

  1、内核事件层实现了内核到用户的消息通知系统。

    1）、内核事件层把事件模拟为信号——从明确的kobject对像发出，所以每个事件源都是一个sysfs路径。如果请求的事件与你的第一个硬盘相关，那么/sys/block/had便是源

              树。实际上，在内核中我们认为事件都是从幕后的kobject对象产生的。

    2）、没饿过事件都被赋予了一个动词或动作字符串表示信号。该字符串会以“被修改过”或“未挂载”等词语来描述事件。

    3）、没个事件都一个可选的负载。相比传递一个表示负载的字符串到用户控件而言，内核事件层使用sysfs属性代表负载。

    4）、从内部实现来讲，内核事件由内核传递到用户空间需要经过netlink。




  2、在内核代码中向用户空间发送信号使用函数kobject+uevent()；

        int  kobject_uevent(struct  kobject  *kobj,  enum  kobject_action  action);




