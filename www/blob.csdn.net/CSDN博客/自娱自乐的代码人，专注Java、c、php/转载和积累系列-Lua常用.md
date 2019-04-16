# 转载和积累系列 - Lua常用 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年12月23日 17:05:13[initphp](https://me.csdn.net/initphp)阅读数：4005








## Lua简介：

 Lua 是一个小巧的脚本语言。是巴西里约热内卢天主教大学（Pontifical Catholic University of Rio de Janeiro）里的一个研究小组，由Roberto Ierusalimschy、Waldemar Celes 和 Luiz Henrique de Figueiredo所组成并于1993年开发。 其设计目的是为了嵌入应用程序中，从而为应用程序提供灵活的扩展和定制功能。Lua由标准C编写而成，几乎在所有操作系统和平台上都可以编译，运行。Lua并没有提供强大的库，这是由它的定位决定的。所以Lua不适合作为开发独立应用程序的语言。Lua
 有一个同时进行的JIT项目，提供在特定平台上的即时编译功能。




## 语法约定：

保留字：


     and       break     do        else      elseif
     end       false     for       function  if
     in        local     nil       not       or
     repeat    return    then      true      until     while




## Hello World:

学习Lua的前提是假设你已经拥有一门其他语言的经验，例如Java，或者PHP。

一般学习语言入门，第一步都是输入hello world

lua代码的结尾没有“;”



`print("hello world")`



## Lua的值与类型：

Lua 是一种 动态类型语言。这意味着变量没有类型，只有值才有类型。语言中不存在类型定义。而所有的值本身携带它们自己的类型信息。

Lua 中有八种基本类型： nil, boolean, number, string, function, userdata, thread, and table 

下面是每一种类型的例子：

> 
### nil类型：

nil类型相当于其它语言的null。空类型。

```
--nil type 注释
username = nil --定义username为nil类型
--if else判断
if (username == nil) then 
        print("this is nil");
else
        print("this is not nil");
end
```


### Boolean类型：

布尔类型，true或者false。

```
--boolean
isTrue = true
print(isTrue)
```



### Number类型：

数值类型，Lua没有Int Float等类型。计算的过程会强制转换。

```
--number
a = 100
b = 100.5
print(a + b)
```


### String类型：

字符串类型之间的连接通过 .. 实现


```
--String
str = "hello lua"
print(str)       
str2 = " haha"
print(str..str2)
```


### Function类型：

方法类型。Lua，没有Class等一些高级语言拥有的语言特性。

```
--function       
function parse(a, b)
        local c = 0.5;
        return (a + b) * c
end                       
print(parse(10,20))       
print(parse(5, 11.2))
```




### userdata类型：

userdata 类型用来将任意 C 数据保存在 Lua 变量中。这个类型相当于一块原生的内存，除了赋值和相同性判断，Lua 没有为之预定义任何操作。然而，通过使用 metatable （元表） ，程序员可以为 userdata 自定义一组操作。 userdata 不能在 Lua 中创建出来，也不能在 Lua 中修改。这样的操作只能通过 C API。这一点保证了宿主程序完全掌管其中的数据。





### Thread类型：

*thread* 类型用来区别独立的执行线程，它被用来实现 coroutine （协同例程）。不要把 Lua 线程跟操作系统的线程搞混。 Lua 可以在所有的系统上提供对 coroutine 的支持，即使系统并不支持线程。


### Table类型：

table类型非常强大，相当于PHP中的数组。

*table* 类型实现了一个关联数组。也就是说，数组可以用任何东西（除了**nil**）做索引，而不限于数字。 table 可以以不同类型的值构成；它可以包含所有的类型的值（除**nil** 外）。 table 是 lua 中唯一的一种数据结构；它可以用来描述原始的数组、符号表、集合、记录、图、树、等等。用于表述记录时，lua 使用域名作为索引。语言本身采用一种语法糖，支持以`a.name` 的形式表示`a["name"]`。有很多形式用于在
 lua 中创建一个 table。

```
--function                       
function parse(a, b)             
        local c = 0.5;           
        return (a + b) * c       
end                       
print(parse(10,20))       
print(parse(5, 11.2))     
                          
--table                   
userTable = {}            
userTable.username = "initphp"
userTable.age = 20            
userTable.money = 100         
userTable.info = {}           
userTable.info.height = 172   
userTable.info.func = parse   
print("username:" .. userTable.username .. ";userage:" .. userTable.age)
print("userInfo:" .. userTable.info.height)                             
print("function :" .. parse(10,20))
```




返回：

![](https://img-blog.csdn.net/20131223174926593?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaW5pdHBocA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




**Lua的变量可以是一个数字，一个字符串，也可以是一个方法。所以Lua在语法糖方面是非常简单的和优雅的。**





## Lua语句：

> 
### 多重赋值：

```
--fuzhi                                                                 
a,b = 10,20                                                          
print("a:" .. a .. " b:" .. b);
```


### IF 语句：

```
val = 2000                                                        
if val == 100 then                                             
        print("val=100")                                      
elseif val == 200 then                                      
        print("val=200")                                       
else                                                                  
        print("val=" .. val)                                     
end
```


### AND OR：

```
--and or                                                                
a = 10                                                                  
b = 20                                                                  
print(a and b)                                                       
print(a or b)
```


### #取长度操作符：

```
--#
str = "hello"
print("LEN:" .. #str) --#str取字符串长度
myTable = {}
myTable[0] = "111"
myTable[1] = "2222"
print("LEN:" .. #myTable) --数组的话是展示的下标，不太准哦..
```


### Lua操作符优先级：
     or
     and
     <     >     <=    >=    ~=    ==
     ..
     +     -
     *     /     %
     not   #     - (unary)
     ^


### Local关键词，局部变量和全局变量：

```
--local                                                               
a = 100     --全局                                             
c = 50                                                               
function add(b)                                                 
        local c = 100    --局部
        print(a + b + c)                                         
end                                                                   
add(20)
```


### functioncall，函数可以赋值给一个变量：

```
--functioncall  
function adddo(a, b, c)      
        print(a + b + c)  
end 
test = adddo                                                    
test(10,20,30)
```


### for循环：

```
--for
for i = 1, 10, 1
do       
        print("TEST:" .. i)
end
```

```
--打印Table
days = {"Suanday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"
}          
for i, v in ipairs(days)              
do    
print(v)  
end
```


### while循环：

```
--while                                                                  count = 10           
while count > 0                    
do              
        print("WHILE:" .. count)               
        count = count - 1           
end
```


### Repeat循环：

```
--repeat                         
count2 = 10
repeat
        print("REPEAT:" .. count2)
        count2 = count2 - 1
until count2 < 1
```



### 变量作用域：

```
x = 10                -- 全局变量
     do                    -- 新的语句块
       local x = x         -- 新的一个 'x', 它的值现在是 10
       print(x)            --> 10
       x = x+1
       do                  -- 另一个语句块
         local x = x+1     -- 又一个 'x'
         print(x)          --> 12
       end
       print(x)            --> 11
     end
     print(x)              --> 10  （取到的是全局的那一个）
```


### function返回多个值：

```
--返回两个值
function returnData()
	return 100,200
end

a,b = returnData()
print("a:" .. a .. "b:" .. b)
```







## Lua Basic Function 基本方法库：

> 
### assert 断言：

assert (v [, message])

说明：

相当于C的断言，参数：

v：当表达式v为nil或false将触发错误,

message：发生错误时返回的信息，默认为"assertion failed!"

例子：

```
local a = "HELLO"
if (assert(a)) then
	print("HELLO")
end
```


### collectgarbage 垃圾收集器接口：

collectgarbage (opt [, arg])

说明：

是垃圾收集器的通用接口，用于操作垃圾收集器。参数：

opt：操作方法标志

"stop": 停止垃圾收集器

"restart": 重启垃圾收集器

"collect": 执行一次全垃圾收集循环

"count": 返回当前Lua中使用的内存量(以KB为单位)

"step": 单步执行一个垃圾收集. 步长 "Size" 由参数arg指定　(大型的值需要多步才能完成)，如果要准确指定步长，需要多次实验以达最优效果。如果步长完成一次收集循环，将返回True

"setpause": 设置 arg/100 的值作为暂定收集的时长

"setstepmul": 设置 arg/100 的值，作为步长的增幅(即新步长＝旧步长*arg/100)

```
local a = "HELLO"
if (assert(a)) then
	print("HELLO")
end
--打印内存使用量
print(collectgarbage("count"))
```


### dofile 执行文件：

dofile (filename)

说明：

打开并且执行一个lua块,当忽略参数filename时，将执行标准输入设备(stdin)的内容。返回所有块的返回值。当发生错误时，dofile将错误反射给调用者。

不能在保护模式下运行。

test2.lua
`print("test2Hello")`

test.lua`dofile("test2.lua")`


### error 错误处理：

error (message [, level])

功能：终止正在执行的函数，并返回message的内容作为错误信息(error函数永远都不会返回)

通常情况下，error会附加一些错误位置的信息到message头部.

Level参数指示获得错误的位置,

Level=1[默认]：为调用error位置(文件+行号)

Level=2：指出哪个调用error的函数的函数

Level=0:不添加错误位置信息






## Lua Module 模块化：



### 　　module (name [, ···])



　　功能：建立一个模块。

　　当package.loaded[name]中存在时，当中的表作为module;

　　当在全局表中存在name指定的表时，此表作为module;

　　当以前两种情况都不存表name时，将新建一个表，并使其作为全局名name的值，并package.loaded[name],而且设t._NAME为name,t._M为module,t._PACKAGE为包的全名(模块名-组件a.b.c);最后把此module设t作为当前函数的新环境表和package.loaded[name]的新值(也就是说，旧的环境表将不能访问,除了加上package.seeall参数外)，以被require使用

　　module(name)后的可选参数为接收module名的函数,如package.seeall




### 　　require (modname)

　　功能：加载指定的模块。

　　此函数先检测package.loaded表中是否存在modname，存在则直接返回当中的值，没有则通过郰定义的加载器加载modname。

　　查找加载器顺序：

　　(1)检测package.preload表是否存在modname，有则加载

　　(2)通过Lua Loader加载,通过查找存放于package.path的路径加载，有则加载

　　(3)通过C Loader加载，通过查找存放于package.cpath的路径加载，有则加载

　　(4)通过all-in-one Loader加载：

　　通过查找modname.dll并查找当中的luaopen_

　　其中XXXX为载块名-后的字符用_替换.后的字符:如：a.v1-b.c 当函数名为luaopen_b_c

　　当require查找的不是一个Lua库或C库，它就会调用all-in-one loader,此加载器是用C路径作为载块的目录，

　　当查找到合适的加载器时，require就会加载其中的模块，当加载器有返回值，将会存放于package.loaded[modname]表。最后返回package.loaded[modname]表

　　当加载失败时，require将触发错误




### 　  package.cpath

　　功能：用于require　C loader的搜索路径

　　可以通过修改LUA_CPATH变量(luaconf.h)修改此值




### 　  package.loaded

　　功能：一个用于让require知道哪些模块已加载的记录表，如果package.loaded已经有require要的值，则直接返回此值




### 　  package.loadlib (libname, funcname)

　　功能：通过动态连接C函数库方式加载Lua扩展库

　　libname为库文件名，funcname为入口函数(此函数必须为纯C接口函数 c++则需用 extern "C" {} 进行限制)




###       package.path

　　功能：用于require　Lua loader的搜索路径

　　可以通过修改LUA_PATH变量(luaconf.h)修改此值




### 　  package.preload

　　功能：一个用于保存特殊模块加载器的表




### 　  package.seeall(module)

　　功能：为module设置一个元表，此元表的__index字段的值为全局环境_G。所以module可以访问全局环境







