# sort和qsort函数对结构体的二级排序 - weixin_33985507的博客 - CSDN博客
2012年03月25日 20:14:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8

const int N=100010；
结构体：
struct POINT{
        int  x;
        int  y;
};
POINT ap[N];
在c++中结构体不需要用struct POINT
## 1.qsort
头文件：stdlib
qsort函数调用时格式：
qsort(ap,n,sizeof(ap[0]),cmp);n是比较元素的个数。
cmp函数：
                  int cmp(const void *a,const void *b)
                  {
                           POINT * p1 = (POINT*)a;
                           POINT * p2 = (POINT*)b;
                           if(p1->x != p2->x) return p1->x-p2->x;
                           return p1->y-p2->y;
                  }
## 2.sort
头文件：algorithm
sort函数调用时格式：
sort(ap,ap+n,cmp);n是比较元素的个数。
cmp函数：
int cmp(POINT &a,POINT &b)
{
    if(a.x!=b.x) return a.x>b.x;
    return a.y>b.y;
}
记住cmp函数中返回表达式，一个是用“-”，另一个是用“>”。
