# Reorder the Books（HDU-5500） - Alex_McAvoy的博客 - CSDN博客





2019年01月21日 22:04:42[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：76
个人分类：[HDU																[基础算法——模拟](https://blog.csdn.net/u011815404/article/category/7812141)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

dxy has a collection of a series of books called "The Stories of SDOI",There are n(n≤19) books in this series.Every book has a number from 1 to n. 


dxy puts these books in a book stack with the order of their numbers increasing from top to bottom. dxy takes great care of these books and no one is allowed to touch them. 


One day Evensgn visited dxy's home, because dxy was dating with his girlfriend, dxy let Evensgn stay at home himself. Evensgn was curious about this series of books.So he took a look at them. He found out there was a story about "Little E&Little Q". While losing himself in the story,he disrupted the order of the books. 


Knowing that dxy would be back soon,Evensgn needed to get the books ordered again.But because the books were too heavy.The only thing Evensgn could do was to take out a book from the book stack and and put it at the stack top. 


Give you the order of the disordered books.Could you calculate the minimum steps Evensgn would use to reorder the books? If you could solve the problem for him,he will give you a signed book "The Stories of SDOI 9: The Story of Little E" as a gift. 

# Input

There are several testcases. 

There is an positive integer T(T≤30) in the first line standing for the number of testcases. 

For each testcase, there is an positive integer nn in the first line standing for the number of books in this series. 

Followed nn positive integers separated by space standing for the order of the disordered books,the ithith integer stands for the ithith book's number(from top to bottom). 

# Output

For each testcase,output one line for an integer standing for the minimum steps Evensgn would use to reorder the books.

# **Sample Input**

**244 1 2 351 2 3 4 5**

# Sample Output

**30**


————————————————————————————————————————————

题意：t 组数据，每组给出 n 个数，这 n 个数从小到大的顺序为 1~n，现给出这 n 个数的顺序，每次可以从这 n 个数中任意选一个数放到这 n 个数的最前面，问最少操作多少次使得这 n 个数满足从小到大的 1~n 的顺序

思路：实质就是将两个连续的数字之间的其他数字移走，不用拿在意移走后数字的先后顺序，因为在相同的操作数内总有一种方法能使得其构成所需的顺序数列，从前向后不断找最大的数，然后判断其之后需要移动的数的个数即可

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
#define MOD 1000000007
#define INF 0x3f3f3f3f
#define N 101
#define LL long long
using namespace std;
int a[N];
int main(){
    int t;
    scanf("%d",&t);
    while(t--){
        int n;
        scanf("%d",&n);
        for(int i=1;i<=n;i++)
            scanf("%d",&a[i]);

        int res=n;
        int num=n;
        for(int i=1;i<=n;i++){
            for(int j=1;j<=num;j++){
                if(a[j]==res){
                    res--;
                    num=j;
                    break;
                }
            }
        }

        printf("%d\n",res);
    }
    return 0;
}
```






