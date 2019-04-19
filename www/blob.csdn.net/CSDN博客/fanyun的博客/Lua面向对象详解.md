# Lua面向对象详解 - fanyun的博客 - CSDN博客
2017年04月04日 19:28:19[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：262
所属专栏：[巧用脚本](https://blog.csdn.net/column/details/15130.html)
简单说说Lua中的面向对象，Lua中的table就是一种对象，看以下一段简单的代码：
复制代码代码如下:
```cpp
local tb1 = {a = 1, b = 2}
local tb2 = {a = 1, b = 2}
local tb3 = tb1
 
if tb1 == tb2 then
     print("tb1 == tb2")
else
     print("tb1 ~= tb2")
end
 
tb3.a = 3
print(tb1.a)
```
上述代码会输出tb1 ~= tb2。说明两个具有相同值得对象是两个不同的对象，同时在Lua中table是引用类型的。我在《[Lua中的模块与包](http://www.jb51.net/article/55818.htm)》中也总结了，我们是基于table来实现的模块，在table中可以定义函数，也就是说，每个table对象都可以拥有其自己的操作。看一段代码：
复制代码代码如下:
Account = {balance = 0}
function Account.withDraw(v)
     Account.balance = Account.balance - v
end
Account.withDraw(10) -- 调用函数
print(Account.balance)
上面的代码创建了一个新函数，并将该函数存入Account对象的withDraw字段中，然后我们就可以调用该函数了。不过，在函数中使用全局名称Account是一个不好的编程习惯，因为这个函数只能针对特定对象工作，并且，这个特定对象还必须存储在特定的全局变量中。如果改变了对象的名称，withDraw就再也不能工作了。例如以下代码：
复制代码代码如下:
a = Account
Account = nil
a.withDraw(100)
这样就会出现错误。我在这里使用Account创建了一个新的对象a，当将Account赋值为nil时，应该要对a对象不产生任何影响。但是，由于在函数withDraw内部使用了Account，而不是变量a，所以就出现了错误。如果我们将withDraw函数内部的Account.balance = Account.balance – v语句修改为：a.balance =a.balance – v，这样就不会出现错误了。这就表明，当我们需要对一个函数进行操作时，需要指定实际的操作对象，即这里的a，这就需要一个额外的参数来表示该操作者，就好比C++中的this一样，只不过这里将这个关键字换成了self，换完以后的代码如下：
复制代码代码如下:
Account = {balance = 0}
function Account.withDraw(self, v)
     self.balance = self.balance - v
end
a = Account
Account = nil
a.withDraw(a, 100)
print(a.balance)
这样再调用，就不会出现错误了。
使用self参数是所有面向对象语言的一个核心。大多数面向对象语言都对程序员隐藏了self参数，从而使得程序员不必显示地声明这个参数。Lua也可以，当我们在定义函数时，使用了冒号，则能隐藏该参数，那么上述代码使用冒号来改下，就是下面这个样子了。
复制代码代码如下:
Account = {balance = 0}
function Account:withDraw(v) -- 注意这里的冒号":"
     self.balance = self.balance - v
end
a = Account
Account = nil
a:withDraw(100) -- 注意这里的调用时，也需要冒号":"
print(a.balance)
冒号的作用很简单，就是在方法定义中添加一个额外的隐藏参数，以及在一个方法调用中添加一个额外的实参。冒号只是一种语法便利，并没有引入任何新的东西；如果你愿意，你可以可以不使用self，而是在每次定义一个函数时，手动的加上self，只要你处理好了self，它们都是一样的。
这里乱乱的讲了一些Lua中的东西，主要还是说了table是一个不一样的东西，还有self。接下来，就正式进入面向对象的世界。不要忘了，上面总结的东西是非常有用的。
类
类是什么？一个类就是一个创建对象的模具。例如C++中，每个对象都是某个特定类的实例。在C++中，如果一个类没有进行实例化，那这个类中对应的操作，基本就是一堆“没有用”的代码；而Lua则不一样，即使你不实例化一个“类”，你照样也可以使用“类”名直接调用它的方法（对于C++，请忽视静态的方法）；这说明Lua中的“类”的概念与C++这种高级语言中类的概念还是有差别的。在Lua中则没有类的概念，而我们都是通过Lua现有的支持，去模拟类的概念。在Lua中，要表示一个类，只需创建一个专用作其他对象的原型（prototype）。原型也是一种常规的对象，也就是说我们可以直接通过原型去调用对应的方法。当其它对象（类的实例）遇到一个未知操作时，原型会先查找它。
在Lua中实现原型是非常简单的，比如有两个对象a和b，要让b作为a的原型，只需要以下代码就可以完成：
复制代码代码如下:
setmetatable(a, {__index = b})  -- 又是元表，不会的请看前几篇关于元表的文章:http://www.jb51.net/article/55812.htm
设置了这段代码以后，a就会在b中查找所有它没有的操作。若将b称为是对象a的“类”，就仅仅是术语上的变化。现在我就从最简单的开始，要创建一个实例对象，必须要有一个原型，就是所谓的“类”，看以下代码：
复制代码代码如下:
local Account = {}  -- 一个原型
好了，现在有了原型，那如何使用这个原型创建一个“实例”呢？接着看以下代码：
复制代码代码如下:
function Account:new(o)  -- 这里是冒号哦
     o = o or {}  -- 如果用户没有提供table，则创建一个
     setmetatable(o, self)
     self.__index = self
     return o
end
当调用Account:new时，self就相当于Account。接着，我们就可以调用Account:new来创建一个实例了。再看：
复制代码代码如下:
local a = Account:new{value = 100} -- 这里使用原型Account创建了一个对象a
a:display()
上面这段代码是如何工作的呢？首先使用Account:new创建了一个新的实例对象，并将Account作为新的实例对象a的元表。再当我们调用a:display函数时，就相当于a.display(a)，冒号就只是一个“语法糖”，只是一种方便的写法。我们创建了一个实例对象a，当调用display时，就会查找a中是否有display字段，没有的话，就去搜索它的元表，所以，最终的调用情况如下：
复制代码代码如下:
getmetatable(a).__index(display(a))
a的元表是Account，Account的__index也是Account。因此，上面的调用也可以使这样的：
复制代码代码如下:
Account.display(a)
所以，其实我们可以看到的是，实例对象a表中并没有display方法，而是继承自Account方法的，但是传入display方法中的self确是a。这样就可以让Account（这个“类”）定义操作。除了方法，a还能从Account继承所有的字段。
继承不仅可以用于方法，还可以作用于字段。因此，一个类不仅可以提供方法，还可以为实例中的字段提供默认值。看以下代码：
复制代码代码如下:
local Account = {value = 0}
function Account:new(o)  -- 这里是冒号哦
     o = o or {}  -- 如果用户没有提供table，则创建一个
     setmetatable(o, self)
     self.__index = self
     return o
end
function Account:display()
     self.value = self.value + 100
     print(self.value)
end
local a = Account:new{} -- 这里使用原型Account创建了一个对象a
a:display() --(1)
a:display() --(2)
在Account表中有一个value字段，默认值为0；当我创建了实例对象a时，并没有提供value字段，在display函数中，由于a中没有value字段，就会查找元表Account，最终得到了Account中value的值，等号右边的self.value的值就来源自Account中的value。调用a:display()时，其实就调用以下代码：
复制代码代码如下:
a.display(a)
在display的定义中，就会变成这样子：
复制代码代码如下:
a.value = getmetatable(a).__index(value) + 100
第一次调用display时，等号左侧的self.value就是a.value，就相当于在a中添加了一个新的字段value；当第二次调用display函数时，由于a中已经有了value字段，所以就不会去Account中寻找value字段了。
继承
由于类也是对象（准确地说是一个原型），它们也可以从其它类（原型）获得（继承）方法。这种行为就是继承，可以很容易的在Lua中实现。现在我们有一个类（原型，其实在Lua中说类这个概念，还是很别扭的，毕竟用C++的脑袋去想，还是觉的有点奇怪的。）CA：
复制代码代码如下:
local CA = {value = 0}
function CA:new(o)
     o = o or {}
     setmetatable(o, self)
     self.__index = self
     return o
end
function CA:display()
     print(self.value)
end
function CA:addValue(v)
     self.value = self.value + v
end
现在需要从这个CA类派生出一个子类CLittleA，则需要创建一个空的类，从基类继承所有的操作：
复制代码代码如下:
local CLittleA = CA:new()
现在，我创建了一个CA类的一个实例对象，在Lua中，现在CLittleA既是CA类的一个实例对象，也是一个原型，就是所谓的类，就相当于CLittleA类继承自CA类。再如下面的代码:
复制代码代码如下:
local s = CLittleA:new{value1 = 10}
CLittleA从CA继承了new；不过，在执行CLittleA:new时，它的self参数表示为CLittleA，所以s的元表为CLittleA，CLittleA中字段__index的值也是CLittleA。然后，我们就会看到，s继承自CLittleA，而CLittleA又继承自CA。当执行s:display时，Lua在s中找不到display字段，就会查找CLittleA；如果仍然找不到display字段，就查找CA，最终会在CA中找到display字段。可以这样想一下，如果在CLittleA中存在了display字段，那么就不会去CA中再找了。所以，我们就可以在CLittleA中重定义display字段，从而实现特殊版本的display函数。
多重继承
说到多重继承，我在写C++代码的时候也用的很少，一般都是使用组合的方式解决的，对于“组合”这个概念不明白的朋友，可以阅读我的设计模式系列的文章。既然说到了Lua中的多重继承，那也总结一下，顺便开拓一下视野和知识面。
实现单继承时，依靠的是为子类设置metatable，设置其metatable为父类，并将父类的__index设置为其本身的技术实现的。而多继承也是一样的道理，在单继承中，如果子类中没有对应的字段，则只需要在一个父类中寻找这个不存在的字段；而在多重继承中，如果子类没有对应的字段，则需要在多个父类中寻找这个不存在的字段。
就像上图表示一样，Lua会在多个父类中逐个的搜索display字段。这样，我们就不能像单继承那样，直接指定__index为某个父类，而是应该指定__index为一个函数，在这个函数中指定搜索不存在的字段的规则。这样便可实现多重继承。这里就出现了两个需要去解决的问题：
1.保存所有的父类；
2.指定一个搜索函数来完成搜索任务。
对于以上的多重继承，我们来看一段头疼的代码：
复制代码代码如下:
-- 在多个父类中查找字段k
local function search(k, pParentList)
    for i = 1, #pParentList do
        local v = pParentList[i][k]
        if v then
            return v
        end
    end
end
function createClass(...)
    local c = {} -- 新类
    local parents = {...}
    -- 类在其元表中搜索方法
    setmetatable(c, {__index = function (t, k) return search(k,parents) end})
    -- 将c作为其实例的元表
    c.__index = c
    -- 为这个新类建立一个新的构造函数
    function c:new(o)
        o = o or {}
        setmetatable(o, self)
        -- self.__index = self 这里不用设置了，在上面已经设置了c.__index = c
        return o
    end
    -- 返回新的类（原型）
    return c
end
-- 一个简单的类CA
local CA = {}
function CA:new(o)
    o = o or {}
    setmetatable(o, {__index = self})
    self.__index = self
    return o
end
function CA:setName(strName)
    self.name = strName
end
-- 一个简单的类CB
local CB = {}
function CB:new(o)
    o = o or {}
    setmetatable(o, self)
    self.__index = self
    return o
end
function CB:getName()
    return self.name
end
-- 创建一个c类，它的父类是CA和CB
local c = createClass(CA, CB)
-- 使用c类创建一个实例对象
local objectC = c:new{name = "Jelly"}
-- 设置objectC对象一个新的名字
objectC:setName("JellyThink")
local newName = objectC:getName()
print(newName)
代码虽然头疼，但是还的继续看。首先大体阅读一下上面的代码，看不懂不要紧。现在我来解释上面的代码。
1.使用createClass创建了一个类（原型），将CA和CB设置为这个类（原型）的父类（原型）；在创建的这个类（原型）中，设置了该类的__index为一个search函数，在这个search函数中寻找在创建的类中没有的字段；
2.创建的新类中，有一个构造函数new；这个new和之前的单继承中的new区别不大，很好理解；
3.调用new构造函数，创建一个实例对象，该实例对象有一个name字段；
4.调用object:setName(“JellyThink”)语句，设置一个新的名字；但是在objectC中没有这个字段，怎么办？好了，去父类找，先去CA找，一下子就找到了，然后就调用了这个setName，setName中的self指向的是objectC；设置以后，就相当于修改了objectC字段的name值；
5.调用objectC:getName()，objectC还是没有这个字段。找吧，CA也没有，那就接着找，在CB中找到了，就调用getName，在getName中的self指向的是objectC。所以，在objectC:getName中返回了objectC中name的值，就是“JellyThink”。
还有什么？什么也没有了，对于多重继承，貌似看起来很难，很麻烦，其实也就这么点东西。不懂的话，再来一遍。
我拿什么保护你
我们都知道，在C++或Java中，对于类中的成员函数或变量都有访问权限的。public，protected和private这几个关键字还认识吧。那么在Lua中呢？Lua中是本身就是一门“简单”的脚本语言，本身就不是为了大型项目而生的，所以，它的语言特性中，本身就没有带有这些东西，那如果非要用这样的保护的东西，该怎么办？我们还是“曲线救国”。思想就是通过两个table来表示一个对象。一个table用来保存对象的私有数据；另一个用于对象的操作。对象的实际操作时通过第二个table来实现的。为了避免未授权的访问，保存对象的私有数据的表不保存在其它的table中，而只是保存在方法的closure中。看一段代码：
复制代码代码如下:
function newObject(defaultName)
     local self = {name = defaultName}
     local setName = function (v) self.name = v end
     local getName = function () return self.name end
     return {setName = setName, getName = getName}
end
local objectA = newObject("Jelly")
objectA.setName("JellyThink") -- 这里没有使用冒号访问
print(objectA.getName())
这种设计给予存储在self table中所有东西完全的私密性。当调用newObject返回以后，就无法直接访问这个table了。只能通过newObject中创建的函数来访问这个self table；也就相当于self table中保存的都是私有的，外部是无法直接访问的。大家可能也注意到了，我在访问函数时，并没有使用冒号，这个主要是因为，我可以直接访问的self table中的字段，所以是不需要多余的self字段的，也就不用冒号了。
