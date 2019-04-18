# 类方法学习五:getGenericSuperclass,getSuperclass - z69183787的专栏 - CSDN博客
2014年07月09日 20:21:47[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2456
一.说明
1.Class<? super T> getSuperclass():返回本类的父类
2.Type getGenericSuperclass():返回本类的父类,包含泛型参数信息
二.例子
1.ClassA.java
**[java]**[view
 plain](http://blog.csdn.net/csdn1234/article/details/2589443#)[copy](http://blog.csdn.net/csdn1234/article/details/2589443#)
- publicclass  ClassA <T>{  
- 
- private T obj;  
- 
- publicvoid setObject(T obj) {  
- 
- this.obj = obj;  
- 
-   }  
- 
- 
- 
- public T getObject() {  
- 
- return obj;  
- 
-   }  
- 
- }  
2.Test.java
**[java]**[view
 plain](http://blog.csdn.net/csdn1234/article/details/2589443#)[copy](http://blog.csdn.net/csdn1234/article/details/2589443#)
- import java.lang.reflect.Type;  
- 
- import java.lang.reflect.ParameterizedType;  
- 
- 
- 
- publicclass Test<T> extends ClassA<T>{  
- 
- 
- 
- publicstaticvoid main(String args[]) throws Exception{  
- 
-     System.out.println("======getSuperclass======:");  
- 
-     System.out.println(Test.class.getSuperclass().getName());  
- 
- 
- 
-     System.out.println("======getGenericSuperclass======:");  
- 
-     Type t = Test.class.getGenericSuperclass();  
- 
-     System.out.println(t);  
- 
- if (ParameterizedType.class.isAssignableFrom(t.getClass())) {  
- 
-       System.out.print("----------->getActualTypeArguments:");  
- 
- for (Type t1:((ParameterizedType)t).getActualTypeArguments()) {  
- 
-         System.out.print(t1 + ",");  
- 
-       }  
- 
-       System.out.println();  
- 
-     }  
- 
-   }  
- 
- }  
输出结果:
======getSuperclass======:
ClassA
======getGenericSuperclass======:
ClassA<T>
----------->getActualTypeArguments:T,
