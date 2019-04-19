# Lua变长参数详解 - fanyun的博客 - CSDN博客
2017年04月04日 19:08:26[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：3957
所属专栏：[巧用脚本](https://blog.csdn.net/column/details/15130.html)
**Lua变长参数:**
          Lua支持变长参数，使用简单（借助于table、多重赋值）,  Lua中的函数还可以接受不同数量的实参。例如：在调用print时可以传入一个、两个或多个实参。参数中的三个点（...）表示该函数可接受不同数量的实参。当这个函数被调用时，它的所有参数都会被收集到一起，这部分收集起来的实参称为这个函数的“变长参数”。 一个函数要访问他的变长参数时，仍需要用到3个点（...）,只是此时的3个点作为一个表达式来使用的(如上)。表达式“...”的行为类似于一个具有多重返回值的函数，它返回的是当前函数的所有变长参数。
示例代码1如下：
```cpp
function add(...)
local s = 0
  for i, v in ipairs{...} do   --> {...} 表示一个由所有变长参数构成的数组
    s = s + v
  end
  return s
end
print(add(3,4,5,6,7))  --->25
```
代码示例2：
```cpp
function fwrite(fmt, ...)  --->3点之前有个固定的参数fmt。具有变长参数的函数同样也可以用拥有任意数量的固定参数，但是固定参数必须放在变长参数之前。
                                lua会将前面的实参赋予固定参数，而将余下的实参（如果有的话）视为变长参数。
 return io.write(string.format(fmt, ...))   
end 
fwrite()  --->fmt = nil,没有变长参数。
fwrite("a")   --->fmt = "a",没有变长参数。
fwrite("%d%d", 4, 5) ---->fmt = "%d%d",变长参数= 4 和 5
```
代码示例3如下:
```cpp
function f(...)
for k,v in ipairs({...}) do
print(k,v)
end
end
f(2,3,3)
1       2
2       3
3       3
```
使用多重赋值的方式
代码如下:
```cpp
function sum3(...)
a,b,c = ...
a = a or 0
b = b or 0
c = c or 0
return a + b +c
end
=sum3(1,2,3,4)
6
return sum3(1,2)
3
```
通常在遍历变长参数的时候只需要使用{…}，然而变长参数可能会包含一些nil；那么就可以用select函数来访问变长参数了：select('#', …)或者 select(n, …)
select('#', …)返回可变参数的长度，select(n,…)用于访问n到select('#',…)的参数
代码如下:
```cpp
=select('#', 1,2,3)
3
return select('#', 1,2, nil,3)
4
=select(3, 1,2, nil,3)
nil       3
=select(2, 1,2, nil,3)
2       nil        3
```
注意：Lua5.0中没有提供…表达式，而是通过一个隐含的局部table变量arg来接收所有的变长参数，arg.n表示参数的个数；
