# Lua语法小贴士（六）迭代器 - 凯奥斯的注释 - CSDN博客





2016年10月27日 14:10:23[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：504
所属专栏：[Lua语法小贴士](https://blog.csdn.net/column/details/13146.html)









# Lua里面可以使用for in语句来遍历table，一般会使用内置的ipairs或pairs方法，而这两个方法会返回迭代器（关于迭代器的相关理论知识，参考[小话设计模式（十五）迭代器模式](http://blog.csdn.net/ecidevilin/article/details/52727049)）。本文就介绍一下如何自定义迭代器。

首先我们看一下ipairs的写法：



```
local tbl = {"a","b","c"}
for i, var in ipairs(tbl) do
    print(i, var)
end
--1    a
--2    b
--3    c
```

还有pairs的写法：




```
local tbl = {a="x",b="y",c="z"}
for key, var in pairs(tbl) do
    print(key, var)
end
--a    x
--b    y
--c    z
```

简单来讲，ipairs用来遍历数组形式的table，而pairs可以遍历任何形式的table（也包括数组形式，不过效率比ipairs低）。


那么它们是怎么实现的呢？我们一步一步来，首先我们实现一个迭代器方法：



```
local function inverse(max, idx)
    if idx < max then
        idx = idx + 1
        return idx, -idx
    end
end
```
使用：




```
for i, n in inverse, 3, 0 do
    print(i, n)
end
--1    -1
--2    -2
--3    -3
```
我们看到in后面有三个值，第一个就是我们自定义的迭代器方法inverse，而后面两个就是inverse方法的两个起始参数，最大值和idx，把第三个值从0改成1的话，我们会看到打印结果是从2开始的。


那么为什么写法和ipairs/pairs不一样呢？因为ipairs/pairs并不是迭代器方法，而是迭代器的生成器方法。我们可以写一个迭代器生成器方法：



```
local function toinverse(count)
    return inverse, count, 0
end
```
使用：




```
for i, n in toinverse(2) do
    print(i,n)
end
--1    -1
--2    -2
```


如果在inverse方法里面添加：



`max = max / 2`
然后修改使用，改为toinverse(1024)，你会发现，会从1到打印到512，这是因为inverse方法每次会把idx返回给i（-idx返回给n），然后再次调用的时候，会把i作为第二个参数idx传给inverse，也就是说使用i保存了idx的状态，而max的状态却没有地方保存。那么如果想保存max的状态该怎么办呢？


这里我们考虑使用闭包：



```
local function inversenumber(max)
    local m = max
    return function(max, idx)
        m = m / 2
        if idx < m then
             idx = idx + 1
             return idx, -idx
        end
    end
    ,count,0
end
```
使用：




```
for i, n in inversenumber(1024) do
    print(i, n)
end
```
会从1打印到7。


我们这里使用m来保存最大值的状态，这就跟迭代器方法的参数max没有关系了。










