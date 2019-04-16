# java8第二课：接口里可以写什么 - PeterBishop - CSDN博客





2018年12月07日 20:14:25[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：25
个人分类：[Java学习](https://blog.csdn.net/qq_40061421/article/category/8087498)









  这节课我们来讲讲java8里接口可以写什么，再次强调，这种语法只在jdk1.8以上有效。


public class Java8InterfaceTeach {



    public static void main(String[] args) {

        //这里会报错是因为这个接口里没有未实现的抽象方法

        //要想使用lambda表达式，接口里必须有且仅有一个未实现的抽象方法

        Java8Interface java8Interface = new Java8Interface() {};

        System.*out*.println(java8Interface.add(1,2));

        System.*out*.println(Java8Interface.*add*("12","21"));

    }

}



interface Java8Interface{



    */**     * default**方法:     * java8里允许在接口里实现方法，不过要加default关键字，这样的方法不算抽象方法，就和普通方法一样     * 继承该接口的接口或实现该接口的类可以不必重写该方法，当然如果想要重写也没有问题     */    *default int add(int i1, int i2)

    {

        return i1 + i2;

    }



    */**     * **接口里还可以写静态方法     */    *static String add(String s1, String s2)

    {

        return s1 + s2;

    }

}



*/** * **如果一个类实现了两个接口，而这两个接口中有方法名和参数表完全相同的默认实现方法(就是加了default的方法) * 那么这个类必须重写这个方法 */*class Test implements interface1,interface2{



    @Override

    public int add(int i1, int i2) {

        return i1 + i2;

    }

}



interface interface1{



    default int add(int i1, int i2)

    {

        return i1 + i2;

    }



}



interface interface2{

    default int add(int i1, int i2)

    {

        return i1 + i2 + 1;

    }

}




