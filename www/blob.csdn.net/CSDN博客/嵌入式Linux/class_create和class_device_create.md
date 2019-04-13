
# class_create和class_device_create - 嵌入式Linux - CSDN博客

2013年12月12日 14:37:52[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：666



```python
//主要是在/sys/class/ 下创建一个 “name”的文件夹 ！
 
从linux内核2.6的某个版本之后，devfs不复存在，udev成为devfs的替代。相比devfs，udev有很多优势，在此就不罗嗦了

，提醒一点，udev是应用层的东东，不要试图在内核的配置选项里找到它;加入对udev的支持很简单，以作者所写的一个字

符设备驱动为例，在驱动初始化的代码里调用class_create为该设备创建一个class，再为每个设备调用 

class_device_create创建对应的设备。大致用法如下：
struct class *myclass = class_create(THIS_MODULE, “my_device_driver”);
class_device_create(myclass, NULL, MKDEV(major_num, 0), NULL, “my_device”);
这样的module被加载时，udev daemon就会自动在/dev下创建my_device设备文件。

class_create()
-------------------------------------------------
linux-2.6.22/include/linux/device.h
struct class *class_create(struct module *owner, const char *name)
    class_create - create a struct class structure
    @owner: pointer to the module that is to "own" this struct class
    @name: pointer to a string for the name of this class.
在/sys/class/下创建类目录

class_device_create()
-------------------------------------------------
linux-2.6.22/include/linux/device.h
struct class_device *class_device_create(struct class        *cls,
                                         struct class_device *parent,
                                         dev_t               devt,
                                         struct device       *device,
                                         const char          *fmt, ...)

    class_device_create - creates a class device and registers it with sysfs
    @cls: pointer to the struct class that this device should be registered to.
    @parent: pointer to the parent struct class_device of this new device, if any.
    @devt: the dev_t for the char device to be added.
    @device: a pointer to a struct device that is assiociated with this class device.
    @fmt: string for the class device's name
//主要是在/sys/class/ 下创建一个 “name”的文件夹 ！
 
从linux内核2.6的某个版本之后，devfs不复存在，udev成为devfs的替代。相比devfs，udev有很多优势，在此就不罗嗦了，提醒一点，udev是应用层的东东，不要试图在内核的配置选项里找到它;加入对udev的支持很简单，以作者所写的一个字符设备驱动为例，在驱动初始化的代码里调用class_create为该设备创建一个class，再为每个设备调用 class_device_create创建对应的设备。大致用法如下：
struct class *myclass = class_create(THIS_MODULE, “my_device_driver”);
class_device_create(myclass, NULL, MKDEV(major_num, 0), NULL, “my_device”);
这样的module被加载时，udev daemon就会自动在/dev下创建my_device设备文件。

class_create()
-------------------------------------------------
linux-2.6.22/include/linux/device.h
struct class *class_create(struct module *owner, const char *name)
    class_create - create a struct class structure
    @owner: pointer to the module that is to "own" this struct class
    @name: pointer to a string for the name of this class.
在/sys/class/下创建类目录

class_device_create()
-------------------------------------------------
linux-2.6.22/include/linux/device.h
struct class_device *class_device_create(struct class        *cls,
                                         struct class_device *parent,
                                         dev_t               devt,
                                         struct device       *device,
                                         const char          *fmt, ...)
    class_device_create - creates a class device and registers it with sysfs
    @cls: pointer to the struct class that this device should be registered to.
    @parent: pointer to the parent struct class_device of this new device, if any.
    @devt: the dev_t for the char device to be added.
    @device: a pointer to a struct device that is assiociated with this class device.
    @fmt: string for the class device's name
```


