# HDU1584 蜘蛛牌 DFS回溯 - 紫芝的博客 - CSDN博客





2018年05月29日 22:51:06[紫芝](https://me.csdn.net/qq_40507857)阅读数：46








# [点击打开链接](http://acm.hdu.edu.cn/showproblem.php?pid=1584)

# 蜘蛛牌

****Time Limit: 10000/5000 MS (Java/Others)    Memory Limit: 32768/32768 K (Java/Others)****



****Total Submission(s): 5229    Accepted Submission(s): 2222****

Problem Description

蜘蛛牌是windows xp操作系统自带的一款纸牌游戏，游戏规则是这样的：只能将牌拖到比她大一的牌上面（A最小，K最大），如果拖动的牌上有按顺序排好的牌时，那么这些牌也跟着一起移动，游戏的目的是将所有的牌按同一花色从小到大排好，为了简单起见，我们的游戏只有同一花色的10张牌，从A到10，且随机的在一行上展开，编号从1到10，把第i号上的牌移到第j号牌上，移动距离为abs(i-j)，现在你要做的是求出完成游戏的最小移动距离。

Input

第一个输入数据是T，表示数据的组数。

每组数据有一行，10个输入数据，数据的范围是[1,10]，分别表示A到10，我们保证每组数据都是合法的。

Output

对应每组数据输出最小移动距离。

Sample Input

1

1 2 3 4 5 6 7 8 9 10

Sample Output

9

Author

xhd

Source

[冬练三九之二](http://acm.hdu.edu.cn/search.php?field=problem&key=%B6%AC%C1%B7%C8%FD%BE%C5%D6%AE%B6%FE&source=1&searchmode=source)

Recommend

lcy   |   We have carefully selected several similar problems for you:  [1430](http://acm.hdu.edu.cn/showproblem.php?pid=1430)[1732](http://acm.hdu.edu.cn/showproblem.php?pid=1732)[1495](http://acm.hdu.edu.cn/showproblem.php?pid=1495)[1518](http://acm.hdu.edu.cn/showproblem.php?pid=1518)[1667](http://acm.hdu.edu.cn/showproblem.php?pid=1667)

##  思路：

搜索的策略是每次枚举10张牌，看它可以放到哪张牌的上面

枚举一下每次移动哪张牌，再判断一下当前的这张牌要移动到哪张牌上

```cpp
#include<bits/stdc++.h>
using namespace std;
int ans;//结果
int a[12];//每一个牌的位置
int mark[12];//表记是否移动过
void dfs(int num,int sum)//num是移动次数，sum是移动距离
{
    int i,j;
    if(sum>=ans)    return ;//剪枝，ans是当前最优解
    if(num==9){
        ans=sum;
        return;
    }
    //搜索
    for(i=1;i<10;i++)
    {
        if(!mark[i])//如果这张牌没有移动过
        {
            mark[i]=1;//标记已经移动过
            //找到要移动到的地方
            //j就是移动的地方，因为移动到的地方肯定比i大
            for(j=i+1;j<=10;j++)
            {
                if(!mark[j]){
                    dfs(num+1,sum+abs(a[j]-a[i]));
                    break;
                }
            }
            mark[i]=0;//回溯
        }
    }
}
int main()
{
    int t,x;
    cin>>t;
    while(t--){
        for(int i=1;i<=10;i++)
        {
            cin>>x;
            a[x]=i;
        }
        memset(mark,0,sizeof(mark));
        //把所有的牌都标记为没有移动
        ans=10000;
        dfs(0,0);
        cout<<ans<<endl;
    }
    return 0;
}
```





