
# Add Binary - LeetCode - OraYang的博客 - CSDN博客

2017年07月14日 12:22:41[OraYang](https://me.csdn.net/u010665216)阅读数：152标签：[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：Given two binary strings, return their sum (also a binary string).
For example,
a =`"11"`
b =`"1"`
Return`"100"`.
思路：
这道题目，主要思想就是从两个字符串尾部，逐位相加，并设置标志位，对一些特殊条件一定要注意，本身算法并不难，主要是if-else逻辑判断比较多
代码：

```python
class Solution {
public:
    string addBinary(string a, string b) {
        int temp,len_a,len_b;
        len_a = a.size();
        len_b = b.size();
        
        string::iterator itr_a = a.end() - 1;
        string::iterator itr_b = b.end() - 1;
        string str_a =a;
        string str_b =b;
        if(len_a<len_b)
        {
            a =str_b;
            b =str_a;
            itr_a = a.end() - 1;
            itr_b = b.end() - 1;
        }
            int c =0;
            while(itr_b!=b.begin()-1)
            {
                if(c == 0)
                {
                if(*itr_a ==*itr_b&&*itr_a == '1')
                {
                    
                    c=1;
                    *itr_a = '0';
                    itr_a --;
                    itr_b --;
                }
                else if(*itr_a == *itr_b&&*itr_a =='0')
                {
                    itr_a --;
                    itr_b --;
                    c = 0;
                }
                else
                {
                    *itr_a = '1';
                    c = 0;
                    itr_a --;
                    itr_b --;
                }
                    }
                else 
                {
                    if(*itr_a==*itr_b&&*itr_a=='0')
                    {
                        *itr_a ='1';
                        c = 0;
                        itr_a --;
                    itr_b --;
                    }
                    else if(*itr_a !=*itr_b)
                    {
                        c =1;
                        *itr_a = '0';
                        itr_a --;
                    itr_b --;
                    }
                    else
                    {
                        c =1;
                        *itr_a ='1';
                        itr_a --;
                    itr_b --;
                    }
                }
                
            }
        if(c==1)
        {
            while(itr_a!=a.begin()-1)
            {
                  cout<<a<<endl;
                if(c==1)
                {
                if(*itr_a=='0')
                {
                    c =0;
                    *itr_a = '1';
                    itr_a --;
                    return a;
                }
                else if(*itr_a =='1')
                {
                    c =1;
                    *itr_a = '0';
                    itr_a --;
                }
                  
            }
        }
            if(c==1)
            {
                a.insert(0,"1");
                return a;
            }
        }
 return a;
    }
};
```



