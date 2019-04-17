# The Stable Marriage Problem（POJ-3487） - Alex_McAvoy的博客 - CSDN博客





2018年08月03日 22:02:31[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：169








> 
# Problem Description

The stable marriage problem consists of matching members of two different sets according to the member’s preferences for the other set’s members. The input for our problem consists of:

a set M of n males;

a set F of n females;

for each male and female we have a list of all the members of the opposite gender in order of preference (from the most preferable to the least).

A marriage is a one-to-one mapping between males and females. A marriage is called stable, if there is no pair (m, f) such that f ∈ F prefers m ∈ M to her current partner and m prefers f over his current partner. The stable marriage A is called male-optimal if there is no other stable marriage B, where any male matches a female he prefers more than the one assigned in A.

Given preferable lists of males and females, you must find the male-optimal stable marriage.

# **Input**

The first line gives you the number of tests. The first line of each test case contains integer n (0 < n < 27). Next line describes n male and n female names. Male name is a lowercase letter, female name is an upper-case letter. Then go n lines, that describe preferable lists for males. Next n lines describe preferable lists for females.

# Output

For each test case find and print the pairs of the stable marriage, which is male-optimal. The pairs in each test case must be printed in lexicographical order of their male names as shown in sample output. Output an empty line between test cases.

# Sample Input

**2**

**3**

**a b c A B C**

**a:BAC**

**b:BAC**

**c:ACB**

**A:acb**

**B:bac**

**C:cab**

**3**

**a b c A B C**

**a:ABC**

**b:ABC**

**c:BCA**

**A:bac**

**B:acb**

**C:abc**

# Sample Output

**a A**

**b B**

**c C**

**a B**

**b A**

**c C**


**题意：**

稳定婚姻系统问题如下：

1）集合 M 表示 n 个男性

2）集合 F 表示 n 个女性

3）对于每个人我们都按异性的中意程度给出一份名单（从最中意的到最不中意的） 

如果没有 ![(m,f),f\in F,m\in M](https://private.codecogs.com/gif.latex?%28m%2Cf%29%2Cf%5Cin%20F%2Cm%5Cin%20M)，f 对 m 比对她的配偶中意的同时 m 对 f 比对他的配偶更加中意，那么这个婚姻是稳定的。

如果一个稳定配对不存在另一个稳定婚姻配对，其中所有的男性的配偶都比现在强，那么这样的稳定配对称为男性最优配对。

给出 n 对男女，以小写字母表示男性名字，大写字母表示女性名字，再给出每个男性的中意名单（从大到小），最后是女性的中意名单（从大到小）。

对于每组测试数据输出一组男性最优配对，男性按字典序从小到大排序，每行为：“男姓名 女性名” 的形式。

**思路：**裸的延迟认可算法，此题为模版题

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<string>
#include<cstdlib>
#include<queue>
#include<set>
#include<map>
#include<stack>
#include<ctime>
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 10001
#define MOD 123
#define E 1e-6
using namespace std;
int couple;//总共多少对
int Male_Like[N][N];//男性对女性的喜欢程度
int Female_Like[N][N];//女性对男性的喜欢程度
int Male_Choice[N];//男性的选择
int Female_Choice[N];//女性的选择
int Male_Name[N],Female_Name[N];//姓名的hash
queue<int> Free_Male;//没有配对的男性
char str[N];
int main()
{
    int t;
    while(scanf("%d",&t)!=EOF)
    {
        scanf("%d",&couple);

        while(!Free_Male.empty())//清空队列
            Free_Male.pop();

        for(int i=0;i<couple;i++)//存入男性名字，初始都没有配对
        {
            scanf("%s",str);
            Male_Name[i]=str[0]-'a';
            Free_Male.push(Male_Name[i]);
        }

        for(int i=0;i<couple;i++)//存入女性名字，初始都没有配对
        {
            scanf("%s",str);
            Female_Name[i]=str[0]-'A';
        }

        sort(Male_Name,Male_Name+couple);//对名字排序

        for(int i=0;i<couple;i++)//男性对女性的印象，按降序排列
        {
            scanf("%s",str);
            for(int j=0;j<couple;j++)
                Male_Like[i][j]=str[j+2]-'A';
        }

        for(int i=0;i<couple;i++)//女性对男性的印象，添加一虚拟人物，编号为couple，为女性的初始对象
        {
            scanf("%s",str);
            for(int j=0;j<couple;j++)
                Female_Like[i][str[j+2]-'a']=couple-j;
            Female_Like[i][couple]=0;
        }

        memset(Male_Choice,0,sizeof(Male_Choice));//所有男性初始选择都是最喜欢的女性

        for(int i=0;i<couple;i++)//先添加女性的初始对象
            Female_Choice[i]=couple;

        while(!Free_Male.empty())
        {
            int male=Free_Male.front();//找出一个未配对的男性
            int female=Male_Like[male][Male_Choice[male]];//男性心仪的女性

            if(Female_Like[female][male]>Female_Like[female][Female_Choice[female]])//女性对男性的喜爱度大于当前对象
            {
                Free_Male.pop();//男性脱单

                if(Female_Choice[female]!=couple)//如果有前男友且不为虚拟对象couple
                {
                    Free_Male.push(Female_Choice[female]);//其前男友进入队列，重新变为光棍
                    Male_Choice[Female_Choice[female]]++;//其前男友考虑其下一对象
                }

                Female_Choice[female]=male;//当前男友为当前男性
            }
            else//如果被女性拒绝
                Male_Choice[male]++;//考虑下一对象
        }

        for(int i=0;i<couple;i++)
            printf("%c %c\n",Male_Name[i]+'a',Male_Like[Male_Name[i]][Male_Choice[Male_Name[i]]]+'A');

        printf("\n");
    }

    return 0;
}
```





