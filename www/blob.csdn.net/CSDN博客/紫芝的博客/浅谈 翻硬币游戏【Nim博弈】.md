# 浅谈 翻硬币游戏【Nim博弈】 - 紫芝的博客 - CSDN博客





2018年11月06日 20:36:22[紫芝](https://me.csdn.net/qq_40507857)阅读数：278








### [ACM博客_kuangbin](https://www.cnblogs.com/kuangbin/)

## [博弈-翻硬币游戏](https://www.cnblogs.com/kuangbin/p/3218060.html)

### [hihoCoder](http://hihocoder.com/problemset/problem/1172)

### 1172 : 博弈游戏·Nim游戏·二

时间限制:10000ms

单点时限:1000ms

内存限制:256MB

描述

Alice和Bob这一次准备玩一个关于硬币的游戏：

N枚硬币排成一列，有的正面朝上，有的背面朝上，**从左到右**依次编号为1..N。现在两人轮流翻硬币，每次只能将一枚正面朝上的硬币翻过来，并且可以随自己的意愿，在一枚硬币翻转后决定要不要将该硬币**左边**的任意一枚硬币也翻一次(正面翻到背面或背面翻到正面)。翻最后一枚正面向上的硬币的人获胜。同样的，这次游戏里面Alice仍然先手，两人均采取最优的策略，对于给定的初始局面，Alice会获胜还是Bob会获胜？

[提示：Turning Turtles](http://hihocoder.com/problemset/problem/1172#)

输入

第1行：1个正整数N，表示硬币数量。1≤N≤10,000

第2行：1个字符串，第i个字符表示编号为i的硬币状态，’H’表示正面朝上，’T’表示背面朝上。

输出

第1行：1个字符串，若Alice能够获胜输出"Alice"，否则输出"Bob"

样例输入

```
8
HHTHTTHT
```

样例输出
`Bob`
### Turning Turtles

这个游戏叫做Turning Turtles，它的本质就是Nim游戏。那么它到底是如何转化为Nim游戏的呢？让我们一步一步来分析。

**首先，我们先将局面分解一下，每一次我们只考虑一枚硬币。**

不妨设所有硬币全部背面朝上的局面为局面0

假设现在N枚硬币，只有第1枚是正面朝上的。该局面只能转化为全部硬币背面朝上的局面。我们假定该局面为 局面1，则局面1可以转化为局面0。

假设只有第2枚是正面朝上的。该局面可以转化为：只有硬币1正面朝上；全部硬币背面朝上。我们假定该局面为 局面2，局面2可以转化为局面1和局面0。

同理我们可以推定，第i枚硬币正面朝上的局面为局面i，局面i可以转化为局面0..i-1。

**现在，我们考虑把给定的局面拆成单个硬币的局面集合**

比如给定了{HHTHTTHT}，其中H表示正面朝上，T表示背面朝上。那么就是当前局面={局面1，局面2，局面4，局面7}。每一次我们可以改变其中个一个局面，当出现局面0时就从集合中删去。

这样一看是不是就变成了Nim游戏了？然而事实并没有那么简单。


进一步分析，若同时存在i,j(j<i)两枚硬币正面朝上。我们将这个局面拆成2个单一的局面：即局面i和局面j。

在反转i的时候我们考虑从局面i转移到局面j，那么我们会有两个局面j。

表示第j枚被反转了2次，也就是回到了背面朝上的状态。

那么我们得到这个游戏一个性质：当出现两个同样的局面时，等价于这两个局面合并变成了局面0。


这种情况在Nim游戏中是没有的，那么它会对Nim游戏的状态造成影响么？

我们想一想，在Nim游戏中，如果出现两个数量相同的堆时，比如A[i]=A[j]。在计算Nim游戏状态时我们采用的xor操作，xor有交换律和结合律。则我们可以变成：

(A[i] xor A[j]) xor Other

因为A[i] = A[j]，所以A[i] xor A[j] = 0。上式实际就是：

0 xor Other

也就是说在原Nim游戏中，若出现了两个数量相同的堆时，实际上这两堆已经不对总局面造成影响了，也就可以认为这两对合并为了一个数量为0的堆。


到此，我们可以发现这个硬币游戏完全满足Nim游戏的规则，其解答也满足Nim游戏的性质，这题也就很简单的转化为了普通的Nim游戏。在实际的博弈游戏中会发现很多都是可以转化为Nim游戏模型。如何正确的建立模型和转化游戏模型也就是解决博弈游戏一个很重要的手段。


比如Nimble游戏：

游戏开始时有许多硬币任意分布在楼梯上，共N阶楼梯从地面由下向上编号为0到N。游戏者在每次操作时可以将任意一枚硬币向下移动，直至地面。游戏者轮流操作，将最后一枚硬币移至地面(即第0阶)的人获胜。在双方都采取最优策略的情况下，对于给定的初始局面，问先手必胜还是先手必败。

每一枚硬币仍然对应了一个石子堆，向下移动就等价于从石子堆里面取出石子。


同样的例子还有很多，有些游戏甚至需要做好几次转换才能移动到Nim游戏模型，在之后我们就会见到。

```java
import java.util.*;
import java.math.*;
 
public class Main{
	static int MAXN=(int)(500+10);
	
	public static void main(String[] args) {
		Scanner cin=new Scanner(System.in);
		while(cin.hasNext()) {
			int n=cin.nextInt();
			String str=cin.next();
			int len=str.length();
			int ret=0;
			for(int i=0;i<len;i++) {
				if(str.charAt(i)=='H')
					ret^=(i+1);
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

HDU3537

题意：

有一排硬币，告诉 你n个正面朝上的硬币的位置，你可以选择任意位置的1~3个硬币翻转一下，但是问你先手是否会输。

打表找规律

```cpp
#include<stdio.h>
#include<string.h>
//统计x的二进制表示中1的个数
static int countOne(int x) {
    int ans=0;
    while(x!=0) {
        if((x&1)!=0)	ans++;
        x=x>>1;
    }
    return ans;
}
int vis[1007],sg[1007];
int main()
{
    for(int i=1;i<100;i++)
    {
        memset(vis,0,sizeof(vis));
        vis[0]=1;//翻一枚
        for(int j=0;j<i;j++)
            vis[sg[j]]=1;//翻两枚
        for(int j=0;j<i;j++)
            for(int k=0;k<j;k++)
                vis[sg[j]^sg[k]]=1;//翻三枚
        for(int k=0;;k++)
            if(!vis[k]){sg[i]=k;break;}
        printf("%d,%d,%d\n",i-1,sg[i],countOne(i-1));
    }
}
```

规律：
- 若X的二进制表示里1的个数为奇数，则SG[x]=2*x；
- 若X的二进制表示里1的个数为偶数，则SG[x]=2*x+1；

最后N个X的SG值异或为0，先手必败；

```cpp
#include<iostream>
#include<algorithm>
#include<cstdio>
#include<cstring>
#include<math.h>
#include<set>
using namespace std;
const int  maxn = 1e6+10;
typedef long long LL;
//统计x的二进制表示中1的个数
static int countOne(int x) {
    int ans=0;
    while(x!=0) {
        if((x&1)!=0)	ans++;
        x=x>>1;
    }
    return ans;
}
//计算x的SG值
static int SG(int x) {
    int one=countOne(x);
    if(one%2==1)
        return x<<1;//sg=2*x；
    return x<<1|1;//sg=2*x+1
}
int main()
{
   int n;
   while(scanf("%d",&n)!=EOF){
        set<int>s;//自动去重
    for(int i=0;i<n;i++){
        int x;
        scanf("%d",&x);
        s.insert(x);
    }
    int ret=0;
    set<int>::iterator it;
    for(it=s.begin();it!=s.end();it++){
        //printf("%d\n",*it);
        ret^=SG(*it);
    }
    if(ret==0)
        printf("Yes\n");
    else
        printf("No\n");
   }
   return 0;
}
```





