# LeetCode-Palindrome Number - yixianfeng41的专栏 - CSDN博客
2015年12月02日 10:49:26[第2梦](https://me.csdn.net/yixianfeng41)阅读数：666
个人分类：[数据结构](https://blog.csdn.net/yixianfeng41/article/category/5829745)
所属专栏：[数据结构与算法](https://blog.csdn.net/column/details/14515.html)
### 什么是回文数字：
就是正着看和反着看是一样的数字，就是回文数字，比如：1234321,45654是回文数字，而1234这样的就不是回文数字。
### 代码（72ms）：
```cpp
bool isPalindrome(int x) 
    {
        if(x==0) //0也是回文数字
        {
            return true;
        }
        else if(x<0)  //负数一定不是回文数字
        {
            return false;
        }
        else
        {
            int new_x=0;
            int temp=x;
            while(x)
            {
    		 new_x=new_x*10+x%10;
    	  	 x=x/10;
    	    }
    	    if(temp==new_x)
            {
    		 return true;
    	    }
        }
	return false;
    }
```
