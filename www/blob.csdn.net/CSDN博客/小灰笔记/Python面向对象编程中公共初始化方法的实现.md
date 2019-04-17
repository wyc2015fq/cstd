# Python面向对象编程中公共初始化方法的实现 - 小灰笔记 - CSDN博客





2017年07月04日 00:33:08[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：392








     其实，公共初始化的实现肯定是可以通过多次编写同样的初始化方法实现。但是，多次编写的这种方式不仅繁杂而且不容易管理。如果要做到管理方便、软件的结构清晰，最好还是创建一个基类然后让其他的类全都继承此基类。

       例如，有代码如下：

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

       通过基类Card的__init__方法共享了全局的初始化功能，这样，继承自这个类的类都能够实现相应的初始化。利用以上的几个类进行对象的创建如下：

card1 =AceCard('A','spade')

card2 =NumberCard('3','heart')

card3 =FaceCard('K','Diamond')

       代码执行测试后，没有创建上的问题。其实，从这个简单的功能上也可以看得出Python面向对象编程的一个很重要的作用，那就是可以通过对象继承的方式使得代码能够被重用。



