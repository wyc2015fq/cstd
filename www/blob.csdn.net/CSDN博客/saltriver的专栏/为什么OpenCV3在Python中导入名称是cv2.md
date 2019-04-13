
# 为什么OpenCV3在Python中导入名称是cv2 - saltriver的专栏 - CSDN博客


2017年10月06日 17:02:57[saltriver](https://me.csdn.net/saltriver)阅读数：9096


我们来看一下：
```python
import
```
```python
cv3
print(cv3.__version__)
```
输出报错：
```python
Traceback (most recent
```
```python
call
```
```python
last
```
```python
):
ImportError:
```
```python
No
```
```python
module
```
```python
named
```
```python
'cv3'
```
再来看看：
```python
import
```
```python
cv2
print(cv2.__version__)
```
输出：
```python
3.3.0
```
为什么OpenCV3在Python中包名称是cv2，而不是cv3？
实际上，”cv2”中的”2”并不表示OpenCV的版本号。我们知道，OpenCV是基于C/C++的，”cv”和”cv2”表示的是底层C API和C++API的区别，”cv2”表示使用的是C++API。这主要是一个历史遗留问题，是为了保持向后兼容性。

