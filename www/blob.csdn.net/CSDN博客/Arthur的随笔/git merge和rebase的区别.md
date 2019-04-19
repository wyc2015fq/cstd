# git merge和rebase的区别 - Arthur的随笔 - CSDN博客
2012年04月03日 09:36:21[largetalk](https://me.csdn.net/largetalk)阅读数：14376
git merge 会生成一个新得合并节点，而rebase不会
比如：
```
D---E test
     /
A---B---C---F master
```
使用merge合并：
```
D--------E
     /          \
A---B---C---F----G   test, master
```
而使用rebase则：
`A---B---D---E---C'---F'   test, master`
使用git pull时默认是merge， 加 --rebase参数使其使用rebase方式
` git pull --rebase `
一些git命令： [http://blog.csdn.net/youngshook/article/details/7199533](http://blog.csdn.net/youngshook/article/details/7199533)
创建远程分支： git push origin master:zz
删除远程分支： git push origin :zz
创建本地跟踪指定远程分支： git co -t origin/zz
github fork 之后，如原repo有更新， 用下面操作更新：
```
git remote add firstguy git://github.com/firstguy/repo.git
```
After that's all set up, you should indeed be able to
```
git pull firstguy master
git push origin
```
