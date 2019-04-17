# hdu3069——Saruman's Army - westbrook1998的博客 - CSDN博客





2018年08月06日 22:28:59[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：52








> 
Saruman the White must lead his army along a straight path from Isengard to Helm’s Deep. To keep track of his forces, Saruman distributes seeing stones, known as palantirs, among the troops. Each palantir has a maximum effective range of R units, and must be carried by some troop in the army (i.e., palantirs are not allowed to “free float” in mid-air). Help Saruman take control of Middle Earth by determining the minimum number of palantirs needed for Saruman to ensure that each of his minions is within R units of some palantir. 

  Input 

  The input test file will contain multiple cases. Each test case begins with a single line containing an integer R, the maximum effective range of all palantirs (where 0 ≤ R ≤ 1000), and an integer n, the number of troops in Saruman’s army (where 1 ≤ n ≤ 1000). The next line contains n integers, indicating the positions x1, …, xn of each troop (where 0 ≤ xi ≤ 1000). The end-of-file is marked by a test case with R = n = −1. 

  Output 

  For each test case, print a single integer indicating the minimum number of palantirs needed. 

  Sample Input 

  0 3 

  10 20 20 

  10 7 

  70 30 1 7 15 20 50 

  -1 -1 

  Sample Output 

  2 

  4 

  Hint 

  In the first test case, Saruman may place a palantir at positions 10 and 20. Here, note that a single palantir with range 0 can cover both of the troops at position 20. 

  In the second test case, Saruman can place palantirs at position 7 (covering troops at 1, 7, and 15), position 20 (covering positions 20 and 30), position 50, and position 70. Here, note that palantirs must be distributed among troops and are not allowed to “free float.” Thus, Saruman cannot place a palantir at position 60 to cover the troops at positions 50 and 70.
真是巧妙的一道题啊  

贪心思想 

第一个点不标记 因为左边已经没有点了，标记了就是浪费，这里就有贪心思想，所以一直往右走，找到第一个点的R覆盖不到的地方，然后这个点的左边一个点就是要标记的点，以他为中心，就恰好能覆盖第一个点，而又不会浪费，然后再以这个点为起点，往右走找到第一个R覆盖不到的点，那么这个点左边的所有点就可以被刚才标记的那个点给覆盖了，所以就从当前这个点开始再重复操作
代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int MAXN=1020;
int a[MAXN];
int main(void){
    int r,n;
    while(~scanf("%d%d",&r,&n)){
        if(r==-1 && n==-1){
            break;
        }
        memset(a,0,sizeof(a));
        for(int i=0;i<n;i++){
            scanf("%d",&a[i]);
        }
        sort(a,a+n);
        int i=0;
        int ans=0;
        while(i<n){
            int s=a[i++];
            //一直向右找到距s距离大于r的点i 标记i-1
            //相当于这个直径的左部分
            while(i<n && a[i]<=s+r){
                i++;
            }
            //标记
            int p=a[i-1];
            //相当于直径的右部分
            //找到的这个点就是下一个区域的第一点点（不标记），重复操作
            while(i<n && a[i]<=p+r){
                i++;
            }
            ans++;
        }
        printf("%d\n",ans);
    }
    return 0;
}
```






