# Stopwatch的使用 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [Stopwatch的使用](https://www.cnblogs.com/xdp-gacl/p/3525487.html)



```
1 using System;
 2 using System.Collections.Generic;
 3 using System.Linq;
 4 using System.Text;
 5 using System.Threading.Tasks;
 6 
 7 namespace Stopwatch的使用
 8 {
 9     class Program
10     {
11       
12         /// <summary>
13         /// 使用Stopwatch比较for循环和foreach循环的效率
14         /// </summary>
15         /// <param name="args"></param>
16         static void Main(string[] args)
17         {
18 
19             int[] intArr = new int[1000000];
20             for (int i = 1; i <= 1000000; i++)
21             {
22                 intArr[i - 1] = i;
23             }
24 
25             //使用Stopwatch统计程序运行的时间
26             /*
27              Stopwatch提供了几个方法用以控制Stopwatch对象。
28              * Start方法开始一个计时操作，Stop方法停止计时。
29              * 此时如果第二次使用 Start方法，将继续计时，最终的计时结果为两次计时的累加。
30              * 为避免这种情况，在第二次计时前用Reset方法将对象归零。这三个方法都不需要参数
31              */
32             System.Diagnostics.Stopwatch sw = new System.Diagnostics.Stopwatch(); 
33             sw.Start(); //开始计时
34 
35             int sum = 0;
36 
37             for (int i = 1; i <= intArr.Length; i++)
38             {
39                 sum += i;
40             }
41             Console.WriteLine("sum={0}",sum);
42            
43             sw.Stop(); //停止计时
44             long result = sw.ElapsedMilliseconds;//获得程序运行的时间
45             Console.WriteLine("使用for循环计算从1加到1000000的和所需要的时间是：{0}", result);
46 
47             Console.WriteLine("****************************************************************");
48             sw.Reset();
49             sw.Start();
50 
51             sum = 0;
52             foreach (var item in intArr)
53             {
54                 sum += item;
55             }
56             Console.WriteLine("sum={0}", sum);
57             Console.WriteLine("使用foreach循环计算从1加到1000000的和所需要的时间是：{0}", sw.ElapsedMilliseconds);
58             sw.Stop();
59 
60             Console.ReadKey();
61         }
62     }
63 }
```











