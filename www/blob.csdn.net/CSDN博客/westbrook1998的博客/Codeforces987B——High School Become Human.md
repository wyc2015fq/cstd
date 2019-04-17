# Codeforces987B——High School: Become Human - westbrook1998的博客 - CSDN博客





2018年10月31日 13:10:10[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：34标签：[浮点数精度](https://so.csdn.net/so/search/s.do?q=浮点数精度&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









判断`x^y` 和 `y^x`的大小关系

很简单的题，取个log就行，比较坑的地方是取完log是一个浮点数，会出现精度误差，这样判断大小还行，判断相等就出问题了，虽然在本地a-b<0.0000001能过，提交上去就不行了，所以直接在输入两个数的时候判断就行了

代码：

```
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
ll x,y;
int main(void){
    scanf("%lld%lld",&x,&y);
    if(x==y){
        printf("=\n");
        return 0;
    }
    double a=y*log(x);
    double b=x*log(y);
    if(a<b){
        printf("<\n");
    }
    else if(a>b){
        printf(">\n");
    }
    else{
        printf("=\n");
    }
    return 0;
}
```



