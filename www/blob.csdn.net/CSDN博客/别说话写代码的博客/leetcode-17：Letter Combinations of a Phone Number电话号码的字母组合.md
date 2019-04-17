# leetcode-17：Letter Combinations of a Phone Number电话号码的字母组合 - 别说话写代码的博客 - CSDN博客





2018年12月20日 10:27:18[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：23标签：[leetcode																[Letter Combinations of a Phone																[电话号码的字母组合](https://so.csdn.net/so/search/s.do?q=电话号码的字母组合&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=Letter Combinations of a Phone&t=blog)




题目：
|Given a string containing digits from `2-9` inclusive, return all possible letter combinations that the number could represent.A mapping of digit to letters (just like on the telephone buttons) is given below. Note that 1 does not map to any letters.![](https://img-blog.csdnimg.cn/20181220102719648)**Example:****Input: **"23"**Output:** ["ad", "ae", "af", "bd", "be", "bf", "cd", "ce", "cf"].|给定一个仅包含数字 `2-9` 的字符串，返回所有它能表示的字母组合。给出数字到字母的映射如下（与电话按键相同）。注意 1 不对应任何字母。![](https://img-blog.csdnimg.cn/20181220102719648)**示例:****输入：**"23"**输出：**["ad", "ae", "af", "bd", "be", "bf", "cd", "ce", "cf"].|
|----|----|

思路：先建立一个映射表dict。在遍历数字时，在dict中取出数字对应的字符str。建立一个临时字符数组t，将str的每个字符都连接到 res的每个元素的后面。

```cpp
class Solution {
public:
    vector<string> letterCombinations(string digits) {
        if(digits.empty()) return {};
        vector<string> res{""};
        string dict[]={"","","abc","def","ghi","jkl","mno","pqrs","tuv","wxyz"};
        for(int i=0;i<digits.size();++i)
        {
            string str=dict[digits[i]-'0'];
            vector<string> t;
            for(int j=0;j<str.size();j++)
                for(string s:res) t.push_back(s+str[j]);
            res = t;
        }return res;
    }
};
```

![](https://img-blog.csdnimg.cn/20181220102159454.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)

参考：[http://www.cnblogs.com/grandyang/p/4452220.html](http://www.cnblogs.com/grandyang/p/4452220.html)](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)




