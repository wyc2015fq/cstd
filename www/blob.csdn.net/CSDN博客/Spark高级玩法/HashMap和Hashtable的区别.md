# HashMap和Hashtable的区别 - Spark高级玩法 - CSDN博客
2018年03月25日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：185

![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVuHicouIiciay0qyTC0hghrW2M5ahIf50tygCNP5a8783soPLwQf3WfQyjL7SXhgNdVKcBA31bmkIibA/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
# HashMap和Hashtable的区别
**1.两者最主要的区别在于Hashtable是线程安全，而HashMap则非线程安全**
Hashtable的实现方法里面都添加了synchronized关键字来确保线程同步，因此相对而言HashMap性能会高一些，我们平时使用时若无特殊需求建议使用HashMap，在多线程环境下若使用HashMap需要使用Collections.synchronizedMap()方法来获取一个线程安全的集合（Collections.synchronizedMap()实现原理是Collections定义了一个SynchronizedMap的内部类，这个类实现了Map接口，在调用方法时使用synchronized来保证线程同步,当然了实际上操作的还是我们传入的HashMap实例，简单的说就是Collections.synchronizedMap()方法帮我们在操作HashMap时自动添加了synchronized来实现线程同步，类似的其它Collections.synchronizedXX方法也是类似原理）
2.HashMap可以使用null作为key，而Hashtable则不允许null作为key
虽说HashMap支持null值作为key，不过建议还是尽量避免这样使用，因为一旦不小心使用了，若因此引发一些问题，排查起来很是费事
HashMap以null作为key时，总是存储在table数组的第一个节点上
3.HashMap是对Map接口的实现，HashTable实现了Map接口和Dictionary抽象类
4.HashMap的初始容量为16，Hashtable初始容量为11，两者的填充因子默认都是0.75
HashMap扩容时是当前容量翻倍即:capacity*2，Hashtable扩容时是容量翻倍+1即:capacity*2+1
5.两者计算hash的方法不同
Hashtable计算hash是直接使用key的hashcode对table数组的长度直接进行取模
int hash = key.hashCode();int index = (hash & 0x7FFFFFFF) % tab.length;
HashMap计算hash对key的hashcode进行了二次hash，以获得更好的散列值，然后对table数组长度取摸
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVuHicouIiciay0qyTC0hghrW2BQ6LXR7ADHj0Mup2MZeKhsn79HfdakPxKjeXyIUm8J0luFcdjk26DA/640?wx_fmt=png)
6.HashMap和Hashtable的底层实现都是数组+链表结构实现
# HashSet和HashMap、Hashtable的区别
除开HashMap和Hashtable外，还有一个hash集合HashSet，有所区别的是HashSet不是key value结构，仅仅是存储不重复的元素，相当于简化版的HashMap，只是包含HashMap中的key而已
通过查看源码也证实了这一点，HashSet内部就是使用HashMap实现，只不过HashSet里面的Ha11shMap所有的value都是同一个Object而已，因此HashSet也是非线程安全的，至于HashSet和Hashtable的区别，HashSet就是个简化的HashMap的，所以你懂的
下面是HashSet几个主要方法的实现
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVuHicouIiciay0qyTC0hghrW2TZcMjoyZYCuzibRypcqs2XicEBMkZklsxjRHibyMby6MGaENmufTBSt0A/640?wx_fmt=png)
# HashMap和Hashtable的实现原理
HashMap和Hashtable的底层实现都是数组+链表结构实现的，这点上完全一致
添加、删除、获取元素时都是先计算hash，根据hash和table.length计算index也就是table数组的下标，然后进行相应操作，下面以HashMap为例说明下它的简单实现
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVuHicouIiciay0qyTC0hghrW2JK7wibFTVFdAoaZfHibCibhX8o9TxY29V7aA48LpGCoZndf2HPPsxUjyQ/640?wx_fmt=png)
**HashMap的创建**
HashMap默认初始化时会创建一个默认容量为16的Entry数组，默认加载因子为0.75，同时设置临界值为16*0.75
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVuHicouIiciay0qyTC0hghrW2EDMreOcKmrM70JbV9AnIhRvXd4suetNygVm6ht48xtrTLkxU4dY6NQ/640?wx_fmt=png)
**put方法**
HashMap会对null值key进行特殊处理，总是放到table[0]位置
put过程是先计算hash然后通过hash与table.length取摸计算index值，然后将key放到table[index]位置，当table[index]已存在其它元素时，会在table[index]位置形成一个链表，将新添加的元素放在table[index]，原来的元素通过Entry的next进行链接，这样以链表形式解决hash冲突问题，当元素数量达到临界值(capactiy*factor)时，则进行扩容，是table数组长度变为table.length*2
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVuHicouIiciay0qyTC0hghrW2nua8LDicyRyJ6HVziaJQia5HDna3Padc4qJ66c9Uht9QZTbJNjqTibPicHg/640?wx_fmt=png)
**get方法**
同样当key为null时会进行特殊处理，在table[0]的链表上查找key为null的元素
get的过程是先计算hash然后通过hash与table.length取摸计算index值，然后遍历table[index]上的链表，直到找到key，然后返回
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVuHicouIiciay0qyTC0hghrW2MrtmbTZR8K2S1sLHAMSOW3QgELPAtoJaKfBZq1I1KQJjQOE2sc3YOw/640?wx_fmt=png)
**remove方法**
remove方法和put get类似，计算hash，计算index，然后遍历查找，将找到的元素从table[index]链表移除
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVuHicouIiciay0qyTC0hghrW2mOJ4L8Co7IAWnZJqu6ZXX4ISAQn3YZiaUpxctQxOvhlzsvPEXC1schA/640?wx_fmt=png)
**resize方法**
resize方法在hashmap中并没有公开，这个方法实现了非常重要的hashmap扩容，具体过程为：先创建一个容量为table.length*2的新table，修改临界值，然后把table里面元素计算hash值并使用hash与table.length*2重新计算index放入到新的table里面
这里需要注意下是用**每个元素的hash全部重新计算index**，而不是简单的把原table对应index位置元素简单的移动到新table对应位置
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVuHicouIiciay0qyTC0hghrW2NdT1bwWpJVfibialia09sAs5uicCXIFs5XmicjTc1EruFymt5j2F5kQKj0w/640?wx_fmt=png)
**clear()方法**
clear方法非常简单，就是遍历table然后把每个位置置为null，同时修改元素个数为0
需要注意的是clear方法只会清楚里面的元素，并不会重置capactiy
containsKey和containsValue
containsKey方法是先计算hash然后使用hash和table.length取摸得到index值，遍历table[index]元素查找是否包含key相同的值
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVuHicouIiciay0qyTC0hghrW2ARdFHoT6xuguGxFVKHycOickerS058m8rVxAcT4iaF73ibghPtXMFWUFQ/640?wx_fmt=png)
containsValue方法就比较粗暴了，就是直接遍历所有元素直到找到value，由此可见HashMap的containsValue方法本质上和普通数组和list的contains方法没什么区别，你别指望它会像containsKey那么高效
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVuHicouIiciay0qyTC0hghrW2KXo4TQLibUeUDJwGbzSmic52lLqGGnMnuUpOlsN0Qs3icLopcEzNVQA4A/640?wx_fmt=png)
**hash和indexFor**
indexFor中的h & (length-1)就相当于h%length，用于计算index也就是在table数组中的下标
hash方法是对hashcode进行二次散列，以获得更好的散列值
为了更好理解这里我们可以把这两个方法简化为 int index= key.hashCode()/table.length,以put中的方法为例可以这样替换
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVuHicouIiciay0qyTC0hghrW2AgdkB5pR76BSqwqONOVqfz1ia95vZ2rt8m6G34AFRzCWVMZK5jcIB0w/640?wx_fmt=png)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFVuHicouIiciay0qyTC0hghrW25vxYnOdJ1KL6e3C1erkLZds9rAzXz9lApAKgFIRSYqa2JBeCkscIwQ/640?wx_fmt=png)
本文转载自：http://www.cnblogs.com/lzrabbit/p/3721067.html#h1
**推荐阅读：**
1，[垂直拆分和水平拆分](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484614&idx=1&sn=3a995b98596f02a48bbf5f6ed22a469c&chksm=9f38e7eea84f6ef8a3cab9291e54eacf7758451d72e9e5601f316364b02a0adf076d3e0f1b4d&scene=21#wechat_redirect)
2，[Java动态代理原理及解析](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484556&idx=1&sn=7ef7d0de2216698eb5b5d8dbf08fdcb0&chksm=9f38e7a4a84f6eb23ffc5b6d6247b8b1e79df57ebbf02402f8a198243764677b0fbee680a70c&scene=21#wechat_redirect)
3，[超越Spark，大数据集群计算的生产实践](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484395&idx=1&sn=82ae3b4fd460d4666d46aa2348f540c6&chksm=9f38e0c3a84f69d5b0d871133d9f94c3612c7a98e9dc93ac74d0f2ccce12a13f18207cbd9ce0&scene=21#wechat_redirect)
4，[JAVA程序员面试总结，高手整理加强版](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484457&idx=1&sn=550d4c58e05c021602cef7a93ed0ba8b&chksm=9f38e701a84f6e179cd3760fdfef8fb240fcdc8cd31af049621680a9be5af5376b428e6d492f&scene=21#wechat_redirect)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640?wx_fmt=png)
