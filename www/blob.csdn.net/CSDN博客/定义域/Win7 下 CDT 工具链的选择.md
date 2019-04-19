# Win7 下 CDT 工具链的选择 - 定义域 - CSDN博客
2015年11月12日 18:05:41[定义域](https://me.csdn.net/u010278882)阅读数：683
# Win7 下 CDT 工具链的选择
## 开发环境简介
`Win7`下搭建`C/C++`开发环境，我这里选择我熟悉的`eclipse`。使用到的基本组件是`eclipse + CDT + ToolChain`。 
`CDT`是为了便于在`eclipse`中进行`C/C++`开发而开发的`eclipse`插件，其提供的主要功能与支持`Java`和`Android`开发的大致相同，比如提供的代码语法高亮、代码知识工具（调用查看、类集成关系查看）、类结构视图等工具。就像`Java`开发需要`JDK`一样，`C/C++`开发也需要相应的支持工具（提供编译、调试工具以及类库，不过没有运行时，因为编译好的可执行文件就是在编译平台上执行的，比如我这里使用的Windows 7）。`CDT`所需要的是一个叫做工具链的一组工具集。不同的平台有不同的工具链，根据`eclipse`的文档描述，`Windows`上主要使用的工具链有两个：
- MinGw
- CygWin
## 工具链比较
工具链需要提供编译器、连接器和调试器以及构建工具等组件。
### 依赖包的区别
使用`MinGw`作为工具链需要下载`Base Setup`中的`ming32-base、ming32-gcc-g++、msys-base`（目前不清楚这些包中提供了哪些具体的组件，大部分资料都是使用者三个包）。 
使用`Cygwin` 作为工具链需要下载 `gcc-core、gcc-g++、gdb、make` 四个工具包。
下载安装完成相应的依赖后，需要添加相应的安装目录到 `path` 中。
### eclipse 集成上的区别
#### 使用CYGWIN
需要添加C/C++项目环境变量
![这里写图片描述](https://img-blog.csdn.net/20151113101502141)
```
# 为GNU CPP添加环境变量
CYGWIN_CPP_INCLUDE_1_6  ${CYGWIN_HOME}/lib/gcc/x86_64-pc-cygwin/4.9.3/include
CYGWIN_CPP_INCLUDE_2_6  ${CYGWIN_HOME}/lib/gcc/x86_64-pc-cygwin/4.9.3/include-fixeds
CYGWIN_CPP_INCLUDE_3_6  ${CYGWIN_HOME}/usr/include
CYGWIN_CPP_INCLUDE_4_6  ${CYGWIN_HOME}/lib/gcc/x86_64-pc-cygwin/4.9.3/include/c++
CYGWIN_CPP_INCLUDE_5_6  ${CYGWIN_HOME}/lib/gcc/x86_64-pc-cygwin/4.9.3/include/c++/backward
CYGWIN_CPP_INCLUDE_6_6  ${CYGWIN_HOME}/lib/gcc/x86_64-pc-cygwin/4.9.3/include/c++/x86_64-pc-cygwin
# 为GNU C添加环境变量
CYGWIN_C_INCLUDE_1_3    ${CYGWIN_HOME}/lib/gcc/x86_64-pc-cygwin/4.9.3/include
CYGWIN_C_INCLUDE_2_3    ${CYGWIN_HOME}/lib/gcc/x86_64-pc-cygwin/4.9.3/include-fixed
CYGWIN_C_INCLUDE_3_3    ${CYGWIN_HOME}/usr/include
```
对于`Cygwin`，如果不进行配置，项目创建完成后，在输入`#include <iostream.h>` 可能出现 `Unresolved inclusion iostream.h 的错误`；在输入`cout` 时也会出现错误提示。但是当构建和运行程序时，却发现能够正常有构建和运行。出现该问题的原因是，`eclipse` 没有为该项目配置正确的符号索引，所以找寻不到需要的头文件。解决的方法，就是进行配置：
![Unresolved inclusion](https://img-blog.csdn.net/20151113104201608)
配置后，可以在`Project Explorer` 中看到导入的头文件。
![配置前](https://img-blog.csdn.net/20151113104247795)
![添加配置](https://img-blog.csdn.net/20151113104309286)
![配置后](https://img-blog.csdn.net/20151113104321909)
经过上述配置后，可能发现仍然有错误，其中可能的原因之一是：你所使用的C++语言标准不同。 
比如，对于`iostream` 头文件在标准化之前，使用的是`iostream.h`，在标准化之后（C++98）文件名变更为`iostream`。将`iostream.h` 改为`iostream` 便可发现找到了头文件。
#### 使用MinGw
然而，使用`MinGw` 却不需要使用配置。这也是推荐使用`MinGw` 的原因。
可以发现使用`MinGw` 作为工具链的项目会自动导入相关的头文件。
![GNU CPP头文件](https://img-blog.csdn.net/20151113104805546)
![GNU C头文件](https://img-blog.csdn.net/20151113104824409)
