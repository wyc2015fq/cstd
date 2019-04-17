# 【字符串计算】#STC 1 A. Ball Game - CD's Coding - CSDN博客





2014年07月29日 01:51:07[糖果天王](https://me.csdn.net/okcd00)阅读数：529

















A kindergarten teacher Natalia Pavlovna has invented a new ball game. This game not only develops the children's physique, but also teaches them how to count.


The game goes as follows. Kids stand in circle. Let's agree to think of the children as numbered with numbers from 1 to *n* clockwise
 and the child number 1 is holding the ball. First the first child throws the ball to the next one clockwise, i.e. to the child number 2.
 Then the child number 2 throws the ball to the next but one child, i.e. to the child number 4,
 then the fourth child throws the ball to the child that stands two children away from him, i.e. to the child number 7, then the ball is thrown to the child who stands 3 children
 away from the child number 7, then the ball is thrown to the child who stands 4 children
 away from the last one, and so on. It should be mentioned that when a ball is thrown it may pass the beginning of the circle. For example, if *n* = 5, then
 after the third throw the child number 2 has the ball again. Overall, *n* - 1 throws
 are made, and the game ends.


The problem is that not all the children get the ball during the game. If a child doesn't get the ball, he gets very upset and cries until Natalia Pavlovna gives him a candy. That's why Natalia Pavlovna asks you to help her to identify the numbers of the children
 who will get the ball after each throw.




Input


The first line contains integer *n* (2 ≤ *n* ≤ 100) which
 indicates the number of kids in the circle.




Output


In the single line print *n* - 1 numbers which are the numbers of children who will get the ball after each throw. Separate the numbers by spaces.




Sample test(s)




input
10




output
2 4 7 1 6 2 9 7 6




input
3




output
2 1


















C*++语言和C++语言非常相似，然而C*++的程序有时会出现意想不到的结果。比如像这样的算术表达式：

　　表达式=基本式 / 表达式+基本式 / 表达式-基本式

　　基本式=增量 / 系数*增量

　　增量=a++ / ++a

　　系数=0/1/2/……/1000

　　如“5*a++-3*++a+a++”是合法的C*++表达式。

　　计算这样的表达式的值的方法：首先是每个基本式进行计算，然后按照正常的算术运算法则计算。如果一个基本式包含“a++”，则先进行乘法运算再使变量a权值+1；如果一个基本式包含“++a”，则先使变量a权值+1再进行乘法运算。

　　然而基本式可以按任意顺序计算，这就是为什么计算结果是完全无法预料的。

　　你的任务就是去找到最大的可能结果。

　　第一行，一个整数n，表示变量a的初始值。

　　第二行，一个合法的C*++表达式。

　　共一行，一个整数ans，表示最大可能结果。


From [weixinding](http://my.csdn.net/weixinding)@CSDN

Code：



```cpp
#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
bool flag;
int a,n,i,j,k,x,o,ans;
int w[10011],tot;
char s[100011];
int main()
{
		scanf("%d\n%s",&a,s+1);
		n=strlen(s+1);
		if (s[1]!='-')
		{
			 for (i=n+1;i>1;i--) s[i]=s[i-1];
			 s[1]='+';
			 n++;
	  }
		for (i=1;i<=n;i+=3)
		{
				if (s[i++]=='+') k=1;
				else						 k=-1;
				j=0;
				while (i<=n&&'0'<=s[i]&&s[i]<='9') (j*=10)+=s[i++]-48;
				if (s[i]=='*') i++;
				else					 j=1;
				w[++tot]=(k*=j);
				ans+=(a-(s[i]=='a'))*k;
		}
		sort(w+1,w+tot+1);
		for (i=1;i<=tot;i++) ans+=i*w[i];
	 	cout<<ans;
	return 0;
}
```










