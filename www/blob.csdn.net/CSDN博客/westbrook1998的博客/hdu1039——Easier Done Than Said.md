# hdu1039——Easier Done Than Said? - westbrook1998的博客 - CSDN博客





2018年05月09日 21:14:10[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：43标签：[hdu																[字符串																[简单题](https://so.csdn.net/so/search/s.do?q=简单题&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)





> 
Password security is a tricky thing. Users prefer simple passwords that are easy to remember (like buddy), but such passwords are often insecure. Some sites use random computer-generated passwords (like xvtpzyo), but users have a hard time remembering them and sometimes leave them written on notes stuck to their computer. One potential solution is to generate “pronounceable” passwords that are relatively secure but still easy to remember.  

  FnordCom is developing such a password generator. You work in the quality control department, and it’s your job to test the generator and make sure that the passwords are acceptable. To be acceptable, a password must satisfy these three rules:  

  It must contain at least one vowel.  

  It cannot contain three consecutive vowels or three consecutive consonants.  

  It cannot contain two consecutive occurrences of the same letter, except for ‘ee’ or ‘oo’.  

  (For the purposes of this problem, the vowels are ‘a’, ‘e’, ‘i’, ‘o’, and ‘u’; all other letters are consonants.) Note that these rules are not perfect; there are many common/pronounceable words that are not acceptable.  

  Input 

  The input consists of one or more potential passwords, one per line, followed by a line containing only the word ‘end’ that signals the end of the file. Each password is at least one and at most twenty letters long and consists only of lowercase letters.  

  Output 

  For each password, output whether or not it is acceptable, using the precise format shown in the example.  

  Sample Input 

  a 

  tv 

  ptoui 

  bontres 

  zoggax 

  wiinq 

  eep 

  houctuh 

  end 

  Sample Output 
`<a>` is acceptable. 
`<tv>` is not acceptable. 
`<ptoui>` is not acceptable. 
`<bontres>` is not acceptable. 
`<zoggax>` is not acceptable. 
`<wiinq>` is not acceptable. 
`<eep>` is acceptable. 
`<houctuh>` is acceptable.
也是一道比较简单的字符串处理的题目，应该说是一道水题，仍然是遍历一次字符串然后判断处理，WA了三发都是题意没看清楚，当输入end时应该结束，而且不允许两个连续相同字母是除了“ee” “oo”之外，看成了举个例子…不应该

代码：

```cpp
#include <cstdio>
#include <cstring>
using namespace std;
char pass[25];
char vowels[]={'a','e','i','o','u'};
bool isvow(char c){
    for(int i=0;i<5;i++){
        if(c==vowels[i]){
            return true;
        }
    }
    return false;
}
int main(void){
    while(scanf("%s",pass)!=EOF){
        if(pass[0]=='e' && pass[1]=='n' && pass[2]=='d'){
            break;
        }
        bool ac=true;
        int len=strlen(pass);
        int vow=0;
        for(int i=0;i<len;i++){
            if(i<len-1 && pass[i]==pass[i+1] && pass[i]!='e' && pass[i]!='o'){
                ac=false;
                break;
            }
            if(isvow(pass[i])){
                vow++;
                if(i<len-2 && isvow(pass[i+1]) && isvow(pass[i+2])){
                    ac=false;
                    break;
                }
            }
            else{
                if(i<len-2 && !isvow(pass[i+1]) && !isvow(pass[i+2])){
                    ac=false;
                    break;
                }
            }
        }
        if(vow==0){
            ac=false;
        }
        if(ac){
            printf("<%s> is acceptable.\n",pass);
        }
        else{
            printf("<%s> is not acceptable.\n",pass);
        }
    }
    return 0;
}
```](https://so.csdn.net/so/search/s.do?q=字符串&t=blog)](https://so.csdn.net/so/search/s.do?q=hdu&t=blog)




