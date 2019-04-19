# Python使用CFFI调用C动态库 - 宇宙浪子的专栏 - CSDN博客
2018年01月05日 17:00:19[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：232
原文链接 [http://blog.csdn.net/itisyang/article/details/54426631](http://blog.csdn.net/itisyang/article/details/54426631)
## 初始化
```python
from cffi import FFI
ffi = FFI()
```
- 1
- 2
## 加载C动态库
`lib_IOTCAPIs = ffi.dlopen("./tutk_lib/libIOTCAPIs.so")`- 1
这种方法加载动态库若是多个库之间有交错调用，则会报错，若多个库之间存在交错调用，可使用下面的方法
`lib_IOTCAPIs = ffi.dlopen("./tutk_lib/libIOTCAPIs.so", flags=256)  # RTLD_GLOBAL = 256`- 1
此处dlopen使用方法同标准C中void *dlopen(const char *filename, int flags);相似
## 加载并调用C动态库函数
```
ffi.cdef("int IOTC_Initialize2(unsigned short nUDPPort);")
lib_IOTCAPIs.IOTC_Initialize2(0)  # 调用
```
- 1
- 2
## 加载并调用C结构体
```
ffi.cdef("""
typedef struct{
    unsigned int channel;
    unsigned char reserved[];
}SMsgAVIoctrlAVStream;
""")
buf = ffi.new('SMsgAVIoctrlAVStream *', [ChannelID, 4]) 
buf1 = ffi.new('SMsgAVIoctrlAVStream ', [ChannelID, “1234”])  # 调用
buf2 = ffi.new('SMsgAVIoctrlAVStream[]', 10)
buf3 = ffi.new('SMsgAVIoctrlAVStream[10]')
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
分别调用了结构体指针、结构体、结构体数组，buf2和buf3效果是相同的
## 加载并调用C基本类型 int、 char、unsigned int、unsigned char、 char[]、int[]、int*、char **…
```
x = ffi.new("char[]", "hello")
print len(x),x[0],ffi.string(x)
```
- 1
- 2
输出 6 h hello
```php
array = ffi.new("int[]", [1, 2, 3, 4])
```
- 1
相当于int array[] = { 1, 2, 3, 4 };
```php
x = ffi.new("int *")
x[0] = 42
print x[0]
```
- 1
- 2
- 3
输出 42
## 调用标准C函数
```python
ffi.cdef("""
int printf(const char *format, ...);   // copy-pasted from the man page
""")
C = ffi.dlopen(None)                     # 导入全部C函数命名空间
arg = ffi.new("char[]", "world")         # 相当于 char arg[] = "world";
C.printf("hi there, %s.\n", arg)         # 调用 printf
```
- 1
- 2
- 3
- 4
- 5
- 6
输出 hi there, world.
## 转换指针类型
```
frame_info = ffi.new('PPSDEV_MEDIA_HEADER *')
frame_p = ffi.cast("char *", frame)
```
- 1
- 2
将 PPSDEV_MEDIA_HEADER* 指针转换为 char* 指针
## 加载C头文件
```python
ffi.cdef("""
#include <library.h>
""")
```
- 1
- 2
- 3
若需要使用的库函数过多，一个一个加载麻烦，可采用这种方法一次性加载 
注意头文件中的函数接口需要写成标准的方式，一些为跨平台写的东西有可能报错，跨平台性能最好是在动态库中包含，在头文件剔除
