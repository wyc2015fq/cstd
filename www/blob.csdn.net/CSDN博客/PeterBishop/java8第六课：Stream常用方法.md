# java8第六课：Stream常用方法 - PeterBishop - CSDN博客





2018年12月07日 20:19:09[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：29








  这节课我们来讲讲stream常用方法


import java.util.ArrayList;

import java.util.List;

import java.util.Optional;

import java.util.stream.Stream;



public class Teach02 {



    */**     * Stream API**使用流程     * 1. 创建流     * 2. 中间操作     * 3. 终止操作     */    *public static void main(String[] args) {

        List<String> stringList =  new ArrayList<>();

        for (int i = 0; i < 10; i++) {

            stringList.add(String.*valueOf*(i));

        }



        //1.创建流

        Stream<String> stream = stringList.stream();



        //2. 中间操作

        //中间操作的返回值类型一定是Stream或Stream的子类

        */**         * limit(int maxsize) **限制操作元素个数，创建一个新的流，取maxsize个元素到新流中，若原流中元素个数小于maxsize，则全部取出即可，然后返回新的流         * map 创建一个新的流，将旧流中的这组元素中的每个元素都作为参数执行Function接口的apply方法然后将返回值加入流中，然后返回新的流         */        *stream = stream.limit(5).map((str)->{return str + "1";});



        //3. 终止操作 返回值类型不为Stream,终止操作不会创建新的流

        //终止操作后此流不可继续使用

        stream.forEach(System.*out*::println);



        Stream<String> stream2 = stringList.stream();



        */**         * reduce() **方法，会将一组数据的前两个元素作为参数传递给apply(Object o1, Object o2)方法，         * 然后将返回的结果和下一个元素继续作为参数继续执行apply知道取到最后一个元素         *         * 原理:         *         * Optional fun(List list)         * {         *      Optional optional = new Optional();         *     int size = list.size();         *     if(size < 2)         *     {         *          optional.set(null);         *          return optional;         *     }         *     Object o1 = list.get(0),o2 = list.get(1);         *     for(int i = 2; i < size; i++)         *     {         *         Object o3 =  apply(o1,o2);         *         o1 = o3;         *         o2 = list.get(i);         *     }         *     optional.set(apply(o1,o2));         *     return optional;         * }         */        *Optional optional = stream2.reduce((s1, s2)->{return s1 + s2;});



        Object o = optional.get();



        System.*out*.println(o);



        Stream<String> stream3 = stringList.stream();



        */**         * filter **过滤，filter会创建一个新的流，然后对旧流里每个元素执行boolean test(T t)方法，         * 如果返回true就将这个元素加入到新流中，否则不加入         */        *stream3 = stream3.limit(5).filter((s)->{return Integer.*parseInt*(s) > 2;});



        stream3.forEach(System.*out*::println);

    }

}








