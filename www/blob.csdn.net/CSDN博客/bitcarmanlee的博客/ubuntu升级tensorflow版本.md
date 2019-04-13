
# ubuntu升级tensorflow版本 - bitcarmanlee的博客 - CSDN博客


2018年09月17日 15:44:15[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：950


机器中的tensorflow版本有点老旧，想升级一下，发现用
```python
pip
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
upgrade
```
```python
-
```
```python
-
```
```python
ignore
```
```python
-
```
```python
installed
```
```python
tensorflow
```
升级并没有奏效
于是使用
```python
pip uninstall tensorflow
```
先将老版本卸载，然后再重新安装：
```python
pip
```
```python
install
```
```python
tensorflow
```
不过发现装上去的还是原来的旧版本。
找了一下解决方案：
```python
pip install tensorflow
```
```python
=
```
```python
1.6
```
后面接上要安装的版本号即可。

