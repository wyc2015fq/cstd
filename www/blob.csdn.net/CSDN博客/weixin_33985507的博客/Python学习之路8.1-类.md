# Python学习之路8.1-类 - weixin_33985507的博客 - CSDN博客
2018年05月30日 18:43:07[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
> 《Python编程：从入门到实践》笔记。
本章主要介绍一种重要的编程思想：面向对象编程，包括了类与对象等概念及操作。
# 1. 概述
**面向对象编程(Object-oriented programming, OOP)**是最有效的软件编写方法之一。面向对象的思想也是人类自古认识世界的方法，即“分门别类”。而在以往的经验里，笔者印象最深刻的面向对象思想就是中学生物课本上对自然界的分类：界门纲目科属种。这里要明白两个概念：类与对象。类是一个总的抽象概念，是一群相似事物的总括，是一个虚的概念，而这些“事物”便是对象，例如：“狗”这一概念，这就是一个“类”，哪怕是具体到某一个特定的种类，比如哈士奇，这也是个类，只有当真正具体到某一条狗时，比如“你家的哈士奇A”，这才到达了“对象”这一概念，综上：**类是抽象的，对象是实际的**。而从类到对象的过程，就叫做**类的实例化**。
# 2. 创建和使用类
## 2.1 创建一个Car类
在Python中类名一般采用驼峰命名法，即每个单词的首字母大写，而不使用下划线，实例名和模块名都采用小写，用下划线拼接。并且，不论是在写函数，类，还是代码文件，最好都加上一个文档字符串，比如下面的三引号字符串。
```python
class Car:
    """一次模拟汽车的简单尝试"""
    def __init__(self, make, model, year):
        """初始化描述汽车的属性"""
        self.make = make
        self.model = model
        self.year = year
        self.odometer_reading = 0  # 里程表
    def get_descriptive_name(self):
        """返回整洁的描述性信息"""
        long_name = str(self.year) + " " + self.make + " " + self.model
        return long_name.title()
    def read_odometer(self):
        """打印一条指出汽车历程的消息"""
        print("This car has " + str(self.odometer_reading) + " miles on it.")
    def update_odometer(self, mileage):
        """将里程表读书设置为指定的值，且禁止读数回调"""
        if mileage <= 0:
            print("Mileage must be bigger than 0!")
        elif mileage >= self.odometer_reading:
            self.odometer_reading = mileage
        else:
            print("You can't roll back an odometer!")
    def increment_odometer(self, miles):
        """将里程表读数增加指定的量，且该量必须为正数"""
        if miles > 0:
            self.odometer_reading += miles
        else:
            print("Mile must be bigger than 0!")
    def fill_gas_tank(self):
        """将油箱装满"""
        print("The gas tank has been filled!")
```
以下有几点需要注意：
①类中的函数称为方法，比如上述定义的三个函数；类中与`self`相绑定的变量称为属性，比如`make`，`model`，`year`（不是指那三个**形参**，而是与`self`绑定的**变量**）。
②每一个类必有一个`__init()__`方法，这个方法被称为构造方法（在C++中被称为构造函数，不过不用太纠结到底是“方法”还是“函数”，一个东西放在了不同地方有了不同的名字而已）。当然它也有默认的版本，即只有一个`self`参数，并且该函数什么也不做，这也表明，你甚至都不用定义这个方法，到时候Python会自动生成并调用默认构造方法，不过“不定义构造方法”这种情况估计也就只有像笔者这样初学的时候才能遇到 ^_^。
③Python中`self`参数是类中每个非静态方法必须要有的形参，且必须放在第一个，它是一个指向实例本身（不是类本身！）的一个引用，让实例能够访问类中的属性和方法，我们在调用类的方法时不用手动传入该参数，它会自动被传入。类中的属性在类中所有的方法里都能被访问，这便是通过`self`参数实现的。如果站在C++的角度理解，`self`就相当于C++类里的`this`指针，指向对象自身。
④类中的每个属性都必须有**初始值**，哪怕这个值是0，空字符串或者`None`。比如本例中的四个属性，前三个属性的值由用户传入，`odometer_reading`的值被设为了0。
⑤在上述代码的第一行类名`Car`后面可带可不带小括号，即`class Car:`这种写法可行，`class Car():`这种写法也可以。
## 2.2 使用该Car类
以下代码创建了一个Car类的对象，并对该对象进行了简单的操作。
```python
# 代码：
class Car:
    -- snip --     # 这不是一个Python语法！这里只是表示省略。
my_new_car = Car("audi", "a4", 2016)
print(my_new_car.get_descriptive_name())
my_new_car.read_odometer()
# 直接修改属性
my_new_car.odometer_reading = -100
my_new_car.read_odometer()
my_new_car.odometer_reading += -1
my_new_car.read_odometer()
# 通过方法修改属性
my_new_car.update_odometer(-100)
my_new_car.read_odometer()
my_new_car.increment_odometer(-1)
my_new_car.read_odometer()
my_new_car.update_odometer(100)
my_new_car.read_odometer()
my_new_car.increment_odometer(1)
my_new_car.read_odometer()
# 结果：
2016 Audi A4
This car has 0 miles on it.
This car has -100 miles on it.
This car has -101 miles on it.
Mileage must be bigger than 0!
This car has -101 miles on it.
Mile must be bigger than 0!
This car has -101 miles on it.
This car has 100 miles on it.
This car has 101 miles on it.
```
从上述代码可以看出，Python和C++，Java一样，也是使用句点表示法来访问属性以及调用方法。从上述代码及结果可以看出，实例的属性可以直接也可以通过方法进行访问和修改。
直接访问对象的属性可以使操作变得简单，但这违反了封闭性原则，并且直接修改属性也不利于规范对属性的操作。比如代码中将里程设置为一个负值，且在增加里程时增量也是一个负值，这显然不符合常理（虽然有时也可以这么做）。而如果将对属性的操作放入方法中，则可以规范这些操作，如上述的`read_odometer()`，`update_odometer()`，`increment_odometer()`等方法。并且这也是面向对象编程所提倡的做法，尽量不要将属性直接对外暴露。但可惜的是，Python中任何种类的属性都能被直接操作。
# 3. 继承
编写类时并非总是从零开始，如果要编写的类是现有类的特殊版本，即有相同或相似的属性和方法，则可以从现有类继承（派生）出新的类。被继承的类称为**“父类”**、**“基类”**或**“超类(superclass)”**，新的类称为**“子类“**或**”派生类“**。
但要注意的是，继承关系应只发生在有较强相互关系的类之间，比如从车类派生出电动车类，没有从车类派生出哈士奇这种骚操作。
以下是从`Car`类派生出`ElectricCar`类的代码：
```python
# 代码：
class Car:
    -- snip --
    
class ElectricCar(Car):
    """电动汽车的独特之处"""
    def __init__(self, make, model, year):
        """初始化父类的属性，再初始化电动汽车特有的属性"""
        super().__init__(make, model, year)
        self.battery_size = 70
    def describe_battery(self):
        """打印一条描述电池容量的消息"""
        print("This car has a " + str(self.battery_size) + "-kWh battery.")
    def fill_gas_tank(self):   # 重写了父类的方法
        """电动车没有油箱"""
        print("This car doesn't need a gas tank!")
my_audi = Car("audi", "a4", 2018)
print(my_audi.get_descriptive_name())
my_audi.fill_gas_tank()
print()     # 用作空行
my_tesla = ElectricCar("tesla", "model s", 2018)
print(my_tesla.get_descriptive_name())
my_tesla.describe_battery()
my_tesla.fill_gas_tank()
# 结果：
2018 Audi A4
The gas tank has been filled!
2018 Tesla Model S
This car has a 70-kWh battery.
This car doesn't need a gas tank!
```
从以上代码可以总结出几点：
①创建子类的实例时，Python首先需要对父类进行初始化操作，通过`super()`函数返回父类的引用，然后再调用父类的构造方法，即`super().__init__(参数列表)`。在Python2中，对父类的初始化需要以如下方式初始化父类：
```python
super(ElectricCar, self).__init__(make, model, year)
```
在Python3中也可以按上述方式来初始化父类，但也可以在**单继承**时省略`super()`函数中的参数。
②子类可以访问父类的所有属性，还可以增加新的属性：`my_tesla`对象访问了父类的`make`, `model`, `year`等属性，并且还增加了`battery_size`属性。
③子类可以重写父类的方法：`ElectricCar`类重写了`Car`类的`fill_gas_tank()`方法。
这里需要区分两个概念：**重写(Override)**与**重载(Overload)**
重写也叫覆盖，主要是用在继承上。当继承关系上的类中有相同的方法，但子类和父类在该方法中的操作不相同时，子类对该方法进行重新编写，覆盖掉从父类继承下来的方法。在调用时，Python会自动判断该对象是否是派生类来调用该方法相应的实现。正是有了重写，面向对象中**多态(Polymorphism)**这一特性才得以实现。
重载主要用于函数(方法)。在像C/C++，Java这样的语言中，可以有多个同名的函数，但参数列表必须不相同，比如参数个数，参数类型不相同。这些语言则根据参数列表来区分到底调用的是同名函数中的哪一个函数。但**重载并不属于多态性！**这些语言在编译源文件的时候，会根据参数列表来对同名函数生成不同的函数名(具体方法就是添加前缀或后缀)，然后将源代码中的这些同名函数都替换成新函数名，所以重载并不属于多态。但是**Python中并没有函数重载这种说法！**因为Python有关键字参数和可变参数这种神器（当然C++也有变长参数，它用三个点表示，不知道Python可变参数的底层实现是不是就和C++的变长参数有关）。
然而这都不重要！明白重写和重载的概念，会用就行了，至于这俩和多态究竟有没有关系并不重要，至今网上对这俩与多态的关系都没有一个准确的说法。笔者以前看C++的书的时候记得专门把重载的底层实现给提了出来（哪本书忘了），但笔者才疏学浅，暂不清楚重写在编译时是个什么情况，说不定也是靠生成新函数名并替换，如果这样的话，那重载也可以算多态了，**不过这只是笔者的猜测！**感兴趣的小伙伴可自行研究这俩在编译时的情况。
之所以把这俩单独提出来，主要是好多人在考研复试或者找工作面试的时候载到了这个概念上。尤其是考研，考研复试似乎更倾向于重写属于多态，重载不属于多态。
## 3.1 将实例用作属性
使用代码模拟实物时，随着开发的进展，势必一个类的属性和方法将会越来越多，单单一个类的代码就会越来越长。这时可以考虑是否能将其中一部分代码单独提取出来作为一个新的类。比如前面的`ElectricCar`类里的电池就可以单独提出来作为一个类。
```python
# 代码：
class Car:
    -- snip --
class Battery:
    """一次模拟电动汽车电池的简单尝试"""
    def __init__(self, battery_size=70):
        """初始化电池的属性"""
        self.battery_size = battery_size
    def describe_battery(self):
        """打印一条描述电池容量的信息"""
        print("This car has a " + str(self.battery_size) + "-kWh battery.")
    def get_range(self):
        """输出电池的续航里程"""
        if self.battery_size == 70:
            miles = 240
        elif self.battery_size == 85:
            miles = 270
        message = "This car can go approximately " + str(miles) + " miles on a full charge."
        print(message)
class ElectricCar(Car):
    def __init__(self, make, model, year):
        super().__init__(make, model, year)
        self.battery = Battery()
my_tesla = ElectricCar("tesla", "model s", 2018)
print(my_tesla.get_descriptive_name())
my_tesla.battery.describe_battery()
my_tesla.battery.get_range()
# 结果：
2018 Tesla Model S
This car has a 70-kWh battery.
This car can go approximately 240 miles on a full charge.
```
模拟复杂的实物时，需要解决一些有趣的问题，比如续航里程是电池的属性还是汽车的属性呢？如果只描述一辆车，那将`get_range()`方法放入`Battery()`中并无不妥，但如果要描述整个汽车产品线呢？比如这一款车型能跑多远，那也许将该方法放入`ElectricCar`类则比较合适。但不管怎样，这里强调的是应该站在一个更高的逻辑层面考虑问题。
# 4. 从模块导入类
与上一篇写关于函数的文章相似，类也可以单独形成模块。可以一个类就是一个模块，也可以多个类（一般是相关联的类）放入一个模块。比如将上述的`Car`类单独放在一个文件中，除去此类的代码，其他代码均删除，最后将该文件命名为`car.py`（注意这里的文件名是小写的）。然后再在程序中带入该类：
```python
from car import Car
# 如果命名有冲突，也可以给Car类起个别名
# from car import Car as C
my_new_car = Car("audi", "a4", 2018)
print(my_new_car.get_descriptive_name())
my_new_car.odometer_reading = 23
my_new_car.read_odometer()
```
也可以将多个相关联的类放入同一个文件中，形成一个模块，比如上面的`Car`类，`ElectricCar`类和`Battery`类，将该文件命名为`cars.py`，最后导入该文件：
```python
from cars import Car, ElectricCar
my_beetle = Car("volkswagen", "beetle", 2018)
my_tesla = ElectricCar("tesla", "model s", 2018)
-- snip --     # 后面的代码和之前的类似，不在赘述
```
也可以将整个模块导入，并使用句点表示法使用模块中的类：
```python
import cars
my_car = car.Car("volkswagen", "beetle", 2018)
my_tesla = car.ElectricCar("tesla", "model s", 2018)
```
还可以导入模块中的所有类（不推荐此法，容易产生命名冲突！），此时便不需要使用句点表示法。
```python
from cars import *
my_beetle = Car("volkswagen", "beetle", 2018)
```
还可以在模块中导入另一个模块，比如，将`Car`类单独放在一个文件中形参一个模块，命名为`car.py`，再新建一个模块`electric_car.py`用于存放`Battery`类和`ElectricCar`类，并在该模块中带入`Car`类：
```python
from car import Car
class Battery:
    -- snip --
class ElectricCar(Car):
    -- snip --
```
最后在执行文件的源代码中根据需要导入类：
```python
# 这是书中导入两个类的代码
from car import Car
from electric_car import ElectricCar     
my_car = Car("audi", "a4", 2018)
my_tesla = ElectricCar("tesla", "model s", 2018)
```
之前读到这的时候觉得能不能像以下这样的方式导入`Car`类：
```python
from electric_car import Car, ElectricCar
my_car = Car("audi", "a4", 2018)
my_tesla = ElectricCar("tesla", "model s", 2018)
```
后来亲测，这样做也是可以的。那问题就来了，像书中那样的导入方式是不是发生了代码的覆盖呢？哪种导入的效率更高呢？笔者在这里还有点懵，后续再更新吧。
模块导入的方法还有很多，甚至能直接从GitHub导入模块，上述的导入方式只是皮毛。最后用一个从标准库导入`OrderedDict`类的示例结束本文。之前版本的Python中普通字典类是不确保键值对之前的顺序的，想要确保顺序就得使用`OrderedDict`类。但现在从3.6版本起，Python也确保了普通字典里键值对也是有序的了，但是为了兼容性考虑（有可能你的代码还要运行在3.6之前的版本），目前还是建议使用`OrderedDict`类。
```python
# 代码：
from collections import OrderedDict
favorite_languages = OrderedDict()
favorite_languages["jen"] = "python"
favorite_languages["sarah"] = "c"
favorite_languages["edward"] = "ruby"
favorite_languages["phil"] = "python"
for name, language in favorite_languages.items():
    print(name.title() + "'s favorite_language is " + language.title())
# 结果：
Jen's favorite_language is Python
Sarah's favorite_language is C
Edward's favorite_language is Ruby
Phil's favorite_language is Python
```
> 迎大家关注我的微信公众号"代码港" & 个人网站 [www.vpointer.net](http://www.vpointer.net/) ~
![](https://image-static.segmentfault.com/382/764/3827649824-5b2205a10b390_articlex)
