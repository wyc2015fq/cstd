# [CareerCup] 14.5 Object Reflection 对象反射 - Grandyang - 博客园







# [[CareerCup] 14.5 Object Reflection 对象反射](https://www.cnblogs.com/grandyang/p/4971473.html)







14.5 Explain what object reflection is in Java and why it is useful.



Java中的对象反射机制可以获得Java类和对象的反射信息，并可采取如下操作：

1. 在运行阶段获得类内部的方法和字段信息

2. 新建类的实例

3.通过获取字段引用来获得和舍弃对象字段，无论该字段是私有还是公有的。



下列代码是对象反射的一个例子：

```
// Parameters
Object[] doubleArgs = new Object[] {4.2, 3.9};
// Get class
Class rectangleDefinition = Class.forNmae("MyProj.Rectangle");
// Equivalent: Rectange = new Rectangle(4.2, 3.9);
Class[] doubleArgsClass = new Class[] {double.class, double.class};
Constructor doubleArgsConstructor = rectangleDefinition.getConstructor(doubleArgsClass);
Rectangle rectangle = (Rectangle) doubleArgsConstructor.newInstance(doubleArgs);
// Equivalent: Double area = rectangle.area();
Method m = rectangleDefinition.getDeclaredMethod("area");
Double area = (Double) m.invoke(rectangle);

// This code does the equivalent of 
Rectangle rectangle = new Rectange(4.2, 3.9);
Double area = rectangle.area();
```



对象反射为什么有用？

1. 观测和操控应用运行时的行为

2. 帮助调试和测试程序，通过直接接触到方法，构造函数和字段

3. 我们可以调用方法即便我们事先不知道该方法。比如，我们可以让用户传入类名称，构建函数的参数，和方法名称。我们利用这些信息区建立一个对象并调用一个方法。



想了解更多关于Java反射机制的内容，可以参见[Java 反射机制学习资料](http://www.cnblogs.com/grandyang/p/4987329.html)。












