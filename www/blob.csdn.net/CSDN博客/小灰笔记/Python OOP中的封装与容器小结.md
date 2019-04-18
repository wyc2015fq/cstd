# Python OOP中的封装与容器小结 - 小灰笔记 - CSDN博客





2017年07月19日 23:06:44[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：198








       从我手头的书籍资料中并没有封装与容器这两个概念的定义，我自己给总结定义如下：

              封装：设计一个类使其能够含有所有的类的对象的过程。

              容易：封装后的类实例化后就生成一个容器。

       之前的代码中设计了扑克牌的相应描述类，具体代码如下：

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

       elif rank == 11:

              return FaceCard('J',suit)

       elif rank == 12:

              return FaceCard('Q',suit)

       elif rank == 13:

              return FaceCard('K',suit)

       else:

              raise Exception("Rank out ofrange")

       在此基础上，设计封装类如下：

Club,Diamond,Heart,Spade=Suit('Club','CCC'),Suit('Diamond','DDD'),Suit('Heart','HHH'),Suit('Spade','SSS')



class Deck():

       def __init__(self):

              self._cards = [card(rank,suit) \

                     for rank in range(1,14)

                     for suit in(Club,Diamond,Heart,Spade)]

              random.shuffle(self._cards)



       def pop(self):

              return self._cards.pop()

       这样就可以创建并使用相应的容器，具体的示范可以如下：

d = Deck()

hand =[d.pop(),d.pop(),d.pop()]

for h in hand:

       print([h.rank,h.suit.name,h.hard,h.soft])

       代码的执行结果如下：

E:\WorkSpace\programme_language\python\OOP>python1_7_1.py

['Q', 'Diamond',10, 10]

['Q', 'Club', 10,10]

['8', 'Diamond',8, 8]

       从上面可以看出，通过这样的构建已经能够完成一个扑克牌的随机发牌功能了。一副扑克牌除了大小王之外的其他牌面全都可以覆盖，功能还算是完整。



