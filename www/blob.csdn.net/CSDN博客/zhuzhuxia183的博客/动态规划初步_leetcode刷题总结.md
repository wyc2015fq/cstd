# 动态规划初步_leetcode刷题总结 - zhuzhuxia183的博客 - CSDN博客





2019年01月15日 21:43:25[喵小姐的邻居小小朱](https://me.csdn.net/zhuzhuxia183)阅读数：53
个人分类：[动态规划																[数据结构和算法](https://blog.csdn.net/zhuzhuxia183/article/category/8608092)](https://blog.csdn.net/zhuzhuxia183/article/category/8614640)

所属专栏：[数据结构与算法(leetcode)](https://blog.csdn.net/column/details/33915.html)








## 动态规划，我之前也是不了解的，嘻嘻嘻

只是后面发现在相关搜索题目中，如果只需要求解的树木或者相关数值问题时候，往往使用动态规划会比深度遍历搜索高效很多。jio得还可以好吧

因为考虑到大家对于动态规划不是很熟悉，我们先来一个简单的小练习来上手下动态规划。

## 对于爬楼梯的问题，相信大家都比较熟悉吧，一次可以走一步，也可以走两步，请问走到第100步，有多少种走法？
- 首先，这个问题，很容易想到 f(100) = (f99) + f(98) 吧，ok，然后如果有回溯的基础的话，去用递归求解应该是一种很容易的求解方法好吧,

```
public int f(int level){
    if(level==1)
        return 1;
    if(level==2)
        return 2;
    else:
        return f(level-1)+f(level-2);

}
```
- 但是呢，动态规划是一种更加高效的方法好吧，代码是这样的：

```
public int f(int length){
    int[] result = new int[length+1];
    result[1] = 1;
    result[2] = 2;
    for(int i=3;i<length+1;i++){
        result[i] = result[i-1]+result[i-2];
    }
    return result[length];
}
```

简单的来对比以下，这两个有啥区别呢？其实递归的解法的话,求return f(level-1)+f(level-2);的过程中，可以认为f(level-2)和f(level-1)的计算过程有着很多一样的计算过程，就是说同样的计算步骤计算重复了两遍，造成的一个问题是，使得浪费了时间复杂度，emmmmm,所以我们通过使用动态规划的办法，可以减少这种不必要的运算～

```
ps: 有些书中提到，其实在递归求解中加入一个int[]的全局变量，在遍历之前先判断之前有没有运算过，也可以到达这种效果，称之为备忘录式的递归，但是个人觉得，没有必要吧，直接使用动态规划就可以了嘛～
```

好，是不是觉得有点味道，好，我们来撕撕几道题目。
- 
1、跳跃游戏 [https://leetcode-cn.com/problems/jump-game/](https://leetcode-cn.com/problems/jump-game/)

给定一个非负整数数组，你最初位于数组的第一个位置。

数组中的每个元素代表你在该位置可以跳跃的最大长度。

判断你是否能够到达最后一个位置。

示例 1:

输入: [2,3,1,1,4]

输出: true

解释: 从位置 0 到 1 跳 1 步, 然后跳 3 步到达最后一个位置。

示例 2:

输入: [3,2,1,0,4]

输出: false

解释: 无论怎样，你总会到达索引为 3 的位置。但该位置的最大跳跃长度是 0 ， 所以你永远不可能到达最后一个位置。
这个题目，我们可以用一个数组int[] dp来记录每一步当前能够到的最远的距离，最后再判断int[nums.length-1]是不是>=nums.length-1即可～代码：

```
public boolean canJump(int[] nums) {
        int[] dp = new int[nums.length];
        dp[0] = nums[0];
        for (int i = 1; i <nums.length; i++) {
            if (dp[i-1]<i)
                return false;
            else
                dp[i] = Math.max(dp[i-1],nums[i]+i);
        }
        return dp[nums.length-1]>=nums.length-1;
    }
```

但是分析上面的代码，不难发现，其实我们是可以优化，因为最后其实只是用到了最后一个

```
public boolean canJump(int[] nums) {
        int max_length = nums[0];

        for (int i = 1; i <nums.length; i++) {
            if (max_length<i)
                return false;
            else
                max_length = Math.max(max_length,nums[i]+i);
                if (max_length>=nums.length-1)
                    return true;
        }
        return max_length>=nums.length-1;
    }
```

这种就只用了一个空间来记录最大长度，jio得还可以～
- 
2、跳跃游戏 II  [https://leetcode-cn.com/problems/jump-game-ii/](https://leetcode-cn.com/problems/jump-game-ii/)

给定一个非负整数数组，你最初位于数组的第一个位置。

数组中的每个元素代表你在该位置可以跳跃的最大长度。

你的目标是使用最少的跳跃次数到达数组的最后一个位置。

示例:

输入: [2,3,1,1,4]

输出: 2

解释: 跳到最后一个位置的最小跳跃数是 2。

从下标为 0 跳到下标为 1 的位置，跳 1 步，然后跳 3 步到达数组的最后一个位置。

嗯，其实这道题如果用贪心会更加的好，但是为了dp而dp一把，呜～
```
public int jump2(int[] nums) {
        //假设到达位置i所需要的步数
		int[] dp = new int[nums.length];
        dp[0]= 0;
        int cur_max_length = 0;
        int next_max_length = nums[0];
        for (int i = 1; i <nums.length; i++) {
            if (i<=cur_max_length){
                dp[i] = dp[i-1];
                next_max_length = Math.max(next_max_length,nums[i]+i);
            }else {
                if (next_max_length>=i){
                    cur_max_length = next_max_length;
                    dp[i]=dp[i-1]+1;
                    next_max_length = Math.max(next_max_length,nums[i]+i);
                }else {
                    return -1;
                }
            }
        }
        return dp[nums.length-1];
    }
```

嗯，这道题，如果用贪心的话，可以这样：

```
public int jump(int[] nums) {
        int length = nums.length;
        int cur_max_length = 0;
        int next_max_length =  nums[0];
        int res =0;
        for (int i = 1; i < length; i++) {
            if (i>cur_max_length){
                res++;
                cur_max_length = next_max_length;
            }
            next_max_length = Math.max(next_max_length,nums[i]+i);
        }
        return res;
    }
```
- 
4、不同路径 [https://leetcode-cn.com/problems/unique-paths/](https://leetcode-cn.com/problems/unique-paths/)

一个机器人位于一个 m x n 网格的左上角 （起始点在下图中标记为“Start” ）。

机器人每次只能向下或者向右移动一步。机器人试图达到网格的右下角（在下图中标记为“Finish”）。

问总共有多少条不同的路径？

例如，上图是一个7 x 3 的网格。有多少可能的路径？

说明：m 和 n 的值均不超过 100。

示例 1:

输入: m = 3, n = 2

输出: 3

解释:

从左上角开始，总共有 3 条路径可以到达右下角。- 向右 -> 向右 -> 向下
- 向右 -> 向下 -> 向右
- 向下 -> 向右 -> 向右

示例 2:

输入: m = 7, n = 3

输出: 28
这个题目，嗯，我们可以用一个二维的dp数组来求解，上代码：

```
public int uniquePaths(int m, int n) {
        //创建一个m*n的数组矩阵来存储
        int [][] result = new int[m+1][n+1];

        for (int i = 1; i < m+1; i++) {
            result[i][1]=1;
        }

        for (int i = 1; i < n+1; i++) {
            result[1][i]=1;
        }

        for (int i = 2; i <m+1 ; i++) {
            for (int j = 1; j <n+1; j++) {
                result[i][j] = result[i-1][j]+result[i][j-1];
            }
        }
        return result[m][n];
    }
```
- 5、 不同路径 II [https://leetcode-cn.com/problems/unique-paths-ii/](https://leetcode-cn.com/problems/unique-paths-ii/)

现在考虑网格中有障碍物。那么从左上角到右下角将会有多少条不同的路径？

网格中的障碍物和空位置分别用 1 和 0 来表示。

说明：m 和 n 的值均不超过 100。

示例 1:

输入:

[

[0,0,0],

[0,1,0],

[0,0,0]

]

输出: 2

解释:

3x3 网格的正中间有一个障碍物。

从左上角到右下角一共有 2 条不同的路径：- 向右 -> 向右 -> 向下 -> 向下
- 向下 -> 向下 -> 向右 -> 向右

```
public int uniquePathsWithObstacles(int[][] obstacleGrid) {
      int height = obstacleGrid.length;
        int width =obstacleGrid[0].length;
        int[][] result = new int[height+1][width+1];
        boolean flag = true;
        for (int i = 1; i < height+1; i++) {
            if (obstacleGrid[i-1][0]==1){
                flag=false;
            }
            result[i][1]=flag?1:0;
        }

        flag = true;
        for (int i = 1; i < width+1; i++) {
            if (obstacleGrid[0][i-1]==1){
                flag=false;
            }
            result[1][i]=flag?1:0;
        }

        for (int i = 2; i < height+1; i++) {
            for (int j = 2; j < width+1; j++) {
                if (obstacleGrid[i-1][j-1]==1){
                    result[i][j]=0;
                }else {
                    result[i][j]=result[i-1][j]+result[i][j-1];
                }
            }
        }

        return result[height][width];  
    }
```

基本上差不多的～
- 
6、 编辑距离 [https://leetcode-cn.com/problems/edit-distance/](https://leetcode-cn.com/problems/edit-distance/)

给定两个单词 word1 和 word2，计算出将 word1 转换成 word2 所使用的最少操作数 。

你可以对一个单词进行如下三种操作：

插入一个字符

删除一个字符

替换一个字符

示例 1:
输入: word1 = “horse”, word2 = “ros”

输出: 3

解释:

horse -> rorse (将 ‘h’ 替换为 ‘r’)

rorse -> rose (删除 ‘r’)

rose -> ros (删除 ‘e’)

示例 2:
输入: word1 = “intention”, word2 = “execution”

输出: 5

解释:

intention -> inention (删除 ‘t’)

inention -> enention (将 ‘i’ 替换为 ‘e’)

enention -> exention (将 ‘n’ 替换为 ‘x’)

exention -> exection (将 ‘n’ 替换为 ‘c’)

exection -> execution (插入 ‘u’)

这道题比之前的题目会绕一点，就是得想到用二维数组dp[length1][length2]来表示``	public int minDistance(String word1, String word2) {
        int length1 = word1.length();
        int length2 = word2.length();
        int[][] result = new int[length1+1][length2+1];
        result[0][0] = 0;
        for (int i = 1; i <length1+1;i++) {
            result[i][0] = result[i-1][0]+1;
        }
        for (int i = 1; i <length2+1; i++) {
            result[0][i] = result[0][i-1]+1;
        }

        for (int i = 1; i < length1+1 ; i++) {
            for (int j = 1; j < length2+1; j++) {
                int distance1 = result[i-1][j-1]+(word1.charAt(i-1)==word2.charAt(j-1)?0:1);
                int distance2 = result[i-1][j]+1;
                int distance3 = result[i][j-1]+1;
                result[i][j] = Math.min(Math.min(distance1,distance2),distance3);
            }
        }
        return result[length1][length2];
    }
- 


7、交错字符串 [](https://leetcode-cn.com/problems/interleaving-string/)https://leetcode-cn.com/problems/interleaving-string/

给定三个字符串 s1, s2, s3, 验证 s3 是否是由 s1 和 s2 交错组成的。

示例 1:

输入: s1 = “aabcc”, s2 = “dbbca”, s3 = “aadbbcbcac”

输出: true

示例 2:

输入: s1 = “aabcc”, s2 = “dbbca”, s3 = “aadbbbaccc”

输出: false

这里我们也可以利用一个二维的dp[s1.length][s2.length]来进行记录：``	public boolean isInterleave(String s1, String s2, String s3) {
        int length1 = s1.length();
        int length2 = s2.length();
        int length3 = s3.length();
        if (length1+length2!=length3)
            return false;
        boolean[][] dp = new boolean[length1+1][length2+1];
        dp[0][0] = true;
        for (int i = 1; i <=length1; i++) {
            dp[i][0] = dp[i-1][0]&&(s1.charAt(i-1)==s3.charAt(i-1));
        }


        for (int i = 1; i <=length2; i++) {
            dp[0][i] = dp[0][i-1]&&(s2.charAt(i-1)==s3.charAt(i-1));
        }

        for (int i = 1; i <=length1; i++) {
            for (int j = 1; j <= length2; j++) {
                boolean down = dp[i][j-1]&&(s2.charAt(j-1)==s3.charAt(i+j-1));
                boolean right = dp[i-1][j]&& (s1.charAt(i-1)==s3.charAt(i+j-1));
                dp[i][j] = down || right;
            }
        }
        return dp[length1][length2];
    }


这种方法也有点像是走迷宫一样，是不是越说越神似勒！～哈哈哈哈，还好玩不

其实，动态规划的内容还有些，但是我要回家了，找时间再补补吧～















