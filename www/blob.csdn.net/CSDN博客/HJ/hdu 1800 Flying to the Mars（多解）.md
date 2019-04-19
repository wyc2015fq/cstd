# hdu  1800 Flying to the Mars（多解） - HJ - CSDN博客
2017年01月25日 17:11:58[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：268
Problem Description 
![这里写图片描述](http://acm.hdu.edu.cn/data/images/1800-1.jpg)
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
Author 
PPF@JLU
题目大意： 
   统计一组字符串中重复出现单个字符串次数最多的结果
解法一： 
字典树
```cpp
#include<iostream>
#include<cstring>
#include<cstdio>
using namespace std;
struct node{
    int count;
    node *child[15];
    node(){
        memset(child,0,sizeof(child));
        count=0;
    }
}; 
node *root;
int maxn;
void insert(char *s)
{
    node *p=root;
    for(int i=0;i<strlen(s);i++)
    {
        int m=s[i]-'0';
        if(!p->child[m])
          p->child[m]=new node;
        p=p->child[m];
    } 
    p->count++;
} 
void find(node *p)
{
    if(p->count>maxn)
      maxn=p->count;
    for(int i=0;i<10;i++)
    {
        if(p->child[i])
          find(p->child[i]);
    }
}
void del(node *root)
{
    for(int i=0;i<10;i++)
      if(root->child[i])
        del(root->child[i]);
    delete root;
}
int main()
{
    int n;
    while(~scanf("%d",&n))
    {
        char s[35];
        root=new node;
        maxn=0;
        for(int i=0;i<n;i++)
        {
            scanf("%s",s);
            int j;
            for(j=0;j<strlen(s);j++)
              if(s[j]!='0')
                break;
            insert(s+j);
        }
        find(root);
        printf("%d\n",maxn);
        del(root);
    }
    return 0;
}
```
解法二： 
map
```cpp
#include<iostream>
#include<cstdio>
#include<map>
#include<algorithm>
using namespace std;
const int INF=0x3f3f3f3f;
int main()
{
    int n,b;
    while(~scanf("%d",&n))
    {
        map<int,int>mp;
        int max=-INF;
        for(int i=0;i<n;i++)
        {
            scanf("%d",&b);
            mp[b]++;
            if(mp[b]>max)
              max=mp[b];
        }
        printf("%d\n",max);
    }
    return 0;
}
```
解法三： 
哈希
```cpp
#include<iostream>
#include<cstdio> 
#include<cstring>
#include<map> 
using namespace std;
unsigned int BKDRHash(char *a)
{
    while(*a=='0')
      *a++;
    unsigned int seed=131;
    unsigned int hash=0;
    while(*a)
      hash=hash*seed+*a++;      //字符串哈希 
    return hash&0x7FFFFFFF;
}
int main()
{
    int n;
    while(~scanf("%d",&n))
    {
        char ss[35];
        int max=0,index;
        map<int,int>Hash;
        for(int i=0;i<n;i++)
        {
            scanf("%s",ss);
            index=BKDRHash(ss);
            Hash[index]++;
            if(Hash[index]>max)
              max=Hash[index];
        }
        printf("%d\n",max);
    }
    return 0;
}
```
解法四： 
暴力。。。（居然能过）
```cpp
#include<iostream>
#include<cstdio>
#include<algorithm>
using namespace std;
struct node{
    char s[35];
}a[3005];
int cmp(node x,node y)
{
    return strcmp(x.s,y.s)<0;
}
int main()
{
    int n;
    while(~scanf("%d",&n))
    {
        int ans=1,cnt=1;
        for(int i=0;i<n;i++)
          scanf("%s",a[i].s);
        sort(a,a+n,cmp);
        for(int i=n-1;i>0;i--)
        {
            if(strcmp(a[i].s,a[i-1].s)==0)
              cnt++;
            else
            {
                if(ans<cnt)
                  ans=cnt;
                cnt=1;
            }
        }
        if(ans<cnt)
          ans=cnt;
        printf("%d\n",ans);
    }
    return 0;
}
```
可能我做的是假题
