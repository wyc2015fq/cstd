# hdu1432——Lining Up - westbrook1998的博客 - CSDN博客





2018年07月27日 13:28:42[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：36标签：[简单几何](https://so.csdn.net/so/search/s.do?q=简单几何&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
“How am I ever going to solve this problem?” said the pilot.  

  Indeed, the pilot was not facing an easy task. She had to drop packages at specific points scattered in a dangerous area. Furthermore, the pilot could only fly over the area once in a straight line, and she had to fly over as many points as possible. All points were given by means of integer coordinates in a two-dimensional space. The pilot wanted to know the largest number of points from the given set that all lie on one line. Can you write a program that calculates this number?  

  Your program has to be efficient!  

  Input 

  The input consists of multiple test cases, and each case begins with a single positive integer on a line by itself indicating the number of points, followed by N pairs of integers, where 1 < N < 700. Each pair of integers is separated by one blank and ended by a new-line character. No pair will occur twice in one test case.  

  Output 

  For each test case, the output consists of one integer representing the largest number of points that all lie on one line, one line per case. 

  Sample Input 

  5 

  1 1 

  2 2 

  3 3 

  9 10 

  10 11 

  Sample Output 

  3
判断最多有多少个点在同一直线上 直接暴力枚举

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cmath>
#include <cstring>
using namespace std;
const int MAXN=800;
int x[MAXN];
int y[MAXN];
int main(void){
    int n;
    while(~scanf("%d",&n)){
        memset(x,0,sizeof(x));
        memset(y,0,sizeof(y));
        for(int i=0;i<n;i++){
            scanf("%d%d",&x[i],&y[i]);
        }
        int ans=0;
        int cnt=0;
        for(int i=0;i<n-2;i++){
            for(int j=i+1;j<n-1;j++){
                cnt=2;
                for(int k=j+1;k<n;k++){
                    if((x[i]-x[j])*(y[i]-y[k])==(y[i]-y[j])*(x[i]-x[k])){
                        cnt++;
                    }
                }
                ans=max(ans,cnt);
            }
        }
        printf("%d\n",ans);
    }
    return 0;
}
```





