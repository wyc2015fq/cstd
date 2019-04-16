# 2018年EC Final 校内选拔赛【解题报告】 - 紫芝的博客 - CSDN博客





2018年11月03日 17:12:19[紫芝](https://me.csdn.net/qq_40507857)阅读数：741








## 问题 A: C基础-求同存异

时间限制: 1 Sec  内存限制: 128 MB

提交: 1  解决: 1

[[提交](http://sustoj.com/JudgeOnline/submitpage.php?cid=1030&pid=0&langmask=262128)][[状态](http://sustoj.com/JudgeOnline/problemstatus.php?id=1051)][[讨论版](http://sustoj.com/JudgeOnline/bbs.php?pid=1051&cid=1030)][命题人:外部导入][[Edit](http://sustoj.com/JudgeOnline/admin/problem_edit.php?id=1051&getkey=7D7EF15DA1)] [TestData]

## 题目描述

输入两个数组（数组元素个数6和8），输出在两个数组中都出现的元素（如a[6]={234567}b[8]={357911131519}则输出3、5、7）。

## 输入

第一行输入a数组，第二行输入b数组

## 输出

输出在两个数组中都出现的元素

## 样例输入

```
2 3 4 5 6 7
3 5 7 9 11 13 15 19
```

## 样例输出

```
3
5
7
```

```java
import java.util.*;
import java.math.*;

public class Main{
	static int maxn=(int)(5e4+10);
	public static void main(String[] args) {
		Scanner cin=new Scanner(System.in);
		LinkedList<Integer> a=new LinkedList<Integer>();
		ArrayList b=new ArrayList<Integer>();
		
		String A=cin.nextLine();
		int lenA=A.length();
		int num=0;
		for(int i=0;i<lenA;i++) {
			while(i<lenA) {
				num*=10;
				num+=(A.charAt(i)-'0');
				i++;
				if(i<lenA&&A.charAt(i)==' ') {
					a.add(num);
					num=0;
					break;
				}
			}
		}
		a.add(num);
		
		String B=cin.nextLine();
		int lenB=B.length();
		num=0;
		for(int i=0;i<lenB;i++) {
			while(i<lenB) {
				num*=10;
				num+=(B.charAt(i)-'0');
				i++;
				if(i<lenB&&B.charAt(i)==' ') {
					b.add(num);
					num=0;
					break;
				}
			}
		}
		b.add(num);

		ArrayList<Integer> ans=new ArrayList<Integer>();
		for(int i=0;i<a.size();i++) {
			boolean flag=false;
			for(int j=0;j<b.size();j++) {
				if(a.get(i)==b.get(j)) {
					ans.add(a.get(i));
					break;
				}
			}
		}
		
		for(int i=0;i<ans.size();i++) {
			System.out.println(ans.get(i));
		}
		cin.close();
	}
}
```

## 问题 B: C基础-对角线和

时间限制: 1 Sec  内存限制: 128 MB

提交: 2  解决: 2

[[提交](http://sustoj.com/JudgeOnline/submitpage.php?cid=1030&pid=1&langmask=262128)][[状态](http://sustoj.com/JudgeOnline/problemstatus.php?id=1050)][[讨论版](http://sustoj.com/JudgeOnline/bbs.php?pid=1050&cid=1030)][命题人:外部导入][[Edit](http://sustoj.com/JudgeOnline/admin/problem_edit.php?id=1050&getkey=C104FAD2F9)] [TestData]

## 题目描述

求一个3×3矩阵对角线元素之和。

## 样例输入

```cpp
1 2 3
4 5 6
7 8 9
```

## 样例输出
`15`
```java
import java.util.*;
import java.math.*;

public class Main{
	static int maxn=(int)(5e4+10);
	public static void main(String[] args) {
		Scanner cin=new Scanner(System.in);
		int ans=0;
		for(int i=1;i<=3;i++)
			for(int j=1;j<=3;j++) {
				int x=cin.nextInt();
				if(i==j)	ans+=x;
			}
		System.out.println(ans);		
		cin.close();
	}
}
```

## 问题 C: C基础-局部求合

时间限制: 1 Sec  内存限制: 128 MB

提交: 9  解决: 4

[[提交](http://sustoj.com/JudgeOnline/submitpage.php?cid=1030&pid=2&langmask=262128)][[状态](http://sustoj.com/JudgeOnline/problemstatus.php?id=1049)][[讨论版](http://sustoj.com/JudgeOnline/bbs.php?pid=1049&cid=1030)][命题人:外部导入][[Edit](http://sustoj.com/JudgeOnline/admin/problem_edit.php?id=1049&getkey=CD81A3220A)] [TestData]

## 题目描述

输入20个整数，输出其中能被数组中其它元素整除的那些数组元素。

## 样例输入
`2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21`
## 样例输出

```
4
6
8
9
10
12
14
15
16
18
20
21
```

```java
import java.util.*;
import java.math.*;

public class Main{
	static int maxn=(int)(5e4+10);
	public static void main(String[] args) {
		Scanner cin=new Scanner(System.in);
	
		ArrayList<Integer> a=new ArrayList<Integer>();
		ArrayList<Integer> ans=new ArrayList<Integer>();
		for(int i=0;i<20;i++) {
			int x=cin.nextInt();
			a.add(x);
		}
		//a.sort(null);
		for(int i=0;i<20;i++) {
			for(int j=0;j<20;j++) {
				if(i!=j&&a.get(i)%a.get(j)==0) {
					ans.add(a.get(i));
					break;
				}
			}
		}
		for(int i=0;i<ans.size();i++)
			System.out.println(ans.get(i));
		cin.close();
	}
}
```



## 问题 D: 杨辉三角

时间限制: 1 Sec  内存限制: 128 MB

提交: 1  解决: 1

[[提交](http://sustoj.com/JudgeOnline/submitpage.php?cid=1030&pid=3&langmask=262128)][[状态](http://sustoj.com/JudgeOnline/problemstatus.php?id=1657)][[讨论版](http://sustoj.com/JudgeOnline/bbs.php?pid=1657&cid=1030)][命题人:外部导入][[Edit](http://sustoj.com/JudgeOnline/admin/problem_edit.php?id=1657&getkey=D6D3999E39)] [TestData]

## 题目描述

还记得中学时候学过的杨辉三角吗？具体的定义这里不再描述，你可以参考以下的图形： 

1 

1 1 

1 2 1 

1 3 3 1 

1 4 6 4 1 

1 5 10 10 5 1

## 输入

输入数据包含多个测试实例，每个测试实例的输入只包含一个正整数n（1<=n<=30），表示将要输出的杨辉三角的层数

## 输出

对应于每一个输入，请输出相应层数的杨辉三角，每一层的整数之间用一个空格隔开，每一个杨辉三角后面加一个空行。

## 样例输入
`2 3`
## 样例输出

```
1
1 1

1
1 1
1 2 1
```

```java
import java.util.*;
import java.math.*;

public class Main{
	static int maxn=(int)(5e4+10);
	static long[][] f=new long[32][32];
	static void init() {
		f[1][1]=1;
		for(int i=2;i<=30;i++) {
			for(int j=1;j<=i;j++) {
				f[i][j]=f[i-1][j-1]+f[i-1][j];
			}
		}
	}
	public static void main(String[] args) {
		Scanner cin=new Scanner(System.in);
		init();
		int ca=1;
		while(cin.hasNext()) {
			int n=cin.nextInt();
			if(ca!=1)
				System.out.println();
			ca++;
			for(int i=1;i<=n;i++) {
				for(int j=1;j<=i;j++) {
					System.out.print(f[i][j]);
					if(i==j)
						System.out.println();
					else
						System.out.print(" ");
				}
			}
		}
		cin.close();
	}
}
```

## 问题 E: Amon 爱游戏

时间限制: 1 Sec  内存限制: 128 MB

提交: 1  解决: 1

[[提交](http://sustoj.com/JudgeOnline/submitpage.php?cid=1030&pid=4&langmask=262128)][[状态](http://sustoj.com/JudgeOnline/problemstatus.php?id=1919)][[讨论版](http://sustoj.com/JudgeOnline/bbs.php?pid=1919&cid=1030)][命题人:[winsoul](http://sustoj.com/JudgeOnline/userinfo.php?user=winsoul)][[Edit](http://sustoj.com/JudgeOnline/admin/problem_edit.php?id=1919&getkey=CC60FC3B10)] [TestData]

## 题目描述

Amon 非常喜欢和 Congcong 玩游戏。

现在有一堆含有 n 个石头的石头堆。

游戏规则如下：

        1. Amon先手。

        2. 两人轮流取石头，每次可以从石堆取出 1 ~ 3 个石头。

        3. 第一个没有石头取的同学输掉比赛。

Amon 很好奇，如果他们都用最优策略来玩这个游戏，谁可以获胜。

## 输入

输入包含多组测试数据。

对于每组测试样例，输入一个正整数 n，表示石堆有 n 个石头。(0 ≤ n ≤ 100000)。

## 输出

对于每组测试输出一行。

如果 Amon 赢得游戏，输出“Amon!!!”。

否则输出“Congcong!!!”。

## 样例输入

```
1
2
3
4
```

## 样例输出

```
Amon!!!
Amon!!!
Amon!!!
Congcong!!!
```

### [四种博弈](https://blog.csdn.net/qq_40507857/article/details/81274426)

**巴什博弈：有N个物品，每次可以取1~M个，最后取完者胜。**

假设N=M+1，那么无论先手取多少个，后者一次便可取完，先手必败

假设N=（M+1）* R+S（R为任意自然数，S<=M)，那么先手拿走S个物品，后手拿走K个物品（K<=M)，先手拿走M+1-K个物品，剩余（M+1）*（R-1）个物品，保持这样的取法，先手必胜。

**胜利法则：若N%（M+1）不等于0，先手必胜**

```java
import java.util.*;
import java.math.*;

public class Main{
	static int maxn=(int)(5e4+10);
	
	public static void main(String[] args) {
		Scanner cin=new Scanner(System.in);
		int ca=1;
		while(cin.hasNext()) {
			int n=cin.nextInt();
			if(n%4!=0)
				System.out.println("Amon!!!");
			else
				System.out.println("Congcong!!!");
		}
		cin.close();
	}
}
```

## 问题 F: 有趣的体能测试

时间限制: 1 Sec  内存限制: 128 MB

提交: 2  解决: 2

[[提交](http://sustoj.com/JudgeOnline/submitpage.php?cid=1030&pid=5&langmask=262128)][[状态](http://sustoj.com/JudgeOnline/problemstatus.php?id=1922)][[讨论版](http://sustoj.com/JudgeOnline/bbs.php?pid=1922&cid=1030)][命题人:[201606060136](http://sustoj.com/JudgeOnline/userinfo.php?user=201606060136)][[Edit](http://sustoj.com/JudgeOnline/admin/problem_edit.php?id=1922&getkey=E08721FB89)] [TestData]

## 题目描述

“苍天啊！为什么我一年比一年跑的慢，而体侧及格线一年比一年高？”尤其是1000M，每次跑到怀疑人生！

为此，今年体育部出提出了一种新的体测项目：学生从出发点开始跑，必须经过两个传感器，而且只能沿着南北方向和东西方向跑。

聪明的ACM选手，你知道怎样跑最省力气吗？

## 输入

本题包含多组测试，请处理到文件结束。

每组测试样例，第一行为四个整数，[x1，y1]，[x2，y2]，代表要经过的两个传感器的坐标。

为了简化问题，出发点设为[0，0].

## 输出

对于每组测试，输出一个整数，代表最短距离

## 样例输入
`1  1  2  2`
## 样例输出
`4`
## 提示

所有操作数在32bit整型数范围内

### 分析：

曼哈顿距离（或出租车距离）是1种使用在几何度量空间的几何学用语，用以标明两个点在标准坐标系上的绝对轴距总和。

图中红线代表曼哈顿距离，绿色代表[欧氏距离](https://baike.baidu.com/item/%E6%AC%A7%E6%B0%8F%E8%B7%9D%E7%A6%BB)，也就是[直线距离](https://baike.baidu.com/item/%E7%9B%B4%E7%BA%BF%E8%B7%9D%E7%A6%BB)，而蓝色和黄色代表等价的曼哈顿距离。

[曼哈顿](https://baike.baidu.com/item/%E6%9B%BC%E5%93%88%E9%A1%BF)距离——两点在南北方向上的距离，加上在东西方向上的距离

![](https://gss1.bdstatic.com/9vo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D220/sign=3643f5d17d1ed21b7dc929e79d6fddae/8326cffc1e178a8208d61b83f603738da977e82f.jpg)

**例如在平面上，坐标（x1, y1）的i点与坐标（x2, y2）的j点的曼哈顿距离为：**

**d(i,j)=|X1-X2|+|Y1-Y2|.**

**既然必须经过两个点，那么只有两种可能【原点->1号点->2号点】或者【原点->2号点->1号点】**

```cpp
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <queue>
#include <map>
#include <vector>
#include <set>
#include <string>
#include <math.h>

using namespace std;
const double eps = 1e-8;
const int maxn=2e5+10;

int main()
{
    int x1,y1,x2,y2;
    scanf("%d%d%d%d",&x1,&y1,&x2,&y2);
    int ans1=abs(x1)+abs(y1)+abs(x2-x1)+abs(y2-y1);
    int ans2=abs(x2)+abs(y2)+abs(x1-x2)+abs(y1-y2);
    printf("%d\n",min(ans1,ans2));
}
```

## 问题 G: 图书管理

时间限制: 1 Sec  内存限制: 128 MB

提交: 22  解决: 6

[[提交](http://sustoj.com/JudgeOnline/submitpage.php?cid=1030&pid=6&langmask=262128)][[状态](http://sustoj.com/JudgeOnline/problemstatus.php?id=1923)][[讨论版](http://sustoj.com/JudgeOnline/bbs.php?pid=1923&cid=1030)][命题人:[201606060136](http://sustoj.com/JudgeOnline/userinfo.php?user=201606060136)]

## 题目描述

小虎参加陕西科技大学青年志愿者服务团，经常去做志愿活动，其中一项就是在图书馆帮忙整理书籍。

小虎的工作很简单，就是把图书分类放到书架上。比如，现在有一摞书【1，2，3】，（为了简化问题，我们把书编号为1，2，3.....）

1号书在最上面，书架上需要把按【2，1，3】整理摆放。第一次小虎取2号书时，他会把【1，2】两本书放到书架上；第二次需要1号书时，

1号书已经在书架上啦；第三次，他会把3号书放到书架上

## 输入

第一行一个整数T，表示有T组数据。 

每组数据第一行一个正整数N（N<=2*10^5）表示有N本书

接下来有N个正整数，a1，a2，a3....an，代表一摞书，a1号书在最上面（1<=ai<=N）

接下来N个正数，b1，b2，b3....bn，代表N本书在书架上摆放的顺序（1<=bi<=N）

## 输出

对于每组测试样例，输出N个数，代表每次取书的数量。

## 样例输入

```
2
3
1 2 3
2 1 3
5
1 2 3 4 5
1 2 3 4 5
```

## 样例输出

```
2 0 1
1 1 1 1 1
```

## 提示

对于第一个样例，小虎一共取了三次书，第一次取2本，第二次取0本，第三次取1本，输出【2  0  1】

分析：

根据题意，每次只能从一摞书的上面取走连续的K本书，那么考虑使用一个栈来解决先进后出的问题，比如，现在有一摞书【1，2，3】，1号书在最上面，即栈顶元素；书架上需要把按【2，1，3】整理摆放。第一次小虎取2号书时，他会把【1，2】两本书放到书架上，即先弹出1，然后弹出2；第二次需要1号书时，1号书已经在书架上啦；第三次，他会把3号书放到书架上，把弹出3；每次记录一下出栈元素个数即可

```cpp
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <queue>
#include <set>
#include <map>
#include <vector>
#include <string>
#include <math.h>
 
using namespace std;
const double eps = 1e-8;
const int maxn=2e5+10;
int a[maxn],b[maxn];
bool vis[maxn];
int ans[maxn];
int main()
{
    //freopen("test.in","r",stdin);
    //freopen("test.out","w",stdout);
    int T;
    scanf("%d",&T);
    while(T--){
        int n;
    scanf("%d",&n);
    for(int i=1;i<=n;i++)
        scanf("%d",&a[i]);
 
    for(int j=1;j<=n;j++)
        scanf("%d",&b[j]);
    memset(ans,0,sizeof ans);
    memset(vis,0,sizeof vis);
    int top=1;
    for(int i=1;i<=n;i++){
        if(vis[b[i]]){
            ans[i]=0;
            continue;
        }
        int num=0;
        while(top<=n){
            int cur=a[top++];
            vis[cur]=1;
            num++;
            if(cur==b[i])
                break;
        }
        ans[i]=num;
    }
    for(int i=1;i<=n;i++)
        printf("%d%c",ans[i],i==n?'\n':' ');
    }
    return 0;
}
```

## 问题 H: 电音之王

时间限制: 1 Sec  内存限制: 128 MB

提交: 3  解决: 1

[[提交](http://sustoj.com/JudgeOnline/submitpage.php?cid=1030&pid=7&langmask=262128)][[状态](http://sustoj.com/JudgeOnline/problemstatus.php?id=1924)][[讨论版](http://sustoj.com/JudgeOnline/bbs.php?pid=1924&cid=1030)][命题人:[201606060136](http://sustoj.com/JudgeOnline/userinfo.php?user=201606060136)][[Edit](http://sustoj.com/JudgeOnline/admin/problem_edit.php?id=1924&getkey=FFC249D5BE)] [TestData]

## 题目描述

终于活成了自己讨厌的样子。

听说多听电音能加快程序运行的速度。

小娜打了四年ACM，获奖无数，毕业之后进入TX公司，成为一名算法工程师。他所在的团队负责应用程序的底层架构开发。

团队中每个程序员都有几个朋友，当然自己和自己不算朋友，因为拥有朋友，会获得一定的快乐值。比如，现在团队中有３个人，

如果一个程序员有１个朋友，那么他的快乐值为２；如果一个程序员有２个朋友，那么他的快乐值为１.团队的快乐值就是团队中

所有程序员的快乐值之和，团队快乐值越大，工作效率越高。齐老板想知道团队快乐值最大是多少？

## 输入

首先是一个整数T，代表有T组测试样例。

对于每组测试样例，第一行是一个数N，代表团队中有N个人（N<=1000000)

第二行是N-1个数，a1，a2，a3，a4.......（<=100000000).，a1代表程序员有一个朋友时的快乐值，

a2代表程序员有两个朋友时的快乐值，以此类推

## 输出

对于每组测试样例，输出团队最大的快乐值

## 样例输入

```
1
3
2 1
```

## 样例输出
`5`
### 分析：

团队中有N个人，如果两个是朋友，就建一条边；显然一个人最多N-1个朋友，自己和自己不算朋友（说明没有自环），每个人至少一个朋友（说明图是连通图）；

这样n个点构成一棵树，有n-1条边，一共是2*n-2个度，而且确定的一点是每一个点都至少有1个度。这样还剩下n-2个度。问题就转化成了n-2个度分给n个点（每个点拥有的度数可以是0）。

但是题目给了n-1个f(i)意思就是说，有n-1种物品，每一种物品的种类都是无限的（因为每个点的度数是可以相同的），并且每件物品的价值都是v【i】=1，并且每件物品的体积（重量）都是i（就是这个物品的度数），然后把这n-1种物品，正好装入一个容量为n-2的背包里

但是这道题还有个坑，就是我们默认了每个点都有了一个度了，所以实际上我们在枚举物品的时候，他们的度都是i+1（假设从1开始循环）

```java
import java.math.*;
import java.util.*;
public class Main {
	static int maxn=2020;
	static int INF=(int)1e9;
	static int n,v;
	static int[] f=new int[maxn];
	static int[] dp=new int[maxn];
	
	static void DP() {
		for(int i=1;i<=n;i++)
			dp[i]=-INF;//求恰好装满背包时的最优解，初始化为—INF
		dp[0]=0;//背包容量为0，价值为0
		 v=n-2;//背包容量

		 for(int i=1;i<=n-1;i++)//枚举物品
			 for(int j=i;j<=v;j++)//枚举背包容量
				 dp[j]=Math.max(dp[j], dp[j-i]+f[i+1]-f[1]);//状态转移方程
	}
    public static void main (String[] args) {
    	 Scanner cin=new Scanner(System.in);
    	 int T=cin.nextInt();
    	 while((T--)!=0) {
    		 n=cin.nextInt();
    		 for(int i=1;i<=n-1;i++)
    			 f[i]=cin.nextInt();
    		 DP();
    		 System.out.println(dp[v]+f[1]*n);
    	 }
    	 cin.close();
      }
    }
```

## 问题 I: 模拟人生：大学生活

时间限制: 1 Sec  内存限制: 128 MB

提交: 7  解决: 3

[[提交](http://sustoj.com/JudgeOnline/submitpage.php?cid=1030&pid=8&langmask=262128)][[状态](http://sustoj.com/JudgeOnline/problemstatus.php?id=1858)][[讨论版](http://sustoj.com/JudgeOnline/bbs.php?pid=1858&cid=1030)][命题人:[winsoul](http://sustoj.com/JudgeOnline/userinfo.php?user=winsoul)][[Edit](http://sustoj.com/JudgeOnline/admin/problem_edit.php?id=1858&getkey=380B860CBF)] [TestData]

## 题目描述

*            如果不喜欢编程，学习算法 和 学习高数有什么不同？  —— 渣渣辉*

小虎很喜欢《模拟人生：大学生活》这款游戏，因为小虎可以在这款游戏中控制虚拟角色做任何事情。

这款游戏讲述的是一个主人公在大学里面的生活。

小虎非常喜欢算法，当然在游戏中也不例外。他说：“大学里，只有算法伴我左右。”。

于是他在游戏中利用他的虚拟角色在学校学习了各种各样的算法，并发明了 KHP 算法登上了 *Time'*  封面。

KHP 算法是这样描述的：

            1. 给定一个长度为 n 的非负整数序列 a ，和长度同为 n 的序列 b。( 0 <= ai <= 109，1 <= bi <= n)

            2. 你可以找出符合条件的 an+1，an+2，...，a2n，使得 ![](http://sustoj.com/JudgeOnline/upload/image/20180817/20180817180600_52677.png) 最大。

找出 an，an+1，...，a2n 的方法如下：

            1. 对于每一个 ai ，你需要在 b 序列中选择一个 bk ( b 中每个元素最多只能选一次)。

            2.  ai = max{ aj - j | bk ≤ j < i}。

请你计算出![](http://sustoj.com/JudgeOnline/upload/image/20180817/20180817180923_24098.png)。

## 输入

输入包含多组测试数据。

对于每组测试数据：

第一行输入为正整数 n，表示序列 a、b 的长度。(1 ≤ n ≤ 2*105)

第二行输入为 n 个非负整数，为序列 a 的元素。

第三行输入为 n 个正整数，为序列 b 的元素。

mod: 1000000007

## 输出

对于每组测试数据，输出![](http://sustoj.com/JudgeOnline/upload/image/20180817/20180817181646_75762.png)的结果，占一行。

## 样例输入

```
4
8 11 8 5
3 1 4 2
```

## 样例输出
`59`
```cpp
#include <bits/stdc++.h>
using namespace std;
const int maxn = 400010;
const int mod = 1000000007;
long long int a[maxn], b[maxn], segtree[maxn << 4];

void build_tree(int l, int r, int now) {
  if (l == r) {
    segtree[now] = a[l] - l;
    return;
  }
  int mid = l + (r - l)/2;
  build_tree(l, mid, now << 1);
  build_tree(mid + 1, r, (now << 1)|1);
  segtree[now] = max(segtree[now << 1], segtree[(now << 1)|1]);
}

long long int query(int L, int R, int l, int r, int now) {
  if (L <= l && R >= r) {
    return segtree[now];
  }
  long long int ans = 0;
  int mid = l + (r - l)/2;
  if (L <= mid) ans = max(ans, query(L, R, l, mid, now << 1));
  if (R > mid) ans = max(ans, query(L, R, mid + 1, r, (now << 1)|1));
  return ans;
}

void update(int l, int r, int tar, int c, int now) {
  if (l == r) {
    segtree[now] = c;
    return;
  }
  int mid = l + (r - l)/2;
  if (tar <= mid) update(l, mid, tar, c, now << 1);
  else update(mid + 1, r, tar, c, (now << 1)|1);
  segtree[now] = max(segtree[now << 1], segtree[(now << 1)|1]);
}

int main()
{
  int n;
  while (scanf("%d", &n) != EOF) {
    memset(segtree, 0, sizeof segtree);
    memset(a, 0, sizeof a);
    memset(b, 0, sizeof b);
    long long int ans = 0;
    for (int i = 1; i <= n; i++) {
      scanf("%lld", &a[i]);
      ans += a[i]%mod;
    }
    for (int i = 1; i <= n; i++) {
      scanf("%lld", &b[i]);
    }

    sort(b + 1, b + n + 1);
    build_tree(1, 2*n, 1);

    for (int i = 1; i <= n; i++) {
      long long int ai = query(b[i], n + i - 1, 1, 2*n, 1);
      ans += ai%mod;
      update(1, 2*n, n + i, ai - n - i, 1);
    }
    printf("%lld\n", ans%mod);
  }

  return 0;
}
```





