# [CareerCup] 14.1 Private Constructor 私有构建函数 - Grandyang - 博客园







# [[CareerCup] 14.1 Private Constructor 私有构建函数](https://www.cnblogs.com/grandyang/p/4942457.html)







14.1 In terms of inheritance, what is the effect of keeping a constructor private?



这道题问我们用继承特性时，如果建立一个私有的构建函数会怎样。

只有能访问该函数的私有变量或函数的东西才能访问私有构建函数，比如内部类就可以访问，比如类A是类Q的一个内部类，那么Q的其他内部类可以访问类A的私有构建函数。

由于一个子类可以调用父类的构建函数，类A可以被继承，所以只有类A的子类或者是类Q的其他子类可以访问类A的私有构建函数。












