
# 657. Judge Route Circle - OraYang的博客 - CSDN博客

2017年08月31日 11:43:03[OraYang](https://me.csdn.net/u010665216)阅读数：200标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Initially, there is a Robot at position (0, 0). Given a sequence of its moves, judge if this robot makes a circle, which means it moves back tothe original place.
The move sequence is represented by a string. And each move is represent by a character. The valid robot moves are`R`(Right),`L`(Left),`U`(Up)
 and`D`(down). The output should be true or false representing whether the robot makes
 a circle.
Example 1:
Input:"UD"Output:true
Example 2:
Input:"LL"Output:false思路：本题思路很简单，建立map容器，统计各个字符出现的次数，只要‘U'与’D';'R‘与’L'出现次数相等，就能回到原点，否则回不到原点。
代码：

```python
class Solution {
public:
    bool judgeCircle(string moves) {
        map<char,int> mp;
        for(int i=0;i<moves.size();i++)
            mp[moves[i]]++;
        if(mp['U']==mp['D']&&mp['L']==mp['R'])
            return true;
        else
            return false;    
    }
};
```


