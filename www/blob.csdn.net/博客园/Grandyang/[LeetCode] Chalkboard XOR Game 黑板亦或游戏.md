# [LeetCode] Chalkboard XOR Game 黑板亦或游戏 - Grandyang - 博客园







# [[LeetCode] Chalkboard XOR Game 黑板亦或游戏](https://www.cnblogs.com/grandyang/p/9445951.html)







We are given non-negative integers nums[i] which are written on a chalkboard.  Alice and Bob take turns erasing exactly one number from the chalkboard, with Alice starting first.  If erasing a number causes the bitwise XOR of all the elements of the chalkboard to become 0, then that player loses.  (Also, we'll say the bitwise XOR of one element is that element itself, and the bitwise XOR of no elements is 0.)

Also, if any player starts their turn with the bitwise XOR of all the elements of the chalkboard equal to 0, then that player wins.

Return True if and only if Alice wins the game, assuming both players play optimally.
Example:
Input: nums = [1, 1, 2]
Output: false
Explanation: 
Alice has two choices: erase 1 or erase 2. 
If she erases 1, the nums array becomes [1, 2]. The bitwise XOR of all the elements of the chalkboard is 1 XOR 2 = 3. Now Bob can remove any element he wants, because Alice will be the one to erase the last element and she will lose. 
If Alice erases 2 first, now nums becomes [1, 1]. The bitwise XOR of all the elements of the chalkboard is 1 XOR 1 = 0. Alice will lose.


Notes:
- `1 <= N <= 1000`. 
- `0 <= nums[i] <= 2^16`.



这道题介绍了一种亦或游戏，写在黑板上，（黑板一脸懵逼，跟我有个毛关系）。爱丽丝和鲍勃两个人轮流擦除一个数字，如果剩下的数字亦或值为0的话，那么当前选手就输了。反过来也可以这么说，如果某一个选手开始游戏时，当前数字的亦或值为0了，那么直接就赢了。现在给了我们一个数组，问先手的爱丽丝能否获胜。那么其实这道题是一道有技巧的题，并不是让我们按照游戏规则那样去遍历所有的情况，有海量的运算。这题有点像之前那道[Nim Game](http://www.cnblogs.com/grandyang/p/4873248.html)，重要的是技巧！技巧！技巧！重要的事情说三遍～但我等凡夫俗子如何看的出技巧啊，看不出技巧只能看Discuss了，博主也没看出来。但实际上这道题的解法可以非常的简单，两三行就搞定了。辣么开始讲解吧：首先根据题目的描述，我们知道了某个选手在开始移除数字之前，如果数组的亦或值为0的话，选手直接获胜，那么先手爱丽丝在开始开始之前也应该检查一遍数组的亦或值，如果是0的话，直接获胜。我们再来分析亦或值不为0的情况，既然不为0，那么亦或值肯定是有一个值的，我们假设其是x。下面就是本题的精髓了，是要考虑数组个数的奇偶情况（尼玛谁能想到！），这个数组个数一旦是偶数的话，就大有文章了，现在数字个数是偶数，且亦或值不为0，说明数组中的数字不全相同，因为偶数个相同数字的亦或值为0，那么爱丽丝只要移除一个不为x的数字就行了，这样移除后数组的亦或值也不会是0，那么由于鲍勃也是个机智的boy，他也不会移除一个使得剩余数组亦或值为0的数字，but，到了最后一个数字时，鲍勃别无选择只能移除最后一个数字，此时数组为0，亦或值为0，爱丽丝获胜。那此时你可能会有疑问，为啥奇数个数字且亦或值不为0时，爱丽丝一定会输？因为即便爱丽丝先移除掉了一个数字，使得数组亦或值仍不为0，那么此时鲍勃面对的情况就是偶数个数字使得数组亦或值不为0，这跟上面推论爱丽丝一定会赢的情况一样，鲍勃也是个聪明的蓝孩纸，所以爱丽丝会输，参见代码如下：



解法一：

```
class Solution {
public:
    bool xorGame(vector<int>& nums) {
        int x = 0, n = nums.size();
        for (int num : nums) x ^= num;
        return x == 0 || n % 2 == 0;
    }
};
```



下面这种解法就很秀了，比大军师大司马吴秀波还秀，直接用个accumulate一行搞定亦或值，博主只想吐槽这道题的难度级别，大家有见过一行解出一道Hard题吗，做梦都要笑醒了吧～



解法二：

```
class Solution {
public:
    bool xorGame(vector<int>& nums) {
        return nums.size() % 2 == 0 || !accumulate(nums.begin(), nums.end(), 0, bit_xor<int>());
    }
};
```



类似题目：

[Nim Game](http://www.cnblogs.com/grandyang/p/4873248.html)



参考资料：

[https://leetcode.com/problems/chalkboard-xor-game/solution/](https://leetcode.com/problems/chalkboard-xor-game/solution/)

[https://leetcode.com/problems/chalkboard-xor-game/discuss/133807/1-line-C++-solution](https://leetcode.com/problems/chalkboard-xor-game/discuss/133807/1-line-C++-solution)

[https://leetcode.com/problems/chalkboard-xor-game/discuss/121696/C++JavaPython-3-lines-Easy-Solution-with-Complaint-and-Explanation](https://leetcode.com/problems/chalkboard-xor-game/discuss/121696/C++JavaPython-3-lines-Easy-Solution-with-Complaint-and-Explanation)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












