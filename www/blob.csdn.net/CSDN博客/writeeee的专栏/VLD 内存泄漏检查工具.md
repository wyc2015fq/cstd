# VLD 内存泄漏检查工具 - writeeee的专栏 - CSDN博客
2018年11月19日 19:49:30[writeeee](https://me.csdn.net/writeeee)阅读数：39
个人分类：[c++](https://blog.csdn.net/writeeee/article/category/5634855)

### VLD 内存泄漏检查工具
1.安装vld-2.5.1
2.配置依赖库，工程添加路径。
Additional Include Directories:…\Soup\Visual Leak Detector\include;
Additional Depandencies:…\Soup\Visual Leak Detector\lib\Win32\vld.lib;
3.main.cpp或其他任意cpp文件 中 #include “vld.h”
4.每个dll库都需要有一个文件 #include “vld.h”
5.重新编译运行
1）关于vld检测单件说明，只能使用下面方式实现
```
static FooSingleton* getInstance(void) 
{
            /* THIS WOULD CAUSE LEAKS TO BE DETECTED
               SINCE THE DESTRUCTOR WILL NEVER BE CALLEd
               AND THE MAP IS NOT CLEARED.
            */
            // FooSingleton* instance = new FooSingleton;
            // return instance;
            static FooSingleton instance;
            return &instance;
 }
```
2）代码中主动屏蔽vld
```
void addString(const std::string& val)
 {
    VLDDisable();
    _map.insert(std::make_pair(val, new std::string(val)));
    VLDEnable();
}
```
