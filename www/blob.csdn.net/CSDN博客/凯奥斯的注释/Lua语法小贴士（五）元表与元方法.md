# Lua语法小贴士（五）元表与元方法 - 凯奥斯的注释 - CSDN博客





2016年10月19日 13:59:08[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：800
所属专栏：[Lua语法小贴士](https://blog.csdn.net/column/details/13146.html)









# Lua里的元表（metatable）是指一套预定操作的集合，而这些操作称为元方法（metamethod）。

例如我们可以让两个数字相加，但是却没办法将两个table相加。如果想让表a加上表b，那么就需要使用元表里的__add元方法。



```
local tbl = {}
local mt = {}
mt.__add = function(a,b)
    return tbl.new(a.val + b.val)
end
function tbl.new(val)
    local t = {}
    setmetatable(t,mt)
    t.val = val
    return t
end

local a = tbl.new(1)
local b = tbl.new(2)
print((a+b).val)
```


元方法类似于c++里的运算符方法。


Lua里的算术操作除了__add之外，还有__mul（乘法）、__sub（减法）、__div（除法）、__unm（相反数）、__mod（取模）和__pow（乘幂）。此外还有__concat（连接）。

然后是关系操作：__eq（等于）、__lt（小于）和__le（小于等于），而对应的三种关系运算符（不等于，大于等于，大于）会通关前面的方法取反获得，不需要也不能实现。

接着是库定义的操作：__tostring（转字符串，print方法会自动调用这个元方法）

最后是访问操作：__index（索引）和__newindex（新索引），访问table的时候会优先访问table自身的索引，如果没有找到这个索引，会调用__index（查询）或__newindex（赋值）方法。

例如下面这段代码：



```
local tbl = {}
local mt = {
__index = function(t,k)
    print("index "..k)
    return rawget(t,k)
end,
__newindex = function(t,k,v)
    print("newindex "..k)
    rawset(t,k,v)
end
}
setmetatable(tbl,mt)
tbl.val = 1  --newindex val
tbl.val = 2
local val = tbl.val
local a = tbl.a --index a
```
我们可以借助这两个元反方实现一个只读的table：




```
local function readonly(t)
    local proxy = {}
    local mt = {
        _index = t,
        _newindex=function(t,k,v)
            error("!!Readonly!!", 2)
        end
    }
    setmetatable(proxy,mt)
    return proxy
end
local days = readonly({"Sun","Mon","Tue","Wed","Thu","Fri","Sat"})
print(days[1]) --Sun
days[2]="Noday" --!!Readonly!!
```
（此段代码改写自《Programming in Lua (Second Edition)》）


需要注意的两个点：


1、__index元方法可以直接使用原table代替函数，这样简单且高效。不过，需要每个只读代理都要创建一个新的元表。

2、这里使用了代理模式，可以参考[小话设计模式（十二）代理模式](http://blog.csdn.net/ecidevilin/article/details/52718068)





