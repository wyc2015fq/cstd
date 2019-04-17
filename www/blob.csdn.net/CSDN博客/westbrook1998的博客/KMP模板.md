# KMP模板 - westbrook1998的博客 - CSDN博客





2018年06月24日 16:31:38[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：58标签：[KMP																[字符串](https://so.csdn.net/so/search/s.do?q=字符串&t=blog)](https://so.csdn.net/so/search/s.do?q=KMP&t=blog)
个人分类：[杂七杂八](https://blog.csdn.net/westbrook1998/article/category/7458454)








拖了很久对这个KMP还是比较模糊的 虽然可以背起来 但是有些细节还是不知为何 但确实是很巧妙

代码+注释：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
//next数组和最大前缀后缀数组有所不同
//next数组相当于将最大前缀后缀和整体右移一位，并将第一位补为-1
//也就是相当于该字符前(不包括该字符)的最大相同前缀后缀长度 所以第一位设为-1
int Next[100];
//kmp预处理 得到Next数组
void kmp_pre(char* pat){
    int l=strlen(pat);
    //第一个字符的next值规定为-1
    Next[0]=-1;
    //i为当前字符的下标 且前i个字符的最大相同前缀后缀长度其实是i+1的next值 所以i<l-1
    int i=0;
    //j临时保存最大相同前缀后缀长度
    int j=-1;
    while(i<l-1){
        //跟匹配的代码类似 思路也是类似
        //如果pat[i]==pat[j]  比如abcdabchh 当i到5也就是b时，next[6]=2
        //而此时c==c 即pat[i]==pat[j] 那么i j都可以后移一位，然后next[i]=next[j](也就是j)+1
        if(j==-1 || pat[i]==pat[j]){
            i++;
            j++;
            Next[i]=j;
        }
        //如果不匹配，那么j(此时代表这个前缀的最后一个字符)，就要回到前面再判断是否和pat[i]相等，也就是尝试减少这个next值
        else{
            j=Next[j];
        }
    }
}
//kmp匹配 返回第一个匹配首位置
int kmp_search(char* str,char* pat){
    int i=0;
    int j=0;
    int sl=strlen(str);
    int pl=strlen(pat);
    while(i<sl && j<pl){
        //为什么要判断j==-1呢？ 因为我们规定pat第一个字符的next值为-1
        //其实这里j=-1也是失配的一种情况！！！，但是因为不存在next[-1]，而且这种情况也是只向右移动一位，所以归在匹配这里
        if(j==-1 || str[i]==pat[j]){
            i++;
            j++;
        }
        //即j!=-1 && str[i]!=pat[j] 失配
        //失配时 pat向右移动的位数=已匹配的字符数-失配字符前面字符串的最大相同前缀后缀长度
        else{
            //j往前走 其实相当于整个pat串往后移！！！ 移动使得当前j字符前面的字符串的最大相同前缀来到最大相同后缀这个位置 
            j=Next[j];
        }
    }
    //如果j到pl了 说明循环的退出是因为j 即pat得到匹配 匹配的首位置就是当前匹配的字符位置i减去pat的长度 
    if(j==pl){
        return i-j;
    }
    //否则 返回-1 无法匹配pat
    else{
        return -1;
    }
}
//无注释版
void kmp_pre_no_com(char* pat){
    int l=strlen(pat);
    int i=0,j=-1;
    Next[0]=-1;
    while(i<l-1){
        if(j==-1 || pat[i]==pat[j]){
            i++;
            j++;
            Next[i]=j;
        }
        else{
            j=Next[j];
        }
    }
}
void kmp_search_no_com(char* str,char* pat){
    memset(Next,0,sizeof(Next));
    kmp_pre_no_com(pat);
    int i=0,j=0;
    int slen=strlen(str);
    int plen=strlen(pat);
    while(i<slen && j<plen){
        if(j==-1 || str[i]==pat[j]){
            i++;
            j++;
        }
        else{
            j=Next[j];
        }
    }
    if(j==plen){
        return i-j;
    }
    else{
        return -1;
    }
}
int main(void){
    char pat[]="abcdabd";
    kmp_pre(pat);
    for(int i=0;i<10;i++){
        printf("%d ",Next[i]);
    }
    printf("\n");
    return 0;
}
```




