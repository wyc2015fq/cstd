# Play on Words（POJ-1386） - Alex_McAvoy的博客 - CSDN博客





2018年11月02日 21:32:00[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：56
个人分类：[POJ																[图论——图的遍历](https://blog.csdn.net/u011815404/article/category/8236846)](https://blog.csdn.net/u011815404/article/category/7504500)








> 
# Problem Description

Some of the secret doors contain a very interesting word puzzle. The team of archaeologists has to solve it to open that doors. Because there is no other way to open the doors, the puzzle is very important for us. 

There is a large number of magnetic plates on every door. Every plate has one word written on it. The plates must be arranged into a sequence in such a way that every word begins with the same letter as the previous word ends. For example, the word ``acm'' can be followed by the word ``motorola''. Your task is to write a computer program that will read the list of words and determine whether it is possible to arrange all of the plates in a sequence (according to the given rule) and consequently to open the door. 

# **Input**

The input consists of T test cases. The number of them (T) is given on the first line of the input file. Each test case begins with a line containing a single integer number Nthat indicates the number of plates (1 <= N <= 100000). Then exactly Nlines follow, each containing a single word. Each word contains at least two and at most 1000 lowercase characters, that means only letters 'a' through 'z' will appear in the word. The same word may appear several times in the list.

# Output

Your program has to determine whether it is possible to arrange all the plates in a sequence such that the first letter of each word is equal to the last letter of the previous word. All the plates from the list must be used, each exactly once. The words mentioned several times must be used that number of times. 

If there exists such an ordering of plates, your program should print the sentence "Ordering is possible.". Otherwise, output the sentence "The door cannot be opened.". 

# Sample Input

**32acmibm3acmmalformmouse2okok**

# Sample Output

**The door cannot be opened.Ordering is possible.The door cannot be opened.**


题意：给出 n 个单词，问能否将所有单词组成一个序列，要求序列的前一个单词的尾字母与后一个单词的头字母相同

思路：实质是求一个有向图是否存在欧拉回路或欧拉通路的问题

由于每个单词只能从单词头走到单词尾，因此将每个单词看成一个有向边，把26个字母看成图的节点，然后求序列就是求是否存在欧拉回路或欧拉通路

若图中所有点的入度等于出度，则存在欧拉回路，若有两个点入度不等于出度，但两者之差的绝对值等于1，则存在欧拉通路。

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
#define N 1001
#define MOD 16007
#define E 1e-6
#define LL long long
using namespace std;
int m;
int degIn[N],degOut[N];
int father[N];
int Find(int x){
    if(father[x]==-1)
        return x;
    return father[x]=Find(father[x]);
}
void Union(int x,int y){
    x=Find(x);
    y=Find(y);
    if(x!=y)
        father[x]=y;
}
int main(){

    int t;
    scanf("%d",&t);
    while(t--){
        memset(degIn,0,sizeof(degIn));
        memset(degOut,0,sizeof(degOut));
        memset(father,-1,sizeof(father));

        scanf("%d",&m);
        for(int i=1;i<=m;i++){
            char str[2000];
            scanf("%s",str);

            int len=strlen(str);
            int x=str[0]-'a';
            int y=str[len-1]-'a';

            degIn[x]++;
            degOut[y]++;
            Union(x,y);
        }

        int cnt=0;//统计连通分量
        for(int i=0;i<26;i++)
            if( (degIn[i]||degOut[i]) && (Find(i)==i) )
                    cnt++;
        if(cnt>1)
            printf("The door cannot be opened.\n");
        else{
             //出度不等于入度的三种情况
            int situation1=0;
            int situation2=0;
            int situation3=0;

            for(int i=0;i<26;i++)
            {
                if(degIn[i]==degOut[i])
                    continue;
                else if(degIn[i]-degOut[i]==1)
                    situation1++;
                else if(degOut[i]-degIn[i]==1)
                    situation2++;
                else
                    situation3++;
            }
            if( ( (situation1==situation2&&situation1==1)||(situation1==situation2&&situation1==0) )&&situation3==0 )
                printf("Ordering is possible.\n");
            else
                printf("The door cannot be opened.\n");
        }
    }
    return 0;
}
```






