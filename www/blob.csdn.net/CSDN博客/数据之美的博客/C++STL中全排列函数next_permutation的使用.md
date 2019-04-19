# C++STL中全排列函数next_permutation的使用 - 数据之美的博客 - CSDN博客
2017年08月30日 18:47:41[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：239
个人分类：[算法																[数据结构](https://blog.csdn.net/oppo62258801/article/category/6551324)](https://blog.csdn.net/oppo62258801/article/category/6551325)
   next_permutation函数
    组合数学中经常用到排列，这里介绍一个计算序列全排列的函数：next_permutation（start,end），和prev_permutation（start,end）。这两个函数作用是一样的，区别就在于前者求的是当前排列的下一个排列，后一个求的是当前排列的上一个排列。至于这里的“前一个”和“后一个”，我们可以把它理解为序列的字典序的前后，严格来讲，就是对于当前序列pn，他的下一个序列pn+1满足：不存在另外的序列pm，使pn<pm<pn+1.
对于next_permutation函数，其函数原型为：
     #include <algorithm>
     bool next_permutation(iterator start,iterator end)
当当前序列不存在下一个排列时，函数返回false，否则返回true
我们来看下面这个例子：
**[cpp]**[view
 plain](http://blog.csdn.net/ac_gibson/article/details/45308645#)[copy](http://blog.csdn.net/ac_gibson/article/details/45308645#)
- #include <iostream>
- #include <algorithm>
- usingnamespace std;  
- int main()  
- {  
- int num[3]={1,2,3};  
- do
-     {  
-         cout<<num[0]<<" "<<num[1]<<" "<<num[2]<<endl;  
-     }while(next_permutation(num,num+3));  
- return 0;  
- }  
输出结果为：
![](https://img-blog.csdn.net/20150427130510043?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQUNfR2lic29u/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
当我们把while(next_permutation(num,num+3))中的3改为2时，输出就变为了：
![](https://img-blog.csdn.net/20150427130713826?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQUNfR2lic29u/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
由此可以看出，next_permutation(num,num+n)函数是对数组num中的前n个元素进行全排列，同时并改变num数组的值。
另外，需要强调的是，next_permutation（）在使用前需要对欲排列数组按升序排序，否则只能找出该序列之后的全排列数。比如，如果数组num初始化为2,3,1，那么输出就变为了：
![](https://img-blog.csdn.net/20150427131704330?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQUNfR2lic29u/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
此外，next_permutation（node,node+n,cmp）可以对结构体num按照自定义的排序方式cmp进行排序。
[](http://blog.csdn.net/ac_gibson/article/details/45308645#)[](http://blog.csdn.net/ac_gibson/article/details/45308645#)[](http://blog.csdn.net/ac_gibson/article/details/45308645#)[](http://blog.csdn.net/ac_gibson/article/details/45308645#)[](http://blog.csdn.net/ac_gibson/article/details/45308645#)[](http://blog.csdn.net/ac_gibson/article/details/45308645#)
