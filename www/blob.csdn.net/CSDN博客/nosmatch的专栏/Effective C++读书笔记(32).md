# Effective C++读书笔记(32) - nosmatch的专栏 - CSDN博客
2013年07月21日 12:16:21[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：677
条款32：确定你的public继承塑模出is-a关系
> 
以public继承的类，其父类的所有的性质都应该使用与子类，任何需要父类的地方都应该能用子类来代替，任何子类类型
的对象也同时是父类的：
> 
```cpp
class Person{...};
class Student : public Person{...};
void eat(const Person& p);
void study(const Student& s);
Person p;
Student s;
eat(p);
eat(s);
study(s);
study(p);     //错误
```
在上面的例子中student是Person的子类，以public继承，因为每个student都是一个Person，每个Person的属性都是适合
Student，在现实生活中，我们要注意public的关系，例如对于企鹅和鸟，正方形和矩形等，看起来可能是public继承比较合适，
但是企鹅并不含有鸟的一切特征，例如飞，正方形也不具有矩形的所有特征，例如正方形不会出现长和宽不相等的情况，因此
在设计public继承的时候要注意！
请记住：
- public继承意味着"is-a",使用与base-class的每一件事情也一定适用于derived cleass身上，因为没个drived对象一定是个base对象
