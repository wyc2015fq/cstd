# Java —— 内部类（Inner类） - Alex_McAvoy的博客 - CSDN博客





2018年10月05日 16:48:59[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：47
个人分类：[Java学习笔记](https://blog.csdn.net/u011815404/article/category/8863016)

所属专栏：[Java 学习笔记](https://blog.csdn.net/column/details/34317.html)









内部类即为在其他类内部定义的子类，根据位置、是否被 static 修饰、是否有类名分为以下四种。

# 1.成员内部类

同成员变量一样，属于类的全局成员，可以用权限修饰符、static、final、abstract 修饰，在其中可以调用外部类的属性、方法，还可在其中定义内部类。

创建非静态内部类的对象时，必须先创建外部类的对象，通过外部类的对象调用内部类的构造器

```java
class A{    //外部类
    int a;
    String b;
    class B{    //非静态内部类
        ...
    }
}
class Main{
    public static void main(String[] args){
        //创建非静态内部类对象
        A obj=new A();    //创建外部类对象
        A.B obj1=obj.new B();    //通过外部类对象创建非静态内部类对象
    }
}
```

# 2.静态内部类

与静态变量类似，使用 static 进行修饰，被整个类所共享，创建静态内部类的对象时，可以直接通过外部类来调用静态内部类的构造器。

```java
class A{    //外部类
    int a;
    String b;
    static class B{    //静态内部类
    }
}
class Main{
    public static void main(String[] args){
        //创建静态内部类对象
        A.B obj2=new A.B();
    }
}
```

# 3.局部内部类

同局部变量一样，仅在方法内部有效，当一个方法返回值为某个类或接口的对象，通常使用局部内部类

```java
class A{
    int a;
    String b;
    public void method(){
        class B{    //局部内部类
            public int method(){
                return 1;
            }
        }

        return new B();    //返回一个实现类的对象
    }
}
```

# 4.匿名内部类

没有名称的内部类，常用于创建接口的唯一实现类或某个类的唯一子类，其对象只可调用一次

```java
interface Apple{            //定义Apple接口
    public void say();      //定义say()方法
}
public class Sample{                          //创建Sample类
    public static void print(Apple apple){    //创建print()方法
        apple.say();
    }
}
public static void main(String[] args){
    Sample.print(new Apple()){                //为print()方法的传递
        public void say(){                    //实现Apple接口
            System.out.println("这是一个苹果") //匿名类做参数
        }
    }
}
```



