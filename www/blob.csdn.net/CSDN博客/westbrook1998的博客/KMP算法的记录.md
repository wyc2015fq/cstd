# KMP算法的记录 - westbrook1998的博客 - CSDN博客





2018年05月11日 12:26:59[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：48








```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
//Next数组用来保存模式字符串最大的前缀后缀相同的字符个数
//比如Next[4]=2 表示的是从下标为0到下标为4的这一段字符串中
//最大的相同前后缀字符个数是3个（2+1）
//如果字符串“ababaa”
int Next[10010];
void kmp_pre(char x[],int m,int Next[]){
    //有一个递推的关系，通过Next[i]来算出Next[i+1]
    int i,j;
    //初始化，这里默认Next数组从下标1开始，所以Next[0]初始化为-1
    j=Next[0]=-1;
    //i是表示当前判断的字符串下标
    i=0;
    //遍历一次字符串
    while(i<m){
        //需要满足j不等于-1，因为这里j是表示要与x[i]字符比较的字符下标
        //也就是Next[i-1]表示的那个最长前缀的最后一个字符的下标
        //所以要保证有意义（大于（或不等于）-1）
        while(-1!=j && x[i]!=x[j]){
            //当x[i]和x[j]不同时，j要往前回溯
            //因为此时j是作为最大前缀的最后一个字符，j=Next[j]的作用是
            //让j回溯到当前的j是作为最大后缀的情况下的最大前缀位置
            //有点绕，就是当前j的位置是作为最大前缀来和i比较的，把它看成是最大后缀
            //然后找到此时的最大前缀位置再赋给j
            j=Next[j];
        }
        //x[i]和x[j]相同时，Next[i]直接在Next[i-1]的基础上加1
        //但这里并不可以直接Next[++i]=Next[i]++;
        //因为并不是遍历过程中的所有字符都满足x[i]==x[j]
        //因为Next数组下标从1开始，所以这里要先++i
        Next[++i]=++j;
    }
}
//返回模式串在主串中的首位置
int KMP_Search(char pat[],int m,char str[],int n){
    //主串和模式串的指针
    int i=0,j=0;
    //首个匹配子字符串下标
    int idx=-1;
    //预处理
    kmp_pre(pat,m,Next);
    while(i<n){
        while(-1!=j && str[i]!=pat[j]){
            printf("此时i=%d j=%d\n",i,j);
            //不匹配回溯
            //意思就是当前匹配的这一个字符作为后缀，直接回溯到相同前缀位置（Next数组已算出）
            j=Next[j];
            printf("j跳到%d\n",j);
        }
        //匹配或者j==-1即模式串已经无法再回溯
        i++;
        j++;
        printf("匹配，i=%d j=%d\n",i,j);
        //匹配的长度大于等于模式串长度，即成功匹配一个
        if(j>=m){
            idx=j;
            break;
        }
    }
    return idx;
}
//优化版的Next数组预处理（暂时不清楚原理...）
void preKMP(char x[],int m,int kmpNext[]){
    int i,j;
    j=kmpNext[0]=-1;
    i=0;
    while(i<m){
        while(-1!=j && x[i]!=x[j]){
            j=kmpNext[j];
        }
        //从这里开始有所不同
        if(x[++i]==x[++j]){
            kmpNext[i]=kmpNext[j];
        }
        else{
            kmpNext[i]=j;
        }
    }
}
//返回x模式串在y主串中出现的次数
int KMP_Count(char pat[],int m,char str[],int n){
    int i,j;
    int ans=0;
    kmp_pre(pat,m,Next);
    i=j=0;
    while(i<n){
        while(-1!=j && str[i]!=pat[j]){
            j=Next[j];
        }
        i++;
        j++;
        if(j>=m){
            ans++;
            j=Next[j];
        }
    }
    return ans;
}
int main(void){
    char str[]="aabaccabababc";
    char pat[]="abab";
    memset(Next,0,sizeof(Next));
    //kmp_pre(pat,strlen(pat),Next);
    //preKMP(pat,strlen(pat),Next);
    int idx=KMP_Search(pat,strlen(pat),str,strlen(str));
    printf("Next数组\n");
    for(int i=1;i<=strlen(pat);i++){
        printf("%d ",Next[i]);
    }
    printf("\n");
    printf("模式串索引：%d\n",idx);
    int ans=KMP_Count(pat,strlen(pat),str,strlen(str));
    printf("模式串个数：%d\n",ans);
    return 0;
}
```

纯属记给自己看，希望以后还能看懂



