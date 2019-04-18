# Java反射，根据类名创建带参数的实例（获取参数类型，参数等） - z69183787的专栏 - CSDN博客
2014年04月16日 20:52:03[OkidoGreen](https://me.csdn.net/z69183787)阅读数：10213
Java代码  ![收藏代码](http://rich8w.iteye.com/images/icon_star.png)
- //根据类名获取Class对象
-   Class c=Class.forName("java.lang.Integer");    
- //参数类型数组
-   Class[] parameterTypes={String.class};   
- //根据参数类型获取相应的构造函数
-   java.lang.reflect.Constructor constructor=c.getConstructor(parameterTypes);  
- //参数数组
-   Object[] parameters={"1"};  
- //根据获取的构造函数和参数，创建实例
-   Object o=constructor.newInstance(parameters);  
- 
