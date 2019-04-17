# Codeforces602B——Approximating a Constant Range - westbrook1998的博客 - CSDN博客





2018年08月27日 23:15:46[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：33








> 
When Xellos was doing a practice course in university, he once had to measure the intensity of an effect that slowly approached equilibrium. A good way to determine the equilibrium intensity would be choosing a sufficiently large number of consecutive data points that seems as constant as possible and taking their average. Of course, with the usual sizes of data, it’s nothing challenging — but why not make a similar programming contest problem while we’re at it? 

  You’re given a sequence of n data points a1, …, an. There aren’t any big jumps between onsecutive data points — for each 1 ≤ i < n, it’s guaranteed that |ai + 1 - ai| ≤ 1. 

  A range [l, r] of data points is said to be almost constant if the difference between the largest and the smallest value in that range is at most 1. Formally, let M be the maximum and m the minimum value of ai for l ≤ i ≤ r; the range [l, r] is almost constant if M - m ≤ 1. 

  Find the length of the longest almost constant range. 

  Input 

  The first line of the input contains a single integer n (2 ≤ n ≤ 100 000) — the number of data points. 

  The second line contains n integers a1, a2, …, an (1 ≤ ai ≤ 100 000). 

  Output 

  Print a single number — the maximum length of an almost constant range of the given sequence. 

  Examples 

  Input 

  5 

  1 2 3 3 2 

  Output 

  4 

  Input 

  11 

  5 4 5 5 6 7 8 8 8 7 6 

  Output 

  5 

  Note 

  In the first sample, the longest almost constant range is [2, 5]; its length (the number of data points in it) is 4. 

  In the second sample, there are three almost constant ranges of length 4: [1, 4], [6, 9] and [7, 10]; the only almost constant range of the maximum length 5 is [6, 10].
跑去cf看外国朋友的代码题解 

真实简介易懂。。也是第二次看到multiset这个东西了 

唉 自己分析题目总是一团糟 只能看题解才能理解
代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <set>
using namespace std;
const int N=100050;
int a[N];
multiset<int> st;
int main(void){
    int n;
    scanf("%d",&n);
    for(int i=0;i<n;i++){
        scanf("%d",&a[i]);
    }
    int ans=0;
    //双指针
    int l=0,r=0;
    while(r<n){
        //加入最右边的数
        st.insert(a[r]);
        if(abs(*st.begin()-*st.rbegin())>1){
            //加入这个数之后这个区间不满足
            //就将这个区间最左的删掉，比较前后两个答案哪个更优
            st.erase(st.find(a[l++]));
        }
        ans=max(ans,int(st.size()));
        r++;
    }
    printf("%d\n",ans);
    return 0;
}
```






