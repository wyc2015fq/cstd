# 第十四届华中科技大学程序设计竞赛F——Sorting Trees - westbrook1998的博客 - CSDN博客





2018年04月29日 22:57:54[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：86








这题一来题意没理解好，二来还看错，三来就算看对了也超时了…

题面：

> 
It’s universally acknowledged that there’re innumerable trees in the campus of HUST. 

  One day the tree manager of HUST asked you to sort N trees increasing by their heights ai. Because you were a warm-hearted and professional programmer, you immediately wrote the Bubble Sort to finish the task. However, you were so tired that made a huge mistake:


```matlab
for(int i=1;i<n;i++)
    for(int j=1;j<=n-i;j++)
         if(a[j+k]<a[j])swap(a[j+k],a[j]);
```

> 
Usually k=1 is the normal Bubble Sort. But you didn’t type it correctly. When you now realize it, the trees have been replanted. In order to fix them you must find the first place that goes wrong, compared with the correctly sorted tree sequence. 

  If every trees are in their correct places then print -1. And you should know k can be equal to 1 which means you didn’t make any mistake at all. Also you don’t need to consider the part j+k beyond n.


题目是说将一个数组通过错误的k冒泡排序和正确的排序进行对比，输出第一个出错的位置

主要的问题是在这个错误的k冒泡排序这里，如果不优化直接两个循环居然超时了…

代码：

```cpp
#include <algorithm>
#include <cstdio>
using namespace std;
int a[100005], b[100005];
int main(){
    int n, k;
    scanf("%d%d", &n, &k);
    for (int s = 0; s < n; s++){
        //输入原排序的数组并同时复制给另一个数组
        scanf("%d", &a[s]);
        b[s] = a[s];
    }
    //特判，没有错误
    if (k == 1){
        printf("-1\n");
        return 0;
    }
    //错误的冒泡排序
    //优化成一层循环，因为二层循环所比较的两个数其实在第一层循环已经比较过了！（小数组手动模拟一下即可退出）
    for (int s = 0; s + k < n; s++){
        if (a[s + k] < a[s]){
            swap(a[s], a[s + k]);
        }
    }
    //正确的排序
    sort(b, b + n);
    //记录第一个出错位置
    int ans = -1;
    for (int s = 0; s < n; s++){
        if (a[s] != b[s]){
            ans = s+1;
            break;
        }
    }
    printf("%d\n",ans);
    return 0;
}
```



