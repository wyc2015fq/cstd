# Codeforces1008A——Romaji - westbrook1998的博客 - CSDN博客





2018年08月19日 19:07:19[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：45








> 
Vitya has just started learning Berlanese language. It is known that Berlanese uses the Latin alphabet. Vowel letters are “a”, “o”, “u”, “i”, and “e”. Other letters are consonant. 

  In Berlanese, there has to be a vowel after every consonant, but there can be any letter after any vowel. The only exception is a consonant “n”; after this letter, there can be any letter (not only a vowel) or there can be no letter at all. For example, the words “harakiri”, “yupie”, “man”, and “nbo” are Berlanese while the words “horse”, “king”, “my”, and “nz” are not. 

  Help Vitya find out if a word s is Berlanese. 

  Input 

  The first line of the input contains the string s consisting of |s| (1≤|s|≤100) lowercase Latin letters. 

  Output 

  Print “YES” (without quotes) if there is a vowel after every consonant except “n”, otherwise print “NO”. 

  You can print each letter in any case (upper or lower). 

  Examples 

  Input 

  sumimasen 

  Output 

  YES 

  Input 

  ninja 

  Output 

  YES 

  Input 

  codeforces 

  Output 

  NO 

  Note 

  In the first and second samples, a vowel goes after each consonant except “n”, so the word is Berlanese. 

  In the third sample, the consonant “c” goes after the consonant “r”, and the consonant “s” stands on the end, so the word is not Berlanese.
A题 暴力判断一下就行 

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
int main(void){
    char s[105];
    scanf("%s",s);
    int len=strlen(s);
    int i=0;
    bool flag=false;
    for(;i<len-1;i++){
        if(s[i]!='a' && s[i]!='e' && s[i]!='i' && s[i]!='o' && s[i]!='u'){
            if(s[i]!='n'){
                if(s[i+1]!='a' && s[i+1]!='e' && s[i+1]!='i' && s[i+1]!='o' && s[i+1]!='u'){
                    printf("NO\n");
                    flag=true;
                    break;
                }
            }
        }
    }
    if(!flag && s[len-1]!='a' && s[len-1]!='e' && s[len-1]!='i' && s[len-1]!='o' && s[len-1]!='u' && s[len-1]!='n'){
        printf("NO\n");
        return 0;
    }
    if(i==len-1){
        printf("YES\n");
    }
    return 0;
}
```





