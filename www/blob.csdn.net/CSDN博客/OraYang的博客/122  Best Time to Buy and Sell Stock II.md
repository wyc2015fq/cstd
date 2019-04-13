
# 122. Best Time to Buy and Sell Stock II - OraYang的博客 - CSDN博客

2018年03月19日 17:45:08[OraYang](https://me.csdn.net/u010665216)阅读数：243所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



# 题目
Say you have an array for which the*i*thelement is the price of a given stock on day*i*.
Design an algorithm to find the maximum profit. You may complete as many transactions as you like (ie, buy one and sell one share of the stock multiple times). However, you may not engage in multiple transactions at the same time (ie, you must sell the stock before you buy again).

# 思路
因为本题可以买卖多次，所以很简单，直接看代码
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
maxProfit(
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
& prices) {
```
```python
//diff
```
```python
int
```
```python
N = prices.size();
```
```python
if
```
```python
(N<=
```
```python
1
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
diff;
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
1
```
```python
;i<N;i++)
        {
            diff.push_back(prices[i]-prices[i-
```
```python
1
```
```python
]);
        }
```
```python
//扫描算法
```
```python
int
```
```python
profits=
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
0
```
```python
;i<diff.size();i++)
        {
```
```python
if
```
```python
(diff[i]>
```
```python
0
```
```python
)
                profits+=diff[i];
        }
```
```python
return
```
```python
profits;
    }
};
```

