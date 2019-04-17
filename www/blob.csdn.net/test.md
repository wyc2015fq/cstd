# 2018年全国多校算法寒假训练营练习比赛（第二场）A	吐泡泡 - westbrook1998的博客 - CSDN博客





2018年05月04日 20:40:45[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：63标签：[算法																[模拟																[字符串](https://so.csdn.net/so/search/s.do?q=字符串&t=blog)
个人分类：[ACM练习赛																[牛客网](https://blog.csdn.net/westbrook1998/article/category/7740908)](https://blog.csdn.net/westbrook1998/article/category/7635377)





算是字符串的处理，踩了一些坑

> 
题目描述  

  小鱼儿吐泡泡，嘟嘟嘟冒出来。小鱼儿会吐出两种泡泡：大泡泡”O”，小泡泡”o”。 

  两个相邻的小泡泡会融成一个大泡泡,两个相邻的大泡泡会爆掉。 

  （是的你没看错，小气泡和大气泡不会产生任何变化的，原因我也不知道。） 

  例如:ooOOoooO经过一段时间以后会变成oO。
> 
输入描述: 

  数据有多组，处理到文件结束。 

  每组输入包含一行仅有’O’与’o’组成的字符串。
> 
输出描述: 

  每组输出仅包含一行，输出一行字符串代表小鱼儿吐出的泡泡经过融合以后所剩余的泡泡。


> 
示例1 

  输入 

  ooOOoooO 

  输出 

  oO
> 
说明 

  自左到右进行合并


> 
备注: 

  对于100%的数据， 

  字符串的长度不超过100。
代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <string>
#include <cstring>
using namespace std;
char s[101];
void printS(char* s,int len){
    for(int i=0;i<len;i++){
        printf("%c",s[i]);
    }
    printf("\n");
}
int main(void){
    while(scanf("%s",s)!=EOF){
        for(int i=0;i<strlen(s);i++){
            if(s[i]=='o'){
                int j=i;
                while(s[j+1]=='x'){
                    j++;
                }
                if(s[j+1]=='o'){
                    s[i]='x';
                    s[j+1]='O';
                    i=-1;
                    //printS(s,strlen(s));
                    continue;
                }
            }
            if(s[i]=='O'){
                int j=i;
                while(s[j+1]=='x'){
                    j++;
                }
                if(s[j+1]=='O'){
                    s[i]='x';
                    s[j+1]='x';
                    i=-1;
                    //printS(s,strlen(s));
                    continue;
                }
            }
            else{
                continue;
            }
        }
        for(int i=0;i<strlen(s);i++){
            if(s[i]!='x'){
                printf("%c",s[i]);
            }
        }
        printf("\n");
    }
    return 0;
}
```

思路其实就是按泡泡的合并和爆炸模拟一遍，因为字符串也不大（100），然后合并或者爆炸之后剩下的坑就用一个x字符代替，不用去移动数组，然后每次出现操作之后把i重置再重新遍历一遍，这里应该可以不用重置到开头，没再细想了，然后最后就是说一下坑，，不是o和0 是o和大O…我怎么一看题就老是觉得它是0呢…](https://so.csdn.net/so/search/s.do?q=模拟&t=blog)](https://so.csdn.net/so/search/s.do?q=算法&t=blog)




