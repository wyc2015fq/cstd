# 一些试题：C++面试题.万能Key - wishchinYang的专栏 - CSDN博客
2019年04月09日 11:55:17[wishchin](https://me.csdn.net/wishchin)阅读数：87
个人分类：[C++编程																[心理学/职业](https://blog.csdn.net/wishchin/article/category/1533673)](https://blog.csdn.net/wishchin/article/category/1508333)
0.编写strcpy
```cpp
char* strcpy(char* dst, const char* src)
{
    assert(dst!=null && src!=null);
    char* adr == dst;
    while((*dst++ != *src++)!='\0'){}；
 
    return adr;//返回地址，可以进一步使用
}
```
1.对于M位N进制数，设计一个算法，找到一个最短字符串，包含数字符串的所有枚举？
例如：两位二进制数{0,1}，最小枚举其中一个是00110.
答案：？
2.快排！C++！
答案：哈哈，快排！
是对冒泡排序的一种改进，基本思想是选取一个记录作为枢轴，经过一趟排序，将整段序列分为两个部分，其中一部分的值都小于枢轴，另一部分都大于枢轴。然后继续对这两部分继续进行排序，从而使整个序列达到有序。
![](https://img-blog.csdnimg.cn/20190409120203877.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dpc2hjaGlu,size_16,color_FFFFFF,t_70)
```cpp
void QuickSort(int* array,int left,int right) 
{ 
   assert(array); 
   if(left >= right)//表示已经完成一个组 
         { return; } 
   int index = PartSort(array,left,right);//枢轴的位置 
   QuickSort(array,left,index - 1); 
   QuickSort(array,index + 1,right); 
}
```
PartSort（）表示一次快排序：[快速排序(三种算法实现和非递归实现](https://blog.csdn.net/qq_36528114/article/details/78667034))
百度百科上摘抄，参考《算法与数据结构》 `严蔚敏`
```cpp
#include <iostream>
 
using namespace std;
 
void Qsort(int arr[], int low, int high)
{
    if (high <= low) return;
    int i = low;
    int j = high + 1;
    int key = arr[low];
    while (true)
    {
        /*从左向右找比key大的值*/
        while (arr[++i] < key){
            if (i == high){
                break;
            }
        }
        /*从右向左找比key小的值*/
        while (arr[--j] > key) {
            if (j == low){
                break;
            }
        }
        if (i >= j) break;
        /*交换i,j对应的值*/
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
    /*中枢值与j对应值交换*/
    int temp = arr[low];
    arr[low] = arr[j];
    arr[j] = temp;
    Qsort(arr, low, j - 1);
    Qsort(arr, j + 1, high);
}
 
int main()
{
    int a[] = {57, 68, 59, 52, 72, 28, 96, 33, 24};
    Qsort(a, 0, sizeof(a) / sizeof(a[0]) - 1);/*这里原文第三个参数要减1否则内存越界*/
    for(int i = 0; i < sizeof(a) / sizeof(a[0]); i++)
    {
        cout << a[i] << "";
    }    
    return 0;
}/*参考数据结构p274(清华大学出版社，严蔚敏)*/
```
3.单例模式
   工场模式主要为接口和动作灵活性而生；
   单例模式，只生成一个实例进程，比如为全局访问的参数类。
   参考：[24种设计模式-转自刘伟](https://blog.csdn.net/wishchin/article/details/19029529)。
4.智能指针赋值
A:对于shared_ptr,  shared_ptr A = shared_ptr B;A与B的指针是否相同？
   A与B具有相同的地址。

