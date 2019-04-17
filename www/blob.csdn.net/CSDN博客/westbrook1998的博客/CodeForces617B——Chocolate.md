# CodeForces617B——Chocolate - westbrook1998的博客 - CSDN博客





2018年08月05日 23:41:11[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：62








> 
Bob loves everything sweet. His favorite chocolate bar consists of pieces, each piece may contain a nut. Bob wants to break the bar of chocolate into multiple pieces so that each part would contain exactly one nut and any break line goes between two adjacent pieces. 

  You are asked to calculate the number of ways he can do it. Two ways to break chocolate are considered distinct if one of them contains a break between some two adjacent pieces and the other one doesn’t. 

  Please note, that if Bob doesn’t make any breaks, all the bar will form one piece and it still has to have exactly one nut. 

  Input 

  The first line of the input contains integer n (1 ≤ n ≤ 100) — the number of pieces in the chocolate bar. 

  The second line contains n integers ai (0 ≤ ai ≤ 1), where 0 represents a piece without the nut and 1 stands for a piece with the nut. 

  Output 

  Print the number of ways to break the chocolate into multiple parts so that each part would contain exactly one nut. 

  Examples 

  Input 

  3 

  0 1 0 

  Output 

  1 

  Input 

  5 

  1 0 1 0 1 

  Output 

  4 

  Note 

  In the first sample there is exactly one nut, so the number of ways equals 1 — Bob shouldn’t make any breaks. 

  In the second sample you can break the bar in four ways: 

  10|10|1 

  1|010|1 

  10|1|01 

  1|01|01
应该算是一道思维题 一个01串分割成多个部分 每个部分必须要有且只有一个1  

尝试了一下发现很简单 只要找出两个1之间有多少个0就行 n个0就有n+1中划分方式 然后最后把这些乘起来 

然后就是注意前导零要忽略 全0的特判 只有1个1的特判
代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;
const int MAXN=105;
int a[MAXN];
int main(void){
    int n;
    int one_num=0;
    scanf("%d",&n);
    for(int i=0;i<n;i++){
        scanf("%d",&a[i]);
        if(a[i]==1){
            one_num++;
        }
    }
    if(one_num==1){
        printf("1\n");
    }
    else if(one_num==0){
        printf("0\n");
    }
    else{
        vector<int> o;
        bool first=false;
        int tmp=1;
        for(int i=0;i<n;i++){
            if(!first){
                if(a[i]==0){
                    continue;
                }
                else{
                    first=true;
                }
            }
            else{
                if(a[i]==0){
                    tmp++;
                }
                else{
                    o.push_back(tmp);
                    tmp=1;
                }
            }
        }
        int s=o.size();
        long long res=1;
        for(int i=0;i<s;i++){
            res*=o[i];
        }
        printf("%lld\n",res);
    }
    return 0;
}
```






