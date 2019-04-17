# 动态规划初步续_leetcode刷题总结 - zhuzhuxia183的博客 - CSDN博客





2019年01月16日 15:29:09[喵小姐的邻居小小朱](https://me.csdn.net/zhuzhuxia183)阅读数：34
所属专栏：[数据结构与算法(leetcode)](https://blog.csdn.net/column/details/33915.html)









## 紧接着上一篇，继续撕～

上一篇已经举例了不少，可能大家会有一种感觉是，动态规划计算顺序十分的直接，从前往后算就行啦，但是这可能是一种错觉哦～

比如这一题：
- 
8、地下城游戏   [https://leetcode-cn.com/problems/dungeon-game/](https://leetcode-cn.com/problems/dungeon-game/)

一些恶魔抓住了公主（P）并将她关在了地下城的右下角。地下城是由 M x N 个房间组成的二维网格。我们英勇的骑士（K）最初被安置在左上角的房间里，他必须穿过地下城并通过对抗恶魔来拯救公主。

骑士的初始健康点数为一个正整数。如果他的健康点数在某一时刻降至 0 或以下，他会立即死亡。

有些房间由恶魔守卫，因此骑士在进入这些房间时会失去健康点数（若房间里的值为负整数，则表示骑士将损失健康点数）；其他房间要么是空的（房间里的值为 0），要么包含增加骑士健康点数的魔法球（若房间里的值为正整数，则表示骑士将增加健康点数）。

为了尽快到达公主，骑士决定每次只向右或向下移动一步。

编写一个函数来计算确保骑士能够拯救到公主所需的最低初始健康点数。

例如，考虑到如下布局的地下城，如果骑士遵循最佳路径 右 -> 右 -> 下 -> 下，则骑士的初始健康点数至少为 7。


这个题目，我觉得还是很有意思，因为你可以去计算一波，不出意外的话，从前往后来进行dp求解，应该是无法求解的噢，但是是可以从后往前算

上代码：

```
public int calculateMinimumHP(int[][] dungeon) {
		if (dungeon.length==0|| dungeon[0].length==0)
            return 0;
        int length1 = dungeon.length;
        int length2 = dungeon[0].length;
        int[][] dp = new int[length1][length2];
        dp[length1-1][length2-1] = Math.max(1-dungeon[length1-1][length2-1],1);

        for (int i = length1-2; i >=0 ; i--) {
            dp[i][length2-1] = Math.max(dp[i+1][length2-1]-dungeon[i][length2-1],1);
        }

        for (int i = length2-1; i >=0 ; i--) {
            dp[length1-1][i] = Math.max(dp[length1-1][i+1]-dungeon[length1-1][i],1);
        }

        for (int i = length1-2; i >=0 ; i--) {
            for (int j = length2-2; j >=0 ; j--) {
                int value1 = Math.max(dp[i+1][j]-dungeon[i][j],1);
                int value2 = Math.max(dp[i][j+1]-dungeon[i][j],1);
                dp[i][j] = Math.min(value1,value2);
            }
        }
        return dp[0][0];
	}
```

这里为什么可以从后往前算可以呢？是因为如果油不够了，我们可以继续在前面的时候的加油，这是满足时间发展逻辑的，但是反过来想一想，如果从前往后算，这样可以求解吗？no
- 
9、打家劫舍  [https://leetcode-cn.com/problems/house-robber/](https://leetcode-cn.com/problems/house-robber/)

你是一个专业的小偷，计划偷窃沿街的房屋。每间房内都藏有一定的现金，影响你偷窃的唯一制约因素就是相邻的房屋装有相互连通的防盗系统，如果两间相邻的房屋在同一晚上被小偷闯入，系统会自动报警。

给定一个代表每个房屋存放金额的非负整数数组，计算你在不触动警报装置的情况下，能够偷窃到的最高金额。

示例 1:

输入: [1,2,3,1]

输出: 4

解释: 偷窃 1 号房屋 (金额 = 1) ，然后偷窃 3 号房屋 (金额 = 3)。

偷窃到的最高金额 = 1 + 3 = 4 。

示例 2:
输入: [2,7,9,3,1]

输出: 12

解释: 偷窃 1 号房屋 (金额 = 2), 偷窃 3 号房屋 (金额 = 9)，接着偷窃 5 号房屋 (金额 = 1)。
这个题目，属于从前往后和从后往前都是一致的

```
public int rob(int[] nums) {
		int length = nums.length;
        if (length==0)
            return 0;
        int[] dp = new int[length];
        dp[0] = nums[0];

        for (int i = 1; i < length; i++) {
            dp[i] = Math.max(nums[i]+(i-2>=0?dp[i-2]:0),nums[i-1]+(i-3>=0?dp[i-3]:0));
        }
        return dp[length-1];
	}
```
- 
10、买卖股票的最佳时机 III [https://leetcode-cn.com/problems/best-time-to-buy-and-sell-stock-iii/](https://leetcode-cn.com/problems/best-time-to-buy-and-sell-stock-iii/)

给定一个数组，它的第 i 个元素是一支给定的股票在第 i 天的价格。

设计一个算法来计算你所能获取的最大利润。你最多可以完成 两笔 交易。

注意: 你不能同时参与多笔交易（你必须在再次购买前出售掉之前的股票）。

示例 1:

输入: [3,3,5,0,0,3,1,4]

输出: 6

解释: 在第 4 天（股票价格 = 0）的时候买入，在第 6 天（股票价格 = 3）的时候卖出，这笔交易所能获得利润 = 3-0 = 3 。

随后，在第 7 天（股票价格 = 1）的时候买入，在第 8 天 （股票价格 = 4）的时候卖出，这笔交易所能获得利润 = 4-1 = 3 。

示例 2:
输入: [1,2,3,4,5]

输出: 4

解释: 在第 1 天（股票价格 = 1）的时候买入，在第 5 天 （股票价格 = 5）的时候卖出, 这笔交易所能获得利润 = 5-1 = 4 。

注意你不能在第 1 天和第 2 天接连购买股票，之后再将它们卖出。

因为这样属于同时参与了多笔交易，你必须在再次购买前出售掉之前的股票。

示例 3:
输入: [7,6,4,3,1]

输出: 0

解释: 在这个情况下, 没有交易完成, 所以最大利润为 0。
这个题目呢，也是需要用动态规划来做的，和上面不一样的地方在于，之前的转换公式似乎都比较简单，但是这里的转换公式相对会难一些，比如说这里我们由于再确定购买的次数问题时，区分两个变量一个是 global[] 和local[]两个变量，看代码哈～

```
public int maxProfit(int[] prices) {
        if (prices.length==0)
            return 0;
        int length = prices.length;
        int[][] local = new int[3][length];
        int[][] global = new int[3][length];

        for (int i = 1; i < length; i++) {
            int diff = prices[i] - prices[i-1];
            for (int j = 1; j < 3; j++) {
                local[j][i] = Math.max(global[j-1][i-1]+diff,local[j][i-1]+diff);
				//local[j][i] = Math.max(global[j-1][i-1]+Math.max(0,diff),local[j][i-1]+diff);
                global[j][i] = Math.max(global[j][i-1],local[j][i]);
            }
        }
        return global[2][length-1];
	}
```

上面的代码有两个变量，一个是local[i][j]表示 最后一次是在j天完成的时候完成第i次交易，global[i][j]表示 是在j天内完成第i次交易（注意，不一定最后一天完成第 i次交易）然后外层循环是往前走一天，内层循环是多买一次，然后转换公式是

```
local[j][i] = Math.max(global[j-1][i-1]+diff,local[j][i-1]+diff);
global[j][i] = Math.max(global[j][i-1],local[j][i]);
```

这两个公式的意思，就是

1、 在j天时 完成i次交易 能够取得最大值 = Math.max( 在 j-1天内 完成 i-1次交易的情况下的最大值+j天的交易,  在j-1天时 完成第i次交易的最大值+j天的交易);

2、 在j天内 完成第i次交易 能够取得最大值  =  Math.max(在j天内 第i-1次交易 能够取得最大值,   在j天时完成i次交易 能够取得最大值 );
## 当然这里的问题在于，题目要求是在最多只能卖 2次，这样是不是说我们还得在global[0][length-1]、global[1][length-1]、global[2][length-1]中取较大值呢，当然是可以的，但是我们也可以转变思路，把：

local[i][j]表示 最后一次是在j天时顶多完成第i次交易

global[i][j]表示 是在j天内顶多完成第i次交易

这样我们发现这样的话，

```
local[j][i] = Math.max(global[j-1][i-1]+diff,local[j][i-1]+diff);
global[j][i] = Math.max(global[j][i-1],local[j][i]);
```

这个代码依然成立，含义变为

1、 在j天时 顶多 完成i次交易 能够取得最大值 = Math.max( 在 j-1天内 顶多 完成 i-1次交易的情况下的最大值+j天的交易,  在j-1天时 顶多 完成第i次交易的最大值+j天的交易);

2、 在j天内 顶多 完成第i次交易 能够取得最大值  =  Math.max(在j天内 顶多 完成第i-1次交易 能够取得最大值,   j天时 顶多 完成第i次交易 能够取得最大值 );

嗯，骚年们，是不是感觉到现在为止，上面的代码就解释通了呢～

当然我们会发现有的时候我们并不是需要 第几天的那个维度，即可以把空间缩减到以为的数组,代码如下：

```
public int maxProfit(int[] prices) {
        if (prices.length==0)
            return 0;
        int length= prices.length;
        int[] local = new int[3];
        int[] global =  new int[3];
        for (int i = 1; i < length ; i++) {
            int diff = prices[i] - prices[i-1];
            for (int j = 2; j>=1; j--) {
                local[j] = Math.max(global[j-1],local[j])+diff;
                global[j] = Math.max(global[j],local[j]);
            }
        }
        return global[2];
	}
```

PS：这里为什么要从2-1递减呢，我们可以对比两个函数，上面用空间进行了保存，下面没有一个天数的维度，所以从代码的层面上我们为了避免覆盖导致的影响，所以，反着来～
- 
11、买卖股票的最佳时机 III [https://leetcode-cn.com/problems/best-time-to-buy-and-sell-stock-iii/](https://leetcode-cn.com/problems/best-time-to-buy-and-sell-stock-iii/)

给定一个数组，它的第 i 个元素是一支给定的股票在第 i 天的价格。

设计一个算法来计算你所能获取的最大利润。你最多可以完成 k 笔交易。

注意: 你不能同时参与多笔交易（你必须在再次购买前出售掉之前的股票）。

示例 1:

输入: [2,4,1], k = 2

输出: 2

解释: 在第 1 天 (股票价格 = 2) 的时候买入，在第 2 天 (股票价格 = 4) 的时候卖出，这笔交易所能获得利润 = 4-2 = 2 。

示例 2:
输入: [3,2,6,5,0,3], k = 2

输出: 7

解释: 在第 2 天 (股票价格 = 2) 的时候买入，在第 3 天 (股票价格 = 6) 的时候卖出, 这笔交易所能获得利润 = 6-2 = 4 。

随后，在第 5 天 (股票价格 = 0) 的时候买入，在第 6 天 (股票价格 = 3) 的时候卖出, 这笔交易所能获得利润 = 3-0 = 3 。
额，只要上面的理解透彻了，这道题也是分分钟的事把，but我们要注意一个删减噢，那就是如果最高的次数大于 天数的一半的话，可以退化成一个很简答的问题，就是只要有diff>0 就 累计即可，代码给出

```
public int maxProfit(int k, int[] prices) {
        if (prices.length==0)
            return 0;

        if(k >= prices.length/2){
            return quickSolver(prices);
        }

        int length = prices.length;
        int[] global = new int[k+1];
        int[] local = new int[k=1];
        for (int i = 1; i < length; i++) {
            int diff = prices[i]-prices[i-1];
            for (int j = k; j >=1 ; j--) {
                local[j] = Math.max(local[j-1]+diff,global[j-1]);
                global[j] = Math.max(global[j],local[j]);
            }
        }
        return global[k];

    }

    private int quickSolver(int[] prices) {
        int result_all=0;
        int result_one_day=0;
        for (int i = 0; i < prices.length-1; i++) {
            result_one_day = prices[i+1]-prices[i];
            if (result_one_day>0)
                result_all+=result_one_day;
        }
        return result_all;
    }
```

到这里的话，我觉得题目确实撕了不少了，其实总结起来，就是

1、想办法把问题分成几个子状态

2、想想办法能不能知道子状态之间的一个状态转移公式

3、如果找到了，恭喜你
有些书上列出来，说动态规划的方法运用注意两个性质：

1、最优子结构

2、无后向性

有兴趣的可以去翻阅资料，鉴于本人悟性有限，有兴趣可以网上或者找书看看～
嗯，多思考，多总结，总不会错，先溜一波～
















