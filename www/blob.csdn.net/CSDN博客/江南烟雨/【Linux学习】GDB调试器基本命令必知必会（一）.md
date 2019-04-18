# 【Linux学习】GDB调试器基本命令必知必会（一） - 江南烟雨 - CSDN博客
2013年05月22日 14:54:10[江南烟雨](https://me.csdn.net/xiajun07061225)阅读数：41065
本文介绍Linux下GDB调试器常用的基本命令。
测试均在Ubuntu12.10下完成。
先看看GDB调试的效果图：
![](https://img-blog.csdn.net/20130522144628921)
对应的源代码：
```cpp
//插入排序,GDB调试测试代码
#include <stdio.h>
int x[10],//存放输入数据
        y[10],//工作空间数组
        num_inputs,//输入数组长度
        num_y = 0;//y中当前元素个数
//函数
//功能:从参数中提取输入数据
void get_args(int ac,char **av){
        int i;
        num_inputs = ac - 1;
        for(i = 0;i < num_inputs;++i)
                x[i] = atoi(av[i + 1]);
}
//函数
//功能:向右移动工作数组中索引jj及其后面的所有元素
void scoot_over(int jj){
        int k;
        for(k = num_y;k > jj;++k)
                y[k] = y[k - 1];
}
//函数:
//功能:插入一个数据
void insert(int new_y){
        int j;
        //工作数组为空
        if(0 == num_y){
                y[0] = new_y;
                return;
        }
        //将新元素插入到适当位置:第一个比new_y大的元素前面
        for(j = 0;j < num_y;++j){
                if(new_y < y[j]){
                        //向右移动y[j]后面的所有元素
                        scoot_over(j);
                        y[j] = new_y;
                        return;
                }
        }
}
//函数:
//功能:处理所有输入数据
void process_data(){
        for(num_y = 0;num_y < num_inputs;++num_y)
                insert(x[num_y]);
}
//函数:
//打印结果数据
void print_results(){
        int i;
        for(i = 0;i < num_inputs;++i)
                printf("%d\n",y[i]);
}
int main(int argc,char **argv){
        get_args(argc,argv);
        process_data();
        print_results();
}
```
源代码编译：
gcc -g -Wall -o insert_sort ins.c
这里面一个必要的步骤是：GCC的-g选项：让编译器将符号表（对应于程序的变量和代码行的内存地址列表）保存在生成的可执行文件中。这样才能在调试会话过程中引用源代码中的变量名和行号。
一些基本命令
启动GDB：gdb exeFileName
gdb中中文显示是乱码，暂时还不知道怎么支持中文。
联机帮助，可以通过help命令访问文档：(gdb) help breakpoints
以TUI模式运行GDB：在调用gdb的时候指定-tui选项或者在处于非TUI模式时在GDB中使用Ctrl+X+A组合键，可以将终端屏幕分成原文本窗口和控制台的多个子窗口
退出GDB：quit或者Ctrl+d
可以直接按下回车再次执行最近执行过的那条命令
执行程序：run
查看栈帧：frame num（栈帧编号）
注意栈帧编号规则，当前正在执行的函数的帧被编号为0，其父帧（即该函数的调用者的栈帧）被编号为1，父帧的父帧被编号为2，以此类推。
跳到调用栈中的下一个父帧：up
引向相反方向：down
显示整个栈，即当前存在的所有帧的集合：backtrace
滚动查看代码：↑等箭头键
输出当前值：print或者p
浏览以前的GDB命令：上一个Ctrl+P、下一个Ctrl+N
注意：如果我们需要修改程序，并且重新编译，不必重启GDB。GDB会在运行程序之前自动重新加载新的二分表和新的符号表。
我们在重新编译程序之前仍然不必退出GDB。
断点相关操作
设置断点
GDB设置点的后，该断点的有效性会持续到删除、禁用或退出GDB时。
在某一行设置断点：break line_number（行数）
break filename:line_number 在源代码文件filename的line_number处设置断点。
在某个函数的入口（第一行可执行代码）处设置断点：break function（函数名）。这种方法相对以行数设置断点有一个优点：如果修改了源代码，使得函数不再在这一行处开始，那么如果用函数名指定断点，而不是用行号指定，则断点仍然有效。
注意：break function会在所有具有相同名称的函数上设置断点，注意函数重载的情况。
break filename: function 在源代码文件filename的函数function入口处处设置断点。
当GDB使用多个断点中断一行源代码时，它只会中断一次。
GDB实际设置断点的位置可能和我们请求将断点放置的位置不同。
比如下列代码：
```cpp
int main(void)
{
    int i;
    i = 3;
   return 0;
}
```
如果我们尝试在函数main入口处设置断点，断点实际会被设置在第4行。因为GDB会认为第三行的机器码对我们的调试目的来说没有用处。
条件断点
将正常断点转变为条件断点：condition break_p_num（断点编号） cond（条件）
在有效的C条件语句中几乎可以用任何表达式。也可以用自己的函数或者库函数，只要他们被链接到程序中。
如：condition 1 num_y == 1
只有当满足条件num_y == 1时，GDB才会在断点1处暂停程序的执行。
如果以后要删除条件，但是保持断点，只需键入：cond break_p_num（断点编号）
用break if可以将break和condition命令组合成一个步骤：
break line_num（行号） if cond（条件）
例子：(gdb) nreak 30 if num_y == 1
注意，条件语句可以用括号()括起来，也可以不用。
临时断点：tbreak
与break类似，但是这一命令设置的断点在首次到达该指定行后就不再有效。
删除断点
**基于标识符删除断点：delete**
delete break_point_num（断点编号），可以在后面指定多个断点编号连续删除几个断点
delete：删除所有断点
**另一种删除断点方式：clear**
依据位置删除断点，工作方式和对应的break命令相似：
clear function
clear filename:funtion
clear line_number
clear filename:line_number
clear：清除GDB将执行的下一个指令处的断点
禁用与启用断点
禁用断点：disable breakpoint-list（是用空格分隔开的多个断点标识符）
禁用所有现存断点：disable
启用断点：enable breakpoint-list
在下次引起GDB暂停执行后禁用：enable once breakpoint-list
查看所有断点信息：info break或者是info breakpoints或者简写为i b
断点命令列表
让GDB在每次到达某个断点时自动执行一组命令，从而自动完成某一任务。
使用commands命令设置命令列表：
commands breakpoint_number
...
commands
...
end
例如以下代码：
![](https://img-blog.csdn.net/20130522144849323)
我打算查看传递给fibonacci函数的值以及次序，但是不想在程序中插入printf语句并重新编译代码，可以这样做：
在该函数入口处设置一个断点，然后设置命令列表：
![](https://img-blog.csdn.net/20130522144914759)
printf命令和C中printf函数类似，只是括号是可选的。
运行结果：
![](https://img-blog.csdn.net/20130522144941649)
如果嫌GDB输出太冗长，可以使用silent命令，只需将其添加到设置的命令列表最开始处即可。
![](https://img-blog.csdn.net/20130522145006586)
恢复执行
三种方法
（1）单步执行：step（s）、next（n），仅执行到代码的下一行后再次暂停。
注意二者区别：在函数调用时step会进入函数，next导致下一次暂停出现在调用函数之后。next被称为单步越过（stepping over）函数，而step被称为单步进入（stepping into）函数。
next和step都可以采用一个可选的数值参数，来表示要使用next或step执行的额外行数。
（2）无条件恢复程序的执行：continue（c）
直到遇到另一个断点或者程序结束。
continue可以接受一个可选的数值参数n，要求GDB忽略下面n个断点。
（3）用finish（fin）或until（u）命令恢复。
finish命令指示GDB恢复执行，直到恰好在当前帧完成之后为止。
until命令通常用来在不进一步在循环中暂停（除了循环中的中间断点）的情况下完成正在执行的循环。until会执行循环的其余部分（如果遇到断点，还是会暂停），让GDB在循环后面的第一行代码处暂停。
until命令也可以接受源代码中的位置作为参数，其用法与break命令同。
比如下列代码清单swapflaw.c：
```cpp
#include <stdio.h>
void swap(int *a,int *b){
        int c = *a;
        *a = *b;
        *b = c;
}
int main(void){
        int i = 3;
        int j = 5;
        printf("i:%d,j:%d\n",i,j);
        swap(&i,&j);
        printf("i:%d,j:%d\n",i,j);
        return 0;
}
```
如果GDB触发了main函数入口处的一个断点，那么可以使用下面这些命令方便地使程序一直执行到swap()的入口：
until 13
until swap
until swapflaw.c:13
until swapflaw.c:swap
监视点
监视点是一种特殊类型的断点，区别在于监视点没有“住在”某一行源代码中，而是指示GDB每当某个表达式改变了值就暂停执行。
设置监视点：watch z（变量名）也可以是复杂的表达式
注意只能监视存在且在作用域内的变量。一旦变量不再存在调用栈的任何帧中，GDB会自动删除监视点。
GDB实际上是在变量的内存位置改变值时中断。
文件清单
（gdb) list line1,line2
查看源代码
list lineNum 在lineNum的前后源代码显示出来
list + 列出当前行的后面代码行
list - 列出当前行的前面代码行
list function
set listsize count
设置显示代码的行数
show listsize
显示打印代码的行数
list first,last
显示从first到last的源代码行
**GDB启动文件的使用**
有时候我们在完成调试前可能需要退出GDB，比如需要离开比较长的一段时间而且不能保持登录在计算机中。为了不丢失某些信息，可以将断点和设置的其他命令放在一个GDB启动文件中，然后每次启动GDB时会自动加载它们。
GDB启动文件默认名为.gdbinit。可以将一个文件放在主目录中用于一般用途，另一个文件放在特定项目专用的目录中。例如，可以将设置断点的命令放在后一个目录的启动文件中，在主目录的.gdbinit文件中存储开发的一些通用的宏。最好不要将编程项目放在主目录中，因为不能将项目特有的信息放在.gdbinit中。
在调用GDB时可以指定启动文件，如：
$gdb -command=z x
表示要在可执行文件x上运行GDB，首先要从文件z中读取命令。
