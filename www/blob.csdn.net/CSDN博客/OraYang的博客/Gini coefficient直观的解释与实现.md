
# Gini coefficient直观的解释与实现 - OraYang的博客 - CSDN博客

2017年11月14日 12:10:07[OraYang](https://me.csdn.net/u010665216)阅读数：4749所属专栏：[机器学习](https://blog.csdn.net/column/details/16605.html)



## 引言
大家在机器学习中经常会看到基尼系数的词汇，有时候在做比赛的时候，有些赛题的Scoring Metric就是基尼系数。我们去Google或者Baidu，得到的都是些不甚满意的经济学相关的解释。那么在机器学习、数据挖掘领域，基尼系数在实际的应用场景中又该如何解释以及如何实现呢？
## 基尼系数的经济学解释
首先，我们先看一张从Wiki上找来的经典图片：
![Gini](https://img-blog.csdn.net/20171114105238088?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20171114105238088?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
基尼系数是一个分布不平衡程度的度量。它被定义成大小在0到1之间的比值：分子是均匀分布直线与洛伦兹曲线之间的面积，分母是均匀分布直线下方的面积。它是由意大利统计学家Corrado Gini提出并于1912年发表论文：”Variability and Mutability”。
[

](https://img-blog.csdn.net/20171114105238088?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 基尼系数的计算
[
](https://img-blog.csdn.net/20171114105238088?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)首先我们直接构造赛题结果：真实数据与预测数据
[

](https://img-blog.csdn.net/20171114105238088?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```python
predictions =
```
```python
[
```
```python
0.9
```
```python
,
```
```python
0.3
```
```python
,
```
```python
0.8
```
```python
,
```
```python
0.75
```
```python
,
```
```python
0.65
```
```python
,
```
```python
0.6
```
```python
,
```
```python
0.78
```
```python
,
```
```python
0.7
```
```python
,
```
```python
0.05
```
```python
,
```
```python
0.4
```
```python
,
```
```python
0.4
```
```python
,
```
```python
0.05
```
```python
,
```
```python
0.5
```
```python
,
```
```python
0.1
```
```python
,
```
```python
0.1
```
```python
]
```
```python
actual =
```
```python
[
```
```python
1
```
```python
,
```
```python
1
```
```python
,
```
```python
1
```
```python
,
```
```python
1
```
```python
,
```
```python
1
```
```python
,
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
,
```
```python
0
```
```python
,
```
```python
0
```
```python
,
```
```python
0
```
```python
,
```
```python
0
```
```python
,
```
```python
0
```
```python
,
```
```python
0
```
```python
,
```
```python
0
```
```python
,
```
```python
0
```
```python
]
```
[
](https://img-blog.csdn.net/20171114105238088?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)参考Wiki上关于财富基尼系数计算公式的定义：
In some cases, this equation can be applied to calculate the Gini coefficient without direct reference to the Lorenz curve. For example, (taking y to mean the income or wealth of a person or household): For a population uniform on the values yi, i = 1 to n, indexed in non-decreasing order (yi ≤ yi+1):

$$
G = \frac{1}{n}(n+1-2\frac{\sum_{i=1}^n(n+1-i)y_i}{\sum_{i=1}^ny_i})\tag{$1$}
$$
上面的话我通俗翻译下：在某些情况下，我们能够不直接参考洛伦兹曲线来计算出基尼系数。比如，（假设y代表某人或某个家庭的财富值）：序列$y_i$是非递减序列。那么序列$y_i$就代表着从穷人到富人的排列顺序。因此基尼系数的公式就是：

$$
G = \frac{1}{n}(n+1-2\frac{\sum_{i=1}^n(n+1-i)y_i}{\sum_{i=1}^ny_i})\tag{$1$}
$$
那么这个公式我在这里将它拆分解释下：
> n代表y的个数

> \sum_{i=1}^ny_i
> 代表总财富值

> \sum_{i=1}^n(n+1-i)y_i
> 代表财富值的累计求和

### 1.数据转换
在这里我们并没有穷人到富人的数据序列，我们可以将预测值从小到大排列。
```python
# Sort the actual values by the predictions
```
```python
data = zip(actual, predictions)
sorted_data = sorted(data, key=
```
```python
lambda
```
```python
d: d[
```
```python
1
```
```python
])
sorted_actual = [d[
```
```python
0
```
```python
]
```
```python
for
```
```python
d
```
```python
in
```
```python
sorted_data]
print(
```
```python
'Sorted Actual Values'
```
```python
, sorted_actual)
```
```python
[out]
```
```python
Sorted
```
```python
Actual
```
```python
Values
```
```python
[0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1]
```
### 2.累计求和
在这里我们对排序后的真实值累计求和：
```python
# Sum up the actual values
```
```python
cumulative_actual = np.cumsum(sorted_actual)
cumulative_index = np.arange(
```
```python
1
```
```python
, len(cumulative_actual)+
```
```python
1
```
```python
)
plt.plot(cumulative_index, cumulative_actual)
plt.xlabel(
```
```python
'Cumulative Number of Predictions'
```
```python
)
plt.ylabel(
```
```python
'Cumulative Actual Values'
```
```python
)
plt.show()
```
![cum_sum](https://img-blog.csdn.net/20171114112543165?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20171114112543165?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
上图显示的折线就与我们从wiki上找来的图片中的洛伦兹曲线相对应。
[

](https://img-blog.csdn.net/20171114112543165?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### 3.Normalization
[
](https://img-blog.csdn.net/20171114112543165?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)接下来我们将数据Normalization到0，1之间。并画出45度线。
[

](https://img-blog.csdn.net/20171114112543165?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```python
cumulative_actual_shares = cumulative_actual / sum(actual)
cumulative_index_shares = cumulative_index / len(predictions)
```
```python
# Add (0, 0) to the plot
```
```python
x_values = [
```
```python
0
```
```python
] + list(cumulative_index_shares)
y_values = [
```
```python
0
```
```python
] + list(cumulative_actual_shares)
```
```python
# Display the 45° line stacked on top of the y values
```
```python
diagonal = [x - y
```
```python
for
```
```python
(x, y)
```
```python
in
```
```python
zip(x_values, y_values)]
plt.stackplot(x_values, y_values, diagonal)
plt.xlabel(
```
```python
'Cumulative Share of Predictions'
```
```python
)
plt.ylabel(
```
```python
'Cumulative Share of Actual Values'
```
```python
)
plt.show()
```
[
](https://img-blog.csdn.net/20171114112543165?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![normalization](https://img-blog.csdn.net/20171114113251897?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### 4.计算橙色区域面积
我们使用线性代数库scipy，求得橙色区域面积：
```python
fy = scipy.interpolate.interp1d(x_values, y_values)
blue_area, _ = scipy.integrate.quad(fy,
```
```python
0
```
```python
,
```
```python
1
```
```python
, points=x_values)
orange_area =
```
```python
0.5
```
```python
- blue_area
print(
```
```python
'Orange Area: %.3f'
```
```python
% orange_area)
```
```python
[
```
```python
out
```
```python
] Orange Area:
```
```python
0.189
```
### 5.最大可能的基尼系数
前面我们是按照预测值对真实值排序，得到一个基尼系数；现在我们按照真实值给真实值排序，得到最大可能的基尼系数：
```python
cumulative_actual_shares_perfect = np.cumsum(sorted(actual)) / sum(actual)
y_values_perfect = [
```
```python
0
```
```python
] + list(cumulative_actual_shares_perfect)
```
```python
# Display the 45° line stacked on top of the y values
```
```python
diagonal = [x - y
```
```python
for
```
```python
(x, y)
```
```python
in
```
```python
zip(x_values, y_values_perfect)]
plt.stackplot(x_values, y_values_perfect, diagonal)
plt.xlabel(
```
```python
'Cumulative Share of Predictions'
```
```python
)
plt.ylabel(
```
```python
'Cumulative Share of Actual Values'
```
```python
)
plt.show()
```
```python
# Integrate the the curve function
```
```python
fy = scipy.interpolate.interp1d(x_values, y_values_perfect)
blue_area, _ = scipy.integrate.quad(fy,
```
```python
0
```
```python
,
```
```python
1
```
```python
, points=x_values)
orange_area =
```
```python
0.5
```
```python
- blue_area
print(
```
```python
'Orange Area: %.3f'
```
```python
% orange_area)
```
![maxgini](https://img-blog.csdn.net/20171114114305406?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```python
[
```
```python
out
```
```python
] Orange Area:
```
```python
0.300
```
## 数据挖掘中的Scoring Metric的实现
在这里我们封装好基尼系数的函数，可用来作为比赛中的打分函数。
```python
def
```
```python
gini
```
```python
(actual, pred)
```
```python
:
```
```python
assert
```
```python
(len(actual) == len(pred))
    all = np.asarray(np.c_[actual, pred, np.arange(len(actual))], dtype=np.float)
    all = all[np.lexsort((all[:,
```
```python
2
```
```python
], -
```
```python
1
```
```python
* all[:,
```
```python
1
```
```python
]))]
    totalLosses = all[:,
```
```python
0
```
```python
].sum()
    giniSum = all[:,
```
```python
0
```
```python
].cumsum().sum() / totalLosses
    giniSum -= (len(actual) +
```
```python
1
```
```python
) /
```
```python
2.
```
```python
return
```
```python
giniSum / len(actual)
```
```python
def
```
```python
gini_normalized
```
```python
(actual, pred)
```
```python
:
```
```python
return
```
```python
gini(actual, pred) / gini(actual, actual)

gini_predictions = gini(actual, predictions)
gini_max = gini(actual, actual)
ngini= gini_normalized(actual, predictions)
print(
```
```python
'Gini: %.3f, Max. Gini: %.3f, Normalized Gini: %.3f'
```
```python
% (gini_predictions, gini_max, ngini))
```
```python
[
```
```python
out
```
```python
] Gini:
```
```python
0.189
```
```python
, Max. Gini:
```
```python
0.300
```
```python
, Normalized Gini:
```
```python
0.630
```
## 总结
关于Gini系数的pdf文章，请戳：[传送门](http://download.csdn.net/download/u010665216/10117480)

[
](https://img-blog.csdn.net/20171114112543165?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDY2NTIxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
