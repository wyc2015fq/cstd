# [LeetCode] K-th Symbol in Grammar 语法中的第K个符号 - Grandyang - 博客园







# [[LeetCode] K-th Symbol in Grammar 语法中的第K个符号](https://www.cnblogs.com/grandyang/p/9027098.html)







On the first row, we write a `0`. Now in every subsequent row, we look at the previous row and replace each occurrence of `0` with `01`, and each occurrence of `1` with `10`.

Given row `N` and index `K`, return the `K`-th indexed symbol in row `N`. (The values of `K` are 1-indexed.) (1 indexed).
Examples:
Input: N = 1, K = 1
Output: 0

Input: N = 2, K = 1
Output: 0

Input: N = 2, K = 2
Output: 1

Input: N = 4, K = 5
Output: 1

Explanation:
row 1: 0
row 2: 01
row 3: 0110
row 4: 01101001

Note:
- `N` will be an integer in the range `[1, 30]`.
- `K` will be an integer in the range `[1, 2^(N-1)]`.



这道题说第一行写上了一个0，然后从第二行开始，遇到0，就变为01，遇到1，则变为10，问我们第N行的第K个数字是啥。这是一道蛮有意思的题目，首先如果没啥思路的话，按照给定的方法，一行行generate出来，直到生成第N行，那么第K个数字也就知道了。但是这种brute force的方法无法通过OJ，这里就不多说了，需要想一些更高端的解法。我们想啊，遇到0变为01，那么可不可以把0和1看作上一层0的左右子结点呢，同时，把1和0看作上一层1的左右子结点，这样的话，我们整个结构就可以转为二叉树了，那么前四层的二叉树结构如下所示：

```
0
       /             \
      0               1
   /     \         /     \
  0       1       1       0
 / \     / \     / \     / \
0   1   1   0   1   0   0   1
```

我们仔细观察上面这棵二叉树，第四层K=3的那个红色的左子结点，其父结点的位置是第三层的第 (K+1)/2 = 2个红色结点，而第四层K=6的那个蓝色幽子结点，其父节点的位置是第三层的第 K/2 = 3个蓝色结点。那么我们就可以一层一层的往上推，直到到达第一层的那个0。所以我们的思路是根据当前层K的奇偶性来确定上一层中父节点的位置，然后继续往上一层推，直到推倒第一层的0，然后再返回确定路径上每一个位置的值，这天然就是递归的运行机制啊。我们可以根据K的奇偶性知道其是左结点还是右结点，由于K是从1开始的，所以当K是奇数时，其是左结点，当K是偶数时，其是右结点。而且还能观察出来的是，左子结点和其父节点的值相同，右子结点和其父节点值相反，这是因为0换成了01，1换成了10，左子结点保持不变，右子结点flip了一下。想通了这些，那么我们的递归解法就不难写出来了，参见代码如下：



解法一：

```
class Solution {
public:
    int kthGrammar(int N, int K) {
        if (N == 1) return 0;
        if (K % 2 == 0) return (kthGrammar(N - 1, K / 2) == 0) ? 1 : 0;
        else return (kthGrammar(N - 1, (K + 1) / 2) == 0) ? 0 : 1;
    }
};
```



我们可以简化下上面的解法，你们可能会说，纳尼？已经三行了还要简化？没错，博主就是这样一个精益求精的人（此处应有掌声👏）。我们知道偶数加1除以2，和其本身除以2的值是相同的，那么其实不论K是奇是偶，其父节点的位置都可以用 (K+1)/2 来表示，问题在于K本身的奇偶决定了其左右结点的位置，从而决定要不要flip父节点的值，这才是上面解法中我们要使用 if...else 结构的原因。实际上我们可以通过‘亦或’操作来实现一行搞定，叼不叼。我们来看下变换规则，0换成了01，1换成了10。

0 -> 01

左子结点(0) = 父节点(0) ^ 0 

右子结点(1) = 父节点(0) ^ 1 

1 -> 10

左子结点(1) = 父节点(1) ^ 0 

右子结点(0) = 父节点(1) ^ 1 

那么只要我们知道了父结点的值和当前K的奇偶性就可以知道K的值了，因为左子结点就是父结点值‘亦或’0，右子结点就是父结点值‘亦或’1，由于左子结点的K是奇数，我们可以对其取反再‘与’1，所以就是 (~K & 1)，再‘亦或’上递归函数的返回值即可，参见代码如下：



解法二：

```
class Solution {
public:
    int kthGrammar(int N, int K) {
        if (N == 1) return 0;
        return (~K & 1) ^ kthGrammar(N - 1, (K + 1) / 2);
    }
};
```



下面这种解法的思路也十分巧妙，还是从变换规则入手，0换成了01，1换成了10。我们来看解法一讲解中的二叉树示例图中的蓝色路径，第四层K=6，第三层K=3，第二层K=2，第一层K=1，所以路径就是 6->3->2->1，如果我们换成0开头的计数方法，那么路径就是 5->2->1->0，再换成二进制的表达方式就是 101->10->1->0，我们发现其实就是每次右移一位，直到移动到0，而当最低位是1的时候，表示当前是右子结点，需要flip一下，那么只要我们知道总共flip了多少次，就能推算出第K个位置的值。比如 101->10->1->0，总共flip了两次变成了0，那么说明K=6的位置值为0。于是问题就转化为了统计K-1这个数的二进制表示中位1的个数，使用一个while循环统计出来就可以了，然后根据个数的奇偶性返回1或0即可，参见代码如下：



解法三：

```
class Solution {
public:
    int kthGrammar(int N, int K) {
        int cnt = 0;
        --K;
        while (K) {
            cnt += K & 1;
            K >>= 1;
        }
        return cnt % 2;
    }
};
```



下面这种解法的思路也很清新脱俗啊，一切的一切都是从变换规则入手，0换成了01，1换成了10。那么当K是奇数的时候，我们之前分析了，其一定是左子结点，那么其是01或者10的第一个数字，因为只有这两种组合方式，所以如果第K个数是0的话，那么第K+1个数就是1，同样，如果第K个数是1的话，那么第K+1个数就是0，所以此时第K个数和第K+1个数一定相反，那么我们就可以通过‘亦或’1来实现这个一定相反的操作。

当K是偶数的时候，那么其是01或者10的第二个数字，那么根据之前的分析，其是由上一层的第 K/2 位置的数字生成的，上一层的第 K/2 个数字和当前层的第 K/2 个数字是一样的，如果你仔细观察题目中的例子或者博主画的那个二叉树图，只要K不越界，每一层的第K个数字都是相等的。所以如果第K个数是0的话，那么第 K/2 个数就是1，同样，如果第K个数是1的话，那么第 K/2 个数就是0，所以此时第K个数和第 K/2 个数一定相反，那么我们也可以通过‘亦或’1来实现这个一定相反的操作。

于是乎，我们的操作就是，当K是奇数的时候，我们就将其换成K+1，当K是偶数的时候，我们将其换为K/2。然后每次都对结果res（初始化为0）进行‘亦或’1操作，循环的终止条件是当K等于1时，参见代码如下：



解法四：

```
class Solution {
public:
    int kthGrammar(int N, int K) {
        int res = 0;
        while (K > 1) {
            K = (K % 2 == 1) ? K + 1 : K / 2;
            res ^= 1;
        }
        return res;
    }
};
```



下面这种解法跟解法三的思路完全相同，只不过使用了bitset这个内置的数据结构来快速的求出了K-1的二进制表达数中的位1的个数，Java中可以直接使用Integer.bitCount()函数，参见代码如下：



解法五：

```
class Solution {
public:
    int kthGrammar(int N, int K) {
        return bitset<32>(K - 1).count() % 2;
    }
};
```



参考资料：

[https://leetcode.com/problems/k-th-symbol-in-grammar/solution/](https://leetcode.com/problems/k-th-symbol-in-grammar/solution/)

[https://leetcode.com/problems/k-th-symbol-in-grammar/discuss/113705/JAVA-one-line](https://leetcode.com/problems/k-th-symbol-in-grammar/discuss/113705/JAVA-one-line)

[https://leetcode.com/problems/k-th-symbol-in-grammar/discuss/113697/My-3-lines-C++-recursive-solution](https://leetcode.com/problems/k-th-symbol-in-grammar/discuss/113697/My-3-lines-C++-recursive-solution)

[https://leetcode.com/problems/k-th-symbol-in-grammar/discuss/121544/C++JavaPython-Another-Solution-with-Explanation](https://leetcode.com/problems/k-th-symbol-in-grammar/discuss/121544/C++JavaPython-Another-Solution-with-Explanation)

[https://leetcode.com/problems/k-th-symbol-in-grammar/discuss/113721/C++-with-explanation-three-solutions-O(n)-O(logn)-and-O(loglogn)](https://leetcode.com/problems/k-th-symbol-in-grammar/discuss/113721/C++-with-explanation-three-solutions-O(n)-O(logn)-and-O(loglogn))



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












