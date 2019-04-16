# HDU 1052 Tian Ji -- The Horse Racing  田忌赛马 - 紫芝的博客 - CSDN博客





2018年08月23日 18:37:15[紫芝](https://me.csdn.net/qq_40507857)阅读数：22
个人分类：[贪心](https://blog.csdn.net/qq_40507857/article/category/7568514)









# Tian Ji -- The Horse Racing

****Time Limit: 2000/1000 MS (Java/Others)    Memory Limit: 65536/32768 K (Java/Others)Total Submission(s): 36374    Accepted Submission(s): 10956****

**Problem Description**

Here is a famous story in Chinese history.


"That was about 2300 years ago. General Tian Ji was a high official in the country Qi. He likes to play horse racing with the king and others."


"Both of Tian and the king have three horses in different classes, namely, regular, plus, and super. The rule is to have three rounds in a match; each of the horses must be used in one round. The winner of a single round takes two hundred silver dollars from the loser."


"Being the most powerful man in the country, the king has so nice horses that in each class his horse is better than Tian's. As a result, each time the king takes six hundred silver dollars from Tian."


"Tian Ji was not happy about that, until he met Sun Bin, one of the most famous generals in Chinese history. Using a little trick due to Sun, Tian Ji brought home two hundred silver dollars and such a grace in the next match."


"It was a rather simple trick. Using his regular class horse race against the super class from the king, they will certainly lose that round. But then his plus beat the king's regular, and his super beat the king's plus. What a simple trick. And how do you think of Tian Ji, the high ranked official in China?"

![](http://acm.hdu.edu.cn/data/images/1052-1.gif)


Were Tian Ji lives in nowadays, he will certainly laugh at himself. Even more, were he sitting in the ACM contest right now, he may discover that the horse racing problem can be simply viewed as finding the maximum matching in a bipartite graph. Draw Tian's horses on one side, and the king's horses on the other. Whenever one of Tian's horses can beat one from the king, we draw an edge between them, meaning we wish to establish this pair. Then, the problem of winning as many rounds as possible is just to find the maximum matching in this graph. If there are ties, the problem becomes more complicated, he needs to assign weights 0, 1, or -1 to all the possible edges, and find a maximum weighted perfect matching...


However, the horse racing problem is a very special case of bipartite matching. The graph is decided by the speed of the horses --- a vertex of higher speed always beat a vertex of lower speed. In this case, the weighted bipartite matching algorithm is a too advanced tool to deal with the problem.


In this problem, you are asked to write a program to solve this special case of matching problem.

**Input**

The input consists of up to 50 test cases. Each case starts with a positive integer n (n <= 1000) on the first line, which is the number of horses on each side. The next n integers on the second line are the speeds of Tian’s horses. Then the next n integers on the third line are the speeds of the king’s horses. The input ends with a line that has a single 0 after the last test case.

**Output**

For each input case, output a line containing a single number, which is the maximum money Tian Ji will get, in silver dollars.

**Sample Input**

```
3 
92 83 71 
95 87 74 
2 20 20 
20 20 
2 
20 19 
22 18 
0
```

**Sample Output**

```
200 
0 
0
```
` `
**Source**

[2004 Asia Regional Shanghai](http://acm.hdu.edu.cn/search.php?field=problem&key=2004+Asia+Regional+Shanghai+&source=1&searchmode=source)

**Recommend**

JGShining   |   We have carefully selected several similar problems for you:  [1050](http://acm.hdu.edu.cn/showproblem.php?pid=1050)[1051](http://acm.hdu.edu.cn/showproblem.php?pid=1051)[1009](http://acm.hdu.edu.cn/showproblem.php?pid=1009)[1800](http://acm.hdu.edu.cn/showproblem.php?pid=1800)[1053](http://acm.hdu.edu.cn/showproblem.php?pid=1053)

这里讲讲是如何贪心的方法：

1.当田忌最慢的马比齐王最慢的马快，赢一场

2.当田忌最慢的马比齐王最慢的马慢，和齐王最快的马比，输一场

3.当田忌最快的马比齐王最快的马快时，赢一场

4.当田忌最快的马比齐王最快的马慢时，拿最慢的马和齐王最快的马比，输一场

5.当田忌最快的马和齐王最快的马相等时，拿最慢的马来和齐王最快的马比

```cpp
#include<cstdio>
#include<bits/stdc++.h>
using namespace std;
const int maxn=1009;
//int a[maxn],b[maxn];
int main()
{
    int n;
    while(scanf("%d",&n),n){
        vector<int>a(n),b(n);
        for(int i=0;i<n;i++)
            scanf("%d",&a[i]);

        for(int i=0;i<n;i++)
            scanf("%d",&b[i]);
        sort(a.begin(),a.end());
        sort(b.begin(),b.end());
        int win=0,lose=0;
        int ans=0;
        int la=0,ra=n-1;
        int lb=0,rb=n-1;

    for(int i=0;i<n;i++)
    {
//当田忌最慢的马比齐王最慢的马快,赢一场
        if(a[la]>b[lb])
        {
            la++;
            lb++;
            //ans++;
            win++;
        }
//当田忌最快的马比齐王最快的马快时，赢一场
        else if(a[ra]>b[rb])
        {
            ra--;
            rb--;
            win++;
        }
//当田忌最快的马比齐王最快的马慢时，拿最慢的马和齐王最快的马比，输一场
        else if(a[ra]<b[rb])
        {
            la++;
            rb--;
            lose++;
        }
//当田忌最慢的马比齐王最慢的马慢，和齐王最快的马比，输一场
        else if(a[la]<b[lb])
        {
            la++;
            rb--;
            lose++;
        }
//当田忌最快的马和齐王最快的马相等时，拿最慢的马来和齐王最快的马比.
        else
        {
            if(a[la]!=b[rb])
            lose++;
            la++;
            rb--;
        }
    }
    printf("%d\n",(win-lose)*200);
}
    return 0;
}
```

简化版 

```cpp
#include<cstdio>
#include<bits/stdc++.h>
using namespace std;
const int maxn=1009;
//int a[maxn],b[maxn];
int main()
{
    int n;
    while(scanf("%d",&n),n){
        vector<int>a(n),b(n);
        for(int i=0;i<n;i++)
            scanf("%d",&a[i]);

        for(int i=0;i<n;i++)
            scanf("%d",&b[i]);
        sort(a.begin(),a.end());
        sort(b.begin(),b.end());
        int win=0,lose=0;
        int ans=0;
        int la=0,ra=n-1;
        int lb=0,rb=n-1;

    for(int i=0;i<n;i++)
    {
//当田忌最慢的马比齐王最慢的马快,赢一场
        if(a[la]>b[lb])
        {
            la++;
            lb++;
            //ans++;
            win++;
        }
//当田忌最快的马比齐王最快的马快时，赢一场
        else if(a[ra]>b[rb])
        {
            ra--;
            rb--;
            win++;
        }
//拿最慢的马来和齐王最快的马比.
        else
        {
            if(a[la]!=b[rb])//田忌必败
            lose++;
        //两匹马一样快
            la++;
            rb--;
        }
    }
    printf("%d\n",(win-lose)*200);
}
    return 0;
}
```





