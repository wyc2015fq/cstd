# STL 的基本使用 —— 排列组合算法 - Alex_McAvoy的博客 - CSDN博客





2018年08月16日 16:31:27[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：577








# 【概述】

首先要了解什么是 “下一个” 排列组合，什么是 “上一个” 排列组合。

假设有三个数字组成的序列：｛a，b，c｝

则这个序列有6种可能的排列组合：abc、acb、bac、bca、cab、cba

上述的排列组合是根据 less-than 操作符做字典顺序的排序，即：abc 处于第一，每一个元素都小于其后的元素，而 acb 是次一个排列组合，它是固定了序列内最小元素( a )之后所做的新组合。

同理，序列中次小元素( b )而做的排列组合，在次序上将先于那些固定最小元素( c )而做的排列组合，以 bac、bca 为例，bac 在 bca 之前，因为次序 ac 小于序列 ca ，因此，对于 bca，可以说其前一个排列组合是 bac，其后一个排列组合是 cab。

要注意的是，处于排列首的序列 abc 没有 “前一个” 排列组合，处于排列尾的序列 cba 没有“后一个”排列组合。

# 【STL 中提供的算法】

STL 提供了两个用来计算排列组合关系的算法，分别是 next_permutation() 与 prev_permutation()

其中，next_permutation() 是取出当前范围内的排列，并重新排序为下一个排列，prev_permutation() 是取出指定范围内的序列并将它重新排序为上一个序列。如果不存在下一个序列或上一个序列则返回 false，否则返回 true

这个算法有两个版本，其一使用元素类别所提供的操作符来决定下一个或上一个排列组合，其二是以仿函数 comp 来决定

## 1.算法思想

以 next_permutation() 为例：
- 从最尾端开始向前寻找两个相邻的元素，令第一元素为 *i,第二元素为 *ii,且满足 *i<*ii
- 找到上述的一组相邻元素后，从最尾端向前检验，找出第一个大于 *i 的元素，令其为 *j，然后将 i、j 元素交换
- 再将 ii 之后的所有元素颠倒排序

假设存在序列｛0，1，2，3，4｝，下图即为寻找全排列的过程

![](https://img-blog.csdn.net/20180816162225256?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## 2.next_permutation() 的用法

对于给定的任意一种排列组合，如果能求出下一个排列的情况，那么求得所有全排列情况就容易了。

利用 next_permutation() 的返回值，通过判断排列是否结束，即可求出全排列。

```cpp
int a[N];
void all_permutation(int n)
{
    sort(arr,arr+n);
    do{
        for(int i=0; i<n; i++)
            printf("%d ",arr[i]);
        printf("\n");
    }while(next_permutation(arr,arr+n));
}
```

## 3.next_permutation() 与 prev_permutation() 的区别

next_permutation() 函数默认的是从小到大的顺序，而 prev_permutation() 函数默认的是从大到小的顺序。

例如：对于序列 {3,1,2}

用 next_permutation() 函数得到的结果是：312、321

用 prev_permutation() 函数得到的结果是：312、231、213、132、123





