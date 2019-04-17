# Codeforces1064A——Make a triangle! - westbrook1998的博客 - CSDN博客





2018年10月14日 20:19:19[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：53标签：[水题](https://so.csdn.net/so/search/s.do?q=水题&t=blog)
个人分类：[ACM练习题																[codeforce](https://blog.csdn.net/westbrook1998/article/category/7608645)](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
给三条边，判断需要多少次操作才能使得构成三角形，每一次操作为将每条边加1


对边排序一下，然后因为两边和（短边）大于第三边，所以判断一下就可以了

代码：

```
#include <cstdio>
#include <algorithm>
using namespace std;
int a[3];
int main(void){
    scanf("%d%d%d",&a[0],&a[1],&a[2]);
    sort(a,a+3);
    if(a[2]<a[0]+a[1]){
        printf("0\n");
    }
    else{
        printf("%d\n",a[2]-(a[0]+a[1])+1);
    }
    return 0;
}
```




