# Lua语法小贴士（十一）debug库 - 凯奥斯的注释 - CSDN博客





2016年11月05日 23:50:19[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：4017
所属专栏：[Lua语法小贴士](https://blog.csdn.net/column/details/13146.html)









# Lua的调试库包含两种函数：自省函数和钩子。自省函数可以用来检查一个正在运行的程序的信息，而钩子可以跟踪程序的运行。

自省函数主要是debug.getinfo函数，函数声明为：


`debug.getinfo([thread,]func[,what])`



返回一个函数的信息表。可以直接传入函数，也可以传入一个数值，这个数值表示函数（运行在指定线程）的调用栈深度。0表示getinfo自己，1表示调用getinfo的函数,以此类推（一般是2）。如果这个数值大于活动函数的深度，就会返回nil。

返回值是一个table，包含以下字段：

source：函数定义的位置，如果函数是通过loadstring在一个字符串中定义的，那么source就是这个字符串。如果函数是在一个文件中定义的，那么source就是这个文件名加前缀‘@’。

short_src：source的短版本（最多60个字符），可用于错误信息中。

linedefined：该函数定义的源代码中第一行的行号。

lastlinedefined：该函数定义的源代码中最后一行的行号。

what：函数的类型。如果foo是一个普通的Lua行数，则为“Lua”，如果是一个C函数，则为“C”，如果是Lua主程序块（chunk）部分，则为“main”。

name：该函数一个适当的名称。

namewhat：可能是“global”、“local”、“method”、“field”或“”（空字符串）。空字符串表示没有找到该函数的名称。

nups：该函数的upvalue的数量

activelines：一个table，包含了该函数的所有活动行的几何。所谓“活动行”就是含有代码的行（相对于空行或者只包含注释的行）。

func：函数本身。




注：当foo是一个C函数时，只有what、name和namewhat是有意义的。




what可选参数可以指定希望获取那些信息。


|'n'|name & namewhat|
|----|----|
|'f'|func|
|'S'|source, short_src, what, linedefined & lastlinedefined|
|'l'|currentline|
|'L'|activelines|
|'u'|nups|


debug.getlocal可以用来检查任意活动函数的局部变量，该函数有两个参数：一个是希望查询的函数栈层，另一个是变量的索引。返回值为变量的名字和当前值。

而变量的索引是在函数中出现的顺序，只限于函数当前作用域中活跃的变量。



```
local function foo(a,b)
    local x
    do local c = a - b end
    local a = 1
    while true do
        local name,value = debug.getlocal(1,a)
        if not name then break end
        print(name,value)
        a = a + 1
    end
end

foo(10,20)
```


打印结果：


a    10

b    20

x    nil

a    4

另外还可以使用debug.setlocal函数改变局部变量的值，除了与debug.getlocal一样的参数外，还有第三个参数，也就是变量的新值。




此外还有getupvalue和setupvalue，用来访问和修改非局部变量，而第一个参数需要传入函数。




debug库中所有的自省函数都可以在参数列表前增加一个协同程序（参考[Lua语法小贴士（八）协同程序](http://blog.csdn.net/ecidevilin/article/details/53001188)）作为第一个（可选）参数，这样就可以从外部来检查协同程序。

例如：



```
local co = coroutine.create(function()
    local x = 10
    coroutine.yield()
    error("some error")
end)
coroutine.resume(co)
print(debug.traceback(co))
print(debug.getlocal(co,1,1))
```


这样就可以回溯协同程序，并且访问co中的局部变量。




Lua中可以使用debug.sethook来将函数设置为钩子函数。

函数声明：


`debug.sethook([thread,] hook[, mask][, count])`
mask和count表示什么时候hook会被调用，mask字符串中可以包含以下字符：


"c"：每当lua调用（call）函数的时候

"r"：每当lua从函数返回（return）的时候

"l"：每当lua执行一行新代码的时候。

而当count值大于零的时候，每执行完count数量的指令后会触发钩子。

当没有任何参数的时候（debug.sethook）表示关闭钩子。

例如：


`debug.sethook(print,"l")`将print设置为钩子函数，它会打印出程序执行到的每一行。


还可以获取更详细的信息，例如：




```
local function trace(event, line)
    local s = debug.getinfo(2).short_src
    print(s..":"..line)
end
debug.sethook(trace,"l")
```
这里使用了getinfo函数来获取当前文件名。






