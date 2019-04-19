# lua笔记 - Arthur的随笔 - CSDN博客
2013年12月30日 22:49:16[largetalk](https://me.csdn.net/largetalk)阅读数：2236
# lua[](#lua)
先按照官网安装好lua解释器, 下载源码编译也很简单，下载地址：[http://www.lua.org/download.html](http://www.lua.org/download.html)
## lua类型[](#id1)
lua是动态类型语言，变量无须定义
Numbers:
包括整数，无精度限制整数，单精度浮点数，双精度浮点数，复数
>a=>b=a*>print(b)>c=0.7>print(c)
字符和数字转换:
>=tonumber("123")+>=tonumber("123.456e5")
Strings:
>print("hello")>who='lua user'-- 字符串可以使用"或' 多行用[[ ]]>print(who)>print("hello "..who)-- 拼接字符串使用 .. 而不是 +
Boolean:
>x=true>print(notx)>print(==)>print(true~=false)
Tables:
table是一种集合数据类型，集合数据类型用以存放collection(如list, set, arrays和联合数组<associative arrays>), collection包含其他对象(包括numbers, strings, 甚至是集合).
>x={}-- empty table>print(x)
更多table信息见后面
Functions:
如很多动态语言，函数可以赋值给变量
>foo=function()print("hello")end>foo()-- call function>print(foo)
function也可以放入tables
>a="aeiou"-- a string>b=-- a number>c=function()-- a function>print("\n\n\tAin't it grand")>end>d={a,b,c}-- put them in a table>functionprintit(tata)-- print their types.>table.unpack(tata)-- unpack the table>forkey,valueinipairs(tata)doprint(key,type(value))end>end>printit(d)stringnumberfunction
nil values:
>print(x)--x is not defined beforenil>t=nil>print(t)
Userdata:
Userdata变量是lua外部的对象，比如在C中实现的对象。
Thread:
一个thread代表独立的执行线程
Querying type:
lua是反射语言，可以通过type得到变量类型
>x='123'>print(x,type(x))
## Tables[](#tables)
>t={}>t['foo']=>t[]='bar'>=t[]-- return t[3]>t[]=nil-- earse a key/value>f=function()end>t[f]=--任何值都可以作为key，除了nil和NAN(not a number)>t.bar=>=t['bar']--另一种添加key/value的方法>t={["foo"]="bar",[]=}>=t.foobar>=t[]--或者>t={foo="bar"}-- same as ["foo"]="bar" (but not [foo]="bar" , that would use the variable foo)>=t["foo"]bar
如同数组般使用tables
>t={"a","b","c"}>=t[]-- 注意第一个index是1,不是0a>=t[]c--混合模式>t={"a","b",[]="foo","c",name="bar","d","e"}>fork,vinpairs(t)doprint(k,v)endabcdefoonamebar>t={'a','b','c'}>=#t--数组t的长度--add item to the end of array>t={}>table.insert(t,)>t[#t+]=>=t[]>=t[]--也可以指定位置insert>t={"a","c"}>table.insert(t,,"b")>=t[],t[],t[]abc--remove>t={"a","b","c"}>table.remove(t,)>=t[],t[]ac--拼接>t={"a","b","c"}>=table.concat(t,";")a;b;c
table是引用类型，意识是赋值给其他变量时不会产生copy数据
>t={}>u=t>u.foo="bar">=t.foobar>functionf(x)x[]=end>f(t)>=u[]
许多新学习lua的喜欢把table当作数组使用，即使不需要顺序。但这样的问题是删除会很慢（需要移动其他item）检查一个item是否存在也很慢（需要轮寻全部item）
解决办法是把item存在key中，value设置一个dummy值（如true），你就可以像使用无序集合那样来使用table，快速的插入，删除和查找。
这样做的缺点是不好得到item总数（需要循环），也不能存储相同item两次
## Functions[](#functions)
lua中定义函数如下:
function ( args ) body end
return value:
>f=function()>>return"x","y","z"-- return 3 values>>end>a,b,c,d=f()-- assign the 3 values to 4 variables. the 4th variable will be filled with nil>=a,b,c,dxyznil>a,b=(f())-- wrapping a function call in () discards multiple return values>=a,bx,nil>="w"..f()-- using a function call as a sub-expression discards multiple returnswx>print(f(),"w")-- same when used as the arg for another function call...xw>print("w",f())-- ...except when it's the last argwxyz>print("w",(f()))-- wrapping in () also works here like it does with =wx>t={f()}-- multiple returns can be stored in a table>=t[],t[],t[]xyz
参数个数可变:
>f=function(x,...)>>x(...)>>end>f(print,,,)>f=function(...)print(select("#",...))print(select(,...))end>f(,,,,)
named function:
>functionf(...)end-- equivalent to>f=function(...)end
tail calls:
functionfactorial_helper(i,acc)ifi==thenreturnaccendreturnfactorial_helper(i-,acc*i)endfunctionfactorial(x)returnfactorial_helper(x,)end
## Thread[](#thread)
lua的thread其实是协程
yielding:
>functionfoo()>>print("foo",)>>coroutine.yield()>>print("foo",)>>end>
using coroutine.create(fn) to create a coroutine
>co=coroutine.create(foo)-- create a coroutine with foo as the entry>=type(co)-- display the type of object "co"thread
thread state:
>=coroutine.status(co)suspended--The state suspended means that the thread is alive, and as you would expect, not doing anything.
use coroutine.resume() to start the thread, lua will enter the thread and leave when the thread yields
>=coroutine.resume(co)footrue>=coroutine.resume(co)footrue>=coroutine.status(co)dead>=coroutine.resume(co)falsecannotresumedeadcoroutine
## Control Structure[](#control-structure)
ifconditionthenblockelseifconditionthenblockelseblockendwhileconditiondoblockendrepeatblockuntilconditionforvariable=start,stop,stepdoblockendforvar1,var2initeratordoblockendwhiletruedoifconditionthenbreakendendfori=,doifi>thengotocontinueendblock::continue::-- a name surrounded in :: :: is a goto labelendconditionandblock1orblock2
## Metamethods[](#metamethods)
metatable是一个包含一些metamethod的table，通过setmetatable函数把其和某个对象关联起来，那个对象就具备某些功能或能处理某些事件。因为lua是动态语言，给对象添加函数也不是什么大不了的事。
localx={value=}localmt={__add=function(lhs,rhs)-- "add" event handlerreturn{value=lhs.value+rhs.value}end}setmetatable(x,mt)-- use "mt" as the metatable for "x"localy=x+xprint(y.value)--> 10localz=y+y-- error, y doesn't have our metatable. this can be fixed by setting the metatable of the new object inside the metamethod'
getmetatable :
localy=(getmetatable(x).__add(x,x))-- x + x
some event：
__index__newindex__add__eq__lt__le__metatable
## Environments[](#environments)
lua的全局环境是一个table，全局变量就存在这个table里，我们可以为每个函数使用不同的table，这样就能看到不同的全局变量。
默认的全局table存储在一个叫“_G”的变量下
print(_ENV==_G)-- prints true, since the default _ENV is set to the global tablea=localfunctionf(t)localprint=print-- since we will change the environment, standard functions will not be visiblelocal_ENV=t-- change the environment. without the local, this would change the environment for the entire chunkprint(getmetatable)-- prints nil, since global variables (including the standard functions) are not in the new enva=-- create a new entry in t, doesn't touch the original "a" globalb=-- create a new entry in tendlocalt={}f(t)print(a,b)--> 1 nilprint(t.a,t.b)--> 2 3'
## Modules[](#modules)
create an example file mymodule.lua
localmymodule={}functionmymodule.foo()print("hello world!")endreturnmymodule
So that you can require the same module in different files without re-running the module code, Lua caches modules in the package.loaded table.
to actually reload the module, you need:
package.loaded.mymodule = nil
mymodule = require "mymodule"
other ways to create a module:
local mymodule = {}
local function private()
    print("in private function")
end
function mymodule.foo()
    print("Hello World!")
end
function mymodule.bar()
    private()
    mymodule.foo() -- need to prefix function call with module
end
return mymodule
or 我比较喜欢的方式:
local mymodule = {}
local function private()
    print("in private function")
end
local function foo()
    print("Hello World!")
end
mymodule.foo = foo
local function bar()
    private()
    foo()
end
mymodule.bar = bar
return mymodule
package.path (for modules written in Lua) and package.cpath (for modules written in C) are the places where Lua looks for modules.
