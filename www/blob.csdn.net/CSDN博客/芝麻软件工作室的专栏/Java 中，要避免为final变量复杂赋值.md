
# Java 中，要避免为final变量复杂赋值 -  芝麻软件工作室的专栏 - CSDN博客


2016年05月31日 06:38:16[seven-soft](https://me.csdn.net/softn)阅读数：193


为final 变量赋值还有一种方式：通过方法赋值，即直接在声明时通过方法返回值赋值。还是以Person 类为例来说明，代码如下：
public class Person implements Serializable{
private static final long serialVersionUID = 91282334L;
// 通过方法返回值为final 变量赋值
public final String name=initName();
// 初始化方法名
public String initName(){
return " 混世魔王";
}
}
name 属性是通过initName 方法的返回值赋值的，这在复杂类中经常用到，这比使用构造函数赋值更简洁、易修改，那么如此用法在序列化时会不会有问题呢？我们一起来看看。
Person 类写好了（定义为V1.0 版本），先把它序列化，存储到本地文件，其代码与上一建议的Serialize 类相同，不再赘述。
现在，Person 类的代码需要修改，initName 的返回值也改变了，代码如下：
public class Person implements Serializable{
private static final long serialVersionUID = 91282334L;
// 通过方法返回值为final 变量赋值
public final String name=initName();
// 初始化方法名
public String initName(){
return " 德天使";
}
}
上段代码仅仅修改了initName 的返回值（Person 类为V2.0 版本），也就是说通过new生成的Person 对象的 final 变量值都是“德天使”。那么我们把之前存储在磁盘上的实例加载上来，name 值会是什么呢？
结果是：混世魔王。很诧异，上一建议说过final 变量会被重新赋值，但是这个例子又没有重新赋值，为什么？
上个建议所说final 会被重新赋值，其中的“值”指的是简单对象。简单对象包括：8个基本类型，以及数组、字符串（字符串情况很复杂，不通过new 关键字生成String 对象的情况下，final 变量的赋值与基本类型相同），但是不能方法赋值。
其中的原理是这样的，保存到磁盘上（或网络传输）的对象文件包括两部分：类描述信息
包括包路径、继承关系、访问权限、变量描述、变量访问权限、方法签名、返回值，以及变量的关联类信息。要注意的一点是，它并不是class 文件的翻版，它不记录方法、构造
函数、static 变量等的具体实现。之所以类描述会被保存，很简单，是因为能去也能回嘛，这保证反序列化的健壮运行。
非瞬态（transient 关键字）和非静态（static 关键字）的实例变量值
注意，这里的值如果是一个基本类型，好说，就是一个简单值保存下来；如果是复杂对象，也简单，连该对象和关联类信息一起保存，并且持续递归下去（关联类也必须实现Serializable接口，否则会出现序列化异常），也就是说递归到最后，其实还是基本数据类型的保存。
正是因为这两点原因，一个持久化后的对象文件会比一个class 类文件大很多，有兴趣的读者可以自己写个Hello word 程序检验一下，其体积确实膨胀了不少。
总结一下，反序列化时final 变量在以下情况下不会被重新赋值：通过构造函数为final 变量赋值。
通过方法返回值为final 变量赋值。
final 修饰的属性不是基本类型。

