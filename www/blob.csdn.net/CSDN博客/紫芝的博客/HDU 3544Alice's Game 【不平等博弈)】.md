# HDU 3544Alice's Game 【不平等博弈)】 - 紫芝的博客 - CSDN博客





2018年11月07日 10:51:41[紫芝](https://me.csdn.net/qq_40507857)阅读数：300
个人分类：[博弈论](https://blog.csdn.net/qq_40507857/article/category/7632032)











Alice and Bob have got a lot of chocolates. All the chocolates are rectangles of different shapes as X i * Y i.They decide to play an interesting game on the chocolates. They take turns choose a chocolate and split it into two pieces. The one who can not take operations lose. Due to the game is too simple, the additional rules apply. Alice is only allowed to split the chocolate vertically, and Bob is only allowed to split the chocolate horizontally. 

Specifically, for Alice, a chocolate X i * Y i, can only split into A * Y i, and B * Y i where A + B = X i and A, B > 0. And for Bob, a chocolate X i * Y i, can only split into X i * A, and X i * B where A + B = Y i and A, B > 0. 

Alice and Bob are clever enough to take the optimal operation, if Alice plays first, your are to decide who will win.

Input

The input contains multiple test cases. The first line of input contains a single integer denoting the number of test cases. 

For each test case, the first line contains an integer N, the number of pieces of chocolates. (1 <= N <= 100) 

For the next N lines, each line contains two integers X i and Y i, denoting the chocolate sized X i * Y i. (1 <= X i, Y i <= 1000000000)

Output

For each test cases, output "Alice" when Alice will win, and "Bob" otherwise. See sample test cases for further details.

Sample Input

4
1
1 1
1
2 1
2
2 2
2 1
1
3 2
Sample Output

Case 1: Bob
Case 2: Alice
Case 3: Alice
Case 4: Bob
**题意**

给一块n*m的巧克力，Alice只能垂直切，切成A*m和B*m,并且A+B=n,Bob只能横切，只能切成A*n和B*n，并且A+B=m。 

Alice先手，谁不能切则为输

**题解**

http://www.cnblogs.com/AOQNRMGYXLMV/p/4462791.html 

原内容如下

切巧克力的游戏，想得还是不是太明白。

后者会尽量选前着切后其中小的一块来切，那么先手须尽量取中间来切。

So？题解都是这么一句话，不知道是真懂了还是从别人那抄过来的。 

后来找到一个官方题解，分析得比较认真，但我这智商还是没懂太多，QAQ

本题我抄袭自《Winning Ways for your Mathematical Plays》 ，一本关于游戏论的科 

普类图书。 

这题是一个组合游戏，但是并不是一个对等的组合游戏，所以试图使用 SG 函数相关知 

识解答是会面临巨大的挑战的。 书中本题的做法描述得十分简单， 当然对于有这类组合游戏 

知识的同学来说这题也确实十分简单，如果没有相关背景知识，也没有关系，我们来慢慢分 

析这道题目。 

要成功地解答本题需要认真地分析这个游戏的规则，我们首先来考虑一些简单情况。 
- （1） 只有 n*1 和 1*m 的巧克力的时候 
- （2） 2*2 的巧克力 
- （3） 2*3 和 3*2 的巧克力 
- （4） n*2 和 2*m 的巧克力 
- （5） n*3 和 3*m 的巧克力 
- （6） 很多巧克力在一起的情况 

我们来一个一个分析这些情况，对于 n*1 和 1*m 的巧克力，显然 n*1 的巧克力对 alice 

有利， 而 1*m 的巧克力对 bob 有利。 假设 n*1 对于 alice 有 n-1 的 HP 贡献， 而 1*m 对于 bob 

有 m-1 的 HP 贡献。至于谁胜利？自然是谁 HP 多谁就胜利，当然考虑到先 alice 先扣 HP， 

所以如果 HP 一样多， alice 也输了。 为了方便， 我们定义 alice 的 HP 为正， bob 的 HP 为负。 

于是这个局面就可以通过简单的加法获得总的 HP 了。 

那 2*2 的巧克力呢， 认真分析就可以发现 2*2 在这个游戏中纯属幻觉！ 谁也不愿意先拿 

他开刀，切一道送了对方两次切的机会，而自己却只切了一刀。于是我们可以说，2*2 的巧 

克力值 0 的 HP。 

同样 2*3 和 3*2 的巧克力也因为同样的道理就这么被无情地抛弃了。 

对于 n*2 的巧克力，根据直觉 alice 应该感到很高兴（当然不是 1*2） ，bob 自然不会傻 

到来切这个巧克力， 于是 alice 自己要想办法自己尽量多切几刀， 注意到切出 1*2 的巧克力 

是很不利的事情，于是每次都切 2*2 的，可以切(n/2)-1 刀。于是这就是 n*2 的巧克力的 HP 

贡献了。2*m 以及 n*3，3*m 的就不再赘述，都是一样。 

以此类推，4*4，8*8，16*16，都是比较关键的巧克力。

弄一个表吧，再找不到规律„„

![](https://img-blog.csdnimg.cn/20181107104905124.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3,size_16,color_FFFFFF,t_70)

看表格就知道   ![HP(i,j)=HP(\frac{i}{2},\frac{j}{2})](https://private.codecogs.com/gif.latex?HP%28i%2Cj%29%3DHP%28%5Cfrac%7Bi%7D%7B2%7D%2C%5Cfrac%7Bj%7D%7B2%7D%29)

然后HP(1,j)和HP(i,1)都是直接求的。

```java
import java.util.*;
import java.math.*;

public class Main{
	public static void main(String[] args) {
		Scanner cin=new Scanner(System.in);
		int T=cin.nextInt();
		int ca=1;
		while((T--)!=0) {
			int n=cin.nextInt();
			long a=0,b=0;
			for(int i=0;i<n;i++) {
				long x=cin.nextLong();
				long y=cin.nextLong();
				while(x>1&&y>1) {
					x=x>>1;
					y=y>>1;
				}
				if(y==1)	a+=(x-1);
				if(x==1)	b+=(y-1);
			}
			System.out.print("Case "+ ca++ +": ");
			System.out.println((a>b)?"Alice":"Bob");
		}
		cin.close();
	}
}
```





