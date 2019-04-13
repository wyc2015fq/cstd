
# linux两个文件交集，合并，去重 - bitcarmanlee的博客 - CSDN博客


2018年09月05日 16:25:54[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：917



## 1.两个文件的交集，并集
```python
cat file1 file2 |
```
```python
sort
```
```python
| uniq >
```
```python
result
```
```python
cat file1 file2 |
```
```python
sort
```
```python
| uniq -d >
```
```python
result
```
```python
cat file1 file2 |
```
```python
sort
```
```python
| uniq -u >
```
```python
result
```
第一条命名求两个文件的并集，如果有重复的行只保留一行。
第二条命令求两个文件的交集，即两个文件中都有的行。
第三条命令求两个文件的差集，即只有一个文件中有的行。
## 2.两个文件合并
```python
cat file1 file2 >
```
```python
result
```
```python
paste file1 file2 >
```
```python
result
```
第一条命令是追击的方式，如果file1有n行，file2有m行，result为n+m行。
第二条命令是一个文件的内容在左边，一个文件命令在右边。
## 3.单个文件去重
```python
sort
```
```python
file
```
```python
|uniq
```
```python
sort
```
```python
file
```
```python
|uniq -u
```
第一条命令将重复的多行变为一行！
第二条命令是将有重复的行全部去掉！

