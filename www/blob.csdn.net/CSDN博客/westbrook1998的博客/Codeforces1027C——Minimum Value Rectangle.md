# Codeforces1027C——Minimum Value Rectangle - westbrook1998的博客 - CSDN博客





2018年08月28日 13:07:16[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：83标签：[贪心](https://so.csdn.net/so/search/s.do?q=贪心&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
You have n sticks of the given lengths. 

  Your task is to choose exactly four of them in such a way that they can form a rectangle. No sticks can be cut to pieces, each side of the rectangle must be formed by a single stick. No stick can be chosen multiple times. It is guaranteed that it is always possible to choose such sticks. 

  Let S be the area of the rectangle and P be the perimeter of the rectangle. 

  The chosen rectangle should have the value P2S minimal possible. The value is taken without any rounding. 

  If there are multiple answers, print any of them. 

  Each testcase contains several lists of sticks, for each of them you are required to solve the problem separately. 

  Input 

  The first line contains a single integer T (T≥1) — the number of lists of sticks in the testcase. 

  Then 2T lines follow — lines (2i−1) and 2i of them describe the i-th list. The first line of the pair contains a single integer n (4≤n≤106) — the number of sticks in the i-th list. The second line of the pair contains n integers a1,a2,…,an (1≤aj≤104) — lengths of the sticks in the i-th list. 

  It is guaranteed that for each list there exists a way to choose four sticks so that they form a rectangle. 

  The total number of sticks in all T lists doesn’t exceed 106 in each testcase. 

  Output 

  Print T lines. The i-th line should contain the answer to the i-th list of the input. That is the lengths of the four sticks you choose from the i-th list, so that they form a rectangle and the value P2S of this rectangle is minimal possible. You can print these four lengths in arbitrary order. 

  If there are multiple answers, print any of them. 

  Example 

  Input 

  3 

  4 

  7 2 2 7 

  8 

  2 8 1 4 8 2 1 5 

  5 

  5 5 5 5 5 

  Output 

  2 7 7 2 

  2 2 1 1 

  5 5 5 5 

  Note 

  There is only one way to choose four sticks in the first list, they form a rectangle with sides 2 and 7, its area is 2⋅7=14, perimeter is 2(2+7)=18. 18214≈23.143. 

  The second list contains subsets of four sticks that can form rectangles with sides (1,2), (2,8) and (1,8). Their values are 622=18, 20216=25 and 1828=40.5, respectively. The minimal one of them is the rectangle (1,2). 

  You can choose any four of the 5 given sticks from the third list, they will form a square with side 5, which is still a rectangle with sides (5,5).
先找出有效的边（出现次数大于2） 然后从大到小排序 ，贪心地取相邻的两条边与前面的答案进行比较

代码：

```
#include <cstdio>
#include <algorithm>
#include <cstring>
#define _clr(x,a) sizeof(x,a,sizeof(x));
using namespace std;
const int N=1e6+50;
const int INF=0x3f3f3f3f;
int a[N];
int x[N];
int n;
bool cmp(int ai,int bi){
    return ai>bi;
}
int main(void){
    //freopen("data.txt","r",stdin);
    int t;
    scanf("%d",&t);
    while(t--){
        _clr(a,0);
        _clr(x,0);
        scanf("%d",&n);
        for(int i=0;i<n;i++){
            scanf("%d",&a[i]);
        }
        sort(a,a+n,cmp);
        int k=0;
        for(int i=0;i<n-1;i++){
            if(a[i]==a[i+1]){
                x[k++]=a[i];
                int cnt=1;
                while(a[i]==a[i+1]){
                    i++;
                    cnt++;
                    if(cnt==2){
                        break;
                    }
                }
            }
        }
        // for(int i=0;i<k;i++){
        //     printf("%d ",x[i]);
        // }
        // printf("\n");
        double m=INF*1.0;
        int xi=0;
        int yi=0;
        for(int i=0;i<k-1;i++){
            double t=4.0*(x[i]+x[i+1])*(x[i]+x[i+1])/(x[i]*x[i+1]);
            //printf("%lf\n",t);
            if(m-t>1e-12){
                m=t;
                xi=x[i];
                yi=x[i+1];
            }
        }
        printf("%d %d %d %d\n",xi,xi,yi,yi);
    }
    return 0;
}
```





