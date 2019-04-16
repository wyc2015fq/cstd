# POJ3614Sunscreen(优先队列+贪心) - 紫芝的博客 - CSDN博客





2018年07月21日 11:39:41[紫芝](https://me.csdn.net/qq_40507857)阅读数：37
个人分类：[贪心](https://blog.csdn.net/qq_40507857/article/category/7568514)









# Sunscreen
|**Time Limit:** 1000MS||**Memory Limit:** 65536K|
|----|----|----|
|**Total Submissions:** 8435||**Accepted:** 2981|

Description

To avoid unsightly burns while tanning, each of the *C* (1 ≤ *C* ≤ 2500) cows must cover her hide with sunscreen when they're at the beach. Cow *i* has a minimum and maximum *SPF* rating (1 ≤ *minSPFi* ≤ 1,000; *minSPFi* ≤ *maxSPFi* ≤ 1,000) that will work. If the *SPF* rating is too low, the cow suffers sunburn; if the *SPF* rating is too high, the cow doesn't tan at all........

The cows have a picnic basket with *L* (1 ≤ *L* ≤ 2500) bottles of sunscreen lotion, each bottle *i* with an *SPF* rating *SPFi* (1 ≤ *SPFi* ≤ 1,000). Lotion bottle *i* can cover *coveri* cows with lotion. A cow may lotion from only one bottle.

What is the maximum number of cows that can protect themselves while tanning given the available lotions?

Input

* Line 1: Two space-separated integers: *C* and *L*

* Lines 2..*C*+1: Line *i* describes cow *i*'s lotion requires with two integers: *minSPFi* and *maxSPFi*

* Lines *C*+2..*C*+*L*+1: Line *i*+*C*+1 describes a sunscreen lotion bottle *i* with space-separated integers: *SPFi* and *coveri*

Output

A single line with an integer that is the maximum number of cows that can be protected while tanning

Sample Input

3 2
3 10
2 5
1 5
6 2
4 1
Sample Output

2
Source

[USACO 2007 November Gold](http://poj.org/searchproblem?field=source&key=USACO+2007+November+Gold)

题意：有C头奶牛要去沐光浴，太阳光太强烈会晒坏皮肤，太弱又会没效果。每头牛都有一个太阳光适宜的范围经行沐光浴，分别给出minspf_i和maxspf_i。 有L种防晒霜，每种防晒霜可以把所受阳光固定于一个值spf_i，每种有cover_i瓶。 问最多会有几头牛得到合适的光晒强度？

**题解：**

**贪心策略，在满足minspf的条件下，尽量将spf的防晒霜涂到maxspf小的奶牛身上，因为maxspf大的奶牛有更多的选择。**

**这里就需要一个优先队列来储存满足minspf的奶牛的maxspf的值。 具体解题步骤如下：**

**1.将奶牛按照minspf升序排列，将防晒霜按照spf升序排列。**

**2.枚举防晒霜，将minspf<=spf的奶牛的maxspf存到优先队列中，然后值小的先出队列，**

**看是否满足maxspf>=spf，更新记录值。**

```cpp
#include<bits/stdc++.h>
using namespace std;
struct cow
{
    int min_spf,max_spf;
};
struct suns
{
    int spf,num;
};
vector<cow> c;
vector<suns> s;
int n,m;
bool cmp1(cow a,cow b)
{
    return a.min_spf<b.min_spf;
}
bool cmp2(suns a,suns b)
{
    return a.spf<b.spf;
}
priority_queue<int,vector<int>,greater<int> >qua;
int main()
{
    cin>>n>>m;
    for(int i=0;i<n;i++)
    {
        cow tmp;
        cin>>tmp.min_spf>>tmp.max_spf;
        c.push_back(tmp);
    }
    for(int i=0;i<m;i++)
    {
        suns tmp;
        cin>>tmp.spf>>tmp.num;
        s.push_back(tmp);
    }
    sort(c.begin(),c.end(),cmp1);
    sort(s.begin(),s.end(),cmp2);
    int ans=0,cnt=0;
    for(int i=0;i<m;i++)
    {
        while(cnt<n&&c[cnt].min_spf<=s[i].spf){
            qua.push(c[cnt].max_spf);
            cnt++;
        }
        while(s[i].num!=0&&!qua.empty()){
            int k=qua.top();
            qua.pop();
            if(k>=s[i].spf)
            {
                ans++;s[i].num--;
            }
        }
    }
    cout<<ans<<endl;
}
```





