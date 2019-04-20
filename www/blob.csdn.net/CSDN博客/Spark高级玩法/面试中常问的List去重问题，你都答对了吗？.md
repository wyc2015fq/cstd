# 面试中常问的List去重问题，你都答对了吗？ - Spark高级玩法 - CSDN博客
2018年12月13日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：183
转载自微信公众号【Java知音】
原文：https://mp.weixin.qq.com/s/AAeaY1-0xclmMKBXp7XVUA
面试中经常被问到的list如何去重，用来考察你对list数据结构，以及相关方法的掌握，体现你的java基础学的是否牢固。
我们大家都知道，set集合的特点就是没有重复的元素。如果集合中的数据类型是基本数据类型，可以直接将list集合转换成set，就会自动去除重复的元素，这个就相对比较简单。
如下示例：
```
public class Test {
    public static void main(String[] args) {
        List list = new ArrayList();
        list.add(11);
        list.add(12);
        list.add(13);
        list.add(14);
        list.add(15);
        list.add(11);
        System.out.println(list);
        Set set = new HashSet();
        List newList = new ArrayList();
        set.addAll(list);
        newList.addAll(set);
        System.out.println(newList);
    }
}
```
输出结果为：
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/JfTPiahTHJhrTexNIibsrb0cyw2OEAQ8icV4Nuv9S1QPHB4P8XWz2HZkRpvqHMPlPEdSr2AKqLlXaInL7Osy8mVNA/640?)
我们可以看到去重成功了。
在面试中被问到list去重问题，大部分回答都会是list和set互转，利用set自动去除重复属性的方法去重，但是这样的回答并不会得分。
当list集合中存储的类型是对象类型的时候，我们就不能简单的只把list集合转换成set集合。
我们定义一个对象类：
```
public class People {
    private String name;
    private String phoneNumber;
    public String getName() {
        return name;
    }
    public void setName(String name) {
        this.name = name;
    }
    public String getPhoneNumber() {
        return phoneNumber;
    }
    public void setPhoneNumber(String phoneNumber) {
        this.phoneNumber = phoneNumber;
    }
    public People(String name, String phoneNumber) {
        super();
        this.name = name;
        this.phoneNumber = phoneNumber;
}
    @Override
    public String toString() {
        return "People{" +
                "name='" + name + '\'' +
                ", phoneNumber='" + phoneNumber + '\'' +
                '}';
    }
}
```
我们使用上面的set去重的方法去重：
```
public static void main(String[] args) {
    List<People> listPeople = new ArrayList<People>();
    listPeople.add(new People("张三", "11111"));
    listPeople.add(new People("张三",  "22222"));
    listPeople.add(new People("李四",  "33333"));
    listPeople.add(new People("张三",  "22222"));
    Set<People> setData = new HashSet<People>();
    setData.addAll(listPeople);
    System.out.println("list：" + listPeople.toString());
    System.out.println("set：" + setData.toString());
}
```
运行后输出的结果为：
![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/JfTPiahTHJhrTexNIibsrb0cyw2OEAQ8icVGXRdBEqZKwxUglBFcNpUV2YNK1t7FpA3VEHnUQfiaz3OM3mPYdYxmAQ/640?)
我们可以看到第二个张三和最后一个张三，信息都一样，却没有被去重。
当list集合中存储的是对象时，我们需要在对象的实体类中去重写equals()方法和hashCode()方法，如下：
```
public class People {
    private String name;
    private String phoneNumber;
    public String getName() {
        return name;
    }
    public void setName(String name) {
        this.name = name;
    }
    public String getPhoneNumber() {
        return phoneNumber;
    }
    public void setPhoneNumber(String phoneNumber) {
        this.phoneNumber = phoneNumber;
    }
    public People(String name, String phoneNumber) {
        super();
        this.name = name;
        this.phoneNumber = phoneNumber;
}
    @Override
    public String toString() {
        return "People{" +
                "name='" + name + '\'' +
                ", phoneNumber='" + phoneNumber + '\'' +
                '}';
    }
    @Override
    public boolean equals(Object arg0) {
        // TODO Auto-generated method stub
        People p = (People) arg0;
        return name.equals(p.name) && phoneNumber.equals(p.phoneNumber);
    }
    @Override
    public int hashCode() {
        // TODO Auto-generated method stub
        String str = name + phoneNumber;
        return str.hashCode();
    }
}
```
此时运行上面的test方法，去重就成功了。
最后，我们拿出String中的equals()方法和hashCode()方法源码来加深认识：
**equals()**
```
public boolean equals(Object anObject) {
        if (this == anObject) {
            return true;
        }
        if (anObject instanceof String) {
            String anotherString = (String)anObject;
            int n = count;
            if (n == anotherString.count) {
                char v1[] = value;
                char v2[] = anotherString.value;
                int i = offset;
                int j = anotherString.offset;
                while (n-- != 0) {
                    if (v1[i++] != v2[j++])
                        return false;
                }
                return true;
            }
        }
        return false;
    }
```
比较两个对象时，首先先去判断两个对象是否具有相同的地址，如果是同一个对象的引用，则直接放回true；如果地址不一样，则证明不是引用同一个对象，接下来就是挨个去比较两个字符串对象的内容是否一致，完全相等返回true，否则false。
**hashCode()**
```
public int hashCode() {
        int h = hash;
        if (h == 0 && count > 0) {
            int off = offset;
            char val[] = value;
            int len = count;
            for (int i = 0; i < len; i++) {
                h = 31*h + val[off++];
            }
            hash = h;
        }
        return h;
    }
```
##### hashCode()官方定义：
hashcode方法返回该对象的哈希码值。支持该方法是为哈希表提供一些优点，例如，java.util.Hashtable 提供的哈希表。
##### hashCode 的常规协定是：
在 Java 应用程序执行期间，在同一对象上多次调用 hashCode 方法时，必须一致地返回相同的整数，前提是对象上 equals 比较中所用的信息没有被修改。从某一应用程序的一次执行到同一应用程序的另一次执行，该整数无需保持一致。
如果根据 equals(Object) 方法，两个对象是相等的，那么在两个对象中的每个对象上调用 hashCode 方法都必须生成相同的整数结果。
以下情况不是必需的：如果根据 equals(java.lang.Object) 方法，两个对象不相等，那么在两个对象中的任一对象上调用 hashCode 方法必定会生成不同的整数结果。但是，程序员应该知道，为不相等的对象生成不同整数结果可以提高哈希表的性能。
实际上，由 Object 类定义的 hashCode 方法确实会针对不同的对象返回不同的整数。（这一般是通过将该对象的内部地址转换成一个整数来实现的，但是 JavaTM 编程语言不需要这种实现技巧。）
当equals方法被重写时，通常有必要重写 hashCode 方法，以维护 hashCode 方法的常规协定，该协定声明相等对象必须具有相等的哈希码。
当然，List去重的方法很多，可以用for循环或者使用java8新特性stream等等，欢迎讨论！
**推荐阅读**
[面试 | Java8 HashMap原理](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486032&idx=1&sn=99bbcb444e347ba30faa42f20b43d7df&chksm=9f38e978a84f606e6c44811cdf24f62417cd3ad7cf3ce0e4f06b1a39dbc6628bac5f09ebc4b5&scene=21#wechat_redirect)
[你注意ArrayList扩容原理了吗](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486017&idx=1&sn=b0e9987dd4a4319db00f62b631e87c50&chksm=9f38e969a84f607f87867d80a04602ea0a64b41fae272ad2df94a449dfa48540066c0cb81fbb&scene=21#wechat_redirect)
[源码:Spark SQL 分区特性第一弹](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486061&idx=1&sn=3662feca085f66e7a686aa6e627d60ec&chksm=9f38e945a84f60538936a1ecfc462127cddb7cfc44bda4a8d1ed4f3300e6906f733917f83634&scene=21#wechat_redirect)
[Spark Streaming 场景应用](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486069&idx=1&sn=26117da71923993cc4e49717196bb247&chksm=9f38e95da84f604b36c8cb69ed43208799dcfb2b10f5f93dbcf93a5a75482062cced60109363&scene=21#wechat_redirect)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640)
