# 四种基础博弈 巴什博奕+威佐夫博奕+斐波那契博弈+K倍博弈 - 紫芝的博客 - CSDN博客





2018年07月29日 22:51:00[紫芝](https://me.csdn.net/qq_40507857)阅读数：430








# 博弈论(Game Theory)

博弈论是指研究多个个体或团队之间在特定条件制约下的对局中利用相关方的策略，而实施对应策略的学科。有时也称为对策论，或者赛局理论，是研究具有斗争或竞争性质现象的理论和方法，它是应用数学的一个分支，既是现代数学的一个新分支，也是运筹学的一个重要学科。目前在生物学、经济学、国际关系学、计算机科学、政治学、军事战略和其他很多学科都有广泛的应用。主要研究公式化了的激励结构（游戏或者博弈（Game））间的相互作用。

**我们通常研究二人在平等的对局中各自如何利用对方的策略变换自己的对抗策略，达到取胜的目的。**

## 取石子游戏

根据题目的意思，看它属于哪种博弈，属于哪种博弈的变形。 然后根据对应的博弈模型的解题策略来求解， 有时候并不一定能够直接看出它属于哪种模型，那这个时候就可以通过判断自己每步可选的策略，对于自己每步走的，对当前局势的影响。然后推断出与之相对应的博弈模型。

### （一）巴什博奕（Bash Game）

**有n个物品，两个人轮流从这堆物品中取物，规定每次至少取一个，最多取 m个。最后取光者得胜。**

假设n = m + 1，那么无论如何取，先取者必输。因为先取者无论取多少，后者一次性便可将剩余取完。

**胜利法则：若n%（m+1)不等于0，则先手必胜**

如果 n=（m+1）r+s，（r 为任意自然数，s≤m),那么先取者要拿走 s 个物品，如果后取者拿走 k（≤m)个，那么先取者再拿走 m+1-k 个，结果剩下 （m+1）（r-1）个，以后保持这样的取法，那么先取者肯定获胜。总之，要保持给对手留下（m+1）的倍数，就能最后获胜。

```cpp
#include <iostream> 
using namespace std;
int main() {
	int iCase;
	cin >> iCase;
	while (iCase--) {
		int n, m;
		cin >> n >> m;
		if (n % (m + 1) == 0) cout << "lost" << endl;
		else cout << "win" << endl;
	}
	return 0;
}
```

### （二）威佐夫博奕（Wythoff Game）

**有两堆物品，一堆有a个物品，另一堆有b个物品，两个人轮流取走一些石子，有两种取法：**

**1.从某堆中取走任意多个物品**

**2.同时从两堆中取同样多的物品**

**规定每次至少取一个，多者不限，最后取光者得胜。**

**获胜法则：eqa = (1+sqrt(5.0))/2.0;     若a>b,swap(a,b);     若（b-a）*epa不等于a，先手必胜**

这种情况下是颇为复杂的。我们用（ak，bk）（ak ≤ bk ,k=0，1，2，…,n)表示两堆物品的数量并称其为局势，如果甲面对（0，0），那么甲已经输了，这种局势我们 称为奇异局势。前几个奇异局势是：（0，0）、（1，2）、（3，5）、（4，7）、（6，10）、（8，13）、（9，15）、（11，18）、（12，20）。

可以看出,a0=b0=0,ak 是未在前面出现过的最小自然数,而 bk= ak + k，奇异局势有如下三条性质：
- 任何自然数都包含在一个且仅有一个奇异局势中。
- 任意操作都可将奇异局势变为非奇异局势。
- 采用适当的方法，可以将非奇异局势变为奇异局势。

两个人如果都采用正确操作，那么面对非奇异局势，先拿者必胜；反之，则后拿者取胜。

如何判定是否是奇异局势呢？

有如下公式：** ak =[k（1+√5）/2]**，**bk= ak + k （k=0，1，2，…,n 方括号表示取整函数)**

其中（1+√5）/2 = 1.618…,即为黄金分割数。因此,由 ak，bk 组成的矩形近似为黄金矩形，由于 2/（1+√5）=（√5-1）/2，可以先求出 j=[a（√5-1）/2]，若a=[ j（1+√5）/2]，那么 a = aj，bj = aj + j，若不等于，那么 a = aj+1，bj+1 = aj+1 + j + 1，若都不是，那么就不是奇异局势。然后再按照上述法则进行，一定会遇到奇异局势。

# [HDU1527](http://acm.hdu.edu.cn/showproblem.php?pid=1527)

# 取石子游戏

****Time Limit: 2000/1000 MS (Java/Others)    Memory Limit: 65536/32768 K (Java/Others)Total Submission(s): 9302    Accepted Submission(s): 5342****

**Problem Description**

有两堆石子，数量任意，可以不同。游戏开始由两个人轮流取石子。游戏规定，每次有两种不同的取法，一是可以在任意的一堆中取走任意多的石子；二是可以在两堆中同时取走相同数量的石子。最后把石子全部取完者为胜者。现在给出初始的两堆石子的数目，如果轮到你先取，假设双方都采取最好的策略，问最后你是胜者还是败者。

**Input**

输入包含若干行，表示若干种石子的初始情况，其中每一行包含两个非负整数a和b，表示两堆石子的数目，a和b都不大于1,000,000,000。

**Output**

输出对应也有若干行，每行包含一个数字1或0，如果最后你是胜者，则为1，反之，则为0。

**Sample Input**

2 1 

8 4 

4 7

**Sample Output**

0 

1 

0

**Source**

[NOI](http://acm.hdu.edu.cn/search.php?field=problem&key=NOI&source=1&searchmode=source)

**Recommend**

LL   |   We have carefully selected several similar problems for you:  [1404](http://acm.hdu.edu.cn/showproblem.php?pid=1404)[1536](http://acm.hdu.edu.cn/showproblem.php?pid=1536)[1517](http://acm.hdu.edu.cn/showproblem.php?pid=1517)[1524](http://acm.hdu.edu.cn/showproblem.php?pid=1524)[1729](http://acm.hdu.edu.cn/showproblem.php?pid=1729)

```cpp
#include <stdio.h>
#include <math.h>
int main()
{
    int a,b,k;
    double eqa = (1+sqrt(5.0))/2.0;
    while( scanf("%d%d",&a,&b)!=EOF )
    {
        if( a > b )//交换a，b
        {
            a^=b;
            b^=a;
            a^=b;
        }
        k=b-a;
        if( int( k*eqa )==a )  printf("0\n");
        else    printf("1\n");//先手必胜
    }
    return 0;
}
```

# [HDU2177](http://acm.hdu.edu.cn/showproblem.php?pid=2177)

# 取(2堆)石子游戏

****Time Limit: 3000/1000 MS (Java/Others)    Memory Limit: 32768/32768 K (Java/Others)Total Submission(s): 3877    Accepted Submission(s): 2393****

**Problem Description**

有两堆石子，数量任意，可以不同。游戏开始由两个人轮流取石子。游戏规定，每次有两种不同的取法，一是可以在任意的一堆中取走任意多的石子；二是可以在两堆中同时取走相同数量的石子。最后把石子全部取完者为胜者。现在给出初始的两堆石子的数目，如果轮到你先取，假设双方都采取最好的策略，问最后你是胜者还是败者。如果你胜，你第1次怎样取子? 

**Input**

输入包含若干行，表示若干种石子的初始情况，其中每一行包含两个非负整数a和b，表示两堆石子的数目，a和b都不大于1,000,000，且a<=b。a=b=0退出。

**Output**

输出也有若干行，如果最后你是败者，则为0，反之，输出1，并输出使你胜的你第1次取石子后剩下的两堆石子的数量x,y,x<=y。如果在任意的一堆中取走石子能胜同时在两堆中同时取走相同数量的石子也能胜，先输出取走相同数量的石子的情况.

**Sample Input**

1 2 

5 8 

4 7 

2 2 

0 0

**Sample Output**

0 

1 

4   7 

3   5 

0 

1

0 0 

1 2

**Author**

Zhousc

**Source**

[ECJTU 2008 Summer Contest](http://acm.hdu.edu.cn/search.php?field=problem&key=ECJTU+2008+Summer+Contest&source=1&searchmode=source)

### 解题思路：

(a,b)奇异态先手必败，非奇异态先手必胜。先判断初始状态，如果为奇异态，则先手必败，否则，要输出先手第一次取石子后，两堆石子各剩下多少个，即由非奇异态转到奇异态。 做法是枚举所有的状态。比如测试数据中的 初始 5  8 是非奇异态，

则先手第一次取石子两堆可能剩下的状态有

一。两堆取相同的石子数

4  7

3  6

2  5

1  4

0   3

二。在第一堆里取

4 8

3 8

2 8

1 8

0 8

三。在第二堆里取

5 7

5 6

5 5

5 4

5 3

5 2

5 1

5 0

代码：


```cpp
import java.util.*;
import java.math.*;

public class Main{
	static int MAXN=(int)(1e6+10);
	static boolean judge(int a,int b) {//判断奇异局势
		double epa=(1+Math.sqrt(5.0))/2.0;
		if(a>b) {
			a^=b;
			b^=a;
			a^=b;
		}
		int k=b-a;
		if((int)(k*epa)==a)//奇异局势
			return true;
		return false;
	}
	public static void main(String[] args) {
		Scanner cin=new Scanner(System.in);
		
		while(cin.hasNext()) {
			int a=cin.nextInt();
			int b=cin.nextInt();
			if(a==0&&b==0)	break;
			
			if(judge(a,b)) {
				System.out.println(0);
				continue;
			}
			System.out.println(1);
			int i=a,j=b;
			while(i>=0&&j>=0) {
				i--;j--;
				if(judge(i,j))
					System.out.println(i+" "+j);
			}
			i=a;j=b;
			while(i>=0) {
				i--;
				if(judge(i,j))
					System.out.println(i+" "+j);
			}
			if(a==b)	continue;
			i=a;j=b;
			while(j>=0) {
				j--;
				if(judge(i,j))
					System.out.println(j+" "+i);
			}
		}
		cin.close();
	}
}
```

**（三）斐波那契博弈（Fibonacci Nim）**

**有一堆个数为 n 的石子，游戏双方轮流取石子，满足**

**1)先手不能在第一次把所有的石子取完；**

**2)之后每次可以取的石子数介于 1 到对手刚取的石子数的 2 倍之间（包含 1 和对手刚取的石子数的 2 倍）。**

**约定取走最后一个石子的人为赢家，求必败态。**

这个和之前的 Wythoff’s Game 和取石子游戏 有一个很大的不同点，就是游戏规则的动态化。之前的规则中，每次可以取的石子的策略集合是基本固定的，但是这次有规则 2：一方每次可以取的石子数依赖于对手刚才取的石子数。

**胜利法则：先手胜当且 仅当 n 不是 Fibonacci 数。换句话说，必败态构成 Fibonacci 数列。**

证明：

这里需要借助“Zeckendorf定理”（齐肯多夫定理）：任何正整数可以表示为若干个不连续的 Fibonacci 数之和。

FIB 数列的必败证明：

1、当 i=2 时，先手只能取 1 颗，显然必败，结论成立。

2、假设当 i<=k 时，结论成立。则当 i=k+1 时，f[i] = f[k]+f[k-1]。

### **[HDU2516](http://acm.hdu.edu.cn/showproblem.php?pid=2516)**

# 取石子游戏

****Time Limit: 2000/1000 MS (Java/Others)    Memory Limit: 32768/32768 K (Java/Others)Total Submission(s): 7842    Accepted Submission(s): 4734****

**Problem Description**

1堆石子有n个,两人轮流取.先取者第1次可以取任意多个，但不能全部取完.以后每次取的石子数不能超过上次取子数的2倍。取完者胜.先取者负输出"Second win".先取者胜输出"First win".

**Input**

输入有多组.每组第1行是2<=n<2^31. n=0退出.

**Output**

先取者负输出"Second win". 先取者胜输出"First win". 

参看Sample Output.

**Sample Input**

2 

13 

10000 

0

**Sample Output**

Second win 

Second win 

First win

**Source**

[ECJTU 2008 Autumn Contest](http://acm.hdu.edu.cn/search.php?field=problem&key=ECJTU+2008+Autumn+Contest&source=1&searchmode=source)

**Recommend**

lcy   |   We have carefully selected several similar problems for you:  [2509](http://acm.hdu.edu.cn/showproblem.php?pid=2509)[2512](http://acm.hdu.edu.cn/showproblem.php?pid=2512)[1536](http://acm.hdu.edu.cn/showproblem.php?pid=1536)[1907](http://acm.hdu.edu.cn/showproblem.php?pid=1907)[1524](http://acm.hdu.edu.cn/showproblem.php?pid=1524)

```cpp
#include <stdio.h>
#include <string.h>
#include <iostream>
using namespace std;
const int maxn=1e6+10;
int a[maxn];
int main()
{
    int i;
    a[0]=a[1]=1;
    for(i=2;i<maxn;i++)
        a[i]=a[i-1]+a[i-2];
    int n;
    while(scanf("%d",&n)!=EOF&&n){
        bool flag=0;
        for(int i=1;i<maxn;i++)
        {
            if(a[i]==n){
                flag=1;break;
            }
        }
        if(flag)    printf("Second win\n");
        else printf("First win\n");
    }

	return 0;
}
```

### （四）K倍博弈

**共 n 个石子，两个人按顺序依次取石子。先手不能全部取完，之后每人取的个数不能超过另一个人上轮取的 K倍。 对于给定的 n, k， 先手是否有必胜的策略。**

1.当 k=1 的时候 可知必败局面都是 2^i 将 n 分解成二进制，然后先手取掉最后一个 1，然后对方必然无法去掉更高的 1，而对方取完我方至少还能拿掉最后一 个 1 导致对方永远取不完。

2.当 k=2 的时候，必败局面都是斐波那契数列。利用“先手去掉最后一个 1，则后手必不能去掉更高阶的 1 导致取不完”的思想，斐波那契数列有一个非常好 的性质就是：任意一个整数可以写成斐波那契数列中的不相邻的项的和，于是将 n 写成这种形式，先取走最后一个 1，对方能取的数是这个数*2，小于高 2 位的 1，所以取不完。

3.当 K 取任意非0正值的时候， 想办法构造数列，将 n 写成数列中一些项的和，使得这些被取到的项的相邻两个倍数差距>k 那么每次去掉最后一个 1 还是符合上面的条件。

设这个数列已经被构造了 i 项，第 i 项为 a[ i ]，前 i 项可以完美对 1..b[ i ] 编码使得每个编码的任意两项倍数>K 那么有 a[ i+1 ] = b[ i ] + 1;这是显然的 因为 b[ i ] + 1 没法构造出来，只能新建一项表示。然后计算 b[ i+1] 既然要使用 a[ i+1 ] ，那么下一项最多只能是某个 a[ t ] 使得 a[ t ] * K < a[ i+1 ]， 于是b[ i ] = b[ t ] + a[ i+1 ] ，然后判断 n 是否在这个数列里面如果在，那么先手必败。否则不停的减掉数列 a 中的项构造出 n 的分解，最后一位就是了。

### **[POJ3922](http://poj.org/problem?id=3922)**

A simple stone game
|**Time Limit:** 1000MS||**Memory Limit:** 65536K|
|----|----|----|
|**Total Submissions:** 631||**Accepted:** 315|

Description

After he has learned how to play Nim game, Mike begins to try another stone game which seems much easier. 


The game goes like this: Two players start the game with a pile of n stones. They take stones from the pile in turn and every time they take at least one stone. The one who goes first can take at most n-1 stones for his first move. From then on a player can take at most k times as many stones as his opponent has taken last time. For example, if one player take m stones in his turn, then the other player can take at most k * m stones next time. The player who takes the last stone wins the game. Suppose that those two players always take the best moves and never make mistakes, your job is to find out who will definitely win the game.

Input

The first line contains a integer t, indicating that there are t test cases following.(t<=20). 

Each test case is a line consisting of two integer n and k.(2<=n<=108,1<=k<=105).

Output

For each test case, output one line starting with “Case N: ”, N is the case number. And then, if the first player can ensure a winning, print the minimum number of stones he should take in his first turn. Otherwise, print "lose". Please note that there is a blank following the colon.

Sample Input

```
5 
16 1 
11 1 
32 2 
34 2 
19 3
```

Sample Output

```
Case 1: lose
Case 2: 1
Case 3: 3
Case 4: lose
Case 5: 4
```

Hint

When k = 1, the first player will definitely lose if the initial amount of stones is in the set {2, 4, 8, 16, 32, ...}. Let's call this kind of set “first-player-lose set”. 


When k = 2, the first-player-lose set is {2, 3, 5, 8, 13, 21, 34, 57 ...} , which happens to be the Fibonacci sequence starting from 2.

Source

[Beijing 2008](http://poj.org/searchproblem?field=source&key=Beijing+2008)

```cpp
#include <stdio.h>
#define N 20000005
 
int a[N], b[N];			// a 为数列， b 保存 a[0...i] 能构造出的最大的数
 
int main()
{
    int n, k;
    int loop = 0, casei = 1;
	scanf("%d",&loop);
	while(loop --){
        scanf("%d%d",&n,&k);
        a[0] = b[0] = 1;
        int i = 0, j = 0;
 
        while(n > a[i]){			// 构建数列
            i ++;
            a[i] = b[i - 1] + 1;
            while(a[j + 1] * k < a[i])
                j ++;
            if(k * a[j] < a[i])
                b[i] = b[j] + a[i];
            else
				b[i] = a[i];
        }
 
        printf("Case %d: ", casei ++);
        if(n == a[i])
			printf("lose\n");
        else{//win
            int ans;
            while(n){
                if(n >= a[i]){		// 构成 n 的最小的数列中的数字，即为第一次要取的数字
                    n -= a[i];
                    ans = a[i];
                }
                i --;
            }
            printf("%d\n",ans);
        }
    }
 
    return 0;
}
```





