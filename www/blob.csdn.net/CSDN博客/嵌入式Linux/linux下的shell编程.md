
# linux下的shell编程 - 嵌入式Linux - CSDN博客

2014年03月11日 14:19:41[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：559


下面是一个最简单的shell:


例5：输入参数 $\#是指参数个数、$@是所有输入的参数
```python
#! /bin/sh
echo "$# parameters";
echo "$@";
```
```python
wqf@wqf-System-Product-Name:/worksen/linuxshell$ ./echo.sh ad
1 parameters
ad
```

新建一个shell. gedit echo.sh 输入以下字符
例1：

```python
#! /bin/sh
cd /tmp
echo "hello world!"
```
第一种运行shell的方法
1、chmod 777 echo.sh
2、./echo.sh
第二种运行shell的方法
1、chmod 777 echo.sh
2、source echo.sh

两种的结果是不一样的，第一种并没有进去tmp目录。第二种命令才会进入tmp目录。
例2：输出字符串

```python
#! /bin/sh
long_str="linux_shell_programming"
echo $long_str
```
例3：输出两个整数相加

```python
#! /bin/sh
add_1=100
add_2=200
echo $(($add_1+$add_2))
```
例4：局部变量和全局变量 local声明的局部变量只能在函数中可以调用

```python
#! /bin/sh
num=123
func1()
{
	num=321
	echo $num
}
Func2()
{
	local num=456
	echo $num
}
echo $num
func1
echo $num
Func2
echo $num
wqf@wqf-System-Product-Name:/worksen/linuxshell$ ./echo.sh
123
321
321
456
321
```

