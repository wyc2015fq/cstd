
# 289. Game of Life - OraYang的博客 - CSDN博客

2017年12月18日 16:22:09[OraYang](https://me.csdn.net/u010665216)阅读数：374所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



# 289. Game of Life
标签（空格分隔）： leetcode array
---题目

---According to the

---[Wikipedia’s article](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life)

---: “The

---Game of Life

---, also known simply as

---Life

---, is a cellular automaton devised by the British mathematician John Horton Conway in 1970.”

---Given a

---board

---with

---m

---by

---n

---cells, each cell has an initial state

---live

---(1) or

---dead

---(0). Each cell interacts with its

---[eight neighbors](https://en.wikipedia.org/wiki/Moore_neighborhood)

---(horizontal, vertical, diagonal) using the following four rules (taken from the above Wikipedia article):


---Any live cell with fewer than two live neighbors dies, as if caused by under-population.

---Any live cell with two or three live neighbors lives on to the next generation.

---Any live cell with more than three live neighbors dies, as if by over-population..

---Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.


---Write a function to compute the next state (after one update) of the board given its current state.

---Follow up

---:


---Could you solve it in-place? Remember that the board needs to be updated at the same time: You cannot update some cells first and then use their updated values to update other cells.

---In this question, we represent the board using a 2D array. In principle, the board is infinite, which would cause problems when the active area encroaches the border of the array. How would you address these problems?


---Credits:

---Special thanks to

---[@jianchao.li.fighter](https://leetcode.com/discuss/user/jianchao.li.fighter)

---for adding this problem and creating all test cases.


---思路

---这道题目本质上考察的是矩阵的遍历，及本地修改，我们利用两个嵌套for循环来遍历数组，然后判断当前值是否满足相关规则，我们将状态变化：1->0;0->1用值3，4代替。关于那些规则本质上就是if-else语句。

---代码

---class

---Solution {

---public

---:

---void

---gameOfLife(

---vector

---<

---vector

---<

---int

--->

--->

---& board) {

---//1->0:3;0->1:4

---if

---(board.size()==

---0

---)

---return

---;

---int

---m=board.size(),n=board[

---0

---].size();

---for

---(

---int

---i=

---0

---;i<m;i++)
        {

---for

---(

---int

---j=

---0

---;j<n;j++)
            {

---int

---lives =  liveNeighbors(board,m,n,i,j);

---if

---((board[i][j]==

---1

---&&lives<

---2

---)||(board[i][j]==

---1

---&&lives>

---3

---))
                    board[i][j] =

---3

---;

---if

---(board[i][j]==

---0

---&&lives==

---3

---)
                    board[i][j] =

---4

---;
            }
        }

---for

---(

---int

---i =

---0

---; i < m; i++) {

---for

---(

---int

---j =

---0

---; j < n; j++) {

---if

---(board[i][j]==

---3

---) board[i][j]=

---0

---;

---else

---if

---(board[i][j]==

---4

---) board[i][j] =

---1

---;
            }
        }

---return

---;
    }

---private

---:

---int

---liveNeighbors(

---vector

---<

---vector

---<

---int

--->

--->

---& board,

---int

---m,

---int

---n,

---int

---i,

---int

---j) {

---int

---lives =

---0

---;

---for

---(

---int

---x = max(i -

---1

---,

---0

---); x <= min(i +

---1

---, m -

---1

---); x++) {

---for

---(

---int

---y = max(j -

---1

---,

---0

---); y <= min(j +

---1

---, n -

---1

---); y++) {
            lives += board[x][y] &

---1

---;
        }
    }
    lives -= board[i][j] &

---1

---;

---return

---lives;
}
};


