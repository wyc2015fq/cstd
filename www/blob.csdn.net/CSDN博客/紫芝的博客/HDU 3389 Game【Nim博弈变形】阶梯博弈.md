# HDU 3389 Game【Nim博弈变形】阶梯博弈 - 紫芝的博客 - CSDN博客





2018年11月05日 22:56:24[紫芝](https://me.csdn.net/qq_40507857)阅读数：130








Bob and Alice are playing a new game. There are n boxes which have been numbered from 1 to n. Each box is either empty or contains several cards. Bob and Alice move the cards in turn. In each turn the corresponding player should choose a non-empty box A and choose another box B that B<A && (A+B)%2=1 && (A+B)%3=0. Then, take an arbitrary number (but not zero) of cards from box A to box B. The last one who can do a legal move wins. Alice is the first player. Please predict who will win the game.

Input

The first line contains an integer T (T<=100) indicating the number of test cases. The first line of each test case contains an integer n (1<=n<=10000). The second line has n integers which will not be bigger than 100. The i-th integer indicates the number of cards in the i-th box.

Output

For each test case, print the case number and the winner's name in a single line. Follow the format of the sample output.

Sample Input

2
2
1 2
7
1 3 3 2 2 1 2
Sample Output

Case 1: Alice
Case 2: Bob
首先在纸上画了一下转移图：

![](https://images0.cnblogs.com/blog2015/640481/201504/251416559062797.png)

1 3 4号盒子是不能够再转移卡片到其他盒子中去了的，其他盒子中的卡片经过若干步的转移最终也一定会转移到1 3 4号盒子中去。

具体来说，n % 6 == 0 或 2 或 5的盒子，经过**奇数步**转移到1 3 4中去，其他的则须经过**偶数步**才能转移过去。



下面来证明，**所有卡片都在偶数步盒子中是必败状态**。

因为不论先手将偶数步的盒子中卡片移走了多少，后手一定可以把这些卡片再往前移动一个盒子，直到移到1 3 4中去为止。



对于**只有一个盒子有卡片，而且这个盒子是奇数步盒子来说，先手必胜**。

很简单，根据上面的结论，只要先手把这个奇数步盒子中所有卡片全部往下移一个盒子就好了。这样就转移到了先手必败状态。



整个游戏可以看做若干个子游戏的和游戏，偶数步盒子不予考虑，只考虑奇数步盒子中的卡片，这就相当于一个n堆石子的Nim游戏。

在一个奇数步盒子中移走k张卡片，相当于在某一堆石子中取走k个石子。把所有石子取完相当于，所有的卡片都在偶数步的盒子里面，而我们已经证明完这种状态是必败状态了。

所以在代码中就只需要将奇数步盒子中的卡片数异或一下求个Nim和，就能判断胜负了。

```java
import java.util.*;
import java.math.*;

public class Main{
	static int maxn=(int)(1e5+10);

	public static void main(String[] args) {
		Scanner cin=new Scanner(System.in);
		int T=cin.nextInt();
		int ca=1;
		while((T--)!=0) {
			int n=cin.nextInt();
			int ret=0;
			for(int i=1;i<=n;i++) {
				int x=cin.nextInt();
				if(i%6==0||i%6==2||i%6==5)
				ret^=x;
			}
			System.out.print("Case "+ ca++ +": ");
			if(ret==0)	
				System.out.println("Bob");
			else
				System.out.println("Alice");
		}
		cin.close();
	}
}
```





