# LeetCode  Remove Duplicates from Sorted Array删除整型数组中的重复元素并返回剩下元素个数 - xcw0754 - 博客园
# [LeetCode  Remove Duplicates from Sorted Array删除整型数组中的重复元素并返回剩下元素个数](https://www.cnblogs.com/xcw0754/p/4114487.html)
```
1 class Solution {
 2 public:
 3 int removeDuplicates(int A[], int n) {
 4     int *s=&A[0],*e=&A[0];    //s指向开头第一个，e往后遍历相同的
 5     int t,i,j=n;
 6     for(i=1;i<n;i++){
 7         e++;
 8         if(*s==*e)
 9             j--;
10         else{
11             s++;
12             *s=*e;
13         }
14     }
15     return j;
16 }
17 };
```
题意：给一个整型有序数组，将其中重复的元素删除，几个相同的元素只留下一个即可，并返回共有多少种不同的元素。
思路：这是数组，所以有重复的地方就要移动后面的元素，用两个指针s和e，e用来遍历所有元素，s用来指向当前已处理过的地方，最终e会遍历到第n个，s会指向新数组的最后一个元素。
注意：没什么好注意的，这么短的代码。指针不要越界。

