# C++实现二分查找算法 - 数据之美的博客 - CSDN博客
2017年09月06日 14:15:55[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：150
想必二分查找很多人都不陌生，或许说很熟悉，但是在实际生活中又有很多人不能正确的写出它的相应代码，因为二分查找的边界条件等很难控制，下面我们来仔细的分析一下二分查找，这只是个人看法，如有异议，欢迎提出。
1、二分查找可以解决的问题：二分查找可以解决预排序数组的查找问题。只要数组中包含T（即要查找的值），那么通过不断的缩小包含T的范围，最终就可以找到它。一开始，范围覆盖整个数组，将数组的中间项与T进行比较，可以排除一般的元素，范围缩小一半。就这样反复比较反复缩小范围，最终就会在数组中找到T，或者确定原以为T所在的范围实际为空。对于包含N个元素的表，整个查找过程大约要经过Log（2）N次比较。
注意：二分查找是针对有序的数组而言的。
2、二分查找的相关代码如下：
#include<iostream>
usingnamespacestd;
intbinary_search(intarr[],intn,intkey)
{
intleft
 = 0;  //数组的首位置，即arr[0]处
intright
 = n - 1;//数组的最后一个位置，即arr[n-1],数组大小为n
//循环条件一定要注意
while(left
 <= right)
          {
intmid
 = left + ((right - left) >> 1);//此处的mid的计算一定要放在while循环内部，否则mid无法正确更新;并且此处用移位代替除以2可以提高效率，而且可以防止溢出。
if(arr[mid]
 > key)//数组中间的位置得数大于要查找的数，那么我们就在中间数的左区间找
                   {
                             right = mid - 1;
                   }
elseif(arr[mid]
 < key)//数组中间的位置得数小于要查找的数，那么我们就在中间数的右区间找
                   {
                             left = mid + 1;
                   }
else
                   {
returnmid;//中间数刚好是要查找的数字。
                   }
          }
//执行流如果走到此处说明没有找到要查找的数字。
return-1;
}
测试用例如下所示：
voidTest()
{
intarr[5]
 = { 1, 3, 5, 9, 10 };
intret1
 = 0,ret2 = 0,ret3 = 0;
          ret1 = binary_search(arr, 5, 9);
          cout << ret1 << endl;
          ret2 = binary_search(arr, 5, 5);
          cout << ret2 << endl;
          ret3 = binary_search(arr, 5, 2);
          cout << ret3 << endl;
}
3、二分查找代码需要注意的地方：
right=n-1-------->while(left <= right)-------->right=mid-1;
right=n-------->while(left < right)-------->right=mid;
此外：大部分人喜欢在最开始就判断arr[mid]与key相等，但是这样是不明智的，因为相等情况在大多状况下都是少数，写在开始的话，每次循环都需要判断一次相等，浪费时间，效率太低。
