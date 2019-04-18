# LeetCode  Remove Element删除元素 - xcw0754 - 博客园
# [LeetCode  Remove Element删除元素](https://www.cnblogs.com/xcw0754/p/4115134.html)
```
1 class Solution {
 2 public:
 3 int removeElement(int A[], int n, int elem) {
 4     int *p=A,*e=&A[n-1];
 5     int i,num=n;
 6     for(i=0;i<n;i++){    //一共要对比n次，不能用n来处理，会影响循环
 7         if(*p==elem){
 8             if(p==e)    //已经处理到最后一个相同，只需总数减1
 9                 num--;
10             else{
11                 *p=*e;
12                 e--;
13                 num--;
14             }
15         }
16         else
17             p++;
18     }
19     return num;
20 }
21 };
```
题意：给一个整型数组（无序），删除规定的某一元素，返回剩下的元素个数。
思路：两个指针，一个从头扫，一个从尾扫，一共要对比n次，因为有n个元素。从头扫到该删除的元素就用后面扫起的元素代替掉。
注意：数组是无序的，你可以随意将顺序更改，只要得到一个结果：规定删除的元素值不能出现新数组中即可。利用此特点，不用每次扫到一个要删除的元素就将该元素之后的所有元素往前移动了。

