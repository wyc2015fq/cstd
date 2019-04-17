# hdu1370——Biorhythms - westbrook1998的博客 - CSDN博客





2018年09月02日 22:23:57[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：45








> 
Some people believe that there are three cycles in a person’s life that start the day he or she is born. These three cycles are the physical, emotional, and intellectual cycles, and they have periods of lengths 23, 28, and 33 days, respectively. There is one peak in each period of a cycle. At the peak of a cycle, a person performs at his or her best in the corresponding field (physical, emotional or mental). For example, if it is the mental curve, thought processes will be sharper and concentration will be easier.  

  Since the three cycles have different periods, the peaks of the three cycles generally occur at different times. We would like to determine when a triple peak occurs (the peaks of all three cycles occur in the same day) for any person. For each cycle, you will be given the number of days from the beginning of the current year at which one of its peaks (not necessarily the first) occurs. You will also be given a date expressed as the number of days from the beginning of the current year. You task is to determine the number of days from the given date to the next triple peak. The given date is not counted. For example, if the given date is 10 and the next triple peak occurs on day 12, the answer is 2, not 3. If a triple peak occurs on the given date, you should give the number of days to the next occurrence of a triple peak.  

  This problem contains multiple test cases!  

  The first line of a multiple input is an integer N, then a blank line followed by N input blocks. Each input block is in the format indicated in the problem description. There is a blank line between input blocks.  

  The output format consists of N output blocks. There is a blank line between output blocks.  

  Input 

  You will be given a number of cases. The input for each case consists of one line of four integers p, e, i, and d. The values p, e, and i are the number of days from the beginning of the current year at which the physical, emotional, and intellectual cycles peak, respectively. The value d is the given date and may be smaller than any of p, e, or i. All values are non-negative and at most 365, and you may assume that a triple peak will occur within 21252 days of the given date. The end of input is indicated by a line in which p = e = i = d = -1.  

  Output 

  For each test case, print the case number followed by a message indicating the number of days to the next triple peak, in the form:  

  Case 1: the next triple peak occurs in 1234 days.  

  Use the plural form “days” even if the answer is 1.  

  Sample Input 

  1 

  0 0 0 0 

  0 0 0 100 

  5 20 34 325 

  4 5 6 7 

  283 102 23 320 

  203 301 203 40 

  -1 -1 -1 -1 

  Sample Output 

  Case 1: the next triple peak occurs in 21252 days. 

  Case 2: the next triple peak occurs in 21152 days. 

  Case 3: the next triple peak occurs in 19575 days. 

  Case 4: the next triple peak occurs in 16994 days. 

  Case 5: the next triple peak occurs in 8910 days. 

  Case 6: the next triple peak occurs in 10789 days.
中国剩余定理的模板题，给3个数是上一个峰值到现在的时间，其实就是余数了嘛 

然后还给一个d，是上一个三峰到现在的时间，所以最后求出的就是ans-d

代码：

```
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
int m[3]={23,28,33};
int a[3];
int exgcd(int a,int b,int &x,int &y){
    if(!b){
        x=1;
        y=0;
        return a;
    }
    int d=exgcd(b,a%b,x,y);
    int t=x;
    x=y;
    y=t-a/b*y;
    return d;
}
int crt(int n){
    int M=1;
    int ans=0;
    for(int i=0;i<n;i++){
        M*=m[i];
    }
    for(int i=0;i<n;i++){
        int x,y;
        int Mi=M/m[i];
        exgcd(Mi,m[i],x,y);
        ans=(ans+Mi*x*a[i])%M;
    }
    if(ans<0){
        ans+=M;
    }
    return ans;
}
int main(void){
    int n;
    scanf("%d",&n);
    int p,e,i,d;
    int c=1;
    while(~scanf("%d%d%d%d",&a[0],&a[1],&a[2],&d)){
        if(a[0]==-1 && a[1]==-1 && a[2]==-1 && d==-1){
            break;
        }
        int ans=crt(3);
        ans-=d;
        if(ans<=0){
            ans+=21252;
        }
        printf("Case %d: the next triple peak occurs in %d days.\n",c++,ans);
    }
    return 0;
}
```





