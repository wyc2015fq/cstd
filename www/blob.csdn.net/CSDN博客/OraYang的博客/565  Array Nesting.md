
# 565. Array Nesting - OraYang的博客 - CSDN博客

2017年12月29日 10:56:09[OraYang](https://me.csdn.net/u010665216)阅读数：219所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



# 565. Array Nesting
标签（空格分隔）： leetcode array medium
---题目

---A zero-indexed array A of length N contains all integers from 0 to N-1. Find and return the longest length of set S, where S[i] = {A[i], A[A[i]], A[A[A[i]]], … } subjected to the rule below.

---Suppose the first element in S starts with the selection of element A[i] of index = i, the next element in S should be A[A[i]], and then A[A[A[i]]]… By that analogy, we stop adding right before a duplicate element occurs in S.

---Example 1:


---Input:

---A = [5,4,0,3,1,6,2]

---Output:

---6

---Explanation:

---A[0] = 5, A[1] = 4, A[2] = 0, A[3] = 3, A[4] = 1, A[5] = 6, A[6] = 2.
One of the longest S[K]:
S[0] = {A[0], A[5], A[6], A[2]} = {5, 6, 2, 0}

---Note:


---N is an integer within the range [1, 20,000].

---The elements of A are all distinct.

---Each element of A is an integer within the range [0, N-1].


---思路

---本题最直接的思路就是一次遍历数组，看每个下标对应的长度，很明显这种贪心的思想，那么怎么判断发生了循环呢，一个巧妙地做法就是将访问过的数置为-1。我第一种方法额外实现了一种方法，但是TLC了代码如下：

---class

---Solution {

---public

---:

---int

---arrayNesting(

---vector

---<

---int

--->

---& nums) {

---int

---max=INT_MIN,local;

---for

---(

---int

---i=

---0

---;i<nums.size();i++)
        {
            local = lennum(nums,i);
            max = max>local?max:local;
        }

---return

---max;
    }

---private

---:

---int

---lennum(

---vector

---<

---int

--->

---nums,

---int

---k)
    {

---int

---count=

---0

---,index=k;

---while

---(nums[k]>=

---0

---)
        {
            index=nums[k];
            nums[k]=-

---1

---;
            count++;
            k=index;
        }

---return

---count;
    }
};

---然后我将实现的方法合并到原函数中，发现AC了，我也不知道为什么会超时，难道说每次循环，调用方法耗时很长？

---AC的方法：

---class

---Solution {

---public

---:

---int

---arrayNesting(

---vector

---<

---int

--->

---& nums) {

---int

---res=

---0

---;

---for

---(

---int

---i=

---0

---;i<nums.size();i++)
        {

---int

---count=

---0

---;

---for

---(

---int

---k=i;nums[k]>=

---0

---;count++)
            {

---int

---index = nums[k];
                nums[k] = -

---1

---;
                k = index;
            }
            res = res>count?res:count;
        }

---return

---res;
    }
};


