
# android系统开发(六)-HAL层开发基础 - 嵌入式Linux - CSDN博客

2016年06月27日 18:06:59[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：913


Android HAL层，即硬件抽象层，是Google响应厂家“希望不公开源码”的要求推出的新概念
1，源代码和目标位置
源代码： /hardware/libhardware目录,该目录的目录结构如下：
/hardware/libhardware/hardware.c编译成libhardware.so，目标位置为/system/lib目录
/hardware/libhardware/include/hardware目录下包含如下头文件：
hardware.h 通用硬件模块头文件
copybit.h copybit模块头文件
gralloc.h gralloc模块头文件
lights.h  背光模块头文件
overlay.h overlay模块头文件
qemud.h  qemud模块头文件
sensors.h 传感器模块头文件
/hardware/libhardware/modules目录下定义了很多硬件模块
这些硬件模块都编译成xxx.xxx.so，目标位置为/system/lib/hw目录
2，HAL层的实现方式
JNI->通用硬件模块->硬件模块->内核驱动接口
具体一点：JNI->libhardware.so->xxx.xxx.so->kernel
具体来说：android frameworks中JNI调用/hardware/libhardware/hardware.c中定义的hw_get_module函数来获取硬件模块，
然后调用硬件模块中的方法，硬件模块中的方法直接调用内核接口完成相关功能
3,通用硬件模块(libhardware.so)
(1)头文件为：/hardware/libhardware/include/hardware/hardware.h
头文件中主要定义了通用硬件模块结构体hw_module_t，声明了JNI调用的接口函数hw_get_module
hw_module_t定义如下：
typedef struct hw_module_t {
/** tag must be initialized to HARDWARE_MODULE_TAG*/
uint32_t tag;
`/** major version number for the module */
uint16_t version_major;
/** minor version number of the module */
uint16_t version_minor;
/** Identifier of module */
const char *id;
/** Name of this module */
const char *name;
/** Author/owner/implementor of the module */
const char *author;
/** Modules methods */
struct hw_module_methods_t* methods; //硬件模块的方法
/** module's dso */
void* dso;
/** padding to 128 bytes, reserved for future use */
uint32_t reserved[32-7];`} hw_module_t;
硬件模块方法结构体hw_module_methods_t定义如下：
typedef struct hw_module_methods_t {
/** Open a specific device*/
int (*open)(const struct hw_module_t*module, const char* id,
struct hw_device_t** device);
} hw_module_methods_t;
只定义了一个open方法，其中调用的设备结构体参数hw_device_t定义如下：
typedef struct hw_device_t {
/** tag must be initialized to HARDWARE_DEVICE_TAG*/
uint32_t tag;
`/** version number for hw_device_t */
uint32_t version;
/** reference to the module this device belongs to */
struct hw_module_t* module;
/** padding reserved for future use */
uint32_t reserved[12];
/** Close this device */
int (*close)(struct hw_device_t* device);`} hw_device_t;
hw_get_module函数声明如下：
int hw_get_module(const char *id, const struct hw_module_t **module);
参数id为模块标识，定义在/hardware/libhardware/include/hardware目录下的硬件模块头文件中，
参数module是硬件模块地址，定义了/hardware/libhardware/include/hardware/hardware.h中
(2)hardware.c中主要是定义了hw_get_module函数如下：
```python
#
```
```python
define
```
```python
HAL_LIBRARY_PATH "/system/lib/hw"
```
static const char *variant_keys[] = {
“ro.hardware”,
“ro.product.board”,
“ro.board.platform”,
“ro.arch”
};
static const int HAL_VARIANT_KEYS_COUNT =
(sizeof(variant_keys)/sizeof(variant_keys[0]));
int hw_get_module(const char *id, const struct hw_module_t **module)
{
int status;
int i;
const struct hw_module_t *hmi = NULL;
char prop[PATH_MAX];
char path[PATH_MAX];
for (i=0 ; i
```python
#
```
```python
define
```
```python
HAL_MODULE_INFO_SYM         HMI
```
```python
#
```
```python
define
```
```python
HAL_MODULE_INFO_SYM_AS_STR  "HMI"
```
4,硬件模块
硬件模块的开发主要是完成/hardware/libhardware/include/hardware目录下对应的头文件中的内容，主要是硬件模块头文件和hardware.h中
的结构体中定义了一些函数指针，调用内核提供的接口将具体的函数实现，然后编译成指定名称的动态链接库放到/system/lib/hw目录下即可。
用一句话来概括：硬件模块的开发就是定义一个hardware.h中定义的hw_module_t结构体，结构体名称为宏HAL_MODULE_INFO_SYM，然后实现结构体
的相关内容即可。
5,内核驱动
主要是要向用户层开放接口，让硬件模块和内核可以交互。

