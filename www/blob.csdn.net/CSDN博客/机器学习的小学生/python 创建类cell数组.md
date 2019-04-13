
# python 创建类cell数组 - 机器学习的小学生 - CSDN博客


2017年09月18日 10:44:38[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：4337


When you do this:
```python
a = np.array(
```
```python
[[np.array([[2, 2]]), np.array([[3, 3]])]]
```
```python
)
```
the final call to np.array actually concatenates the inner two, so you get one array at the end:
```python
>>> a
array(
```
```python
[[[[2, 2]],
        [[3, 3]]]]
```
```python
)
>>> a.shape
(
```
```python
1
```
```python
,
```
```python
2
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
2
```
```python
)
```
But to mimic a cell array you want to basically have an array of arrays. You can acheive this by setting dtype=object, but you must create the array and set the elements separately to avoid the automatic merging.
```python
three = array(
```
```python
[[array([[2, 2, 2]]), array([[3, 3]])]]
```
```python
)
two = np.empty(three.shape, dtype=object)
two[
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
] = np.array(
```
```python
[[2,2]]
```
```python
)
two[
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
,
```
```python
0
```
```python
] = np.array(
```
```python
[[3,3]]
```
```python
)
```
Then:
```python
sio.savemat(
```
```python
'two.mat'
```
```python
, {
```
```python
'two'
```
```python
:
```
```python
two
```
```python
})
```
to see what they look like:
```python
>>> two
array(
```
```python
[[[array([[2, 2]])],
        [array([[3, 3]])]]
```
```python
], dtype=object)
>>> two.shape
(
```
```python
1
```
```python
,
```
```python
2
```
```python
,
```
```python
1
```
```python
)
```
Note that I may have gotten confused about your desired shape, since you have so many nested brackets, so you might have to reshape some of this, but the idea should hold regardless.
例如：
```python
npose =
```
```python
5
```
```python
nsmile =
```
```python
2
```
```python
poseSmile_cell = np.empty((npose,nsmile),dtype=object)
```
```python
for
```
```python
i
```
```python
in
```
```python
range(
```
```python
5
```
```python
):
```
```python
for
```
```python
k
```
```python
in
```
```python
range(
```
```python
2
```
```python
):
        poseSmile_cell[i,k] = np.zeros((
```
```python
4
```
```python
,
```
```python
4
```
```python
))
```
```python
print
```
```python
poseSmile_cell.shape
```
参考文献：
[https://stackoverflow.com/questions/19797822/creating-matlab-cell-arrays-in-python](https://stackoverflow.com/questions/19797822/creating-matlab-cell-arrays-in-python)

