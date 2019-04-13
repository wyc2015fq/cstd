# bfprt算法求最小的k个数

2016年01月30日 11:59:27 [l-jobs](https://me.csdn.net/qq_26010491) 阅读数：775



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/qq_26010491/article/details/50610601

bfprt算法是一种经典的线性时间内求最小的第k个数或者最小的k个数的算法。

比如我们要知道销量是前几名商品或者是浏览量最多的前几个网站，我们不需要排序，利用bfprt算法就可以完成。

bfprt算法又称快速选择算法，其思想是利用快速排序的划分来确定位置。此算法经典而优雅，值得我们学习。

## 步骤

​        1.将输入的元素划分为5个元素一组，至多有一组不足5个元素。

​        2.利用插入排序寻找每组5个元素的中位数，然后再递归找所有中位数的中位数。

​        3.利用快排的划分方法，以第二步所找的中位数为哨兵划分。返回最后划分的位置i，

​          如果i+1 = k,则返回i,

​          如果i+1<k,说明找到的数小了，在i+1到数组末递归查找k-i-1小的数，

​          同理，i+1>k,在数组开始到i-1的部分递归查找第i小的元素。

## c语言代码



```cpp
#include <stdio.h>
#define N 8
 
void swap(int* a, int* b) 
{
    int t = *a;
    *a = *b;
    *b = t;
}
 
//插入排序
void InsertSort(int a[], int l, int r)
{
    for(int i = l + 1; i <= r; i++)
    {
        if(a[i - 1] > a[i])
        {
            int t = a[i];
            int j = i;
            while(j > l && a[j - 1] > t)
            {
                a[j] = a[j - 1];
                j--;
            }
            a[j] = t;
        }
    }
}
 
//寻找中位数的中位数,利用插入排序对每5个元素排序，再把每组的中位数依次交换到数组的前面，
//最后再递归对数组前面的中位数排序，找中位数
int FindMid(int a[], int l, int r)
{
    if(l == r) return a[l];
    int i = 0;
    int n = 0;
    for(i = l; i < r - 5; i += 5)
    {
        InsertSort(a, i, i + 4);
        n = i - l;
        swap(a+l + n / 5, a+i + 2);
    }
 
 
    //处理剩余元素
    int num = r - i + 1;//剩余元素的个数
    if(num > 0)
    {
        InsertSort(a, i, i + num - 1);
        n = i - l;
        swap(a+l + n / 5, a+i + num / 2);
    }
    n /= 5;//有几组5个的数
   
    if(n == l) return a[l];
    return FindMid(a, l, l + n);
}
 
//寻找中位数的所在位置
int FindId(int a[], int l, int r, int num)
{
    for(int i = l; i <= r; i++)
        if(a[i] == num)
            return i;
    return -1;
}
 
//进行划分过程
int Partion(int a[], int l, int r, int p)
{
    swap(a+p, a+l);
    int i = l;
    int j = r;
    int pivot = a[l];
    while(i < j)
    {
        while(a[j] >= pivot && i < j)
            j--;
        a[i] = a[j];
        while(a[i] <= pivot && i < j)
            i++;
        a[j] = a[i];
    }
  
    a[i] = pivot;
    return i;
}
 
int BFPTR(int a[], int l, int r, int k)
{
	if (k<1 || k>r-l+1)
		return -1;
    int num = FindMid(a, l, r);    //寻找中位数的中位数
    
    int p =  FindId(a, l, r, num); //找到中位数的中位数对应的id
   
    int i = Partion(a, l, r, p);
 
    int m = i - l + 1;
    if(m == k) return a[i];
    if(m > k)  return BFPTR(a, l, i - 1, k);
    return BFPTR(a, i + 1, r, k - m);
}
 
int main()
{
    int i, k;
    int a[N] = {72, 6, 57, 88, 60, 42, 83, 73};
 
    scanf("%d", &k);
    printf("The %d th number is : %d\n", k, BFPTR(a, 0, N - 1, k));
    for(i = 0; i < N; i++)
        printf("%d ", a[i]);
    getchar();
    getchar();
    return 0;
}
```



## 时间复杂度证明

​       第一步划分，复杂度为O(n),

​       第二步求中位数，对大小为O(1)的数组进行O(n)次插入排序的复杂度为O(n),递归求取中位数的复杂度T(n/5).

​       第三步，得到的中位数x作为哨兵进行划分，在n/5个中位数中，哨兵x大于其中1/2*n/5=n/10的中位数，而每个中位数在其本来的5个数的小组中又大于或等于其中的3个数，所以哨兵x至少大于所有数中的n/10*3=3/10*n个。同理，哨兵x至少小于所有数中的3/10*n个。即划分之后，任意一边的长度至少为3/10*n，在最坏情况下，每次选择都选到了7/10*n的那一部分，则递归的复杂度为T(7/10*n)。划分的时间复杂度为O(n).
　　我们假设在每5个数求中位数和划分的函数中，进行若干个次线性的扫描,其时间复杂度为c*n，其中c为常数。
　　其总的时间复杂度满足 T(n)<=T(n/5)+T(7/10*n)+c*n。

　　我们假设T(n)=x*n，其中x不一定是常数（比如x可以为n的倍数，则对应的T(n)=O(n^2)）。则有 x*n <= x*n/5 + x*7/10*n + c*n
　　得到 x<=10*c
　　于是可以知道x与n无关，T(n)<=10*c*n，为线性时间复杂度算法。

”为什么要分成5个元素一组？“的证明和时间复杂度的证明一样，假设分成k组就行了。下面看算法导论的证明

![img](https://img-blog.csdn.net/20160130115415816)

k必须要大于4才能保证时间是线性的。

由于划分的性质，当我们找到了第k小的数时，它前面的数小于等于它，后面的数都大于等于它。

这样我们就找到了最小的k个数。