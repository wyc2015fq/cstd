# leetcode 66.Plus One（简单大数模拟） - HJ - CSDN博客
2018年03月04日 12:45:17[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：109
题目链接： [](https://leetcode.com/problems/plus-one/description/)[https://leetcode.com/problems/plus-one/description/](https://leetcode.com/problems/plus-one/description/)
解题思路：
大数模拟，主要是考虑到+1进位的情况
```cpp
class Solution {
public:
    vector<int> plusOne(vector<int>& digits) {
        for(int i = digits.size() - 1 ; i >= 0 ; i--) {
            if(digits[i] + 1 < 10) {
                digits[i]++;
                break;
            }
            digits[i] = 0;
        }
        if(digits[0] == 0) {
            digits.insert(digits.begin(),1);    //对于出现9+1以及类似的情况，需要特殊处理
        }
        return digits;
    }
};
```
