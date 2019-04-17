# [LeetCode] Reverse Pairs 翻转对 - Grandyang - 博客园







# [[LeetCode] Reverse Pairs 翻转对](https://www.cnblogs.com/grandyang/p/6657956.html)







Given an array `nums`, we call `(i, j)` an *important reverse pair* if `i < j` and `nums[i] > 2*nums[j]`.

You need to return the number of important reverse pairs in the given array.

Example1:
Input: [1,3,2,3,1]
Output: 2



Example2:
Input: [2,4,3,5,1]
Output: 3



Note:
- The length of the given array will not exceed `50,000`.
- All the numbers in the input array are in the range of 32-bit integer.



这道题第一次做的时候就感觉和[Count of Smaller Numbers After Self](http://www.cnblogs.com/grandyang/p/5078490.html)很像，开始也是用的二分查找法来建立有序数组，LintCode上也有一道非常类似的题[Reverse Pairs 翻转对](http://www.cnblogs.com/grandyang/p/5434414.html)，虽然那道题没有加2倍的限制条件，但是思路都是一样的。于是我就开始稍稍改了一下，二分搜索的时候是搜索nums[i]/2.0，加入数组的时候是加入原数num，我记得当时是可以通过OJ的，但是后来OJ变的严格起来了，什么二分搜索啊，BST啊之类的解法统统弄死，据目前来看，侥幸存活下来的方法就只有BIT和MergeSort这两种方法了。对于这类问题的本质的分析，[大神fun4LeetCode的帖子](https://discuss.leetcode.com/topic/79227/general-principles-behind-problems-similar-to-reverse-pairs)讲的非常好，这里也借鉴一下大神的讲解来帮助理解吧。

对于这类数组找数字之间的关系的题，一种很好的解题思路就是拆分数组来解决子问题，就是把大问题拆成小问题，把小问题一一解决来，大问题的答案也就出来了，这么说起来是不是有点像DP的感觉，但是不同的是，博主感觉此类问题很难找到DP的递推公式吧。fun4LeetCode大神归纳了两种拆分方法，一种叫顺序重现关系(Sequential Recurrence Relation)，用式子表示是T(i, j) = T(i, j - 1) + C。这里的C就是处理最后一个数字的子问题，那么用文字来描述就是“已知翻转对的第二个数字为nums[j], 在子数组nums[i, j - 1]中找翻转对的第一个数字”，这里翻转对的两个条件中的顺序条件已经满足，就只需要找比2*nums[j]大的数即可。当然最森破的方法就是线性扫描，但这样整个时间复杂度就会上升到令人发指的O(n2)，这又怎么能逃过连Binary Search都不放过的OJ的魔爪。由于二分搜索和BST等方法已经被OJ阉割，所以我们只能用树状数组Binary Indexed Tree来做了。关于BIT，我之前有篇博客[Range Sum Query - Mutable](http://www.cnblogs.com/grandyang/p/4985506.html) 应该讲的比较清楚了，如果弄懂了那篇博客，我们对BIT的机制也应该有个基本的了解，由于BIT的存储方式不是将数组中的数字对应的一一存入，而是有的对应存入，有的是存若干个数字之和，其设计初衷之一就是要在O(lgn)的时间复杂度下完成求和运算。那么我们该如何利用这一特性呢，这跟这道题又有什么关系呢，别着急，博主会慢慢解释。

首先我们应该确定一个遍历的方向，这里博主推荐从后往前遍历数组，这样做的好处是对于当前遍历到的数字，在已遍历过的数字中找小于当前数字的一半(nums[i]/2.0)的数字，这样的遍历方向也能跟上面的顺序重现关系的定义式统一起来。当然如果你想强行从前往后遍历，也不是不行，那么就需要在已遍历的数字中找大于当前数字的二倍(nums[i]*2)的数字就行了。由于我们要在之前遍历过的数字中找符合条件的数字，怎么样利用BIT的特性来快速的找到是这种解法的最大难点。我们需要将之前遍历过的数字存入BIT中，怎么存是难点。由于之前那篇博客我们知道BIT用update函数来存数，需要提供要存入的位置和要存入的数字这两个参数，那么这里难道我们就按照数字在原数组中的位置存入BIT吗，这样做毫无意义！我们要存的是该数字在有序数组中的位置，而且存入的也不是该数字本身，而是该数字出现的次数1。我们用题目中的第一个例子来说明，我们先给数组排序，得到：

1 1 2 3 3

对于每一个数字我们要确定其在BIT中的位置，由于有重复数字的存在，那么每个数字对应的位置就是其最后出现的位置，而且因为BIT是从1开始的，并不是像一般的数组那样从0开始，那么有如下对应关系：

1->2, 2->3, 3->5

那么当我们遇到数字1了，就update(2,1)，遇到数字2了，就update(3,1)，遇到数字3了，就update(5,1)。我们之前解释了并不把数字本身存入BIT，而是将其对应的位置存入BIT，真正存入的数字是1，这样方便累加，而且由于1是固定的，在下面的代码中就不用将1当作函数的参数了。这样我们知道了如果存入数字，那么我们在遍历到新数字时，为了得到符合要求的数字的个数，需利用BIT的getSum函数。getSum函数需要提供一个位置参数，可以返回该位置之前的所有数之和。同理，我们提供的参数既不是当前遍历到的数字本身，也不是其在原数组中的位置，而是该数字的一半(nums[i]/2.0)在有序数组中的正确位置，可以用lower_bound函数来找第一个不小于目标值的位置，当然我们也可以自己写个二分搜索的子函数来代替lower_bound函数。比如我们当前遍历到的数字是3，那么我们在有序数组中找1.5的位置，返回是2，此时我们在BIT中用getSum来返回位置2之前的数字之和，返回几就表示有几个小于1.5的数字。讲到这里基本上这种解法的核心内容都讲完了，如果你还是一头雾水，那么就是博主的表述能力的问题了(沮丧脸:()。那么博主只能建议你带实例一步一步去试，看看每一步操作后BIT中的结果是啥，下面就列出这些内容：

update(2,1) -> BIT: 0 0 1 0 1 0

update(5,1) -> BIT: 0 0 1 0 1 1

update(3,1) -> BIT: 0 0 1 1 2 1

update(5,1) -> BIT: 0 0 1 1 2 2

update(2,1) -> BIT: 0 0 2 1 3 2



解法一：

```
class Solution {
public:
    int reversePairs(vector<int>& nums) {
        int res = 0, n = nums.size();
        vector<int> v = nums, bit(n + 1);
        sort(v.begin(), v.end());
        unordered_map<int, int> m;
        for (int i = 0; i < n; ++i) m[v[i]] = i + 1;
        for (int i = n - 1; i >= 0; --i) {
            res += getSum(lower_bound(v.begin(), v.end(), nums[i] / 2.0) - v.begin(), bit);
            update(m[nums[i]], bit);
        }
        return res;
    }
    int getSum(int i, vector<int>& bit) {
        int sum = 0;
        while (i > 0) {
            sum += bit[i];
            i -= (i & -i);
        }
        return sum;
    }
    void update(int i, vector<int>& bit) {
        while (i < bit.size()) {
            bit[i] += 1;
            i += (i & -i);
        }
    }
};
```



fun4LeetCode大神归纳的第二种方法叫做分割重现关系(Partition Recurrence Relation)，用式子表示是T(i, j) = T(i, m) + T(m+1, j) + C。这里的C就是处理合并两个部分的子问题，那么用文字来描述就是“已知翻转对的两个数字分别在子数组nums[i, m]和nums[m+1, j]之中，求满足要求的翻转对的个数”，这里翻转对的两个条件中的顺序条件已经满足，就只需要找到满足大小关系的的数对即可。这里两个数字都是不确定的，如果用暴力搜索肯定会被OJ唾弃。但是如果两个子数组是有序的，那么我们可以用双指针的方法在线性时间内就可以统计出符合题意的翻转对的个数。要想办法产生有序的子数组，那么这就和MergeSort的核心思想完美匹配了。我们知道混合排序就是不断的将数组对半拆分成子数组，拆到最小的数组后开始排序，然后一层一层的返回，最后原数组也是有序的了。这里我们在混合排序的递归函数中，对于有序的两个子数组进行统计翻转对的个数，然后再逐层返回，这就完美的实现了上述的分割重现关系的思想。整个的写法非常的简洁，实在是太叼了。博主的直觉表明，fun4LeetCode大神肯定是国人，不要问我为什么，因为这么强的肯定是中国人，哈～



解法二：

```
class Solution {
public:
    int reversePairs(vector<int>& nums) {
        return mergeSort(nums, 0, nums.size() - 1);
    }
    int mergeSort(vector<int>& nums, int left, int right) {
        if (left >= right) return 0;
        int mid = left + (right - left) / 2;
        int res = mergeSort(nums, left, mid) + mergeSort(nums, mid + 1, right);
        for (int i = left, j = mid + 1; i <= mid; ++i) {
            while (j <= right && nums[i] / 2.0 > nums[j]) ++j;
            res += j - (mid + 1);
        }
        sort(nums.begin() + left, nums.begin() + right + 1);
        return res;
    }
};
```



类似题目：

[Count of Range Sum](http://www.cnblogs.com/grandyang/p/5162678.html)

[Count of Smaller Numbers After Self](http://www.cnblogs.com/grandyang/p/5078490.html)



参考资料：

[https://discuss.leetcode.com/topic/78933/very-short-and-clear-mergesort-bst-java-solutions](https://discuss.leetcode.com/topic/78933/very-short-and-clear-mergesort-bst-java-solutions)

[https://discuss.leetcode.com/topic/79227/general-principles-behind-problems-similar-to-reverse-pairs](https://discuss.leetcode.com/topic/79227/general-principles-behind-problems-similar-to-reverse-pairs)

[https://discuss.leetcode.com/topic/79894/evolve-from-brute-force-to-optimal-a-review-of-all-solutions](https://discuss.leetcode.com/topic/79894/evolve-from-brute-force-to-optimal-a-review-of-all-solutions)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












