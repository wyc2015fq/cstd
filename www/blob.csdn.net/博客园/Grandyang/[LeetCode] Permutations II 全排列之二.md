# [LeetCode] Permutations II 全排列之二 - Grandyang - 博客园







# [[LeetCode] Permutations II 全排列之二](https://www.cnblogs.com/grandyang/p/4359825.html)







Given a collection of numbers that might contain duplicates, return all possible unique permutations.

Example:
Input: [1,1,2]
Output:
[
  [1,1,2],
  [1,2,1],
  [2,1,1]
]


这道题是之前那道 [Permutations](http://www.cnblogs.com/grandyang/p/4358848.html) 的延伸，由于输入数组有可能出现重复数字，如果按照之前的算法运算，会有重复排列产生，我们要避免重复的产生，在递归函数中要判断前面一个数和当前的数是否相等，如果相等，且其对应的visited中的值为1，当前的数字才能使用（下文中会解释这样做的原因），否则需要跳过，这样就不会产生重复排列了，代码如下：



解法一：

```
class Solution {
public:
    vector<vector<int>> permuteUnique(vector<int>& nums) {
        vector<vector<int>> res;
        vector<int> out, visited(nums.size(), 0);
        sort(nums.begin(), nums.end());
        permuteUniqueDFS(nums, 0, visited, out, res);
        return res;
    }
    void permuteUniqueDFS(vector<int>& nums, int level, vector<int>& visited, vector<int>& out, vector<vector<int>>& res) {
        if (level >= nums.size()) {res.push_back(out); return;}
        for (int i = 0; i < nums.size(); ++i) {
            if (visited[i] == 1) continue;
            if (i > 0 && nums[i] == nums[i - 1] && visited[i - 1] == 0) continue;
            visited[i] = 1;
            out.push_back(nums[i]);
            permuteUniqueDFS(nums, level + 1, visited, out, res);
            out.pop_back();
            visited[i] = 0;
        }
    }
};
```



在使用上面的方法的时候，一定要能弄清楚递归函数的for循环中两个if的剪枝的意思。在此之前，首先要弄清楚level的含义，这里由于我们是用数组out来拼排列结果，这里的level其实就是当前已经拼成的个数，其实就是out数组的长度。我们看到，for循环的起始是从0开始的，而本题的解法二，三，四都是用了一个start变量，从而for循环都是从start开始，一定要分清楚start和本解法中的level的区别。由于递归的for都是从0开始，难免会重复遍历到数字，而全排列不能重复使用数字，意思是每个nums中的数字在全排列中只能使用一次（当然这并不妨碍nums中存在重复数字）。不能重复使用数字就靠visited数组来保证，这就是第一个if剪枝的意义所在。关键来看第二个if剪枝的意义，这里说当前数字和前一个数字相同，且前一个数字的visited值为0的时候，必须跳过。这里的前一个数visited值为0，并不代表前一个数字没有被处理过，也可能是递归结束后恢复状态时将visited值重置为0了，实际上就是这种情况，下面打印了一些中间过程的变量值，如下所示：

```
level = 0, i = 0 => out: {}
level = 1, i = 0 => out: {1 } skipped 1
level = 1, i = 1 => out: {1 }
level = 2, i = 0 => out: {1 2 } skipped 1
level = 2, i = 1 => out: {1 2 } skipped 1
level = 2, i = 2 => out: {1 2 }
level = 3 => saved  {1 2 2}
level = 3, i = 0 => out: {1 2 2 } skipped 1
level = 3, i = 1 => out: {1 2 2 } skipped 1
level = 3, i = 2 => out: {1 2 2 } skipped 1
level = 2, i = 2 => out: {1 2 2 } -> {1 2 } recovered
level = 1, i = 1 => out: {1 2 } -> {1 } recovered
level = 1, i = 2 => out: {1 } skipped 2
level = 0, i = 0 => out: {1 } -> {} recovered
level = 0, i = 1 => out: {}
level = 1, i = 0 => out: {2 }
level = 2, i = 0 => out: {2 1 } skipped 1
level = 2, i = 1 => out: {2 1 } skipped 1
level = 2, i = 2 => out: {2 1 }
level = 3 => saved  {1 2 2}
level = 3, i = 0 => out: {2 1 2 } skipped 1
level = 3, i = 1 => out: {2 1 2 } skipped 1
level = 3, i = 2 => out: {2 1 2 } skipped 1
level = 2, i = 2 => out: {2 1 2 } -> {2 1 } recovered
level = 1, i = 0 => out: {2 1 } -> {2 } recovered
level = 1, i = 1 => out: {2 } skipped 1
level = 1, i = 2 => out: {2 }
level = 2, i = 0 => out: {2 2 }
level = 3 => saved  {1 2 2}
level = 3, i = 0 => out: {2 2 1 } skipped 1
level = 3, i = 1 => out: {2 2 1 } skipped 1
level = 3, i = 2 => out: {2 2 1 } skipped 1
level = 2, i = 0 => out: {2 2 1 } -> {2 2 } recovered
level = 2, i = 1 => out: {2 2 } skipped 1
level = 2, i = 2 => out: {2 2 } skipped 1
level = 1, i = 2 => out: {2 2 } -> {2 } recovered
level = 0, i = 1 => out: {2 } -> {} recovered
level = 0, i = 2 => out: {} skipped 2
```

注意看这里面的 skipped 1 表示的是第一个if剪枝起作用的地方，skipped 2 表示的是第二个if剪枝起作用的地方。我们主要关心的是第二个if剪枝，那么看上方第一个蓝色标记的那行，再上面的红色一行表示在 level = 1, i = 1 时递归调用结束后，恢复到起始状态，那么此时 out 数组中只有一个1，后面的2已经被pop_back()了，当然对应visited值也重置为0了，这种情况下我们需要剪枝，当然不能再一次把2往里加，因为这种情况在递归中已经加入到结果res中了，所以到了 level = 1, i = 2 的状态时，nums[i] == nums[i-1] && visited[i-1] == 0 的条件满足了，我们的剪枝就起作用了，这种重复的情况就被剪掉了。



还有一种比较简便的方法，在 [Permutations](http://www.cnblogs.com/grandyang/p/4358848.html) 的基础上稍加修改，我们用TreeSet来保存结果，利用其不会有重复项的特点，然后我们再递归函数中的swap的地方，判断如果i和start不相同，但是 nums[i] 和 nums[start] 相同的情况下跳过，继续下一个循环，参见代码如下：



解法二：

```
class Solution {
public:
    vector<vector<int>> permuteUnique(vector<int>& nums) {
        set<vector<int>> res;
        permute(nums, 0, res);
        return vector<vector<int>> (res.begin(), res.end());
    }
    void permute(vector<int>& nums, int start, set<vector<int>>& res) {
        if (start >= nums.size()) res.insert(nums);
        for (int i = start; i < nums.size(); ++i) {
            if (i != start && nums[i] == nums[start]) continue;
            swap(nums[i], nums[start]);
            permute(nums, start + 1, res);
            swap(nums[i], nums[start]);
        }
    }
};
```



对于上面的解法，你可能会有疑问，我们不是在swap操作之前已经做了剪枝了么，为什么还是会有重复出现，以至于还要用TreeSet来取出重复呢。总感觉使用TreeSet去重复有点耍赖，可能并没有探究到本题深层次的内容。这是很好的想法，首先我们先尝试将上面的TreeSet还原为vector，并且在主函数调用递归之前给nums排个序（代码参见评论区三楼），然后测试一个最简单的例子：[1, 2, 2]，得到的结果为：

[[1,2,2], [2,1,2], [2,2,1], [2,2,1], [2,1,2]]

我们发现有重复项，那么我们的剪枝究竟在做些什么，怎么还是没法防止重复项的产生！我们的那个剪枝只是为了防止当 start = 1, i = 2 时，将两个2交换，这样可以防止 {1, 2, 2} 被加入两次。但是没法防止其他的重复情况，要闹清楚为啥，我们需要仔细分析一些中间过程，下面打印了一些中间过程的变量：

```
start = 0, i = 0 => {1 2 2} 
start = 1, i = 1 => {1 2 2} 
start = 2, i = 2 => {1 2 2} 
start = 3 => saved  {1 2 2}
start = 1, i = 2 => {1 2 2} skipped
start = 0, i = 1 => {1 2 2} -> {2 1 2}
start = 1, i = 1 => {2 1 2} 
start = 2, i = 2 => {2 1 2} 
start = 3 => saved  {2 1 2}
start = 1, i = 2 => {2 1 2} -> {2 2 1}
start = 2, i = 2 => {2 2 1} 
start = 3 => saved  {2 2 1}
start = 1, i = 2 => {2 2 1} -> {2 1 2} recovered
start = 0, i = 1 => {2 1 2} -> {1 2 2} recovered
start = 0, i = 2 => {1 2 2} -> {2 2 1}
start = 1, i = 1 => {2 2 1} 
start = 2, i = 2 => {2 2 1} 
start = 3 => saved  {2 2 1}
start = 1, i = 2 => {2 2 1} -> {2 1 2}
start = 2, i = 2 => {2 1 2} 
start = 3 => saved  {2 1 2}
start = 1, i = 2 => {2 1 2} -> {2 2 1} recovered
start = 0, i = 2 => {2 2 1} -> {1 2 2} recovered
```

问题出在了递归调用之后的还原状态，参见上面的红色的两行，当 start = 0, i = 2 时，nums 已经还原到了 {1, 2, 2} 的状态，此时 nums[start] 不等于 nums[i]，我们的剪枝在这已经失效了，那么交换后的 {2, 2, 1} 还会被存到结果res中，而这个状态我们在之前就已经存过了一次。

我们注意到当 start = 0, i = 1 时，nums交换之后变成了 {2, 1, 2}，如果我们能保持这个状态，那么当 start = 0, i = 2 时，此时 nums[start] 就等于 nums[i] 了，我们的剪枝操作就可以发挥作用了。怎么才能当递归结束后，不还原成为交换之前的状态的呢？答案就是不进行还原，这样还是能保存为之前交换后的状态。我们只是将最后一句 swap(nums[i], nums[start]) 删掉是不行的，因为我们的递归函数的参数 nums 是加了&号，就表示引用了，那么之前调用递归函数之前的 nums 在递归函数中会被修改，可能还是无法得到我们想要的顺序，所以我们要把递归函数的nums参数的&号也同时去掉才行，参见代码如下：



解法三：

```
class Solution {
public:
    vector<vector<int>> permuteUnique(vector<int>& nums) {
        vector<vector<int>> res;
        sort(nums.begin(), nums.end());
        permute(nums, 0, res);
        return res;
    }
    void permute(vector<int> nums, int start, vector<vector<int>>& res) {
        if (start >= nums.size()) res.push_back(nums);
        for (int i = start; i < nums.size(); ++i) {
            if (i != start && nums[i] == nums[start]) continue;
            swap(nums[i], nums[start]);
            permute(nums, start + 1, res);
        }
    }
};
```



好，我们再测试下 [1, 2, 2] 这个例子，并且把中间变量打印出来：

```
start = 0, i = 0 => {1 2 2} 
start = 1, i = 1 => {1 2 2} 
start = 2, i = 2 => {1 2 2} 
start = 3 => saved  {1 2 2}
start = 1, i = 2 => {1 2 2} skipped
start = 0, i = 1 => {1 2 2} -> {2 1 2}
start = 1, i = 1 => {2 1 2} 
start = 2, i = 2 => {2 1 2} 
start = 3 => saved  {2 1 2}
start = 1, i = 2 => {2 1 2} -> {2 2 1}
start = 2, i = 2 => {2 2 1} 
start = 3 => saved  {2 2 1}
start = 1, i = 2 => {2 2 1} recovered
start = 0, i = 1 => {2 1 2} recovered
start = 0, i = 2 => {2 1 2} skipped
```

明显发现短了许多，说明我们的剪枝发挥了作用，我们看上面红色部分，当 start = 0, i = 1 时，递归函数调用完了之后，nums数组保持了 {2, 1, 2} 的状态，那么到 start = 0, i = 2 的时候，nums[start] 就等于 nums[i] 了，我们的剪枝操作就可以发挥作用了。

这时候你可能会想，调用完递归不恢复状态，感觉怪怪的，跟哥的递归模版不一样啊，容易搞混啊，而且一会加&号，一会不加的，这尼玛谁能分得清啊。别担心，I gotcha covered! 好，既然还是要恢复状态的话，我们就只能从剪枝入手了，原来那种naive的剪枝方法肯定无法使用，矛盾的焦点还是在于，当 start = 0, i = 2 时，nums被还原成了 start = 0, i = 1 的交换前的状态 {1, 2, 2}，这个状态已经被处理过了，再去处理一定会产生重复，我们怎么才知道这被处理过了呢，当前的 i = 2，我们需要往前去找是否有重复出现，由于数组已经排序过了，如果有重复，那么前面数一定和当前的相同，所以我们用一个while循环，往前找和 nums[i] 相同的数字，找到了就停下，当然如果小于start了也要停下，那么如果没有重复数字的话，j 一定是等于 start-1 的，那么如果不等于的话，就直接跳过就可以了，这样就可以去掉所有的重复啦，参见代码如下：



解法四：

```
class Solution {
public:
    vector<vector<int>> permuteUnique(vector<int>& nums) {
        vector<vector<int>> res;
        sort(nums.begin(), nums.end());
        permute(nums, 0, res);
        return res;
    }
    void permute(vector<int>& nums, int start, vector<vector<int>>& res) {
        if (start >= nums.size()) res.push_back(nums);
        for (int i = start; i < nums.size(); ++i) {
            int j = i - 1;
            while (j >= start && nums[j] != nums[i]) --j;
            if (j != start - 1) continue;
            swap(nums[i], nums[start]);
            permute(nums, start + 1, res);
            swap(nums[i], nums[start]);
        }
    }
};
```



同样，我们再测试下 [1, 2, 2] 这个例子，并且把中间变量打印出来：

```
start = 0, i = 0 => {1 2 2} , j = -1
start = 1, i = 1 => {1 2 2} , j = 0
start = 2, i = 2 => {1 2 2} , j = 1
start = 3 => saved  {1 2 2}
start = 1, i = 2 => {1 2 2} skipped, j = 1
start = 0, i = 1 => {1 2 2} -> {2 1 2}, j = -1
start = 1, i = 1 => {2 1 2} , j = 0
start = 2, i = 2 => {2 1 2} , j = 1
start = 3 => saved  {2 1 2}
start = 1, i = 2 => {2 1 2} -> {2 2 1}, j = 0
start = 2, i = 2 => {2 2 1} , j = 1
start = 3 => saved  {2 2 1}
start = 1, i = 2 => {2 2 1} -> {2 1 2} recovered
start = 0, i = 1 => {2 1 2} -> {1 2 2} recovered
start = 0, i = 2 => {1 2 2} skipped, j = 1
```

到 start = 0, i = 2 的时候，j 此时等于1了，明显不是start-1，说明有重复了，直接skip掉，这样我们的剪枝操作就可以发挥作用了。



之前的 [Permutations](http://www.cnblogs.com/grandyang/p/4358848.html) 中的解法三也可以用在这里，只不过需要借助TreeSet来去重复，博主还未想出其他不用集合的去重复的方法，哪位看官大神们知道的话，请一定要留言告知博主，参见代码如下：



解法五：

```
class Solution {
public:
    vector<vector<int>> permuteUnique(vector<int>& nums) {
        if (nums.empty()) return vector<vector<int>>(1, vector<int>());
        set<vector<int>> res;
        int first = nums[0];
        nums.erase(nums.begin());
        vector<vector<int>> words = permuteUnique(nums);
        for (auto &a : words) {
            for (int i = 0; i <= a.size(); ++i) {
                a.insert(a.begin() + i, first);
                res.insert(a);
                a.erase(a.begin() + i);
            }
        }   
        return vector<vector<int>> (res.begin(), res.end());
    }
};
```



之前的 [Permutations](http://www.cnblogs.com/grandyang/p/4358848.html) 中的解法四博主没法成功修改使其可以通过这道题，即便是将结果res用TreeSet来去重复，还是不对。同样，哪位看官大神们知道的话，请一定要留言告知博主。不过之前的 [Permutations](http://www.cnblogs.com/grandyang/p/4358848.html) 中的解法五却可以原封不动的搬到这道题来，看来自带的 next_permutation() 函数就是叼啊，自带去重复功能，叼叼叼！参见代码如下：



解法六：

```
class Solution {
public:
    vector<vector<int>> permuteUnique(vector<int>& nums) {
        vector<vector<int>> res;
        sort(nums.begin(), nums.end());
        res.push_back(nums);
        while (next_permutation(nums.begin(), nums.end())) {
            res.push_back(nums);
        }
        return res;
    }
};
```



类似题目：

[Permutations](http://www.cnblogs.com/grandyang/p/4358848.html)

[Next Permutation](http://www.cnblogs.com/grandyang/p/4428207.html)

[Palindrome Permutation II](http://www.cnblogs.com/grandyang/p/5315227.html)



参考资料：

[https://leetcode.com/problems/permutations-ii/](https://leetcode.com/problems/permutations-ii/)

[https://leetcode.com/problems/permutations-ii/discuss/18601/Short-iterative-Java-solution](https://leetcode.com/problems/permutations-ii/discuss/18601/Short-iterative-Java-solution)

[https://leetcode.com/problems/permutations-ii/discuss/18596/A-simple-C%2B%2B-solution-in-only-20-lines](https://leetcode.com/problems/permutations-ii/discuss/18596/A-simple-C%2B%2B-solution-in-only-20-lines)

[https://leetcode.com/problems/permutations-ii/discuss/18594/Really-easy-Java-solution-much-easier-than-the-solutions-with-very-high-vote](https://leetcode.com/problems/permutations-ii/discuss/18594/Really-easy-Java-solution-much-easier-than-the-solutions-with-very-high-vote)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












