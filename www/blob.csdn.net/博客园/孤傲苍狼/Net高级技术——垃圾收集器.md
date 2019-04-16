# .Net高级技术——垃圾收集器 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [.Net高级技术——垃圾收集器](https://www.cnblogs.com/xdp-gacl/p/3541050.html)



## 垃圾收集器概述

　　大排档和学校食堂。一个是别人帮你收拾盘子，一个是自己收拾盘子。

　　垃圾收集**GC****（****Garbage Collection****）**。内存的分配、回收不需要程序员操心，程序员只要需要的时候new就可以，用完了不用管，有人帮你清除不用的内存。这个人就是“垃圾收集器”。**.Net****程序员一般不用像****C++****程序员那样处理内存的申请和回收，是因为有****GC**。

GC优点：程序员不用关心内存的申请和回收，缺点：不用的内存不能立即得到回收（**GC****并不会实时去回收不用的内存，而是找时机才回收**）。

　　当对象一定不再有用的时候GC就可以将对象回收了（悲观！）。**判断一个对象是否一定不再有用的标准就是没有任何的变量指向它**。当一个变量设置为null的时候。

```
1 Person p1=new Person(“lilei”);//在内存中创建了Person对象
 2 Person p2 = p1;//把p2指向p1指向的对象！这一刻p1指向着leilei，p2就顺着p1找到了lilei
 3 p1=null;//p1不再指向lilei 。lilei不能回收，因此p2还在指着它
 4 p2= new Person(“hanmeimei”);//lilei可以回收，因为p2已经指向了另外一个对象，此时再也没有任何变量指着它。
 5 --------------------------------------------------------------------------------------------------------------------------
 6 DataSet ds1 = new dataset();
 7 ds2=ds1;
 8 ds1=null;
 9 ds2=null;//new dataset()的对象已经悬空，没有任何变量能再指向它，他就不能再用了 --------------------------------------------------------------------------------------------------------------------------
10 void Main() {
11 Person p1 = new Person();
12 p2 = p1;//让p2指向p1指向的对象。
13 Console.WriteLine("hello");
14 p1.SayHello();
15 p1=null;// p1指向的对象不可以被回收，因为还有p2指向它
16 p2=null;//可以被回收，没有任何对象指向它
17 p3=...
18 }
19 --------------------------------------------------------------------------------------------------------------------------
20 void Hello() {
21 Person p1 = new Person();
22 p1.SayHello();
23 }//出了Hello，p1指向的对象就可以被回收了
24 private Person myp;
25 void Hello1() {
26 Person p1 = new Person();
27 p1.Sayhello();
28 myp=p1;
29 }
30 //出了hello1，p1指向的对象还不会被回收。因为还有myp指向它
```

**当一个对象一旦被最后一个指向它的变量抛弃的时候****(****“变量****=null****”或者变量出了作用域****)****，不可能再有任何变量指向它，它就像断了线的风筝，再也飞不回来了，因此就可以被回收了。当一个对象找不回来的时候就可以被****GC****了。**

## 垃圾收集GC其他说明

**不会再被使用的内存（对象）就是垃圾。**

对象可以被回收**不一定会立即回收**，GC就像清洁工，不是有了垃圾就去清扫，否则GC累，而且清扫垃圾就影响系统的正常工作(服务员收盘子的时候都会影响旁边的人吃饭)。**频繁的****GC****（垃圾回收）造成系统性能下降，不是有了垃圾就清理。**垃圾收集器会找一个合适的时机去回收。

GC.Collect();//服务员，收一下！就会立即进行内存的回收了

GC类中提供了对GC进行控制、监测的方法，比如**GC.Collect()****可以强制立即开始进行****GC**，但是一般不要去手动干预GC。没有特殊理由，不要去调用GC.Collect()，让.net自己决定什么时候去回收内存。

 WeakReference，通过WeakReference来引用一个对象就可以知道引用的对象是否还活着，即有没有被GC回收，WeakReference就相当于一本生死簿，记录着指向的对象的生死。

```
1 Person p1 = new Person();
2 WeakReference wr = new WeakReference(p1);//弱引用
3 wr.IsAlive;// 判断指向的对象是否还活着
```

**注意：**GC只能回收**托管（****Managed）**内存资源，对于**数据库连接、文件句柄、****Socket连接**等这些资源**（非托管资源，****UnManaged）**就无能为力，必须程序员自己控制资源的回收。非托管资源：SqlConnection、FileStream等。

**sqlconnection也是在net里创建的，为什么就和别的不一样呢，new Person()只是占用对象的内存资源，而new sqlconnection()则除了占用内存资源，还占用了一个到数据库的连接资源，这个是GC管不了的。如果conn不用了，没有Dispose，那么conn对象占用的内存也会被回收，但是数据库连接资源不会。**

**垃圾收集器测试代码：**

```
1 using System;
 2 using System.Collections.Generic;
 3 using System.Linq;
 4 using System.Text;
 5 namespace GC垃圾回收器
 6 {
 7     class Program
 8     {
 9         static void Main(string[] args)
10         {
11             Person p1 = new Person();
12             WeakReference wr = new WeakReference(p1);//弱引用
13             p1 = null;//p1设置为null之后，此时已经没有任何变量指向Person对象了，此时Person对象就等着被GC回收了
14             Console.WriteLine("我是Person1对象，我还活着吗，WeakReference(生死簿)上显示说是：" + (wr.IsAlive ? "Yes" : "NO"));//判断指向的对象是否还活着
15             GC.Collect();//调用垃圾回收器回收那些没有任何变量指向的对象了，不写可能IsAlive还为true
16             Console.WriteLine("我是Person1对象，我还活着吗，WeakReference(生死簿)上显示说是：" + (wr.IsAlive ? "Yes" : "NO"));//判断指向的对象是否还活着
17             Console.WriteLine("================================================");
18             //通过WeakReference来引用一个对象就可以知道引用的对象是否还活着，即有没有被GC回收
19             //WeakReference就相当于一本生死簿，记录着指向的对象的生死
20             WeakReference wr1 = null;
21             Person p2 = new Person();
22             wr1 = new WeakReference(p2);
23             Console.WriteLine("我是Person2对象，我还活着吗，WeakReference(生死簿)上显示说是：" + (wr1.IsAlive ? "Yes" : "NO"));//判断指向的对象是否还活着
24             GC.Collect();//调用垃圾回收器回收那些没有任何变量指向的对象了，不写可能IsAlive还为true
25             Console.WriteLine("我是Person2对象，我还活着吗，WeakReference(生死簿)上显示说是：" + (wr1.IsAlive ? "Yes" : "NO"));//判断指向的对象是否还活着
26             Console.ReadKey();
27         }
28     }
29     class Person { }
30 }
```

**运行效果：**

**![](https://images0.cnblogs.com/blog/289233/201402/082115540781049.png)**









