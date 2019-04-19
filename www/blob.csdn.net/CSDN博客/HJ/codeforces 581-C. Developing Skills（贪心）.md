# codeforces 581-C. Developing Skills（贪心） - HJ - CSDN博客
2017年07月11日 11:26:45[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：220
所属专栏：[codeforces](https://blog.csdn.net/column/details/17151.html)
Petya loves computer games. Finally a game that he’s been waiting for so long came out!
The main character of this game has n different skills, each of which is characterized by an integer ai from 0 to 100. The higher the number ai is, the higher is the i-th skill of the character. The total rating of the character is calculated as the sum of the values ​​of for all i from 1 to n. The expression ⌊ x⌋ denotes the result of rounding the number x down to the nearest integer.
At the beginning of the game Petya got k improvement units as a bonus that he can use to increase the skills of his character and his total rating. One improvement unit can increase any skill of Petya’s character by exactly one. For example, if a4 = 46, after using one imporvement unit to this skill, it becomes equal to 47. A hero’s skill cannot rise higher more than 100. Thus, it is permissible that some of the units will remain unused.
Your task is to determine the optimal way of using the improvement units so as to maximize the overall rating of the character. It is not necessary to use all the improvement units. 
Input
The first line of the input contains two positive integers n and k (1 ≤ n ≤ 105, 0 ≤ k ≤ 107) — the number of skills of the character and the number of units of improvements at Petya’s disposal.
The second line of the input contains a sequence of n integers ai (0 ≤ ai ≤ 100), where ai characterizes the level of the i-th skill of the character. 
Output
The first line of the output should contain a single non-negative integer — the maximum total rating of the character that Petya can get using k or less improvement units. 
Examples 
Input
2 4 
7 9
Output
2
Input
3 8 
17 15 19
Output
5
Input
2 2 
99 100
Output
20
Note
In the first test case the optimal strategy is as follows. Petya has to improve the first skill to 10 by spending 3 improvement units, and the second skill to 10, by spending one improvement unit. Thus, Petya spends all his improvement units and the total rating of the character becomes equal to lfloor frac{100}{10} rfloor +  lfloor frac{100}{10} rfloor = 10 + 10 =  20.
In the second test the optimal strategy for Petya is to improve the first skill to 20 (by spending 3 improvement units) and to improve the third skill to 20 (in this case by spending 1 improvement units). Thus, Petya is left with 4 improvement units and he will be able to increase the second skill to 19 (which does not change the overall rating, so Petya does not necessarily have to do it). Therefore, the highest possible total rating in this example is ![这里写图片描述](http://codeforces.com/predownloaded/fc/45/fc454bca74c6dd49ec349b2d4e24724056e91237.png).
In the third test case the optimal strategy for Petya is to increase the first skill to 100 by spending 1 improvement unit. Thereafter, both skills of the character will be equal to 100, so Petya will not be able to spend the remaining improvement unit. So the answer is equal to![这里写图片描述](http://codeforces.com/predownloaded/05/f8/05f8611f710c0ca7c188f9020a10e40475030c69.png) .
题目大意： 
有n个技能，每个技能有一个初始熟练度([1,100])。现在有k个技能点，每个技能点可以让某个技能的熟练度+1（可以重复给同一个技能加熟练度，但熟练度不能超过100）。最后的得分是每个技能的熟练度整除10的求和。即∑（(int)(skill/10)）。问最后得分的最大值是多少？
解题思路： 
建一个结构体，一个存x，一个存10-x%10，然后按后面的数据从小到大排序即可。注意x的范围是不能超过100，因此还需要判断一些特殊条件。
详见代码：
```cpp
#include<iostream>
#include<algorithm>
using namespace std;
#define maxn 100005
struct node{
    int x,p;
}s[maxn];
int cmp(node a,node b)
{
    return a.p<b.p;
}
int main()
{
    int n,k;
    while(cin>>n>>k)
    {
        int cnt=0,q=0,sum=0;
        for(int i=0;i<n;i++)
        {
            cin>>s[i].x;
            sum+=s[i].x;
            cnt+=s[i].x/10;
            s[i].p=10-s[i].x%10;
        }
        if(n*100-sum<k)         //此时k已经大到让所有数据加到100仍有剩余 
        {
            cout<<n*10<<endl;
            continue;
        }
        sort(s,s+n,cmp);
        for(int i=0;i<n;i++,q++)
        {
            if(k<s[i].p)
              break;
            k-=s[i].p;
            cnt++;
        } 
        if(q==n)            //贪心取完余，剩下的就对10取整累加即可 
          cnt+=k/10;
        cout<<cnt<<endl;
    }
    return 0;
}
```
