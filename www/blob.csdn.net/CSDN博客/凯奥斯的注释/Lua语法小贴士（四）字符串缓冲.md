# Lua语法小贴士（四）字符串缓冲 - 凯奥斯的注释 - CSDN博客





2016年10月11日 13:53:31[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：851
所属专栏：[Lua语法小贴士](https://blog.csdn.net/column/details/13146.html)









我们在写Lua代码的时候，如果需要对一个字符串进行反复的连接操作的话，会造成极大的性能开销。因为字符串是不可变的值，所以当我们对字符串操作的时候，每次都会生成一个新的字符串。例如我们需要对一个10000字节的字符串str增加一个字符的时候，就会创建一个10001字节的字符串str_copy，然后再复制str到str_copy里面。所以我们需要字符串缓冲（string buffer）来解决这个问题，而Lua里面，string buffer就可以使用table来实现。



```
local t = {}
local test = {"a","x","def","type","fire","time","bottle","xo","wtf"}
for i, var in ipairs(test) do
    t[#t+1] = var
    t[#t+1] = "\n"
end
local s = table.concat(t)
print(s)
```


当然我们也可以指定一个分隔符作为table.concat方法的第二个参数：



```
local t = {}
local test = {"a","x","def","type","fire","time","bottle","xo","wtf"}
for i, var in ipairs(test) do
    t[#t+1] = var
end
t[#t+1] = ""
local s = table.concat(t,"\n")
print(s)
```


参考：


[Lua语法小贴士（一）字符串基础操作](http://blog.csdn.net/ecidevilin/article/details/52775530)


[Lua语法小贴士（二）string库](http://blog.csdn.net/ecidevilin/article/details/52786404)


[Lua语法小贴士（三）魔法字符](http://blog.csdn.net/ecidevilin/article/details/52786488)




