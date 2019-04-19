# leetcode(24):Unique Paths（不同路径） - 刘炫320的博客 - CSDN博客
2019年04月14日 18:52:56[刘炫320](https://me.csdn.net/qq_35082030)阅读数：21
所属专栏：[算法编程习题解答（java版）](https://blog.csdn.net/column/details/16358.html)[算法思想培养指南](https://blog.csdn.net/column/details/36524.html)
这周我们重温一下经典的格子世界。
# 1. Unique Paths（不同路径 62）
## 1.1问题描述
> 
A robot is located at the top-left corner of a m x n grid (marked ‘Start’ in the diagram below).
The robot can only move either down or right at any point in time. The robot is trying to reach the bottom-right corner of the grid (marked ‘Finish’ in the diagram below).
How many possible unique paths are there?
一个机器人位于一个 m x n 网格的左上角 （起始点在下图中标记为“Start” ）。
机器人每次只能向下或者向右移动一步。机器人试图达到网格的右下角（在下图中标记为“Finish”）。
问总共有多少条不同的路径？
## 1.2例子
Example
> 
输入: m = 3, n = 2
输出: 3
解释:
从左上角开始，总共有 3 条路径可以到达右下角。
1.向右 -> 向右 -> 向下
2.向右 -> 向下 -> 向右
3.向下 -> 向右 -> 向右
输入: m = 7, n = 3
输出: 28
## 1.3思路
这是非常简单的一道题，最早出现于高中的排列与组合中。即若有一个 m x n 的网格，从对角线的一端到达另一端的路径有多少种？根据高中的知识可得，只需要选取(m-1)+(n-1)步数中的(m-1)个步骤为竖着即可。
## 1.4代码
```java
class Solution {
    public int uniquePaths(int m, int n) {
        if (m>n){
			return uniquePaths(n, m);
		}
		return (int) Combination(m-1, m+n-2);
    }
    public long factorial(int k){
    	long sum=1;
    	for(int i=2;i<=k;i++){
    		sum*=i;
    	}
    	return sum;
    }
    public long Combination(int m, int n){
    	//m在上，n在下
    	return m <= n ? factorial(n) / (factorial(m) * factorial((n - m))) : 0;
    }
}
```
对于较小的数是没有问题的（在leetcode中通过数目为37 / 62）。但这里有一个最大的问题，就是如果乘积过大是无法计算的，因为是int类型，尤其时n=100时，便已无法计算。那就只能使用递归方法求解了。
## 1.5 更进一步
我们可以从1 X 1的格子开始递推，一直到m*n的格子，情况如下：
![在这里插入图片描述](https://img-blog.csdn.net/20150407150210507)
由以上得到推导公式，这就转换为一个动态规划问题，简单的递归调用即可解：
$S(m,n)=S(m,n-1)+S(m-1,n)$
## 1.6 代码
```java
class Solution {
    public int uniquePaths(int m, int n) {
        if (m == 1 && n == 1) {
			return 1;
		}
		if (m == 1 && n > 1) {
			return 1;
		}
		if (n == 1 && m > 1) {
			return 1;
		}
		// 递归调用
		return uniquePaths(m - 1, n) + uniquePaths(m, n - 1);
    }
}
```
## 1.7 更进一步
上述代码仍然存在问题，即递归调用不能够太深（在leetcode中通过数目为61 / 62），因此我们转为非递归的二维矩阵即可解，首先确定二维数组，然后确定边界条件，最后根据递推公式即可获得。
## 1.8 代码
```java
class Solution {
    public int uniquePaths(int m, int n) {
        if(m<=1 && n<=1){
			return 1;
		}
		int[][] array=new int[m][n];
		for(int rol=0;rol<m;rol++){
			array[rol][0]=1;
		}
		for(int col=0;col<n;col++){
			array[0][col]=1;
		}
		for(int rol=1;rol<m;rol++){
			for(int col=1;col<n;col++){
				array[rol][col]=array[rol-1][col]+array[rol][col-1];
			}
		}
		return array[m-1][n-1];
    }
}
```
## 1.9 更进一步
二维矩阵可以进一步缩小其空间复杂度，转换为1维矩阵。
## 1.10代码
```java
class Solution {
    public int uniquePaths(int m, int n) {
        int d[] = new int[n];
        d[0] = 1;
        for(int i = 0; i < m; i++){
            for(int j = 1; j < n; j++) {
                d[j] = d[j] + d[j - 1];
            }
        }
        return d[n - 1];
    }
}
```
# 2. Unique Paths II (不同路径2 63)
## 2.1 问题描述
> 
A robot is located at the top-left corner of a m x n grid (marked ‘Start’ in the diagram below).
The robot can only move either down or right at any point in time. The robot is trying to reach the bottom-right corner of the grid (marked ‘Finish’ in the diagram below).
Now consider if some obstacles are added to the grids. How many unique paths would there be?
一个机器人位于一个 m x n 网格的左上角 （起始点在下图中标记为“Start” ）。
机器人每次只能向下或者向右移动一步。机器人试图达到网格的右下角（在下图中标记为“Finish”）。
现在考虑网格中有障碍物。那么从左上角到右下角将会有多少条不同的路径？
## 2.2 例子
Example
> 
Input:
[
[0,0,0],
[0,1,0],
[0,0,0]
]
Output: 2
Explanation:
There is one obstacle in the middle of the 3x3 grid above.
There are two ways to reach the bottom-right corner:
1.Right -> Right -> Down -> Down
2.Down -> Down -> Right -> Right
## 2.3 思路
和第一题的不同之处在于有障碍物不允许我们走了，那么我们只需要在每一步递归的时候，条件判断一下即可。
## 2.4 代码
```java
class Solution {
    public int uniquePathsWithObstacles(int[][] obstacleGrid) {
       int m=obstacleGrid.length;
        int n=obstacleGrid[0].length;
        if (m<1 || n<1){
        	return 0;
        }
        if(m==1 && n==1){
        	return 1-obstacleGrid[0][0];
		}
		int[][] array=new int[m][n];
		for(int rol=0;rol<m && obstacleGrid[rol][0]==0;rol++){
			array[rol][0]=1;
		}
		for(int col=0;col<n&& obstacleGrid[0][col]==0;col++){
			array[0][col]=1;
		}
		for(int rol=1;rol<m;rol++){
			for(int col=1;col<n;col++){
			//递归条件判断
				if (obstacleGrid[rol][col]==0){
					array[rol][col]=array[rol-1][col]+array[rol][col-1];
				}
				else{
					array[rol][col]=0;
				}
				
			}
		}
		return array[m-1][n-1];
    }
    
}
```
# 3. Unique Paths III (不同路径3 980)
## 3.1 问题描述
> 
On a 2-dimensional grid, there are 4 types of squares:
1 represents the starting square.  There is exactly one starting square.
2 represents the ending square.  There is exactly one ending square.
0 represents empty squares we can walk over.
-1 represents obstacles that we cannot walk over.
Return the number of 4-directional walks from the starting square to the ending square, that walk over every non-obstacle square exactly once.
## 3.2 例子
> 
Example 1:
Input: [[1,0,0,0],[0,0,0,0],[0,0,2,-1]]
Output: 2
Explanation: We have the following two paths:
1.(0,0),(0,1),(0,2),(0,3),(1,3),(1,2),(1,1),(1,0),(2,0),(2,1),(2,2)
2.(0,0),(1,0),(2,0),(2,1),(1,1),(0,1),(0,2),(0,3),(1,3),(1,2),(2,2)
Example 2:
Input: [[1,0,0,0],[0,0,0,0],[0,0,0,2]]
Output: 4
Explanation: We have the following four paths:
1.(0,0),(0,1),(0,2),(0,3),(1,3),(1,2),(1,1),(1,0),(2,0),(2,1),(2,2),(2,3)
2.(0,0),(0,1),(1,1),(1,0),(2,0),(2,1),(2,2),(1,2),(0,2),(0,3),(1,3),(2,3)
3.(0,0),(1,0),(2,0),(2,1),(2,2),(1,2),(1,1),(0,1),(0,2),(0,3),(1,3),(2,3)
4.(0,0),(1,0),(2,0),(2,1),(1,1),(0,1),(0,2),(0,3),(1,3),(1,2),(2,2),(2,3)
Example 3:
Input: [[0,1],[2,0]]
Output: 0
Explanation:
There is no path that walks over every empty square exactly once.
Note that the starting and ending square can be anywhere in the grid.
## 3.3思路
这题是上一题的加强版，这里连起点和终点都是不确定的，并且，对于路径也增加了限制，即所有的能走的路径都只能走一次。这就是最普通的格子世界，这种小规模的场景，一般使用深度搜索进行求解，就像最笨的人类一样，把所有的路径都试一遍就到达了。而如果这个场景相当大的话，其搜索的复杂度相当大，这时候，一般都是使用强化学习进行求解。
首先要做的就是确定开始位置和我们要走的步数。然后，在每一步时都依次进行以下判断：
- 是不是越界了
- 是不是不能走的
- 是否到达终点并且步数正好
如果没有走到终点，则首先将当前位置置为不可达，其次减少一个要走的步数。然后递归，递归后再复原原有地图坐标，即可。
## 3.4 代码
```java
class Solution {
    //X的操作
	static int[] dx = new int[]{0, 1, -1, 0};
	//Y的操作
	static int[] dy = new int[]{1, 0, 0, -1};
    //最终的结果
	static int result = 0;
    public static int uniquePathsIII2(int[][] grid) {
        int n = grid.length;
        int m = grid[0].length;
        int x = -1;
        int y = -1;
        int t = 1;
        //统计需要走的步数和开始位置。
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (grid[i][j] == 0) {
                    t++;
                } else if (grid[i][j] == 1){
                   x = i; 
                   y = j;
                }
            }
        }
        //深度优先搜索
        dfs(grid, x, y, t);
        return result;
    }
    public static void dfs(int[][] grid, int x, int y, int n) {
        //如果越界了
    	if (x < 0 || y < 0 || x >= grid.length || y >= grid[0].length) {
            return;
        }
    	//如果是不能走的
        if (grid[x][y] == -1) {
            return ;
        }
        //如果是到达终点，如果是0则将结果++
        if (grid[x][y] == 2) {
            if (n == 0) {
                result++;
            }
            return;
        }
        //如果没有到达终点，则将该点设置为不可走的位置。
        grid[x][y] = -1;
        for (int i = 0; i < 4; i++) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            dfs(grid, nx, ny, n - 1);
        }
        //再把当前值复原
        grid[x][y] = 0;
    }
}
```
# 4.小结
本次主要对不同路径的3个题目进行了拓展，最主要的是我们复习了动态规划和代理探索的常规框架，为以后的学习打下基础。
