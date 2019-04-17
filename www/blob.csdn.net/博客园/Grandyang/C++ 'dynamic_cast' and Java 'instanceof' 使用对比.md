# C++ 'dynamic_cast' and Java 'instanceof' 使用对比 - Grandyang - 博客园







# [C++ 'dynamic_cast' and Java 'instanceof' 使用对比](https://www.cnblogs.com/grandyang/p/4809774.html)







在Java中，如果A是基类，B是A的派生类，那么instanceof可以用来判断一个实例对象是A还是B，相当于一个二元操作符，但与==, >, < 不同的是，它是由字母组成，是Java的保留字。而在C++中却没有这样关键字，不过我们可以使用dynamic_cast来实现同样的功能，请参见下面的代码：



Java 'instanceof'

```
// Java 'instanceof'
public class A {
// ...
}

public class B extends A {
// ...
}

public class C extends A {
// ...
}

public void process(A a) {
    if (a instanceof B) {
        // Process B
    }
    if (a instanceof C) {
        // Process C
    }
}
```



C++ 'dynamic_cast'

```
// C++ 'dynamic_cast'
class A {
// ...
}

class B: public A {
// ...
}

class C: public A {
// ...
}

void process(A *a) {
    if (B *b = dynamic_cast<B*> (a)) {
        // Process B
    }
    if (C *c = dynamic_cast<C*> (a)) {
        // Process C
    }
}
```














