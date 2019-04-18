# 【java开发系列】—— 自定义注解 - z69183787的专栏 - CSDN博客
2016年05月12日 15:42:50[OkidoGreen](https://me.csdn.net/z69183787)阅读数：811
> 
　　之前在开发中，就总纳闷，为什么继承接口时，会出现@Override注解，有时候还会提示写注解@SuppressWarnings？
## 　　原来这是java特有的特性，注解！
**　　那么什么是注解呢？**
　　注解就是某种注解类型的一个实例，我们可以用它在某个类上进行标注，这样编译器在编译我们的文件时，会根据我们自己设定的方法来编译类。
**注解都是什么呢？**看下面这张图就明白了！![](http://images.cnitblog.com/blog/449064/201409/111632494497100.png)
　　上面的图可以看出，注解大体上分为三种：标记注解，一般注解，元注解
　　这里面Override这个没测试出来，因为目前的Eclipse会自动帮我们排错，如果类型不符，是没有办法进行覆盖的。
　　而Deprecated注解，除了多个删除线，并没有什么拦截功能。![](http://images.cnitblog.com/blog/449064/201409/111640426375905.png)
　　上面的测试，也仅仅是针对IDE，如果是利用javac，应该会有提示的。
　　其他的不多说了，**标准元注解 都是干嘛的呢？**
@Documented 标记生成javadoc
　　@Inherited 标记继承关系
　　@Retention 注解的生存期
　　@Target 标注的目标
## 下面我们自己做一个注解！
　　首先声明一个接口，并未它添加注解内容！
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 package testAnnotation;
 2 
 3 import java.lang.annotation.Documented;
 4 import java.lang.annotation.Retention;
 5 import java.lang.annotation.RetentionPolicy;
 6 
 7 @Documented
 8 @Retention(RetentionPolicy.RUNTIME)
 9 public @interface Person{
10     String name();
11     int age();
12 }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　然后利用反射机制查看类的注解内容
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 package testAnnotation;
 2 
 3 @Person(name="xingoo",age=25)
 4 public class test3 {
 5     public static void print(Class c){
 6         System.out.println(c.getName());
 7         
 8         //java.lang.Class的getAnnotation方法，如果有注解，则返回注解。否则返回null
 9         Person person = (Person)c.getAnnotation(Person.class);
10         
11         if(person != null){
12             System.out.println("name:"+person.name()+" age:"+person.age());
13         }else{
14             System.out.println("person unknown!");
15         }
16     }
17     public static void main(String[] args){
18         test3.print(test3.class);
19     }
20 }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　运行结果，读取到了注解的内容
```
testAnnotation.test3
name:xingoo age:25
```
