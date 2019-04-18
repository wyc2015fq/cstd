# LeetCode Remove Duplicates from Sorted Array II 删除整型数组中的重复元素并返回剩下元素个数2 - xcw0754 - 博客园
# [LeetCode Remove Duplicates from Sorted Array II 删除整型数组中的重复元素并返回剩下元素个数2](https://www.cnblogs.com/xcw0754/p/4126743.html)
```
1 class Solution {
 2 public:
 3     int removeDuplicates(int A[], int n) {
 4         int *s=&A[0],*e=&A[0];    //s指向“连续数字”的第一个，e往后遍历相同的
 5         int i,flag=0,j=n;
 6         for(i=1;i<n;i++){
 7             e++;
 8             if(*s!=*e||(*s==*e&&flag==0) ){        //新的数字，直接加复制到s处
 9                 if(*s==*e)
10                     flag=1;
11                 else
12                     flag=0;
13                 s++;
14                 *s=*e;
15             }
16             else
17                 j--;
18         }
19         return j;
20     }
21 
22 };
```
题意：提供一个整型数组，和该数组内有n个元素，数组已经有序。相同的数字不允许>2个，要么1个，要么2个。修改原数组，并返回剩下的元素个数。
思路：增加一个标志flag=0，遇到多个相同的数字时，第一个正常处理，在第2个数字的处理时将标志改变一下为非零，那么后面相同的就可以去掉了。

