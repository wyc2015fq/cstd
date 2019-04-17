# Oulipo（POJ-3461） - Alex_McAvoy的博客 - CSDN博客





2019年02月27日 16:45:05[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：58








> 
# Problem Description

The French author Georges Perec (1936–1982) once wrote a book, La disparition, without the letter 'e'. He was a member of the Oulipo group. A quote from the book: 

Tout avait Pair normal, mais tout s’affirmait faux. Tout avait Fair normal, d’abord, puis surgissait l’inhumain, l’affolant. Il aurait voulu savoir où s’articulait l’association qui l’unissait au roman : stir son tapis, assaillant à tout instant son imagination, l’intuition d’un tabou, la vision d’un mal obscur, d’un quoi vacant, d’un non-dit : la vision, l’avision d’un oubli commandant tout, où s’abolissait la raison : tout avait l’air normal mais… 

Perec would probably have scored high (or rather, low) in the following contest. People are asked to write a perhaps even meaningful text on some subject with as few occurrences of a given “word” as possible. Our task is to provide the jury with a program that counts these occurrences, in order to obtain a ranking of the competitors. These competitors often write very long texts with nonsense meaning; a sequence of 500,000 consecutive 'T's is not unusual. And they never use spaces. 

So we want to quickly find out how often a word, i.e., a given string, occurs in a text. More formally: given the alphabet {'A', 'B', 'C', …, 'Z'} and two finite strings over that alphabet, a word W and a text T, count the number of occurrences of W in T. All the consecutive characters of W must exactly match consecutive characters of T. Occurrences may overlap. 

# **Input**

The first line of the input file contains a single number: the number of test cases to follow. Each test case has the following format: 

One line with the word W, a string over {'A', 'B', 'C', …, 'Z'}, with 1 ≤ |W| ≤ 10,000 (here |W| denotes the length of the string W). 

One line with the text T, a string over {'A', 'B', 'C', …, 'Z'}, with |W| ≤ |T| ≤ 1,000,000. 

# Output

For every test case in the input file, the output should contain a single number, on a single line: the number of occurrences of the word W in the text T. 

# Sample Input

**3BAPCBAPCAZAAZAZAZAVERDIAVERDXIVYERDIAN**

# Sample Output

**130**


题意：t 组数据，每组给出两个字符串，第一个字符串是模式串，第二个字符串是文本串，求匹配串再目标串中出现次数，若没有则输出 0

思路：求文本串中模式串出现次数，允许使用重复的字符，此外注意模式串和文本串的位置即可

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
#define N 1000001
#define LL long long
const int MOD=20091226;
const int dx[]= {-1,1,0,0};
const int dy[]= {0,0,-1,1};
using namespace std;

int Next[N];
char str1[N],str2[N];
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
int KMP(char t[],char p[]) {
    int tLen=strlen(t);
    int pLen=strlen(p);
    getNext(p);

    int res=0;
    int j=0;
    for(int i=0;i<tLen;i++){
        while(j&&p[j]!=t[i]){
            j=Next[j];
        }
        if(p[j]==t[i]){
            j++;
        }
        if(j==pLen){
            res++;
        }
    }
    return res;
}

int main(){
    int t;
    scanf("%d",&t);

    while(t--){
        scanf("%s",str1);
        scanf("%s",str2);

        printf("%d\n",KMP(str2,str1));
    }
    return 0;
}
```





