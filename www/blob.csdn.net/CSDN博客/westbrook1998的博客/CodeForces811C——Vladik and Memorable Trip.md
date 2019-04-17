# CodeForces811C——Vladik and Memorable Trip - westbrook1998的博客 - CSDN博客





2018年10月02日 23:18:23[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：49标签：[dp																[预处理](https://so.csdn.net/so/search/s.do?q=预处理&t=blog)](https://so.csdn.net/so/search/s.do?q=dp&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
Vladik often travels by trains. He remembered some of his trips especially well and I would like to tell you about one of these trips:

Vladik is at initial train station, and now n people (including Vladik) want to get on the train. They are already lined up in some order, and for each of them the city code ai is known (the code of the city in which they are going to).

Train chief selects some number of disjoint segments of the original sequence of people (covering entire sequence by segments is not necessary). People who are in the same segment will be in the same train carriage. The segments are selected in such way that if at least one person travels to the city x, then all people who are going to city x should be in the same railway carriage. This means that they can’t belong to different segments. Note, that all people who travel to the city x, either go to it and in the same railway carriage, or do not go anywhere at all.

Comfort of a train trip with people on segment from position l to position r is equal to XOR of all distinct codes of cities for people on the segment from position l to position r. XOR operation also known as exclusive OR.

Total comfort of a train trip is equal to sum of comfort for each segment.

Help Vladik to know maximal possible total comfort.

Input

First line contains single integer n (1 ≤ n ≤ 5000) — number of people.

Second line contains n space-separated integers a1, a2, …, an (0 ≤ ai ≤ 5000), where ai denotes code of the city to which i-th person is going.

Output

The output should contain a single integer — maximal possible total comfort.

Examples

Input

6

4 4 2 5 2 3

Output

14

Input

9

5 1 3 1 5 2 4 2 5

Output

9

Note

In the first test case best partition into segments is: [4, 4] [2, 5, 2] [3], answer is calculated as follows: 4 + (2 xor 5) + 3 = 4 + 7 + 3 = 14

In the second test case best partition into segments is: 5 1 [3] 1 5 [2, 4, 2] 5, answer calculated as follows: 3 + (2 xor 4) = 3 + 6 = 9.
因为要保证同一个数要放在同个区间里，或者就不取，所以要预处理一下每个数最左边和最右边，还有一个限制是不能异或同一个数，所以就用个vis数组记录一下，然后就dp，设dp[i]为从1到i的最大符合条件的区间异或和，然后其实数据很小差不多就是枚举i（区间右端点），然后往前枚举j（区间左端点），然后通过预处理和vis判断约束一下

代码：

```
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int N=5005;
int n;
int a[N],l[N],r[N];
int dp[N];
bool vis[N];
int main(void){
    scanf("%d",&n);
    //顺便预处理出每个数的最左和最右位置
    for(int i=1;i<=n;i++){
        scanf("%d",&a[i]);
        if(!l[a[i]]){
            l[a[i]]=i;
        }
        r[a[i]]=i;
    }
    //dp[i]表示到i位置的最大异或和
    for(int i=1;i<=n;i++){
        memset(vis,false,sizeof(vis));
        //先默认这一个数不取
        dp[i]=dp[i-1];
        int ans=0;
        int Min=i;
        //变量i前面的数
        for(int j=i;j>=1;j--){
            int t=a[j];
            //避免重复异或
            if(!vis[t]){
                //如果前面有一个数的最右位置大于i，那么i这个线段就拓展到这里位置，如果要包含j这个数就必须包含后面的最右
                if(r[t]>i){
                    break;
                }
                //找到这个线段能延续到的最左端
                Min=min(Min,l[t]);
                //将这个数异或并标记
                ans=ans^t;
                vis[t]=1;
            }
            printf("%d %d %d\n",i,j,Min);
            //此时尝试选取的线段就是[j,i]
            //状态转移，j<=Min说明这个线段包含了j这个数的最左
            if(j<=Min){
                dp[i]=max(dp[i],dp[j-1]+ans);
            }
        }
    }
    printf("%d\n",dp[n]);
    return 0;
}
```






