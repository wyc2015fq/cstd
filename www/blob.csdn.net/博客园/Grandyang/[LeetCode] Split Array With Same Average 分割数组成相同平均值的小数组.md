# [LeetCode] Split Array With Same Average 分割数组成相同平均值的小数组 - Grandyang - 博客园







# [[LeetCode] Split Array With Same Average 分割数组成相同平均值的小数组](https://www.cnblogs.com/grandyang/p/10285531.html)







In a given integer array A, we must move every element of A to either list B or list C. (B and C initially start empty.)

Return true if and only if after such a move, it is possible that the average value of B is equal to the average value of C, and B and C are both non-empty.
Example :
Input: 
[1,2,3,4,5,6,7,8]
Output: true
Explanation: We can split the array into [1,4,5,8] and [2,3,6,7], and both of them have the average of 4.5.

Note:
- The length of `A` will be in the range [1, 30].
- `A[i]` will be in the range of `[0, 10000]`.



这道题给了我们一个数组A，问我们是否可以把数组分割成两个小数组，并且要求分成的这两个数组的平均值相同。之前我们有做过分成和相同的两个小数组 [Split Array with Equal Sum](http://www.cnblogs.com/grandyang/p/6854492.html)，看了题目中的给的例子，你可能会有种错觉，觉得这两个问题是一样的，因为题目中分成的两个小数组的长度是一样的，那么平均值相同的话，和一定也是相同的。但其实是不对的，很简单的一个例子，比如数组 [2, 2, 2]，可以分成平均值相同的两个数组 [2, 2] 和 [2]，但是无法分成和相同的两个数组。 我们现在唯一知道的就是这两个数组的平均值相等，这里有个隐含条件，就是整个数组的平均值也和这两个数组的平均值相等，这个不用多说了吧，加法的结合律的应用啊。由于平均值是由数字总和除以个数得来的，那么假设整个数组有n个数组，数字总和为sum，分成的其中一个数组有k个，假设其数字和为sum1，那么另一个数组就有n-k个，假设其数组和为sum2，那么我们就有如下等式：

sum / n = sum1 / k = sum2 / (n - k)

我们看前两个等式，sum / n = sum1 / k，可以变个形，sum * k / n = sum1，那么由于数字都是正数，所以 sum * k 一定可以整除 n，这个可能当作一个快速的判断条件。下面来考虑k的取值范围，由于要分成两个数组，我们可以始终将k当作其中较短的那个数组，则k的取值范围就是 [1, n/2]，就是说，如果在这个范围内的k，没有满足的 sum * k % n == 0 的话，那么可以直接返回false，这是一个快速的剪枝过程。如果有的话，我们也不能立即说可以分成满足题意的两个小数组，最简单的例子，比如数组 [1, 3]，当k=1时，sum * k % n == 0 成立，但明显不能分成两个平均值相等的数组。所以还需要进一步检测，当我们找到满足的 sum * k % n == 0 的k了时候，我们其实可以直接算出 sum1，通过 sum * k / n，那么我们就知道较短的数组的数字之和，只要我们能在原数组中数组找到任意k个数字，使其和为sum1，那么就可以split成功了。问题到这里就转化为了如何在数组中找到任意k个数字，使其和为一个给定值。有点像 [Combination Sum III](http://www.cnblogs.com/grandyang/p/4537983.html) 那道题，我们当然可以根据不同的k值，都分别找原数组中找一遍，但我们想更高效一点，因为毕竟k的范围是固定的，我们可以事先任意选数组中k个数字，将其所有可能出现的数字和保存下来，最后再查找。那么为了去重复跟快速查找，我们可以使用HashSet来保存数字和，我们可以建立 n/2 + 1 个HashSet，多加1是为了不做数组下标的转换，并且防止越界，因为我们在累加的过程中，计算k的时候，需要用到k-1的情况。讲到这里，你会不会一拍大腿，吼道，这尼玛不就是动态规划Dynamic Programming么。恭喜你骚年，没错，这里我们的dp数组就是一个包含HashSet的数组，其中 dp[i] 表示数组中任选 i 个数字，所有可能的数字和。我们首先在 dp[0] 中加入一个0，这个是为了防止越界。我们更新 dp[i] 的思路是，对于dp[i-1] 中的每个数字，都加上一个新的数字，所以最外层的for循环是遍历原数组的中的每个数字的，中间的for循环是遍历k的，从n/2遍历到1，然后最内层的for循环是遍历dp[i-1]中的所有数组，加上最外层遍历到的数字，并存入dp[i]即可。整个dp数组更新好了之后，下面就是验证的环节了，对于每个k值，验证若 sum * k / n == 0 成立，并且 sum * i / n 在dp[i]中存在，则返回true。最后都没有成立的话，返回false，参见代码如下：



解法一：

```
class Solution {
public:
    bool splitArraySameAverage(vector<int>& A) {
        int n = A.size(), m = n / 2, sum = accumulate(A.begin(), A.end(), 0);
        bool possible = false;
        for (int i = 1; i <= m && !possible; ++i) {
            if (sum * i % n == 0) possible = true;
        }
        if (!possible) return false;
        vector<unordered_set<int>> dp(m + 1);
        dp[0].insert(0);
        for (int num : A) {
            for (int i = m; i >= 1; --i) {
                for (auto a : dp[i - 1]) {
                    dp[i].insert(a + num);
                }
            }
        }
        for (int i = 1; i <= m; ++i) {
            if (sum * i % n == 0 && dp[i].count(sum * i / n)) return true;
        }
        return false;
    }
};
```



下面这种解法跟上面的解法十分的相似，唯一的不同就是使用了bitset这个数据结构，在之前那道 [Partition Equal Subset Sum](http://www.cnblogs.com/grandyang/p/5951422.html) 的解法二中，我们也使用了biset，了解了其使用方法后，就会发现使用这里使用它只是单纯的为了炫技而已。由于biset不能动态变换大小，所以初始化的时候就要确定，由于题目中限定了数组中最多30个数字，每个数字最大10000，那么就初始化n/2+1个biset，每个大小为300001即可。然后每个都初始化个1进去，之后更新的操作，就是把 bits[i-1] 左移 num个，然后或到 bits[i]即可，最后查找的时候，有点像二维数组的查找方式一样，直接两个中括号坐标定位即可，参见代码如下：



解法二：

```
class Solution {
public:
    bool splitArraySameAverage(vector<int>& A) {
        int n = A.size(), m = n / 2, sum = accumulate(A.begin(), A.end(), 0);
        bool possible = false;
        for (int i = 1; i <= m && !possible; ++i) {
            if (sum * i % n == 0) possible = true;
        }
        if (!possible) return false;
        bitset<300001> bits[m + 1] = {1};
        for (int num : A) {
            for (int i = m; i >= 1; --i) {
                bits[i] |= bits[i - 1] << num;
            }
        }
        for (int i = 1; i <= m; ++i) {
            if (sum * i % n == 0 && bits[i][sum * i / n]) return true;
        }
        return false;
    }
};
```



再来看一种递归的写法，说实话在博主看来，一般不使用记忆数组的递归解法，等同于暴力破解，基本很难通过OJ，除非你进行了大量的剪枝优化处理。这里就是这种情况，首先还是常规的k值快速扫描一遍，确保可能存在解。然后我们给数组排了序，然后对于满足 sum * k % n == 0 的k值，进行了递归函数的进一步检测。需要传入当前剩余数字和，剩余个数，以及在原数组中的遍历位置，如果当前数字剩余个数为0了，说明我们已经取完了k个数字了，那么如果剩余数字和为0了，则说明成功的找到了k个和为 sum * k / n 的数字，返回ture，否则false。然后看若当前要加入的数字大于当前的平均值，则直接返回false，因为我们已经给原数组排过序了，之后的数字只会越来越大，一旦超过了平均值，就不可能再降下来了，这是一个相当重要的剪枝，估计能过OJ全靠它。之后我们开始从start开始遍历，当前遍历的结束位置是原数组长度n减去当前剩余的数字，再加1，因为我们确保给curNum留够足够的位置来遍历。之后就是跳过重复，对于重复的数字，我们只检查一遍就好了。调用递归函数，此时的curSum要减去当前数字A[i]，curNum要减1，start为i+1，若递归函数返回true，则整个返回true。for循环退出后返回false。令博主感到惊讶的是，这个代码的运行速度比之前的DP解法还要快，叼，参见代码如下：



解法三：

```
class Solution {
public:
    bool splitArraySameAverage(vector<int>& A) {
        int n = A.size(), m = n / 2, sum = accumulate(A.begin(), A.end(), 0);
        bool possible = false;
        for (int i = 1; i <= m && !possible; ++i) {
            if (sum * i % n == 0) possible = true;
        }
        if (!possible) return false;
        sort(A.begin(), A.end());
        for (int i = 1; i <= m; ++i) {
            if (sum * i % n == 0 && helper(A, sum * i / n, i, 0)) return true;
        }
        return false;
    }
    bool helper(vector<int>& A, int curSum, int curNum, int start) {
        if (curNum == 0) return curSum == 0;
        if (A[start] > curSum / curNum) return false;
        for (int i = start; i < A.size() - curNum + 1; ++i) {
            if (i > start && A[i] == A[i - 1]) continue;
            if(helper(A, curSum - A[i], curNum - 1, i + 1)) return true;
        }
        return false;
    }
};
```



类似题目：

[Combination Sum III](http://www.cnblogs.com/grandyang/p/4537983.html)

[Partition Equal Subset Sum](http://www.cnblogs.com/grandyang/p/5951422.html)

[Split Array with Equal Sum](http://www.cnblogs.com/grandyang/p/6854492.html)



参考资料：

[https://leetcode.com/problems/split-array-with-same-average/](https://leetcode.com/problems/split-array-with-same-average/)

[https://leetcode.com/problems/split-array-with-same-average/discuss/120660/Java-accepted-recursive-solution-with-explanation](https://leetcode.com/problems/split-array-with-same-average/discuss/120660/Java-accepted-recursive-solution-with-explanation)

[https://leetcode.com/problems/split-array-with-same-average/discuss/120842/DP-with-bitset-over-*sum*-(fast-PythonRuby-decent-C%2B%2B)](https://leetcode.com/problems/split-array-with-same-average/discuss/120842/DP-with-bitset-over-*sum*-(fast-PythonRuby-decent-C%2B%2B))

[https://leetcode.com/problems/split-array-with-same-average/discuss/120667/C%2B%2B-Solution-with-explanation-early-termination-(Updated-for-new-test-case)](https://leetcode.com/problems/split-array-with-same-average/discuss/120667/C%2B%2B-Solution-with-explanation-early-termination-(Updated-for-new-test-case))



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












