# Codeforces451B——Sort the Array - westbrook1998的博客 - CSDN博客





2018年09月03日 22:33:42[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：79标签：[排序																[数组区间反转](https://so.csdn.net/so/search/s.do?q=数组区间反转&t=blog)](https://so.csdn.net/so/search/s.do?q=排序&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)








> 
Being a programmer, you like arrays a lot. For your birthday, your friends have given you an array a consisting of n distinct integers. 

  Unfortunately, the size of a is too small. You want a bigger array! Your friends agree to give you a bigger array, but only if you are able to answer the following question correctly: is it possible to sort the array a (in increasing order) by reversing exactly one segment of a? See definitions of segment and reversing in the notes. 

  Input 

  The first line of the input contains an integer n (1 ≤ n ≤ 105) — the size of array a. 

  The second line contains n distinct space-separated integers: a[1], a[2], …, a[n] (1 ≤ a[i] ≤ 109). 

  Output 

  Print “yes” or “no” (without quotes), depending on the answer. 

  If your answer is “yes”, then also print two space-separated integers denoting start and end (start must not be greater than end) indices of the segment to be reversed. If there are multiple ways of selecting these indices, print any of them. 

  Examples 

  Input 

  3 

  3 2 1 

  Output 

  yes 

  1 3 

  Input 

  4 

  2 1 3 4 

  Output 

  yes 

  1 2 

  Input 

  4 

  3 1 2 4 

  Output 

  no 

  Input 

  2 

  1 2 

  Output 

  yes 

  1 1 

  Note 

  Sample 1. You can reverse the entire array to get [1, 2, 3], which is sorted. 

  Sample 3. No segment can be reversed such that the array will be sorted. 

  Definitions 

  A segment [l, r] of array a is the sequence a[l], a[l + 1], …, a[r]. 

  If you have an array a of size n and you reverse its segment [l, r], the array will become: 

  a[1], a[2], …, a[l - 2], a[l - 1], a[r], a[r - 1], …, a[l + 1], a[l], a[r + 1], a[r + 2], …, a[n - 1], a[n].
不会做

找到一个比较清晰的题解，就是先从左到右找到不满足排序（也就是后面数比前面小）的这个位置，然后从右到左找到不满足的位置，（这两个位置可能需要交换）作为区间的左右端点，然后反转，然后判断是否排序

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
const int N=1e5+10;
int n;
int a[N];
int main(void){
    scanf("%d",&n);
    for(int i=1;i<=n;i++){
        scanf("%d",&a[i]);
    }
    int l=1,r=1;
    //从左到右找出比前一个数小的数的位置，这个数就是交换区间的右端点，不用管后面的，因为题目限制只转一次
    for(int i=2;i<=n;i++){
        if(a[i]<a[i-1]){
            r=i-1;
            break;
        }
    }
    //同理，从右到左找到一个比后一个数大的数的位置，作为交换区间的左端点
    for(int i=n-1;i>0;i--){
        if(a[i]>a[i+1]){
            l=i+1;
            break;
        }
    }
    //printf("%d %d\n",l,r);
    //反转区间
    if(l>r){
        swap(l,r);
    }
    int i=l;
    int j=r;
    while(i<=j){
        swap(a[i],a[j]);
        i++;
        j--;
    }
    //检查是否排序
    bool flag=false;
    for(int i=1;i<n;i++){
        if(a[i]>a[i+1]){
            flag=true;
            break;
        }
    }
    if(flag){
        printf("no\n");
    }
    else{
        printf("yes\n");
        printf("%d %d\n",l,r);
    }
    return 0;
}
```






