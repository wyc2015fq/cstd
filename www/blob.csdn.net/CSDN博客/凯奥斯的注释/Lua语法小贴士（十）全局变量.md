# Lua语法小贴士（十）全局变量 - 凯奥斯的注释 - CSDN博客





2016年11月04日 22:44:09[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：789
所属专栏：[Lua语法小贴士](https://blog.csdn.net/column/details/13146.html)









# Lua里我们可以随时声明或调用全局变量。

例如：



```
test = 1
print(test)
```

当我们访问一个不存在的全局变量的时候，会返回nil，例如：



`print(tst)`



虽然很方便，但是往往会因为拼写错误而导致一些不容易发现的BUG。

Lua可以使用_G来表示全局变量所在的表，这样我们就可以修改_G表的__index和__newindex两个元方法（参考[Lua语法小贴士（五）元表与元方法](http://blog.csdn.net/ecidevilin/article/details/52858422)）来限制全局变量的访问。



```
local declaredNames = {}
setmetatable(_G,{
__newindex = function(t,n,v)
    if not declaredNames[n] then
        local w = debug.getinfo(2,"S").what
        if w ~= "main" and w ~= "C" then
            error("attempt to write to undeclared variable "..n, 2)
        end
        declaredNames[n]= true
    end
    rawset(t,n,v)
end,
__index = function(_, n)
    if not declaredNames[n] then
        error("attempt to read undeclared variable"..n, 2)
    else
        return nil
    end
end,})
```

我们通过debug.getinfo（参考[Lua语法小贴士（十一）debug库](http://blog.csdn.net/ecidevilin/article/details/53048445)）获取了函数的类型，Lua表示是普通的lua函数，C表示是C函数，main是在主chunk里，也就是没有被function
 end包围的区域。


这样我们在主chunk里面声明全局变量就没有问题，例如：



```
testChunk = 23
print(testChunk)
```

但是下面这段在函数里面访问未声明的全局变量时，就会报错：




```
local function a()
    funcTest = 1
end

a()
```







