# HDU3951 创建对称局势【博弈水题】2018年ACM-ICPC 南京亚洲区域赛 A题 - 紫芝的博客 - CSDN博客





2018年11月04日 18:55:45[紫芝](https://me.csdn.net/qq_40507857)阅读数：29








## Coin Game

After hh has learned how to play Nim game, he begins to try another coin game which seems much easier. 

The game goes like this: 

Two players start the game with a circle of n coins. 

They take coins from the circle in turn and every time they could take 1~K continuous coins. 

(imagining that ten coins numbered from 1 to 10 and K equal to 3, since 1 and 10 are continuous, you could take away the continuous 10 , 1 , 2 , but if 2 was taken away, you couldn't take 1, 3, 4, because 1 and 3 aren't continuous) 

The player who takes the last coin wins the game. 

Suppose that those two players always take the best moves and never make mistakes. 

Your job is to find out who will definitely win the game.

Input

The first line is a number T(1<=T<=100), represents the number of case. The next T blocks follow each indicates a case. 

Each case contains two integers N(3<=N<=10 9,1<=K<=10).

Output

For each case, output the number of case and the winner "first" or "second".(as shown in the sample output)

Sample Input

```
2
3 1
3 2
```

Sample Output

```
Case 1: first
Case 2: second
```

### 题意：

**给你一串有连续编号石子，首尾连成一个环（即1和N相邻），每次可以取相邻的1~K个石子，最后取完者胜**

### 分析：
- 若先手一次可以取完（N<=K)，先手必胜
- 若K=1，显然N为奇数，先手必胜
- 若K>=2，后手只需要创建一个对称局势，即把这条链分成两条相等的链，先手必败

```java
import java.util.*;
import java.math.*;

public class Main{
	static int MAXN=(int)(2e5+10);
	static long n;
	
	public static void main(String[] args) {
		Scanner cin=new Scanner(System.in);
		int T=cin.nextInt();
		int ca=1;
		while((T--)!=0) {
			n=cin.nextLong();
			int k=cin.nextInt();
			System.out.print("Case "+ ca++ +": ");
			if(n<=k) {
				System.out.println("first");
				continue;
			}
			if(k==1) {
				if(n%2==1)
				System.out.println("first");
				else
				System.out.println("second");
				continue;
			}
			System.out.println("second");
		}
		cin.close();
	}
}
```

# 2018年ACM-ICPC 南京亚洲区域赛

# [A.Adrien and Austin](https://nanti.jisuanke.com/t/33670)

### **题意**

**有一堆数量为N的石子，石子编号从1⋯N排成一列，两个人玩游戏，每次可以取1⋯K个连续编号的石子，Adrien先手，如果有谁不能取了则他输，两个人为Adrien 和Austin**

## 思路

**当K为1时显然的和N的奇偶性有关，那么我们考虑一下K>1的情况对于先手的Adrien来说，他对任意的N颗石子，他都可以将这N颗石子分成两段相当数量的石子(从中间开始拿对于奇数拿一颗，对于偶数拿两颗)，那么剩下的两段相当于是独立的两个相同的游戏了，如果后手拿其中一段，那么我先手就对另一段进行这样相同的拆分，那么能保证我先手总是有石子能拿，所以我先手必胜，也就是说当K>1的时候先手必胜，K==1的时候判断奇偶，N==0的时候特判即可**

```cpp
#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <math.h>
#include <stack>
#include <list>
int main()
{
    int n,k;
    scanf("%d%d",&n,&k);
    if(n==0)
    {
        printf("Austin\n");
    }
    else if(k==1)
    {
        if(n%2==1)
            printf("Adrien\n");
        else
            printf("Austin\n");
    }
    else
        printf("Adrien\n");
    return 0;
}
```





