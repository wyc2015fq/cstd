# lua学习（一） - xmdxcsj的专栏 - CSDN博客





2015年10月18日 22:58:59[xmucas](https://me.csdn.net/xmdxcsj)阅读数：765








学习资源：

官方手册：http://www.lua.org/manual/5.3/





用户社区：http://lua-users.org/





programming in lua中文版：http://www.centoscn.com/uploads/file/20130903/13781389409335.pdf





一、lua概况

可扩展性。Lua 的扩展性非常卓越，以至于很多人把 Lua用作搭建领域语言的工具（注：比如游戏脚本）。

简单。Lua 本身简单，小巧；内容少但功能强大，这使得 Lua易于学习，很容易实现一些小的应用。

高效率。Lua 有很高的执行效率，统计表明 Lua是目前平均效率最高的脚本语言。

与平台无关。Lua 几乎可以运行在所有我们听说过的系统上。

二、细节

1.注释

单行注释：--

多行注释：--[[ --]]

2.命令行参数

lua [options] [script [args]]

全局变量arg存放Lua的命令行参数。

prompt> lua -e "sin=math.sin" script a b

arg表如下：

arg[-3] = "lua"

arg[-2] = "-e"

arg[-1] = "sin=math.sin"

arg[0] = "script"

arg[1] = "a"

arg[2] = "b"

3.基本类型

8种基本类型：nil、boolean、number、string、userdata、function、thread和table

Booleans：除了false和nil以外都是真，0和空串都是真

Number：只有实数，lua没有整数

String：使用[[]]表示多行字符串

Table：{x=10, y=45; "one", "two", "three"}

4.局部变量

local：限制局部变量的有效范围


1.避免命名冲突

2.访问局部变量的速度比全局变量更快.


使用do...end控制局部变量的范围

三、基本语法

1.if判断

if conditions then

     then-part

end;

if conditions then

     then-part

else

     else-part

end;

if conditions then

     then-part

elseif conditions then

     elseif-part

.. --->多个elseif

else

     else-part

end;

2.while循环语句

while condition do

     statements;

end;

3.for语句

for var=exp1,exp2,exp3 do

loop-part

end

for将用exp3作为step从exp1（初始值）到exp2（终止值），执行loop-part。其中exp3可以省略，默认step=1

-- print all values of array 'a'

for i,v in ipairs(a) do print(v) end

四、函数

1.形式

function func_name (arguments-list)

     statements-list;

end;

2.使用表作为函数的参数

function rename (arg)

     return os.rename(arg.old, arg.new)

end




rename{old="temp.lua", new="temp1.lua"}




3.函数也是一种类型，可以作为参数，也可以放在表里

a = {p = print}

a.p("Hello World") --> Hello World

print = math.sin -- `print' now refers to the sine function

a.p(print(1)) --> 0.841470

sin = a.p -- `sin' now refers to the print function

sin(10, 20) --> 10 20


既然函数是值，那么表达式也可以创建函数了，Lua中我们经常这样写：

function foo (x) return 2*x end

这实际上是利用Lua提供的“语法上的甜头”（syntactic sugar）的结果，下面是原本的函数：

foo = function (x) return 2*x end



如果我们想通过表的name域排序：

table.sort(network, function (a,b)

return (a.name > b.name)

end)

以其他函数作为参数的函数在Lua中被称作高级函数，高级函数在Lua中并没有特权，只是Lua把函数当作第一类函数处理的一个简单的结果。


4.闭包


当一个函数内部嵌套另一个函数定义时，内部的函数体可以访问外部的函数的局部变量，这种特征我们称作词法定界。虽然这看起来很清楚，事实并非如此，词法定界加上第一类函数在编程语言里是一个功能强大的概念，很少语言提供这种支持。

下面看一个简单的例子，假定有一个学生姓名的列表和一个学生名和成绩对应的表；现在想根据学生的成绩从高到低对学生进行排序，可以这样做：

names = {"Peter", "Paul", "Mary"}

grades = {Mary = 10, Paul = 7, Peter = 8}

table.sort(names, function (n1, n2)

     return grades[n1] > grades[n2] -- compare the grades

end)

假定创建一个函数实现此功能：

function sortbygrade (names, grades)

     table.sort(names, function (n1, n2)

      return grades[n1] > grades[n2] -- compare the grades

     end)

end

例子中包含在sortbygrade函数内部的sort中的匿名函数可以访问sortbygrade的参数grades，在匿名函数内部grades不是全局变量也不是局部变量，我们称作外部的局部变量（external local variable）或者upvalue



看下面的代码：

function newCounter()

     local i = 0

     return function() -- anonymous function

          i = i + 1

          return i


end


end

c1 = newCounter()

print(c1()) --> 1

print(c1()) --> 2


匿名函数使用upvalue i保存他的计数，当我们调用匿名函数的时候i已经超出了作用范围，因为创建i的函数newCounter已经返回了。然而Lua用闭包的思想正确处理了这种情况。简单的说闭包是一个函数加上它可以正确访问的upvalues。如果我们再次调用newCounter，将创建一个新的局部变量i，因此我们得到了一个作用在新的变量i上的新闭包。

c2 = newCounter()

print(c2()) --> 1

print(c1()) --> 3

print(c2()) --> 2

c1、c2是建立在同一个函数上，但作用在同一个局部变量的不同实例上的两个不同的闭包。



4.非全局函数-表里的函数


1.表和函数放在一起

Lib = {}

Lib.foo = function (x,y) return x + y end

Lib.goo = function (x,y) return x - y end

2.使用表构造函数

Lib = {

     foo = function (x,y) return x + y end,

     goo = function (x,y) return x - y end



}

3. Lua提供另一种语法方式

Lib = {}

function Lib.foo (x,y)

     return x + y

end

function Lib.goo (x,y)

     return x - y

end

当我们将函数保存在一个局部变量内时，我们得到一个局部函数，也就是说局部函数像局部变量一样在一定范围内有效。这种定义在包中是非常有用的：因为Lua把chunk当作函数处理，在chunk内可以声明局部函数（仅仅在chunk内可见），词法定界保证了包内的其他函数可以调用此函数


四、迭代器与泛型for

1.迭代器


迭代器是一种支持指针类型的结构，它可以遍历集合的每一个元素。在Lua中我们常常使用函数来描述迭代器，每次调用该函数就返回集合的下一个元素。

迭代器需要保留上一次成功调用的状态和下一次成功调用的状态，也就是他知道来自于哪里和将要前往哪里。闭包提供的机制可以很容易实现这个任务。记住：闭包是一个内部函数，它可以访问一个或者多个外部函数的外部局部变量。每次闭包的成功调用后这些外部局部变量都保存他们的值（状态）。当然如果要创建一个闭包必须要创建其外部局部变量。所以一个典型的闭包的结构包含两个函数：一个是闭包自己；另一个是工厂（创建闭包的函数）。



举一个简单的例子，我们为一个list写一个简单的迭代器，与ipairs()不同的是我们实现的这个迭代器返回元素的值而不是索引下标：

function list_iter (t)

     local i = 0

     local n = table.getn(t)

     return function ()

          i = i + 1

          if i <= n then return t[i] end

     end

end



我们设计的这个迭代器也很容易用于范性for语句

t = {10, 20, 30}

for element in list_iter(t) do

     print(element)

end

范性for为迭代循环处理所有的薄记（bookkeeping）：首先调用迭代工厂；内部保留迭代函数，因此我们不需要iter变量；然后在每一个新的迭代处调用迭代器函数；当迭代器返回nil时循环结束（后面我们将看到范性for能胜任更多的任务）。


2.范性for的语义

for line in io.lines() do

io.write(line, '\n')

end


首先，初始化，计算in后面表达式的值，表达式应该返回范性for需要的三个值：迭代函数，状态常量和控制变量；与多值赋值一样，如果表达式返回的结果个数不足三个会自动用nil补足，多出部分会被忽略。

第二，将状态常量和控制变量作为参数调用迭代函数（注意：对于for结构来说，状态常量没有用处，仅仅在初始化时获取他的值并传递给迭代函数）。

第三，将迭代函数返回的值赋给变量列表。



第四，如果返回的第一个值为nil循环结束，否则执行循环体。

第五，回到第二步再次调用迭代函数。


3.无状态的迭代器

a = {"one", "two", "three"}

for i, v in ipairs(a) do

print(i, v)

end

4.多状态的迭代器

很多情况下，迭代器需要保存多个状态信息而不是简单的状态常量和控制变量，最简单的方法是使用闭包，还有一种方法就是将所有的状态信息封装到table内，将table作为迭代器的状态常量，因为这种情况下可以将所有的信息存放在table内，所以迭代函数通常不需要第二个参数。



