## 如何找出字符串的字典序全排列的第N种

**1.题目**

如何找出字符串的字典序全排列的第N种？（字符串全排列的变种）

**2.思路**

主要想通过这题，介绍一下康托展开式。基于康托展开式可以解决这个问题。

一般的解法：①求出所有全排列 ②按照字典序排个序 ③取第N个

**3.康托展开与逆展开**

康托展开是一个全排列到一个自然数的双射，常用于构建哈希表时的空间压缩。 康托展开的实质是计算当前排列在所有由小到大全排列中的顺序，因此是可逆的。（[引用](http://www.acmerblog.com/cantor-expansion-3647.html)）

**3.1公式**
X=a[n]*(n-1)!+a[n-1]*(n-2)!+…+a[i]*(i-1)!+…+a[1]*0!

其中,a[i]为整数,并且0<=a[i]<i,1<=i<=n。

a[i]的意义参见举例中的解释部分

**3.2举例**
例如，3 5 7 4 1 2 9 6 8 展开为 98884。因为X=2*8!+3*7!+4*6!+2*5!+0*4!+0*3!+2*2!+0*1!+0*0!=98884.

**解释：**

排列的第一位是3，比3小的数有两个，以这样的数开始的排列有8!个，因此第一项为2*8!

排列的第二位是5，比5小的数有1、2、3、4，由于3已经出现，因此共有3个比5小的数，这样的排列有7!个，因此第二项为3*7!

以此类推，直至0*0!

**伪代码实现**

① Cantor(A, n) 求一个字符数组的康托值

```
1 Cantor(A, n)
2 for i←0 to n-1
3     result ← result + Less(A[i]) * F[i]
4 return result
```

定义：

- A：待求康托值的字符数组
- n：字符数组长度，如公式中的n
- F：阶乘的结果集，如公式中(n-1)!、i!、2!、1!、0!
- Less：函数，求比自己小的数的个数，如公式中的a[i]的意义

②Less(n, set) 求比自己小的数的个数，公式中a[i]

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
1 Less(n, set)
2 for(m : set )
3     if m < n
4         count ← count+1
5 add(set, n)
6 return n - count -1
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

定义：

- n：待求比自己小的数
- set：存放已经出现过的数
- count：比3小的数有1,2，如果1,2在set中出现了，count就计数这个。
- 返回值：-1的目的是为了得到a[i]

**3.3用途**

显然，n位（0~n-1）全排列后，其康托展开唯一且最大约为n!，因此可以由更小的空间来储存这些排列。由公式可将X逆推出对应的全排列。

**3.4康托展开的逆运算**
既然康托展开是一个双射，那么一定可以通过康托展开值求出原排列，即可以求出n的全排列中第x大排列。

如n=5,x=96时：

1. 首先用96-1得到95，说明x之前有95个排列.(将此数本身减去！)
2. 用95去除4! 得到3余23，说明有3个数比第1位小，所以第一位是4.
3. 用23去除3! 得到3余5，说明有3个数比第2位小，所以是4，但是4已出现过，因此是5.
4. 用5去除2!得到2余1，类似地，这一位是3.
5. 用1去除1!得到1余0，这一位是2.
6. 最后一位只能是1.
7. 所以这个数是45321.

**伪代码实现**

①ResolveCantor(A, X, n)：给第X种，求该全排列n的字符串

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
1 ResolveCantor(A, X, n)
2 for i←0 to n-1
3     a ← X div F[i]
4     b ← X mod F[i]
5     A[i] ← Solve(a, visit)
6     X ← b
7 return A
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

定义：

- A：存储字符串的结果
- X：字典序全排列的X种（0,1,2,3,...），这个值是康托值
- n：字符数组长度，如康托公式中的n
- F：阶乘的结果集，如公式中(n-1)!、i!、2!、1!、0!
- Solve：函数，求某个输出字符

②Solve(a, visit)：求某个输出字符

```
1 Solve(a, visit)
2 while a is visited
3     a← a+1
4     see a is visited or not
5 return a +1
```

定义：

- a：康托公式中的a[i]
- visit：boolean数组，visit[a]表示a是否已经出现过了。
- 返回值：+1 为了构建出输出字符

如果用这个算法去求字符串的全排列，时间复杂度是O(n3)，优于递归算法的O(n!)。

**3.5 Java代码实现**

为了实现简单一些，实现部分采用的是int数组。



[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 import java.util.HashSet;
 2 import java.util.Set;
 3 
 4 public class Cantor {
 5 
 6     public static final int LEN = 3;
 7     private static int[] f = new int[LEN];
 8     private static Set<Integer> set = new HashSet<Integer>();
 9     private static boolean[] visit = new boolean[LEN];
10 
11     static {
12         int re = 1;
13         for (int i = 1; i < LEN; i++) {
14             re *= i;
15             f[LEN - 1 - i] = re;
16         }
17         f[LEN - 1] = 1;
18         for (int i = 0; i < LEN; i++) {
19             visit[i] = false;
20         }
21         System.out.println("F[0]: " + f[0]);
22     }
23 
24     public static void main(String[] args) {
25         int[] a = { 2, 1, 3 };
26         int n = a.length;
27         int x = cantor(a, n);
28         String str = "";
29         for (int i = 0; i < n; i++) {
30             str += "" + a[i];
31         }
32         System.out.println("src String: " + str);
33         System.out.println("cantor value: " + x);
34         int[] b = new int[LEN];
35         resolveCantor(b, x, n);
36         str = "";
37         for (int i = 0; i < n; i++) {
38             str += "" + b[i];
39         }
40         System.out.println("resolve cantor str: " + str);
41     }
42 
43     static int cantor(int[] a, int n) {
44         int result = 0;
45         for (int i = 0; i < n; i++) {
46             result += less(a[i]) * f[i];
47         }
48         return result;
49     }
50 
51     private static int less(int n) {
52         int count = 0;
53         for (Integer m : set) {
54             if (m < n)
55                 count++;
56         }
57         set.add(n);
58         return n - count - 1;
59     }
60 
61     static int[] resolveCantor(int[] arr, int x, int n) {
62         int a, b;
63         for (int i = 0; i < n; i++) {
64             a = x / f[i];
65             b = x % f[i];
66             arr[i] = solve(a);
67             System.out.println("a: " + a + " b: " + b + " arr[i]: " + arr[i]);
68             x = b;
69         }
70         return arr;
71     }
72 
73     private static int solve(int a) {
74         boolean flag = true;
75         while (flag) {
76             if (visit[a] == false) {
77                 visit[a] = true;
78                 flag = false;
79             } else {
80                 a++;
81             }
82         }
83         return a + 1;
84     }
85 }
```

