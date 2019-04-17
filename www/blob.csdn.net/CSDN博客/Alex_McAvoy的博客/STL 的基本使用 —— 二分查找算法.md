# STL 的基本使用 —— 二分查找算法 - Alex_McAvoy的博客 - CSDN博客





2019年02月11日 20:09:20[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：60
个人分类：[STL 的使用——算法](https://blog.csdn.net/u011815404/article/category/8793970)









STL 中，在 <algorithm> 头文件里提供了两个利用二分查找的方法在一个排好序的数组中进行查找。

在一个从小到大的排好序的数组中：
- **lower_bound(begin,end,num)**：从数组的 begin 位置到 end-1 位置二分查找**第一个大于等于 num****的数字**，找到返回该数字的地址，不存在则返回 end
- **upper_bound( begin,end,num)**：从数组的 begin 位置到 end-1 位置二分查找**第一个大于 num 的数字**，找到返回该数字的地址，不存在则返回 end

而在一个从大到小的排好序的数组中：
- **lower_bound( begin,end,num,greater<type>() )**：从数组的 begin 位置到 end-1 位置二分查找**第一个小于等于 num****的数字**，找到返回该数字的地址，不存在则返回 end
- **upper_bound( begin,end,num,greater<type>() )**：从数组的 begin 位置到 end-1 位置二分查找**第一个小于 num 的数字**，找到返回该数字的地址，不存在则返回 end。

可以通过这两个方法返回的地址减去起始地址 begin，即可得到数字在数组中的下标。

```cpp
int cmp(int a,int b){
	return a>b;
}
int num[5]={1,4,6,9,8}; 
int pos;

sort(num,num+5);//按从小到大 
pos=lower_bound(num,num+5,7)-num;//返回数组中第一个大于或等于7的值 
pos=upper_bound(num,num+5,7)-num;//返回数组中第一个大于7的值

sort(num,num+6,cmp);//按从大到小
pos=lower_bound(num,num+5,7,greater<int>())-num;//返回数组中第一个小于或等于7的值 
pos=upper_bound(num,num+5,7,greater<int>())-num;//返回数组中第一个小于7的值
```





