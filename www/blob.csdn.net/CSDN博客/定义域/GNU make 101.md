# GNU make 101 - 定义域 - CSDN博客
2015年11月18日 17:51:22[定义域](https://me.csdn.net/u010278882)阅读数：252标签：[makefile](https://so.csdn.net/so/search/s.do?q=makefile&t=blog)
个人分类：[工具](https://blog.csdn.net/u010278882/article/category/5802879)
# GNU 项目自动构建工具 Make
用途：自动化构建工具，通过编写处理不同情况的脚本（Make工具使用，存储为MakeFile文件，扩展名是.mk）来达到自动构建并且智能构建的目标。
> 
程序的编译过程和产生物 
  源文件（.c/.cpp） - 编译 - 中间文件（Windows：.obj; UNIX:.o）   - 连接 - 可执行文件(Windows:.exe; UNIX:.out) - 打包 - 可运行库（Windows:.dll; UNIX: .so） 
                                                                  - 归档 - 库文件（Window:.lib; UNIX:.a）
常见项目构建任务
- 源码均为编译，对全部源码编译
- 部分源码经过修改，对修改过的源码重新编译
- 部分头文件修改，对引入修改过的头文件的源码重新编译
问题：`#include <a.h>` 找不到头文件，而 “a.h”却可以找到头文件 
问题：脚本文件名为`makefile`则可以执行，如果为`makefile.mk`则不可执行
## make 101
make脚本由如下的单元构成：
```
target:prerequisites
    command
```
该指令对应的自然语言描述为“要获取`target`，需要使用`prerequisites`中指定的资源依赖，并对这些资源执行`command`；如果`target`的版本比较`prequisites`的版本新，那么就重新执行命令”。`target`定义要获取的目标，`prerequisites`定义获取目标需要处理的文件，`command`定义对`prerequisite`的处理。
- 如果没有prerequisite，那么默认就不执行该target处理（即不执行对应的command），需要通过 make target的方式来执行；
- command前一定要有一个Tab；
- 如果命令行过长，可以使用反斜杠()来连接位于多行的命令
> 
make如何知道target没有prequisites新?
make通过比较文件的修改日期来比较
> 
高级1：使用变量来定义重复使用的字符串
定义：varname=varvalue 
引用：将原本字符串替换为$(varname)
> 
高级2：利用make的自动推导功能减少构建脚本编写量
make能够自动推导使用的依赖和命名：编译fileName.c文件则不需要声明fileName.o的依赖和执行cc -c fileName.c
> 
习惯1：每个makefile都声明一个清空生成物的target，放在结尾不要放在开头。rm前加上-表示如果出现问题忽略。
高级3：引入其他makefile
在需要引入其他makefile的地方使用 include filename 来引入
> 
问题：错误使用`include`，将其放在文件头部而不是需要引入的地方
