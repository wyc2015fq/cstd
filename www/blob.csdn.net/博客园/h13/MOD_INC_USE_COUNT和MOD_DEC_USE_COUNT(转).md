# MOD_INC_USE_COUNT和MOD_DEC_USE_COUNT(转) - h13 - 博客园
2.4内核中，模块自身通过 MOD_INC_USE_COUNT, MOD_DEC_USE_COUNT宏来管理自己被使用的计数。
   2.6内核提供了更健壮、灵活的模块计数管理接口 try_module_get(&module), module_put(&module)取代2.4中的模块使用计数管理宏；模块的使用计数不必由自身管理，而且在管理模块使用计数时考虑到 SMP与PREEMPT机制的影响。
   int try_module_get(struct module *module); 用于增加模块使用计数；若返回为0，表示调用失败，希望使用的模块没有被加载或正在被卸载中。
   void module_put(struct module *module); 减少模块使用计数。
   try_module_get与module_put 的引入与使用与2.6内核下的设备模型密切相关。模块是用来管理硬件设备的，2.6内核为不同类型的设备定义了struct module *owner 域，用来指向管理此设备的模块。如字符设备的定义：
struct cdev
{
    struct kobject kobj;
    struct module *owner;
    struct file_operations *ops;
    struct list_head list;
    dev_t dev;
    unsigned int count;
};
    从设备使用的角度出发，当需要打开、开始使用某个设备时，使用 try_module_get(dev->owner)去增加管理此设备的 owner模块的使用计数；当关闭、不再使用此设备时，使用module_put(dev->owner)减少对管理此设备的owner模块的使用 计数。这样，当设备在使用时，管理此设备的模块就不能被卸载；只有设备不再使用时模块才能被卸载。
   2.6内核下，对于为具体设备写驱动的开发人员而言，基本无需使用 try_module_get与module_put，因为此时开发人员所写的驱动通常为支持某具体设备的owner模块，对此设备owner模块的计数管理由内核里更底层的代码如总线驱动或是此类设备共用的核心模块来实现，从而简化了设备驱动开发。
