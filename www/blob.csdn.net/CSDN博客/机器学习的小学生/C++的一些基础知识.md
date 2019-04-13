
# C++的一些基础知识 - 机器学习的小学生 - CSDN博客


2018年03月11日 08:44:19[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：73个人分类：[C/C++																](https://blog.csdn.net/xuluhui123/article/category/1705293)



### back_inserter
```python
std
```
```python
::
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
v1(
```
```python
3
```
```python
,
```
```python
10
```
```python
);
```
```python
std
```
```python
::
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
v2(
```
```python
4
```
```python
,
```
```python
9
```
```python
);
```
```python
std
```
```python
::copy(v1.begin(), v1.end(), v2.begin());
```
```python
std
```
```python
::
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
v3(
```
```python
1
```
```python
);
```
```python
//下列这行代码会导致程序崩溃，因为目标容器v3的大小必须大于等于源容器的大小v1
```
```python
//std::copy(v1.begin(), v1.end(), v3.begin());
```
```python
//为了解决这个问题可以使用std::back_inserter()函数。
```
```python
std
```
```python
::copy(v1.begin(), v1.end(),
```
```python
std
```
```python
::back_inserter(v3));
```
`参考文献：http://blog.csdn.net/analogous_love/article/details/51218934`

