# Cyclic Nacklace（HDU-3746） - Alex_McAvoy的博客 - CSDN博客





2019年03月02日 13:29:46[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：68
个人分类：[HDU																[字符串处理——单模式匹配(KMP)](https://blog.csdn.net/u011815404/article/category/8698890)](https://blog.csdn.net/u011815404/article/category/7923257)








> 
# Problem Description

CC always becomes very depressed at the end of this month, he has checked his credit card yesterday, without any surprise, there are only 99.9 yuan left. he is too distressed and thinking about how to tide over the last days. Being inspired by the entrepreneurial spirit of "HDU CakeMan", he wants to sell some little things to make money. Of course, this is not an easy task. 

As Christmas is around the corner, Boys are busy in choosing christmas presents to send to their girlfriends. It is believed that chain bracelet is a good choice. However, Things are not always so simple, as is known to everyone, girl's fond of the colorful decoration to make bracelet appears vivid and lively, meanwhile they want to display their mature side as college students. after CC understands the girls demands, he intends to sell the chain bracelet called CharmBracelet. The CharmBracelet is made up with colorful pearls to show girls' lively, and the most important thing is that it must be connected by a cyclic chain which means the color of pearls are cyclic connected from the left to right. And the cyclic count must be more than one. If you connect the leftmost pearl and the rightmost pearl of such chain, you can make a CharmBracelet. Just like the pictrue below, this CharmBracelet's cycle is 9 and its cyclic count is 2: 

![](https://img-blog.csdnimg.cn/20190227205244467.jpg)

Now CC has brought in some ordinary bracelet chains, he wants to buy minimum number of pearls to make CharmBracelets so that he can save more money. but when remaking the bracelet, he can only add color pearls to the left end and right end of the chain, that is to say, adding to the middle is forbidden. 

CC is satisfied with his ideas and ask you for help.

# **Input**

The first line of the input is a single integer T ( 0 < T <= 100 ) which means the number of test cases. 

Each test case contains only one line describe the original ordinary chain to be remade. Each character in the string stands for one pearl and there are 26 kinds of pearls being described by 'a' ~'z' characters. The length of the string Len: ( 3 <= Len <= 100000 ).

# Output

For each case, you are required to output the minimum count of pearls added to make a CharmBracelet.

# Sample Input

**3aaaabcaabcde**

# Sample Output

**025**


题意：给出 t 个字符串，对于每个字符串，问最少在结尾补几个字符能使得字符串变为循环串

思路：使用 KMP 的 next 数组即可

对于一个字符串，要使其变为循环串，最少具有两个循环节，根据 next 数组可知，对于长度为 n 的字符串，当 n%(n-next[n])=0 时，说明字符串循环，且循环节长度为 n-next[n]

因此可先判断每个字符串是否已为循环串，若已循环，则不需加字符，输出 0 即可，若不为循环串，输出 n-Next[n]-Next[n]%(n-Next[n]) 即可

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<string>
#include<cstring>
#include<cmath>
#include<ctime>
#include<algorithm>
#include<utility>
#include<stack>
#include<queue>
#include<vector>
#include<set>
#include<map>
#define PI acos(-1.0)
#define E 1e-9
#define INF 0x3f3f3f3f
#define LL long long
const int MOD=10007;
const int N=200000+5;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;
int Next[N];
char str[N];
int num[N];
void getNext(char p[]){
    Next[0]=-1;
    int len=strlen(p);
    int j=0;
    int k=-1;

    while(j<len){
        if(k==-1||p[j]==p[k]) {
            k++;
            j++;
            Next[j]=k;
        }else{
            k=Next[k];
        }
    }
}
int main(){
    int t;
    scanf("%d",&t);
    while(t--){
        scanf("%s",str);
        getNext(str);
        int n=strlen(str);//字符串长度
        int len=n-Next[n];//循环节的长度


        if(len!=n&&n%len==0)//已为循环串
            printf("0\n");
        else
            printf("%d\n",len-Next[n]%len);//n-Next[n]-Next[n]%(n-Next[n])
    }
    return 0;
}
```






