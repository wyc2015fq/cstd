# Java中的反射与动态代理 - LC900730的博客 - CSDN博客
2017年11月12日 17:13:31[lc900730](https://me.csdn.net/LC900730)阅读数：236标签：[java](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[java](https://blog.csdn.net/LC900730/article/category/7027036)
通过一个字符串，获取这个字符串所代表的那个类的属性/方法等。 
拿到字符串，不能new字符串。
拿到构造函数
```java
//personClass是拿到之后的类
Constructor constructor=personClass.getConstructor(Long.class,String.class);
Person person=(Person) constructor.newInstance(100L,"zhangsan");
//私有的构造方法
//暴力反射
public void getPrivateConstructor() throws Exception{
    //一个string类型的构造参数
    Constructor con=personClass.getDeclaredConstructor(String.class);
    con.setAccessible(true);
    Person person2=(Person)con.newInstance("zhangsan");
    System.out.println("*"+person2.getName());
}
```
### 获取非私有成员
```
Object obj=constructor.newInstance(100L,"zhangsan");
Field field=personClass.getField("name");
//给具体对象设置
field.set(obj,"lisi");
```
### 获取非私有成员
```
Object obj=constructor.newInstance(100L);
Field field2=personClass.getDeclaredField("id");
field2.setAccessible(true);
field2.set(obj,10000L);
System.out.println(field2.get(obj));
```
### 获取非私有函数
```
Object obj=constructor.newInstance();
//这个对象来实现这个方法。
Object object=personClass.getMethod("toString").invoke(obj);
//即：Method toStringMethod=personClass.getMethod("tostring");
Object object=toStringMethod.invoke(obj);
System.out.println(object);
```
