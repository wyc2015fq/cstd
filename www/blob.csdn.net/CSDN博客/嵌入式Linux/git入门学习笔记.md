
# git入门学习笔记 - 嵌入式Linux - CSDN博客

2016年06月15日 18:27:54[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：251个人分类：[Git																](https://blog.csdn.net/weiqifa0/article/category/1438593)



## 安装和配置
安装
```python
#sudo
```
```python
apt
```
```python
-get
```
```python
install git
```
```python
#sudo
```
```python
apt
```
```python
-get
```
```python
install git
```
```python
-doc
```
```python
git
```
```python
-svn
```
```python
git
```
```python
-email
```
```python
git
```
```python
-gui
```
```python
gitk
```
如果是第一次使用git，需要进行如下设置：
```python
#git
```
```python
config
```
```python
--
```
```python
global
```
```python
user
```
```python
.
```
```python
name
```
```python
"weiqifa"
```
```python
#git
```
```python
config
```
```python
--
```
```python
global
```
```python
user
```
```python
.
```
```python
email
```
```python
"w@wei.com"
```
## git常用操作
创建一个自己的版本库
```python
＃mkdir -p project
```
```python
#cd project
```
```python
#git init
```
这时候就可以向这个代码仓库加如代码了
```python
#cd project
```
```python
#touch weiqifa.txt
```
```python
#git add weiqifa.txt
```
```python
#git commit -m "WeiqifaFirstInit"
```
## 同步其他分支的某个文件
git show yan3_dev:scripts/resource_tool>scripts/resource_tool
把yan3_dev 分支下的resource_tool  同步到当前分支下
## 打包项目源码
git clone –bare kernel/  这样生成kernel.git   这里会包含.git文件夹
git clone kernel.git/ 这样来解包
## 打补丁
git diff ./ > *.patch
patch -p1 < xxx.patch
## 查看历史分支图
git log –graph –all

