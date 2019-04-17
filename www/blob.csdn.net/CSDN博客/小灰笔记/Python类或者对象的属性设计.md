# Python类或者对象的属性设计 - 小灰笔记 - CSDN博客





2017年07月04日 00:54:17[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：178








       我个人是个相对纯粹一些的C程序员，而且是一个嵌入式控制器软件设计的C程序员。平时接触到的面向对象的东西比较少，即使是类似的思想也很少。偶尔能够用到一点复杂的结构体，基本用到的属性的概念基本上也都是基础的数据类型。这让我自己在接触Python面向对象编程的知识时有了一些思维上的定式。

       其实，在Python面向对象的程序设计中，类或者对象的属性可以非常灵活。常用的方式不仅仅是简单的基础数据类型对象，也有可能是复杂类的对象。

       简单举例，有如下代码：

#!/usr/bin/python



class Card:

       def __init__(self,rank,suit):

              self.suit = suit

              self.rank = rank

              self.hard,self.soft =self._points()



classNumberCard(Card):

       def _points(self):

              returnint(self.rank),int(self.rank)



classAceCard(Card):

       def _points(self):

              return 1,11



classFaceCard(Card):

       def _points(self):

              return 10,10



class Suit:

       def __init__(self,name,symbol):

              self.name = name

              self.symbol = symbol

       在上面的类中，rank以及suit两个属性其实都没有什么严格的格式或者类型的要求。因此，在实现上可能会出现多种方式创建对象都可用的现象。接下来，针对之前自己不常用的一种方式做一个简单的示范：

Club,Diamond,Heart,Spade= Suit('Club','CCC'),Suit('Diamond','DDD'),Suit('Heart','HHH'),Suit('Spade','SSS')



card1 =NumberCard('3',Club)

card2 =AceCard('A',Spade)

       上面的代码中，首先创建了四种花色的对象。然后，任意挑选了两个花色创建了两张不同的牌。当然，创建更多的牌只需要选择合适的组合方式，更多的例子不再进行示范。

       其实，之前自己惯性思维中OOP中属性只能够用基础数据类型的错误观念让我一直没有领悟到OOP的方便之处。从这里，慢慢地发现，其实OOP的思想以及设计技巧比我之前想象中的要强大的多。



