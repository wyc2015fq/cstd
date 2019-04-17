# [LeetCode] Random Flip Matrix 随机翻转矩阵 - Grandyang - 博客园







# [[LeetCode] Random Flip Matrix 随机翻转矩阵](https://www.cnblogs.com/grandyang/p/9781289.html)







You are given the number of rows `n_rows` and number of columns `n_cols` of a 2D binary matrix where all values are initially 0. Write a function `flip` which chooses a 0 value [uniformly at random](https://en.wikipedia.org/wiki/Discrete_uniform_distribution), changes it to 1, and then returns the position `[row.id, col.id]` of that value. Also, write a function `reset` which sets all values back to 0. Try to minimize the number of calls to system's Math.random() and optimize the time and space complexity.

Note:
- `1 <= n_rows, n_cols <= 10000`
- `0 <= row.id < n_rows` and `0 <= col.id < n_cols`
- `flip` will not be called when the matrix has no 0 values left.
- the total number of calls to `flip` and `reset` will not exceed 1000.

Example 1:
Input: 
["Solution","flip","flip","flip","flip"]
[[2,3],[],[],[],[]]
Output: [null,[0,1],[1,2],[1,0],[1,1]]


Example 2:
Input: 
["Solution","flip","flip","reset","flip"]
[[1,2],[],[],[],[]]
Output: [null,[0,0],[0,1],null,[0,0]]

Explanation of Input Syntax:

The input is two lists: the subroutines called and their arguments. `Solution`'s constructor has two arguments, `n_rows` and `n_cols`. `flip` and `reset`have no arguments. Arguments are always wrapped with a list, even if there aren't any.



这道题让我们随机翻转矩阵中的一个位置，由于之前连续做了好几道随机选点的题 [Implement Rand10() Using Rand7()](https://www.cnblogs.com/grandyang/p/9727206.html)，[Generate Random Point in a Circle](https://www.cnblogs.com/grandyang/p/9741220.html)，和 [Random Point in Non-overlapping Rectangles](https://www.cnblogs.com/grandyang/p/9752145.html)。以为这道题也要用拒绝采样Rejection Sampling来做，其实不是的。这道题给了我们一个矩形的长和宽，让我们每次随机翻转其中的一个点，其中的隐含条件是，之前翻转过的点，下一次不能再翻转回来，而我们随机生成点是有可能有重复的，一旦很多点都被翻转后，很大概率会重复生成之前的点，所以我们需要有去重复的操作，而这也是本题的难点所在。博主最先的想法是，既然有可能生成重复的点，那么我们使用一个while循环，只要生成了之前的点，我们就重新再生成一个，这么一说，感觉又有点像[拒绝采样Rejection Sampling](https://en.wikipedia.org/wiki/Rejection_sampling)的原理了。不管了，不管黑猫白猫，能抓耗子🐭的就是好猫🐱。题目中说了让我们尽量减少空间使用度，那么我们就不能生成整个二维数组了，我们可以用一个HashSet来记录翻转过了点，这样也方便我们进行查重操作。所以我们每次都随机出一个长和宽，然后看这个点是否已经在HashSet中了，不在的话，就加入HashSet，然后返回即可，参见代码如下：



解法一：

```
class Solution {
public:
    Solution(int n_rows, int n_cols) {
        row = n_rows; col = n_cols;
    }
    
    vector<int> flip() {
        while (true) {
            int x = rand() % row, y = rand() % col;
            if (!flipped.count(x * col + y)) {
                flipped.insert(x * col + y);
                return {x, y};
            }
        }
    }
    
    void reset() {
        flipped.clear();
    }

private:
    int row, col;
    unordered_set<int> flipped;
};
```



由于题目中让我们尽量少用rand()函数，所以我们可以进行优化一样，不在同时生成两个随机数，而是只生成一个，然后拆分出长和宽即可，其他部分和上面均相同，参见代码如下：



解法二：

```
class Solution {
public:
    Solution(int n_rows, int n_cols) {
        row = n_rows; col = n_cols;
    }
    
    vector<int> flip() {
        while (true) {
            int val = rand() % (row * col);
            if (!flipped.count(val)) {
                flipped.insert(val);
                return {val / col, val % col};
            }
        }
    }
    
    void reset() {
        flipped.clear();
    }

private:
    int row, col;
    unordered_set<int> flipped;
};
```



其实我们还可以进一步的优化rand()的调用数，我们可以让每个flip()函数只调用一次rand()函数，这该怎么做呢，这里就有一些trick了。我们需要使用一个变量size，初始化为矩形的长乘以宽，然后还是只生成一个随机数id，并使用另一个变量val来记录它。接下来我们给size自减1，我们知道 rand() % size 得到的随机数的范围是 [0, size-1]，那么假如第一次随机出了size-1后，此时size自减1之后，下一次不必担心还会随机出size-1，因为此时的size比之前减少了1。如果第一次随机出了0，假设最开始size=4，那么此时自减1之后，size=3，此时我们将0映射到3。那么下次我们如果再次随机出了0，此时size自减1之后，size=2，现在0有映射值，所以我们将id改为其映射值3，然后再将0映射到2，这样下次就算再摇出了0，我们还可以改变id值。大家有没有发现，我们的映射值都是没有没使用过的数字，这也是为啥开始先检测id是否被使用了，若已经被使用了，则换成其映射值，然后再更新之前的id的映射值，找到下一个未被使用的值即可，参见代码如下：



解法三：

```
class Solution {
public:
    Solution(int n_rows, int n_cols) {
        row = n_rows; col = n_cols;
        size = row * col;
    }
    
    vector<int> flip() {
        int id = rand() % size, val = id;
        --size;
        if (m.count(id)) id = m[id];
        m[val] = m.count(size) ? m[size] : size;
        return {id / col, id % col};
    }
    
    void reset() {
        m.clear();
        size = row * col;
    }

private:
    int row, col, size;
    unordered_map<int, int> m;
};
```



参考资料：

[https://leetcode.com/problems/random-flip-matrix/](https://leetcode.com/problems/random-flip-matrix/)

[https://leetcode.com/problems/random-flip-matrix/discuss/177809/c%2B%2B-solution](https://leetcode.com/problems/random-flip-matrix/discuss/177809/c%2B%2B-solution)

[https://leetcode.com/problems/random-flip-matrix/discuss/154053/Java-AC-Solution-call-Least-times-of-Random.nextInt()-function](https://leetcode.com/problems/random-flip-matrix/discuss/154053/Java-AC-Solution-call-Least-times-of-Random.nextInt()-function)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












