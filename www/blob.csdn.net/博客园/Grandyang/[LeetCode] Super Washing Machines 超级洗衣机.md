# [LeetCode] Super Washing Machines 超级洗衣机 - Grandyang - 博客园







# [[LeetCode] Super Washing Machines 超级洗衣机](https://www.cnblogs.com/grandyang/p/6648557.html)







You have n super washing machines on a line. Initially, each washing machine has some dresses or is empty.

For each move, you could choose any m (1 ≤ m ≤ n) washing machines, and pass one dress of each washing machine to one of its adjacent washing machines at the same time .

Given an integer array representing the number of dresses in each washing machine from left to right on the line, you should find the minimum number of moves to make all the washing machines have the same number of dresses. If it is not possible to do it, return -1.

Example1
Input: [1,0,5]

Output: 3

Explanation: 
1st move:    1     0 <-- 5    =>    1     1     4
2nd move:    1 <-- 1 <-- 4    =>    2     1     3    
3rd move:    2     1 <-- 3    =>    2     2     2   

Example2
Input: [0,3,0]

Output: 2

Explanation: 
1st move:    0 <-- 3     0    =>    1     2     0    
2nd move:    1     2 --> 0    =>    1     1     1     

Example3
Input: [0,2,0]

Output: -1

Explanation: 
It's impossible to make all the three washing machines have the same number of dresses. 



Note:
- The range of n is [1, 10000].
- The range of dresses number in a super washing machine is [0, 1e5].



这道题题给了我们一堆工藤新一，噢不，是滚筒洗衣机。我们有许多洗衣机，每个洗衣机里的衣服数不同，每个洗衣机每次只允许向相邻的洗衣机转移一件衣服，问要多少次才能使所有洗衣机的衣服数相等。注意这里的一次移动是说所有洗衣机都可以移动一件衣服到其相邻的洗衣机。这道题的代码量其实不多，难点是在于解题思路，难的是对问题的等价转换等。博主也没有做出这道题，博主想到了要先验证衣服总数是否能整除洗衣机的数量，然后计算出每个洗衣机最终应该放的衣服数，返回跟初始状态衣服数之差的最大值，但这种解法是不对的，无法通过这个test case [0, 0, 11, 5]，最终每个洗衣机会留4件衣服，我想的那方法会返回7，然后正确答案是8。想想也是，如果这么是这么简单的思路，这题怎么会标记为Hard呢，还是图样图森破啊。这里直接参照[大神Chidong的帖子](https://discuss.leetcode.com/topic/79938/super-short-easy-java-o-n-solution)来做，我们就用上面那个例子，有四个洗衣机，装的衣服数为[0, 0, 11, 5]，最终的状态会变为[4, 4, 4, 4]，那么我们将二者做差，得到[-4, -4, 7, 1]，这里负数表示当前洗衣机还需要的衣服数，正数表示当前洗衣机多余的衣服数。我们要做的是要将这个差值数组每一项都变为0，对于第一个洗衣机来说，需要四件衣服可以从第二个洗衣机获得，那么就可以把-4移给二号洗衣机，那么差值数组变为[0, -8, 7, 1]，此时二号洗衣机需要八件衣服，那么至少需要移动8次。然后二号洗衣机把这八件衣服从三号洗衣机处获得，那么差值数组变为[0, 0, -1, 1]，此时三号洗衣机还缺1件，就从四号洗衣机处获得，此时差值数组成功变为了[0, 0, 0, 0]，成功。那么移动的最大次数就是差值数组中出现的绝对值最大的数字，8次，参见代码如下：



```
class Solution {
public:
    int findMinMoves(vector<int>& machines) {
        int sum = accumulate(machines.begin(), machines.end(), 0);
        if (sum % machines.size() != 0) return -1;
        int res = 0, cnt = 0, avg = sum / machines.size();
        for (int m : machines) {
            cnt += m - avg;
            res = max(res, max(abs(cnt), m - avg));
        }
        return res;
    }
};
```



参考资料：

[https://discuss.leetcode.com/topic/79938/super-short-easy-java-o-n-solution](https://discuss.leetcode.com/topic/79938/super-short-easy-java-o-n-solution)

[https://discuss.leetcode.com/topic/79923/c-16ms-o-n-solution-with-trivial-proof](https://discuss.leetcode.com/topic/79923/c-16ms-o-n-solution-with-trivial-proof)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












