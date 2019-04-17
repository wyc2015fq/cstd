# Python中的工厂函数 - 小灰笔记 - CSDN博客





2017年07月04日 01:19:06[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：4092








       参考了很多代码、别人的文章以及书籍，最后自己做了适合自己理解的一种理解总结方式。

       很多人说，工厂函数看上去像函数，其实是类。我觉得这种方法有些牵强，虽说在OOP中Python有其自己对类以及对象的定义。在一定程度上这么说是可以的。

       我个人的理解，既然叫做工厂函数，那么这一定是一个函数。不过，看了许多例子后发现，其实工厂函数基本上都是带有返回对象的。而这些返回对象则是通过调用一种或者多种类来创建的对象。

       例如，有如下代码：

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



defcard(rank,suit):

       if rank == 1: 

              return AceCard('A',suit)

       elif 2 <= rank < 11:

              return NumberCard(str(rank),suit)

       elif 11 <= rank < 14:

              name ={11:'J',12:'Q',13:'K'}[rank]

              return FaceCard(name,suit)

       else:

              raise Exception("Rank out ofrange")

       上面代码中的card其实就是通过调用各种类来创建对象，而这个函数就是一个工厂函数。这种函数的设计，通常是为了满足批量创建大量的对象或实例而创建。比如，创建所有的扑克牌对象可以通过如下代码：

Club,Diamond,Heart,Spade=Suit('Club','CCC'),Suit('Diamond','DDD'),Suit('Heart','HHH'),Suit('Spade','SSS')



deck =[card(rank,suit)

       for rank in range(1,14)

              for suit in(Club,Diamond,Heart,Spade)]

       上面的代码，首先创建了不同的花色，然后通过复合for语句创建了整套扑克牌。如果简化一些，自然也可以把花色存储到一个专门的元组或者列表中增强一下可读性。

       以上是我自己通过不同的资料、文档、书籍以及代码示例总结出的Python中工厂函数的理解以及使用方式。应该没有太大偏差了吧？如果有的话，后续继续纠正吧，毕竟学习本身应该是一条曲折的路线。



