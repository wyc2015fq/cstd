# POJ 3320 Jessica's Reading Problem（尺取） - HJ - CSDN博客
2017年03月21日 23:36:29[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：247
个人分类：[====ACM====																[----------数学----------																[---------OJ---------																[POJ																[尺取&母函数](https://blog.csdn.net/feizaoSYUACM/article/category/6308036)](https://blog.csdn.net/feizaoSYUACM/article/category/6308043)](https://blog.csdn.net/feizaoSYUACM/article/category/6308067)](https://blog.csdn.net/feizaoSYUACM/article/category/6130298)](https://blog.csdn.net/feizaoSYUACM/article/category/6308070)
Description
Jessica’s a very lovely girl wooed by lots of boys. Recently she has a problem. The final exam is coming, yet she has spent little time on it. If she wants to pass it, she has to master all ideas included in a very thick text book. The author of that text book, like other authors, is extremely fussy about the ideas, thus some ideas are covered more than once. Jessica think if she managed to read each idea at least once, she can pass the exam. She decides to read only one contiguous part of the book which contains all ideas covered by the entire book. And of course, the sub-book should be as thin as possible.
A very hard-working boy had manually indexed for her each page of Jessica’s text-book with what idea each page is about and thus made a big progress for his courtship. Here you come in to save your skin: given the index, help Jessica decide which contiguous part she should read. For convenience, each idea has been coded with an ID, which is a non-negative integer.
Input
The first line of input is an integer P (1 ≤ P ≤ 1000000), which is the number of pages of Jessica’s text-book. The second line contains P non-negative integers describing what idea each page is about. The first integer is what the first page is about, the second integer is what the second page is about, and so on. You may assume all integers that appear can fit well in the signed 32-bit integer type.
Output
Output one line: the number of pages of the shortest contiguous part of the book which contains all ideals covered in the book.
Sample Input
5 
1 8 8 8 1
Sample Output
2
题目大意：
现在有一本书有n页，每一页上有一个知识点标号a[i]可能重复，要求选择一个最小的区间使得能够覆盖所有知识点
解题思路：
这个题目用尺取法解决。
**所谓的尺取法就是就是两个指针表示区间[l,r]的开始与结束然后根据题目来将端点移动**，是一种十分有效的做法。适合连续区间的问题
在这道题看来，[l,r]区间推进，统计区间中能够覆盖的知识点数，对于每一个l，r都是满足可以覆盖所有知识点的最小r，处理好区间知识点数的统计就好了。
```cpp
#include<iostream>
#include<cstdio>
#include<map>
#include<set>
using namespace std;
map<int,int>mm;
set<int>ss;
int a[1000005];
int main()
{
    int n;
    while(~scanf("%d",&n))
    {
        for(int i=0;i<n;i++)
        {
            scanf("%d",&a[i]);
            ss.insert(a[i]);
        }
        int l=0,r=0,m=ss.size(),cnt=0,ans=n; //m为数据种类，cnt记录当前[l,r]区间内数据种类，ans记录满足所有数据种类的最短区间长度 
        while(l<n)         //区间左边点从0开始 
        {
            while(r<n&&cnt<m)         //接下来区间右边点开始往后面累加 直到能把所有数据种类都包括在内，就停止累加 
            {
                if(mm[a[r]]==0)
                  cnt++;
                mm[a[r]]++;
                r++;
            }
            if(cnt<m)   //如果区间右边点直到遍历最后一个数据都不能把所有数据种类包括在内，说明当前区间以及以后尺取的空间都不再可用 
              break;          
            ans=min(ans,r-l);    //选择当前区间的长度记录到ans中，取较小值 
            mm[a[l]]--;            //然后将区间左节点右移一位 
            if(mm[a[l]]==0)
              cnt--;
            l++;            //进行下一次的尺取 
        }
        printf("%d\n",ans);
    }
    return 0;
}
```
