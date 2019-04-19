# Lua函数的尾调用 - fanyun的博客 - CSDN博客
2017年04月04日 19:12:37[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：1222
所属专栏：[巧用脚本](https://blog.csdn.net/column/details/15130.html)
1. 尾调用
所谓尾调用，就是一个函数返回另一个函数的返回值：
代码如下:
```cpp
function f()
…
return g()
en
```
因为调用g()后，f()中不再执行任何代码，所以不需要保留f()的调用桟信息；Lua做了这样的优化，称为"尾调用消除"，g()返回后，控制点直接返回到调用f()的地方。
这种优化对尾递归非常有益，通常递归意味着调用桟的不断增长，甚至可能造成堆栈溢出；而尾递归提供了优化条件，编译器可以优化掉调用桟。  
下面的递归函数没有使用尾递归，而参数为大数时，堆栈溢出：
代码如下:
```cpp
function f(n)
if n <= 0 then
return 0
end
a = f(n-1)
return n * a
end
f(10000000000)
stdin:5: stack overflow
stack traceback:
stdin:5: in function 'f'
stdin:5: in function 'f'
stdin:5: in function 'f'
stdin:5: in function 'f'
stdin:5: in function 'f'
stdin:5: in function 'f'
stdin:5: in function 'f'
stdin:5: in function 'f'
stdin:5: in function 'f'
stdin:5: in function 'f'
...
stdin:5: in function 'f'
stdin:5: in function 'f'
stdin:5: in function 'f'
stdin:5: in function 'f'
stdin:5: in function 'f'
stdin:5: in function 'f'
stdin:5: in function 'f'
stdin:5: in function 'f'
stdin:5: in function 'f'
stdin:1: in main chunk
[C]: in ?
```
优化为尾递归
代码如下:
```cpp
function f(n, now)
if n <= 0 then
return now
end
return f(n-1, now*n)
end
f(10000000000, 1)
```
运行n久也无堆栈溢出。
