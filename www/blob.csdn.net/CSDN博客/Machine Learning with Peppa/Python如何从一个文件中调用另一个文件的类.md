# Python如何从一个文件中调用另一个文件的类 - Machine Learning with Peppa - CSDN博客





2018年01月24日 21:55:47[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：2285
所属专栏：[用Python玩转数据](https://blog.csdn.net/column/details/18811.html)








如果是在同一个 module中(也就是同一个py文件里),直接用就可以
如果在不同的module里,例如
a.py里有 class  A:
b.py 里有 class B:
如果你要在class B里用class A 需要在 b.py的开头写上 from a import A举个例子：比如我在文件lingkingtables.py的文件中构造了三个类，在另一个文件中想要引用这三个类，那么用如下语句即可- from lingkingtables import Lnode  
- from lingkingtables import Llist  
- from lingkingtables import LinkListUnderFlow  
注意：类名要和文件名符合，且引用时要注意原始文件类的基类与派生类的关系


