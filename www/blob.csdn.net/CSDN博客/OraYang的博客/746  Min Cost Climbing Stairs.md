
# 746. Min Cost Climbing Stairs - OraYang的博客 - CSDN博客

2018年03月21日 10:40:27[OraYang](https://me.csdn.net/u010665216)阅读数：279



# 题目
On a staircase, the`i`-th step has some non-negative cost`cost[i]`assigned (0 indexed).
Once you pay the cost, you can either climb one or two steps. You need to find minimum cost to reach the top of the floor, and you can either start from the step with index 0, or the step with index 1.
**Example 1:**
**Input:**cost = [10, 15, 20]**Output:**15**Explanation:**Cheapest is start on cost[1], pay that cost and go to the top.
**Example 2:**
**Input:**cost = [1, 100, 1, 1, 1, 100, 1, 1, 100, 1]**Output:**6**Explanation:**Cheapest is start on cost[0], and only step on 1s, skipping cost[3].
**Note:**
costwill have a length in the range[2, 1000].
Everycost[i]will be an integer in the range[0, 999].


# 思路
本题是个典型的dp问题。写出对应的状态子方程即可。
dp[i]=min(dp[i−1]+cost[i−1],dp[i−2]+cost[i−2]);$dp[i] = min(dp[i-1]+cost[i-1],dp[i-2]+cost[i-2]);$
# 代码
```python
class
```
```python
Solution {
```
```python
public
```
```python
:
```
```python
int
```
```python
minCostClimbingStairs(
```
```python
vector
```
```python
<
```
```python
int
```
```python
>
```
```python
& cost) {
```
```python
if
```
```python
(cost.size()<=
```
```python
2
```
```python
)
```
```python
return
```
```python
0
```
```python
;
```
```python
vector
```
```python
<
```
```python
int
```
```python
>
```
```python
dp(cost.size()+
```
```python
1
```
```python
,
```
```python
0
```
```python
);
```
```python
for
```
```python
(
```
```python
int
```
```python
i=
```
```python
0
```
```python
;i<cost.size();i++)
        {
            dp[i] = cost[i];
        }
        dp[
```
```python
0
```
```python
] =
```
```python
0
```
```python
;
        dp[
```
```python
1
```
```python
] =
```
```python
0
```
```python
;
```
```python
for
```
```python
(
```
```python
int
```
```python
i=
```
```python
2
```
```python
;i<=cost.size();i++)
        {
            dp[i] = min(dp[i-
```
```python
1
```
```python
]+cost[i-
```
```python
1
```
```python
],dp[i-
```
```python
2
```
```python
]+cost[i-
```
```python
2
```
```python
]);
        }
```
```python
return
```
```python
dp[cost.size()];
    }
};
```

