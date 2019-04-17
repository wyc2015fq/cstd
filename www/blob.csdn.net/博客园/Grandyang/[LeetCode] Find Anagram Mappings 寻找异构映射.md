# [LeetCode] Find Anagram Mappings 寻找异构映射 - Grandyang - 博客园







# [[LeetCode] Find Anagram Mappings 寻找异构映射](https://www.cnblogs.com/grandyang/p/8570939.html)







Given two lists `A `and `B`, and `B` is an anagram of `A`. `B` is an anagram of `A` means `B` is made by randomizing the order of the elements in `A`.

We want to find an *index mapping*`P`, from `A` to `B`. A mapping `P[i] = j` means the `i`th element in `A` appears in `B` at index `j`.

These lists `A` and `B` may contain duplicates. If there are multiple answers, output any of them.

For example, given
A = [12, 28, 46, 32, 50]
B = [50, 12, 32, 46, 28]



We should return
[1, 4, 3, 2, 0]

as `P[0] = 1` because the `0`th element of `A` appears at `B[1]`, and `P[1] = 4` because the `1`st element of `A`appears at `B[4]`, and so on.



Note:
- `A, B` have equal lengths in range `[1, 100]`.
- `A[i], B[i]` are integers in range `[0, 10^5]`.



这道题给了我们两个数组A和B，说是A和B中的数字都相同，但是顺序不同，有点类似错位词的感觉。让我们找出数组A中的每个数字在数组B中的位置。这道题没有太大的难度，用个HashMap建立数组B中的每个数字和其位置之间的映射，然后遍历数组A，在HashMap中查找每个数字的位置即可，参见代码如下：



```
class Solution {
public:
    vector<int> anagramMappings(vector<int>& A, vector<int>& B) {
        vector<int> res;
        unordered_map<int, int> m;
        for (int i = 0; i < B.size(); ++i) m[B[i]] = i;
        for (int num : A) res.push_back(m[num]);
        return res;
    }
};
```



类似题目：

[Find All Anagrams in a String](http://www.cnblogs.com/grandyang/p/6014408.html)

[Valid Anagram](http://www.cnblogs.com/grandyang/p/4694988.html)

[Anagrams](http://www.cnblogs.com/grandyang/p/4385822.html)




[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)













