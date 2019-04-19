# java中泛型的使用 - Big Smile - CSDN博客
2017年06月11日 21:42:24[王啸tr1912](https://me.csdn.net/tr1912)阅读数：327标签：[java																[泛型](https://so.csdn.net/so/search/s.do?q=泛型&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[Java学习](https://blog.csdn.net/tr1912/article/category/6453602)
所属专栏：[java学习](https://blog.csdn.net/column/details/26775.html)
        小编这几天在搞java的代码的时候用到了泛型这个集合，但是在使用的时候发现他和我们之前在.net环境中使用的时候还是有很大差别的，在此分享给大家。
        Ps.泛型的知识小编之前有写过，可以参考之前的博客。
# 一、java中的List接口
        我们可以很清楚的在java的代码中找到，List<>是一个接口，众所周知，接口并不能直接实例化，所以我们不能写诸如“List<entity> myEntity = new List<entity>();”的代码了，那么这个时候我们要怎么写呢？
![](https://img-blog.csdn.net/20170611212450105?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        如果了解了java基础的同学我们应该知道，在java中的集合类型中，有哈希set，枚举,数组，还有List这种集合，但是我们在学习的时候接触的最多的就是ArryList了，这个ArrayList是有序的元素对象集合，还有一种List集合是LinkList，这两种集合类型都是List的实现，所以我们在使用List的时候，是不可以实例化List的，必须实例化一个List的实现，也就是一个ArryList，或者LinkList。
# 二、原因
        因为List接口实现了Collection接口，所以List接口拥有Collection接口提供的所有常用方法，又因为List是列表类型，所以他有很多方法可以进行使用：
![](https://img-blog.csdn.net/20170611213508571?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdHIxOTEy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        List接口提供的适合于自身的常用方法均与索引有关，这是因为List集合为列表类型，以线性方式存储对象，可以通过对象的索引操作对象。
        List接口的常用实现类有ArrayList和LinkedList，在使用List集合时，通常情况下声明为List类型，实例化时根据实际情况的需要，实例化为ArrayList或LinkedList，例如：
List<String> l = new ArrayList<String>();// 利用ArrayList类实例化List集合
List<String> l2 = new LinkedList<String>();// 利用LinkedList类实例化List集合
        但是为什么要用接口和实现来用List这个集合类型呢？
       首先我们都知道有里氏代换原则这个设计模式存在，这个原则就是讲的父类可以被子类所替换，但是所有子类又逃离不了父类，所以这里的List接口就是相当于父类的一个存在，他的下面可以有很多的实现（子类），从而达到上面的List可以变为ArrayList或者LinkList的目的。
        并且还有一点，在实际使用的时候后面实例化的具体实现的尖括号里面是不用写实体类型的，如：
List<entity> myList = new ArrayList<>();

