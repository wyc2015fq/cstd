# HDU3032 / LightOJ 1199,1296【Nim博弈】SG函数打表找规律 - 紫芝的博客 - CSDN博客





2018年07月29日 20:47:18[紫芝](https://me.csdn.net/qq_40507857)阅读数：63








# Nim or not Nim?

****Time Limit: 2000/1000 MS (Java/Others)    Memory Limit: 32768/32768 K (Java/Others)Total Submission(s): 2794    Accepted Submission(s): 1446****

**Problem Description**

Nim is a two-player mathematic game of strategy in which players take turns removing objects from distinct heaps. On each turn, a player must remove at least one object, and may remove any number of objects provided they all come from the same heap.


Nim is usually played as a misere game, in which the player to take the last object loses. Nim can also be played as a normal play game, which means that the person who makes the last move (i.e., who takes the last object) wins. This is called normal play because most games follow this convention, even though Nim usually does not.


Alice and Bob is tired of playing Nim under the standard rule, so they make a difference by also allowing the player to separate one of the heaps into two smaller ones. That is, each turn the player may either remove any number of objects from a heap or separate a heap into two smaller ones, and the one who takes the last object wins.

**Input**

Input contains multiple test cases. The first line is an integer 1 ≤ T ≤ 100, the number of test cases. Each case begins with an integer N, indicating the number of the heaps, the next line contains N integers s[0], s[1], ...., s[N-1], representing heaps with s[0], s[1], ..., s[N-1] objects respectively.(1 ≤ N ≤ 10^6, 1 ≤ S[i] ≤ 2^31 - 1)

**Output**

For each test case, output a line which contains either "Alice" or "Bob", which is the winner of this game. Alice will play first. You may asume they never make mistakes.

**Sample Input**


```
2 

3 

2 2 3

2 

3 3
```

**Sample Output**

Alice 

Bob

**Source**

[2009 Multi-University Training Contest 13 - Host by HIT](http://acm.hdu.edu.cn/search.php?field=problem&key=2009+Multi-University+Training+Contest+13+-+Host+by+HIT&source=1&searchmode=source)

**Recommend**

gaojie   |   We have carefully selected several similar problems for you:  [3031](http://acm.hdu.edu.cn/showproblem.php?pid=3031)[3033](http://acm.hdu.edu.cn/showproblem.php?pid=3033)[3038](http://acm.hdu.edu.cn/showproblem.php?pid=3038)[3035](http://acm.hdu.edu.cn/showproblem.php?pid=3035)[3034](http://acm.hdu.edu.cn/showproblem.php?pid=3034)



### 题意：

nim博弈的变形，n堆石头，每一轮允许两会中操作之一：

**①、从一堆石子中取走任意多个;**

**②、将一堆数量不少于 2 的石子分成都不为空的两堆。**

最后取完的获胜。

### 分析：

**SG函数的计算：g(x)=mex{ g(y)|y是x的后继状态 }，其中mex S表示: S集合中未出现的最小非负整数，g(x0)=0;（x0为终止状态）**

设状态用石子数量（a1,a2,a3,a4......am)表示，单堆状态表示为a；

**将一堆石子分为两堆，等于将一个一堆的Nim游戏A变成两个一堆的游戏的和B，这两个一堆游戏之和也是A的后继状态；计算A的SG函数时，除考虑普通的后继状态之外，还要将B的SG值加入MEX计算，而B的ＳＧ值是两个成员游戏SG值的异或和（和游戏定理）。**

**g(0) = 0;**
**g(1) = mex { g(0) }= mex{ 0 } = 1;**

**g(2) = mex{ g(0), g(1), g(1, 1) } = mex { g(0), g(1), g(1)^g(1) } = mex{ 0, 1 } =2;**

**g(3) = mex{ g(0), g(1), g(2), g(1,2) = mex { g(0), g(1), g(2), g(1)^g(2) }= mex{ 0, 1, 2, 3 }=4;**

**g(4) = mex{ g(0), g(1), g(2), g(3), g(1,3), g(2,2) }= mex{ g(0), g(1), g(2), g(3), g(1)^g(3), g(2)^g(2) }=mex={ 0,1,2,4,5,0 }=3;**

**g(5) = mex{ g(0), g(1), g(2), g(3), g(4), g(1,4), g(2,3) } **

**       =mex{ g(0), g(1), g(2), g(3), g(4), g(1)^g(4), g(2)^g(3) }**

**       =mex{ 0, 1, 2, 4, 3, 2, 6 } =5;**

**g(6) = mex{  g(0), g(1), g(2), g(3), g(4), g(5), g(1,5), g(2,4), g(3,3) }**

**       = mex{ g(0), g(1), g(2), g(3), g(4), g(5), g(1)^g(5), g(2)^g(4), g(3)^g(3) }**

**       = mex{ 0,1, 2, 4, 3, 5, 4, 1, 0 } = 6;**

**g(7) =mex{ g(0), g(1), g(2), g(3), g(4), g(5), g(6), g(1,6), g(2,5), g(3,4) }**

**       =mex{ g(0), g(1), g(2), g(3), g(4), g(5), g(6), g(1)^g(6), g(2)^g(5), g(3)^g(4) }**

**       =mex{ 0, 1, 2, 4, 3, 5, 6, 7, 7, 7 } =8;**

**g(8) =mex{ g(0), g(1), g(2), g(3), g(4), g(5), g(6), g(7), g(1,7), g(2,6), g(3,5), g(4,4) }**

**       =mex{ g(0), g(1), g(2), g(3), g(4), g(5), g(6), g(7), g(1)^g(7), g(2)^g(6), g(3)^g(5), g(4)^g(4) }**

**       =7;**

**g(9) = mex{ g(0), g(1), g(2), g(3), g(4), g(5), g(6), g(7), g(8), g(1,8), g(2,7), g(3,6), g(4,5) }**

**       = mex{ g(0), g(1), g(2), g(3), g(4), g(5), g(6), g(7), g(8), g(1)^g(8), g(2)^g(7), g(3)^g(6), g(4)^g(5) }**

**       =9;**

          ..........

**    最后根据数学归纳法推出的sg值，得到：**

对于所有的k >= 0，有 sg( 4k+1 ) = 4k+1； sg( 4k+2 ) = 4k+2;     sg( 4k+3 )= 4k+4； sg( 4k+4 ) = 4k+3。

即**k%4=1,SG(k)=k;  k%4=2,SG(k)=k;  k%4=3,SG(k)=k+1;  k%4=0,SG(K)=n-1;**

对于N堆石子，可以分为N个子游戏:

若SG(a1)^SG(a2)^SG(a3)^SG(a4)^SG(a5)^SG(a6)^SG(a7)^........SG(an)=0,先手必败

若SG(a1)^SG(a2)^SG(a3)^SG(a4)^SG(a5)^SG(a6)^SG(a7)^........SG(an)!=0,先手必胜

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
               if(x%4==0)   //sg( 4k+4 ) = 4k+3
                ans^=(x-1);
               else if(x%4==1||x%4==2) //sg( 4k+1 ) = 4k+1； sg( 4k+2 ) = 4k+2;
               ans^=x;
               else if(x%4==3)//sg( 4k+3 )= 4k+4；
               ans^=(x+1);
			}
			if(ans==0)//必败
			printf("Bob\n");
			else
			printf("Alice\n");
		}
	return 0;
 }
```

 Java

```java
import java.util.*;
import java.math.*;

public class Main{
	static int maxn=(int)(2e5+10);
	//除最后一次外，每一次取石子个数为[p,q]
	static int SG(int x) {
		if(x%4==1||x%4==2)
			return x;
		if(x%4==3)
			return x+1;
		if(x%4==0)
			return x-1;
		return x;
	}
	public static void main(String[] args) {
		Scanner cin=new Scanner(System.in);
		int T=cin.nextInt();
		while((T--)!=0) {
			int n=cin.nextInt();
			int ret=0;
			for(int i=0;i<n;i++) {
				int x=cin.nextInt();
				ret^=SG(x);
			}
			if(ret==0)
				System.out.println("Bob");
			else
				System.out.println("Alice");
		}
		cin.close();
	}
}
```

## Partitioning Game

### 题意：

nim博弈的变形，n堆石头，每一轮允许的操作：

**将一堆数量不少于 2 的石子分成都不为空且数量不同的两堆。**

最后一次分割者获胜。

### 分析：

**SG函数的计算：由于N的范围是【1，10000】，所以暴力SG打表，然后将N堆石子的SG值异或，若结果为0，先手必败**

**（刚开始想着打表找规律，半个小时后，根本没有规律，后来直接暴力竟然过啦）**

```cpp
#include <cstdio>
#include <cstring>
#include <algorithm>
#include<iostream>
using namespace std;
typedef long long LL;
const int  maxn = 1e4+10;
bool vis[maxn];
int sg[maxn];
void init(){
    memset(sg,0,sizeof sg);
    for(int i=1;i<=10000;i++){
        memset(vis,0,sizeof vis);
        int mid=i/2;
        if(i%2==0)  mid--;
        for(int j=1;j<=mid;j++)
            vis[sg[j]^sg[i-j]]=1;

        for(int j=0;j<maxn;j++)
            if(!vis[j]){
                sg[i]=j;
                break;
            }
    }
}
int main(){
    int T;
    int ca=1;
    init();
    scanf("%d",&T);
    while(T--){
        int n,m;
        scanf("%d",&n);
        LL ans=0;
        int x;
        for(int i=0;i<n;i++){
            scanf("%d",&x);
            ans^=sg[x];
        }
        printf("Case %d: ",ca++);
        if(ans==0)
            printf("Bob\n");
        else
            printf("Alice\n");
    }
    return 0;
}
```

### **【LightOJ-1296 Again Stone Game】**

**题意：**

**n堆石子，对于每堆的操作是：一堆数量为k个的石子，每堆至少取1个至多取k/2个(特殊：1个石子取不了)。数据范围[1, 1e9].**

**思路：**

**数据范围太大，不能预处理SG值，所以打出前100个石子的SG进行找规律，发现偶数时SG值为其本身/2，奇数时，发现其SG值为其不断除以2直至变成一个偶数，其SG值等于该偶数的SG值。**

```cpp
#include <cstdio>
#include <cstring>
#include <algorithm>
#include<iostream>
using namespace std;
typedef long long LL;
const int  maxn = 1e4+10;
int SG(int x){
    if(x%2==0)
        return x/2;
    return SG((x-1)/2);
    //return (x-1)/4;错误
}
/*
另一种方法
int SG(int x){
   while(x&1)   x>>=1;
   return x>>1;
}
*/

int main(){
    int T;
    int ca=1;

    scanf("%d",&T);
    while(T--){
        int n,m;
        scanf("%d",&n);
        LL ans=0;
        int x;
        for(int i=0;i<n;i++){
            scanf("%d",&x);
            ans^=SG(x);
        }
        printf("Case %d: ",ca++);
        if(ans==0)
            printf("Bob\n");
        else
            printf("Alice\n");
    }
    return 0;
}
```

**SG打表代码**

```cpp
#include<iostream>
#include<algorithm>
#include<cstdio>
#include<cstring>
#include<math.h>
#include<set>
using namespace std;
const int  maxn = 20+10;
bool vis[maxn];
int sg[maxn];
int SG(int x){
    if(x%2==0)
        return x/2;
    //return SG((x-1)/2);
    return (x-1)/4;
}
int main(){
    memset(sg,0,sizeof sg);
    for(int i=1;i<maxn;i++){
        memset(vis,0,sizeof vis);
        int mid=i/2;
        for(int j=1;j<=mid;j++)
            vis[sg[i-j]]=1;

        for(int j=0;j<maxn;j++)
            if(!vis[j]){
                sg[i]=j;
                break;
            }

            printf("i=%d,%d\n",i,sg[i]);
            printf("%d\n",SG(i));
    }
    return 0;
}
```





