# Java 动态绑定 - Simple 专栏 - CSDN博客
2018年11月16日 11:59:23[Simple_Zz](https://me.csdn.net/love284969214)阅读数：45标签：[动态绑定																[java](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=动态绑定&t=blog)
个人分类：[Java](https://blog.csdn.net/love284969214/article/category/1235868)
### 动态绑定：在程序运行过程中，根据具体的实例对象才能具体确定是哪个方法。
动态绑定是多态性得以实现的重要因素，它通过方法表来实现：每个类被加载到虚拟机时，在方法区保存元数据，其中，包括一个叫做方法表（methodtable）的东西，表中记录了这个类定义的方法的指针，每个表项指向一个具体的方法代码。如果这个类重写了父类中的某个方法，则对应表项指向新的代码实现处。从父类继承来的方法位于子类定义的方法的前面。
将一个子类的对象地址，交给 Father （父类） 引用
java的动态绑定机制的小结
- 如果调用的是方法，则Jvm会将该方法和对象的内存地址绑定
- 如果调用的是属性，则没有动态绑定机制，在那调用，就返回对应值。
**例:**
```java
// 输出 
Father obj = new Son();
System.out.println(obj.sum());    //40
System.out.println(obj.sum1());   //30
class Father {
    public int i = 10;
    public int sum()  {        return getI() + 10; }
    public int sum1() {        return i + 10;      }
    public int getI() {        return i;           }
}
class Son extends Father {
    public int i = 20;
    public int sum()  {        return getI() + 20; }
    public int sum1() {        return i + 10;      }
    public int getI() {        return i;           }
}
-----------------------------------------------------
// 输出 
Father obj = new Son();
System.out.println(obj.sum());    //30
System.out.println(obj.sum1());   //20
class Father {
    public int i = 10;
    public int sum()  {        return getI() + 10; }
    public int sum1() {        return i + 10;      }
    public int getI() {        return i;           }
}
class Son extends Father {
    public int i = 20;
    public int getI() {        return i;           }
}
```
