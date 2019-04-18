# Leetcode全游戏问题 - 無名黑洞 - CSDN博客
2014年02月06日 13:07:10[wangxiaojun911](https://me.csdn.net/wangxiaojun911)阅读数：1689
**目录**
**1、编号34 Valid Sudoku2、编号36 Sudoku Solver3、编号41 Trapping Rain Water4、编号46 Jump Game5、编号47 Jump Game II 6、编号50 N-Queens7、编号52 N-Queens II 8、编号61 UniquePaths9、编号63 Unique Paths II10、编号70 Climbing Stairs11、编号122 Best Time to Buy and Sell Stock12、编号123 Best Time to Buy and Sell Stock II 13、编号124 Best Time to Buy and Sell Stock III14、编号131 Surrounded Regions15、编号135 Gas Station16、编号136 Candy17、编号147 LRU Cache**
**1、编号34 Valid Sudoku**
Determine if a Sudoku is valid, according to: Sudoku Puzzles - The Rules.
The Sudoku board could be partially filled, where empty cells are filled with the character '.'.
检查数独。没什么好说的，依次按行，列，九宫检查。
```cpp
class Solution {
public:
    bool isValidSudoku(vector<vector<char> > &board) {
        int m = board.size();
        int n = board[0].size();
        int flagM[m];
        int flagN[n];
        int block[9];
        
        /*Check Row*/
        for(int i = 0; i < m; i++){
            for(int j = 0; j < n; j++) flagN[j] = 0;
            for(int j = 0; j < n; j++){
                int c = board[i][j] - '1';/*Remember to use 1 here*/
                if(board[i][j] != '.'){
                    if(flagN[c] == 1) return false;
                    flagN[c]++; 
                }
            }
        }
        
        /*Check Column*/
        for(int i = 0; i < n; i++){
            for(int j = 0; j < m; j++) flagM[j] = 0;
            for(int j = 0; j < m; j++){
                int c = board[j][i] - '1';
                if(board[j][i] != '.'){
                    if(flagM[c] == 1) return false;    
                    flagM[c]++;
                }
            }
        }
        
        /*Check Block*/
        for(int i1 = 0; i1 < 3; i1++){
            for(int i2 = 0; i2 < 3; i2++){
                for(int j = 0; j < 9; j++) block[j] = 0;
                for(int j = 0; j < 3; j++){
                    for(int k = 0; k < 3; k++){
                        int c = board[j+3*i1][k+3*i2] - '1';
                        if(board[j+3*i1][k+3*i2] != '.'){
                            if(block[c] == 1) return false;
                            block[c]++;
                        }
                    }
                }
            }
        }
        return true;
    }
};
```
**2、编号36 Sudoku Solver**
Write a program to solve a Sudoku puzzle by filling the empty cells.
Empty cells are indicated by the character '.'.
You may assume that there will be only one unique solution.
使用DFS（也就是Backtracking递归）来尝试所有组合。下面解法假设数独矩阵是9*9的。基本就是尝试所有可能性的暴力解法。
```cpp
bool isValid(vector<vector<char> > &board, int x, int y)  
    {  
        int i, j;  
        for (i = 0; i < 9; i++)  
            if (i != x && board[i][y] == board[x][y])  return false; 
            
        for (j = 0; j < 9; j++)  
            if (j != y && board[x][j] == board[x][y])  return false;  
            
        for (i = 3 * (x / 3); i < 3 * (x / 3 + 1); i++)  
            for (j = 3 * (y / 3); j < 3 * (y / 3 + 1); j++)  
                if (i != x && j != y && board[i][j] == board[x][y])  return false;  
        return true;  
    }  
    bool solveSudoku(vector<vector<char> > &board)  
    {  
        for (int i = 0; i < 9; ++i)  
            for (int j = 0; j < 9; ++j){
                if ('.' == board[i][j])  {  
                    for (int k = 1; k <= 9; ++k){  
                        board[i][j] = '0' + k;  
                        if (isValid(board, i, j) && solveSudoku(board)) return true;  
                        board[i][j] = '.';  
                    }  
                    return false;  
                }  
            }  
        return true;  
    }  
};
```
**3、编号41 Trapping Rain Water**
Given n non-negative integers representing an elevation map where the width of each bar is 1, compute how much water it is able to trap after raining.
For example, Given [0,1,0,2,1,0,1,3,2,1,2,1], return 6
建立两个辅助数组，里面存储某点i左边最高和右边最高的高度。最后遍历数组，左右最小值减当前值就是结果。
```cpp
class Solution {
public:
    int trap(int A[], int n) {
        int result = 0;
        int LeftMost[n];
        int RightMost[n];
        
        LeftMost[0] = A[0];
        for(int i = 1; i < n; i++)
            LeftMost[i] = LeftMost[i-1] > A[i-1] ? LeftMost[i-1] : A[i-1];
        
        RightMost[n-1] = A[n-1];
        for(int i = n-2; i >= 0; i--)
            RightMost[i] = RightMost[i+1] > A[i+1] ? RightMost[i+1] : A[i+1];
        
        for(int i = 1; i < n-1; i++)
            if(min(LeftMost[i], RightMost[i]) > A[i])
                result += (min(LeftMost[i], RightMost[i]) - A[i]);
        
        return result;
    }
};
```
**4、编号46 Jump Game**
Given an array of non-negative integers, you are initially positioned at the first index of the array.
Each element in the array represents your maximum jump length at that position.
Determine if you are able to reach the last index.
For example: A = [2,3,1,1,4], return true.
A = [3,2,1,0,4], return false.
贪心算法，每次走最大，如果走到0就不能走了，需要倒退。但发现不用倒退也能通过？
```cpp
class Solution {
public:
    bool canJump(int A[], int n) {
        int index = 0;
        while(index < (n-1)){
            if(A[index] == 0) return false;
            index += A[index];
        }
        return true;
    }
};
```
**5、编号47 Jump Game II **
Each element in the array represents your maximum jump length at that position.
Your goal is to reach the last index in the minimum number of jumps.
For example: Given array A = [2,3,1,1,4]
The minimum number of jumps to reach the last index is 2. (Jump 1 step from index 0 to 1, then 3 steps to the last index.)
依旧贪心去推，贪心的规则就是在能够到达的范围之内，选择一个能够到达最远距离的 点，更新步数，和更新最远到达的范围。如果用DFS的话会超时。
```cpp
class Solution {
public:
    int jump(int A[], int n) {
        int max = 0, lastStepMax = 0, step = 0;  
        for(int i = 0; i < n;)  {  
            if(lastStepMax >= n-1) break;  
            
            while(i <= lastStepMax)  {  
                if((i+A[i]) > max) max = i+A[i];  
                i++;  
            }  
            
            step++;  
            lastStepMax = max;              
        }  
        return step;  
    }
};
```
**6、编号50 N-Queens**
The n-queens puzzle is the problem of placing n queens on an n×n chessboard such that no two queens attack each other.
Given an integer n, return all distinct solutions to the n-queens puzzle.
Each solution contains a distinct board configuration of the n-queens' placement, where 'Q' and '.' both indicate a queen and an empty space respectively.
For example, There exist two distinct solutions to the 4-queens puzzle:
[
 [".Q..",  // Solution 1
  "...Q",
  "Q...",
  "..Q."],
 ["..Q.",  // Solution 2
  "Q...",
  "...Q",
  ".Q.."]
]
用一个place辅助数组来记录每次放的queen的位置。有点难理解。
```cpp
class Solution {  
public:  
    bool check(int row, int* place)    {    
        for (int i = 0; i < row; ++i)    {    
            int diff = abs(place[i] - place[row]);    
            if (diff == 0 || diff == row - i)  return false;    
        }    
        return true;    
    }    
        
    void placeQueens(int row, int n, int &count, int* place,  vector<vector<string> > &result)    {    
        if (row == n)    {  
            vector<string> tmp;  
            for (int i = 0; i < row; ++i)  {  
                string str(n, '.');  
                str[place[i]] = 'Q';  
                tmp.push_back(str);  
            }  
            result.push_back(tmp);  
            return;    
        }    
            
        for (int i = 0; i < n; ++i)    {    
            place[row] = i;    
            if (check(row, place))    
                placeQueens(row+1, n, count, place, result);    
        }    
    }    
    
    vector<vector<string> > solveNQueens(int n) {  
        int* place = new int[n]; /*Use  an array to record the position of the queens*/ 
        vector<vector<string> > result;  
        placeQueens(0, n, count, place, result);   
        return result;  
    }  
};
```
**7、编号52 N-Queens II **
Follow up for N-Queens problem.
Now, instead outputting board configurations, return the total number of distinct solutions.
用一个count变量来记录不同解答的数量。其它和上题一样。
```cpp
class Solution {
public:
    int totalNQueens(int n) {
        /*Use  an array to record the position of the queens*/
        int* place = new int[n]; 
        int count = 0;    
        vector<vector<string> > result;  
        placeQueens(0, n, count, place, result);   
        return count;
    }
    
    bool check(int row, int* place)    {    
        for (int i = 0; i < row; ++i)    {    
            int diff = abs(place[i] - place[row]);    
            if (diff == 0 || diff == row - i)  return false;    
        }    
        return true;    
    }    
        
    void placeQueens(int row, int n, int &count, int* place,  vector<vector<string> > &result)    {    
        if (row == n)    {  
            vector<string> tmp;  
            for (int i = 0; i < row; ++i)  {  
                string str(n, '.');  
                str[place[i]] = 'Q';  
                tmp.push_back(str);  
            }  
            result.push_back(tmp);  
            count++;
            return;    
        }    
            
        for (int i = 0; i < n; ++i)    {    
            place[row] = i;    
            if (check(row, place))    
                placeQueens(row+1, n, count, place, result);    
        }    
    }    
    
};
```
**8、编号61 UniquePaths**
A robot is located at the top-left corner of a m x n grid (marked 'Start' in the diagram below).
The robot can only move either down or right at any point in time. The robot is trying to reach the bottom-right corner of the grid (marked 'Finish' in the diagram below).
How many possible unique paths are there?
DFS递归和迭代可解，但运行超时。因为限制了只能向两个方向移动，使用2维DP可解。不用dp会超时。
```cpp
class Solution {
public:
    int uniquePaths(int m, int n) {  
        if(m == 0 || n == 0)  return 0;  
        int **dp = new int*[m];  
        for(int i = 0; i < m; i++)  dp[i] = new int[n];  
        dp[0][0] = 1;  
        for(int i = 1; i < m; i++)   dp[i][0] = 1;  
        for(int j = 1; j < n; j++)   dp[0][j] = 1;  
        
        for(int i = 1; i < m; i++) 
            for(int j = 1; j < n; j++)  
                dp[i][j] = dp[i][j-1] + dp[i-1][j];  
 
        int res = dp[m-1][n-1];  
        for(int i = 0; i < m; i++)  delete [] dp[i];  
          
        delete [] dp;  
        return res;  
    }
};
```
**9、编号63 Unique Paths II**
Follow up for "Unique Paths":
Now consider if some obstacles are added to the grids. How many unique paths would there be?
An obstacle and empty space is marked as 1 and 0 respectively in the grid.
For example, There is one obstacle in the middle of a 3x3 grid as illustrated below.[
  [0,0,0],
  [0,1,0],
  [0,0,0]
]
The total number of unique paths is 2.
Note: m and n will be at most 100.
同样dp。注意有障碍的地方走不过去，要分开处理就可以了。
```cpp
class Solution {
public:
    int uniquePathsWithObstacles(vector<vector<int> > &obstacleGrid) {
        int m = obstacleGrid.size();
        if(m == 0) return 0;
        int n = obstacleGrid[0].size();
        if(n == 0) return 0;
        if(obstacleGrid[0][0] == 1) return 0;
        /*Initialize*/
        vector<vector<int>> dp;
        for(int i = 0; i < m; i++) {
            vector<int> tmp;
            for(int j = 0; j < n; j++)tmp.push_back(0);
            dp.push_back(tmp);
        }
        
        /*Set first col and row*/
        bool obs = false;
        dp[0][0] = 1;  
        for(int i = 1; i < m; i++)   {
            if(obstacleGrid[i][0] == 1 || obs == true){
              dp[i][0] = 0;
              obs = true;
            } 
            else dp[i][0] = 1;
        }
        obs = false;
        for(int j = 1; j < n; j++)   {
            if(obstacleGrid[0][j] == 1 || obs == true) {
                dp[0][j] = 0;
                obs = true;
            }else dp[0][j] = 1;  
        }
        
        /*Main dp loop*/
        for(int i = 1; i < m; i++) 
            for(int j = 1; j < n; j++){
                if(obstacleGrid[i][j] == 1) dp[i][j] = 0;
                else dp[i][j] = dp[i][j-1] + dp[i-1][j];  
            }
        
        return dp[m-1][n-1];
    }
};
```
**10、编号70 Climbing Stairs**
You are climbing a stair case. It takes n steps to reach to the top.
Each time you can either climb 1 or 2 steps. In how many distinct ways can you climb to the top?
比较容易的dp题。
```cpp
You are climbing a stair case. It takes n steps to reach to the top.
Each time you can either climb 1 or 2 steps. In how many distinct ways can you climb to the top?
class Solution {
public:
    int climbStairs(int n) {
        int dp[n];
        dp[0] = 1;
        dp[1] = 2;
        for(int i = 2; i < n; i++)
            dp[i] = dp[i-1] + dp[i-2];
        
        return dp[n-1];
    }
};
```
**11、编号122 Best Time to Buy and Sell Stock**
Say you have an array for which the ith element is the price of a given stock on day i.
If you were only permitted to complete at most one transaction (ie, buy one and sell one share of the stock), design an algorithm to find the maximum profit.
交易系列的第一题。用一个min变量去找之前最低的价钱，每走一步就减一次。
```cpp
class Solution {
public:
    int maxProfit(vector<int> &prices) {
        if(prices.size() == 0) return 0;
        
        int min = prices[0];
        int max = 0;
        for(int i = 1; i < prices.size(); i++){
            if(prices[i] < min) min = prices[i];
            int tmp = prices[i] - min;
            if(tmp > max) max = tmp;
        }
        
        return max;
    }
};
```
**12、编号123 Best Time to Buy and Sell Stock II **
Say you have an array for which the ith element is the price of a given stock on day i.
Design an algorithm to find the maximum profit. You may complete as many transactions as you like (ie, buy one and sell one share of the stock multiple times). However, you may not engage in multiple transactions at the same time (ie, you must sell the stock
 before you buy again).
这个更简单，每次遇到比前面高的价钱就卖出。
```cpp
class Solution {
public:
    int maxProfit(vector<int> &prices) {
        if(prices.size() == 0) return 0;
        
        int r = 0;
        for(int i = 1; i < prices.size(); i++)
            if(prices[i] > prices[i-1])
                r += (prices[i] - prices[i-1]);
                
        return r;
    }
};
```
**13、编号124 Best Time to Buy and Sell Stock III**
Say you have an array for which the ith element is the price of a given stock on day i.
Design an algorithm to find the maximum profit. You may complete at most two transactions.
Note: You may not engage in multiple transactions at the same time (ie, you must sell the stock before you buy again).
这个比较有难度。可以把数组分成两份，分别求最大，但通不过大数据。在网上搜到一个O(n)解答如下。对于每个i，看左边，还有右边的最大的，然后加起来，选中间最大的。.
```cpp
class Solution {
public:
    int maxProfit(vector<int> &prices) {
        if(prices.size() == 0) return 0;
        
        int f[3] = {0}; //f[0] is never used
        int g[3] = {0}; //g[0] is always zero
        for (int i = 0; i < (prices.size() - 1); ++i) {
                int diff = prices[i+1] - prices[i];
                int m = min(i+1, 2);
                for (int j = m; j >= 1; --j) {
                    f[j] = max(f[j], g[j-1]) + diff;
                    g[j] = max(g[j], f[j]);
                }
        }
        return max(g[1], g[2]);
```
**14、编号131 Surrounded Regions**
Given a 2D board containing 'X' and 'O', capture all regions surrounded by 'X'.
A region is captured by flipping all 'O's into 'X's in that surrounded region .
For example,
X X X X
X O O X
X X O X
X O X X
After running your function, the board should be:
X X X X
X X X X
X X X X
X O X X
用BFS。不过只用从四周一圈点BFS就可以了。因为如果不跟四边接触就肯定受X影响。BFS把所有O设为Y。然后把所有X设为O。最后把Y设为O就可以了。
```cpp
class Solution {
public:
    void solve(vector<vector<char>> &board) {
        int row = board.size();
        if(row == 0) return;
        int col = board[0].size();
        if(col == 0) return;
        
        queue<int> q;    
        for(int i = 0; i < row; i++){
            if(board[i][0]=='O') BFS(board, i, row, 0, col, q);
            if(board[i][col-1]=='O') BFS(board, i, row, col-1, col, q);
        }
        for(int i = 1 ; i< col-1; i++){
            if(board[0][i] == 'O') BFS(board, 0, row, i, col, q);
            if(board[row-1][i] == 'O') BFS(board, row-1, row, i, col, q);
        }
        for(int i = 0; i< row; i++)
            for(int j = 0; j< col; j++)
                if(board[i][j] == 'O')  board[i][j]='X';
                else if(board[i][j] == 'Y')  board[i][j]='O';   
    }   
    
    void BFS(vector<vector<char> >&b, int cr, int row, int cc, int col, queue<int> &q){
        q.push(cr*col+cc);
        b[cr][cc]='Y';
        int x=0, y=0;
        while(!q.empty()){
            x = q.front() / col;
            y = q.front() % col;
            q.pop();
            if(x > 0 && b[x-1][y] == 'O'){
                b[x-1][y]='Y';
                q.push((x-1)*col + y);
            }    
            if(x < row-1 && b[x+1][y] == 'O'){
                b[x+1][y]='Y';
                q.push((x+1)*col + y);
            }    
            if(y > 0 && b[x][y-1] == 'O'){
                b[x][y-1]='Y';
                q.push(x*col + y - 1);
            }    
            if(y < col-1 && b[x][y+1] == 'O'){
                b[x][y+1]='Y';
                q.push(x*col + y + 1);
            }    
        }
        return;    
    }
};
```
**15、编号135 Gas Station**
There are N gas stations along a circular route, where the amount of gas at station i is gas[i].
You have a car with an unlimited gas tank and it costs cost[i] of gas to travel from station i to its next station (i+1). You begin the journey with an empty tank at one of the gas stations.
Return the starting gas station's index if you can travel around the circuit once, otherwise return -1.
Note: The solution is guaranteed to be unique.
两重循环。任取i为起点，用j遍历，如果走不回去就用i+1继续寻找。注意i = j那一句的枝减，要是没有的话通不过大数据。
```cpp
class Solution {
public:
    int canCompleteCircuit(vector<int> &gas, vector<int> &cost) {
        for(int i = 0; i < gas.size(); i++){ //try starting from all nodes
            int tank = 0; 
            for(int j = i; j < i + gas.size(); j++){ //travel in j
                int k = j;
                if(k >= gas.size()) k -= gas.size();
                tank = tank + gas[k] - cost[k];
                if(tank < 0) {
                    i = j; //!!!!!!!!!save time
                    break;
                }
                if(j == i + gas.size() - 1) return i;
            }
        }   
        return -1;
    }
};
```
**16、编号136 Candy**
There are N children standing in a line. Each child is assigned a rating value.
You are giving candies to these children subjected to the following requirements:
Each child must have at least one candy.
Children with a higher rating get more candies than their neighbors.
What is the minimum candies you must give?
一开始没理解题目做错了。小学生的糖只要比旁边的小朋友多就可以了（真好骗呀）。做法是建立辅助数组。从前往后走一次，再从后往前走一次，每次遇到分数高的小朋友就给更多一个糖果，记录放在辅助数组里。最后全部加起来就可以了。
```cpp
class Solution {
public:
    /*Children with a higher rating get more candies than their !!!!!!neighbors!!!!!!*/
    /*[1,2,2] = 4*/
    int candy(vector<int> &ratings) {
        if(ratings.size() == 0) return 0;
        if(ratings.size() == 1) return 1;
        
        vector<int> tmp(ratings.size(), 0);//initial vector
        
        for(int i = 1; i < ratings.size(); i++)
            if(ratings[i] > ratings[i-1]) tmp[i] = tmp[i-1] + 1;
        
        for(int i = ratings.size() - 2; i >= 0 ; i--)
            if(ratings[i] > ratings[i+1]) tmp[i] = max(tmp[i], tmp[i+1] + 1);
        
        int result = 0;
        for(int i = 0; i < ratings.size(); i++) result += tmp[i];
        
        return result + ratings.size();
    }
};
```
**17、编号147 LRU Cache**
Design and implement a data structure for Least Recently Used (LRU) cache. It should support the following operations: get and set.
get(key) - Get the value (will always be positive) of the key if the key exists in the cache, otherwise return -1.
set(key, value) - Set or insert the value if the key is not already present. When the cache reached its capacity, it should invalidate the least recently used item before inserting a new item.
这个不是游戏，但就当跟游戏差不多吧。有两个要点，一是保持最新的cache在上面，而是保持映射。
```cpp
class LRUCache{
public:
    struct CacheEntry{
        int key;
        int value;
        CacheEntry(int k, int v) : key(k), value(v) {}
    };
private:
    int m_capacity;
    list<CacheEntry> m_LRU_cache;// list，保存对象新旧程度的序列。不一定是list，也可以用vector，不过list的好处是已经实现了头尾操作的api
    unordered_map<int, list<CacheEntry>::iterator> hashmap;//保存key和对象位置的映射
public:
    LRUCache(int capacity) {
      m_capacity = capacity;  
    }
    
    int get(int key) {
        if(hashmap.find(key) == hashmap.end()) return -1;
        MoveToHead(key);
        return hashmap[key]->value;
    }
    
    void set(int key, int value) {
        if(hashmap.find(key) == hashmap.end()){ //if new one is not in hashmap
            CacheEntry newItem(key, value);
            if(m_LRU_cache.size() >= m_capacity){//if hashmap is full, remove the oldest one, both hashmap and cache list
                hashmap.erase(m_LRU_cache.back().key); 
                m_LRU_cache.pop_back();
            }
            m_LRU_cache.push_front(newItem);//add new element
            hashmap[key] = m_LRU_cache.begin();
        }else{
            hashmap[key]->value = value;//just update element
            MoveToHead(key);
        }
    }
    
    void MoveToHead(int key){
        auto updateEntry = *hashmap[key];
        m_LRU_cache.erase(hashmap[key]);
        m_LRU_cache.push_front(updateEntry);
        hashmap[key] = m_LRU_cache.begin();
    }
};
```
参考文献：
http://oj.leetcode.com/problems/
