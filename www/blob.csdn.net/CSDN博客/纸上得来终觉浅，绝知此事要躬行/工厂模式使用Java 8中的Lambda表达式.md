# 工厂模式使用Java 8中的Lambda表达式 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年03月06日 22:22:17[boonya](https://me.csdn.net/boonya)阅读数：558








原文地址：[https://dzone.com/articles/factory-pattern-using-lambda-expression-in-java-8?utm_source=Top%205&utm_medium=email&utm_campaign=2017-03-04](https://dzone.com/articles/factory-pattern-using-lambda-expression-in-java-8?utm_source=Top%205&utm_medium=email&utm_campaign=2017-03-04)




### 工厂模式是Java中最着名的模式之一。如果你使用lambda表达式，你可以使用它们来实现模式，但要小心缩放。



![](https://dz2cdn4.dzone.com/thumbnail?fid=4407623&w=80)通过
[Monika
 Goel](https://translate.googleusercontent.com/translate_c?depth=1&hl=zh-CN&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://dzone.com/users/1380583/goel.mittal.html&usg=ALkJrhjtRw-v2q7Y9lpEv3ZuL7PACFp6QQ)
·
3月01日，17·[Java区](https://translate.googleusercontent.com/translate_c?depth=1&hl=zh-CN&rurl=translate.google.com&sl=en&sp=nmt4&tl=zh-CN&u=https://dzone.com/java-jdk-development-tutorials-tools-news&usg=ALkJrhhDZUQOLkFunye3CpwFeS84EVGthg)






工厂模式是Java中最常用的设计模式之一。这种类型的设计模式属于主机的创造模式，因为这种模式提供了一种最好的方式来创建一个对象。工厂设计模式允许您创建对象，而不将实例化逻辑暴露给客户端。

在这篇文章中，我想给出一个Factory模式的示例，然后使用在Java 8中引入的lambda表达式重写相同的示例。

## 工厂模式：一个例子

我将创建一个*Shape*接口以及实现*Shape*接口的具体类。一个工厂类*ShapeFactory*被定义为下一步。

创建一个接口：*Shape.java*

```java
public interface Shape {

   void draw();

}
```



考虑这个Shape接口的两个实现：*Circle.java*＆*Rectangle.java*

```java
public class Rectangle implements Shape {

   @Override

   public void draw() {

      System.out.println("Inside Rectangle::draw() method.");

   }

}

public class Circle implements Shape {

   @Override

   public void draw() {

      System.out.println("Inside Circle::draw() method.");

   }

}
```



创建工厂根据给定信息生成具体类的对象。

```java
public class ShapeFactory {

   //use getShape method to get object of type shape 

   public Shape getShape(String shapeType){

      if(shapeType == null){

         return null;

      }

      if(shapeType.equalsIgnoreCase("CIRCLE")){

         return new Circle();

      } else if(shapeType.equalsIgnoreCase("RECTANGLE")){

         return new Rectangle();         

      }       

      return null;

   }

}
```



使用Factory通过传递类型为：*FactoryPatternDemo.java的信息*来获取具体类的对象

```java
public class FactoryPatternDemo {

   public static void main(String[] args) {

      ShapeFactory shapeFactory = new ShapeFactory();

      //get an object of Circle and call its draw method.

      Shape shape1 = shapeFactory.getShape("CIRCLE");

      //call draw method of Circle

      shape1.draw();

      //get an object of Rectangle and call its draw method.

      Shape shape2 = shapeFactory.getShape("RECTANGLE");

      //call draw method of Rectangle

      shape2.draw();

   }

}
```



验证输出：

```
Inside Circle::draw() method.

Inside Rectangle::draw() method.
```



## 工厂模式：使用Lambda表达式的重构

在Java 8中，我们可以参考构造函数，就像我们引用方法一样，通过使用
方法引用。例如，下面是如何引用Circle构造函数：

```java
Supplier<Shape> circleSupplier = Circle::new;

Circle circle = circleSupplier.get();
```



使用这种技术，我们可以通过创建映射形状的Map来重写前面的代码
名称到其构造函数：

```java
final static Map<String, Supplier<Shape>> map = new HashMap<>();

  static {

    map.put("CIRCLE", Circle::new);

    map.put("RECTANGLE", Rectangle::new);

  }
```



我们现在可以使用这个Map来实例化不同的形状，就像我们以前的工厂代码一样：

```java
public class ShapeFactory {

  final static Map<String, Supplier<Shape>> map = new HashMap<>();

  static {

    map.put("CIRCLE", Circle::new);

    map.put("RECTANGLE", Rectangle::new);

  }   

  public Shape getShape(String shapeType){

     Supplier<Shape> shape = map.get(shapeType.toUpperCase());

     if(shape != null) {

       return shape.get();

     }

     throw new IllegalArgumentException("No such shape " + shapeType.toUpperCase());

  }

}
```



使用工厂获得具体类的对象，使用lambda表达式：*FactoryPatternDemo.java*

```java
public class FactoryPatternDemo {

   public static void main(String[] args) {

     Supplier<ShapeFactory> shapeFactory =  ShapeFactory::new;

  //call draw method of circle

     shapeFactory.get().getShape("circle").draw();

     //call draw method of Rectangle

     shapeFactory.get().getShape("rectangle").draw();      

   }

}
```



验证输出：



```
Inside Circle::draw() method.

Inside Rectangle::draw() method.
```


这是一个相当整洁的方式使用Java 8功能来实现相同的意图工厂模式。但是这种技术不能很好地缩放，如果工厂方法getShape需要采取多个参数传递给Shape构造函数！你必须提供与简单的供应商不同的功能接口。







