# hdu2037——今年暑假不AC - westbrook1998的博客 - CSDN博客





2018年04月11日 18:55:43[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：52








### 输入描述:

> 
输入数据包含多个测试实例，每个测试实例的第一行只有一个整数n(n<=100)，表示你喜欢看的节目的总数，然后是n行数据，每行包括两个数据Ti_s,Ti_e (1<=i<=n)，分别表示第i个节目的开始和结束时间，为了简化问题，每个时间都用一个正整数表示。n=0表示输入结束，不做处理。


### 输出描述:

> 
对于每个测试实例，输出能完整看到的电视节目的个数，每个测试实例的输出占一行。


### 代码：

```cpp
#include <stdio.h>
#include <stdlib.h>
#define max 100
//定义节目结构体数组，结构体存储开始时间和结束时间
struct node{
    int s;
    int e;
}res[max];
//定义一个比较函数用于排序
int cmp(const void* a,const void* b){
    struct node* x=(struct node*)a;
    struct node* y=(struct node*)b;
    return x->e-y->e;
}
int main(void){
    int n;
    //套路输入
    while(scanf("%d",&n)!=EOF){
        if(n==0){
            break;
        }
        int i;
        for(i=0;i<n;i++){
            scanf("%d %d",&res[i].s,&res[i].e);
        }
        //调用stdlib.h里的qsort对数组的结束时间进行排序
        qsort(res,n,sizeof(res[0]),cmp);
        //定义变量记录节目数和上一个节目结束时间
        int count=0;
        int lastend=-1;
        //循环，从第一个节目开始判断，如果该节目开始时间大于上一个节目结束时间，即说明可以看这个节目，那么count加一，否则i加一，判断下一个节目（因为节目已经按结束时间排序好）
        for(i=0;i<n;i++){
            if(res[i].s>=lastend){
                count++;
                lastend=res[i].e;
            }
        }
        printf("%d\n",count);
    }
    return 0;
}
```

学的一点点c快忘光了/(ㄒoㄒ)/~~



