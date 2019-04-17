# pandas where函数用法 - 墨流觞的博客 - CSDN博客





2018年09月22日 23:04:42[墨氲](https://me.csdn.net/dss_dssssd)阅读数：2325标签：[python																[pandas](https://so.csdn.net/so/search/s.do?q=pandas&t=blog)](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[Python数据处理及可视化](https://blog.csdn.net/dss_dssssd/article/category/8077244)







- 
Series.where(cond, other=nan, inplace=False, axis=None, level=None, errors=‘raise’, try_cast=False, raise_on_error=None)

如果 cond 为真，保持原来的值，否则替换为other， inplace为真标识在原数据上操作，为False标识在原数据的copy上操作。

other must be the same shape as self: other的形状必须与self相同。
mask 函数和 where 作用刚好相反。

```python
```python
s = pd.Series(range(5))
  s.where(s > 1, 10)
  0    10.0
  1    10.0
  2    2.0
  3    3.0
  4    4.0

  s.mask(s > 1, 10)
  0    0.0
  1    1.0
  2    10.0
  3    10.0
  4    10.0
  df = pd.DataFrame(np.arange(10).reshape(-1, 2), columns=['A', 'B'])
  m = df % 3 == 0
  # df.where(m, np.array([1,2,3,4,5]).reshape(-1, 5))  #此句话报错
  df.where(m, -df)
  A  B
  0  0 -1
  1 -2  3
  2 -4 -5
  3  6 -7
  4 -8  9
```
```
- 
np.ceil 向上取整

The ceil of the scalar x is the smallest integer i, such that i >= x.

函数返回最小的整数i，满足i >= x

```python
```python
a = np.array([-1.7, -1.5, -0.2, 0.2, 1.5, 1.7, 2.0])
  np.ceil(a)
  # array([-1., -1., -0.,  1.,  2.,  2.,  2.])
```
```


利用以上两个函数将数据集按照地区收入分类：

```python
```python
housing["income_cat"] = np.ceil(housing["median_income"] / 1.5)
    housing["income_cat"].where(housing["income_cat"] < 5, 5.0, inplace=True)
```
```
- 






