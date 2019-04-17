# hdu1433——Simply Syntax - westbrook1998的博客 - CSDN博客





2018年07月25日 15:49:31[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：66








> 
In the land of Hedonia the official language is Hedonian. A Hedonian professor had noticed that many of her students still did not master the syntax of Hedonian well. Tired of correcting the many syntactical mistakes, she decided to challenge the students and asked them to write a program that could check the syntactical correctness of any sentence they wrote. Similar to the nature of Hedonians, the syntax of Hedonian is also pleasantly simple. Here are the rules:  

  0. The only characters in the language are the characters p through z and N, C, D, E, and I.  

  1. Every character from p through z is a correct sentence.  

  2. If s is a correct sentence, then so is Ns.  

  3. If s and t are correct sentences, then so are Cst, Dst, Est and Ist.  

  4. Rules 0. to 3. are the only rules to determine the syntactical correctness of a sentence.  

  You are asked to write a program that checks if sentences satisfy the syntax rules given in Rule 0. - Rule 4.  

  Input 

  The input consists of a number of sentences consisting only of characters p through z and N, C, D, E, and I. Each sentence is ended by a new-line character. The collection of sentences is terminated by the end-of-file character. If necessary, you may assume that each sentence has at most 256 characters and at least 1 character.  

  Output 

  The output consists of the answers YES for each well-formed sentence and NO for each not-well-formed sentence. The answers are given in the same order as the sentences. Each answer is followed by a new-line character, and the list of answers is followed by an end-of-file character.  

  Sample Input 

  Cp 

  Isz 

  NIsz 

  Cqpq 

  Sample Output 

  NO 

  YES 

  YES 

  NO
递归 判断 好久没自己独立做题了哈哈哈

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
char s[300];
bool check(int l,int r){
    // for(int i=l;i<=r;i++){
    //     printf("%c",s[i]);
    // }
    // printf("\n");
    if(l==r){
        if(s[l]>='p' && s[l]<='z'){
            return true;
        }
        else{
            return false;
        }
    }
    else{
        if(s[l]=='N'){
            if(check(l+1,r)){
                return true;
            }
            else{
                return false;
            }
        }
        else if(s[l]=='C' || s[l]=='D' || s[l]=='E' ||s[l]=='I'){
            for(int k=l+1;k<=r;k++){
                if(l+1==r){
                    return false;
                }
                else{
                    if(check(l+1,k) && check(k+1,r)){
                        return true;
                    }
                }
            }
            return false;
        }
        else{
            return false;
        }
    }
}
int main(void){
    while(~scanf("%s",s)){
        if(check(0,strlen(s)-1)){
            printf("YES\n");
        }
        else{
            printf("NO\n");
        }
    }
    return 0;
}
```





