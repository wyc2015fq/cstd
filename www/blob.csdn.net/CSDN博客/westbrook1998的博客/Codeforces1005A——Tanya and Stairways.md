# Codeforces1005A——Tanya and Stairways - westbrook1998的博客 - CSDN博客





2018年08月08日 22:18:42[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：33








> 
Little girl Tanya climbs the stairs inside a multi-storey building. Every time Tanya climbs a stairway, she starts counting steps from 1 to the number of steps in this stairway. She speaks every number aloud. For example, if she climbs two stairways, the first of which contains 3 steps, and the second contains 4 steps, she will pronounce the numbers 1,2,3,1,2,3,4. 

  You are given all the numbers pronounced by Tanya. How many stairways did she climb? Also, output the number of steps in each stairway. 

  The given sequence will be a valid sequence that Tanya could have pronounced when climbing one or more stairways. 

  Input 

  The first line contains n (1≤n≤1000) — the total number of numbers pronounced by Tanya. 

  The second line contains integers a1,a2,…,an (1≤ai≤1000) — all the numbers Tanya pronounced while climbing the stairs, in order from the first to the last pronounced number. Passing a stairway with x steps, she will pronounce the numbers 1,2,…,x in that order. 

  The given sequence will be a valid sequence that Tanya could have pronounced when climbing one or more stairways. 

  Output 

  In the first line, output t — the number of stairways that Tanya climbed. In the second line, output t numbers — the number of steps in each stairway she climbed. Write the numbers in the correct order of passage of the stairways. 

  Examples 

  Input 

  7 

  1 2 3 1 2 3 4 

  Output 

  2 

  3 4  

  Input 

  4 

  1 1 1 1 

  Output 

  4 

  1 1 1 1  

  Input 

  5 

  1 2 3 4 5 

  Output 

  1 

  5  

  Input 

  5 

  1 2 1 2 1 

  Output 

  3 

  2 2 1 
暑期训练第一天（其实只有两个人，论弱校的悲哀… 

水题 

代码：
```cpp
#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;
int n,ans;
const int MAXN=1010;
int a[MAXN];
vector<int> res;
int main(void){
    scanf("%d",&n);
    for(int i=0;i<n;i++){
        scanf("%d",&a[i]);
    }
    int t=0;
    for(int i=0;i<n;i++){
        if(a[i]==1){
            if(t>0){
                res.push_back(t);
            }
            ans++;
            t=1;
        }
        else if(i>0 && a[i]-a[i-1]==1){
            t++;
        }
    }
    res.push_back(t);
    printf("%d\n",ans);
    for(int i=0;i<res.size()-1;i++){
        printf("%d ",res[i]);
    }
    printf("%d\n",res[res.size()-1]);
    return 0;
}
```






