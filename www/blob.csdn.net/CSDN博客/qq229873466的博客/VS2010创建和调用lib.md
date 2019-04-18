# VS2010创建和调用lib - qq229873466的博客 - CSDN博客

2015年11月30日 00:10:23[qq229873466](https://me.csdn.net/qq229873466)阅读数：324标签：[visual studio 2010																[lib](https://so.csdn.net/so/search/s.do?q=lib&t=blog)](https://so.csdn.net/so/search/s.do?q=visual studio 2010&t=blog)
个人分类：[VC应用](https://blog.csdn.net/qq229873466/article/category/5990495)


**1.创建lib**

文件->新建->项目->Win32(Visual C++)->Win32控制台应用程序->输入项目名字，选择保存路径，弹出框选下一步->点击静态库，选择空项目->完成

在头文件中添加*.h文件，源文件中添加*.cpp文件

h文件输入：

```cpp
#define PI 3.1415
extern "C" double getPI();
```

cpp文件输入：

```cpp
#include"getLib.h"
double getPI(){
return PI;
}
```

->生成->生成解决方案

然后在Debug文件夹中会产生个*.lib文件。

**2.调用lib**

文件->新建->项目->Win32(Visual C++)->Win32控制台应用程序->输入项目名字，选择保存路径，填出框选下一步->点击控制台应用程序（默认），选择空项目->完成

在源文件中添加*.cpp文件，并输入如下代码

```cpp
#include<stdio.h>
#include"getLib.h"
void main(){
printf("%f\n",getPI());
}
```

->右键项目名->属性

->C/C++->常规->附加包含目录->输入*.h所在目录

->链接器->常规->附加库目录->输入lib所在目录

->输入->附加依赖项->输入所有lib的名字

->点击应用->调试->开始执行（不调试）

