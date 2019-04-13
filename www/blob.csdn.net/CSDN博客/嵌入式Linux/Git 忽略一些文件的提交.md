
# Git 忽略一些文件的提交 - 嵌入式Linux - CSDN博客

2015年03月12日 10:05:54[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：839


首先

git init 初始化一个git 代码仓库
git add .  向这个仓库提东西

git config --global core.excludesfile .gitignore  用.gitignore  这个文件来忽略不比较的内容

vi .gitignore

```python
/Music/res/
```
对这个文件下的内容不进行比较
然后还要 git rm --cached /Music/res/* -r                               -r指的是要递归删除掉

git add .gitignore
git commit -m "gitignore"  这样后，以后res文件下的内容发生变化就不会再进行比较



