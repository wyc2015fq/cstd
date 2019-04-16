# HDU2176  【 Nim博弈】 SG函数求解 - 紫芝的博客 - CSDN博客





2018年07月30日 09:48:35[紫芝](https://me.csdn.net/qq_40507857)阅读数：166








# 取(m堆)石子游戏

m堆石子,两人轮流取.只能在1堆中取.取完者胜.先取者负输出No.先取者胜输出Yes,然后输出怎样取子.例如5堆 5,7,8,9,10先取者胜,先取者第1次取时可以从有8个的那一堆取走7个剩下1个,也可以从有9个的中那一堆取走9个剩下0个,也可以从有10个的中那一堆取走7个剩下3个.

Input

输入有多组.每组第1行是m,m<=200000. 后面m个非零正整数.m=0退出. 

Output

先取者负输出No.先取者胜输出Yes,然后输出先取者第1次取子的所有方法.如果从有a个石子的堆中取若干个后剩下b个后会胜就输出a b.参看Sample Output. 

Sample Input

2
45 45
3
3 6 9
5
5 7 8 9 10
0
Sample Output

No
Yes
9 5
Yes
8 1
9 0
10 3
Nim游戏：

有N堆石子，每堆石子的个数是有限的，合法的操作是：选择一堆石子，拿走若干颗，不能不拿；最后拿走最后一颗石子者胜。

结论：

必败状态：a1^a2^......^an=0（奇异局势）

必胜状态：a1^a2^.......^an=k (其中k不为零)

**遇到奇异局势的选手必败**

证明：

N=1时，先手第一次可以全部取完，先手必胜；

**1.对于某个局面(a1,a2,...,an)，若a1^a2^...^an!=0，一定存在某个合法的移动，将ai改变成ai'后满足a1^a2^...^ai'^...^an=0。**

不妨设**a1^a2^...^an=k**，则一定存在某个ai，它的二进制表示在k的最高位上是1（否则k的最高位那个1是怎么得到的）。

这时ai^k<ai一定成立。则我们可以将ai改变成ai'=ai^k，此时 **a1^a2^...^ai'^...^an=a1^a2^...^an^k=0。**

**2.对于某个局面(a1,a2,...,an)，若a1^a2^a3...^an=0，先手遇到奇异局势，先手必败；**

一定不存在某个合法的移动，将ai改变成ai'后,足 **a1^a2^...^ai'^...^an=0。**

因为[异或](http://baike.baidu.com/view/674171.htm)运算满足消去率，由**a1^a2^...^an=a1^a2^...^ai'^...^an**可以得到ai=ai'。所以将ai改变成ai'不是一个合法的移动

答案：
- 如果a1^a2^a3^......^an=0,先手遇到奇异局势，先手必败；
- 如果a1^a2^a3^......^an=k，先手必胜，先手选择一堆石子，取temp个之后，出现奇异局势，后手必败

假设从第 i 堆石子中取走(ai^k)个石子，(a1^a2^a3^...ai^....^an)^k=k^k=0，剩余石子必然是奇异局势，后手必败；
- **若(ai^k) < ai，即从第i堆中取走的石子个数小于等于总个数，那么就可以从这一堆石子中取**
- **若(ai^k) > ai，即从第i堆中取走的石子个数大于总个数，那么就不可以从这一堆石子中取**

在这道题中如果当前是必胜的话，那么就要下一个移动的人必败，所以就要改变一个ai变成ai'使得原本的a1^...ai^...^an!=0变成a1^...ai'...^an=0，可以利用ai^k<ai' 判断

```java
import java.util.*;
import java.math.*;

public class Main{
	static int MAXN=(int)(2e5+10);
	static int[] a=new int[MAXN];//存储每堆石子的个数
	static int n;
	
	public static void main(String[] args) {
		Scanner cin=new Scanner(System.in);
		
		while(cin.hasNext()) {
			n=cin.nextInt();
			if(n==0)	break;
			int ret=0;
		
			for(int i=1;i<=n;i++) {
				a[i]=cin.nextInt();
				ret^=a[i];//根据Nim游戏的策略，对每堆石子进行异或运算
			}	
			
			if(ret==0) {//先手遇到奇异局势，必败
				System.out.println("No");
				continue;
			}
			System.out.println("Yes");
			//先手取一次之后，让后手面对奇异局势
			for(int i=1;i<=n;i++) {
				int temp=a[i]^ret;
				if(temp<=a[i]) 
					System.out.println(a[i]+" "+temp);
			}
		}
		cin.close();
	}
}
```

## 取硬币游戏 

时间限制: 1 Sec  内存限制: 128 MB

## 题目描述

现在有n堆硬币，第i堆硬币有xi个硬币。yoyo和灰灰轮流进行操作，每次操作只能选择一堆硬币，然后从这一堆硬币中取任意多个硬币（1~x，x为该堆最大数量），但不能不取。轮到的人如果没有硬币可取，则输。yoyo先手，谁能获胜？

## 输入

首行输入t，代表t组样例

每组样例第一行输入n，代表n堆硬币。n<=1000;

接下来n个数字（a1,a2,a3......an）代表每堆硬币的硬币数。an<=1000;

## 输出

输出谁赢。yoyo必胜输出yoyo，否则输出zhazhahui

## 样例输入

```
2
3
2 3 4
4
2 3 4 5
```

## 样例输出

```
yoyo
zhazhahui
```

分析：

对于一堆硬币，假设有x个，则这堆硬币取一次后，剩余的个数可能是mex={0，1，2.....x-2，x-1}，

SG表示最小的不属于mex集合的最小非负整数，即SG（x）=x；

所以ans=x1^x2^x3.......xn;若ans=0，说明先手必败，否则先手必胜

```cpp
#include<bits/stdc++.h>
using namespace std;

int main()
{
	ios::sync_with_stdio(false);
	int n,t;
	cin>>t;
	while(t--){
			cin>>n;
			int x,ans=0;
			for(int i=0;i<n;i++){
                cin>>x;
                ans^=x;
			}

			if(ans==0)
			printf("zhazhahui\n");
			else
			printf("yoyo\n");
		}
	return 0;
 }
```





