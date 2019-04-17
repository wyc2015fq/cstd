# Java —— 对象类（Object类） - Alex_McAvoy的博客 - CSDN博客





2018年10月03日 12:02:32[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：56
个人分类：[Java学习笔记](https://blog.csdn.net/u011815404/article/category/8863016)

所属专栏：[Java 学习笔记](https://blog.csdn.net/column/details/34317.html)









# 【概述】
- Object 类位于 java.lang 包中，java.lang 包包含着 Java 最基础和核心的类，在编译时会自动导入。
- Object 类是所有 Java 类的根父类（祖先），每个类都使用 Object 作为超类，如果在类的声明中未使用 extends 指定父类，则默认父类为 Object 类。
- 所有对象、数组都可实现这个类的方法，可以使用类型为 Object 的变量指向任意类型的对象。

# 【equals() 方法】

1.equals() 方法用于比较两个对象是否相等，若相等则返回 true，若不等则返回 false

2.所有的对象都拥有标识（内存地址）和状态（数据），使用 “==” 比较两个对象是比较的内存地址，因此使用 Object 的 equals() 方法实际上仍是比较两个对象的内存地址是否相等，即若结果为 true，则表示两者实际上是引用同一个对象。

源码

```java
public boolean equals(Object obj) {
       return (this == obj);
}
```

3.Object 的 equals() 方法可以满足一些基本要求，但大部分情况都是进行两个对象的状态比较，此时 Object 的 equals() 方法就不可以了，因此在 JDK 中，String、Math 等封装类都对 equals() 方法进行了重写，即比较状态是否完全相同。

# 【toString() 方法】

1.该方法返回值是 String 类型，返回的是此对象所在的类及对应的堆空间对象实体的首地址值，该字符串由类名（对象是该类的一个实例）、at 标记符“@”、此对象哈希码的无符号十六进制组成。

即：一个对象的 toString() 返回字符串的一般形式为：包名.类名@内存的引用地址

```java
//源码
public String toString() {
    return getClass().getName() + "@" + Integer.toHexString(hashCode());
}

//实例
Object obj=new Object();
System.out.println(onj.toString());//结果为：java.lang.Object@19e0bfd
```

2.建议所有的子类都重写此方法（将对象的属性信息返回），若没有重写，则调用的就是 Object 中定义的 toString() 方法


例如：

```java
public String toString(){
    return "Person:name="+name+" age="+age;
}
```

3.在进行 String 与其它数据类型进行连接操作时，会自动调用该方法

```java
Data now=new Data();
System.out.println("now="+now);//隐式
System.out.println("now="+now.toString());//显式
```

4.String、Date、File类、包装类等，已经实现了 Object 类中 toString() 方法的重写，即输出对象中的实体内容





