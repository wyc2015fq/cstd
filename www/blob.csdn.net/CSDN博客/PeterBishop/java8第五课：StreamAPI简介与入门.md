# java8第五课：StreamAPI简介与入门 - PeterBishop - CSDN博客





2018年12月07日 20:18:30[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：26








  这节课开始我们开始学Java8的另一个特性: Stream API

  首先说明，这玩意依然用的不多，但是，还是希望大家知道有这么个东西。



  什么是Stream API? 就是java8给我们提供了一个新的类叫Stream，这个类可以用来很方便的操作一组数据(集合、数组)。



  下面看代码:
import java.util.ArrayList;

import java.util.Arrays;

import java.util.List;

import java.util.Random;

import java.util.stream.Stream;



public class Teach01 {



    public static void main(String[] args) {

        */**         * **先来看看API文档。         * 首先，既然StreamAPI是java8的新特性，所以看java1.6的API文档是看不到的，要看1.8的         *         * 先来看看创建流的几个方法吧:         *         * 1. Collection有个方法stream()可以用来创建流         *         * 2. Arrays.stream()静态方法         *         * 3. Stream.of()静态方法         *         * 4. 无限流         *    通过Stream.iterate()或generat()方法产生无限流         */        *//1. Collection有个方法stream()可以用来创建流

        List<String> list = new ArrayList<>();

        for (int i = 0; i < 10; i++) {

            list.add(String.*valueOf*(i));

        }



        //List是Collection的子接口，自然也有这个stream()方法

        Stream<String> stream1 = list.stream();



        //2. Arrays.stream()静态方法

        String[] strings = new String[]{"1","2","3"};

        Stream<String> stream2 = Arrays.*stream*(strings);



        //3. Stream.of()静态方法'

        Stream<Integer> stream3 = Stream.*of*(1, 2, 3);



        //4. 无限流

        //通过Stream.iterate()或generat()方法产生无限流



        */**         * **@param ****seed **种子         *             此参数将会被作为第二个参数的未实现方法的参数然后不断被执行         * **@param ****f **UnaryOperator**类型         *          UnaryOperator**<T**,T>**继承Function**<T**,R>**接口         *          所以UnaryOperator中未实现的抽象方法为T apply(T t)         *         */        *Stream<Integer> stream4 = Stream.*iterate*(0, (t) -> { return t + 2; });

        */**         * iterate**原理类似如下         *     List iterate(Object seed,UnaryOperator f)         *     {         *          List list = new ArrayList();         *          Object i = seed;         *          int count = 0;         *          while(true)         *          {         *              //这个maxsize就是limit里的，如果不设置就会陷入死循环不断地生成数据         *              if(count >= maxsize)         *              {         *                  break;         *              }         *              list.add(i);         *              i = f.apply(i);         *         *          }         *          return list;         *     }         *         */        /**         * **@Param **s Supplier类型         *            未实现的抽象方法为: T get();         *        generate就是用哪个get()不断的产生数据，同样可以用limit()控制数量         */        *Stream<Integer> stream5 = Stream.*generate*(() -> { return (new Random()).nextInt(); });



        */**         * **接下来介绍常用方法:         * 1. forEach         *      **@Param **action Consumer类型         *      未实现的方法为void accept(T t);         *         * 2. limit()         *      **@Param **maxsize int         *      限制流操作的数据的个数         */        *//stream4.forEach(System.out::println);

        //无限生成，所以叫无限流

        stream4.limit(10).forEach(System.*out*::println);

    }

}




