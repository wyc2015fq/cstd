# Java中的String真的无法修改吗 - walkabc - 博客园
# [Java中的String真的无法修改吗](https://www.cnblogs.com/hitfire/p/8728037.html)
Java中String一旦赋值将无法修改，每次对String值的修改都是返回新的String。
如何在不创建新的String对象的情况下，对String的值进行修改呢？
String类中的包含一个字段
```java;gutter
private final char value[];
```
这个value中保存String的值。企图对String的修改，其实就是对value的修改。
首先获取到value的对象的引用，因为value是私有对象，所以我们需要通过反射来获得value。
```java;gutter
String string = "HelloWorld";
Field valueField = String.class.getDeclaredField("value");
valueField.setAccessible(true);
char[] value = (char[]) valueField.get(string);
```
获取到value之后，因为value是一个字符数组，所以直接使用
```
value[0] = 'Y';
```
这样就可以直接修改String的值了。

