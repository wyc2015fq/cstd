# HDU 4268 Alice and Bob 贪心STL O(nlogn) - weixin_33985507的博客 - CSDN博客
2015年03月07日 15:04:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2

B - Alice and Bob
**Time Limit:**5000MS     **Memory Limit:**32768KB     **64bit IO Format:**%I64d & %I64u
**Description**
　　Alice and Bob's game never ends. Today, they introduce a new game. In this game, both of them have N different rectangular cards respectively. 　　
　　Alice wants to use his cards to cover Bob's. The card A can cover the card B if the height of A is not smaller than B and the width of A is not smaller than B. As the best programmer, you are asked to compute the maximal number of Bob's cards that Alice can cover. 
Please pay attention that each card can be used only once and the cards cannot be rotated.
       
**Input**
 　　The first line of the input is a number T (T <= 40) which means the number of test cases. 
       
　　For each case, the first line is a number N which means the 
number of cards that Alice and Bob have respectively. Each of the 
following N (N <= 100,000) lines contains two integers h (h <= 
1,000,000,000) and w (w <= 1,000,000,000) which means the height and 
width of Alice's card, then the following N lines means that of Bob's.
       
**Output**
 　　For each test case, output an answer using one line which contains just one number.
       
**Sample Input**
 2
2
1 2
3 4
2 3
4 5
3
2 3
5 7
6 8
4 1
2 5
3 4 
**Sample Output**
1 2
 题意：10W个数据，A有n张牌，B有n张牌，然后如果a.x>=b.x&&a.y>=b.y 那么A就可以覆盖B
然后问你最多覆盖多少张
题解：暴力非常好想，O（n^2)跑一发就是，但是会T
那么我们就二分查找，或者用STL就好
```
```cpp
multiset<int> myset;
multiset<int>::iterator it;
const int maxn=200000;
struct node
{
    int x,y;
    bool operator<(const node& b)const
    {
        return x<b.x;
    }
}a[maxn],b[maxn];
int main()
{
    int sec,n;
    scanf("%d",&sec);
    for(int z=1;z<=sec;z++)
    {
        myset.clear();
        scanf("%d",&n);
        for(int i=1;i<=n;i++)
        scanf("%d%d",&a[i].x,&a[i].y);
        for(int i=1;i<=n;i++)
        scanf("%d%d",&b[i].x,&b[i].y);
        sort(a+1,a+1+n);//按x从小到大排序
        sort(b+1,b+1+n);//按x从小到大排序
        int j=1;int ans=0;//j是一个指向B数组位置的指针
        for(int i=1;i<=n;i++)
        {
            while(j<=n&&b[j].x<=a[i].x)
            {
                myset.insert(b[j].y);
                j++;
            }
            it=myset.upper_bound(a[i].y);
            if(myset.size()>0&&it!=myset.begin())it--;
            if(myset.size()>0&&(*it)<=a[i].y)
            {
                ans++;
                myset.erase(it);
            }
        }
        printf("%d\n",ans);
    }
    return 0;
}
```
```
