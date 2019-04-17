# hdu1034——Candy Sharing Game - westbrook1998的博客 - CSDN博客





2018年07月07日 13:20:25[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：56标签：[模拟](https://so.csdn.net/so/search/s.do?q=模拟&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
A number of students sit in a circle facing their teacher in the center. Each student initially has an even number of pieces of candy. When the teacher blows a whistle, each student simultaneously gives half of his or her candy to the neighbor on the right. Any student, who ends up with an odd number of pieces of candy, is given another piece by the teacher. The game ends when all students have the same number of pieces of candy.  

  Write a program which determines the number of times the teacher blows the whistle and the final number of pieces of candy for each student from the amount of candy each child starts with.  

  Input 

  The input may describe more than one game. For each game, the input begins with the number N of students, followed by N (even) candy counts for the children counter-clockwise around the circle. The input ends with a student count of 0. Each input number is on a line by itself.  

  Output 

  For each game, output the number of rounds of the game followed by the amount of candy each child ends up with, both on one line.  

  Sample Input 

  6 

  36 

  2 

  2 

  2 

  2 

  2 

  11 

  22 

  20 

  18 

  16 

  14 

  12 

  10 

  8 

  6 

  4 

  2 

  4 

  2 

  4 

  6 

  8 

  0 

  Sample Output 

  15 14 

  17 22 

  4 8
模拟一遍过了 注意最后一个人的二分之一要在while内取

代码:

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int MAXN=10050;
int a[MAXN];
bool check(int a[],int n){
    for(int i=0;i<n-1;i++){
        if(a[i]!=a[i+1]){
            return false;
        }
    }
    return true;
}
int main(void){
    int n;
    while(~scanf("%d",&n) && n){
        memset(a,0,sizeof(a));
        int cnt=0;
        int step=0;
        for(int i=0;i<n;i++){
            scanf("%d",&a[i]);
        }
        int ans=0;
        while(!check(a,n)){
            ans++;
            int last=a[n-1]/2;
            for(int i=0;i<n;i++){
                int t=a[i]/2;
                a[i]=a[i]-t+last;
                last=t;
                if(a[i]%2){
                    a[i]++;
                }
            }
        }
        printf("%d %d\n",ans,a[0]);
    }
    return 0;
}
```





