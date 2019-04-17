# hdu1800——Flying to the Mars - westbrook1998的博客 - CSDN博客





2018年05月31日 14:01:51[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：61








> 
In the year 8888, the Earth is ruled by the PPF Empire . As the population growing , PPF needs to find more land for the newborns . Finally , PPF decides to attack Kscinow who ruling the Mars . Here the problem comes! How can the soldiers reach the Mars ? PPF convokes his soldiers and asks for their suggestions . “Rush … ” one soldier answers. “Shut up ! Do I have to remind you that there isn’t any road to the Mars from here!” PPF replies. “Fly !” another answers. PPF smiles :“Clever guy ! Although we haven’t got wings , I can buy some magic broomsticks from HARRY POTTER to help you .” Now , it’s time to learn to fly on a broomstick ! we assume that one soldier has one level number indicating his degree. The soldier who has a higher level could teach the lower , that is to say the former’s level > the latter’s . But the lower can’t teach the higher. One soldier can have only one teacher at most , certainly , having no teacher is also legal. Similarly one soldier can have only one student at most while having no student is also possible. Teacher can teach his student on the same broomstick .Certainly , all the soldier must have practiced on the broomstick before they fly to the Mars! Magic broomstick is expensive !So , can you help PPF to calculate the minimum number of the broomstick needed .  

  For example :  

  There are 5 soldiers (A B C D E)with level numbers : 2 4 5 6 4;  

  One method :  

  C could teach B; B could teach A; So , A B C are eligible to study on the same broomstick.  

  D could teach E;So D E are eligible to study on the same broomstick;  

  Using this method , we need 2 broomsticks.  

  Another method:  

  D could teach A; So A D are eligible to study on the same broomstick.  

  C could teach B; So B C are eligible to study on the same broomstick.  

  E with no teacher or student are eligible to study on one broomstick.  

  Using the method ,we need 3 broomsticks.  

  …… 
After checking up all possible method, we found that 2 is the minimum number of broomsticks needed.  

Input 

Input file contains multiple test cases.  

In a test case,the first line contains a single positive number N indicating the number of soldiers.(0<=N<=3000)  

Next N lines :There is only one nonnegative integer on each line , indicating the level number for each soldier.( less than 30 digits);  

Output 

For each case, output the minimum number of broomsticks on a single line. 

Sample Input 

4 

10 

20 

30 

04 

5 

2 

3 

4 

3 

4 

Sample Output 

1 

2
题意就是找出有几个严格的单调递增序列，但因为数字会很大，30位，所以要以字符串读取，然后用新学的哈希算法，相当于把这些字符串转换成各不相同的数字，然后找出其中有多少个相同的即至少有多少个上升子序列

代码：

```cpp
#include <cstdio>
#include <algorithm>
using namespace std;
const int MAXN=3050;
char s[40];
int Hash[MAXN];
int BKDRHash(char *s){
    long long seed=131;
    long long hash=0;
    while(*s=='0'){
        s++;
    }
    while(*s){
        hash=hash*seed+(*s++);
    }
    return (hash & 0x7FFFFFFF);
}
int main(void){
    int n;
    while(~scanf("%d",&n)){
        int ans=1;
        for(int i=0;i<n;i++){
            scanf("%s",s);
            Hash[i]=BKDRHash(s);
        }
        sort(Hash,Hash+n);
        int t=1;
        for(int i=1;i<n;i++){
            if(Hash[i]==Hash[i-1]){
                t++;
                if(t>ans){
                    ans=t;
                }
            }
            else{
                t=1;
            }
        }
        printf("%d\n",ans);
    }
}
```






