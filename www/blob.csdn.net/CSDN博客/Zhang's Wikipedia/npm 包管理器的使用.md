
# npm 包管理器的使用 - Zhang's Wikipedia - CSDN博客


2018年05月12日 11:22:04[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：112



## 1. 权限问题
[Warning “root” does not have permission to access the dev dir  · Issue \#454 · nodejs/node-gyp · GitHub](https://github.com/nodejs/node-gyp/issues/454)
Warning “root” does not have permission to access the dev dir ，使用 sudo 仍然抛出这样的权限异常，可在 npm 的参数中增加： –unsafe-perm，如下：
```python
$
```
```python
sudo
```
```python
npm
```
```python
install
```
```python
-
```
```python
-
```
```python
unsafe
```
```python
-
```
```python
perm
```
```python
-
```
```python
-
```
```python
verbose
```
```python
-
```
```python
g
```
```python
sails
```

