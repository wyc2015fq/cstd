# HDU 2544 Floyd算法 - 紫芝的博客 - CSDN博客





2018年04月06日 18:55:39[紫芝](https://me.csdn.net/qq_40507857)阅读数：41








# 最短路
**Time Limit: 5000/1000 MS (Java/Others)    Memory Limit: 32768/32768 K (Java/Others)Total Submission(s): 80236    Accepted Submission(s): 34757**


Problem Description

在每年的校赛里，所有进入决赛的同学都会获得一件很漂亮的t-shirt。但是每当我们的工作人员把上百件的衣服从商店运回到赛场的时候，却是非常累的！所以现在他们想要寻找最短的从商店到赛场的路线，你可以帮助他们吗？






Input

输入包括多组数据。每组数据第一行是两个整数N、M（N<=100，M<=10000），N表示成都的大街上有几个路口，标号为1的路口是商店所在地，标号为N的路口是赛场所在地，M则表示在成都有几条路。N=M=0表示输入结束。接下来M行，每行包括3个整数A，B，C（1<=A,B<=N,1<=C<=1000）,表示在路口A与路口B之间有一条路，我们的工作人员需要C分钟的时间走过这条路。
输入保证至少存在1条商店到赛场的路线。





Output

对于每组输入，输出一行，表示工作人员从商店走到赛场的最短时间




Sample Input


2 11 2 33 31 2 52 3 53 1 20 0





Sample Output


32




```cpp
/*给不连接的点赋无穷大，然后每两个点都去更新，
把两点可走的最短路都找出来，则时间复杂度O(n^3)
*/ 
#include<stdio.h>
#include<string.h>

int map[105][105];

void floyd(int n)
{
     for(int k = 1 ; k <= n ; k++ )
     for(int i = 1 ; i <= n ; i++ )
     for(int j = 1 ; j <= n ; j++ )     
     if(map[i][j]>map[i][k]+map[k][j])  
  //i j 两点，和i 到 k 然后再从 k 到 j  距离的比较。   
     map[i][j] = map[i][k]+map[k][j]; //核心代码   
}

int main()
{   
    int  n , m ; 
    while(scanf("%d%d",&n,&m)!=EOF&&n&&m)
    {
     memset(map,0x3f,sizeof(map));   
	  //给不连接的点赋无穷大 
     
     for(int i = 1 ; i <= n ; i++)
     map[i][i] = 0 ;       //自己与自己无权值 
     
     while(m--)
     {
      int a , b ,dis;
      scanf("%d%d%d",&a,&b,&dis);
      map[a][b] = map[b][a] = dis;
     }
     
     floyd(n);
     
     printf("%d\n",map[1][n]);
//标号为1的路口是商店所在地
//标号为N的路口是赛场所在地
    }
    
    return 0;
}
```




