
# 670. Maximum Swap - OraYang的博客 - CSDN博客

2017年12月20日 13:18:48[OraYang](https://me.csdn.net/u010665216)阅读数：402标签：[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[array																](https://so.csdn.net/so/search/s.do?q=array&t=blog)[medium																](https://so.csdn.net/so/search/s.do?q=medium&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=array&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
[
																					](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)[
							](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)



# 670. Maximum Swap
标签（空格分隔）： leetcode medium
---题目

---Given a non-negative integer, you could swap two digits

---at most

---once to get the maximum valued number. Return the maximum valued number you could get.

---Example 1:


---Input:

---2736

---Output:

---7236

---Explanation:

---Swap the number 2 and the number 7.

---Example 2:


---Input:

---9973

---Output:

---9973

---Explanation:

---No swap.

---Note:


---The given number is in the range [0, 10

---8

---]


---思路

---本题的目的是通过最多交换一次两个数字，从而使得数值最大。

---这道题目答题思路是：

---step1:维护一个指针指向当前首位数字

---step2:如果当前首位数字不是最大值，那么寻找当前索引后面数字的最大值与其交换；

---step3:如果当前首位数字是最大值，那么指针右移,跳转到step2

---很明显上述方法的复杂度为O(n)，为了降低复杂度必须把寻找最大值的方法进行改进，思路就是构造特殊的数据结构。来迅速地帮助我们找到最大值。

---我们构造一个特殊的数据结构叫做：位置数组pos[]

---该位置数组存储数据的格式如下，我们举个例子说明下：

---input 9 8 6 3 8

---pos 0 4 4 4 4

---比如说我们有个数值：98638

---那么从个位数开始遍历，当遍历到i索引位置时，pos[i]的值为遍历过的最大数字的索引。再举个例子

---input 5 4 3 1 2

---pos 0 1 2 4 4

---当pos数组存储完索引后。我们看num[pos[i]]的数值与num[i]之间的关系：

---num 9 8 6 3 8

---pos 0 4 4 4 4

---num[pos] 9 8 8 8 8

---num 5 4 3 1 2

---pos 0 1 2 4 4

---num[pos] 5 4 3 2 2

---很明显，同时遍历数组num[pos[i]]及num[i]，当出现第一次数字不一样时，进行数字交换。

---代码

---//这道题目答题思路是，

---//step1:维护一个指针指向当前首位数字

---//step2:如果当前首位数字不是最大值，那么寻找当前索引后面数字的最大值与其交换；

---//step3:如果当前首位数字是最大值，那么指针右移,跳转到step2

---//很明显上述方法的复杂度为O(n)，为了降低复杂度必须把寻找最大值的方法进行改进，思路就是构造特殊的数据结构。来迅速地帮助我们找到最大值。

---class

---Solution {

---public

---:

---int

---maximumSwap(

---int

---num) {

---//将数字转换成字符串

---string

---s = to_string(num);

---int

---len = s.size();

---//维护一个数组用来记录当前位置之后最大数字的索引。

---vector

---<

---int

--->

---pos(len,-

---1

---);

---int

---cur_index=len-

---1

---;

---for

---(

---int

---i=len-

---1

---;i>=

---0

---;i--)
        {

---if

---(s[i]>s[cur_index])
                cur_index=i;
            pos[i]=cur_index;
        }

---//比较第一个不同的位置

---for

---(

---int

---j=

---0

---;j<len;j++)
        {

---if

---(s[j]!=s[pos[j]])
            {
                swap(s[j],s[pos[j]]);

---break

---;
            }
        }

---return

---stoi(s);
    }
};


