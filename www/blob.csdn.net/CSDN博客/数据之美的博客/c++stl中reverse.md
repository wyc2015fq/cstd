# c++stl中reverse - 数据之美的博客 - CSDN博客
2017年09月23日 15:03:20[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：146
目录[(?)](http://blog.csdn.net/dongfengkuayue/article/details/21106949#)[[-]](http://blog.csdn.net/dongfengkuayue/article/details/21106949#)
- [看到有人将int型指针传入reverse函数中长知识了](http://blog.csdn.net/dongfengkuayue/article/details/21106949#t0)
- [reverse](http://blog.csdn.net/dongfengkuayue/article/details/21106949#t1)- [参考](http://blog.csdn.net/dongfengkuayue/article/details/21106949#t2)
# 看到有人将int型指针传入reverse函数中，长知识了。
# reverse
**Visual Studio 2013**
其他版本
![](http://i3.msdn.microsoft.com/Areas/Epx/Content/Images/ImageSprite.png)
此主题尚未评级 - [评价此主题](http://msdn.microsoft.com/zh-cn/library/bcc5299d.aspx#feedback)
反转元素的顺序在范围内。

template<class BidirectionalIterator>
   void reverse(
      BidirectionalIterator _First, 
      BidirectionalIterator _Last
   );
参数
- _First
指向第一个元素的位置的双向迭代器在元素交换的范围。
- _Last
指向通过最终元素的位置的一双向迭代器在元素交换的范围。
备注
引用的源范围必须是有效的;所有指针必须dereferenceable，并在该序列中最后位置以访问按增量。
示例

// alg_reverse.cpp
// compile with: /EHsc
#include <vector>
#include <algorithm>
#include <iostream>
int main( ) {
   using namespace std;
   vector <int> v1;
   vector <int>::iterator Iter1;
   int i;
   for ( i = 0 ; i <= 9 ; i++ )
   {
      v1.push_back( i );
   }
   cout << "The original vector v1 is:\n ( " ;
   for ( Iter1 = v1.begin( ) ; Iter1 != v1.end( ) ; Iter1++ )
      cout << *Iter1 << " ";
   cout << ")." << endl;
   // Reverse the elements in the vector 
   reverse (v1.begin( ), v1.end( ) );
   cout << "The modified vector v1 with values reversed is:\n ( " ;
   for ( Iter1 = v1.begin( ) ; Iter1 != v1.end( ) ; Iter1++ )
      cout << *Iter1 << " ";
   cout << ")." << endl;
}
与反转的值的已修改的向量v1是：(9 8 7 6 5 4 3 2 1 0)。
要求
标头: <algorithm>
命名空间: std
请参见
#### 参考
[reverse（STL 示例）](http://msdn.microsoft.com/zh-cn/library/93wa19x9.aspx)
[标准模板库](http://msdn.microsoft.com/zh-cn/library/c191tb28.aspx)
