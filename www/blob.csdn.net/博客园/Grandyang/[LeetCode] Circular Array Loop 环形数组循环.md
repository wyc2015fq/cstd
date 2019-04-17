# [LeetCode] Circular Array Loop 环形数组循环 - Grandyang - 博客园







# [[LeetCode] Circular Array Loop 环形数组循环](https://www.cnblogs.com/grandyang/p/7658128.html)







You are given an array of positive and negative integers. If a number n at an index is positive, then move forward n steps. Conversely, if it's negative (-n), move backward n steps. Assume the first element of the array is forward next to the last element, and the last element is backward next to the first element. Determine if there is a loop in this array. A loop starts and ends at a particular index with more than 1 element along the loop. The loop must be "forward" or "backward'.

Example 1: Given the array [2, -1, 1, 2, 2], there is a loop, from index 0 -> 2 -> 3 -> 0.

Example 2: Given the array [-1, 2], there is no loop.

Note: The given array is guaranteed to contain no element "0".

Can you do it in O(n) time complexity and O(1) space complexity?



说实话，这道题描述的并不是很清楚，比如题目中有句话说循环必须是forward或是backward的，如果不给例子说明，不太容易能get到point。所谓的循环必须是一个方向的就是说不能跳到一个数，再反方向跳回来，这不算一个loop。比如[1, -1]就不是一个loop，而[1, 1]是一个正确的loop。看到论坛中一半的帖子都是各种需要clarify和不理解test case就感觉很好笑～博主也成功踩坑了。弄清楚了题意后来考虑如何做，由于从一个位置只能跳到一个别的位置，而不是像图那样一个点可以到多个位置，所以这里我们就可以根据坐标建立一对一的映射，一旦某个达到的坐标已经有映射了，说明环存在，当然我们还需要进行一系列条件判断。首先我们需要一个visited数组，来记录访问过的数字，然后我们遍历原数组，如果当前数字已经访问过了，直接跳过，否则就以当前位置坐标为起始点开始查找，进行while循环，计算下一个位置，计算方法是当前位置坐标加上对应的数字，由于是循环数组，所以结果可能会超出数组的长度，所以我们要对数组长度取余。当然上面的数字也可能是负数，加完以后可能也是负数，所以在取余之前还得再补上一个n，使其变为正数。此时我们判断，如果next和cur相等，说明此时是一个数字的循环，不符合题意，再有就是检查二者的方向，数字是正数表示forward，若是负数表示backward，在一个loop中必须同正或同负，我们只要让二者相乘，如果结果是负数的话，说明方向不同，直接break掉。此时如果next已经有映射了，说明我们找到了合法的loop，返回true，否则建立一个这样的映射，将next位置在visited数组中标记true，继续循环，参见代码如下：



解法一：

```
class Solution {
public:
    bool circularArrayLoop(vector<int>& nums) {
        int n = nums.size();
        vector<bool> visited(n, false);
        for (int i = 0; i < n; ++i) {
            if (visited[i]) continue;
            visited[i] = true;
            unordered_map<int, int> m;
            int cur = i;
            while (true) {
                int next = (cur + nums[cur] + n) % n;
                if (next == cur || nums[next] * nums[cur] < 0) break;
                if (m.count(next)) return true;
                m[cur] = next;
                cur = next;
                visited[next] = true;
            }
        }
        return false;
    }
};
```



我们还可以简化上面的代码，可以不用visited数组，直接在nums中标记，由于题目中说了nums中不会有0，所以可以把访问过的位置标记为0。然后计算next位置，通过cur加上i，再加上n之后，对n取余。如果next和i相等，直接跳过，因为这表示循环只有一个数字，不合题意。然后开始循环，当cur和nums[next]的乘积为正时，说明此时是一个方向的，我们将cur赋值为nums[next]，将nums[next]赋值为0，表示已经访问过，然后再计算新的next位置。直到循环被打破，若此时next和i相等，说明有大于1个数字的环存在，返回true。最终for循环退出后，返回false，参见代码如下：



解法二：

```
class Solution {
public:
    bool circularArrayLoop(vector<int>& nums) {
        int n = nums.size();
        for (int i = 0; i < n; ++i) {
            if (nums[i] == 0) continue;
            int cur = nums[i];
            nums[i] = 0;
            int next = (cur + i + n) % n;
            if (next == i) continue;
            while (cur * nums[next] > 0) {
                cur = nums[next];
                nums[next] = 0;
                next = (next + cur + n) % n;
            }
            if (next == i) return true;
        }
        return false;
    }
};
```



参考资料：

[https://leetcode.com/problems/circular-array-loop/](https://leetcode.com/problems/circular-array-loop/)

[https://leetcode.com/problems/circular-array-loop/discuss/227401/java-Easy-to-understand-beat-100](https://leetcode.com/problems/circular-array-loop/discuss/227401/java-Easy-to-understand-beat-100)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












