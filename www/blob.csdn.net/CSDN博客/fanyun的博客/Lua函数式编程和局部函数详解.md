# Lua函数式编程和局部函数详解 - fanyun的博客 - CSDN博客
2017年04月04日 19:12:01[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：852
所属专栏：[巧用脚本](https://blog.csdn.net/column/details/15130.html)
         函数式编程中的函数这个术语不是指计算机中的函数（实际上是Subroutine），而是指数学中的函数，即自变量的映射。也就是说一个函数的值仅决定于函数参数的值，不依赖其他状态。比如sqrt(x)函数计算x的平方根，只要x不变，不论什么时候调用，调用几次，值都是不变的。
在函数式语言中，函数作为一等公民，可以在任何地方定义，在函数内或函数外，可以作为函数的参数和返回值，可以对函数进行组合。
函数做一个First-Class Value可以赋值给变量，用后者进行调用：
代码如下:
```cpp
a = function() print 'hello' end
a()
hello
b = a
b()
hello
```
匿名函数
代码如下:
```cpp
g = function() return function() print'hello' end end
g()()
hello
```
函数g返回一个匿名函数；
闭包是函数式编程的一种重要特性，Lua也支持
代码如下:
```cpp
g = function(a) return function()print('hello'.. a); a = a + 1 end end
f = g(3)
f()
hello3
f()
hello4
```
    局部函数可以理解为在当前作用域有效的函数，可以用local变量来引用一个函数：
代码如下:
```cpp
do
local lf = function() print 'hello' end
lf()
end
hello
lf()
stdin:1: attempt to call global 'lf' (a nilvalue)
stack traceback:
stdin:1: in main chunk
[C]: in ?
```
需要注意的是，对于递归函数的处理
代码如下:
```cpp
do
local lf = function(n)
if n <= 0 then
return
end
print 'hello'
n = n -1
lf(n)
end
lf(3)
end
hello
stdin:8: attempt to call global 'lf' (a nilvalue)
stack traceback:
stdin:8: in function 'lf'
stdin:9: in main chunk
[C]: in ?
```
而应该首先声明local lf, 在进行赋值
代码如下:
```cpp
do
local lf;
lf = function(n)
if n <= 0 then
return
end
print 'hello'
n = n -1
lf(n)
end
lf(3)
end
hello
hello
hello
```
Lua支持一种local function(…) … end的定义形式：
代码如下:
```cpp
do
local function lf(n)
if n <= 0 then
return
end
print 'hello'
n = n -1
lf(n)
end
lf(3)
end
hello
hello
hello
lf(3)
stdin:1: attempt to call global 'lf' (a nilvalue)
stack traceback:
stdin:1: in main chunk
[C]: in ?
```
