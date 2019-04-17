# [LeetCode] Couples Holding Hands 两两握手 - Grandyang - 博客园







# [[LeetCode] Couples Holding Hands 两两握手](https://www.cnblogs.com/grandyang/p/8716597.html)







N couples sit in 2N seats arranged in a row and want to hold hands. We want to know the minimum number of swaps so that every couple is sitting side by side. A *swap* consists of choosing any two people, then they stand up and switch seats.

The people and seats are represented by an integer from `0` to `2N-1`, the couples are numbered in order, the first couple being `(0, 1)`, the second couple being `(2, 3)`, and so on with the last couple being `(2N-2, 2N-1)`.

The couples' initial seating is given by `row[i]` being the value of the person who is initially sitting in the i-th seat.

Example 1:
Input: row = [0, 2, 1, 3]
Output: 1
Explanation: We only need to swap the second (row[1]) and third (row[2]) person.



Example 2:
Input: row = [3, 2, 0, 1]
Output: 0
Explanation: All couples are already seated side by side.



Note:
- `len(row)` is even and in the range of `[4, 60]`.
- `row` is guaranteed to be a permutation of `0...len(row)-1`.



这道题给了我们一个长度为n的数组，里面包含的数字是 [0, n-1] 范围内的数字各一个，让我们通过调换任意两个数字的位置，使得相邻的奇偶数靠在一起。因为要两两成对，所以题目限定了输入数组必须是偶数个。我们要明确的是，组成对儿的两个是从0开始，每两个一对儿的。比如0和1，2和3，像1和2就不行。而且检测的时候也是两个数两个数的检测，左右顺序无所谓，比如2和3，或者3和2都行。当我们暂时对如何用代码来解决问题没啥头绪的时候，一个很好的办法是，先手动解决问题，意思是，假设这道题不要求你写代码，就让你按照要求排好序怎么做。我们随便举个例子来说吧，比如：

[3   1   4   0   2   5]

我们如何将其重新排序呢？首先明确，我们交换数字位置的动机是要凑对儿，如果我们交换的两个数字无法组成新对儿，那么这个交换就毫无意义。来手动交换吧，我们两个两个的来看数字，前两个数是3和1，我们知道其不成对儿，数字3的老相好是2，不是1，那么怎么办呢？我们就把1和2交换位置呗。好，那么现在3和2牵手成功，度假去了，再来看后面的：

[3   2   4   0   1   5]

我们再取两数字，4和0，互不认识！4跟5有一腿儿，不是0，那么就把0和5，交换一下吧，得到：

[3   2   4   5   1   0]

好了，再取最后两个数字，1和0，两口子，不用动！前面都成对的话，最后两个数字一定成对。而且这种方法所用的交换次数一定是最少的，不要问博主怎么证明，博主也不会|||-.-～明眼人应该已经看出来了，这就是一种贪婪算法Greedy Algorithm。思路有了，代码就很容易写了，注意这里在找老伴儿时用了一个trick，一个数‘异或’上1就是其另一个位，这个不难理解，如果是偶数的话，最后位是0，‘异或’上1等于加了1，变成了可以的成对奇数。如果是奇数的话，最后位是1，‘异或’上1后变为了0，变成了可以的成对偶数。参见代码如下：



解法一：

```
class Solution {
public:
    int minSwapsCouples(vector<int>& row) {
        int res = 0, n = row.size();
        for (int i = 0; i < n; i += 2) {
            if (row[i + 1] == (row[i] ^ 1)) continue;
            ++res;
            for (int j = i + 1; j < n; ++j) {
                if (row[j] == (row[i] ^ 1)) {
                    row[j] = row[i + 1];
                    row[i + 1] = row[i] ^ 1;
                    break;
                }
            }
        }
        return res;
    }
};
```



下面我们来看一种使用联合查找Union Find的解法。该解法对于处理群组问题时非常有效，比如岛屿数量有关的题就经常使用UF解法。核心思想是用一个root数组，每个点开始初始化为不同的值，如果两个点属于相同的组，就将其中一个点的root值赋值为另一个点的位置，这样只要是相同组里的两点，通过find函数会得到相同的值。 那么如果总共有n个数字，则共有 n/2 对儿，所以我们初始化 n/2 个群组，我们还是每次处理两个数字。每个数字除以2就是其群组号，那么属于同一组的两个数的群组号是相同的，比如2和3，其分别除以2均得到1，所以其组号均为1。那么这对解题有啥作用呢？作用忒大了，由于我们每次取的是两个数，且计算其群组号，并调用find函数，那么如果这两个数的群组号相同，那么find函数必然会返回同样的值，我们不用做什么额外动作，因为本身就是一对儿。如果两个数不是一对儿，那么其群组号必然不同，在二者没有归为一组之前，调用find函数返回的值就不同，此时我们将二者归为一组，并且cnt自减1，忘说了，cnt初始化为总群组数，即 n/2。那么最终cnt减少的个数就是交换的步数，还是用上面讲解中的例子来说明吧：

[3   1   4   0   2   5]

最开始的群组关系是：

群组0：0，1

群组1：2，3

群组2：4，5

取出前两个数字3和1，其群组号分别为1和0，带入find函数返回不同值，则此时将群组0和群组1链接起来，变成一个群组，则此时只有两个群组了，cnt自减1，变为了2。

群组0 & 1：0，1，2，3

群组2：4，5

此时取出4和0，其群组号分别为2和0，带入find函数返回不同值，则此时将群组0&1和群组2链接起来，变成一个超大群组，cnt自减1，变为了1。

群组0 & 1 & 2：0，1，2，3，4，5

此时取出最后两个数2和5，其群组号分别为1和2，因为此时都是一个大组内的了，带入find函数返回相同的值，不做任何处理。最终交换的步数就是cnt减少值，为2，参见代码如下：



解法二：

```
class Solution {
public:
    int minSwapsCouples(vector<int>& row) {
        int res = 0, n = row.size(), cnt = n / 2;
        vector<int> root(n, 0);
        for (int i = 0; i < n; ++i) root[i] = i;
        for (int i = 0; i < n; i += 2) {
            int x = find(root, row[i] / 2);
            int y = find(root, row[i + 1] / 2);
            if (x != y) {
                root[x] = y;
                --cnt;
            }
        }
        return n / 2 - cnt;
    }
    int find(vector<int>& root, int i) {
        return (i == root[i]) ? i : find(root, root[i]);
    }
};
```



下面这种使用HashMap的解法，本质其实也是联合查找Union Find。我们知道只有群组里面是数字，才能使用root数组，有些非数字的情况，比如字符串，就要使用HashMap了，当然数字也是可以使用HashMap的。我们这里的helper子函数相当于同时包括了链接群组和find查找两部分，在主函数中，我们还是两个两个处理，并且把群组号带入helper函数，在helper函数中，我们将较小数和较大数区分出来，如果二者相同，表明是同一个群组的，不做任何处理，直接返回。否则的话，建立二者的映射，这就是上面解法中的链接群组操作，这样看出来了吧，二者的本质其实是一样的，参见代码如下：



解法三：

```
class Solution {
public:
    int minSwapsCouples(vector<int>& row) {
        unordered_map<int, int> m;
        for (int i = 0; i < row.size(); i += 2) {
            helper(m, row[i] / 2, row[i + 1] / 2);
        }
        return m.size();
    }
    void helper(unordered_map<int, int>& m, int x, int y) {
        int c1 = min(x, y), c2 = max(x, y);
        if (c1 == c2) return;
        if (m.count(c1)) helper(m, m[c1], c2);
        else m[c1] = c2;
    }
};
```



这道题的一个Follow up就是[fun4LeetCode大神的帖子](https://leetcode.com/problems/couples-holding-hands/discuss/113362/JavaC++-O(N)-solution-using-cyclic-swapping)中讨论的N整数问题 N Integers Problems，简单来说就是最少使用几步可以将所有的数字移回其正确位置，比如数组 [0 3 1 2] 变回 [0 1 2 3] 需要几步，两步就够了，先交换3和2，变成 [0 2 1 3]，再交换2和1，变回 [0 1 2 3]。怎么做呢？实际上在遍历某一个位置i，如果发现 i != rows[i]，我们就不同的通过交换i和rows[i]，然后让 row[i] 等于 row[row[i]]，使其最终相等，是不是也有点Union Find的影子在里面呢？真是很有趣呢～面白以~



类似题目：

[Missing Number](http://www.cnblogs.com/grandyang/p/4756677.html)

[First Missing Positive](http://www.cnblogs.com/grandyang/p/4395963.html)



参考资料：

[https://leetcode.com/problems/couples-holding-hands/solution/](https://leetcode.com/problems/couples-holding-hands/solution/)

[https://leetcode.com/problems/couples-holding-hands/discuss/113353/Monster-Style-C++-O(n)-unordered_map](https://leetcode.com/problems/couples-holding-hands/discuss/113353/Monster-Style-C++-O(n)-unordered_map)

[https://leetcode.com/problems/couples-holding-hands/discuss/113362/JavaC++-O(N)-solution-using-cyclic-swapping](https://leetcode.com/problems/couples-holding-hands/discuss/113362/JavaC++-O(N)-solution-using-cyclic-swapping)

[https://leetcode.com/problems/couples-holding-hands/discuss/117520/Java-union-find-easy-to-understand-5-ms](https://leetcode.com/problems/couples-holding-hands/discuss/117520/Java-union-find-easy-to-understand-5-ms)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












