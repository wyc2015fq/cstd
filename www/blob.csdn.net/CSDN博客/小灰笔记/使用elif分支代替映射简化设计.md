# 使用elif分支代替映射简化设计 - 小灰笔记 - CSDN博客





2017年07月07日 00:46:47[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：189
个人分类：[Python																[OOP](https://blog.csdn.net/grey_csdn/article/category/6926551)](https://blog.csdn.net/grey_csdn/article/category/6639459)








       在Python的程序设计中，字典这样的映射程序总是可以用elif结构来代替。通常，这样可以使得设计获得更好的一致性。例如，之前有代码如下：

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

       在实现card工厂函数的时候，上面的代码中使用了映射。此函数可以进行修改，设计为如下方式：

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

       通过上面的修改，虽然代码长度有所增加，但是代码获得了更好的一致性。相比之下，如果是用于处理简单的情况，通常使用字典这样的映射会更为高效地工作也具有更好的可读性。但是elif分支的实现方式，能够更为一致性的结构和更清晰的实现意图。




