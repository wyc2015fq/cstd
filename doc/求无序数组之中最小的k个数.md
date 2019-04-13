# 求无序数组之中最小的k个数

2016年01月29日 15:56:57 [l-jobs](https://me.csdn.net/qq_26010491) 阅读数：1319



版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/qq_26010491/article/details/50607735



在无序数组中国找最小的k个数,一般有以下几种办法：

  1.比较排序，然后找最小的k个数，时间复杂度下界为O(nlogn)

  2.计数排序，时间复杂度可以做到O(n),但是要看具体的数据情况。也比较费空间。

以上做法都是把所有元素的个数排好序，但是我们只需要找到前k个最小的数，不要求所有数据有序。

  3.遍历所有数据，找到最小的数。再遍历剩下的数，找第二小的数，如此遍历k次,时间复杂度为O(n*k)

  4.堆排序思想，建n个元素的最小堆，取到k个元素的时候结束。时间复杂度O(klogn).

  5.堆选择思想，建一个k个元素的最大堆，然后在剩下的n-k个元素与堆顶元素比较，如果比堆顶元素小，则交换，调整堆，最后的堆就是最小的k个数。时间复杂度O(nlogk).此方法适合n太大,不能直接建n大小的堆的情况。但时间复杂度其实比第4个大。

  6.快排思想的快速选择bfprt算法。时间复杂度O(n),但其时间复杂度的常数因子较大，和用堆的方法谁快要看数据情况。
下面给出第4和第5种方法的代码，原代码出处[程序员编程艺术](http://blog.csdn.net/v_july_v/article/details/6370650)

```cpp
//建n大小的堆
//先对元素数组原地建最小堆，O(n)。然后提取K次，但是每次提取时，  
//换到顶部的元素只需要下移顶多k次就足够了，下移次数逐次减少。此种方法的复杂度为O（n+klogn）。  
#include <stdio.h>  
#include <stdlib.h>  
#define MAXLEN 123456  
#define K 100  
  
// 堆调整
void HeapAdjust(int array[], int i, int Length)  
{  
    int child,temp;  
    for(temp=array[i];2*i+1<Length;i=child)  
    {  
        child = 2*i+1;  
        if(child<Length-1 && array[child+1]<array[child])  
            child++;  
        if (temp>array[child])  
            array[i]=array[child];  
        else  
            break;  
        array[child]=temp;  
    }  
}  
  
void Swap(int* a,int* b)  
{  
    *a=*a^*b;  
    *b=*a^*b;  
    *a=*a^*b;  
}  
//取堆顶元素，然后把堆顶元素和最后一个元素交换  
int GetMin(int array[], int Length,int k)  
{  
    int min=array[0];  
    Swap(&array[0],&array[Length-1]);  
      
    int child,temp;  
    int i=0,j=k-1;  
    for (temp=array[0]; j>0 && 2*i+1<Length; --j,i=child)  
    {  
        child = 2*i+1;  
        if(child<Length-1 && array[child+1]<array[child])  
            child++;  
        if (temp>array[child])  
            array[i]=array[child];  
        else  
            break;  
        array[child]=temp;  
    }  
      
    return min;  
}  
  
void Kmin(int array[] , int Length , int k)  
{  
    for(int i=Length/2-1;i>=0;--i)   
        //初始建堆，时间复杂度为O(n)  
        HeapAdjust(array,i,Length);  
      
    int j=Length;  
    for(i=k;i>0;--i,--j)     
    {  
        int min=GetMin(array,j,i);  
        printf("%d,", min);  
    }  
}  
  
int main()  
{  
    int array[MAXLEN];  
    for(int i=MAXLEN;i>0;--i)  
        array[MAXLEN-i] = i;  
      
    Kmin(array,MAXLEN,K);  
    return 0;  
}  
```

下面是

只建k大小的堆，借助堆，查找最小的k个数 ，作者用了一个超大数据文件来测试。

```cpp
#include <iostream>  
#include <assert.h>  
using namespace std;  
void MaxHeap(int heap[], int i, int len);  
// 建立最大堆  
void BuildHeap(int heap[], int len)  
{  
    if (heap == NULL)  
        return;  
      
    int index = len / 2;  
    for (int i = index; i >= 1; i--)  
        MaxHeap(heap, i, len);  
}  
 
//调整最大堆  
void MaxHeap(int heap[], int i, int len)  
{  
    int largeIndex = -1;  
    int left = i * 2;  
    int right = i * 2 + 1;  
      
    if (left <= len && heap[left] > heap[i])  
        largeIndex = left;  
    else  
        largeIndex = i;  
      
    if (right <= len && heap[right] > heap[largeIndex])  
        largeIndex = right;  
      
    if (largeIndex != i)  
    {  
        swap(heap[i], heap[largeIndex]);  
        MaxHeap(heap, largeIndex, len);  
    }  
}  
int main()  
{  
    // 定义数组存储堆元素  
    int k;  
    cin >> k;  
    int *heap = new int [k+1];   //注，只需申请存储k个数的数组  
    FILE *fp = fopen("data.txt", "r");   //从文件导入海量数据（便于测试，只截取了9M的数据大小）  
    assert(fp);  
      
    for (int i = 1; i <= k; i++)  
        fscanf(fp, "%d ", &heap[i]);  
      
    BuildHeap(heap, k);      //建堆  
      
    int newData;  
    while (fscanf(fp, "%d", &newData) != EOF)  
    {  
        if (newData < heap[1])   //如果遇到比堆顶元素kmax更小的，则更新堆  
        {  
            heap[1] = newData;  
            MaxHeap(heap, 1, k);   //调整堆  
        }  
          
    }  
      
    for (int j = 1; j <= k; j++)  
        cout << heap[j] << " ";  
    cout << endl;  
      
    fclose(fp);  
    return 0;  
}  
```





bfprt算法





经典算法，由快速排序的划分思想而来。

限于篇幅，介绍请看博客。