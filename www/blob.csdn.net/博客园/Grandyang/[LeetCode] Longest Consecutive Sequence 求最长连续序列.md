# [LeetCode] Longest Consecutive Sequence 求最长连续序列 - Grandyang - 博客园







# [[LeetCode] Longest Consecutive Sequence 求最长连续序列](https://www.cnblogs.com/grandyang/p/4276225.html)







Given an unsorted array of integers, find the length of the longest consecutive elements sequence.

Your algorithm should run in O(*n*) complexity.

Example:
Input: [100, 4, 200, 1, 3, 2]
Output: 4
Explanation: The longest consecutive elements sequence is `[1, 2, 3, 4]`. Therefore its length is 4.


这道题要求求最长连续序列，并给定了O(n)复杂度限制，我们的思路是，使用一个集合HashSet存入所有的数字，然后遍历数组中的每个数字，如果其在集合中存在，那么将其移除，然后分别用两个变量pre和next算出其前一个数跟后一个数，然后在集合中循环查找，如果pre在集合中，那么将pre移除集合，然后pre再自减1，直至pre不在集合之中，对next采用同样的方法，那么next-pre-1就是当前数字的最长连续序列，更新res即可。这里再说下，为啥当检测某数字在集合中存在当时候，都要移除数字。这是为了避免大量的重复计算，就拿题目中的例子来说吧，我们在遍历到4的时候，会向下遍历3，2，1，如果都不移除数字的话，遍历到1的时候，还会遍历2，3，4。同样，遍历到3的时候，向上遍历4，向下遍历2，1，等等等。如果数组中有大量的连续数字的话，那么就有大量的重复计算，十分的不高效，所以我们要从HashSet中移除数字，代码如下：



C++ 解法一：

```
class Solution {
public:
    int longestConsecutive(vector<int>& nums) {
        int res = 0;
        unordered_set<int> s(nums.begin(), nums.end());
        for (int val : nums) {
            if (!s.count(val)) continue;
            s.erase(val);
            int pre = val - 1, next = val + 1;
            while (s.count(pre)) s.erase(pre--);
            while (s.count(next)) s.erase(next++);
            res = max(res, next - pre - 1);
        }
        return res;
    }
};
```



Java 解法一：

```
public class Solution {
    public int longestConsecutive(int[] nums) {
        int res = 0;
        Set<Integer> s = new HashSet<Integer>();
        for (int num : nums) s.add(num);
        for (int num : nums) {
            if (s.remove(num)) {
                int pre = num - 1, next = num + 1;
                while (s.remove(pre)) --pre;
                while (s.remove(next)) ++next;
                res = Math.max(res, next - pre - 1);
            }
        }
        return res;
    }
}
```



我们也可以采用哈希表来做，刚开始HashMap为空，然后遍历所有数字，如果该数字不在HashMap中，那么我们分别看其左右两个数字是否在HashMap中，如果在，则返回其哈希表中映射值，若不在，则返回0，虽然我们直接从HashMap中取不存在的映射值，也能取到0，但是一旦去取了，就会自动生成一个为0的映射，那么我们这里再for循环的开头判断如果存在映射就跳过的话，就会出错。然后我们将left+right+1作为当前数字的映射，并更新res结果，同时更新num-left和num-right的映射值。

下面来解释一下为啥要判断如何存在映射的时候要跳过，这是因为一旦某个数字创建映射了，说明该数字已经被处理过了，那么其周围的数字很可能也已经建立好了映射了，如果再遇到之前处理过的数字，再取相邻数字的映射值累加的话，会出错。举个例子，比如数组 [1, 2, 0, 1]，当0执行完以后，HashMap中的映射为 {1->2, 2->3, 0->3}，可以看出此时0和2的映射值都已经为3了，那么如果最后一个1还按照原来的方法处理，随后得到结果就是7，明显不合题意。还有就是，之前说的，为了避免访问不存在的映射值时，自动创建映射，我们使用m.count() 先来检测一下，只有存在映射，我们才从中取值，否则就直接赋值为0，参见代码如下：



C++ 解法二：

```
class Solution {
public:
    int longestConsecutive(vector<int>& nums) {
        int res = 0;
        unordered_map<int, int> m;
        for (int num : nums) {
            if (m.count(num)) continue;
            int left = m.count(num - 1) ? m[num - 1] : 0;
            int right = m.count(num + 1) ? m[num + 1] : 0;
            int sum = left + right + 1;
            m[num] = sum;
            res = max(res, sum);
            m[num - left] = sum;
            m[num + right] = sum;
        }
        return res;
    }
};
```



Java 解法二：

```
public class Solution {
    public int longestConsecutive(int[] nums) {
        int res = 0;
        Map<Integer, Integer> m = new HashMap<Integer, Integer>();
        for (int num : nums) {
            if (m.containsKey(num)) continue;
            int left = m.containsKey(num - 1) ? m.get(num - 1) : 0;
            int right = m.containsKey(num + 1) ? m.get(num + 1) : 0;
            int sum = left + right + 1;
            m.put(num, sum);
            res = Math.max(res, sum);
            m.put(num - left, sum);
            m.put(num + right, sum);
        }
        return res;
    }
}
```



类似题目：

[Binary Tree Longest Consecutive Sequence](http://www.cnblogs.com/grandyang/p/5252599.html)



参考资料：

[https://leetcode.com/problems/longest-consecutive-sequence/](https://leetcode.com/problems/longest-consecutive-sequence/)

[https://leetcode.com/problems/longest-consecutive-sequence/discuss/41055/my-really-simple-java-on-solution-accepted](https://leetcode.com/problems/longest-consecutive-sequence/discuss/41055/my-really-simple-java-on-solution-accepted)

[https://leetcode.com/problems/longest-consecutive-sequence/discuss/41060/a-simple-csolution-using-unordered_setand-simple-consideration-about-this-problem](https://leetcode.com/problems/longest-consecutive-sequence/discuss/41060/a-simple-csolution-using-unordered_setand-simple-consideration-about-this-problem)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












