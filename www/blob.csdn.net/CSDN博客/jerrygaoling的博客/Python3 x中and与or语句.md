
# Python3.x中and与or语句 - jerrygaoling的博客 - CSDN博客


2018年03月22日 11:15:10[jerrygaoling](https://me.csdn.net/jerrygaoling)阅读数：4139


## Python3.x中实现
在没有其他限定时，and的优先级大于or
在x or y的值只可能是x或y。 当x为true时是x,x为false时是y
在x and y的值只可能是x或y。 当x为true时是y,x为false时是x
例：`3 or 9 and 4`: 先算`9 and 4`, 9为`true`, 值为`4`. 再算`3 or 4`,`3`为`true`,**值为**：`3`
```python
a
```
```python
=
```
```python
3
```
```python
or
```
```python
9
```
```python
and
```
```python
4
```
```python
print(
```
```python
a
```
```python
)
```
![实现结果](//img-blog.csdn.net/20180322111227247?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
`(3 or 9 )and 4`: 先算`3 or 9`, 3为`true`, 值为`3`. 再算`3 and 4`,`3`为`true`,**值为**：`4`
```python
a
```
```python
= (
```
```python
3
```
```python
or
```
```python
9
```
```python
)
```
```python
and
```
```python
4
```
```python
print(
```
```python
a
```
```python
)
```
![这里写图片描述](//img-blog.csdn.net/20180322111329412?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L2plcnJ5Z2FvbGluZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

