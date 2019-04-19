# Java 封装/继承/多态 - Simple 专栏 - CSDN博客
2012年01月05日 19:04:10[Simple_Zz](https://me.csdn.net/love284969214)阅读数：344标签：[Java面试																[面向对象																[编程思想																[java](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=编程思想&t=blog)](https://so.csdn.net/so/search/s.do?q=面向对象&t=blog)](https://so.csdn.net/so/search/s.do?q=Java面试&t=blog)
个人分类：[Java](https://blog.csdn.net/love284969214/article/category/1235868)
所属专栏：[Java 浅谈](https://blog.csdn.net/column/details/27397.html)
# 为什么要封装、继承、多态？
- 目的都是为了——代码重用。
- 封装可以隐藏实现细节，使得代码模块化。
- 继承可以扩展已存在的代码模块（类）。
- 多态可以解决项目中紧偶合的问题，提高程序的可扩展性.。
### 封装
- [百度词条定义](https://baike.baidu.com/item/%E5%B0%81%E8%A3%85/2796965?fr=aladdin)
- 封装就是将事物拥有的属性和动作隐藏起来，只保留特定的方法与外界联系。
```java
// 代码理解
public class Human {
    // private 私有成员变量外部不可访问
    private int age;
    private String name;
    // public 提供公共方法让外部访问
    // 把每个私有的成员变量，提供两个方法get/set让外部访问
    public int getAge() {
        return age;
    }
    public void setAge(int age) throws Exception {
        // 封装age的检验逻辑，而不是暴露给每个调用者去处理
        if(age > 120) {
            throw new Exception("注意：这个人超过了120岁，要成精了。");
        }
        this.age = age;
    }
    public String getName() {
        return name;
    }
    public void setName(String name) {
        this.name = name;
    }
}
```
### 继承
- [百度词条定义](https://baike.baidu.com/item/%E7%BB%A7%E6%89%BF/20267560#viewPageContent)
- 继承就是将事物公共的属性和方法暴露出来，子类继承父类的属性和方法或者重新定义、追加属性和方法等。
- 只能继承 类、抽象类，不能继承接口、final类。  [【类、抽象类、接口】 相关文章](https://blog.csdn.net/love284969214/article/details/7182428)
```java
/** 
  * 继承以后子类可以得到什么：
  *  1.子类拥有父类非private的属性和方法。
  *  2.子类可以添加自己的方法和属性，即对父类进行扩展。
  *  3.子类可以重新定义父类的方法。
  * 关于构造函数：
  *  1.构造函数不能被继承，子类可以通过super()显示调用父类的构造函数。
  *  2.创建子类时，编译器会自动调用父类的 无参构造函数。
  *  3.如果父类没有定义无参构造函数，子类必须在构造函数的第一行代码使用super()显示调用。
  */
```
### 多态
- [百度词条定义](https://baike.baidu.com/item/%E5%A4%9A%E6%80%81/2282489?fr=aladdin)
- 多态允许不同子类 对象对同一消息作出不同的响应。
- 多态就是允许方法重名，参数或返回值是由父类决定。
- 多态的缺点：由于子类引用父类的类型可以调用父类中定义的所有属性和方法，对于只存在与子类中的方法和属性它就望尘莫及了。
- 多态实现的三个必要条件：继承、重写、向上转型。
多态性分为 **编译时**的多态性 和 **运行时**的多态性 。
编译时的多态性：就是在程序编译的时候，也就是生成解决方案的时候就决定要实现什么操作。
运行时的多态性：就是指直到系统运行时，才根据实际情况决定实现何种操作。
**方法重载** 实现的是编译时的多态性（也称为前绑定）。
**方法重写** 实现的是运行时的多态性（也称为后绑定）。[【重写、重载】相关文章](https://blog.csdn.net/love284969214/article/details/7183955)
理解多态：
如果将对象的方法视为对象向外界提供的服务，那么当 Son 访问 Father 提供的服务时，Father 有多种提供服务的方式，这一切对 Son 来说都是透明的。
（就像 Son 是学习者，Father 是技能掌握者，Father 会修电脑、换灯泡、做饭等等，Son 只会通过 Father 对象来调用 修电脑 的方法，但并不知道 修电脑 的底层实现是什么，究竟通过何种方式把电脑修好，反正电脑坏了找 Father 调用修电脑的方法即可）。
