#  			[Python进阶内容（五）--- type和object的关系](https://www.cnblogs.com/echo1937/p/7616891.html) 		



## 面向对象编程（OOP）的两大关系

继承与实现

- 继承关系： 
  - 子类继承自父类（base），可以使用父类的一些方法（method）和属性（attribute）
- 实现关系： 
  - 以类为模板，实例化一个对象，即：对象（object）是类（class）的实例（instance）

　　　　![img](https://images2017.cnblogs.com/blog/922424/201710/922424-20171001100234919-1708508883.png)

实际举例：

- snake是一种爬行动物（reptile）（子类和父类的继承关系）
- 我养的宠物蛇squasher是snake的一个实例（对象是类的实例，实现关系）

一图胜千言：

　　　　![img](https://images2017.cnblogs.com/blog/922424/201710/922424-20171001110907028-2120128744.png)

Python中一切都是对象，言外之意：

- 在Python中，函数是一个对象，类也是一个对象。那么既然类是一个对象，谁是它的类？我们都知道，一个对象肯定会有一个类模板。（类也是对象，对象是类的实例）

## 实例关系

上面我们说到，类也是一个对象，谁是它的类模板。

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
class A:
    pass


def foo(cls):
    print(cls())


B = A
a = A()

if __name__ == "__main__":
    print(type(a))              # <class '__main__.A'>
    print(type(1))              # <class 'int'>
    print(type(foo))            # <class 'function'>

    print(type("abc"))          # <class 'str'>
    print(type(type("abc")))    # <class 'type'>

    print(type(A))              # <class 'type'>
    print(type(type))           # <class 'type'>
    print(type(object))         # <class 'type'>

    # 也可以使用__class__来判断是哪个类的实例
    print(a.__class__)          # <class '__main__.A'>
    print("abc".__class__)      # <class 'str'>
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

从上图可以看出：

- 自定义类A的实例a（ a = A() ）的类模板就是 自定义类A，字符串"abc"/整数1 分别是 类str/类int 的实例
- 自定义类A是类type的一个实例，那所有类的基类object，是谁的实例：type类（的实例）
- 那type类是哪个类的实例：type类。

**总结：type这个类模板产生了一切实例**

## 继承关系

继承关系是 类与类之间的关系，与对象无关，也不涉及任何实例。

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
class A:
    pass


class B(A):
    pass


if __name__ == "__main__":
    print(A.__bases__)          # (<class 'object'>,)
    print(B.__bases__)          # (<class '__main__.A'>,)
    print(str.__bases__)        # (<class 'object'>,)
    print(int.__bases__)        # (<class 'object'>,)
    print(type.__bases__)       # (<class 'object'>,)
    print(object.__bases__)     # ()
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

从上面我们可以学到：

- A继承自object，B继承自A，A-->B-->object
- str int等等内置类都是object的子类
- object类的父类为None

可是type类的父类是object，object类不是type类的实例吗？



该图表示：

- object类是type类的一个实例，type类是type类的一个实例（实现关系）
- type类是object类的子类（继承关系）

像type这种类，实例化后的对象是类，这种类叫做元类

![img](https://images2017.cnblogs.com/blog/922424/201710/922424-20171001124305340-561832709.png)

 



分类: [Python](https://www.cnblogs.com/echo1937/category/966904.html)