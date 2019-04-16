# 转载和积累系列 - Lua 有趣的面向对象实现 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2014年01月14日 14:34:50[initphp](https://me.csdn.net/initphp)阅读数：951








                
Lua中的table就是一种对象，但是如果直接使用仍然会存在大量的问题，见如下代码：

```
1 Account = {balance = 0}
2 function Account.withdraw(v)
3     Account.balance = Account.balance - v
4 end
5 --下面是测试调用函数
6 Account.withdraw(100.00)
```

    在上面的withdraw函数内部依赖了全局变量Account，一旦该变量发生改变，将会导致withdraw不再能正常的工作，如：

```
1 a = Account; Account = nil
2 a.withdraw(100.00)  --将会导致访问空nil的错误。
```

    这种行为明显的违反了面向对象封装性和实例独立性。要解决这一问题，我们需要给withdraw函数在添加一个参数self，他等价于Java/C++中的this，见如下修改：

```
1 function Account.withdraw(self,v)
2     self.balance = self.balance - v
3 end
4 --下面是基于修改后代码的调用：
5 a1 = Account; Account = nil
6 a1.withdraw(a1,100.00)  --正常工作。
```

    针对上述问题，Lua提供了一种更为便利的语法，即将点**(.)**替换为冒号**(:)**，这样可以在定义和调用时均隐藏self参数，如:

```
1 function Account:withdraw(v)
2     self.balance = self.balance - v
3 end
4 --调用代码可改为：
5 a:withdraw(100.00)
```


1. 类：

    Lua在语言上并没有提供面向对象的支持，因此想实现该功能，我们只能通过table来模拟，见如下代码及关键性注释：

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

 1 --[[
 2 在这段代码中，我们可以将Account视为class的声明，如Java中的：
 3 public class Account 
 4 {
 5     public float balance = 0;
 6     public Account(Account o);
 7     public void deposite(float f);
 8 }
 9 --]]
10 --这里balance是一个公有的成员变量。
11 Account = {balance = 0}
12 
13 --new可以视为构造函数
14 function Account:new(o)
15     o = o or {} --如果参数中没有提供table，则创建一个空的。
16     --将新对象实例的metatable指向Account表(类)，这样就可以将其视为模板了。
17     setmetatable(o,self)
18     --在将Account的__index字段指向自己，以便新对象在访问Account的函数和字段时，可被直接重定向。
19     self.__index = self
20     --最后返回构造后的对象实例
21     return o
22 end
23 
24 --deposite被视为Account类的公有成员函数
25 function Account:deposit(v)
26     --这里的self表示对象实例本身
27     self.balance = self.balance + v
28 end
29 
30 --下面的代码创建两个Account的对象实例
31 
32 --通过Account的new方法构造基于该类的示例对象。
33 a = Account:new()
34 --[[
35 这里需要具体解释一下，此时由于table a中并没有deposite字段，因此需要重定向到Account，
36 同时调用Account的deposite方法。在Account.deposite方法中，由于self(a对象)并没有balance
37 字段，因此在执行self.balance + v时，也需要重定向访问Account中的balance字段，其缺省值为0。
38 在得到计算结果后，再将该结果直接赋值给a.balance。此后a对象就拥有了自己的balance字段和值。
39 下次再调用该方法，balance字段的值将完全来自于a对象，而无需在重定向到Account了。
40 --]]
41 a:deposit(100.00)
42 print(a.balance) --输出100
43 
44 b = Account:new()
45 b:deposit(200.00)
46 print(b.balance) --输出200

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```


    2. 继承：

    继承也是面向对象中一个非常重要的概念，在Lua中我们仍然可以像模拟类那样来进一步实现面向对象中的继承机制，见如下代码及关键性注释：

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

 1 --需要说明的是，这段代码仅提供和继承相关的注释，和类相关的注释在上面的代码中已经给出。
 2 Account = {balance = 0}
 3 
 4 function Account:new(o)
 5     o = o or {}
 6     setmetatable(o,self)
 7     self.__index = self
 8     return o
 9 end
10 
11 function Account:deposit(v)
12     self.balance = self.balance + v
13 end
14 
15 function Account:withdraw(v)
16     if v > self.balance then
17         error("Insufficient funds")
18     end
19     self.balance = self.balance - v
20 end
21 
22 --下面将派生出一个Account的子类，以使客户可以实现透支的功能。
23 SpecialAccount = Account:new()  --此时SpecialAccount仍然为Account的一个对象实例
24 
25 --派生类SpecialAccount扩展出的方法。
26 --下面这些SpecialAccount中的方法代码(getLimit/withdraw)，一定要位于SpecialAccount被Account构造之后。
27 function SpecialAccount:getLimit()
28     --此时的self将为对象实例。
29     return self.limit or 0
30 end
31 
32 --SpecialAccount将为Account的子类，下面的方法withdraw可以视为SpecialAccount
33 --重写的Account中的withdraw方法，以实现自定义的功能。
34 function SpecialAccount:withdraw(v)
35     --此时的self将为对象实例。
36     if v - self.balance >= self:getLimit() then
37         error("Insufficient funds")
38     end
39     self.balance = self.balance - v
40 end
41 
42 --在执行下面的new方法时，table s的元表已经是SpecialAccount了，而不再是Account。
43 s = SpecialAccount:new{limit = 1000.00}
44 --在调用下面的deposit方法时，由于table s和SpecialAccount均未提供该方法，因此访问的仍然是
45 --Account的deposit方法。
46 s:deposit(100)
47 
48 
49 --此时的withdraw方法将不再是Account中的withdraw方法，而是SpecialAccount中的该方法。
50 --这是因为Lua先在SpecialAccount(即s的元表)中找到了该方法。
51 s:withdraw(200.00)
52 print(s.balance) --输出-100

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```


    3. 私密性：

    私密性对于面向对象语言来说是不可或缺的，否则将直接破坏对象的封装性。Lua作为一种面向过程的脚本语言，更是没有提供这样的功能，然而和模拟支持类与继承一样，我们仍然可以在Lua中通过特殊的编程技巧来实现它，这里我们应用的是Lua中的闭包函数。该实现方式和前面两个示例中基于元表的方式有着很大的区别，见如下代码示例和关键性注释：

```
![复制代码](http://common.cnblogs.com/images/copycode.gif)

 1 --这里我们需要一个闭包函数作为类的创建工厂
 2 function newAccount(initialBalance)
 3     --这里的self仅仅是一个普通的局部变量，其含义完全不同于前面示例中的self。
 4     --这里之所以使用self作为局部变量名，也是为了方便今后的移植。比如，以后
 5     --如果改为上面的实现方式，这里应用了self就可以降低修改的工作量了。
 6     local self = {balance = initialBalance} --这里我们可以将self视为私有成员变量
 7     local withdraw = function(v) self.balance = self.balance - v end
 8     local deposit = function(v) self.balance = self.balance + v end
 9     local getBalance = function() return self.balance end
10     --返回对象中包含的字段仅仅为公有方法。事实上，我们通过该种方式，不仅可以实现
11     --成员变量的私有性，也可以实现方法的私有性，如：
12     --local privateFunction = function() --do something end
13     --只要我们不在输出对象中包含该方法的字段即可。
14     return {withdraw = withdraw, deposit = deposit, getBalance = getBalance}
15 end
16 
17 --和前面两个示例不同的是，在调用对象方法时，不再需要self变量，因此我们可以直接使用点(.)，
18 --而不再需要使用冒号(:)操作符了。
19 accl = newAccount(100.00)
20 --在函数newAccount返回之后，该函数内的“非局部变量”表self就不再能被外部访问了，只能通过
21 --该函数返回的对象的方法来操作它们。
22 accl.withdraw(40.00)
23 print(acc1.getBalance())

![复制代码](http://common.cnblogs.com/images/copycode.gif)
```

    事实上，上面的代码只是给出一个简单的示例，在实际应用中，我们可以将更多的私有变量存放于上例的局部self表中。




