# hdu3746——Cyclic Nacklace - westbrook1998的博客 - CSDN博客





2018年08月21日 11:17:49[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：36标签：[KMP																[字符串匹配](https://so.csdn.net/so/search/s.do?q=字符串匹配&t=blog)](https://so.csdn.net/so/search/s.do?q=KMP&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
CC always becomes very depressed at the end of this month, he has checked his credit card yesterday, without any surprise, there are only 99.9 yuan left. he is too distressed and thinking about how to tide over the last days. Being inspired by the entrepreneurial spirit of “HDU CakeMan”, he wants to sell some little things to make money. Of course, this is not an easy task. ![这里写图片描述](https://odzkskevi.qnssl.com/53f6d763e6047bb71352513fe2805f00?v=1534368950)

  As Christmas is around the corner, Boys are busy in choosing christmas presents to send to their girlfriends. It is believed that chain bracelet is a good choice. However, Things are not always so simple, as is known to everyone, girl’s fond of the colorful decoration to make bracelet appears vivid and lively, meanwhile they want to display their mature side as college students. after CC understands the girls demands, he intends to sell the chain bracelet called CharmBracelet. The CharmBracelet is made up with colorful pearls to show girls’ lively, and the most important thing is that it must be connected by a cyclic chain which means the color of pearls are cyclic connected from the left to right. And the cyclic count must be more than one. If you connect the leftmost pearl and the rightmost pearl of such chain, you can make a CharmBracelet. Just like the pictrue below, this CharmBracelet’s cycle is 9 and its cyclic count is 2: 
Now CC has brought in some ordinary bracelet chains, he wants to buy minimum number of pearls to make CharmBracelets so that he can save more money. but when remaking the bracelet, he can only add color pearls to the left end and right end of the chain, that is to say, adding to the middle is forbidden.  

CC is satisfied with his ideas and ask you for help. 

Input 

The first line of the input is a single integer T ( 0 < T <= 100 ) which means the number of test cases.  

Each test case contains only one line describe the original ordinary chain to be remade. Each character in the string stands for one pearl and there are 26 kinds of pearls being described by ‘a’ ~’z’ characters. The length of the string Len: ( 3 <= Len <= 100000 ). 

Output 

For each case, you are required to output the minimum count of pearls added to make a CharmBracelet. 

Sample Input 

3 

aaa 

abca 

abcde 

Sample Output 

0 

2 

5
要求补充多少个字符之后能得到循环 

求一下最小循环节的长度`len-Next[len]` 然后看剩下的字符串

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
#define _clr(x,a) memset(x,a,sizeof(x))
using namespace std;
const int N=100020;
char s[N];
int Next[N];
void kmp_pre(char* p){
    _clr(Next,0);
    int i=0;
    int j=-1;
    int l=strlen(p);
    Next[0]=-1;
    while(i<l){
        if(j==-1 || p[i]==p[j]){
            i++;
            j++;
            Next[i]=j;
        }
        else{
            j=Next[j];
        }
    }
}
int main(void){
    int t;
    scanf("%d",&t);
    while(t--){
        scanf("%s",s);
        kmp_pre(s);
        int n=strlen(s);
        // for(int i=0;i<=n;i++){
        //     printf("%d ",Next[i]);
        // }
        // printf("\n");
        // int cnt=n-2*Next[n];
        // if(cnt<0){
        //     cnt=0;
        // }
        int cnt=n-Next[n];
        if(n%cnt==0){
            cnt=0;
        }
        else{
            cnt=cnt-n%cnt;
        }
        if(Next[n]==0){
            cnt=n;
        }
        printf("%d\n",cnt);
    }
    return 0;
}
```







