# Java深入 - Java反射机制详解 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2014年06月22日 20:18:59[initphp](https://me.csdn.net/initphp)阅读数：2005
所属专栏：[Java深入](https://blog.csdn.net/column/details/myjava.html)









Java反射机制Reflection，是在程序运行的时候能够获取和改变类的属性、方法等信息。

因为反射的动态控制类的特性，所以大多被用在框架开发中等。

我主要通过实例的方式来展现Java反射的常用方法。

Java 反射机制主要提供了以下功能：

1. 在运行时判断任意一个对象所属的类;

2. 在运行时构造任意一个类的对象;

3. 在运行时判断任意一个类所具有的成员变量和方法;

4. 在运行时调用任意一个对象的方法;

5. 生成动态代理。

在 JDK 中，主要由以下类来实现Java 反射机制，这些类都位于java.lang.reflect

包中。

Class类：代表一个类。

Field类：代表类的成员变量(成员变量也称为类的属性)。

Method类：代表类的方法。

Constructor 类：代表类的构造方法。

Array类：提供了动态创建数组，以及访问数组元素的静态方法。


### 1. 获取类的包名称

Demo类



```java
package com.xxx.test;


/**
 * 一个测试的Demo类
 * @author zhuli
 * @date 2014-6-22
 */
public class Demo {

    private Integer age;

    private String  userName;

    public Integer getAge() {
        return age;
    }

    public void setAge(Integer age) {
        this.age = age;
    }

    public String getUserName() {
        return userName;
    }

    public void setUserName(String userName) {
        this.userName = userName;
    }

    @Override
    public String toString() {
        return "Demo [age=" + age + ", userName=" + userName + "]";
    }

}
```







Java main：





```java
package com.xxx.test;


public class JavaTest {

    public static void main(String args[]) {
        //第一种方法
        Class<?> demoClass = Demo.class; //通过 类名.class 直接获取类的Class
        System.out.println("Demo.class的方式获取包名：" + demoClass.getName());

        //第二种方法
        Demo demo = new Demo();
        Class<?> demoClass2 = demo.getClass(); //通过对象的getClass获取类的Class
        System.out.println("demo.getClass()的方式获取包名：" + demoClass2.getName());

        //第三种方法
        Class<?> demoClass3 = null;
        try {
            demoClass3 = Class.forName("com.xxx.test.Demo");
        } catch (Exception e) {

        }
        System.out.println("Class.forName(\"Demo\")的方式获取包名：" + demoClass3.getName());
    }
}
```






### 2. 获取类的方法信息

通过getDeclaredMethods()方法就可以获取到类中的方法信息。

通过getDeclaredFields()方法获取类的域信息



```java
package com.xxx.test;

import java.lang.reflect.Field;
import java.lang.reflect.Method;


public class JavaTest {

    public static void main(String args[]) {
        //获取类提供的方法
        Class<?> demoClass = Demo.class; //通过 类名.class 直接获取类的Class
        Demo demo = null;
        //获取类的域信息
        Field[] fields = demoClass.getDeclaredFields();
        for (int i = 0; i < fields.length; i++) {
            System.out.println(fields[i].getName() + fields[i].getType());
        }
        //获取方法信息
        Method methods[] = demoClass.getDeclaredMethods();
        for (int i = 0; i < methods.length; i++) {
            System.out.println(methods[i]);
        }
    }
}
```





输出：



```java
ageclass java.lang.Integer
userNameclass java.lang.String
public java.lang.Integer com.xxx.test.Demo.getAge()
public void com.xxx.test.Demo.setAge(java.lang.Integer)
public void com.xxx.test.Demo.setUserName(java.lang.String)
public java.lang.String com.xxx.test.Demo.toString()
public java.lang.String com.xxx.test.Demo.getUserName()
```







### 3. 实例化一个类

```java
package com.xxx.test;


public class JavaTest {

    public static void main(String args[]) {
        //获取类提供的方法
        Class<?> demoClass = Demo.class; //通过 类名.class 直接获取类的Class
        Demo demo = null;
        try {
            demo = (Demo) demoClass.newInstance();
        } catch (InstantiationException e) {
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            e.printStackTrace();
        }
        demo.setAge(20);
        demo.setUserName("initphp");
        System.out.println(demo.toString());
        
    }
}
```


输出：



Demo [age=20, userName=initphp]




### 4. 通过反射设置私有属性值

```java
package com.xxx.test;

import java.lang.reflect.Field;


public class JavaTest {

    public static void main(String args[]) throws Exception {
         //获取类提供的方法
        Class<?> demoClass = Demo.class; //通过 类名.class 直接获取类的Class
        Demo demoObj = (Demo) demoClass.newInstance();
        //设置类的属性值
        Field field = demoClass.getDeclaredField("age");
        field.setAccessible(true); //设置私有属性范围  
        field.set(demoObj, 20);
        
        Field field2 = demoClass.getDeclaredField("userName");
        field2.setAccessible(true);
        field2.set(demoObj, "initphp");
        System.out.println("获取一个值：" + field2.get(demoObj));
        
        System.out.println(demoObj.toString());
    }
}
```


输出：



获取一个值：initphp

Demo [age=20, userName=initphp]



### 5. 通过反射调用类的方法



```java
package com.xxx.test;

import java.lang.reflect.Method;


public class JavaTest {

    public static void main(String args[]) throws Exception {
        //获取类提供的方法
        Class<?> demoClass = Demo.class; //通过 类名.class 直接获取类的Class
        Demo demoObj = (Demo) demoClass.newInstance();
        
        //调用类的方法
        Method method = demoClass.getMethod("setAge", Integer.class); //获取类方法信息
        method.invoke(demoObj, 100); //调用方法
        Method method2 = demoClass.getMethod("getAge");
        System.out.println("Age:" + method2.invoke(demoObj));
        
        Method method3 = demoClass.getMethod("setUserName", String.class);
        method3.invoke(demoObj, "initphp");
        Method method4 = demoClass.getMethod("getUserName");
        System.out.println("UserName:" + method4.invoke(demoObj));
        
        System.out.println(demoObj.toString());
    }
}
```


输出：



Age:100

UserName:initphp

Demo [age=100, userName=initphp]




### 6. 获取父类/interface/构造函数等信息

新增Demo的接口/父类



```java
package com.xxx.test;


public class People {
    
    private String sex;

    public String getSex() {
        return sex;
    }

    public void setSex(String sex) {
        this.sex = sex;
    }

}
package com.xxx.test;


public interface Test {

}
package com.xxx.test;


/**
 * 一个测试的Demo类
 * @author zhuli
 * @date 2014-6-22
 */
public class Demo extends People implements Test {

    public Demo() {
    }

    public Demo(String userName, Integer age) {
        this.age = age;
        this.userName = userName;
    }

    private Integer age;

    private String  userName;

    public Integer getAge() {
        return age;
    }

    public void setAge(Integer age) {
        this.age = age;
    }

    public String getUserName() {
        return userName;
    }

    public void setUserName(String userName) {
        this.userName = userName;
    }

    @Override
    public String toString() {
        return "Demo [age=" + age + ", userName=" + userName + "]";
    }

}
```



例子：



```java
package com.xxx.test;


import java.lang.reflect.Constructor;

public class JavaTest {

    public static void main(String args[]) throws Exception {
        //获取类提供的方法
        Class<?> demoClass = Demo.class; //通过 类名.class 直接获取类的Class

        //获取构造函数信息
        Constructor<?> ctorlist[] = demoClass.getDeclaredConstructors();
        for (int i = 0; i < ctorlist.length; i++) {
            Constructor<?> ct = ctorlist[i];
            System.out.print("构造函数 = " + ct.getName());
            Class<?> pvec[] = ct.getParameterTypes(); //获取参数类型。数组形式
            for (int j = 0; j < pvec.length; j++) {
                System.out.print(" 参数： " + pvec[j]);
            }
            System.out.println("");

            //通过构造函数实例化对象
            if (pvec.length > 0) {
                Demo demo = (Demo) ct.newInstance("zhul", 10);
                System.out.println("通过构造函数(Demo) ct.newInstance(\"zhul\", 10)实例化：" + demo.toString());
            }
        }

        //获取父类信息
        Class<?> temp = demoClass.getSuperclass(); //Java只能单继承，所以只有一个父类
        System.out.println("父类名称：" + temp.getName());

        //获取interface信息
        Class<?> interfaces[] = demoClass.getInterfaces(); //Java可以支持多个接口，所以是数组
        for (int i = 0; i < interfaces.length; i++) {
            System.out.println("实现的接口   " + interfaces[i].getName());
        }

    }
}
```




输出：



构造函数 = com.xxx.test.Demo

构造函数 = com.xxx.test.Demo 参数： class java.lang.String 参数： class java.lang.Integer

通过构造函数(Demo) ct.newInstance("zhul", 10)实例化：Demo [age=10, userName=zhul]

父类名称：com.xxx.test.People

实现的接口   com.xxx.test.Test





























