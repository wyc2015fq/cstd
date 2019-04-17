# hdu1686——Oulipo - westbrook1998的博客 - CSDN博客





2018年08月21日 11:15:23[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：36标签：[KMP																[字符串匹配](https://so.csdn.net/so/search/s.do?q=字符串匹配&t=blog)](https://so.csdn.net/so/search/s.do?q=KMP&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
The French author Georges Perec (1936–1982) once wrote a book, La disparition, without the letter ‘e’. He was a member of the Oulipo group. A quote from the book:  

  Tout avait Pair normal, mais tout s’affirmait faux. Tout avait Fair normal, d’abord, puis surgissait l’inhumain, l’affolant. Il aurait voulu savoir où s’articulait l’association qui l’unissait au roman : stir son tapis, assaillant à tout instant son imagination, l’intuition d’un tabou, la vision d’un mal obscur, d’un quoi vacant, d’un non-dit : la vision, l’avision d’un oubli commandant tout, où s’abolissait la raison : tout avait l’air normal mais…  

  Perec would probably have scored high (or rather, low) in the following contest. People are asked to write a perhaps even meaningful text on some subject with as few occurrences of a given “word” as possible. Our task is to provide the jury with a program that counts these occurrences, in order to obtain a ranking of the competitors. These competitors often write very long texts with nonsense meaning; a sequence of 500,000 consecutive ‘T’s is not unusual. And they never use spaces.  

  So we want to quickly find out how often a word, i.e., a given string, occurs in a text. More formally: given the alphabet {‘A’, ‘B’, ‘C’, …, ‘Z’} and two finite strings over that alphabet, a word W and a text T, count the number of occurrences of W in T. All the consecutive characters of W must exactly match consecutive characters of T. Occurrences may overlap.  

  Input 

  The first line of the input file contains a single number: the number of test cases to follow. Each test case has the following format:  

  One line with the word W, a string over {‘A’, ‘B’, ‘C’, …, ‘Z’}, with 1 ≤ |W| ≤ 10,000 (here |W| denotes the length of the string W).  

  One line with the text T, a string over {‘A’, ‘B’, ‘C’, …, ‘Z’}, with |W| ≤ |T| ≤ 1,000,000.  

  Output 

  For every test case in the input file, the output should contain a single number, on a single line: the number of occurrences of the word W in the text T.  

  Sample Input 

  3 

  BAPC 

  BAPC 

  AZA 

  AZAZAZA 

  VERDI 

  AVERDXIVYERDIAN 

  Sample Output 

  1 

  3 

  0
KMP的裸题 求模式串在字符串中出现的次数

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
char p[10010];
char s[1000010];
int Next[10010];
void kmp_pre(char* p){
    memset(Next,0,sizeof(Next));
    int i=0;
    int j=-1;
    Next[0]=-1;
    int l=strlen(p);
    while(i<l-1){
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
int kmp_cnt(char *s,char* p){
    kmp_pre(p);
    int sl=strlen(s);
    int pl=strlen(p);
    int i=0;
    int j=0;
    int cnt=0;
    while(i<sl && j<pl){
        if(j==-1 || s[i]==p[j]){
            i++;
            j++;
        }
        else{
            j=Next[j];
        }
        if(j==pl){
            cnt++;
            i--;
            j--;
            j=Next[j];
        }
    }
    return cnt;
}
int main(void){
    int t;
    scanf("%d",&t);
    while(t--){
        scanf("%s",p);
        scanf("%s",s);
        int ans=kmp_cnt(s,p);
        printf("%d\n",ans);
    }
    return 0;
}
```






