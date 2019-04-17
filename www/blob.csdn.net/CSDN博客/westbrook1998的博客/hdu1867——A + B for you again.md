# hdu1867——A + B for you again - westbrook1998的博客 - CSDN博客





2018年11月04日 11:19:19[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：31标签：[kmp](https://so.csdn.net/so/search/s.do?q=kmp&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









居然要用到kmp…

瞎搞了半天…不对是四个月，今天无意中翻到这题还是不会做，抄了下题解

首先这个kmp的匹配就稍微变形了一点，用kmp(a,b)然后不是输出匹配的位置，也不是输出匹配的数量，而是输入当i==sl的时候，也就是原串匹配完了，这时候pat匹配到那个位置，输入j，这样把a和b分别作为模式串去匹配另一个，看看谁的前缀能连上另一个的后缀的长度更长一些，如果相同再比较字典序
代码：

```
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <string>
using namespace std;
const int N=200050;
int Next[N];
char a[N];
char b[N];
char t[N];
void getNext(char* s){
    memset(Next,0,sizeof(Next));
    int i=0;
    int j=-1;
    Next[0]=-1;
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
int kmp(char *str,char *pat){
    memset(Next,0,sizeof(Next));
    getNext(pat);
    int i=0,j=0;
    int sl=strlen(str);
    int pl=strlen(pat);
    while(i<sl && j<pl){
        if(j==-1 || str[i]==pat[j]){
            i++;
            j++;
        }
        else{
            j=Next[j];
        }
    }
    if(i==sl){
        return j;
    }
    else{
        return -1;
    }
}
int main(void){
    while(~scanf("%s %s",a,b)){
        int t1=kmp(a,b);
        int t2=kmp(b,a);
        if(t1>t2){
            printf("%s",a);
            printf("%s\n",&b[t1]);
        }
        else if(t1<t2){
            printf("%s",b);
            printf("%s\n",&a[t2]);
        }
        else{
            if(strcmp(a,b)<0){
                printf("%s%s\n",a,&b[t1]);
            }
            else{
                printf("%s%s\n",b,&a[t2]);
            }
        }
    }
    return 0;
}
```




