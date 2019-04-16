# HDU4809 Wow! Such City! Dijkstra算法 - 紫芝的博客 - CSDN博客





2018年05月18日 17:03:30[紫芝](https://me.csdn.net/qq_40507857)阅读数：66
个人分类：[最短路算法](https://blog.csdn.net/qq_40507857/article/category/7617885)









[点击打开链接](http://acm.hdu.edu.cn/showproblem.php?pid=4849)



# Wow! Such City!
**Time Limit: 15000/8000 MS (Java/Others)    Memory Limit: 102400/102400 K (Java/Others)Total Submission(s): 2366    Accepted Submission(s): 808**


Problem Description

Doge, tired of being a popular image on internet, is considering moving to another city for a new way of life.
In his country there are N (2 ≤N≤ 1000) cities labeled 0 . . . N - 1. He is currently in city 0. Meanwhile, for each pair of cities, there exists a road connecting them, costing Ci,j (a positive integer) for traveling from city i to city j. Please note that Ci,j may not equal to Cj,i for any given i ≠ j.
Doge is carefully examining the cities: in fact he will divide cities (his current city 0 is **NOT** included) into M (2 ≤ M ≤ 106) categories as follow: If the minimal cost from his current city (labeled 0) to the city i is Di, city i belongs to category numbered **Di mod M**.Doge wants to know the “minimal” category (a category with minimal number) which contains at least one city.
For example, for a country with 4 cities (labeled 0 . . . 3, note that city 0 is not considered), Doge wants to divide them into 3 categories. Suppose category 0 contains no city, category 1 contains city 2 and 3, while category 2 contains city 1, Doge consider category 1 as the minimal one.
Could you please help Doge solve this problem?

Note:

Ci,j is generated in the following way:
Given integers X0, X1, Y0, Y1, (1 ≤ X0, X1, Y0, Y1≤ 1234567), for k ≥ 2 we have
Xk  = (12345 + Xk-1 * 23456 + Xk-2 * 34567 + Xk-1 * Xk-2 * 45678)  mod  5837501
Yk  = (56789 + Yk-1 * 67890 + Yk-2 * 78901 + Yk-1 * Yk-2 * 89012)  mod  9860381
The for k ≥ 0 we have

Zk = (Xk * 90123 + Yk ) mod 8475871 + 1

Finally for 0 ≤ i, j ≤ N - 1 we have

Ci,j = Zi*n+j for i ≠ j
Ci,j = 0   for i = j





Input

There are several test cases. Please process till EOF.
For each test case, there is only one line containing 6 integers N,M,X0,X1,Y0,Y1.See the description for more details.




Output

For each test case, output a single line containing a single integer: the number of minimal category.




Sample Input


3 10 1 2 3 4
4 20 2 3 4 5





Sample Output


1
10

For the first test case, we have


	   0	   1	   2	   3	   4	   5	   6	   7	   8
X	   1	   2	 185180	 788997	1483212	4659423	4123738	2178800	 219267
Y	   3	   4	1633196	7845564	2071599	4562697	3523912	317737	1167849
Z	 90127	 180251	1620338	2064506	 625135	5664774	5647950	8282552	4912390

the cost matrix C is
	         	   0	 180251	1620338
	                2064506	   0	5664774
	                5647950	8282552	   0



*Hint*
So the minimal cost from city 0 to city 1 is 180251, while the distance to city 2 is 1620338.
Given M = 10, city 1 and city 2 belong to category 1 and 8 respectively.
Since only category 1 and 8 contain at least one city,
the minimal one of them, category 1, is the desired answer to Doge’s question.






Source

[2014西安全国邀请赛](http://acm.hdu.edu.cn/search.php?field=problem&key=2014%CE%F7%B0%B2%C8%AB%B9%FA%D1%FB%C7%EB%C8%FC&source=1&searchmode=source)




Recommend

liuyiding   |   We have carefully selected several similar problems for you:  [6275](http://acm.hdu.edu.cn/showproblem.php?pid=6275)[6274](http://acm.hdu.edu.cn/showproblem.php?pid=6274)[6273](http://acm.hdu.edu.cn/showproblem.php?pid=6273)[6272](http://acm.hdu.edu.cn/showproblem.php?pid=6272)[6271](http://acm.hdu.edu.cn/showproblem.php?pid=6271)




题解：

根据给出的计算公式，计算任意两点之间的距离，然后求出起点0到其他n-1个点的最短距离，

输出最短距离对m取余之后的最小值




```cpp
#include <iostream>
#include <cstdio>
#include <cstring>
#include <queue>
using namespace std;
typedef long long ll;
ll inf=0x3f3f3f3f;
ll x[1010*1010+1010],y[1010*1010+1010],z[1010*1010+1010];
int Map[1010][1010];
int ans[1010];//ans[i]表示起点到i的最短距离
int vis[1010];//vis[i]表示i是否在队列中
int n,m;

void init()
{   //根据MAP数组的计算公式，i的最大值为n*n
    for(int i=2; i<(n-1)*n+n; i++)
    {   //这里和题目给定的公式不同，实际计算需要边计算，边取模
        x[i]=(12345+x[i-1]*23456%5837501+x[i-2]*34567%5837501+(x[i-1]*x[i-2]%5837501)*45678%5837501)%5837501;
        y[i]=(56789+y[i-1]*67890%9860381+y[i-2]*78901%9860381+(y[i-1]*y[i-2]%9860381)*89012%9860381)%9860381;
    }
    for(int k=0; k<(n-1)*n+n; k++)
    {
        z[k]= (x[k]*90123%8475871+y[k])%8475871+1;
    }
    for(int i=0; i<n; i++)
    {
        for(int j=0; j<n; j++)
        {
            if(i==j)
            {
                Map[i][j]=0;
            }
            else
            {
                Map[i][j]= z[i*n+j];
            }
        }
    }

}

void spfa()
{
    for (int i=0; i<n; i++)
    {
        ans[i]=inf;
        vis[i]=0;
    }
    queue<int>q;
    vis[0]=1;
    ans[0]=0;
    q.push(0);
    while (!q.empty())
    {
        int s=q.front();
        q.pop();
        vis[s]=0;
        for (int i=0; i<n; i++)
        {
            if (ans[i]>Map[s][i]+ans[s])
            {
                ans[i]=Map[s][i]+ans[s];
                if (!vis[i])
                {
                    q.push(i);
                    vis[i]=1;
                }
            }
        }
    }
}


int main()
{
    while (~scanf("%d%d%lld%lld%lld%lld",&n,&m,&x[0],&x[1],&y[0],&y[1]))
    {
        init();
        spfa();
        int Min=ans[1]%m;
        for (int i=2; i<n; i++)
        {

            if (Min>ans[i]%m)
                Min=ans[i]%m;
        }
        printf ("%d\n",Min);
        /*
        for(int i=0;i<n;i++)
        printf("%d %d\n",ans[i],ans[i]%m);
        */
    }
    return 0;
}
```






