# hdu2328——Corporate Identity - westbrook1998的博客 - CSDN博客





2018年08月21日 21:23:49[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：33标签：[字符串																[KMP																[暴力](https://so.csdn.net/so/search/s.do?q=暴力&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)





> 
Beside other services, ACM helps companies to clearly state their “corporate identity”, which includes company logo but also other signs, like trademarks. One of such companies is Internet Building Masters (IBM), which has recently asked ACM for a help with their new identity. IBM do not want to change their existing logos and trademarks completely, because their customers are used to the old ones. Therefore, ACM will only change existing trademarks instead of creating new ones.  

  After several other proposals, it was decided to take all existing trademarks and find the longest common sequence of letters that is contained in all of them. This sequence will be graphically emphasized to form a new logo. Then, the old trademarks may still be used while showing the new identity.  

  Your task is to find such a sequence. 

  Input 

  The input contains several tasks. Each task begins with a line containing a positive integer N, the number of trademarks (2 ≤ N ≤ 4000). The number is followed by N lines, each containing one trademark. Trademarks will be composed only from lowercase letters, the length of each trademark will be at least 1 and at most 200 characters.  

  After the last trademark, the next task begins. The last task is followed by a line containing zero. 

  Output 

  For each task, output a single line containing the longest string contained as a substring in all trademarks. If there are several strings of the same length, print the one that is lexicographically smallest. If there is no such non-empty string, output the words “IDENTITY LOST” instead. 

  Sample Input 

  3 

  aabbaabb 

  abbababb 

  bbbbbabb 

  2 

  xyz 

  abc 

  0 

  Sample Output 

  abb 

  IDENTITY LOST
也是求共同子串的问题，也是比较暴力，就是这次匹配的时候没用find而是用kmp来匹配

代码：

```
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <string>
#include <iostream>
#define _clr(x,a) memset(x,a,sizeof(x));
using namespace std;
const int N=4020;
string s[N];
int n;
int Next[N];
void kmp_pre(string a){
    int l=a.size();
    _clr(Next,0);
    Next[0]=-1;
    int i=0;
    int j=-1;
    while(i<l){
        if(j==-1 || a[i]==a[j]){
            Next[++i]=++j;
        }
        else{
            j=Next[j];
        }
    }
}
int kmp(string s,string p){
    kmp_pre(p);
    int sl=s.size();
    int pl=p.size();
    int i=0,j=0;
    while(i<sl && j<pl){
        if(j==-1 || s[i]==p[j]){
            i++;
            j++;
        }
        else{
            j=Next[j];
        }
    }
    if(j==pl){
        return i-j;
    }
    else{
        return -1;
    }
}
int main(void){
    while(~scanf("%d",&n) && n){
        int _min=0x3f3f3f3f;
        int idx=0;
        for(int i=0;i<n;i++){
            cin>> s[i];
            if(s[i].size()<_min){
                _min=s[i].size();
                idx=i;
            }
        }
        bool flag=false;
        string sub;
        string ans;
        //枚举子串长度
        for(int i=_min;i>0;i--){
            //枚举子串起点
            for(int j=0;j+i<=_min;j++){
                sub=s[idx].substr(j,i);
                //匹配
                int k=0;
                for(;k<n;k++){
                    if(k==idx){
                        continue;
                    }
                    if(kmp(s[k],sub)==-1){
                        break;
                    }
                }
                if(k==n){
                    if(ans.compare("")==0 || ans.compare(sub)>0){
                        ans=sub;
                    }
                    flag=true;
                }
            }
            if(flag){
                break;
            }
        }
        if(flag){
            cout << ans << endl;
        }
        else{
            printf("IDENTITY LOST\n");
        }
    }
    return 0;
}
```](https://so.csdn.net/so/search/s.do?q=KMP&t=blog)](https://so.csdn.net/so/search/s.do?q=字符串&t=blog)




