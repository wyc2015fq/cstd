# hdu1755——A Number Puzzle - westbrook1998的博客 - CSDN博客





2018年07月30日 23:07:32[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：33标签：[暴力																[全排列](https://so.csdn.net/so/search/s.do?q=全排列&t=blog)](https://so.csdn.net/so/search/s.do?q=暴力&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
Lele 最近上课的时候都很无聊，所以他发明了一个数字游戏来打发时间。  

  这个游戏是这样的，首先，他拿出几张纸片，分别写上0到9之间的任意数字(可重复写某个数字)，然后，他叫同学随便写两个数字X和K。Lele要做的事情就是重新拼这些纸牌，组成数字 T ，并且 T + X 是 K 的正整数倍。  

  有时候，当纸片很多的时候，Lele经常不能在一节课之内拼出来，但是他又想知道答案，所以，他想请你帮忙写一个程序来计算答案。  

  Input 

  本题目包含多组测试数据，请处理到文件结束。  

  每组数据第一行包含两个整数 N和M(0
```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
int c[10];
int num[1000050];
int main(void){
    int n,m;
    while(~scanf("%d%d",&n,&m)){
        memset(c,'\0',sizeof(c));
        memset(num,0,sizeof(num));
        for(int i=0;i<n;i++){
            scanf("%d",&c[i]);
        }
        int t=0;
        sort(c,c+n);
        do{
            int sum=0;
            if(c[0]==0){
                continue;
            }
            for(int i=0;i<n;i++){
                sum=10*sum+c[i];
            }
            num[t++]=sum;
        }while(next_permutation(c,c+n));
        int x,k;
        while(m--){
            scanf("%d%d",&x,&k);
            int i;
            for(i=0;i<t;i++){
                if((num[i]+x)%k==0){
                    printf("%d\n",num[i]);
                    break;
                }
            }
            if(i==t){
                printf("None\n");
            }
        }
    }
    return 0;
}
```






