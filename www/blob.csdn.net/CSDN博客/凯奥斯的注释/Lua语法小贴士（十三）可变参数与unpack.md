# Lua语法小贴士（十三）可变参数与unpack - 凯奥斯的注释 - CSDN博客





2016年11月23日 22:40:49[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：4544
所属专栏：[Lua语法小贴士](https://blog.csdn.net/column/details/13146.html)









# Lua支持可变参数，用...表示。

例如：



```
local function func(...)
    print(...)
end
```

当然也可以在前面添加固定参数


```
local function funca(a,...)
    --TODO:
end
```

可以用花括号将...打包起来成为一个table，例如：


```
local function func(...)
    print(...)
    local tbl = {...}
    print(#tbl)
end
```

调用：

`func(1,2,3,4)`


另外Lua里面可以使用unpack函数将数组形式的table拆开。

例如：



```
local tbl = {2,4,6,8}
local a,b,c,d = unpack(tbl)
```
这个函数有三个参数，第一个参数就是要拆的table，第二个参数是起始元素的下标，默认为1，第三个参数是结束元素的下标，默认为table的长度。



我们可以将拆包出来的元素作为上面func的参数：


`func(unpack(tbl))`

最后补充一点，我们知道Lua的函数可以返回多个返回值，例如：


```
local function funcpp(a)
    return a, a + 1, a + 2
end
```
unpack可以作为尾调用来提供多个返回值。



另外，返回值也可以用花括号来打包，例如：


`print(#{funcpp(4)})`







