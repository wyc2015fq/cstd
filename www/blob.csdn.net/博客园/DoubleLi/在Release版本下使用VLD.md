# 在Release版本下使用VLD - DoubleLi - 博客园








# 前提

同Debug版本在VC中配置好VLD的相关信息，拷贝

Visual Leak Detector\bin\Win32目录下所有的文件和vld.ini到工程目标路径下。

# 强制检测

在程序入口处的cpp中，定义强制检测宏和包含vld头文件

```cpp
#define VLD_FORCE_ENABLE
#include "vld.h"
```

在程序启动和退出时，分别增加以下函数调用

```cpp
{
    ...
    VLDGlobalEnable();
    VLDReportLeaks();
    //some code...
    VLDGlobalDisable();
}
```

# 增加检测模块

以上配置后，默认情况下只会检测主程序的模块，要想增加其他模块的内存泄露检测，需要配置vld.ini文件中的ForceIncludeModules，多个模块用";"分隔，如：
`ForceIncludeModules = demo1.dll;demo2.dll`

# 配置输出方式

默认只会在调试下的终端中打印内存泄露信息，可配置vld.ini文件中的ReportTo为both，这样可以同时想终端和文件中打印信息，如：
`ReportTo = both`



作者：龙翱天际
链接：https://www.jianshu.com/p/1fb05cfdc76d
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。









