# c++动态库打包为dll文件供C#项目调用 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年09月27日 11:32:15[boonya](https://me.csdn.net/boonya)阅读数：139标签：[C  																[C#																[dll](https://so.csdn.net/so/search/s.do?q=dll&t=blog)
个人分类：[C/C++																[C#](https://blog.csdn.net/boonya/article/category/1776321)](https://blog.csdn.net/boonya/article/category/7395156)





C++与C#接口交互需要通过DLL库来完成。

## 编写C++动态库

创建项目：

![](https://img-blog.csdn.net/2018092711150975?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180927111554219?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

添加C++测试类：

![](https://img-blog.csdn.net/20180927111631399?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

在MathAPI.h定义接口方法：

```cpp
#pragma once
class MathAPI
{
public:
	MathAPI();
	~MathAPI();
		static _declspec(dllexport) double Add(double a, double b);
		static _declspec(dllexport) double Subtract(double a, double b);
		static _declspec(dllexport) double Multiply(double a, double b);
		static _declspec(dllexport) double Devide(double a, double b);
};
```

在MathAPI.cpp实现接口方法：

```cpp
#include "pch.h"
#include "MathAPI.h"
#include "stdexcept"

MathAPI::MathAPI()
{
}


MathAPI::~MathAPI()
{
}
double MathAPI::Add(double a, double b)
{
	return a + b;
}

double MathAPI::Subtract(double a, double b)
{
	return a - b;
}

double MathAPI::Multiply(double a, double b)
{
	return a * b;
}

double MathAPI::Devide(double a, double b)
{
	if (b == 0)
	{
		throw new _exception;
	}
	return a / b;
}
```

修改项目的属性，输出为动态库dll:

![](https://img-blog.csdn.net/20180927112015725?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

然后生成即可。

![](https://img-blog.csdn.net/20180927112047688?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## C#调用dll库

利用vs提供的dumpbin.exe工具可以查看dll的入口，在vs安装目录搜索即可找到。

dumpbin的选项指令：

![](https://img-blog.csdn.net/20180927112327709?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

为了快速看到效果，直接将FFmpegDXVA2dll.dll文件复制到dumpbin.exe所在的目录执行以下命令 ：

![](https://img-blog.csdn.net/20180927112259858?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

上面输出中?Add@MathAPI@@SANNN@Z 表示ADD函数的入口，我们在C#程序中使用时需要设置这个入口。

创建一个C#控制台程序，并将我们需要的dll库放入其中：

![](https://img-blog.csdn.net/20180927113000146?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

C#中定义dll接口访问:

```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace DllImportTest
{
    public class Math
    {
        [DllImport("FFmpegDXVA2dll.dll", EntryPoint = "?Add@MathAPI@@SANNN@Z", CallingConvention = CallingConvention.Cdecl)]
        public static extern double Add(double a, double b);

        [DllImport("FFmpegDXVA2dll.dll", EntryPoint = "?Subtract@MathAPI@@SANNN@Z", CallingConvention = CallingConvention.Cdecl)]
        public static extern double Subtract(double a, double b);

        [DllImport("FFmpegDXVA2dll.dll", EntryPoint = "?Multiply@MathAPI@@SANNN@Z", CallingConvention = CallingConvention.Cdecl)]
        public static extern double Multiply(double a, double b);

        [DllImport("FFmpegDXVA2dll.dll", EntryPoint = "?Devide@MathAPI@@SANNN@Z", CallingConvention = CallingConvention.Cdecl)]
        public static extern double Devide(double a, double b);
    }
}
```

Program运行：

```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DllImportTest
{
    class Program
    {
        static void Main(string[] args)
        {
            double a = 20, b = 5;
            Console.WriteLine(Math.Add(a,b));
            Console.WriteLine(Math.Subtract(a, b));
            Console.WriteLine(Math.Multiply(a, b));
            Console.WriteLine(Math.Devide(a, b));
            Console.Read();
        }
    }
}
```

输出结果：

![](https://img-blog.csdn.net/20180927112451185?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

至此，C#已经成功实现调用C++ 动态dll库的功能了。

参考文章：

[https://blog.csdn.net/nini_boom/article/details/78084408](https://blog.csdn.net/nini_boom/article/details/78084408)

[https://blog.csdn.net/dearkundy/article/details/73331514](https://blog.csdn.net/dearkundy/article/details/73331514)](https://so.csdn.net/so/search/s.do?q=C#&t=blog)](https://so.csdn.net/so/search/s.do?q=C  &t=blog)




