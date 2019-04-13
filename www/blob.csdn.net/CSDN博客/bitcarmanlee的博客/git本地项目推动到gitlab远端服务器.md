
# git本地项目推动到gitlab远端服务器 - bitcarmanlee的博客 - CSDN博客


2018年07月10日 17:45:13[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：1231


本地已经有了现成的项目，需要将此项目push到远端gitlab的服务器上。具体操作步骤如下：
## 1.在gitlab上创建好对应的project
首先，我们现在gitlab服务器上创建git@xxx.xxx.xxx:xxx/xxx.git项目，xxx.git是具体的项目名称。
## 2.初始化本地项目
cd到本地项目目录，然后执行
```python
git init
```
这样就初始化了本地git仓库
## 3.关联远程仓库
```python
git remote
```
```python
add
```
```python
origin git@xxx
```
```python
.xxx
```
```python
.xxx
```
```python
:xxx/xxx
```
```python
.git
```
## 4.提交代码
```python
git
```
```python
add
```
```python
.
git commit -m
```
```python
"init project"
```
```python
git
```
```python
push
```
```python
-u origin master
```
## 5.注意事项
使用`git add .`的时候，经常会将一些不必要的内容带进去。这个时候需要将不必要的内容撤销。由于此时还没有提交所以不存在HEAD版本，不能使用 git reset HEAD命令。
此时，可以使用`git rm -r --cached .`撤销之前的add内容。

