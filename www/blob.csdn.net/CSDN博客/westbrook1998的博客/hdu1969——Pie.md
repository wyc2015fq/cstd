# hdu1969——Pie - westbrook1998的博客 - CSDN博客





2018年05月26日 15:07:55[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：46标签：[二分](https://so.csdn.net/so/search/s.do?q=二分&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
My birthday is coming up and traditionally I’m serving pie. Not just one pie, no, I have a number N of them, of various tastes and of various sizes. F of my friends are coming to my party and each of them gets a piece of pie. This should be one piece of one pie, not several small pieces since that looks messy. This piece can be one whole pie though.  

  My friends are very annoying and if one of them gets a bigger piece than the others, they start complaining. Therefore all of them should get equally sized (but not necessarily equally shaped) pieces, even if this leads to some pie getting spoiled (which is better than spoiling the party). Of course, I want a piece of pie for myself too, and that piece should also be of the same size.  

  What is the largest possible piece size all of us can get? All the pies are cylindrical in shape and they all have the same height 1, but the radii of the pies can be different.  

  Input 

  One line with a positive integer: the number of test cases. Then for each test case:  

  —One line with two integers N and F with 1 <= N, F <= 10 000: the number of pies and the number of friends.  

  —One line with N integers ri with 1 <= ri <= 10 000: the radii of the pies.  

  Output 

  For each test case, output one line with the largest possible volume V such that me and my friends can all get a pie piece of size V. The answer should be given as a floating point number with an absolute error of at most 10^(-3). 

  Sample Input 

  3 

  3 3 

  4 3 3 

  1 24 

  5 

  10 5 

  1 4 2 3 4 5 6 5 4 2 

  Sample Output 

  25.1327 

  3.1416 

  50.2655
题意是讲有n个pie(圆形)，分给f+1个人(包括我自己)，每人分到的必须是完整的一块，不能两小块拼起来，所以是二分枚举每一块的面积(范围是0到最大的pie面积)，然后判断能不能分成f+1块

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <cmath>
using namespace std;
const double PI=acos(-1.0);
const int MAXN=10050;
int n,f;
double A[MAXN];
bool check(double a){
    int sum=0;
    for(int i=0;i<n;i++){
        sum+=floor(A[i]/a);
    }
    return sum>=f+1;
}
int main(void){
    int t;
    scanf("%d",&t);
    while(t--){
        scanf("%d%d",&n,&f);
        int ri;
        double maxA=-1;
        for(int i=0;i<n;i++){
            scanf("%d",&ri);
            A[i]=PI*ri*ri;
            maxA=max(maxA,A[i]);
        }
        double l=0,r=maxA;
        while(r-l>1e-5){
            double m=(l+r)/2;
            if(check(m)){
                l=m;
            }
            else{
                r=m;
            }
        }
        printf("%.4lf\n",l);
    }
}
```





