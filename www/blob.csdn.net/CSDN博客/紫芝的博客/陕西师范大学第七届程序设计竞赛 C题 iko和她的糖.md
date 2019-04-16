# 陕西师范大学第七届程序设计竞赛 C题 iko和她的糖 - 紫芝的博客 - CSDN博客





2018年05月14日 17:05:58[紫芝](https://me.csdn.net/qq_40507857)阅读数：83








                链接：[https://www.nowcoder.com/acm/contest/121/C](https://www.nowcoder.com/acm/contest/121/C)
来源：牛客网



时间限制：C/C++ 1秒，其他语言2秒
空间限制：C/C++ 131072K，其他语言262144K
 64bit IO Format: %lld



## 题目描述



    iko超级超级喜欢吃糖，有一天iko想出去玩，她计划从1点走到N点（按1,2,3，...，n的顺序走），每个点都有一个补给站，第i点的补给站有a[i]颗糖，从i点走到i+1点会消耗掉b[i]颗糖，iko在出游的途中可以选择三个补给站，iko想知道她走完全程到达N点时口袋里最多还能剩下几颗糖（初始时iko的口袋里一颗糖都没有）。 


## 输入描述:
第一行输入N（3<=N<=1000）
第二行输入N个数代表a[1].......a[N]  (0<=a[i]<=1000 )
第三行输入N-1个数代表b[1]......b[N-1]  ( 1<=b[i]<=1000 )
## 输出描述:
输出一个数字表示iko到达n点时口袋里最多剩下的糖，
若不能到达N点输出-1。

示例1



## 输入

3
1 3 4
3 4



## 输出

-1





示例2



## 输入

5
3 4 5 2 4
3 2 2 2



## 输出

3





## 解法1：

深度优先搜索 dfs

void dfs(int index,int sum,int cnt)

当前处于第index个补给站，口袋里有sum颗糖，还可以选择cnt个补给站

初始时iko的口袋里一颗糖都没有，所以sum=0；

若sum<0,说明无法到达下一个补给站

若当前处于最后一个补给站，如果还有补给机会，肯定要补给，如果没有补给机会，则不补给




```cpp
//dfs
#include<bits/stdc++.h>
using namespace std;
const int maxn=1e3+5;
int a[maxn];//补给
int b[maxn];//消耗
int n;//点的个数
int max_sum;//最多还能剩下几颗糖
//index表示现在处于哪一个点，sum表示剩下几颗糖，可以选择cnt个补给站
void dfs(int index,int sum,int cnt)
{
    if(sum<0)
    return ;
    if(index==n)
    {
        //如果还有剩余的补给机会，肯定要补给，这样才最多
        if(cnt>0&&sum+a[index]>max_sum)
            max_sum=sum+a[index];
        else if(cnt==0&&sum>max_sum)
            max_sum=sum;
        return ;
    }
    dfs(index+1,sum-b[index],cnt);//这一站不补给
    if(cnt>0)
    dfs(index+1,sum+a[index]-b[index],cnt-1);//这一站补给
}
int main()
{
	ios::sync_with_stdio(0);
	while(cin>>n)
    {
        max_sum=-1;
        for(int i=1;i<=n;i++)
            cin>>a[i];
        for(int i=1;i<n;i++)
            cin>>b[i];
        dfs(1,0,3);//初始时iko的口袋里一颗糖都没有
            cout<<max_sum<<endl;
}
	return 0;
}
```

## 解法2：

贪心

```cpp
#include<bits/stdc++.h>
using namespace std;
const int maxn=1e3+5;
int a[maxn];//补给
int b[maxn];//消耗
int n;//点的个数
int sum;//最多还能剩下几颗糖
int main()
{
	ios::sync_with_stdio(0);
	while(cin>>n)
    {
        priority_queue<int> q;//每次取出最大
        int first;//第一个补给站
        int cnt=0;//补给次数
        cin>>first;
        q.push(first);//必须取第一个

        for(int i=1;i<n;i++)
            cin>>a[i];
        for(int i=1;i<n;i++)
            cin>>b[i];

        for(int i=1;i<n;i++){
            while(sum<b[i]&&!q.empty()){
                sum+=q.top();
                q.pop();cnt++;
            }
            sum-=b[i];
            if(sum<0)
            {
                cnt=100;
                break;
            }
            q.push(a[i]);
        }
       while(!q.empty()&&cnt<3)
       {
           sum+=q.top();q.pop();
           cnt++;
       }
            cout<<(cnt>3?-1:sum)<<endl;
}
	return 0;
}
```








