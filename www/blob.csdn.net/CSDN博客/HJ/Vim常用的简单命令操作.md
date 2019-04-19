# Vim常用的简单命令操作 - HJ - CSDN博客
2018年12月09日 21:55:43[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：61
个人分类：[=====计算机基础====																[Linux](https://blog.csdn.net/feizaoSYUACM/article/category/7442535)](https://blog.csdn.net/feizaoSYUACM/article/category/7888041)
## 三种模式：命令模式、插入模式、编辑模式
```bash
输入vim/vi filename 进入文本的命令模式
输入i/a/o 进入文本的插入模式
输入:[cmd] 进入文本的编辑模式
```
## 常用命令
### 1、插入命令
```
a/i 在光标所在字符后/前插入
A/I 在光标所在行尾/首插入
o 在光标下插入新行
O 在光标上插入新行
```
### 2、定位命令
```
:set nu/nonu 设置/取消行号
gg/G 到第一行/最后一行
nG/:n 到第n行
$/0 移动至行尾/首
```
### 3、删除命令
```
x/nx 删除光标所在处字符/后n个字符
dd/ndd 删除光标所在行/n行
D 删除光标所在处到行尾内容
:n1,n2d 删除指定范围的行
```
### 4、复制/剪切、粘贴命令
```
yy/nyy 复制光标所在行/n行
dd/ndd 剪切光标所在行/n行
p/P 粘贴在光标所在行下/行上
```
### 5、搜索、替换、恢复命令
```
/string 搜索指定字符串，:set ic 表示搜索时忽略大小写
n 搜索指定字符串的下一个出现位置
:%s/old/new/g 全文替换指定字符串，g表示不询问，相应的c表示询问是否替换
```
### 6、保存、退出命令
```
:q! 不保存退出
:w 保存
:wq 保存退出
```
