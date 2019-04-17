# Leetcode7——Reverse Integer - westbrook1998的博客 - CSDN博客





2018年02月23日 21:37:58[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：49标签：[水题																[算法																[Leetcode](https://so.csdn.net/so/search/s.do?q=Leetcode&t=blog)
个人分类：[Leetcode](https://blog.csdn.net/westbrook1998/article/category/7459919)





Given a 32-bit signed integer, reverse digits of an integer.

Example 1:

Input: 123 

Output:  321 

Example 2:
Input: -123 

Output: -321 

Example 3:
Input: 120 

Output: 21 

Note: 

Assume we are dealing with an environment which could only hold integers within the 32-bit signed integer range. For the purpose of this problem, assume that your function returns 0 when the reversed integer overflows.
代码：

```
class Solution {
    public int reverse(int x) {
        List<Integer> arr=new ArrayList<>();
        int flag=1;
        if(x<0){
            x=-x;
            flag=-1;
        }
        while(x>0){
            arr.add(x%10);
            x/=10;
        }
        int len=arr.size();
        int y=0;
        for(int i=0; i<len;i++){
            y+=arr.get(i)*Math.pow(10,len-i-1);
        }
        if(y>=2147483647){
            return 0;
        }
        else{
            return y*flag;
        }
    }
}
```](https://so.csdn.net/so/search/s.do?q=算法&t=blog)](https://so.csdn.net/so/search/s.do?q=水题&t=blog)




