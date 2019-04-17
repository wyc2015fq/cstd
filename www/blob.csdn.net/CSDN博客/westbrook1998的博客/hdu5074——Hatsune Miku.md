# hdu5074——Hatsune Miku - westbrook1998的博客 - CSDN博客





2018年09月30日 19:08:23[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：31








> 
Hatsune Miku is a popular virtual singer. It is very popular in both Japan and China. Basically it is a computer software that allows you to compose a song on your own using the vocal package.

Today you want to compose a song, which is just a sequence of notes. There are only m different notes provided in the package. And you want to make a song with n notes.
![在这里插入图片描述](https://odzkskevi.qnssl.com/81a91ea1b0eca30a2fd32ede2f9e2c42?v=1538079188)

Also, you know that there is a system to evaluate the beautifulness of a song. For each two consecutive notes a and b, if b comes after a, then the beautifulness for these two notes is evaluated as score(a, b).

So the total beautifulness for a song consisting of notes a 1, a 2, . . . , a n, is simply the sum of score(a i, a i+1) for 1 ≤ i ≤ n - 1.

Now, you find that at some positions, the notes have to be some specific ones, but at other positions you can decide what notes to use. You want to maximize your song’s beautifulness. What is the maximum beautifulness you can achieve?

Input

The first line contains an integer T (T ≤ 10), denoting the number of the test cases.

For each test case, the first line contains two integers n(1 ≤ n ≤ 100) and m(1 ≤ m ≤ 50) as mentioned above. Then m lines follow, each of them consisting of m space-separated integers, the j-th integer in the i-th line for score(i, j)( 0 ≤ score(i, j) ≤ 100). The next line contains n integers, a 1, a 2, . . . , a n (-1 ≤ a i ≤ m, a i ≠ 0), where positive integers stand for the notes you cannot change, while negative integers are what you can replace with arbitrary notes. The notes are named from 1 to m.

Output

For each test case, output the answer in one line.

Sample Input

2

5 3

83 86 77

15 93 35

86 92 49

3 3 3 1 2

10 5

36 11 68 67 29

82 30 62 23 67

35 29 2 22 58

69 67 93 56 11

42 29 73 21 19

-1 -1 5 -1 4 -1 -1 -1 4 -1

Sample Output

270

625
唉读题读了大半天还是看了题解才想清楚

就是给m种音符，然后给一段序列是n个音符，有一些音符是确定是某个音符，有一些-1就代表都可以，然后给一个m*m的矩阵代表每两种音符相邻的值，比如mp[i][j]就表示i音符下一个是j音符可以获得的值，然后要最大化这个值

dp的思想，定义dp[i][j]表示第i位放置j音符的最大值，这样最后的结果就是求出max(dp[n][k]) 枚举k从1到m

这样中间dp的过程就有四种状态转移了

分别是当前音符知道与否和上一个音符知道与否，如果是一个未知，就需要枚举中间的k，如果是两个未知，就需要两重循环枚举
代码：

```
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
int t,n,m;
const int N=105;
int mp[N][N];
int a[N];
//dp[i][j]表示i位置放j音符的最大值
int dp[N][N];
int main(void){
    scanf("%d",&t);
    while(t--){
        scanf("%d%d",&n,&m);
        for(int i=1;i<=m;i++){
            for(int j=1;j<=m;j++){
                scanf("%d",&mp[i][j]);
            }
        }
        for(int i=1;i<=n;i++){
            scanf("%d",&a[i]);
        }
        memset(dp,0,sizeof(dp));
        //从第二个音符开始，因为第一个音符没有上一个音符
        for(int i=2;i<=n;i++){
            //第一种状态转移，前后两个音符都确定
            if(a[i]>0 && a[i-1]>0){
                dp[i][a[i]]=dp[i-1][a[i-1]]+mp[a[i-1]][a[i]];
            }
            //第二种状态转移，已知当前音符，不知上一个音符，枚举上一个音符
            else if(a[i]>0 && a[i-1]<0){
                for(int k=1;k<=m;k++){
                    dp[i][a[i]]=max(dp[i][a[i]],dp[i-1][k]+mp[k][a[i]]);
                }
            }
            //第三种状态转移，已知上一个音符，不知当前音符，枚举当前音符
            else if(a[i]<0 && a[i-1]>0){
                for(int k=1;k<=m;k++){
                    dp[i][k]=max(dp[i][a[i]],dp[i-1][a[i-1]]+mp[a[i-1]][k]);
                }
            }
            //第四种状态转移，上一个音符和当前音符都不知
            else if(a[i]<0 && a[i-1]<0){
                for(int j=1;j<=m;j++){
                    for(int k=1;k<=m;k++){
                        dp[i][j]=max(dp[i][j],dp[i-1][k]+mp[k][j]);
                    }
                }
            }
        }
        int ans=0;
        for(int i=1;i<=m;i++){
            ans=max(ans,dp[n][i]);
        }
        printf("%d\n",ans);
    }
    return 0;
}
```






