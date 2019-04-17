# java8第一课：lambda表达式 - PeterBishop - CSDN博客





2018年12月07日 20:13:34[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：26
个人分类：[Java学习](https://blog.csdn.net/qq_40061421/article/category/8087498)









  这节课开始我们正式进入 java8 新特性



  首先要讲的就是哦Lambda表达式(拉姆达表达式),lambda表达式就是一个匿名函数，也就是说没有函数名。这个表达式的作用就一点: 简化开发。



  语法(参数表)->{功能代码}

  lambda在java里的唯一作用就是替代匿名内部类，要求是接口只有一个未实现的抽象方法


public class Teach01 {



    public static void main(String[] args) {

        List<String> strList = new ArrayList<>();

        strList.add("asdad");

        strList.add("ansdkanw");

        strList.add("asb");

        strList.add("asbsada");

        strList.add("asbqq");

        strList.add("asbdddffg");



        //下面来看看如何用lamda表达式简化开发

        */**         * **口诀:         * 照搬参数         * 加上右箭头         * 大括号写实现方法         */        *Filter<String> stringFilter = (List<String> list)->{

            List<String> stringList = new ArrayList<>();

            for (String s : list) {

                if (s.length() > 5)

                {

                    stringList.add(s);

                }

            }

            return stringList;

        };

        List<String> strListAfterFilter = stringFilter.filter(strList);

        for (String s : strListAfterFilter) {

            System.*out*.println(s);

        }



    }

}



//过滤器

interface Filter<T>{

    List<T> filter(List<T> list);

}



public class Teach01 {



    public static void main(String[] args) {

        //介绍一个常用案例

        */**         * Thread**有一个构造器是最常用的：         * public Thread(Runnable runnable,String name);         *         * Runnable是一个接口，里面只有一个未实现的抽象方法void run();         */        *Thread thread = new Thread(()->{

            //这里面写的其实就是原来run()方法的代码

            for (int i = 0; i < 10; i++) {

                System.*out*.println(i);

            }

        },"线程1");



        thread.start();

    }



}


