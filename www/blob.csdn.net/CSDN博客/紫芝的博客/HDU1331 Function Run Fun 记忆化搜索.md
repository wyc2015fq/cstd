# HDU1331 Function Run Fun 记忆化搜索 - 紫芝的博客 - CSDN博客





2018年07月22日 09:12:31[紫芝](https://me.csdn.net/qq_40507857)阅读数：62
个人分类：[DFS](https://blog.csdn.net/qq_40507857/article/category/7566193)









# Function Run Fun

****Time Limit: 2000/1000 MS (Java/Others)    Memory Limit: 65536/32768 K (Java/Others)Total Submission(s): 5557    Accepted Submission(s): 2588****

**Problem Description**

We all love recursion! Don't we?


Consider a three-parameter recursive function w(a, b, c):


if a <= 0 or b <= 0 or c <= 0, then w(a, b, c) returns:1


if a > 20 or b > 20 or c > 20, then w(a, b, c) returns:

w(20, 20, 20)


if a < b and b < c, then w(a, b, c) returns:

w(a, b, c-1) + w(a, b-1, c-1) - w(a, b-1, c)


otherwise it returns:

w(a-1, b, c) + w(a-1, b-1, c) + w(a-1, b, c-1) - w(a-1, b-1, c-1)


This is an easy function to implement. The problem is, if implemented directly, for moderate values of a, b and c (for example, a = 15, b = 15, c = 15), the program takes hours to run because of the massive recursion.

**Input**

The input for your program will be a series of integer triples, one per line, until the end-of-file flag of -1 -1 -1. Using the above technique, you are to calculate w(a, b, c) efficiently and print the result.

**Output**

Print the value for w(a,b,c) for each triple.

**Sample Input**

1 1 1 

2 2 2 

10 4 6 

50 50 50 

-1 7 18 

-1 -1 -1

**Sample Output**

w(1, 1, 1) = 2

w(2, 2, 2) = 4 

w(10, 4, 6) = 523 

w(50, 50, 50) = 1048576 

w(-1, 7, 18) = 1

**Source**

[Pacific Northwest 1999](http://acm.hdu.edu.cn/search.php?field=problem&key=Pacific+Northwest+1999&source=1&searchmode=source)

**Recommend**

Ignatius.L   |   We have carefully selected several similar problems for you:  [1074](http://acm.hdu.edu.cn/showproblem.php?pid=1074)[1078](http://acm.hdu.edu.cn/showproblem.php?pid=1078)[1160](http://acm.hdu.edu.cn/showproblem.php?pid=1160)[1208](http://acm.hdu.edu.cn/showproblem.php?pid=1208)[1355](http://acm.hdu.edu.cn/showproblem.php?pid=1355)



```cpp
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
ll arr[22][22][22];
ll w(int a,int b,int c)
{
    if(a<=0||b<=0||c<=0)
        return 1;
    else if(a>20||b>20||c>20)
        return w(20,20,20);
    else if(arr[a][b][c]!=-1)
        return arr[a][b][c];//已经算过的不再重复计算
//记忆化搜索
    else if(a<b&&b<c)
        return arr[a][b][c]=w(a,b,c-1)+w(a,b-1,c-1)-w(a,b-1,c);
    else
        return arr[a][b][c]=w(a-1,b,c)+w(a-1,b-1,c)+w(a-1,b,c-1)-w(a-1,b-1,c-1);
}
int main()
{
    int a,b,c;
    while(scanf("%d%d%d",&a,&b,&c)!=EOF){
        if(a==-1&&b==-1&&c==-1)  break;
        memset(arr,-1,sizeof(arr));//标记没有算过
        printf ("w(%d, %d, %d) = ",a,b,c);
		printf ("%lld\n",w(a,b,c));
    }
    return 0;
}
```





