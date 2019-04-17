# hdu2594——Simpsons’ Hidden Talents - westbrook1998的博客 - CSDN博客





2018年08月21日 21:19:58[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：27标签：[KMP																[字符串](https://so.csdn.net/so/search/s.do?q=字符串&t=blog)](https://so.csdn.net/so/search/s.do?q=KMP&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
Homer: Marge, I just figured out a way to discover some of the talents we weren’t aware we had.  

  Marge: Yeah, what is it?  

  Homer: Take me for example. I want to find out if I have a talent in politics, OK?  

  Marge: OK.  

  Homer: So I take some politician’s name, say Clinton, and try to find the length of the longest prefix  

  in Clinton’s name that is a suffix in my name. That’s how close I am to being a politician like Clinton  

  Marge: Why on earth choose the longest prefix that is a suffix???  

  Homer: Well, our talents are deeply hidden within ourselves, Marge.  

  Marge: So how close are you?  

  Homer: 0!  

  Marge: I’m not surprised.  

  Homer: But you know, you must have some real math talent hidden deep in you.  

  Marge: How come?  

  Homer: Riemann and Marjorie gives 3!!!  

  Marge: Who the heck is Riemann?  

  Homer: Never mind.  

  Write a program that, when given strings s1 and s2, finds the longest prefix of s1 that is a suffix of s2. 

  Input 

  Input consists of two lines. The first line contains s1 and the second line contains s2. You may assume all letters are in lowercase. 

  Output 

  Output consists of a single line that contains the longest string that is a prefix of s1 and a suffix of s2, followed by the length of that prefix. If the longest such string is the empty string, then the output should be 0.  

  The lengths of s1 and s2 will be at most 50000. 

  Sample Input 

  clinton 

  homer 

  riemann 

  marjorie 

  Sample Output 

  0 

  rie 3
求两个字符串其中最大的1的前缀2的后缀 

我的想法是把两个字符串连起来 然后求一下next数组 但是忘记考虑这个串不能超过s1和s2 所以先要判断next[len]是不是符合条件 不符合就往前跳next[next[len]]

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
#define _clr(x,a) memset(x,a,sizeof(x));
using namespace std;
const int N=100010;
char s[N];
int Next[N];
void kmp_pre(char* s){
    _clr(Next,0);
    Next[0]=-1;
    int i=0,j=-1;
    int l=strlen(s);
    while(i<l){
        if(j==-1 || s[i]==s[j]){
            Next[++i]=++j;
        }
        else{
            j=Next[j];
        }
    }
}
int main(void){
    while(~scanf("%s",s)){
        int m=strlen(s);
        scanf("%s",s+m);
        kmp_pre(s);
        int n=strlen(s);
        // for(int i=0;i<=n;i++){
        //     printf("%d ",Next[i]);
        // }
        // printf("\n");
        if(Next[n]==0){
            printf("0\n");
        }
        else{
            int t=n;
            while(Next[t]>m || Next[t]>(n-m)){
                t=Next[t];
            }
            int i=0;
            //printf("%d\n",n);
            for(;i<Next[t];i++){
                printf("%c",s[i]);
            }
            printf(" %d\n",i);
        }
    }
    return 0;
}
```






