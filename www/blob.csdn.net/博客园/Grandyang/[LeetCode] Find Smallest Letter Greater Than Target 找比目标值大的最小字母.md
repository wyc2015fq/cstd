# [LeetCode] Find Smallest Letter Greater Than Target 找比目标值大的最小字母 - Grandyang - 博客园







# [[LeetCode] Find Smallest Letter Greater Than Target 找比目标值大的最小字母](https://www.cnblogs.com/grandyang/p/8284940.html)







Given a list of sorted characters `letters` containing only lowercase letters, and given a target letter `target`, find the smallest element in the list that is larger than the given target.

Letters also wrap around. For example, if the target is `target = 'z'` and `letters = ['a', 'b']`, the answer is `'a'`.

Examples:
Input:
letters = ["c", "f", "j"]
target = "a"
Output: "c"

Input:
letters = ["c", "f", "j"]
target = "c"
Output: "f"

Input:
letters = ["c", "f", "j"]
target = "d"
Output: "f"

Input:
letters = ["c", "f", "j"]
target = "g"
Output: "j"

Input:
letters = ["c", "f", "j"]
target = "j"
Output: "c"

Input:
letters = ["c", "f", "j"]
target = "k"
Output: "c"



Note:
- `letters` has a length in range `[2, 10000]`.
- `letters` consists of lowercase letters, and contains at least 2 unique letters.
- `target` is a lowercase letter.



这道题给了我们一堆有序的字母，然后又给了我们一个target字母，让我们求字母数组中第一个大于target的字母，数组是循环的，如果没有，那就返回第一个字母。像这种在有序数组中找数字，二分法简直不要太适合啊。题目中说了数组至少有两个元素，那么我们首先用数组的尾元素来跟target比较，如果target大于等于尾元素的话，直接返回数组的首元素即可。否则就利用二分法来做，这里是查找第一个大于目标值的数组，博主之前做过二分法的总结，参见这个帖子[LeetCode Binary Search Summary 二分搜索法小结](http://www.cnblogs.com/grandyang/p/6854825.html)，参见代码如下：



解法一：

```
class Solution {
public:
    char nextGreatestLetter(vector<char>& letters, char target) {
        if (target >= letters.back()) return letters[0];
        int n = letters.size(), left = 0, right = n;
        while (left < right) {
            int mid = left + (right - left) / 2;
            if (letters[mid] <= target) left = mid + 1;
            else right = mid;
        }
        return letters[right];
    }
};
```



我们也可以用STL自带的upper_bound函数来做，这个就是找第一个大于目标值的数字，如果返回end()，说明没找到，返回首元素即可，参见代码如下：



解法二：

```
class Solution {
public:
    char nextGreatestLetter(vector<char>& letters, char target) {
        auto it = upper_bound(letters.begin(), letters.end(), target);
        return it == letters.end() ? *letters.begin() : *it;
    }
};
```



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












