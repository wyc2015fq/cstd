# vim&git 高级 - V__KING__的专栏 - CSDN博客





2016年12月15日 08:55:36[v__king__](https://me.csdn.net/V__KING__)阅读数：276标签：[git-rebase](https://so.csdn.net/so/search/s.do?q=git-rebase&t=blog)
个人分类：[linux](https://blog.csdn.net/V__KING__/article/category/1733387)









# 一. vim 高级使用

## :NERDtree
- 快捷键： 

t,T:  tab打开files tree中的某个文件 

m:   NERDTree的菜单，可以添加，删除，move操作 

o ： open某个文件
## :tab 一个文件

:tabs     , 显示所有tab 的files 

:tabeditor   , 新建或者编辑一个tab file 

vi -O file1 file2  , 同时边界file1和file2
## 折叠:set foldmethod=syntax

za, zA 

zm, zr

# 二. git高级使用

## 合并几个commit
- git 默认使用的是nano编辑器，但是很不好用，可以重新设置
`git config --global core.editor vim`- git rebase -i commit-id  

记住：中途可以使用 git rebase –abort 中止的，或者使用git rebase –continue继续merge合并。





