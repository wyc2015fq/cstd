
# 292. Nim Game - OraYang的博客 - CSDN博客

2017年08月05日 12:08:03[OraYang](https://me.csdn.net/u010665216)阅读数：222标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[OS																](https://blog.csdn.net/u010665216/article/category/6665157)[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)[
							](https://blog.csdn.net/u010665216/article/category/6665157)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
You are playing the following Nim Game with your friend: There is a heap of stones on the table, each time one of you take turns to remove 1 to 3 stones. The one who removes the last stone will be the winner. You will take the first turn to remove the stones.
Both of you are very clever and have optimal strategies for the game. Write a function to determine whether you can win the game given the number of stones in the heap.
For example, if there are 4 stones in the heap, then you will never win the game: no matter 1, 2, or 3 stones you remove, the last stone will always be removed by your friend.
思路：本题从理解题意上并不困难，一个典型的Nim问题，核心思想就是找到”必胜态“，分析推理过程如下：
当n∈[1,3]时，先手必胜。
当n == 4时，无论先手第一轮如何选取，下一轮都会转化为n∈[1,3]的情形，此时先手必负。
当n∈[5,7]时，先手必胜，先手分别通过取走[1,3]颗石头，可将状态转化为n == 4时的情形，此时后手必负。
当n == 8时，无论先手第一轮如何选取，下一轮都会转化为n∈[5,7]的情形，此时先手必负。
......
当n % 4 != 0时，先手必胜；否则先手必负。代码：
```python
class Solution {
public:
    bool canWinNim(int n) {
        return n%4!=0;   
    }  
};
```


