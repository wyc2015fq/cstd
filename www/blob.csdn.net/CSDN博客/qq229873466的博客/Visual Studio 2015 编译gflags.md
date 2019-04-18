# Visual Studio 2015 编译gflags - qq229873466的博客 - CSDN博客

2016年12月11日 09:29:43[qq229873466](https://me.csdn.net/qq229873466)阅读数：2138


工具：[cmake](https://cmake.org/)， Visual Studio 2015，[gflags](https://github.com/gflags/gflags)

**编译glags**

1.下载安装好以上工具后，打开Visual Studio 2015 -> 工具 -> Visual Studio命令提示，输入cmake-gui.exe路径打开。

![](https://img-blog.csdn.net/20161211091458445)![](https://img-blog.csdn.net/20161211091506844)

2.输入glags里CmakeList.txt所在目录，一般在下载的glags-master目录下。再选择需要建立项目的所在文件夹build_gflags。然后点击configure，点击generate，完成。

![](https://img-blog.csdn.net/20161211091538789)

3.进入生成的项目，右键ALL_BUILD生成，右键INSTALL生成。

**使用gflags**

1.新建vs2015控制台空项目，添加cpp，输入测试代码，如下：

```cpp
#include <iostream>
#include <gflags/gflags.h>
DEFINE_string(var,"","input var");
int main(int argc, char ** argv){
	google::ParseCommandLineFlags(&argc, &argv, true);
	std::cout << FLAGS_var << std::endl;
	return 0;
}
```
2.配置项目，右键项目 -> 属性 -> VC++目录 -> 输入包含目录和库目录，链接器 -> 输入附加依赖项，调试 -> 输入参数。

![](https://img-blog.csdn.net/20161211092355248)

![](https://img-blog.csdn.net/20161211092513420)

![](https://img-blog.csdn.net/20161211092752004)

3.运行程序。

![](https://img-blog.csdn.net/20161211092840687)

