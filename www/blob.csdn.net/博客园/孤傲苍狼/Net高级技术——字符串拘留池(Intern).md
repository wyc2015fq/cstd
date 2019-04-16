# .Net高级技术——字符串拘留池(Intern) - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [.Net高级技术——字符串拘留池(Intern)](https://www.cnblogs.com/xdp-gacl/p/3541074.html)



### 一、深入理解字符串的不可变特性

string可以看做是char的**只读数组**。char c = s[1]

**C#****中字符串有一个重要的特性：****不可变性，字符串一旦声明就不再可以改变**。所以只能通过索引来读取指定位置的char，不能对指定位置的char进行修改。如果要对char进行修改，那么就必须创建一个新的字符串，用s. ToCharArray()方法得到字符串的char数组，对数组进行修改后，调用new string(char[])这个构造函数来创建char数组的字符串。一旦字符串被创建，那么char数组的修改也不会造成字符串的变化。

**常见疑问：“谁说字符串不可变？****string s1 = "abc";s1="123"****，****s1****这不是变了吗”**，要区分变量名和变量指向的值的区别：程序中可以有很多字符串，然后由字符串变量指向他们，变量可以指向其他的字符串，但是字符串本身没有变化。**字符串不可变性指的是内存中的字符串不可变，而不是变量不变**。
**string s2 = s1;//s2****指向****s1****指向的字符串，而不是****s2****指向****s1**，哪怕s1以后指向了其他内存，那么s2还是指向"123"

内存分配分析：

**“****string s1 = "abc";****”**这句代码执行完之后，在栈内存中开辟一块内存空间存储字符串变量s1，在堆内存中开辟一块内存空间来存储字符串对象“abc”，字符串变量s1指向字符串对象“abc”，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201402/082122165757193.png)

**s1="123";****”**这句代码执行完之后，在堆内存中再开辟一块内存空间来存储字符串对象“123”，字符串变量s1指向字符串对象“123”，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201402/082126180893141.png)

此时可以看到，字符串变量s1已经指向了新的字符串对象“123”了，字符串对象“abc”已经没有任何的字符串变量指向它了，此时字符串对象“abc”就再也没有用了，当对象一定不再有用的时候GC(垃圾收集器)就可以将对象回收了，所以GC会在一个合适的时机来回收字符串对象“abc”，释放其占用的内存资源。**判断一个对象是否一定不再有用的标准就是没有任何的变量指向它**。

**string s2 = s1;** 这句代码执行完之后，在栈内存中开辟一块内存空间存储字符串变量s2，字符串变量s2也指向字符串对象“123”

![](https://images0.cnblogs.com/blog/289233/201402/082134375625384.png)

**例子：将字符串中的****l****替换为****i**

```
1 string s = "hell world!";//字符数组 只读的 不能对其 进行赋值
 2 char[] chars = s.ToCharArray();//得到一个字符数组
 3 chars[1]='i';//修改字符数组的值
 4 s=new string(chars);//生成一个新的字符串
 5 Console.WriteLine(s);
 6 for (int i = 0; i < s.Length; i++)
 7 {
 8    Console.WriteLine(s[i]);
 9 }
10 Console.ReadKey();
```

　　因为**字符串是不可变的**，所以CLR(*CLR*是公共语言运行时，Common Language Runtime)**可能**会将相同值的字符串用同一个实例。程序中大量使用字符串，有不少是重复性的，为了降低内存占用，.Net将代码中**声明的字符串**放到字符串拘留池中，值相同的字符串共享同一个实例。字符串是不变的。**不是所有字符串都在拘留池中**，.Net会判断哪些该放。

**object.ReferenceEquals****方法判断两个变量是不是一个实例（同一个对象）**

```
1 string s1 = "abc";
2 string s2 = "abc";
3 Console.WriteLine(object.ReferenceEquals(s1,s2));
```

**动态字符串默认是不在字符串拘留池中的**

**字符串拘留池测试程序：**

```
1 using System;
 2 using System.Collections.Generic;
 3 using System.Linq;
 4 using System.Text;
 5 using System.Drawing;
 6 namespace 字符串拘留池
 7 {
 8     class Program
 9     {
10         static void Main(string[] args)
11         {
12             //重点1
13             //因为有字符串拘留池的存在所以下面的程序有一个string对象
14             //string s1 = "孤傲苍狼";
15             //string s2 = "孤傲苍狼";
16             string s1 = "孤傲苍狼";
17             string s2 = "孤傲苍狼";
18             if (s1 == s2)//这里比较的是s1和s2的内容是不是一样
19             {
20                 Console.WriteLine("s1==s2的结果是：{0}", s1 == s2);
21             }
22             //上面的代码产生一个字符串对象
23             Console.WriteLine("object.ReferenceEquals(s1,s2)的结果是：{0}",object.ReferenceEquals(s1,s2));//这里判断的是s1和s2所指向的对象是不是同一个
24             Console.WriteLine("================================================");
25             string s3 = "yzk";
26             string s4 = "yzk";
27             string s5 = new string(s1.ToCharArray());
28             //object.ReferenceEquals方法判断两个变量是不是一个实例（同一个对象）
29             Console.WriteLine("object.ReferenceEquals(s3, s4)的结果是：{0}",object.ReferenceEquals(s3, s4));//true
30             Console.WriteLine("object.ReferenceEquals(s3, s5)的结果是：{0}", object.ReferenceEquals(s3, s5));//false 
31             Console.WriteLine("object.ReferenceEquals(s4, s5)的结果是：{0}", object.ReferenceEquals(s4, s5));//false
32             Console.WriteLine("================================================");
33             string s6 = "abc";
34             string s7 = "ab"+"c";
35             Console.WriteLine("object.ReferenceEquals(s6, s7)的结果是：{0}", object.ReferenceEquals(s6, s7));//true
36             //默认只有代码中写的字面才会进入字符串拘留池，动态字符串默认是不在字符串拘留池中的
37             string str1 = "abc";//abc就是代码中写的字面
38             string a1 = Console.ReadLine();
39             string a2 = Console.ReadLine();
40             string str2 = a1 + a2;//str2是动态算出来的字符串，
41             Console.WriteLine("str1==str2的结果是：{0}",str1==str2);//结果为true
42             Console.WriteLine("object.ReferenceEquals(str1, str2)的结果是：{0}", object.ReferenceEquals(str1, str2));//结果为false
43             //重点2
44             //new就一定会创建一个新的对象
45             //两个string对象
46             string str3 = new string("abc".ToCharArray());
47             string str4 = "abc";
48             //==是判断内容是否相等
49             Console.WriteLine("str3==str4的结果是：{0}", str3 == str4);//结果为true
50             Console.WriteLine("object.ReferenceEquals(str3, str4)的结果是：{0}", object.ReferenceEquals(str3, str4));//结果为false
51             Console.WriteLine("================================================");
52             Point p1 = new Point(3, 5);
53             Point p2 = new Point(3, 5);
54             //==是判断内容是否相等
55             Console.WriteLine("p1 == p2的结果是：{0}",p1 == p2);
56             //object.ReferenceEquals判断是否是同一个对象
57             Console.WriteLine("object.ReferenceEquals(p1, p2)的结果是：{0}", object.ReferenceEquals(p1, p2));
58             Console.WriteLine("================================================");
59             Console.ReadKey();
60         }
61     }
62 }
```

程序运行结果：

![](https://images0.cnblogs.com/blog/289233/201402/082141516794390.png)

**string****是不变的，因此每次运算都会重新创建一个****string****对象，**例如：s=s+”abc”;，此时将会创建一个新的字符串对象，s指向了新创建的字符串对象

```
1 string s1=”a”;
2 string s2=”b”;
3 string s3=”c”;
4 string s4=s1+s2+s3;//s1+s2产生"ab","ab"+s3产生"abc"，所以产生两个字符串
```

　　大量的字符串相连会产生大量的中间字符串，字符串是对象，对象的产生是慢的，而且会占用大量的内存。所以要避免大量的字符串的连接操作。

　　例如：在for循环中拼接字符串

```
1 for(int i=1;i<=1000;i++)
2 {
3     s=s+i.ToString();
4 }
```

　　每循环一次，就进行一次字符串拼接运算，那么就会创建一个新的字符串，循环1000次，就会创建1000个字符串，就需要开辟1000块内存空间来存储这些中间字符串，这是非常暂用内存资源的。

　　对象的创建是非常慢和消耗资源的，因此对于需要动态拼接字符串的场合（比如创建SQL语句）用StringBuilder来代替string，StringBuilder内部实现了字符串拼接不会有string的缺陷。













