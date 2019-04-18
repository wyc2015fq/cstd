# Timus 1108. Heritage - weixin_33985507的博客 - CSDN博客
2008年06月23日 16:34:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
[Timus 1108. Heritage](http://acm.timus.ru/problem.aspx?space=1&num=1108) 要求对给定的若干继承人作出一个遗产分配方案。
## 1108. Heritage
Time Limit: 2.0 second
Memory Limit: 16 MB
Your
rich uncle died recently, and the heritage needs to be divided among
your relatives and the church (your uncle insisted in his will that the
church must get something). There are *N* relatives (*N* ≤
18) that were mentioned in the will. They are sorted in descending
order according to their importance (the first one is the most
important). Since you are the computer scientist in the family, your
relatives asked you to help them. They need help, because there are
some blanks in the will left to be filled. Here is how the will looks:
Relative #1 will get 1  / ... of the whole heritage, 
Relative #2 will get 1  / ... of the whole heritage,
...
Relative #n will get 1 /  ... of the whole heritage.
The logical desire of the relatives
is to fill the blanks in such way that the uncle’s will is preserved
(i.e the fractions are non-ascending and the church gets something) and
the amount of heritage left for the church is minimized.
### Input
The only line of input contains the single integer *N* (1 ≤ *N* ≤ 18).
### Output
Output the numbers that the blanks need to be filled (on separate lines), so that the heritage left for the church is minimized.
### Sample
|input|output|
|----|----|
|2|2            3|
**Problem Author: **Pavlin Peev
解答如下：
```
1 using System;
 2 
 3 namespace Skyiv.Ben.Timus
 4 {
 5   // http://acm.timus.ru/problem.aspx?space=1&num=1108
 6   sealed class T1108
 7   {
 8     static void Main()
 9     {
10       Console.WriteLine(2);
11       int n = int.Parse(Console.ReadLine());
12       BigInteger a = new BigInteger(2);
13       for (int i = 2; i <= n; i++)
14       {
15         BigInteger b = a + 1;
16         Console.WriteLine(b);
17         if (i < n) a *= b;
18       }
19     }
20   }
21 
22   sealed class BigInteger
23   {
24     int[] digits = new int[30000];
25 
26     public BigInteger(int n)
27     {
28       digits[0] = n;
29       if (digits[0] > 9) Format();
30     }
31 
32     public BigInteger(BigInteger x)
33     {
34       Array.Copy(x.digits, digits, digits.Length);
35     }
36 
37     public static BigInteger operator +(BigInteger x, int y)
38     {
39       BigInteger z = new BigInteger(x);
40       z.digits[0] += y;
41       if (z.digits[0] > 9) z.Format();
42       return z;
43     }
44 
45     public static BigInteger operator *(BigInteger x, BigInteger y)
46     {
47       BigInteger z = new BigInteger(0);
48       int xmax = x.digits.Length - 1;
49       int ymax = y.digits.Length - 1;
50       while (xmax >= 0 && x.digits[xmax] == 0) xmax--;
51       while (ymax >= 0 && y.digits[ymax] == 0) ymax--;
52       for (int xi = xmax; xi >= 0; xi--)
53         for (int yi = ymax; yi >= 0; yi--)
54           z.digits[xi + yi] += x.digits[xi] * y.digits[yi];
55       z.Format();
56       return z;
57     }
58 
59     void Format()
60     {
61       for (int quotient = 0, i = 0; i < digits.Length; i++)
62       {
63         int numerator = digits[i] + quotient;
64         quotient = numerator / 10;
65         digits[i] = numerator % 10;
66       }
67     }
68 
69     public override string ToString()
70     {
71       int n = digits.Length - 1;
72       while (n >= 0 && digits[n] == 0) n--;
73       if (n < 0) return "0";
74       char[] cs = new char[n + 1];
75       for (int i = n; i >= 0; i--) cs[i] = (char)(digits[n - i] + '0');
76       return new string(cs);
77     }
78   }
79 }
```
这个题目是说，你受命分配一份遗产，共有* N* (1 ≤ *N* ≤ 18)
个继承人，每个继承人将得到整个遗产的若干分之一，且后面的继承人分到遗产的份额不能多于前面的继承人。还有，遗产不能分光，必须捐一部分给教堂。你的任
务是给出一个分配方案使教堂得到的份额最小。其实，这题说穿了也很简单，按以下数列来分配就行了：
a1 = 2， an = (an-1 - 1) * an-1 + 1    (n > 1)
该数列的除第一项外的每一项是其前面所有项的乘积加一，这里的加一就是为了给教堂留一份遗产。令：
Tn = 1 - ( 1/a1 + ... + 1 /an )
很容易证明，Tn = 1 / ( an+1 - 1 )，就是捐给教堂的遗产的份额。如果不加一的话，Tn 就会为零。
这个数列的前几项是：
2, 3, 7, 43, 1807, 3263443, 10650056950807, 113423713055421844361000443,
12864938683278671740537145998360961546653259485195807
关键是这个数列增长太快了，第 18 项居然大于 1026680 。所以需要自己写个简单的 BigInteger 类来计算。
