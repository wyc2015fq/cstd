# .Net高级技术——结构体 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [.Net高级技术——结构体](https://www.cnblogs.com/xdp-gacl/p/3541036.html)



### 结构体

结构体和类的区别：**结构体是值类型，类是引用类型**

**结构体非常类似于类，但是值类型（拷贝传递），不能被继承**

Int32、DateTime等都是结构体，从ValueType继承，值类型。

**结构体测试程序：**

```
1 using System;
 2 using System.Collections.Generic;
 3 using System.Linq;
 4 using System.Text;
 5 using System.Drawing;
 6 namespace 结构体
 7 {
 8     class Program
 9     {
10         static void Main(string[] args)
11         {
12             Person p1 = new Person();
13             Person p2 = p1;
14             p1.Age = 24;
15             p1.Name = "孤傲苍狼";
16             Console.WriteLine("p1.Name={0},p1.Age={1}", p1.Name, p1.Age);
17             Console.WriteLine("p2.Name={0},p2.Age={1}", p2.Name, p2.Age);
18             p2.Name = "白虎神皇";
19             Console.WriteLine("p1.Name={0},p1.Age={1}", p1.Name, p1.Age);
20             Console.WriteLine("p2.Name={0},p2.Age={1}", p2.Name, p2.Age);
21             Console.WriteLine(object.ReferenceEquals(p1, p2));
22             Console.WriteLine(object.Equals(p1,p2));
23             Console.WriteLine(p1 == p2);
24             MyPoint point1 = new MyPoint();
25             MyPoint point2 = point1;
26             Console.WriteLine(object.ReferenceEquals(point1, point2));//结构体是值类型，拷贝一份，因此point1和point2是两个不同的对象
27             point1.X = 10;
28             point1.Y = 20;
29             point1.SayHello();
30             Console.WriteLine("point1.X={0},point1.Y={1}", point1.X, point1.Y);
31             Console.WriteLine("point2.X={0},point2.Y={1}", point2.X, point2.Y);
32             Console.ReadKey();
33         }
34     }
35     //结构体非常类似于类，但是值类型（拷贝传递），不能被继承
36     //Int32、DateTime等都是结构体，从ValueType继承，值类型。
37     /// <summary>
38     /// 定义结构体
39     /// </summary>
40     struct MyPoint
41     {
42         /// <summary>
43         /// 定义结构体的属性
44         /// </summary>
45         public int X { get; set; }
46         /// <summary>
47         /// 定义结构体的属性
48         /// </summary>
49         public int Y { get; set; }
50         /// <summary>
51         /// 定义结构体的方法
52         /// </summary>
53         public void SayHello()
54         {
55             Console.WriteLine("HelloWorld!");
56         }
57     }
58     /// <summary>
59     /// 定义类
60     /// </summary>
61     class Person
62     {
63         public int Age { get; set; }
64         public string Name { get; set; }
65     }
66 }
```

程序运行结果：

![](https://images0.cnblogs.com/blog/289233/201402/082103313829943.png)













