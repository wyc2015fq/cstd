# 双向BFS—>NOIP2002 字串变换 - 在思索中前行！ - CSDN博客





2015年09月17日 21:01:20[_Tham](https://me.csdn.net/txl16211)阅读数：910








如果目标也已知的话，用双向BFS能很大提高速度

单向时，是 b^len的扩展。

双向的话，2*b^(len/2)  快了很多，特别是分支因子b较大时

至于实现上，网上有些做法是用两个队列，**交替节点搜索 ×**，如下面的伪代码：
while(!empty())

    {

            扩展正向一个节点

           遇到反向已经扩展的return

            扩展反向一个节点      

            遇到正向已经扩展的return      

      }

**但这种做法是有问题**的，如下面的图：


![](http://pic002.cnblogs.com/images/2011/306117/2011072513131553.jpg)


求S-T的最短路，交替节点搜索（**一次正向节点，一次反向节点**）时

Step 1 : S –> 1 , 2

Step 2 : T –> 3 , 4

Step 3 : 1 –> 5

Step 4 : 3 –> 5   返回最短路为4，错误的，事实是3，S-2-4-T




**我想，正确做法的是交替逐层搜索**，保证了不会先遇到非最优解就跳出，而是**检查完该层所有节点，得到最优值**。

也即如果该层搜索遇到了对方已经访问过的，**那么已经搜索过的层数就是答案了**，可以跳出了，以后不会更优的了。

当某一边队列空时就无解了。



**优化：**提供速度的关键在于使状态扩展得少一些，所以**优先选择队列长度较少的去扩展**，保持两边队列长度平衡。这比较适合于两边的扩展情况不同时，一边扩展得快，一边扩展得慢。如果两边扩展情况一样时，加了后效果不大，不过加了也没事。

无向图时，两边扩展情况类似。有向图时，注意反向的扩展是**反过来**的 **x->y**（NOIP2002G2字串变换）



```cpp
/*
	双向BFS：
	1.正向搜索：从初始结点向目标结点方向搜索，按照正向规则(A$->B$)变换。
    2.逆向搜索：从目标结点向初始结点方向搜索，按照逆向规则(B$->A$)变换。
    当两个方向的搜索生成同一子结点时终止此搜索过程(变换的总步数为此时两个方向BFS的步数总和)。

	双向搜索通常有两种方法：
	1. 两个方向交替扩展。
	2. 选择结点个数较少的那个方向先扩展。
	方法2克服了两方向结点的生成速度不平衡的状态,明显提高了效率。本程序使用方法1，两个方向交替BFS，进行正反规则变换。
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
using namespace std;
struct  Node
{
    char str[41];
    int sep;
    Node()
    {
    	sep=0;
    	memset(str,sizeof(str),0);
	}
    
}q1[20000], q2[20000];

int h1, r1, h2, r2;             //h1,r1...h2,r2,分别保存起始和目标两个状态的队列头和尾
char s1[6][25], s2[6][25];      //存储变换规则

int n,Min=100;                  //Min储存最少规则变换次数

void copy2(int start, int use)  //逆向搜索，当q2[]搜索到逆向规则匹配的字串B$的时候，进行替换
{
    int i, j;
    r2++;                       //队列中元素增加1个字串
    q2[r2].sep = q2[h2].sep + 1;
    for(i = 0; i < start; i++)  //替换匹配规则的字串
	{
        q2[r2].str[i] = q2[h2].str[i];
    }
    for(j = 0; s1[use][j] != '\0'; j++, i++)
	{
        q2[r2].str[i] = s1[use][j];
    }
    for(j = start + strlen(s2[use]); q2[h2].str[j] != '\0'; j++, i++)
	{
        q2[r2].str[i] = q2[h2].str[j];
    }
    //cout<<"q2:"<<q2[r2].str<<endl;
    for(i = 0; i <= r1; i++)    //判断这一次规则替换操作结束后，正向BFS队列中是否会有字串元素与之匹配
	{
        if(strcmp(q1[i].str, q2[r2].str) == 0)
		{
            printf("%d\n", q1[i].sep + q2[r2].sep);
            exit(0);
        //  if(q1[i].sep+q2[r2].sep<Min) Min=q1[i].sep+q2[r2].sep;
        }
    }
}

void copy1(int start, int use)      //正向搜索，当q1[]搜索到与规则匹配的字串A$的时候，进行替换
{
    int i, j;
    r1++;                          
    q1[r1].sep = q1[h1].sep + 1;    //队列q1[]中增加1个新的元素(一次规则变换后的字串)
    for(i = 0; i < start; i++)         //规则替换操作
	{
        q1[r1].str[i] = q1[h1].str[i];
    }
    for(j = 0; s2[use][j] != '\0'; j++, i++)
	{
        q1[r1].str[i] = s2[use][j];
    }
    for(j = start + strlen(s1[use]); q1[h1].str[j] != '\0'; j++, i++)
	{
        q1[r1].str[i] = q1[h1].str[j];
    }
    //cout<<"q1:"<<q1[r1].str<<endl;
    for(i = 0; i <= r2; i++)            //判断这一次规则替换操作结束后，方向BFS队列中是否会有字串元素与之匹配
	{
        if(strcmp(q2[i].str, q1[r1].str) == 0)
		{

            printf("%d\n", q2[i].sep + q1[r1].sep);
            exit(0);
        //  if(q2[i].sep+q1[r1].sep<Min) Min=q2[i].sep+q1[r1].sep;
        }
    }
}

void work(void)
{
    int i, j;
    while(h1 <= r1 && h2 <= r2)           //搜索过程中确保没有一个队列为空，否则搜索不到相交的情况
	{

        if(q1[h1].sep + q2[h2].sep > 10)  //正反搜索的步数总和超过了10，说明这样的转换至少要超过10次才能实现，结束
		{
            printf("NO ANSWER!\n");
            exit(0);
        }

        for(i = 0; i < strlen(q1[h1].str); i++)
		{
            for(j = 0; j < n; j++)       //正向搜索，一共n个变换规则
			{
                if(strncmp(s1[j], &q1[h1].str[i], strlen(s1[j])) == 0)
				{
                    copy1(i, j);
                }
            }
        }
        h1++;       //正向一遍BFS，搜索完所有规则之后，队首元素出队
        
        for(i = 0; i < strlen(q2[h2].str); i++)        //加快搜索的速度，同理从目标开始，方向，并根据逆向规则进行BFS
		{
            for(j = 0; j < n; j++)
			{
                if(strncmp(s2[j], &q2[h2].str[i], strlen(s2[j])) == 0)
				{
                    copy2(i, j);
                }
            }
        }
        h2++;
    }
}

int main()
{
	freopen("in.in", "r", stdin);
//	freopen("string.out", "w", stdout);
    scanf("%s%s", q1[0].str, q2[0].str);
    while(scanf("%s%s", s1[n], s2[n]) == 2)
	{
        n++;
    }
    work();
    printf("NO ANSWER!\n");
    return 0;
}
```








