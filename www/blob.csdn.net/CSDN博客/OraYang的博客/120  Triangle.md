
# 120. Triangle - OraYang的博客 - CSDN博客

2017年12月18日 15:27:48[OraYang](https://me.csdn.net/u010665216)阅读数：301所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



---题目

---Given a triangle, find the minimum path sum from top to bottom. Each step you may move to adjacent numbers on the row below.

---For example, given the following triangle


---[
     [

---2

---],
    [

---3

---,4],
   [6,

---5

---,7],
  [4,

---1

---,8,3]
]

---The minimum path sum from top to bottom is

---11

---(i.e.,

---2

---+

---3

---+

---5

---+

---1

---= 11).

---Note:

---Bonus point if you are able to do this using only

---O

---(

---n

---) extra space, where

---n

---is the total number of rows in the triangle.


---思路

---本题一开始看到三角形结构，我想到了树，从树我又想到了递归，因此想用深度优先遍历，但是仔细观察这个三角形结构，又会发现和最小路径本质上是共享一个分支的，因此这个问题可以分解成子三角形最小和来求解。所以动态规划的思想就呼之欲出了。但是动态规划是自顶向下还是自底向上呢？因为题目有限制条件：得分点是仅使用O(n)的额外空间，如果使用自顶向下就会产生O(n*n)的额外空间，因此我们采用自底向上。那么动态规划状态转移方程为：

---minpath[k][i] = min( minpath[k+1][i], minpath[k+1][i+1]) + triangle[k][i];

---可以简化为：

---For the kth level:

---minpath[i] = min( minpath[i], minpath[i+1]) + triangle[k][i];

---代码

---class

---Solution {

---public

---:

---int

---minimumTotal(

---vector

---<

---vector

---<

---int

--->

--->

---&triangle) {

---int

---n = triangle.size();

---vector

---<

---int

--->

---minlen(triangle.back());

---for

---(

---int

---layer = n-

---2

---; layer >=

---0

---; layer--)

---// For each layer

---{

---for

---(

---int

---i =

---0

---; i <= layer; i++)

---// Check its every 'node'

---{

---// Find the lesser of its two children, and sum the current value in the triangle with it.

---minlen[i] = min(minlen[i], minlen[i+

---1

---]) + triangle[layer][i]; 
        }
    }

---return

---minlen[

---0

---];
}
};


