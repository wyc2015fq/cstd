# UVa10474——Where is the Marble? - westbrook1998的博客 - CSDN博客





2018年05月14日 22:14:55[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：47








> 
Raju and Meena love to play with Marbles. They have got a lot of 

  marbles with numbers written on them. At the beginning, Raju would 

  place the marbles one after another in ascending order of the numbers 

  written on them. Then Meena would ask Raju to find the first marble 

  with a certain number. She would count 1…2…3. Raju gets one point 

  for correct answer, and Meena gets the point if Raju fails. After some 

  fixed number of trials the game ends and the player with maximum 

  points wins. Today it’s your chance to play as Raju. Being the smart 

  kid, you’d be taking the favor of a computer. But don’t underestimate 

  Meena, she had written a program to keep track how much time you’re 

  taking to give all the answers. So now you have to write a program, 

  which will help you in your role as Raju. 

  Input 

  There can be multiple test cases. Total no of test cases is less than 65. Each test case consists begins 

  with 2 integers: N the number of marbles and Q the number of queries Mina would make. The next 

  N lines would contain the numbers written on the N marbles. These marble numbers will not come 

  in any particular order. Following Q lines will have Q queries. Be assured, none of the input numbers 

  are greater than 10000 and none of them are negative. 

  Input is terminated by a test case where N = 0 and Q = 0. 

  Output 

  For each test case output the serial number of the case. 

  For each of the queries, print one line of output. The format of this line will depend upon whether 

  or not the query number is written upon any of the marbles. The two different formats are described 

  below: 

  • ‘x found at y’, if the first marble with number x was found at position y. Positions are numbered 

  1, 2, … , N. 

  • ‘x not found’, if the marble with number x is not present. 

  Look at the output for sample input for details. 

  Sample Input 

  4 1 

  2 

  3 

  5 

  1 

  5 

  5 2 

  1 

  3 

  3 

  3 

  1 

  2 

  3 

  0 0 

  Sample Output 

  CASE# 1: 

  5 found at 4 

  CASE# 2: 

  2 not found 

  3 found at 3
紫书上的练习题，英语终究是个硬伤，最后还是百度加看样例才看懂的，很水的一道题，主要是熟悉`sort()`和`lower_bound()`的操作

代码：

```cpp
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
const int MAXN=10050;
int a[MAXN];
int n,q,t;
int main(void){
    int c=1;
    while(~scanf("%d%d",&n,&q)){
        if(n==0 && q==0){
            break;
        }
        memset(a,0,sizeof(n));
        for(int i=0;i<n;i++){
            scanf("%d",&a[i]);
        }
        sort(a,a+n);
        for(int i=0;i<q;i++){
            scanf("%d",&t);
            if(i==0){
                printf("CASE# %d:\n",c++);
            }
            int f=lower_bound(a,a+n,t)-a;
            if(f>=0 && f<n && a[f]==t){
                printf("%d found at %d\n",t,f+1);
            }
            else{
                printf("%d not found\n",t);
            }
        }
    }
    return 0;
}
```





