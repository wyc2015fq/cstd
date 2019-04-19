# Lua函数的几种用法详解 - fanyun的博客 - CSDN博客
2017年04月04日 19:03:01[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：7781标签：[lua](https://so.csdn.net/so/search/s.do?q=lua&t=blog)
个人分类：[脚本语言](https://blog.csdn.net/fanyun_01/article/category/6838309)
所属专栏：[巧用脚本](https://blog.csdn.net/column/details/15130.html)
一、lua函数
在面向对象语言中函数是一组一起执行任务的语句。可以把代码放到独立的函数中。怎么划分代码功能之间的不同，但在逻辑上划分通常是让每个函数执行特定的任务。在Lua中，函数是作为"第一类值"来使用的，在lua中函数可以存储在变量中，可以通过参数传递给其他函数，或者作为函数的返回值（类比C/C++中的函数指针），这种特性使Lua具有极大的灵活性。Lua对函数式编程提供了良好的支持，可以支持嵌套函数。另外，Lua既可以调用Lua编写的函数，还可以调用C语言编写的函数（Lua所有的标准库都是C语言写的）。Lua语言提供了程序可以调用大量的内置方法。这边有一个对lua常用库函数的总结连接：
Lua函数常用定义方式：
```cpp
optional_function_scope functionfunction_name( argument1, argument2, argument3..., argumentn)
function_body
return result_params_comma_separated
end
```
Lua函数参数：
如果一个函数使用参数，它必须声明接受的参数值变量。这些变量被函数的形式参数调用。正式的参数表现得就像在函数内部其他本地变量和在进入函数创建并在退出时销毁。
Lua函数调用：
当创建一个Lua函数，给什么样的功能，必须做一个定义。要使用一个方法，将不得不调用该函数来执行定义的任务。
当程序调用一个函数，程序的控制转移到被调用的函数。被调用函数进行定义的任务和在执行它的return语句或当其功能的终端到达时，程序控制返回到主程序。
而调用只是需要传递所需的参数以及方法名的方法，如果方法返回一个值，那么你可以存储返回的值。
例子：
```cpp
function max(num1, num2)
 
   if(num1 > num2) then
     result = num1;
  else
     result = num2;
  end
 
  return result;
end
 
-- calling a function
print("Max: ",max(10,2))
print("Max: ",max(5,6))
Max:         10
Min:         6
```
Lua分配和传递函数:
在Lua中，我们可以指定函数的变量，也可以将它们作为另一个函数的参数。使用函数作为参数：
```cpp
myprint = function(param)
  print("Print function：",param,"*")
end
 
function add(num1,num2,funPrint)
  result = num1 + num2
  funPrint(result)
end
myprint(12)
add(3,6,myprint)
输出结果：
Print function：12 *
Print function：9 *
```
Lua函数使用变参：
可以在Lua中创建一个具有可变参数的函数，作为它的参数。我们可以通过看一个例子，利用可变参数该函数将返回平均值。
```cpp
function average(...)
  result = 0
  local arg={...}
  for i,v in ipairs(arg) do
     result = result + v
  end
  return result/#arg
end
 
print("The average is：",average(12,6,3,4,5,6))
输出结果：
The average is：6
```
Lua函数其它使用举例：
```cpp
代码如下：
function hello()
print('hello')
end
hello函数不接收参数，调用：hello()，虽然hello不接收参数，但是还可以可以传入参数：hello(32)
```
另外如果只传递一个参数可以简化成functionname arg的调用形式（注意数值不行）
代码如下：
```cpp
hello '3'
hello
hello {}
hello
hello 3
stdin:1: syntax error near '3'
```
另外对变量名也不适用
代码如下:
```cpp
a = 21
print a
stdin:1: syntax error near 'a'
```
另外，Lua函数不支持参数默认值，可以使用or非常方便的解决（类似Javascript）
代码如下:
```cpp
function f(n)
n = n or 0
print(n)
end
f()
0
f(1)
1
```
Lua支持返回多个值，形式上非常类似Python：
代码如下:
```cpp
function f()
return 1,2,3
end
a,b,c = f()
print(a .. b .. c)
123
```
函数调用的返回值可以用于table：
复制代码代码如下:
```cpp
t = {f()}
print(t[1], t[2], t[3])
1       2       3
```
可见，f()返回的三个值分别称为table的3个元素，但是情况并不总是如此：
复制代码代码如下:
```cpp
t = {f(), 4}
print(t[1], t[2], t[3])
1       4       nil
```
这次，f()返回的1,2,3只有1称为table的元素；
代码如下:
```cpp
t = {f(), f()}
print(t[1], t[2], t[3], t[4], t[5])
1       1       2       3       nil
```
总之：只有最后一项会完整的使用所有返回值（假如是函数调用）。
对于无返回值的函数，可以使用(f())的形式强行返回一个值(nil)
代码如下:
```cpp
function g()
end
print(g())
print((g()))
nil
```
实际上，(f())形式的调用返回一个且只返回一个值
代码如下:
```cpp
print((f()))
1
print(f())
1       2       3
```
