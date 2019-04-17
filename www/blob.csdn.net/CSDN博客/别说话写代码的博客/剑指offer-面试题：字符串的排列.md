# 剑指offer-面试题：字符串的排列 - 别说话写代码的博客 - CSDN博客





2019年03月18日 21:23:59[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：13
所属专栏：[剑指offer刷题c++/python](https://blog.csdn.net/column/details/29881.html)









输入一个字符串,按字典序打印出该字符串中字符的所有排列。例如输入字符串abc,则打印出由字符a,b,c所能排列出来的所有字符串abc,acb,bac,bca,cab和cba。

## 输入描述:

输入一个字符串,长度不超过9(可能有字符重复),字符只包括大小写字母。
思路：注意这个题目可能有 重复的数字，比如aa的排列就是[aa]，不是[aa，aa]，所以在递归 时需要判断。还有要在牛客oj上通过，就需要 将输出排序，因为它这个结果判断 是与排序的结果比较的

```cpp
class Solution {
public:
    void dfs(string str,int begin,vector<string> &res)
    {
        if(begin==str.size()-1) res.push_back(str);
        for(int k=begin;k<str.size();++k)
        {
            if(k!=begin && str[k]==str[begin]) //判断是否有重复
                continue;
            swap(str[k],str[begin]);
            dfs(str,begin+1,res);
            swap(str[k],str[begin]);
        }
    }
    vector<string> Permutation(string str) {
        if(str.empty()) return {};
        vector<string> res;
        dfs(str,0,res);
        sort(res.begin(),res.end());  //将结果都排序下
        return res;
    }
};
```





