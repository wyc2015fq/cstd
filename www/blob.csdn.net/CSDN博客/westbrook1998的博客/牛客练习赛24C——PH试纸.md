# 牛客练习赛24C——PH试纸 - westbrook1998的博客 - CSDN博客





2018年08月10日 21:22:06[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：63








> 
链接：[https://www.nowcoder.com/acm/contest/157/c](https://www.nowcoder.com/acm/contest/157/c)

  来源：牛客网 

  题目描述  

      PH试纸，是一种检测酸碱度的试纸，试纸红色为酸性，蓝色为碱性。 

      HtBest有一个PH试纸，试纸被分成了n段，每一段都可以被染色成红色或者蓝色，WHZ在试纸的每一段上都染为一种颜色，HtBest有m个询问，对于每个询问，Ta想知道某种颜色第qi次在什么地方出现。 

  输入描述: 

  第一行有两个正整数n，m。 

  第二行有n个字母(‘R’或’B’)，每个第i个字母表示PH试纸第i段的颜色。 

  接下来m行，第i行有一个大写字母 ci(‘R’或’B’)和一个正整数qi ，用空格隔开，表示查询颜色ci 第qi 次出现的位置。 

  输出描述: 

  共m行，第i行一个整数，表示查询结果，若颜色ci出现次数少于qi次，则输出-1，否则输出颜色qi第ci次出现的位置。 

  示例1 

  输入 

  复制 

  2 2 

  RB 

  R 1 

  B 1 

  输出 

  复制 

  1 

  2 

  示例2 

  输入 

  复制 

  2 2 

  BB 

  R 1 

  B 2 

  输出 

  复制 

  -1 

  2 

  示例3 

  输入 

  复制 

  3 3 

  BRB 

  B 1 

  B 2 

  R 1 

  输出 

  复制 

  1 

  3 

  2 

  备注: 

  对于100%的测试数据： 

  1 ≤ n, m ≤ 1000000 

  所有输入数据不超过1000000。 

  数据量较大，注意使用更快的输入输出方式。
这题暴力的做法应该就是存数组，然后每一次查询都去遍历一次数组 

因此，这题不可能这么简单，所以要在输入的时候就用两个数组存第`i` 个`R` 或 `B` 的位置，查询的时候直接O(1) 读取

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int MAXN=1000050;
char s[MAXN];
int r[MAXN];
int b[MAXN];
int main(void){
    int n,m;
    scanf("%d%d",&n,&m);
    scanf("%s",s);
    memset(r,-1,sizeof(r));
    memset(b,-1,sizeof(b));
    int ri=1;
    int bi=1;
    for(int i=0;i<n;i++){
        if(s[i]=='R'){
            r[ri++]=i+1;
        }
        else if(s[i]=='B'){
            b[bi++]=i+1;
        }
    }
    char t[10];
    int q;
    while(m--){
        scanf("%s %d",t,&q);
        if(t[0]=='R'){
            printf("%d\n",r[q]);
        }
        else{
            printf("%d\n",b[q]);
        }
    }
    return 0;
}
```





