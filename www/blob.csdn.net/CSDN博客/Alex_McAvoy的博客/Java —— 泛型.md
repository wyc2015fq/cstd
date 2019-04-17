# Java —— 泛型 - Alex_McAvoy的博客 - CSDN博客





2018年10月19日 19:47:11[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：32
个人分类：[Java学习笔记](https://blog.csdn.net/u011815404/article/category/8863016)

所属专栏：[Java 学习笔记](https://blog.csdn.net/column/details/34317.html)









# 【概述】

对于一个集合来说，没有泛型时，任何类型都可以添加进集合，此时会造成类型不安全问题。

为解决对象存储的安全性问题以及获取对象时类型强转的问题，在集合中加入了泛型。

使用泛型后，集合中只能添加泛型类型的对象，即集合中存储的元素全是所指定的泛型类型。

```java
List<String> list = new ArrayList<String>();
Set<Integer> set = new HashSet<Integer>();
Map<String,Integer> map = new HashMap<String,Integer>();
```

# 【自定义泛型】

除 Object 类的泛型外，可自定义泛型类，当实例化泛型类对象时，指明泛型类型，指明后，对应类中所有使用泛型的位置，都变为实例化中指定的泛型类型；但若自定义泛型类后，没有在实例化时使用，则默认类型仍为 Object 类

声明自定义泛型方法如下：

```java
//当通过对象调用方法时，指明泛型方法
public <E> E get(E e){
    return e;
}

public <E> List<E> fromArrayToList(E[] e,List<E> list){
    for(E el:e){
        list.add(e1);
    }
    return list;
}
```

# 【泛型与继承的关系】

若类 A 是类 B 的子类，则两对应的泛型集合不具备继承关系，彼此是并列的

```java
List<Object> list1 = new ArrayList<Object>();
List<String> list2 = new ArrayList<String>();

list1=list2;//报错
```

# 【通配符】

通配符 <?>，是一切泛型的父类，例如：

```java
List<?> list = null;
List<Object> list1 = new ArrayList<Object>();
List<String> list2 = new ArrayList<String>();

list=list1;
list=list2;
```

使用通配符时，可加上 extend、suoer 修饰词来限定范围，如：? extend A ：可存放 A 及其子类（大于等于 A 的），? super A ：可存放 A 及其父类（小于等于 A 的）

```java
List<? extend Number> list1 = null;
List<Integer> list2 = null;
List<? super Number> list3 = null;
List<Object> list4 =null;

list1=list2;
list3=list4;
```

使用泛型时，要注意可以读取声明为通配符的集合类的对象，但不允许向声明为通配符的集合类中写入对象（唯一例外的是 null）

```java
List<Integer> list = new ArrayList<Integer>();
list.add(123);
list.add(456);
list.add(789);

List<?> list1=list;
Iterator<?> it = list1.iterator();
while(it.hasNext())
    System.out.print(it.next()+" ");//结果：123 456 789

list1.add(null);
list1.add(111);//报错
list1.add("abc");//报错
```



