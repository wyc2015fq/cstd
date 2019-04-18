# java 通过反射获取泛型的类型 - z69183787的专栏 - CSDN博客
2014年12月25日 10:20:35[OkidoGreen](https://me.csdn.net/z69183787)阅读数：7057
个人分类：[泛型](https://blog.csdn.net/z69183787/article/category/2736483)
jdk1.5开始支持泛型，所以我们有时需要把泛型里定义的对象的类型拿到 
比如现在我定义了三个类Account， AccountItem和Product类。 
Account聚合AccountItem，AccountItem聚合Prodcut。 
都是用List<AccountItem>和List<Product>来表示的 
我要实现一个功能，需要动态的传入一个Class的类型，然后反射到启动的List，根据List里定义的泛型，知道其中List的具体对象。 
这个需求主要是由于现在的Json-lib还不支持深度的List的unmarshall，而只支持数组的方式。其实这里就是json-lib的beanfactory用到ezmorpher，而ezmorpher不支持泛型的定义方式，所以不知道类型，全转成MorpherDynBean，这样的对象是我们不需要的。 
这样需要修改ezmorpher的代码，注入自己的MorpherBean的processor，这个processor就根据泛型拿到，我们需要转型的对象。 
代码片段如下 
Java代码  ![收藏代码](http://lorry1113.iteye.com/images/icon_star.png)
-  Field[] fs = clazz.getDeclaredFields(); // 得到所有的fields
- 
- for(Field f : fs)   
- {   
-     Class fieldClazz = f.getType(); // 得到field的class及类型全路径
- 
- if(fieldClazz.isPrimitive())  continue;  //【1】 //判断是否为基本类型
- 
- if(fieldClazz.getName().startsWith("java.lang")) continue; //getName()返回field的类型全路径；
- 
- if(fieldClazz.isAssignableFrom(List.class)) //【2】
-     {   
-              Type fc = f.getGenericType(); // 关键的地方，如果是List类型，得到其Generic的类型
- 
- if(fc == null) continue;  
- 
- if(fc instanceof ParameterizedType) // 【3】如果是泛型参数的类型 
-             {   
-                    ParameterizedType pt = (ParameterizedType) fc;  
- 
-                    Class genericClazz = (Class)pt.getActualTypeArguments()[0]; //【4】 得到泛型里的class类型对象。
- 
-                    m.put(f.getName(), genericClazz);  
- 
-                    Map<String, Class> m1 = prepareMap(genericClazz);  
- 
-                    m.putAll(m1);   
-              }   
-       }   
- }  
【解释】： 
1、isPrimitive 
public boolean isPrimitive()判定指定的 Class 对象是否表示一个基本类型。 
有九种预定义的 Class 对象，表示八个基本类型和 void。这些类对象由 Java 虚拟机创建，与其表示的基本类型同名，即 boolean、byte、char、short、int、long、float 和 double。 【注：像Integer，Boolean等包装类不是基本类型！】 
这些对象仅能通过下列声明为 public static final 的变量访问，也是使此方法返回 true 的仅有的几个 Class 对象。  
返回： 当且仅当该类表示一个基本类型时，才返回 true 
从以下版本开始：JDK1.1 
2、isAssignableFrom 
Java代码  ![收藏代码](http://lorry1113.iteye.com/images/icon_star.png)
- System.out.println(String.class.isAssignableFrom(Object.class));     
2.//打印true   
    AA.class.isAssignableFrom(BB.class)的作用是判定AA表示的类或接口是否同参数BB指定的类表示的类或接口相同，或AA是否是BB的父类。 
Java代码  ![收藏代码](http://lorry1113.iteye.com/images/icon_star.png)
-   System.out.println( String.class.isAssignableFrom(Object.class) ) ;       false
- System.out.println( Object.class.isAssignableFrom(Object.class) );       true
- System.out.println( Object.class.isAssignableFrom(String.class) );        true
- String ss = "";     
- System.out.println( ss instanceof Object );                                            true
- Object o = new   Object();  
- System.out.println( o instanceof Object );                                              true
3、ParameterizedType 表示参数化类型，如 Collection<String>。 
4、getGenericSuperclass, getSuperclass, getActualTypeArguments 
     说明 
           1.Class<? super T> getSuperclass():返回本类的父类 
           2.Type getGenericSuperclass():返回本类的父类,包含泛型参数信息 
      例子 
          1.ClassA.java 
Java代码  ![收藏代码](http://lorry1113.iteye.com/images/icon_star.png)
- publicclass  ClassA <T>{       
- private T obj;       
- publicvoid setObject(T obj) {      this.obj = obj;  }     
- public T getObject() {    return obj;   }       
- }    
         2.Test.java 
Java代码  ![收藏代码](http://lorry1113.iteye.com/images/icon_star.png)
- 
- import java.lang.reflect.Type;      
- import java.lang.reflect.ParameterizedType;     
- publicclass Test<T> extends ClassA<T>{     
- private List<String> list;     
- 
- publicvoid testA(){  
-         Type t = Test.class.getDeclaredField("list").getGenericType();  
- if (ParameterizedType.class.isAssignableFrom(t.getClass())) {              
- for (Type t1:((ParameterizedType)t).getActualTypeArguments()) {          
-                 System.out.print(t1 + ",");          
-             }          
-             System.out.println();          
-         }   
-    }  
- publicstaticvoid main(String args[]) throws Exception{       
-             System.out.println("======getSuperclass======:");      
-             System.out.println(Test.class.getSuperclass().getName());     
-             System.out.println("======getGenericSuperclass======:");     
-             Type t = Test.class.getGenericSuperclass();      
-             System.out.println(t);       
- if (ParameterizedType.class.isAssignableFrom(t.getClass())) {    
-                      System.out.print("----------->getActualTypeArguments:");       
- for (Type t1:((ParameterizedType)t).getActualTypeArguments()) {       
-                                     System.out.print(t1 + ",");       
-                       }       
-                      System.out.println();       
-             }    
-    }     
- 
-   }    
              输出结果: 
class java.lang.String, 
                    ======getSuperclass======: 
                    ClassA 
                    ======getGenericSuperclass======: 
                    ClassA<T> 
                    ----------->getActualTypeArguments:T,
