# [LeetCode] 24 Game 二十四点游戏 - Grandyang - 博客园







# [[LeetCode] 24 Game 二十四点游戏](https://www.cnblogs.com/grandyang/p/8395062.html)








You have 4 cards each containing a number from 1 to 9. You need to judge whether they could operated through `*`, `/`, `+`, `-`, `(`, `)`to get the value of 24.

Example 1:
Input: [4, 1, 8, 7]
Output: True
Explanation: (8-4) * (7-1) = 24



Example 2:
Input: [1, 2, 1, 2]
Output: False



Note:
- The division operator `/` represents real division, not integer division. For example, 4 / (1 - 2/3) = 12.
- Every operation done is between two numbers. In particular, we cannot use `-` as a unary operator. For example, with `[1, 1, 1, 1]` as input, the expression `-1 - 1 - 1 - 1` is not allowed.
- You cannot concatenate numbers together. For example, if the input is `[1, 2, 1, 2]`, we cannot write this as 12 + 12.



这道题就是经典的24点游戏了，记得小时候经常玩这个游戏，就是每个人发四张牌，看谁最快能算出24，这完全是脑力大比拼啊，不是拼的牌技。玩的多了，就会摸出一些套路来，比如尽量去凑2和12，3和8，4和6等等，但是对于一些特殊的case，比如 [1, 5, 5, 5] 这种，正确的解法是 5 * (5 - 1 / 5)，一般人都会去试加减乘，和能整除的除法，而像这种带小数的确实很难想到，但是程序计算就没问题，可以遍历所有的情况，这也是这道题的实际意义所在吧。那么既然是要遍历所有的情况，我们应该隐约感觉到应该是要使用递归来做的。我们想，任意的两个数字之间都可能进行加减乘除，其中加法和乘法对于两个数字的前后顺序没有影响，但是减法和除法是有影响的，而且做除法的时候还要另外保证除数不能为零。我们要遍历任意两个数字，然后对于这两个数字，尝试各种加减乘除后得到一个新数字，将这个新数字加到原数组中，记得原来的两个数要移除掉，然后调用递归函数进行计算，我们可以发现每次调用递归函数后，数组都减少一个数字，那么当减少到只剩一个数字了，就是最后的计算结果，所以我们在递归函数开始时判断，如果数组只有一个数字，且为24，说明可以算出24，结果res赋值为true返回。这里我们的结果res是一个全局的变量，如果已经为true了，就没必要再进行运算了，所以第一行应该是判断结果res，为true就直接返回了。我们遍历任意两个数字，分别用p和q来取出，然后进行两者的各种加减乘除的运算，将结果保存进数组临时数组t，记得要判断除数不为零。然后将原数组nums中的p和q移除，遍历临时数组t中的数字，将其加入数组nums，然后调用递归函数，记得完成后要移除数字，恢复状态，这是递归解法很重要的一点。最后还要把p和q再加回原数组nums，这也是还原状态，参见代码如下：



解法一：

```
class Solution {
public:
    bool judgePoint24(vector<int>& nums) {
        bool res = false;
        double eps = 0.001;
        vector<double> arr(nums.begin(), nums.end());
        helper(arr, eps, res);
        return res;
    }
    void helper(vector<double>& nums, double eps, bool& res) {
        if (res) return;
        if (nums.size() == 1) {
            if (abs(nums[0] - 24) < eps) res = true;
            return;
        }
        for (int i = 0; i < nums.size(); ++i) {
            for (int j = 0; j < i; ++j) {
                double p = nums[i], q = nums[j];
                vector<double> t{p + q, p - q, q - p, p * q};
                if (p > eps) t.push_back(q / p);
                if (q > eps) t.push_back(p / q);
                nums.erase(nums.begin() + i);
                nums.erase(nums.begin() + j);
                for (double d : t) {
                    nums.push_back(d);
                    helper(nums, eps, res);
                    nums.pop_back();
                }
                nums.insert(nums.begin() + j, q);
                nums.insert(nums.begin() + i, p);
            }
        }
    }
};
```



来看一种很不同的递归写法，这里将加减乘除操作符放到了一个数组ops中。并且没有用全局变量res，而是让递归函数带有bool型返回值。在递归函数中，还是要先看nums数组的长度，如果为1了，说明已经计算完成，直接看结果是否为0就行了。然后遍历任意两个数字，注意这里的i和j都分别从0到了数组长度，而上面解法的j是从0到i，这是因为上面解法将p - q, q - p, q / q, q / p都分别列出来了，而这里仅仅是nums[i] - nums[j], nums[i] / nums[j]，所以i和j要交换位置，但是为了避免加法和乘法的重复计算，我们可以做个判断，还有别忘记了除数不为零的判断，i和j不能相同的判断。我们建立一个临时数组t，将非i和j位置的数字都加入t，然后遍历操作符数组ops，每次取出一个操作符，然后将nums[i]和nums[j]的计算结果加入t，调用递归函数，如果递归函数返回true了，那么就直接返回true。否则移除刚加入的结果，还原t的状态，参见代码如下：



解法二：

```
class Solution {
public:
    bool judgePoint24(vector<int>& nums) {
        double eps = 0.001;
        vector<char> ops{'+', '-', '*', '/'};
        vector<double> arr(nums.begin(), nums.end());
        return helper(arr, ops, eps);
    }
    bool helper(vector<double>& nums, vector<char>& ops, double eps) {
        if (nums.size() == 1) return abs(nums[0] - 24) < eps;
        for (int i = 0; i < nums.size(); ++i) {
            for (int j = 0; j < nums.size(); ++j) {
                if (i == j) continue;
                vector<double> t;
                for (int k = 0; k < nums.size(); ++k) {
                    if (k != i && k != j) t.push_back(nums[k]);
                }
                for (char op : ops) {
                    if ((op == '+' || op == '*') && i > j) continue;
                    if (op == '/' && nums[j] < eps) continue;
                    switch(op) {
                        case '+': t.push_back(nums[i] + nums[j]); break;
                        case '-': t.push_back(nums[i] - nums[j]); break;
                        case '*': t.push_back(nums[i] * nums[j]); break;
                        case '/': t.push_back(nums[i] / nums[j]); break;
                    }
                    if (helper(t, ops, eps)) return true;
                    t.pop_back();
                }
            }
        }
        return false;
    }
};
```



讨论：博主在调试的时候，遇到了这个test case: [1, 3, 4, 6]，返回的是true，但是博主心算了一会，并没有想出其是如何算出24的。所以博主在想，能不能修改下代码，使得其能将运算的过程返回出来。其实并不难改，基于解法二来改一下，我们发现，计算后的结果被存入了临时数组t，进行下一次递归，我们需要将这个过程保存下来，用一个字符串数组，比如"1+3"，或者"1-3"等等，这个数组跟数组t大小相同，操作基本相同，同时需要被传入到下一次递归函数中，而在下一次递归函数中，数组t中取出的就是4和-2，但是字符串数组就可以取出"1+3"和"1-3"，我们就可以继续和别的数进行计算了，比如要乘以4，我们需要给取出的字符串加上括号，就变成了(1+3)*4了，就通过这种方法就可以将过程返回了，运行test case: [1, 3, 4, 6]，返回得到：

(6/(1-(3/4)))

没有问题，还有就是，由于组成24的方法可能不止1种，我们可以将所有情况都返回，那么我们的递归函数就不要有返回值，这样可以遍历完所有的情况，对于test case: [1, 2, 3, 8]，返回如下：

((8-2)*(1+3))
(8/(1-(2/3)))
(3/((2-1)/8))
(3*(8/(2-1)))
(8*(3*(2-1)))
(2*(1+(3+8)))
(8/((2-1)/3))
(3*(8*(2-1)))
((1+3)*(8-2))
((3*8)/(2-1))
(8*(3/(2-1)))
((3*8)*(2-1))
(2*(8+(1+3)))
((2-1)*(3*8))
(2*(3+(1+8)))

被惊到了有木有！居然有这么多种计算方法可以得到24～ (修改后的代码贴在了评论区二楼 ：)



参考资料：

[https://leetcode.com/problems/24-game/](https://leetcode.com/problems/24-game/)

[https://leetcode.com/problems/24-game/discuss/107685/679-24-game-c-recursive](https://leetcode.com/problems/24-game/discuss/107685/679-24-game-c-recursive)

[https://leetcode.com/problems/24-game/discuss/107673/java-easy-to-understand-backtracking](https://leetcode.com/problems/24-game/discuss/107673/java-easy-to-understand-backtracking)[](https://discuss.leetcode.com/topic/104042/679-24-game-c-recursive)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













