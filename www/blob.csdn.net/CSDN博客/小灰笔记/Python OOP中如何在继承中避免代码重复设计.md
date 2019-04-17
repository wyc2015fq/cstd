# Python OOP中如何在继承中避免代码重复设计 - 小灰笔记 - CSDN博客





2017年07月19日 22:42:11[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：204








       代码的重复设计可能会导致重复的问题出现以及重复的维护工作，这不仅仅是工作时间的增加，有时候也会带来很多维护上的困难。从继承角度来看，很多代码的重复是因为代父类的设计不够好。下面通过对比两段代码来看一下相应的问题：

代码1：

class Card:

       pass



classNumberCard(Card):

       def __init__(self,rank,suit):

              self.rank = str(rank)

              self.suit = suit

              self.hard = self.soft = rank



classAceCard(Card):

       def __init__(self,rank,suit):

              self.rank = 'A'

              self.suit = suit

              self.hard,self.soft = 1,11



classFaceCard(Card):

       def __init__(self,rank,suit):

              self.rank ={11:'J',12:'Q',13:'K'}[rank]

              self.suit = suit

              self.hard = rank.soft = 10



       上面的代码能够成功的创建出不同的类用来表达不同的纸牌概念，但是从代码的构件上可以看出，有很多代码的形式是重复的。如何对此进行改进呢？比较好的方法就是把重复的内容设计到父类中。具体的代码如下：

class Card:

       def __init__(self,rank,suit,hard,soft):

              self.rank = rank

              self.suit = suit

              self.hard = hard

              self.soft = soft



classNumberCard(Card):

       def __init__(self,rank,suit):

              super().__init__(str(rank),suit,rank,rank)



classAceCard(Card):

       def __init__(self,suit):

              super().__init__('A',suit,1,11)



classFaceCard(Card):

       def __init__(self,rank,suit):

              super().__init__({11:'J',12:'Q',13:'K'}[rank],suit,10,10)



       上面的这段代码中，重复设计的东西少了很多。更多的是对设计好的方法进行调用，这样的代码在程序的维护以及重构上就要方便的多。



