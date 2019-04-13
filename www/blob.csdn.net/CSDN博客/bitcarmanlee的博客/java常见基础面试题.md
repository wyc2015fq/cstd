
# java常见基础面试题 - bitcarmanlee的博客 - CSDN博客


2017年07月30日 11:24:02[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：274标签：[java																](https://so.csdn.net/so/search/s.do?q=java&t=blog)[面试题																](https://so.csdn.net/so/search/s.do?q=面试题&t=blog)[常见																](https://so.csdn.net/so/search/s.do?q=常见&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=面试题&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=java&t=blog)个人分类：[java																](https://blog.csdn.net/bitcarmanlee/article/category/6139246)
[
																								](https://so.csdn.net/so/search/s.do?q=java&t=blog)


虽然实际中一直在写java代码，但是很多时候一些基础的东西还是需要注意一下的。特意记录一下一些基础方面的知识，权当自己做笔记了，后面也有统一的地方可以查。
## 1.Java中实现多态的机制
多态是面向对象的一个基本特征。java中的多态是指父类或接口定义的引用变量可以指向子类或具体实现类的实例对象，而程序调用的方法在运行期才动态绑定，就是引用变量所指向的具体实例对象的方法，也就是内存里正在运行的那个对象的方法，而不是引用变量的类型中定义的方法。
## 2.abstractclass和interface语法上的区别
1.抽象类可以有构造方法，接口中不能有构造方法。
2.抽象类中可以有普通成员变量，接口中没有普通成员变量
3.抽象类中可以包含非抽象的普通方法，接口中的所有方法必须都是抽象的，不能有非抽象的普通方法。
4. 抽象类中的抽象方法的访问类型可以是public，protected和（默认类型,虽然
eclipse下不报错，但应该也不行），但接口中的抽象方法只能是public类型的，并且默认即为public abstract类型。
5. 抽象类中可以包含静态方法，接口中不能包含静态方法
6. 抽象类和接口中都可以包含静态成员变量，抽象类中的静态成员变量的访问类型可以任意，但接口中定义的变量只能是publicstatic final类型，并且默认即为publicstatic final类型。
7. 一个类可以实现多个接口，但只能继承一个抽象类。

