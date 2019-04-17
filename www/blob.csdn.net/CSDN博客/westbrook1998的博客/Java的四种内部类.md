# Java的四种内部类 - westbrook1998的博客 - CSDN博客





2018年03月28日 19:37:42[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：105








代码如下：

```
package com.zxc.innerclass;

public class OuterClass2 {
    private String str;
    private static String name="keane";

    public static class StaticInnerClass{

        //静态内部类里可以有静态属性或者非静态属性
        private static String string="inner_Keane";

        //静态内部类可以访问外部类的静态属性和方法
        //但不可以访问非静态属性和方法
        public void display(){
            System.out.println("外部类静态属性："+name+"内部类静态属性："+string);
        }

    }

    public class InnerClass{

        //非静态内部类里只能有非静态属性
        private String string="primary_Inner_Keane";

        //非静态内部类里可以访问外部类的所有属性和方法
        public void display(){
            System.out.println("外部类静态属性："+name+"外部类非静态属性："+str+"内部类非静态属性："+string);
        }
    }

    public void display(){

        //直接访问静态内部类的静态属性
        System.out.println(StaticInnerClass.string);
        //无法直接访问非静态内部类的属性
        //System.out.println(InnerClass.string);

        //无需依赖外部类直接创建静态内部类实例
        new StaticInnerClass().display();

        //需要先创建外部类再创建内部类实例
        InnerClass inner=new OuterClass2().new InnerClass();
        inner.display();

        //局部内部类是定义在方法里面或者作用域里面
        class newClass{
            private int num=5;

            public int fun(){
                return num;
            }
        }

        System.out.println(new newClass().fun());

        //匿名内部类
        new Test() {
            @Override
            public void fun() {
                System.out.println("匿名内部类");
            }
        }.fun();
    }

    public static void main(String[] args) {
        OuterClass2 outer=new OuterClass2();
        outer.display();
    }
}

interface Test{
    void fun();
}
```



