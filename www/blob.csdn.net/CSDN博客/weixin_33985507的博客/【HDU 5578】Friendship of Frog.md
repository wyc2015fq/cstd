# 【HDU 5578】Friendship of Frog - weixin_33985507的博客 - CSDN博客
2016年02月18日 08:49:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
[题](http://acm.hust.edu.cn/vjudge/contest/view.action?cid=105080#problem/E)
## 题意
　　求相同字母最近距离
## 分析
　　用数组保存各个字母最后出现的位置，维护最小距离。
## 代码
```
#include <cstdio>
int c[30],n,p,a,minl;
char ch;
int main(){
    scanf("%d ",&n);
    for(int i=1;i<=n;i++){
        for(int j=0;j<27;j++)c[j]=0;
        p=1;minl=2000;
        while((ch=getchar())!='\n'){
            a=ch-'a';
            if(c[a]&&p-c[a]<minl)minl=p-c[a];
            c[a]=p;
            p++;
        }
        printf("Case #%d: ",i);
        if(minl<2000)printf("%d\n",minl);
        else printf("-1\n");
    }
    return 0;
}
```
