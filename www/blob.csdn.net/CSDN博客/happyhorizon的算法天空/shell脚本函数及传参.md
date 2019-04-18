# shell脚本函数及传参 - happyhorizon的算法天空 - CSDN博客
2018年05月24日 10:54:02[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：7048
## shell函数的定义
shell脚本的函数位置相对自由,既可以一个shell脚本单独成一个函数, 也可以在主代码中嵌入内置函数.
在Shell中可以通过下面的两种语法来定义函数，分别如下：
```
function_name ()
{
    statement1
    statement2
    ....
    statementn
}
```
或者
```
function function_name()
{
   statement1
   statement2
   ....
   statementn
}
```
## shell脚本函数传参
当某个函数定义好了以后，用户就可以通过函数名来调用该函数了。在Shell中，函数调用的基本语法如下，
function_name parm1 parm21
例如下面的脚本:
```
#!/usr/bin/env bash
# encoding: utf-8.0
function test_func()
{
    echo "output from inside-function:test_func"
    echo 'input parameter1: '$1
    echo 'input parameter2: '$2
}
echo "here is main function"
echo "now inside function: test_func"
test_func hello world
```
运行后输出:
```
here is main function
now inside function: test_func
input parameter1: hello
 input parameter2:  world
```
还可以更复杂一些.例如整个shell脚本还有控制台的输入参数:
```
#!/usr/bin/env bash
# encoding: utf-8.0
function test_func()
{
    echo "output from inside-function:test_func"
    echo 'input parameter1: '$1
    echo 'input parameter2: '$2
}
function print_list()
{
while read LINE
do 
    echo $LINE
done < $1
}
echo "here is main function"
echo "now inside function: test_func"
test_func hello world 
print_list $1
```
运行`./shell_function.test list.txt`得到的输出如下.可见内置函数内的参数列表和脚本的参数列表互相并不影响.
```
here is main function
now inside function: test_func
output from inside-function:test_func
input parameter1: hello
input parameter2: world
line1
line2
line3
line4
```
