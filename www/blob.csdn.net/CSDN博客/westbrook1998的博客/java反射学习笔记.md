# java反射学习笔记 - westbrook1998的博客 - CSDN博客





2018年02月19日 18:37:51[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：53








## 反射的定义：

> 
JAVA反射机制是在运行状态中，对于任意一个类，都能够知道这个类的所有属性和方法；对于任意一个对象，都能够调用它的任意方法和属性；这种动态获取信息以及动态调用对象方法的功能称为java语言的反射机制。

Java的反射机制是在编译并不确定是哪个类被加载了，而是在程序运行的时候才加载、探知、自审。使用在编译期并不知道的类。这样的特点就是反射。


***反射就是把java类中的各种成分映射成一个个的Java对象***

{% asset_img 反射1.png 反射 %}

## 三种获取class对象的方法：
- 调用实例化对象的getClass方法
- 直接调用类名.class
- 直接调用Clas类的静态方法forname(包名.类名)

### 代码：

```
package reflex;

public class User {

    public User(){
        System.out.println("无参构造");
    }
    public User(String name){
        System.out.println("一个参数："+name);
    }
    public User(String name,int age){
        System.out.println("两个参数："+name+" "+age);
    }
    private User(int age){
        System.out.println("私有构造方法："+age);
    }

}
------
package reflex;

public class Reflex {
    public static void main(String[] args){
        //第一种方式获取Class对象
        User user1 = new User();
        Class userClass1=user1.getClass();
        System.out.println(userClass1.getName());

        //第二种方法获取Class对象
        Class userClass2=User.class;
        System.out.println(userClass2.getName());
        System.out.println(userClass1==userClass2);

        //第三种方法获取Class对象
        try{
            Class userClass3=Class.forName("reflex.User");
            System.out.println(userClass3.getName());
            System.out.println(userClass2==userClass3);
        }catch (ClassNotFoundException e){
            e.printStackTrace();
        }

        /*
        结果：
        无参构造
        reflex.User
        reflex.User
        true
        class reflex.User
        true
        //比较结果说明只有一个Class类
         */
    }
}
```

## 通过反射获取构造方法、成员变量、普通方法：

### 构造方法

```
package reflex;

public class User {

    public User(){
        System.out.println("无参构造");
    }
    public User(String name){
        System.out.println("一个参数："+name);
    }
    public User(String name,int age){
        System.out.println("两个参数："+name+" "+age);
    }
    private User(int age){
        System.out.println("私有构造方法："+age);
    }

}
------
package reflex;

import java.lang.reflect.Constructor;

public class ConstructorTest{
    public static void main(String[] args) throws Exception{
        //1.加载Class对象
        Class class1=Class.forName("reflex.User");
        //2.获取所有构造方法
        System.out.println("---------所有公有构造方法----------");
        Constructor[] conArray=class1.getConstructors();
        for(Constructor c:conArray){
            System.out.println(c);
        }

        System.out.println("---------所有构造方法----------");
        conArray=class1.getDeclaredConstructors();
        for(Constructor c:conArray){
            System.out.println(c);
        }

        System.out.println("---------公有无参构造犯法----------");
        Constructor con=class1.getConstructor(null);
        System.out.println("con="+con);
        //调用构造方法
        Object obj=con.newInstance();

        System.out.println("----------私有构造方法---------");
        con=class1.getDeclaredConstructor(int.class);
        System.out.println("con="+con);
        //调用构造方法
        con.setAccessible(true);
        obj=con.newInstance(20);
    }
}
```

### 成员变量

```
package reflex;

public class User {

    public String name;
    protected int age;
    char sex;
    private String phoneNum;

    @Override
    public String toString(){
        return "User [name=" + name + ", age=" + age + ", sex=" + sex + ",phoneNum=" + phoneNum + "]";
    }

}
------
package reflex;

import java.lang.reflect.Field;

public class FieldTest {
    public static void main(String[] args) throws Exception{
        //1.获取Class对象
        Class class2=Class.forName("reflex.User");
        //2.获取字段
        System.out.println("---------所有公有字段---------");
        Field[] fieldArray=class2.getFields();
        for(Field f:fieldArray){
            System.out.println(f);
        }

        System.out.println("---------所有字段---------");
        fieldArray=class2.getDeclaredFields();
        for(Field f:fieldArray){
            System.out.println(f);
        }

        System.out.println("---------获取公有字段并调用----------");
        Field f=class2.getField("name");
        System.out.println(f);
        //获取对象
        Object obj=class2.getConstructor().newInstance();
        //为字段设置值
        f.set(obj,"Keane");
        //强制类型转换
        User user=(User)obj;
        System.out.println("验证："+user.name);

        System.out.println("---------获取私有字段并调用----------");
        f=class2.getDeclaredField("phoneNum");
        System.out.println(f);
        f.setAccessible(true);
        //为字段设置值
        f.set(obj,"18888888888");
        //强制类型转换
        System.out.println("验证："+user);
    }
}
```

### 普通方法（或者main方法）

```
package reflex;

public class User {

    public void method1(String s){
        System.out.println("调用了：公有的，String参数的show1(): s = " + s);
    }
    protected void method2(){
        System.out.println("调用了：受保护的，无参的show2()");
    }
    void method3(){
        System.out.println("调用了：默认的，无参的show3()");
    }
    private String method4(int age){
        System.out.println("调用了，私有的，并且有返回值的，int参数的show4(): age = " + age);
        return "abcd";
    }
    }

}
------
package reflex;

import java.lang.reflect.Method;

public class MethodTest {
    public static void main(String[] args) throws Exception {
        //1.获取Class对象
        Class class3 = Class.forName("reflex.User");
        //2.获取公有方法
        System.out.println("----------所有公有方法----------");
        Method[] methodArray = class3.getMethods();
        for (Method m : methodArray) {
            System.out.println(m);
        }
        System.out.println("----------所有的方法----------");
        methodArray = class3.getDeclaredMethods();
        for (Method m : methodArray) {
            System.out.println(m);
        }
        System.out.println("----------公有的method1()方法----------");
        Method m = class3.getMethod("method1", String.class);
        System.out.println(m);
        //实例化一个User对象
        Object obj = class3.getConstructor().newInstance();
        m.invoke(obj, "Keane");

        System.out.println("***************私有的method4()方法******************");
        m = class3.getDeclaredMethod("method4", int.class);
        System.out.println(m);
        m.setAccessible(true);//解除私有限定
        Object result = m.invoke(obj, 20);//需要两个参数，一个是要调用的对象（获取有反射），一个是实参
        System.out.println("返回值：" + result);
    }
}
```

## 反射的其他用法

### 解析配置文件、

略……

### 利用反射跳过泛型检查

原理：泛型在编译过后会被擦除掉

```java
package reflex;

import java.lang.reflect.Method;
import java.util.ArrayList;

public class Demo {
    public static void main(String[] args) throws Exception{
        ArrayList<String> list=new ArrayList<>();
        list.add("abc");
        list.add("efg");
        //错误做法
        //list.add(10);
        //正确做法
        Class listClass= list.getClass();
        Method m=listClass.getMethod("add",Object.class);
        m.invoke(list,10);

        for(Object obj:list){
            System.out.println(obj);
        }

    }
}
```



