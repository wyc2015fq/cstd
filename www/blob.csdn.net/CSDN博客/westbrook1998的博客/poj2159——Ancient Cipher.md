# poj2159——Ancient Cipher - westbrook1998的博客 - CSDN博客





2018年05月29日 13:43:56[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：56








> 
Ancient Roman empire had a strong government system with various departments, including a secret service department. Important documents were sent between provinces and the capital in encrypted form to prevent eavesdropping. The most popular ciphers in those times were so called substitution cipher and permutation cipher.  

  Substitution cipher changes all occurrences of each letter to some other letter. Substitutes for all letters must be different. For some letters substitute letter may coincide with the original letter. For example, applying substitution cipher that changes all letters from ‘A’ to ‘Y’ to the next ones in the alphabet, and changes ‘Z’ to ‘A’, to the message “VICTORIOUS” one gets the message “WJDUPSJPVT”.  

  Permutation cipher applies some permutation to the letters of the message. For example, applying the permutation <2, 1, 5, 4, 3, 7, 6, 10, 9, 8> to the message “VICTORIOUS” one gets the message “IVOTCIRSUO”.  

  It was quickly noticed that being applied separately, both substitution cipher and permutation cipher were rather weak. But when being combined, they were strong enough for those times. Thus, the most important messages were first encrypted using substitution cipher, and then the result was encrypted using permutation cipher. Encrypting the message “VICTORIOUS” with the combination of the ciphers described above one gets the message “JWPUDJSTVP”.  

  Archeologists have recently found the message engraved on a stone plate. At the first glance it seemed completely meaningless, so it was suggested that the message was encrypted with some substitution and permutation ciphers. They have conjectured the possible text of the original message that was encrypted, and now they want to check their conjecture. They need a computer program to do it, so you have to write one. 

  Input 

  Input contains two lines. The first line contains the message engraved on the plate. Before encrypting, all spaces and punctuation marks were removed, so the encrypted message contains only capital letters of the English alphabet. The second line contains the original message that is conjectured to be encrypted in the message on the first line. It also contains only capital letters of the English alphabet.  

  The lengths of both lines of the input are equal and do not exceed 100. 

  Output 

  Output “YES” if the message on the first line of the input file could be the result of encrypting the message on the second line, or “NO” in the other case. 

  Sample Input 

  JWPUDJSTVP 

  VICTORIOUS 

  Sample Output 

  YES
读题真的是硬伤额…刚开始以为是进行两次密码代换…

代码：

```cpp
#include <algorithm>
#include <cstdio>
#include <cstring>
using namespace std;
const int MAXN=105;
char s[MAXN];
char p[MAXN];
int sn[26];
int pn[26];
bool cmp(int a,int b){
    return a>b;
}
int main(void){
    while(~scanf("%s",s)){
        scanf("%s",p);
        int n=strlen(s);
        for(int i=0;i<n;i++){
            sn[s[i]-'A']++;
        }
        for(int i=0;i<n;i++){
            pn[p[i]-'A']++;
        }
        sort(sn,sn+26,cmp);
        sort(pn,pn+26,cmp);
        int f=1;
        for(int i=0;i<26;i++){
            if(sn[i]!=pn[i]){
                f=0;
                break;
            }
        }
        if(f){
            printf("YES\n");
        }
        else{
            printf("NO\n");
        }
    }
    return 0;
}
```





