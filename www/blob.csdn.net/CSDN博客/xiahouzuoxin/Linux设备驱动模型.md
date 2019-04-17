# Linux设备驱动模型 - xiahouzuoxin - CSDN博客





2013年05月18日 14:11:31[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：28298









尽管LDD3中说对多数程序员掌握设备驱动模型不是必要的，但对于嵌入式Linux的底层程序员而言，对设备驱动模型的学习非常重要。

Linux设备模型的目的：为内核建立一个统一的设备模型，从而又一个对系统结构的一般性抽象描述。换句话说，Linux设备模型提取了设备操作的共同属性，进行抽象，并将这部分共同的属性在内核中实现，而为需要新添加设备或驱动提供一般性的统一接口，这使得驱动程序的开发变得更简单了，而程序员只需要去学习接口就行了。


在正式进入设备驱动模型的学习之前，有必要把documentation/filesystems/sysfs.txt读一遍（不能偷懒）。sysfs.txt主要描述/sys目录的创建及其属性，sys目录描述了设备驱动模型的层次关系，我们可以简略看一下/sys目录，

![](http://blog.chinaunix.net/attachment/201305/18/25984261_1368856808wpd6.png)






block：所有块设备

devices：系统所有设备（块设备特殊），对应struct device的层次结构

bus：系统中所有总线类型（指总线类型而不是总线设备，总线设备在devices下），bus的每个子目录都包含

    --devices：包含到devices目录中设备的软链接

    --drivers：与bus类型匹配的驱动程序

class：系统中设备类型（如声卡、网卡、显卡等）

fs：一些文件系统，具体可参考filesystems /fuse.txt中例子

dev：包含2个子目录

--char：字符设备链接，链接到devices目录，以<major>:<minor>命名

--block：块设备链接



Linux设备模型学习分为：Linux设备底层模型，描述设备的底层层次实现（kobject）；Linux上层容器，包括总线类型（bus_type）、设备（device）和驱动（device_driver）。




## ====  Linux设备底层模型 ====




谨记：像上面看到的一样，设备模型是层次的结构，层次的每一个节点都是通过kobject实现的。在文件上则体现在sysfs文件系统。

### kobject结构


内核中存在struct kobject数据结构，每个加载到系统中的kobject都唯一对应/sys或者子目录中的一个文件夹。可以这样说，许多kobject结构就构成设备模型的层次结构。每个kobject对应一个或多个struct
 attribute描述属性的结构。





点击(此处)折叠或打开


- struct kobject {

- const char *name;/* 对应sysfs的目录名 */

- 
    struct list_head entry;/* kobjetct双向链表 */

- 
    struct kobject *parent;/* 指向kset中的kobject，相当于指向父目录 */

- 
    struct kset *kset;/*指向所属的kset */

- 
    struct kobj_type *ktype;/*负责对kobject结构跟踪*/

- 
    struct sysfs_dirent *sd;

- 
    struct kref kref;/*kobject引用计数*/

- 
    unsigned int state_initialized:1;

- 
    unsigned int state_in_sysfs:1;

- 
    unsigned int state_add_uevent_sent:1;

- 
    unsigned int state_remove_uevent_sent:1;

- 
    unsigned int uevent_suppress:1;

- };






kobject结构是组成设备模型的基本结构，最初kobject设计只用来跟踪模块引用计数，现已增加支持，


—— sysfs表述：在sysfs中的每个对象都有对应的kobject


—— 数据结构关联：通过链接将不同的层次数据关联


—— 热插拔事件处理：kobject子系统将产生的热插拔事件通知用户空间

kobject一般不单独使用，而是嵌入到上层结构（比如struct device，struct device_driver）当中使用。kobject的创建者需要直接或间接设置的成员有：ktype、kset和parent。kset我们后面再说，parent设置为NULL时，kobject默认创建到/sys顶层目录下，否则创建到对应的kobject目录中。重点来分析ktype成员的类型，







点击(此处)折叠或打开


- #include <kobject.h>

- 
struct kobj_type {

- 
    void (*release)(struct
 kobject *kobj);/* 释放 */

- const struct sysfs_ops *sysfs_ops;/* 默认属性实现 */

- 
    struct attribute **default_attrs;/* 默认属性 */

- const struct kobj_ns_type_operations *(*child_ns_type)(struct
 kobject *kobj);

- const void *(*namespace)(struct
 kobject *kobj);

- };






ktype包含了释放设备、默认属性以及属性的实现方法几个重要成员。每个kobject必须有一个release方法，并且kobject在该方法被调用之前必须保持不变（处于稳定状态）。默认属性的结构如下，







点击(此处)折叠或打开


- #include <linux/sysfs.h>

- 
struct attribute {

- const char *name;/* 属性名称 */

- 
    mode_t mode;/* 属性保护：只读设为S_IRUGO，可写设为S_IWUSR */

- }






kobj_type中的default_attrs为二级结构指针，可以对每个kobject使用多个默认属性，最后一个属性使用NULL填充。struct
 sysfs_ops结构则如下，





点击(此处)折叠或打开


- struct sysfs_ops {

- 
    ssize_t (*show)(struct
 kobject *, struct attribute *,char *);

- 
    ssize_t (*store)(struct
 kobject *,struct attribute *,const char *, size_t);

- };






show方法用于将传入的指定属性编码后放到char *类型的buffer中，store则执行相反功能：将buffer中的编码信息解码后传递给struct
 attribute类型变量。两者都是返回实际的属性长度。


一个使用kobject的简单例子如下，





点击(此处)折叠或打开


- #include <linux/module.h>

- 
#include <linux/init.h>

- 
#include <linux/device.h>

- 
#include <linux/string.h>

- 
#include <linux/sysfs.h>

- 
#include <linux/kernel.h>

- 

- 
MODULE_AUTHOR("xhzuoxin");

- 
MODULE_LICENSE("Dual BSD/GPL");

- 

- 
void my_obj_release(struct kobject *kobj)

- {

- 
    printk("release ok.n");

- }

- 

- 
ssize_t my_sysfs_show(struct kobject *kobj, struct attribute *attr, char *buf)

- {

- 
    printk("my_sysfs_show.n");

- 
    printk("attrname:%s.n", attr->name);

- 
    sprintf(buf,"%s", attr->name);

- 
    return strlen(attr->name)+ 1;

- }

- 

- 
ssize_t my_sysfs_store(struct kobject *kobj, struct attribute *attr,const char *buf,

- 
       size_t count)

- {

- 
    printk("my_sysfs_store.n");

- 
    printk("write:%sn", buf);

- 

- 
    return count;

- }

- 

- 
struct sysfs_ops my_sysfs_ops ={

- .show = my_sysfs_show,

- .store = my_sysfs_store,

- };

- 

- 
struct attribute my_attrs ={

- .name ="zx_kobj",

- .mode = S_IRWXUGO,

- };

- 

- 
struct attribute *my_attrs_def[]={

- &my_attrs,

- NULL,

- };

- 
struct kobj_type my_ktype ={

- .release = my_obj_release,

- .sysfs_ops =&my_sysfs_ops,

- .default_attrs = my_attrs_def,

- };

- 

- 
struct kobject my_kobj ;

- 

- int __init kobj_test_init(void)

- {

- 
    printk("kobj_test init.n");

- 
    kobject_init_and_add(&my_kobj,&my_ktype,NULL,"zx");

- 

- 
    return 0;

- }

- 

- 
void __exit kobj_test_exit(void)

- {

- 
    printk("kobj_test exit.n");

- 
    kobject_del(&my_kobj);

- }

- 

- 
module_init(kobj_test_init);

- 
module_exit(kobj_test_exit);







例子中有两个函数，用于初始化添加和删除kobject结构，





点击(此处)折叠或打开


- int kobject_init_and_add(struct kobject *kobj, struct
 kobj_type *ktype,

- 
            struct kobject *parent,const char *fmt,...);/* fmt指定kobject名称 */

- 
void kobject_del(struct kobject *kobj);







加载模块后，在/sys目录下增加了一个叫zx达到目录，zx目录下创建了一个属性文件zx_kobj，使用tree
 /sys/zx查看。

![](http://blog.chinaunix.net/attachment/201305/18/25984261_1368856809jUAJ.png)


内核提供了许多与kobject结构相关的函数，如下：





点击(此处)折叠或打开


- // kobject初始化函数

- 
void kobject_init(struct kobject * kobj);

- // 设置指定kobject的名称

- int kobject_set_name(struct kobject *kobj,const char *format,...);

- // 将kobj 对象的引用计数加，同时返回该对象的指针

- 
struct kobject *kobject_get(struct kobject *kobj);

- // 将kobj对象的引用计数减，如果引用计数降为，则调用kobject release()释放该kobject对象

- 
void kobject_put(struct kobject * kobj);

- // 将kobj对象加入Linux设备层次。挂接该kobject对象到kset的list链中，增加父目录各级kobject的引// 用计数，在其parent指向的目录下创建文件节点，并启动该类型内核对象的hotplug函数

- int kobject_add(struct kobject * kobj);

- // kobject注册函数,调用kobject init()初始化kobj，再调用kobject_add()完成该内核对象的注册

- int kobject_register(struct kobject * kobj);

- // 从Linux设备层次(hierarchy)中删除kobj对象

- 
void kobject_del(struct kobject * kobj);

- // kobject注销函数. 与kobject register()相反，它首先调用kobject
 del从设备层次中删除该对象，再调// 用kobject put()减少该对象的引用计数，如果引用计数降为，则释放kobject对象

- 
void kobject_unregister(struct kobject * kobj);






### kset结构

![](http://blog.chinaunix.net/attachment/201305/18/25984261_13688568099izS.jpg)




我们先看上图，kobject通过kset组织成层次化的结构，kset将一系列相同类型的kobject使用（双向）链表连接起来，可以这样 认为，kset充当链表头作用，kset内部内嵌了一个kobject结构。内核中用kset数据结构表示为：





点击(此处)折叠或打开


- #include <linux/kobject.h>

- 
struct kset {

- 
    struct list_head list;/* 用于连接kset中所有kobject的链表头 */

- 
    spinlock_t list_lock;/* 扫描kobject组成的链表时使用的锁 */

- 
    struct kobject kobj;/* 嵌入的kobject */

- const struct kset_uevent_ops *uevent_ops;/* kset的uevent操作 */

- };







与kobject 相似，kset_init()完成指定kset的初始化，kset_get()和kset_put()分别增加和减少kset对象的引用计数。Kset_add()和kset_del()函数分别实现将指定keset对象加入设备层次和从其中删除；kset_register()函数完成kset的注册而kset_unregister()函数则完成kset的注销。




## ==== 设备模型上层容器 ====





这里要描述的上层容器包括总线类型（bus_type）、设备（device）和驱动（device_driver），这3个模型环环相扣，参考图9-2。为何称为容器？因为bus_type/device/device_driver结构都内嵌了Linux设备的底层模型（kobject结构）。为什么称为上层而不是顶层？因为实际的驱动设备结构往往内嵌bus_type/device/device_driver这些结构，比如pci，usb等。


总线类型、设备、驱动3者之间关系：


在继续之前，自我感觉需要区分2个概念：总线设备与总线类型。总线设备本质上是一种设备，也需要像设备一样进行初始化，但位于设备的最顶层，总线类型是一种在设备和驱动数据结构中都包含的的抽象的描述（如图9-2），总线类型在/sys/bus目录下对应实体，总线设备在/devices目录下对应实体。

### 总线类型bus_type

内核对总线类型的描述如下：





点击(此处)折叠或打开


- struct bus_type {

- const char *name;/* 总线类型名 */

- 
    struct bus_attribute *bus_attrs;/* 总线的属性 */

- 
    struct device_attribute *dev_attrs;/* 设备属性,为每个加入总线的设备建立属性链表 */

- 
    struct driver_attribute *drv_attrs;/* 驱动属性,为每个加入总线的驱动建立属性链表 */

- 

- /* 驱动与设备匹配函数:当一个新设备或者驱动被添加到这个总线时，这个方法会被调用一次或多次，若指定的驱动程序能够处理指定的设备，则返回非零值。必须在总线层使用这个函数, 因为那里存在正确的逻辑，核心内核不知道如何为每个总线类型匹配设备和驱动程序 */

- int(*match)(struct
 device *dev, struct device_driver *drv);

- /*在为用户空间产生热插拔事件之前，这个方法允许总线添加环境变量（参数和 kset 的uevent方法相同）*/

- int(*uevent)(struct
 device *dev, struct kobj_uevent_env *env);

- int(*probe)(struct
 device *dev);/**/

- int(*remove)(struct
 device *dev);/* 设备移除调用操作 */

- 
    void (*shutdown)(struct
 device *dev);

- 

- int(*suspend)(struct
 device *dev, pm_message_t state);

- int(*resume)(struct
 device *dev);

- 

- const struct dev_pm_ops *pm;

- 

- 
    struct subsys_private *p;/* 一个很重要的域，包含了device链表和drivers链表 */

- };






![](http://blog.chinaunix.net/attachment/201305/18/25984261_1368856810PiZk.jpg)




接着对bus_type中比较关注的几个成员进行简述，

[1] struct bus_attribute结构，device_attribute与driver_attribute将分别在设备和驱动分析过程中看到，





点击(此处)折叠或打开


- struct bus_attribute {

- 
    struct attribute attr;

- 
    ssize_t (*show)(struct
 bus_type *bus, char *buf);

- 
    ssize_t (*store)(struct
 bus_type *bus,const char *buf, size_t
 count);

- };








[2] subsys_private中包含了对加入总线的设备的链表描述和驱动程序的链表描述，省略的部分结构如下





点击(此处)折叠或打开


- struct subsys_private {

- 
    struct kset subsys;

- 
    struct kset *devices_kset;/* 使用kset构建关联的devices链表头 */

- 
    struct kset *drivers_kset;/* 使用kset构建关联的drivers链表头 */

- 
    struct klist klist_devices;/* 通过循环可访问devices_kset的链表 */

- 
    struct klist klist_drivers;/* 通过循环可访问drivers_kset的链表 */

- 
    struct bus_type *bus;/* 反指向关联的bus_type结构 */

- ......

- };






bus_type通过扫描设备链表和驱动链表，使用mach方法查找匹配的设备和驱动，然后将struct device中的*driver设置为匹配的驱动，将struct
 device_driver中的device设置为匹配的设备，这就完成了将总线、设备和驱动3者之间的关联。

bus_type只有很少的成员必须提供初始化，大部分由设备模型核心控制。内核提供许多函数实现bus_type的注册注销等操作，新注册的总线可以再/sys/bus目录下看到。





点击(此处)折叠或打开


- struct bus_type ldd_bus_type ={/* bus_type初始化 */

- .name ="ldd",

- .match = ldd_match,/* 方法实现参见实例 */

- .uevent = ldd_uevent,/* 方法实现参见实例 */

- };

- 
ret = bus_register(&ldd_bus_type);/* 注册，成功返回0 */

- if(ret)

- 
 return ret;

- 
void bus_unregister(struct bus_type *bus);/* 注销 */






### 设备device

设备通过device结构描述，





点击(此处)折叠或打开


- struct device {

- 
    struct device *parent;/* 父设备，总线设备指定为NULL */

- 
    struct device_private *p;/* 包含设备链表，driver_data（驱动程序要使用数据）等信息 */

- 
    struct kobject kobj;

- const char *init_name;/* 初始默认的设备名,但@device_add调用之后又重新设为NULL */

- 
    struct device_type *type;

- 
    struct mutex mutex;/* mutex to synchronize
 calls to its driver */

- 
    struct bus_type *bus;/* type
 of bus device ison*/

- 
    struct device_driver *driver;/* which
 driver has allocated this device */

- 
    void *platform_data;/* Platform
 specific data, device core doesn't touch it */

- 
    struct dev_pm_info power;

- 

- 
#ifdef CONFIG_NUMA

- int numa_node;/* NUMA
 node this device is close to*/

- 
#endif

- 
    u64 *dma_mask;/* dma
 mask (if dma'able device)*/

- 
    u64 coherent_dma_mask;/* Like dma_mask, but for

- 
                       alloc_coherent mappings as

- not all hardware supports

- 
                       64 bit addresses for consistent

- 
                       allocations such descriptors.*/

- 
    struct device_dma_parameters *dma_parms;

- 
    struct list_head dma_pools;/* dma pools (if dma'ble)*/

- 
    struct dma_coherent_mem *dma_mem;/* internal for coherent
 mem override */

- /* arch specific additions */

- 
    struct dev_archdata archdata;

- 
#ifdef CONFIG_OF

- 
    struct device_node *of_node;

- 
#endif

- 

- 
    dev_t devt;/* dev_t, creates
 the sysfs "dev" 设备号 */

- 
    spinlock_t devres_lock;

- 
    struct list_head devres_head;

- 
    struct klist_node knode_class;

- 
    struct class*class;

- const struct attribute_group **groups;/* optional
 groups */

- 

- 
    void (*release)(struct
 device *dev);

- };







设备在sysfs文件系统中的入口可以有属性，这通过struct device_attribute单独描述，提供device_create_file类型函数添加属性。





点击(此处)折叠或打开


- /* interface for exporting
 device attributes */

- 
struct device_attribute {

- 
    struct attribute attr;

- 
    ssize_t (*show)(struct
 device *dev, struct device_attribute *attr,

- 
           char *buf);

- 
    ssize_t (*store)(struct
 device *dev, struct device_attribute *attr,

- const char *buf, size_t count);

- };







使用宏DEVICE_ATTR宏可以方便地再编译时构建设备属性，构建好属性之后就必须将属性添加到设备。





点击(此处)折叠或打开


- /* 最终生成变量dev_attr_##_name描述属性，

- * 比如DEVICE_ATTR(zx,S_IRUGO,show_method,NULL);

- * 则create_file中entry传入实参为dev_attr_zx */

- 
DEVICE_ATTR(_name,_mode,_show,_store);

- /*属性文件的添加与删除使用以下函数 */

- int device_create_file(struct device *device, struct
 device_attribute * entry);

- 
void device_remove_file(struct device * dev, struct device_attribute * attr);







总线设备的注册：总线设备与一般设备一样，需要单独注册，与一般设备不同，总线设备的parent与bus域设为NULL。一般设备注册注销函数为





点击(此处)折叠或打开


- int device_register(struct device *dev);/* 成功返回0，需要检查返回值 */

- 
void device_unregister(struct device *dev);









实际创建新设备时，不是直接使用device结构，而是将device结构嵌入到具体的设备结构当中，比如








点击(此处)折叠或打开


- struct ldd_device {

- 
 char *name;/* 设备名称 */

- 
 struct ldd_driver *driver;/* ldd设备关联的驱动 */

- 
 struct device dev;/* 嵌入的device结构 */

- };

- /* 同时提供根据device结构获取ldd_device结构的宏定义 */

- 
#define to_ldd_device(dev) container_of(dev, struct
 ldd_device, dev);






### 驱动device_driver


驱动结构描述，





点击(此处)折叠或打开


- struct device_driver {

- const char *name;/* 驱动名称,在sysfs中以文件夹名出现 */

- 
    struct bus_type *bus;/* 驱动关联的总线类型 */

- 
    struct module *owner;

- const char *mod_name;/* used for built-in modules */

- 
    bool suppress_bind_attrs;/* disables bind/unbind
 via sysfs */

- 

- 
#if defined(CONFIG_OF)

- const struct of_device_id *of_match_table;

- 
#endif

- 

- int(*probe)(struct
 device *dev);

- int(*remove)(struct
 device *dev);

- 
    void (*shutdown)(struct
 device *dev);

- int(*suspend)(struct
 device *dev, pm_message_t state);

- int(*resume)(struct
 device *dev);

- const struct attribute_group **groups;

- 

- const struct dev_pm_ops *pm;

- 

- 
    struct driver_private *p;

- };

- 
struct driver_private {/* 定义device_driver中的私有数据类型 */

- 
   struct kobject kobj;/* 内建kobject */

- 
   struct klist klist_devices;/* 驱动关联的设备链表，一个驱动可以关联多个设备 */

- 
   struct klist_node knode_bus;

- 
   struct module_kobject *mkobj;

- 
   struct device_driver *driver;/* 连接到的驱动链表 */

- };

- 
#define to_driver(obj) container_of(obj, struct
 driver_private, kobj)






与设备和总线类似，驱动可以有属性，需要单独定义并添加。





点击(此处)折叠或打开


- /* sysfs interface for exporting
 driver attributes */

- 
struct driver_attribute {

- 
    struct attribute attr;

- 
    ssize_t (*show)(struct
 device_driver *driver, char *buf);

- 
    ssize_t (*store)(struct
 device_driver *driver,const char *buf,

- 
            size_t count);

- };

- 
DRIVER_ATTR(_name,_mode,_show,_store);/* 最终创建变量driver_attr_##_name描述属性 */

- /*属性文件创建的方法:*/

- int driver_create_file(struct device_driver * drv, struct
 driver_attribute * attr);

- 
void driver_remove_file(struct device_driver * drv, struct driver_attribute * attr);







驱动的注册与注销






点击(此处)折叠或打开

- 
/*注册device_driver 结构的函数 */
- 
int driver_register(struct device_driver *drv);
- 

void driver_unregister(struct device_driver *drv);







与设备结构一样，在编写新设备的驱动程序时，常常将device_driver结构嵌入到新设备结构当中使用。



==== 实例分析 ====




实例源代码主要来自LDD3提供的示例代码，因为LDD3的代码是linux-2.6.10版本，因此需要对源代码做一些修改。所有源代码参见：[](http://blog.chinaunix.net/sources/device_model.tar.gz)[](http://blog.chinaunix.net/blog/downLoad/fileid/8492.html)![](http://blog.chinaunix.net/image/default/fj.png)device_model.zip。因为两个模块关联，我们这使用一个Makefile文件同时编译2个模块，如下











点击(此处)折叠或打开

- 
obj-m := lddbus.o
 sculld.o


lddbus模块分析


包括2个文件，lddbus.c（example/lddbus/）与lddbus.h（example/include/）。lddbus.h中使用extern申明了将要使用EXPORT_SYMBOL导出的变量ldd_bus_type，lddbus.c中创建了总线类型ldd_bus_type以及总线设备ldd_bus。



lddbus.h

    -> extern ldd_bus_type

lddbus.c

    -> ldd_bus_type (EXPORT_SYMBOL)

    -> ldd_bus


由于版本变迁，对源代码做了修改，（i）热插拔不再使用hotplug函数，因此将该操作去掉了；（ii）dev->bus_id[]改成了使用dev_set_name()设置设备名称，使用init_name也可以设置，但后来发现init_name会在调用device_add之后就被赋值为NULL，这导致一个重大内核错误（kernel
 panic），将在后面详述。


分析源代码：作者定义了ldd_device与ldd_driver，两个变量分别内嵌device与device_driver结构，然后分别为ldd_device定义了注册函数register_ldd_device和注销函数unregister_ldd_device，对ldd_driver也做了类似的工作。还宏定义了to_ldd_driver和to_ldd_device来使用内嵌结构（device/device_driver）访问更上层的容器ldd_device和ldd_driver。但是不用着急，实际模块装载时没有使用ldd_device或者ldd_driver，而是将它们和相关的注册注销等操作使用EXPORT_SYMBOL导出到其它模块使用（这将在实例sculld模块中看到）。



struct ldd_device/register_ldd_device/unregister_ldd_device

    -> struct device/ device_register/device_unregister

    -> to_ldd_device

struct ldd_driver也类似

LDD3的Makefile中普遍使用了CFLAGS变量，但在新的内核版本中，该变量与内核Makefile的CFLAGS变量冲突，因此将所有的Makefile的CFLAGS变量替换成了EXTRA_CFLAGS。


装载模块后，查看/sys/bus目录下，增加了ldd文件夹，/sys/devices目录下增加了ldd0文件夹。

![](http://blog.chinaunix.net/attachment/201305/18/25984261_13688568114m98.png)sculld模块分析

sculld模块是接着lddbus在加载lddbus基础上进行的，sculld使用了lddbus中导出的ldd_device和ldd_driver结构。我们大致分析下总体的设备和驱动注册的调用关系，



scull_init()

->register_ldd_driver()  // 由lddbus模块导出

        ->driver_register() 

->sculld_register_dev()

        ->register_ldd_dev()  // 由lddbus模块导出

           ->device_register()


装载程序后查看bus/ldd/devices目录下，bus/ldd/drivers目录下多了驱动程序，多了4个设备，devices/ldd0下也多了4个设备。

![](http://blog.chinaunix.net/attachment/201305/18/25984261_1368856811Wlw7.png)关于kernel panic错误


在修改lddbus与sculld中，装载sculld模块时遇到如下错误，同时键盘大写字母指示灯闪烁，操作系统被锁定，只能强制关机。现在记录分析及解决错误的过程，

![](http://blog.chinaunix.net/attachment/201305/18/25984261_1368856812RaAp.jpg)


从网上找到资料，kernel panic类型错误要跟踪信息，还好，使用的虚拟机，把出错的状态截屏了。kernnel panic错误分硬件和软件，一般是由于指针指向了NULL。硬件有EIP指示出错位置，如上图有一行



EIP:[<c06044d1>] strncmp+0x11/0x38


好了，strncmp就是指示出错位置，然后到源代码中找到使用该函数地方，出错前为



!strncmp(dev->init_name, driver->name, strlen(driver->name));


前面说过，dev->init_name在调用device_register之后就被设置为NULL了，好了，就是它了，改成如下（通过kobj访问设备名称）就OK。



!strncmp(dev->kobj.name, driver->name, strlen(driver->name));




























































