# [LeetCode] Range Sum Query - Mutable 区域和检索 - 可变 - Grandyang - 博客园







# [[LeetCode] Range Sum Query - Mutable 区域和检索 - 可变](https://www.cnblogs.com/grandyang/p/4985506.html)







Given an integer array *nums*, find the sum of the elements between indices *i* and *j* (*i* ≤ *j*), inclusive.

The *update(i, val)* function modifies *nums* by updating the element at index *i* to *val*.

**Example:**

Given nums = [1, 3, 5]

sumRange(0, 2) -> 9
update(1, 2)
sumRange(0, 2) -> 8

**Note:**

- The array is only modifiable by the *update* function.
- You may assume the number of calls to *update* and *sumRange* function is distributed evenly.



这道题是之前那道 [Range Sum Query - Immutable](http://www.cnblogs.com/grandyang/p/4952464.html) 的延伸，之前那道题由于数组的内容不会改变，所以我们只需要建立一个累计数组就可以支持快速的计算区间值了，而这道题说数组的内容会改变，如果我们还是用之前的方法建立累计和数组，那么每改变一个数字，之后所有位置的数字都要改变，这样如果有很多更新操作的话，就会十分不高效，估计很难通过吧。But，被OJ分分钟打脸，brute force完全没有问题啊，这年头，装个比不容易啊。直接就用个数组data接住nums，然后要更新就更新，要求区域和，就遍历求区域和，就这样naive的方法还能beat百分之二十多啊，这不科学啊，参见代码如下：



解法一：

```
class NumArray {
public:
    NumArray(vector<int> nums) {
        data = nums;
    }
    
    void update(int i, int val) {
        data[i] = val;
    }
    
    int sumRange(int i, int j) {
        int sum = 0;
        for (int k = i; k <= j; ++k) {
            sum += data[k];
        }
        return sum;
    }
    
private:
    vector<int> data;
};
```



咳咳，下面就开始闪亮的装比时间了，光芒必将盖过坂本大佬。上面的方法最大的问题，就是求区域和不高效，如果数组很大很大，每次求一个巨型的区间的和，都要一个一个的遍历去累加，累啊～但是一般的累加数组又无法应对这里的update操作，随便修改一个数字的话，那么其之后的所有累加和都会发生改变。所以解决方案就是二者折中一下，分块累加，各不干预。就是将原数组分为若干块，怎么分呢，这里就让每个block有sqrt(n)个数字就可以了，这个基本是让block的个数跟每个blcok中数字的个数尽可能相同的分割方法。然后我们就需要一个大小跟block个数相同的数组，来保存每个block的数字之和。在需要更新的时候，我们就先确定要更新的位置在哪个block里，然后只更新该block的和。而对于求区域和操作，我们还是要分别确定i和j分别属于哪个block，若属于同一个block，那么直接遍历累加即可，若属于不同的，则先从i累加到该blcok的末尾，然后中间横跨的那些block可以直接将和累加，对于j所在的blcok，则从该block的开头遍历累加到j即可，参见代码如下：



解法二：

```
class NumArray {
public:
    NumArray(vector<int> nums) {
        if (nums.empty()) return;
        data = nums;
        double root = sqrt(data.size());
        len = ceil(data.size() / root);
        block.resize(len);
        for (int i = 0; i < data.size(); ++i) {
            block[i / len] += data[i];
        }
    }
    
    void update(int i, int val) {
        int idx = i / len;
        block[idx] += val - data[i];
        data[i] = val;
    }
    
    int sumRange(int i, int j) {
        int sum = 0;
        int start = i / len, end = j / len;
        if (start == end) {
            for (int k = i; k <= j; ++k) {
                sum += data[k];
            }
            return sum;
        }
        for (int k = i; k < (start + 1) * len; ++k) {
            sum += data[k];
        }
        for (int k = start + 1; k < end; ++k) {
            sum += block[k];
        }
        for (int k = end * len; k <= j; ++k) {
            sum += data[k];
        }
        return sum;
    }
    
private:
    int len;
    vector<int> data, block;
};
```



同样是利用分块区域和的思路，下面这种方法使用了一种新的数据结构，叫做 [树状数组Binary Indexed Tree](https://zh.wikipedia.org/wiki/%E6%A0%91%E7%8A%B6%E6%95%B0%E7%BB%84)，又称Fenwick Tree，这是一种查询和修改复杂度均为O(logn)的数据结构。这个树状数组比较有意思，所有的奇数位置的数字和原数组对应位置的相同，偶数位置是原数组若干位置之和，假如原数组A(a1, a2, a3, a4 ...)，和其对应的树状数组C(c1, c2, c3, c4 ...)有如下关系：

![](data:image/jpeg;base64,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)

C1 = A1

C2 = A1 + A2

C3 = A3

C4 = A1 + A2 + A3 + A4

C5 = A5

C6 = A5 + A6

C7 = A7

C8 = A1 + A2 + A3 + A4 + A5 + A6 + A7 + A8

...

那么是如何确定某个位置到底是有几个数组成的呢，原来是根据坐标的最低位Low Bit来决定的，所谓的最低位，就是二进制数的最右边的一个1开始，加上后面的0(如果有的话)组成的数字，例如1到8的最低位如下面所示：

坐标          二进制          最低位

1               0001          1

2               0010          2

3               0011          1

4               0100          4

5               0101          1

6               0110          2

7               0111          1

8               1000          8

...

最低位的计算方法有两种，一种是
```cpp
x&(x^(x–1))
```
，另一种是利用补码特性
```cpp
x&-x。
```


这道题我们先根据给定输入数组建立一个树状数组bit，比如，对于 nums = {1, 3, 5, 7, 9, 11, 13, 15, 17}，建立出的bit数组为：

bit -> 0 1 4 5 18 11 24 15 74

注意到我们给bit数组开头padding了一个0，这样我们在利用上面的树状数组的性质时就不用进行坐标转换了。可以发现bit数组中奇数位上的数字跟原数组是相同的，参见上面标记蓝色的数字。偶数位则是之前若干位之和，符合上图中的规律。

现在我们要更新某一位数字时，比如将数字5变成2，即update(2, 2)，那么现求出差值 diff = 2 - 5 = -3，然后我们需要更新树状数组bit，根据最低位的值来更新后面含有这一位数字的地方，一般只需要更新部分偶数位置的值即可。由于我们在开头padding了个0，所以我们的起始位置要加1，即 j=3，然后现将 bit[3] 更新为2，然后更新下一位，根据图中所示，并不是bit[3]后的每一位都需要更新的，下一位需要更新的位置的计算方法为 j += (j&-j)，这里我们的j是3，则 (j&-j) = 1，所以下一位需要更新的是 bit[4]，更新为15，现在j是4，则 (j&-j) = 4，所以下一位需要更新的是 bit[8]，更新为71，具体的变换过程如下所示：

0 1 4 5 18 11 24 15 74

0 1 4 2 18 11 24 15 74

0 1 4 2 15 11 24 15 74

0 1 4 2 15 11 24 15 71

接下来就是求区域和了，直接求有些困难，我们需要稍稍转换下思路。比如若我们能求出前i-1个数字之和，跟前j个数字之和，那么二者的差值就是要求的区间和了。所以我们先实现求前任意i个数字之和，当然还是要利用树状数组的性质，此时正好跟update函数反过来，我们的j从位置i开始，每次将bit[j]累加到sum，然后更新j，通过 j -= (j&-j)，这样就能快速的求出前i个数字之和了，从而也就能求出任意区间之和了，参见代码如下：



解法三：

```
class NumArray {
public:
    NumArray(vector<int> nums) {
        data.resize(nums.size());
        bit.resize(nums.size() + 1);
        for (int i = 0; i < nums.size(); ++i) {
            update(i, nums[i]);
        }
    }

    void update(int i, int val) {
        int diff = val - data[i];
        for (int j = i + 1; j < bit.size(); j += (j&-j)) {
            bit[j] += diff;
        }
        data[i] = val;
    }

    int sumRange(int i, int j) {
        return getSum(j + 1) - getSum(i);
    }    

    int getSum(int i) {
        int res = 0;
        for (int j = i; j > 0; j -= (j&-j)) {
            res += bit[j];
        }
        return res;
    }

private:
    vector<int> data, bit;
};
```



下面这种方法使用了[线段树Segment Tree](https://zh.wikipedia.org/wiki/%E7%BA%BF%E6%AE%B5%E6%A0%91_(%E5%8C%BA%E9%97%B4%E6%9F%A5%E8%AF%A2))来做，对线段树不是很了解的童鞋可以参见[网上这个帖子](https://www.jianshu.com/p/91f2c503e62f)，在博主看来，线段树就是一棵加了些额外信息的满二叉树，比如可以加子树的结点和，或者最大值，最小值等等，这样，当某个结点的值发生变化时，只需要更新一下其所有祖先结点的信息，而并不需要更新整棵树，这样就极大的提高了效率。比如对于 [1 3 5 7] 这个数组，我们可以根据其坐标划分，组成一个线段树：

```
[0, 3]
             16
       /            \
    [0, 1]        [2, 3]
      4             12
   /     \       /     \
[0, 0] [1, 1] [2, 2] [3, 3]
   1      3      5      7
```

其中，中括号表示是区间的范围，下方的数字是区间和，这样如果我们如果要更新区间 [2, 2] 中的数字5，那么之后只要再更新 [2, 3] 和 [0, 3] 两个区间即可，并不用更新所有的区间。而如果要求区间和，比如[1, 3] 的话，那么只需要加上这两个区间 [1, 1] 和 [2, 3] 的和即可，感觉跟解法二的核心思想很类似。

将线段树的核心思想理解了之后，我们就要用它来解题了。这里，我们并不用建立专门的线段树结点，因为博主十分的不喜欢在Solution类中新建其他类，博主追求的是简约时尚的代码风格，所以我们可以只用一个数组来模拟线段树。大小是多少呢，首先肯定要包换nums中的n个数字了，对于一个有n个叶结点的平衡的满二叉树，其总结点个数不会超过2n个，不要问博主怎么证明，因为我也不会。但你可以任意举例子来验证，都是正确的。所以我们用一个大小为2n的tree数字，然后就要往里面填数字了。填数字的方式先给tree数组的后n个数字按顺序填上nums数字，比如对于 nums = [1 3 5 7]，那么tree数组首先填上：

_ _ _ _ 1 3 5 7

然后从i=3开始，每次填上tree[2*n] + tree[2*n+1]，那么以此为：

_ _ _ 12 1 3 5 7

_ _ 4 12 1 3 5 7

_ 16 4 12 1 3 5 7

那么最终的tree数组就是 [0 16 4 12 1 3 5 7]，tree[0] 其实没啥作用，所以不用去管它。

接下来看update函数，比如我们想把5换成2，即调用update(2, 2)，由于nums数组在tree数组中是从位置n开始的，所以i也要加n，变成了6。所以先把tree[6]换乘2，那么此时tree数组为：

0 16 4 12 1 3 2 7

然后还要更新之前的数字，做法是若i大于0，则进行while循环，因为我们知道tree数组中i位置的父结点是在 tree[i/2]，所以我们要更新 tree[i/2]，那么要更新父结点值，就要知道左右子结点值，而此时我们需要知道i是左子结点还是右子结点么？其实可以使用一个小trick，就是对于结点i，跟其成对儿的另一个结点位置是 i^1，根据异或的性质，当i为奇数，i^1 为偶数，当i为偶数，i^1 为奇数，二者一直成对出现，这样左右子结点有了，直接更新父结点 i/2 即可，然后i自除以2，继续循环。tree数组的之后变化过程为：

0 16 4 9 1 3 2 7

0 13 4 9 1 3 2 7

13 13 4 9 1 3 2 7

可以看到，tree[0]也被更新，但其实并没有什么卵用，不用理它。

接下来就是求区域和了，比如我们要求sumRange(1, 3)，对于更新后的数组 nums = [1 3 2 7]，我们可以很快算出来，是12。那么对于 tree = [13 13 4 9 1 3 2 7]，我们如何计算呢。当然还是要先进行坐标变换，i变为5，j变为7。然后进行累加，我们的策略是，若i是左子结点，那么跟其成对儿出现的右边的结点就在要求的区间里，则此时直接加上父结点值即可，若i是右子结点，那么只需要加上结点i本身即可。同理，若j是左子结点，那么只需要加上结点j本身，若j是右子结点，那么跟其成对儿出现的左边的结点就在要求的区间里，则此时直接加上父结点值即可。具体的实现方法是，判断若i是奇数，则说明其是右子结点，则加上tree[i]本身，然后i自增1；再判断若j是偶数，则说明其是左子结点，则加上tree[j]本身，然后j自减1。那么你可能有疑问，i是偶数，和j是奇数的情况就不用处理了么，当然不是，这两种情况都是要加父结点的，我们可以到下一轮去加，因为每一轮后，i和j都会除以2，那么i一定会有到奇数的一天，所以不用担心会有值丢失，一定会到某一个父结点上把值加上的，参见代码如下：



解法四：

```
class NumArray {
public:
    NumArray(vector<int> nums) {
        n = nums.size();
        tree.resize(n * 2);
        buildTree(nums);
    }

    void buildTree(vector<int>& nums) {
        for (int i = n; i < n * 2; ++i) {
            tree[i] = nums[i - n];
        }
        for (int i = n - 1; i > 0; --i) {
            tree[i] = tree[i * 2] + tree[i * 2 + 1];
        }
    }

    void update(int i, int val) {
        tree[i += n] = val;
        while (i > 0) {
            tree[i / 2] = tree[i] + tree[i ^ 1];
            i /= 2;
        }
    }

    int sumRange(int i, int j) {
        int sum = 0;
        for (i += n, j += n; i <= j; i /= 2, j /= 2) {
            if ((i & 1) == 1) sum += tree[i++];
            if ((j & 1) == 0) sum += tree[j--];
        }
        return sum;
    }    

private:
    int n;
    vector<int> tree;
};
```



讨论：通过介绍上面四种不同的方法，我们应该如何处理可变的区间和问题有了深刻的认识了。除了第一种brute force的方法是无脑遍历之外，后面三种方法其实都进行了部分数字的和累加，都将整体拆分成了若干的小的部分，只不过各自的拆分方法各不相同，解法二是平均拆分，解法三树状数组是变着花样拆分，解法四线段树是二分法拆分，八仙过海，各显神通吧。



类似题目：

[Range Sum Query 2D - Mutable](http://www.cnblogs.com/grandyang/p/5300458.html)

[Range Sum Query 2D - Immutable](http://www.cnblogs.com/grandyang/p/4958789.html)

[Range Sum Query - Immutable](http://www.cnblogs.com/grandyang/p/4952464.html)



参考资料：

[https://leetcode.com/problems/range-sum-query-mutable/](https://leetcode.com/problems/range-sum-query-mutable/)

[https://leetcode.com/problems/range-sum-query-mutable/discuss/75763/7ms-Java-solution-using-bottom-up-segment-tree](https://leetcode.com/problems/range-sum-query-mutable/discuss/75763/7ms-Java-solution-using-bottom-up-segment-tree)

[https://leetcode.com/problems/range-sum-query-mutable/discuss/75785/Share-my-c%2B%2B-solution%3A-1700ms-using-tree-array](https://leetcode.com/problems/range-sum-query-mutable/discuss/75785/Share-my-c%2B%2B-solution%3A-1700ms-using-tree-array)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












