# hdu2045——不容易系列之(3)—— LELE的RPG难题 - westbrook1998的博客 - CSDN博客





2018年07月07日 19:26:54[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：31标签：[递推](https://so.csdn.net/so/search/s.do?q=递推&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
人称“AC女之杀手”的超级偶像LELE最近忽然玩起了深沉，这可急坏了众多“Cole”（LELE的粉丝,即”可乐”）,经过多方打探，某资深Cole终于知道了原因，原来，LELE最近研究起了著名的RPG难题:  

  有排成一行的ｎ个方格，用红(Red)、粉(Pink)、绿(Green)三色涂每个格子，每格涂一色，要求任何相邻的方格不能同色，且首尾两格也不同色．求全部的满足要求的涂法.  

  以上就是著名的RPG难题.  

  如果你是Cole,我想你一定会想尽办法帮助LELE解决这个问题的;如果不是,看在众多漂亮的痛不欲生的Cole女的面子上,你也不会袖手旁观吧?  

  Input 

  输入数据包含多个测试实例,每个测试实例占一行,由一个整数N组成，(0
```cpp
#include <cstdio>
#include <algorithm>
using namespace std;
long long a[55];
void Init(){
    a[1]=3;
    a[2]=6;
    a[3]=6;
    for(int i=4;i<=50;i++){
        //递推公式 考虑当第n个涂色时，前n-1个满足要求的情况和前n-2个满足要求的情况
        a[i]=a[i-1]+2*a[i-2];
    }
}
int main(void){
    Init();
    int n;
    while(~scanf("%d",&n)){
        printf("%lld\n",a[n]);
    }
    return 0;
}
```





