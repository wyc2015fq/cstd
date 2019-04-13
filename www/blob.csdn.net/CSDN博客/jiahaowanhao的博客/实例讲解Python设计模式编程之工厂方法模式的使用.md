
# 实例讲解Python设计模式编程之工厂方法模式的使用 - jiahaowanhao的博客 - CSDN博客


2018年07月26日 15:00:11[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：36标签：[Python																](https://so.csdn.net/so/search/s.do?q=Python&t=blog)


**实例讲解Python设计模式编程之工厂方法模式的使用**
工厂方法模式是简单工厂模式的进一步抽象和推广，它不仅保持了简单工厂模式能够向客户隐藏类的实例化过程这一优点，而且还通过多态性克服了工厂类过于复杂且不易于扩展的缺点。在工厂方法模式中，处于核心地位的工厂类不再负责所有产品的创建，而是将具体的创建工作交由子类去完成。工厂方法模式中的核心工厂类经过功能抽象之后，成为了一个抽象的工厂角色，仅负责给出具体工厂子类必须实现的接口，而不涉及哪种产品类应当被实例化这一细节。工厂方法模式的一般性结构如下图所示，图中为了简化只给出了一个产品类和一个工厂类，但在实际系统中通常需要设计多个产品类和多个工厂类。
工厂方法模式的实质是将对象的创建延迟到其子类实现，即由子类根据当前情况动态决定应该实例化哪一个产品类。从上图可以看出，工厂方法模式涉及到抽象工厂角色、具体工厂角色、抽象产品角色和具体产品角色四个参与者。
抽象工厂（Creator）角色　　是工厂方法模式的核心，它负责定义创建抽象产品对象的工厂方法。抽象工厂不能被外界直接调用，但任何在模式中用于创建产品对象的工厂类都必须实现由它所定义的工厂方法。
具体工厂（Concrete Creator）角色　　是工厂方法模式的对外接口，它负责实现创建具体产品对象的内部逻辑。具体工厂与应用密切相关，可以被外界直接调用，创建所需要的产品。
抽象产品（Product）角色　　是工厂方法模式所创建的所有对象的父类，它负责描述所有具体产品共有的公共接口。
具体产品（Concrete Product）角色　　是工厂方法模式的创建目标，所有创建的对象都是充当这一角色的某个具体类的实例。
抽象工厂角色负责声明工厂方法（factory method），用来"生产"抽象产品，以下是抽象工厂的示例性Python代码：
creator.py
class Creator:
""" 抽象工厂角色 """
\# 创建抽象产品的工厂方法
def factoryMethod(self):
pass
具体工厂角色负责创建一个具体产品的实例，并将其返回给调用者。具体工厂是与具体产品相关的，实现时一般常用的做法是为每个具体产品定义一个具体工厂。以下是具体工厂的示例性Python代码：
concretecreator.py
class ConcreteCreator(Creator):
""" 具体工厂角色 """
\# 创建具体产品的工厂方法
def factoryMethod(self):
product = ConcreteProduct()
return product
抽象产品角色的主要目的是为所有的具体产品提供一个共同的接口，通常只需给出相应的声明就可以了，而不用给出具体的实现。以下是抽象产品类的示例性Python代码：
product.py
class Product:
""" 抽象产品角色 """
\# 所有产品类的公共接口
def interface(self):
pass
具体产品角色充当最终的创建目标，一般来讲它是抽象产品类的子类，实现了抽象产品类中定义的所有工厂方法，实际应用时通常会具有比较复杂的业务逻辑。以下是具体产品类的示例性Python代码：
concreteproduct.py
class ConcreteProduct(Product):
""" 具体产品角色 """
\# 公共接口的实现
def interface(self):
print "Concrete Product Method"
在应用工厂方法模式时，通常还需要再引入一个客户端角色，由它负责创建具体的工厂对象，然后再调用工厂对象中的工厂方法来创建相应的产品对象。以下是客户端的示例性Python代码：
client.py
class Client:
""" 客户端角色 """
def run(self):
creator = ConcreteCreator()
product = creator.factoryMethod()
product.interface()
\# 主函数
if (__name__ == "__main__"):
client = Client()
client.run()
在这个简单的示意性实现里，充当具体产品和具体工厂角色的类都只有一个，但在真正的实际应用中，通常遇到的都是同时会有多个具体产品类的情况，此时相应地需要提供多个具体工厂类，每个具体工厂都负责生产对应的具体产品。
工厂方法模式的活动序列如下图所示，客户端Client首先创建ConcreteCreator对象，然后调用ConcreteCreator对象的工厂方法factoryMethod()，由它负责"生产"出所需要的ConcreteProduct对象。
![201632101551418.png (492×296)](https://files.jb51.net/file_images/article/201603/201632101551418.png?20162210161)
下面我们来看一个具体案例：
如果你开一家Pizza店（PizzaStore抽象类）卖各种风味的Pizza（Pizza子类），那么你需要根据客户要求准备相应的Pizza（创建Pizza对象），然后烘烤、切片、包装；
最简单的做法就是在PizzaStore中根据客户要求（类型判断）创建相应的Pizza对象，然后调用Pizza自身（由Pizza抽象类实现）的烘烤、切片和包装方法；
但这样的代码缺乏弹性，因为你让一个抽象类去依赖具体的对象；我们可以创建一个工厂来生产Pizza，根据传入的不同类型值返回不同Pizza对象，即从PizzaStore中将创建对象的代码挪到工厂中。但这只是一个编程技巧，并不算模式。
在工厂方法模式中，我们在PizzaStore中定义一个抽象接口（create_pizza）作为抽象的工厂，而order_pizza是它的客户；将Pizza对象的创建放到PizzaStore子类去解决。
现有Cheese和Clam两款Pizza，以及NY和Chicago两家分店，每家店的同款Pizza的口味不同——为迎合当地口味做了改进，主要差别来自不同的原材料，因此我们实现四个Pizza类型（NYStyleCheesePizza、NYStyleClamPizza、ChicagoStyleCheesePizza和ChicagoStyleClamPizza），每种使用不同的原材料组合，根据客户所在城市和选择款式我们创建不同的对象；根据工厂方法，我们将对象创建的代码放到PizzaStore子类去实现。
代码：
\#!/usr/bin/python
class Pizza:
name = ""
dough = ""
sauce = ""
toppings = []
def prepare(self):
print "Preparing %s" % self.name
print " dough: %s" % self.dough
print " sauce: %s" % self.sauce
print " add toppings:"
for n in self.toppings:
print "  %s" % n
def bake(self):
print "Bake for 25 minutes at 350."
def cut(self):
print "Cutting into diagonal slices."
def box(self):
print "Put into official box."
def get_name(self):
return self.name

class PizzaStore:
def order_pizza(self, pizza_type):
self.pizza = self.create_pizza(pizza_type)
self.pizza.prepare()
self.pizza.bake()
self.pizza.cut()
self.pizza.box()
return self.pizza
def create_pizza(self, pizza_type):
pass

class NYStyleCheesePizza(Pizza):
def __init__(self):
self.name = "NY Style Cheese Pizza"
self.dough = "NY Dough"
self.sauce = "NY Sauce"
self.toppings.append("NY toopping A")
self.toppings.append("NY toopping B")

class ChicagoStyleCheesePizza(Pizza):
def __init__(self):
self.name = "Chicago Style Cheese Pizza"
self.dough = "Chicago Dough"
self.sauce = "Chicago Sauce"
sefl.toppings.append("Chicago toopping A")
def cut(self):
print "Cutting into square slices."

class NYStyleClamPizza(Pizza):
def __init__(self):
self.name = "NY Style Clam Pizza"
self.dough = "NY Dough"
self.sauce = "NY Sauce"
self.toppings.append("NY toopping A")
self.toppings.append("NY toopping B")

class ChicagoStyleClamPizza(Pizza):
def __init__(self):
self.name = "Chicago Style Clam Pizza"
self.dough = "Chicago Dough"
self.sauce = "Chicago Sauce"
self.toppings.append("Chicago toopping A")
def cut(self):
print "Cutting into square slices."

class NYPizzaStore(PizzaStore):
def create_pizza(self, pizza_type):
if pizza_type == "cheese":
return NYStyleCheesePizza()
elif pizza_type == "clam":
return NYStyleClamPizza()
else:
return None

class ChicagoPizzaStore(PizzaStore):
def create_pizza(self, pizza_type):
if pizza_type == "cheese":
return ChicagoStyleCheesePizza()
elif pizza_type == "clam":
return ChicagoStyleClamPizza()
else:
return None
if __name__ == "__main__":
ny_store = NYPizzaStore()
chicago_store = ChicagoPizzaStore()
pizza = ny_store.order_pizza("cheese")
print "Mike ordered a %s." % pizza.get_name()
print
pizza = chicago_store.order_pizza("clam")
print "John ordered a %s." % pizza.get_name()
print
输出：
Preparing NY Style Cheese Pizza
dough: NY Dough
sauce: NY Sauce
add toppings:
NY toopping A
NY toopping B
Bake for 25 minutes at 350.
Cutting into diagonal slices.
Put into official box.
Mike ordered a NY Style Cheese Pizza.
Preparing Chicago Style Clam Pizza
dough: Chicago Dough
sauce: Chicago Sauce
add toppings:
NY toopping A
NY toopping B
Chicago toopping A
Bake for 25 minutes at 350.
Cutting into square slices.
Put into official box.
John ordered a Chicago Style Clam Pizza.

