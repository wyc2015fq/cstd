# Codeforces988A——Diverse Team - westbrook1998的博客 - CSDN博客





2018年08月16日 13:08:16[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：52标签：[水题](https://so.csdn.net/so/search/s.do?q=水题&t=blog)








> 
There are n students in a school class, the rating of the i-th student on Codehorses is ai. You have to form a team consisting of k students (1≤k≤n) such that the ratings of all team members are distinct. 

  If it is impossible to form a suitable team, print “NO” (without quotes). Otherwise print “YES”, and then print k distinct numbers which should be the indices of students in the team you form. If there are multiple answers, print any of them. 

  Input 

  The first line contains two integers n and k (1≤k≤n≤100) — the number of students and the size of the team you have to form. 

  The second line contains n integers a1,a2,…,an (1≤ai≤100), where ai is the rating of i-th student. 

  Output 

  If it is impossible to form a suitable team, print “NO” (without quotes). Otherwise print “YES”, and then print k distinct integers from 1 to n which should be the indices of students in the team you form. All the ratings of the students in the team should be distinct. You may print the indices in any order. If there are multiple answers, print any of them. 

  Assume that the students are numbered from 1 to n. 

  Examples 

  Input 

  5 3 

  15 13 15 15 12 

  Output 

  YES 

  1 2 5  

  Input 

  5 4 

  15 13 15 15 12 

  Output 

  NO 

  Input 

  4 4 

  20 10 40 30 

  Output 

  YES 

  1 2 3 4  

  Note 

  All possible answers for the first example: 

  {1 2 5} 

  {2 3 5} 

  {2 4 5} 

  Note that the order does not matter.
这题用map或者数组都可以过，但还是wa了几次，原因就是没考虑不同rating的人数超过k的情况，要break

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <map>
#include <vector>
using namespace std;
int n,k;
vector<int> res;
int a[105];
bool vis[105];
int main(void){
    scanf("%d%d",&n,&k);
    for(int i=1;i<=n;i++){
        scanf("%d",&a[i]);
    }
    for(int i=1;i<=n;i++){
        if(!vis[a[i]]){
            vis[a[i]]=true;
            res.push_back(i);
        }
        if(res.size()==k){
            break;
        }
    }
    if(res.size()<k){
        printf("NO\n");
    }
    else{
        printf("YES\n");
        int l=res.size();
        for(int i=0;i<l-1;i++){
            printf("%d ",res[i]);
        }
        printf("%d\n",res[l-1]);
    }
    return 0;
}
```





