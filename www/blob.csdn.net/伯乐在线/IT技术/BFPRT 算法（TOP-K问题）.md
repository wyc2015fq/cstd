# BFPRT 算法（TOP-K问题） - 文章 - 伯乐在线
原文出处： [刘毅](https://www.61mon.com/index.php/archives/175/)
### 一：背景介绍
在一堆数中求其前k大或前k小的问题，简称TOP-K问题。而目前解决TOP-K问题最有效的算法即是”BFPRT算法”，又称为”中位数的中位数算法”，该算法由Blum、Floyd、Pratt、Rivest、Tarjan提出，最坏时间复杂度为$O(n)$。
在首次接触TOP-K问题时，我们的第一反应就是可以先对所有数据进行一次排序，然后取其前k即可，但是这么做有两个问题：
- 快速排序的平均复杂度为$O(nlogn)$，但最坏时间复杂度为$O(n^2)$，不能始终保证较好的复杂度。
- 我们只需要前k大的，而对其余不需要的数也进行了排序，浪费了大量排序时间。
除这种方法之外，堆排序也是一个比较好的选择，可以维护一个大小为k的堆，时间复杂度为$O(nlogk)$。
那是否还存在更有效的方法呢？受到快速排序的启发，通过**修改快速排序中主元的选取方法**可以降低快速排序在**最坏情况下的时间复杂度**，并且我们的目的只是求出前k，故递归的规模变小，速度也随之提高。下面来简单回顾下快速排序的过程，以升序为例：
（1）：选取主元（数组中随机一个元素）；
（2）：以选取的主元为分界点，把小于主元的放在左边，大于主元的放在右边；
（3）：分别对左边和右边进行递归，重复上述过程。
### 二：BFPRT算法过程及代码
BFPRT算法步骤如下：
（1）：选取主元；
（1.1）：将n个元素划分为$⌊frac n5⌋$个组，每组5个元素，若有剩余，舍去；
（1.2）：使用插入排序找到$⌊frac n5⌋$个组中每一组的中位数；
（1.3）：对于（1.2）中找到的所有中位数，调用BFPRT算法求出它们的中位数，作为主元；
（2）：以（1.3）选取的主元为分界点，把小于主元的放在左边，大于主元的放在右边；
（3）：判断主元的位置与k的大小，有选择的对左边或右边递归。
上面的描述可能并不易理解，先看下面这幅图：
![](http://jbcdn2.b0.upaiyun.com/2017/10/5bbd9517fcc7829ce1a5689afa13c379.png)
BFPRT()调用GetPivotIndex()和Partition()来求解第k小，在这过程中，GetPivotIndex()也调用了BFPRT()，即GetPivotIndex)和BFPRT()为互递归的关系。
下面为代码实现，其所求为**前K小的数**：


```
/**
 * BFPRT算法（前K小数问题）
 *
 * author : 刘毅（Limer）
 * date   : 2017-01-25
 * mode   : C++
 */
#include <iostream>
#include <algorithm>
using namespace std;
/* 插入排序，返回中位数下标 */
int InsertSort(int array[], int left, int right)
{
    int temp;
    int j;
    for (int i = left + 1; i <= right; i++)
    {
        temp = array[i];
        j = i - 1;
        while (j >= left && array[j] > temp)
            array[j + 1] = array[j--];
        array[j + 1] = temp;
    }
    return ((right - left) >> 1) + left;
}
/* 返回中位数的中位数下标 */
int BFPRT(int array[], int left, int right, const int & k);
int GetPivotIndex(int array[], int left, int right)
{
    if (right - left < 5)
        return InsertSort(array, left, right);
    int sub_right = left - 1;
    for (int i = left; i + 4 <= right; i += 5)
    {
        int index = InsertSort(array, i, i + 4);  // 找到五个元素的中位数的下标
        swap(array[++sub_right], array[index]);   // 依次放在左侧
    }
    return BFPRT(array, left, sub_right, ((sub_right - left + 1) >> 1) + 1);
}
/* 利用中位数的中位数的下标进行划分，返回分界线下标 */
int Partition(int array[], int left, int right, int pivot_index)
{
    swap(array[pivot_index], array[right]);  // 把主元放置于末尾
    int divide_index = left;                 // 跟踪划分的分界线
    for (int i = left; i < right; i++)
    {
        if (array[i] < array[right])
            swap(array[divide_index++], array[i]);  // 比主元小的都放在左侧
    }
    swap(array[divide_index], array[right]);  // 最后把主元换回来
    return divide_index;
}
int BFPRT(int array[], int left, int right, const int & k)
{
    int pivot_index = GetPivotIndex(array, left, right);            // 得到中位数的中位数下标
    int divide_index = Partition(array, left, right, pivot_index);  // 进行划分，返回划分边界
    int num = divide_index - left + 1;
    if (num == k)
        return divide_index;
    else if (num > k)
        return BFPRT(array, left, divide_index - 1, k);
    else
        return BFPRT(array, divide_index + 1, right, k - num);
}
int main()
{
    int k = 5;
    int array[10] = { 1,1,2,3,1,5,-1,7,8,-10 };
    cout << "原数组：";
    for (int i = 0; i < 10; i++)
        cout << array[i] << " ";
    cout << endl;
    cout << "第" << k << "小值为：" << array[BFPRT(array, 0, 9, k)] << endl;
    cout << "变换后的数组：";
    for (int i = 0; i < 10; i++)
        cout << array[i] << " ";
    cout << endl;
    return 0;
}
```
``运行如下：
![](http://jbcdn2.b0.upaiyun.com/2017/10/7f4233b03fbad0d9b681860edf4a1bc3.png)
### 三：时间复杂度分析
![QQ截图20171024111911](http://jbcdn2.b0.upaiyun.com/2017/10/d206bda7b56cbfee9973074ed8eac16b.png)
![QQ截图20171024112045](http://jbcdn2.b0.upaiyun.com/2017/10/e4022c9658a1f141e7c9a66c6b4d1626.png)
### 四：参考文献
- 算法导论（第3版）. Page 120.
