# svn结合git一起使用 - Arthur的随笔 - CSDN博客
2011年11月07日 22:28:48[largetalk](https://me.csdn.net/largetalk)阅读数：15309
公司是用svn的，并且没办法改变了，但git好处那么多，就两个一起用。
和公司代码库的交换还是用svn， git用于本地和同事之间的代码提交
先svn checkout 代码
然后在代码目录git init创建代码仓库
编写.gitingore或.git/info/exclude将不需要加入代码库的文件加进去
然后git add .
再 git commit -m "blablabla....."
这就ok了。
一般开发会新建一个分支，如：git branch dev
然后git checkout dev转到这个分支进行开发
在需要提交代码到svn时，先切到某个分支，然后提交，再切回来，工作就可以又继续了，哈哈，不错吧
git在合并代码等时候好处真是太多了，分支消耗也少，不像svn得branch就是把整个目录拷贝一次，太慢了。
然后在ubuntu下，我得git是没有配色的，看起来傻傻得，用下面命名给git加点颜色，呵呵
git config --global color.status auto
git config --global color.branch auto
git config --global color.diff auto
git config --global color.grep auto
git config --global color.interactive auto
git config --global color.ui auto
这些都是默认颜色，不过对我来说基本已经够用了。
ps. 显示git提交tree的命令是：
git log --graph --oneline --all
or：
 export LESS="-R"
 git log --graph --pretty=oneline --abbrev-commit
git alias:
vim ~/.gitconfig and add
```
[alias]  
  
　　co = checkout  
  
　　ci = commit -a  
  
　　st = status  
  
　　br = branch  
  
　　oneline = log --pretty=oneline --since='2 days ago'  
  
　　onelog = log -p -1
```
