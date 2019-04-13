
# 152. Maximum Product Subarray - OraYang的博客 - CSDN博客

2017年12月14日 14:36:16[OraYang](https://me.csdn.net/u010665216)阅读数：261所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



# 题目
Find the contiguous subarray within an array (containing at least one number) which has the largest product.
For example, given the array [2,3,-2,4],
the contiguous subarray [2,3] has the largest product = 6.
# 思路
这道题目是典型的连续子序列的计算问题，接下来我们将使用三种算法来解决这个问题，分别是：浅显算法；两个平方算法；扫描算法。接下来结合代码来详细说明。
# 代码
## 浅显算法
最浅显的程序就是对所有满足0<=i<=j
```python
//solution 1:最浅显的程序就是对所有满足0<=i<=j<n的整数进行迭代。
```
```python
//对每个整数对，程序都要计算nums[i..j]的总和，并检验该总和是否大于迄今为止的最大总和。
```
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
maxProduct(
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
& nums) {
```
```python
int
```
```python
maxsofar = INT_MIN, product;
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
;i<nums.size();i++)
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
j=i;j<nums.size();j++)
            {
                product =
```
```python
1
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
k=i;k<=j;k++)
                    product = product*nums[k];
                maxsofar = max(maxsofar,product);
            }
```
```python
return
```
```python
maxsofar;
    }
};
```
上述代码简洁、直观并且易于理解，但是嵌套使用了三个for循环。因为也被称为**立方**算法。该方法时间复杂度很高，因此提交超时了。
![152](https://img-blog.csdn.net/20171214114608770?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 平方算法
在前面一个算法中，我们在计算nums[i..j]的总乘积与前面已计算出的nums[i..j-1]总乘积密切相关。因为可与利用这个关系的到一个平方算法。
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
maxProduct(
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
& nums) {
```
```python
int
```
```python
maxsofar = INT_MIN, product;
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
;i<nums.size();i++)
        {
            product =
```
```python
1
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
j=i;j<nums.size();j++)
            {
                product = product*nums[j];
                maxsofar = max(maxsofar,product);
            }
        }
```
```python
return
```
```python
maxsofar;
    }
};
```
该方法运用了两个for循环，时间复杂度依旧比较高，提交结果依旧超时。
## 扫描算法
我们现在采用操作数组最简单的算法：从数组最左端（元素nums[0]）开始扫描，一直到最右端（元素nums[n-1]）为止，并记下所遇到的最大乘积子向量。最大乘积的初始值设为nums[0]。假设我们解决了nums[0..i-1]的问题，那么如何将其扩展为包含nums[i]的问题呢？我们使用类似于分治算法的原理：**前i个元素中，最大总乘积子数组要么在前i-1个元素中（我们将其存储在maxsofar中），要么其结束位置为i（我们将其存储在maxendinghere中）。
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
maxProduct(
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
& nums) {
```
```python
int
```
```python
maxsofar = nums[
```
```python
0
```
```python
];
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
,maxend=maxsofar,minend=maxsofar;i<nums.size();i++)
        {
```
```python
if
```
```python
(nums[i]<
```
```python
0
```
```python
)
                swap(maxend,minend);
            maxend = max(nums[i],maxend*nums[i]);
            minend = min(nums[i],minend*nums[i]);
            maxsofar = max(maxsofar,maxend);            
        }
```
```python
return
```
```python
maxsofar;
    }
};
```
上述代码我们用两个变量maxend与minend来保存以nums[i]为结尾的最大值与最小值（因为负负得正，所以也要保存最小值）

