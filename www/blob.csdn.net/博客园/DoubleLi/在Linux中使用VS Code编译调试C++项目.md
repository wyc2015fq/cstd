# 在Linux中使用VS Code编译调试C++项目 - DoubleLi - 博客园






最近项目需求，需要在Linux下开发C++相关项目，经过一番摸索，简单总结了一下如何通过VS Code进行编译调试的一些注意事项。

关于VS Code在Linux下的安装这里就不提了，不管是CentOS还是Ubuntu，如果不懂且搜Q足够的情况下，你会解决的。

## 一. 前置知识——gcc/g++的编译链接过程

在Windows下，如果你用Visual Studio进行开发，C/C++的编译器一般采用微软提供的MSBuild；在Linux下C/C++的编译器大多采用gcc/g++。既然要在Linux下进行C++开发，很有必要了解一下g++编译器的一些基本知识。

假设我现在有一个最简单的C++文件：

```
1 #include <iostream>
2 using namespace std;
3 int main() {
4     cout << "Hello, world!!!!" << endl;
5     return 0;
6 }
```

接下来如何编译呢？简单来说分成两步：先编译，再链接

### 1. 安装g++编译器

启动终端，进入root模式，安装gcc和g++

Ubuntu:

```
xxx@xxx:~$ sudo apt-get install build-essential

xxx@xxx:~$ gcc --version
xxx@xxx:~$ sudo apt-get install g++-4.8
xxx@xxx:~$ g++ --version
```

CentOS:

```
[xxx@xxx ~]$ su
[xxx@xxx ~]# yum install gcc
[xxx@xxx ~]# gcc --version
[xxx@xxx ~]# yum install gcc-g++
[xxx@xxx ~]# g++ --version
```

### 2. 编译hello.cpp

```
[xxx@xxx ~]$ g++ -c hello.cpp
```

输出结果是一个hello.o文件，这是编译过程的生成的中间文件。**-c 表示只编译，不链接**。

### 3. 链接hello.o生成hello.out

```
[xxx@xxx ~]$ g++ -o hello.out hello.o
```

输出结果是一个hello.out文件，这是最终的可执行文件。**-o 表示输出文件**，hello.o是上一步生成的.o文件。

当然，如果第2、3步是可以合并执行，直接执行命令

```
[xxx@xxx ~]$ g++ -o hello.out hello.cpp
```

然而第2、3步分开执行是有意义的，后面会讲到。

### 4. 运行hello.out

最后执行以下hello.out验证一下输出结果呗

```
[xxx@xxx ~]$ ./hello.out
```

## 二. 构建项目

实际开发过程中当然不可能只有一个cpp这么简单，有时候会有非常多的.h和.cpp文件相互配合，那么上面直接通过g++编译可执行文件就没那么简单了。我们需要借助Make这个强大的项目构建工具，帮助我们构建和组织项目代码。

假设现在有如下3个文件：hw2.cpp、solution.h和solution.cpp

```
1 /* solution.h */
2 class Solution {
3 public:
4     void Say();
5 };
```

```
1 /* solution.cpp */
2 #include <iostream>
3 #include "solution.h"
4 void Solution::Say(){
5    std::cout << "HI!" << std::endl;
6 }
```

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

1 /* hw2.cpp */
2 #include "solution.h"
3 int main () {
4     Solution sln;
5     sln.Say();
6     return 0;
7 }

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```



可以看到这个简单例子包括头文件引用、定义和实现分离等情况，如果直接g++ -o hw2.out hw2.cpp将会报未定义引用的错误：

[xxx@xxx ~]$ g++ -o hw2.out hw2.cpp 
/tmp/ccIMYTxf.o：在函数‘main’中：
hw2.cpp:(.text+0x10)：对‘Solution::Say()’未定义的引用
collect2: 错误：ld 返回 1



这时Make就该大显身手了。

首先我们还需要了解一下makefile。

在项目的根目录下**创建一个makefile文件，以告诉Make如何编译和链接程序。**

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

1 build : hw2.o solution.o
2     g++ -o build hw2.o solution.o #注意前面必须是tab，不能是空格
3 hw2.o : hw2.cpp solution.h
4     g++ -g -c hw2.cpp
5 solution.o : solution.h solution.cpp
6     g++ -g -c solution.cpp
7 clean :
8     rm hw2.o solution.o build

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```



先来解释一下makefile的基本语法规则：

```
target ... : prerequisites ...
　　command    #注意前面是tab
```

target是一个目标文件，可以是Object File，也可以是执行文件，还可以是一个标签；

prerequisites是要生成那个target所需要的文件或是目标；

command是make需要执行的命令（任意的Shell命令）。

说白了就是**target这一个或多个目标，依赖于prerequisites列表中的文件，其执行规则定义在command里**。如果prerequisites列表中文件比target要新，就会执行command，否则就跳过。这就是整个make过程的基本原理。



那么，我们回头看看上面定义的makefile文件，我们解释一下每两行的作用

```
1 build : hw2.o solution.o
2     g++ -o build hw2.o solution.o
```

target是build，依赖于hw2.o 和 solution.o，执行的命令是 g++ -o build hw2.o solution.o

意思是通过g++链接hw2.o和solution.o，生成可执行文件build，prerequisites有两个.o文件，是因为代码里hw2引用了solution.h。



```
3 hw2.o : hw2.cpp solution.h
4      g++ -g -c hw2.cpp
```

target是hw2.o，依赖于hw2.cpp和solution.h，执行命令是g++ -g -c hw2.cpp

意思是通过g++编译hw2.cpp文件，生成hw2.o文件，g++命令中 -g 表示生成的文件是可调试的，如果没有-g，调试时无法命中断点。



```
5 solution.o : solution.h solution.cpp
6     g++ -g -c solution.cpp
```

同上，编译solution.cpp文件，生成solution.o文件。



```
7 clean :
8     rm hw2.o solution.o build
```

**这里clean不是一个可执行文件，也不是一个.o文件，它只不过是一个动作名字，类似于label的作用，make不会去找冒号后的依赖关系，也不会自动执行命令。如果要执行该命令，必须在make后显示指出整个动作的名字，如make clean。**



好了，接下来说一下make的工作原理。在默认的方式下，我们只需输入make，则发生了以下行为：

a. make在当前目录下找名为makefile或Makefile的文件；

b. 如果找到，它会找文件中的第一个target，如上述文件中的build，并作为终极目标文件;

c. 如果第一个target的文件不存在，或其依赖的.o 文件修改时间要比target这个文件新，则会执行紧接着的command来生成这个target文件;

d. 如果第一个target所依赖的.o文件不存在，则会在makefile文件中找target为.o的依赖，如果找到则执行command，.o的依赖必是.h或.cpp，于是make可以生成 .o 文件了

e. 回溯到b步执行最终目标



看一下执行结果

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

[xxx@xxx ~]$ make
g++ -g -c hw2.cpp
g++ -g -c solution.cpp
g++ -o build hw2.o solution.o #注意前面必须是tab，不能是空格
[xxx@xxx ~]$ ./build 
*HI!*[xxx@xxx ~]$

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```



由于makefile文件中加了**-g这一选项，于是可以通过gdb进行调试，并且会命中断点**，这里感兴趣可以再了解一下gdb的使用。

接下来我们要说到如何通过VS Code进行调试。



## 三. 在VS Code中编译调试

首先安装完VS Code之后，还需要安装一下扩展**cpptools**，请自行完成。

![](https://images2015.cnblogs.com/blog/425551/201609/425551-20160918135915310-1600094267.png)



点击菜单 查看-> 调试，或直接快捷键ctrl + shift + D

![](https://images2015.cnblogs.com/blog/425551/201609/425551-20160918132929043-1351001957.png)

点击设置图标，在弹出的选择环境中选择C++（GDB/LLDB），会自动创建一个launch.json文件

![](https://images2015.cnblogs.com/blog/425551/201609/425551-20160918133043095-1288105584.png)

顾名思义，laucn.json的作用是告诉VS Code如何执行启动任务，也就是我们要把什么文件启动起来，在上述例子中显然是build这个可执行文件了。修改一下json文件中波浪线的program节点，改成**${workspaceRoot}/build**，其余的暂时不变



```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

 1 {
 2     "version": "0.2.0",
 3     "configurations": [
 4         {
 5             "name": "C++ Launch",
 6             "type": "cppdbg",
 7             "request": "launch",
 8             "program": "${workspaceRoot}/build",
 9             "args": [],
10             "stopAtEntry": false,
11             "cwd": "${workspaceRoot}",
12             "environment": [],
13             "externalConsole": true,
14             "linux": {
15                 "MIMode": "gdb"
16             },
17             "osx": {
18                 "MIMode": "lldb"
19             },
20             "windows": {
21                 "MIMode": "gdb"
22             }
23         },
24         {
25             "name": "C++ Attach",
26             "type": "cppdbg",
27             "request": "attach",
28             "program": "${workspaceRoot}/build",
29             "processId": "${command.pickProcess}",
30             "linux": {
31                 "MIMode": "gdb"
32             },
33             "osx": {
34                 "MIMode": "lldb"
35             },
36             "windows": {
37                 "MIMode": "gdb"
38             }
39         }
40     ]
41 }

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

接着我们尝试一下F5，开始调试，结果可以看到报了一个缺少build文件的错误。原因是我们还没执行make编译出可执行文件呢。我们在launch.json文件中，添加一个preLaunchTask的节点，并设置值为“build”。注意这里的build不是指可执行文件build，而是一个名为build的任务！

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

 1 {
 2     "version": "0.2.0",
 3     "configurations": [
 4         {
 5             "name": "C++ Launch",
 6             "type": "cppdbg",
 7             "request": "launch",
 8             "program": "${workspaceRoot}/build",
 9             "args": [],
10             "stopAtEntry": false,
11             "cwd": "${workspaceRoot}",
12             "environment": [],
13             "externalConsole": true,
14             "preLaunchTask": "build",
15             "linux": {
16                 "MIMode": "gdb"
17             },
18             "osx": {
19                 "MIMode": "lldb"
20             },
21             "windows": {
22                 "MIMode": "gdb"
23             }
24         },
25         {
26             "name": "C++ Attach",
27             "type": "cppdbg",
28             "request": "attach",
29             "program": "${workspaceRoot}/build",
30             "processId": "${command.pickProcess}",
31             "linux": {
32                 "MIMode": "gdb"
33             },
34             "osx": {
35                 "MIMode": "lldb"
36             },
37             "windows": {
38                 "MIMode": "gdb"
39             }
40         }
41     ]
42 }

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

再尝试F5，会提示一个信息：

![](https://images2015.cnblogs.com/blog/425551/201609/425551-20160918133856583-825448387.png)

点击配置任务运行程序，并选择Others， 会自动生成一个tasks.json文件，这个文件的作用就是告诉launch或者编译器需要执行什么操作。显然我们这里要执行make命令，修改tasks.json为如下：

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

 1 {
 2     "version": "0.1.0",
 3     "command": "make",
 4     "showOutput": "always",
 5     "tasks": [
 6         {
 7             "taskName": "clean"
 8         },
 9         {
10             "taskName": "build",
11             "problemMatcher": {
12                 "owner": "cpp",
13                 "fileLocation":  ["relative", "${workspaceRoot}"],
14                 "pattern": {
15                     "regexp": "^(.*):(\\d+):(\\d+):\\s+(warning|error):\\s+(.*)$",
16                     "file": 1,
17                     "line": 2,
18                     "column": 3,
19                     "severity": 4,
20                     "message": 5
21                 }
22             }
23         }
24     ]
25 }

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```

其中tasks节点是一组任务，注意到其中一个名为build的任务，这就是launch.json文件中指定的preLaunchTask，表明在启动可执行程序之前，会先执行一下preLaunchTask即这里的build任务，重新make一下代码，更新可执行程序之后再启动。

当然也可以指运行tasks这些任务而不启动可执行程序，直接ctrl + shift + B，在VSC的console里可以看到和终端执行一样的输出：

![](https://images2015.cnblogs.com/blog/425551/201609/425551-20160918134710227-1456666499.png)

执行完后，项目中会多出.o和build文件

![](https://images2015.cnblogs.com/blog/425551/201609/425551-20160918134813966-1972008463.png)



关于VS Code的launch.json和tasks.json中更多节点的含义，参考

[https://code.visualstudio.com/docs/editor/debugging](https://code.visualstudio.com/docs/editor/debugging)

[https://code.visualstudio.com/docs/editor/tasks](https://code.visualstudio.com/docs/editor/tasks)



接着设置好断点之后F5，就可以进入断点调试了

![](https://images2015.cnblogs.com/blog/425551/201609/425551-20160918134952254-1407959225.png)



 --------------------------------------------------------------------------------------------------------------------------------------------------



本篇总结了gcc/g++和make/makefile的基础知识，以及在Linux下使用VS Code进行调试开发的方法，希望对正在挖坑的同学有所帮助，坑避免一个是一个。









