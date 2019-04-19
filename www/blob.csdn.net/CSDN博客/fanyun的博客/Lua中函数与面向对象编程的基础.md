# Lua中函数与面向对象编程的基础 - fanyun的博客 - CSDN博客
2017年04月04日 18:55:02[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：462
所属专栏：[巧用脚本](https://blog.csdn.net/column/details/15130.html)
Lua语言具有类等面向对象的特性，其函数调用也遵循C++等语言的方法调用，且受到作用域的约束：
1.      lua函数
调用函数都需要写圆括号，即使没有参数，但有一种特殊例外：函数若只有一个参数且参数是字面字符串或table构造式，则圆括号可有可无，如dofile 'a.lua'，f{x=10, y=20}。
Lua为面向对象式的调用提供冒号操作符的特殊语法，如o.fun(o, x)等价于o:fun(x)。和Javascript类似，调用函数时提供的实参数量可以与形参数量不同，若实参多了则舍弃，不足则多余的形参初始化为nil。
1.1 多重返回值
Lua允许函数返回多个结果，函数返回如returnmax, index，接收如s, e = string.find("hello Lua world", "Lua")。如果一个函数调用不是一系列表达式的最后一个元素，则只产生一个值： 
```cpp
function fun() return "a","b" end
x, y = fun(), 20  -- x="a", y=20（fun的第二个返回值被丢弃）
print(fun() .. "x")  -- 输出ax，这是因为当函数出现在一个表达式中时，Lua会将其返回值数量调整为1
```
另外，只有当一个函数调用作为最后一个元素时，返回值才不会被调整，在其他位置都会被调整为1个，如t = {fun2()}则t={“a”, “b”}，t = {fun2(), 4}则t={“a”, 4}。
特殊函数unpack接受一个数组作为参数，并从下标1开始返回该数组的所有元素，如a, b = unpack({10, 20, 30})，则30被丢弃。unpack的一项重要用途体现在“泛型调用”机制中。
1.2 变长参数
函数参数表中3个点（…）表示该函数可接受不同数量的实参。在Lua 5.0中，没有提供“…”表达式，如果要遍历变长参数，可以访问函数内隐含的局部变量arg。如果还有固定参数，则必须放在变长参数之前。
2. 高级主题
2.1 closure闭合函数
和Javascript的闭包基本是一个东西，此处不再赘述。从技术上说，Lua中只有closure，而不存在“函数”，因为函数本身就是一种特殊的closure。closure的应用很广泛，如用于高阶函数的参数、为GUI工具包创建回调、重定义函数并在新实现中调用旧实现、创建“沙盒”安全运行环境等等。
2.2 非全局的函数
大部分Lua库都采用了将函数存储在table中的机制（如io.read，math.sin），例如下面采用了三种方式来定义table的成员函数：
```cpp
MathLib = {
 plus = function(x, y) return x + y end
}
MathLib.minus = function(x, y) return x - yend
function MathLib.multiply(x, y) return x *y end
局部函数的定义：
local f = function(<参数>) <函数体> end
local function f(<参数>) <函数体> end-- Lua提供的语法
```
**注意如果定义递归函数，不能使用上面第一种定义方式（因为在函数体调用f时，f尚未定义完毕），使用第二种“语法糖”则没问题；或者使用“前向声明”，先local f再f = function ...这样定义。
2.3 正确的尾调用
当一个函数调用时另一个函数的最后一个动作时，该调用算是一条“尾调用”，例如function f(x) return g(x) end。由于在尾调用后程序不要保存任何关于该函数的栈信息，所以递归调用不会耗费栈空间，可以递归调用无数次。有一些看似是“尾调用”的代码，其实都违背了这条准则：
```cpp
function f(x) g(x) end  -- 调用g后，f没有立即返回，还需要丢弃g返回的临时结果
function f(x) return g(x) + 1  -- 还要做一次加法
function f(x) return x or g(x)  -- 必须调整为一个返回值
所以，只有形如return <func>(<args>)这样的调用形式才算是尾调用。
```
面向对象编程
Lua中的table就是一种对象，因为它和对象一样可以拥有状态，也拥有一个独立于其值的标识（一个self），也和对象一样具有独立于创建者的生命周期。但是Lua中没有类的概念，只能用元表来实现原型，用原型来模拟类和继承等面向对象特性。本文将介绍Lua关于面向对象编程的内容。
1 self与冒号语法
使用self参数是所有面向对象语言的一个核心，Lua只需使用冒号语法，就能隐藏该参数，例如下面两段代码是等价的。
```cpp
Account = {balance=0}
funtion Account.withdraw(self, v)
 self.balance = self.balance - v
end
a1 = Account; Account = nil
a1.withdraw(a1, 100.0) -- 注意这是可以运行的
 
function Account:withdraw(v)
 self.balance = self.balance - v
end
a2 = Account
a2:withdraw(100.0) -- 省略了a2参数传入
```
2 类的编写
在一些基于原型的语言中，对象是没有类型的，但每个对象都有一个原型。原型是一种常规的对象，当其他对象遇到一个未知操作时，原型会先查找它。在这种语言中要表示一个类，只需创建一个专用做其他对象的原型。Lua中实现原型很简单，只需用元表的__index来实现继承。
（当访问一个table中不存在的字段key时，一般得到结果为nil。事实上，访问会促使解释器去查找一个叫__index的元方法，如果没有这个元方法，则访问结果如前述的nil，否则由这个元方法来提供结果。元方法除了是一个函数，还可以是一个table，如果是table则直接返回该table中key对应的内容。）
如果有两个对象a和b，要让b作为a的一个原型，只需setmetatable(a, {__index=b})。a就会在b中查找它没有的操作。
```cpp
function Account:new(o)
  o =o or {} -- 如果用户没有提供table，则创建一个
 setmetatable(o, self)
 self.__index = self
 return o
end
```
当调用a = Account:new{balance = 0}时，a会将Account（函数中的self）作为其元表。当调用a:withdraw(100.0)时，Lua无法在table a中找到条目withdraw，则进一步搜索元表的__index条目，即getmetatable(a).__index.withdraw(a,100.0)。由于new方法中做了self.__index = self，所以上面的表达式又等价于Account.withdraw(a, 100.0)，这样就传入了a作为self参数，又调用了Account类的withdraw函数。这种创建对象的方式不仅可以作用于方法，还可以作用于所有其他新对象中没有的字段。
3 继承
现在要从Account类派生出一个子类SpecialAccount（以使客户能够透支），只需：
```cpp
SpecialAccount = Account:new()
s = SpecialAccount:new{limit=1000.00}
SpecialAccount从Account继承了new，当执行SpecialAccount:new时，其self参数为SpecialAccount，因此s的元表为SpecialAccount。
```
当调用s不存在的字段时，会向上查找，也可以编写新的重名方法覆盖父类方法。
4 多重继承
上面介绍中为__index元方法赋值一个table实现了单继承，如果要实现多重继承，可以让__index字段成为一个函数，在该函数中搜索多个基类的方法字段。由于这种搜索具有一定复杂性，多重继承的性能不如单一继承。还有一种改进性能的简单做法是将继承的方法复制到子类中，但这种做法的缺点是当系统运行后就较难修改方法的定义，因为这些修改不会沿着继承体系向下传播。
5 私密性
Lua在设计对象时，没有提供私密性机制（private），但其各种元机制使得程序员可以模拟对象的访问控制。这种实现不常用，因此只做基本的了解：通过两个table来表示一个对象，一个用来保存对象的状态，一个用于对象的操作（即接口）。
```cpp
function newAccount(initialBalance)
 local self = {balance = initialBalance}
 local withdraw = function(v)
   self.balance = self.balance -v
  end
 return {
   withdraw = withdraw
  }
end
```
通过闭包的方式，将具有私密性的字段（如balance）保存在self table中，并只公开了withdraw接口，这样就能实现私密性机制。
