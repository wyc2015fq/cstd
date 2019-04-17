# Bad Hair Day（POJ-3250） - Alex_McAvoy的博客 - CSDN博客





2018年06月29日 16:53:40[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：52
个人分类：[POJ																[数据结构——单调栈与单调队列](https://blog.csdn.net/u011815404/article/category/8661014)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

Some of Farmer John's N cows (1 ≤ N ≤ 80,000) are having a bad hair day! Since each cow is self-conscious about her messy hairstyle, FJ wants to count the number of other cows that can see the top of other cows' heads.

Each cow i has a specified height hi (1 ≤ hi ≤ 1,000,000,000) and is standing in a line of cows all facing east (to the right in our diagrams). Therefore, cow i can see the tops of the heads of cows in front of her (namely cows i+1, i+2, and so on), for as long as these cows are strictly shorter than cow i.

Consider this example:

            =

=          =

=    _    =         Cows facing right -->

=    =    =

= _ = = =

= = = = = =

1 2 3 4 5 6 

Cow#1 can see the hairstyle of cows #2, 3, 4

Cow#2 can see no cow's hairstyle

Cow#3 can see the hairstyle of cow #4

Cow#4 can see no cow's hairstyle

Cow#5 can see the hairstyle of cow 6

Cow#6 can see no cows at all!

Let ci denote the number of cows whose hairstyle is visible from cow i; please compute the sum of c1 through cN.For this example, the desired is answer 3 + 0 + 1 + 0 + 1 + 0 = 5.


# **Input**

Line 1: The number of cows, N. 

Lines 2..N+1: Line i+1 contains a single integer that is the height of cow i.

# Output

Line 1: A single integer that is the sum of c1 through cN.

# Sample Input

**6**

**10374122**

# Sample Output

**5**


题意：n头牛排队，每个牛都有一个高度，每头牛只能看见它前面的比它矮的牛，当其前面的一头牛的高度高于当前的牛，那么这头牛以及前面的牛都视为看不到，求每头牛能看见的牛的数量和。

思路：单调栈

如果一头牛前面的牛比它矮或等于它的高度，就加入栈中，否则就出栈，这样不断统计栈的元素即可

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>
#include<ctime>
#include<algorithm>
#include<utility>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-6
#define INF 0x3f3f3f3f
#define N 1000001
#define LL long long
const int MOD=998244353;
const int dx[]={-1,1,0,0};
const int dy[]={0,0,-1,1};
using namespace std;
int main(){
    int n;
    while(scanf("%d",&n)!=EOF&&n){
        LL sum=0;
        stack<int> S;

        for(int i=1;i<=n;i++){
            int x;
            scanf("%d",&x);
            while(!S.empty()&&S.top()<=x)
                S.pop();
            sum+=S.size();
            S.push(x);
        }
        printf("%lld\n",sum);
    }
    return 0;
}
```






