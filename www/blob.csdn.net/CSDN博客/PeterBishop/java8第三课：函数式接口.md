# java8第三课：函数式接口 - PeterBishop - CSDN博客





2018年12月07日 20:15:19[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：19
个人分类：[Java学习](https://blog.csdn.net/qq_40061421/article/category/8087498)









  这节课我们讲讲函数式接口。



  什么叫函数式接口?

  如果一个接口有且仅有一个未实现的方法，我们就说这个接口是函数式接口。


public class FunctionalInterfaceTeach {



    public static void main(String[] args) {

        FuncInterface funcInterface = ()->{};

        funcInterface.init();

    }

}



//在一个接口上加上@FunctionalInterface注解可以让这个接口强行成为函数式接口

//具体表现在如果这个接口不是函数式接口会直接报错

//只有函数式接口才可以在被构造对象时被lambda表达式替换

//函数式接口并不一定要加@FunctionalInterface注解，加这个注解只是保证这个接口是函数式接口而已



@FunctionalInterface

interface FuncInterface{



    void run();



    //注意，函数式接口的要求是这个接口有且仅有一个未实现的抽象方法

    //这意味着你可以写无数个default方法和静态方法



    default void init(){

        System.*out*.println(1);

    }



    static void fun(){}



    default void lalala(){}

}




Java8四大内置函数式接口:

![](https://img-blog.csdnimg.cn/20181207201458688.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)





