# java8第四课：方法引用和构造器引用 - PeterBishop - CSDN博客





2018年12月07日 20:17:40[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：17








  这节课我们开始讲方法引用和构造器引用，直接看代码


public class MethodReference {



    */**     * **方法引用:一种lambda表达式的简写方式     * 语法:     *  对象::普通方法名     *  类::普通方法名     *  类::静态方法名     */    *public static void main(String[] args) {

        //d对象::普通方法名

        I1 i1 = System.*out*::println;

        i1.fun("123");

        */**         * **解释:         * 这个就相当于:         * I1 i1 = (str)->System.out.println(str);         *         * 限制: 未实现的方法的返回值类型和参数类型必须和被引用的方法的返回值类型和参数类型一致         */        *I2 i2 = String::startsWith;

        */**         * **解释:         * 上面这个就相当于:         * I2 i2 = (str1,str2)->return str1.startsWith(str2);         */        *I3<String> i3 = String::new;

        */**         * **上面这个叫构造器引用，语法为 类::new         * 上面这句相当于:         * I3 i3 = ()->return new String();         *         * 注意:         * 使用构造器引用时未被实现的抽象方法的参数表要和对应构造器的参数表相同，返回类型必须就是构造器对应的类         */        *String s = i3.newInstance();//可以看到返回的是String



        */**         * **最后说明，方法引用和构造器引用用的人并不多，你写出来别人还可能看不懂，所以知道就行，要用还是用原生的lambda表达式         */    *}

}



@FunctionalInterface

interface I1{

    void fun(String str);

}



@FunctionalInterface

interface I2{

    boolean fun(String str1, String str2);

}



@FunctionalInterface

interface I3<T>{

    T newInstance();

}




