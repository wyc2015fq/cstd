# shell脚本：函数及传参 - happyhorizon的算法天空 - CSDN博客
2018年12月17日 00:20:20[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：45
## 函数的定义
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
## 函数的调用
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
## shell脚本参数设置
比较复杂的shell脚本通常都会支持多个参数及其输入. 通常参数的形式为`-[a-z,A-Z] [filename/settings]`的形式.  通常在控制台输入脚本名称时,会显示出帮助文件.
可以定义一个Usage() {}函数作为脚本参数设置的帮助,参数比较少的时候,可以echo命令实现,当参数比较多的时候,可以用cat工具配合EOF符号来定义大段的说明文字. 例如:
```
# 帮助语句
Usage() {
    cat <<EOF
Usage:    bet <input> <output> [options]
   ...
  -d          debug (do not delete temporary intermediate images)
  
EOF
    exit 1
}
# 显示帮助, 当只有脚本名称,没有任何参数的时候,显示Usage函数中定义的内容.
[ "$1" = "" ] && Usage
```
这里bet命令的调用格式定义为:`bet <input> <output> [options]`,这也是一种常见的shell脚本参数格式. 当options比较多的时候,shell脚本没有python argpase这样的工具包, 应该如何解析这些参数设置呢?
