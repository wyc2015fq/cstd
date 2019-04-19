# pythonWebApp_Day_01 廖雪峰实践_用户5706034127_新浪博客
||分类：[python](http://blog.sina.com.cn/s/articlelist_5706034127_2_1.html)|
**1，先创建项目文件夹，使用git**
在用git init初始化项目，这时会多出来.git文件夹保存用来管理项目的信息，然后添加个readme.txt文件。
第一步是用
```
git
add
```
把文件添加进去，实际上就是把文件修改添加到暂存区；
第二步是用
```
git
commit
```
提交更改，实际上就是把暂存区的所有内容提交到当前分支。
因为我们创建Git版本库时，Git自动为我们创建了唯一一个`master`分支，所以，现在，
```
git
commit
```
就是往`master`分支上提交更改。
**git status**查看项目的状态 ，**gitdiff**查看某个文件的不同，**git log查看**commit记录
![pythonWebApp_Day_01 <wbr>廖雪峰实践](http://s11.sinaimg.cn/mw690/006e9W8vzy79t0iywNc3a&690)![pythonWebApp_Day_01 <wbr>廖雪峰实践](http://s7.sinaimg.cn/mw690/006e9W8vzy79t1rhlgG26&690)
**Git是分布式版本控制系统。git关联到远程仓库需要ssh协议，现在本地生成ssh私钥和公钥，将公钥添加到账户的shh管理里面去，这样相当于认证。**
![pythonWebApp_Day_01 <wbr>廖雪峰实践](http://s5.sinaimg.cn/mw690/006e9W8vzy79t3aLFGs74&690)
然后就可以本地库关联远程仓库
要关联一个远程库，使用命令
```
git
remote add origin git@server-name:path/repo-name.git
```
；
关联后，使用命令
```
git
push -u origin master
```
第一次推送master分支的所有内容；
**此后，每次修改后先add到暂存区，在commit到本地库，在push到远程库。**
```
git push origin master
```
推送最新修改；
git add . 表示本目录都添加
![pythonWebApp_Day_01 <wbr>廖雪峰实践](http://s16.sinaimg.cn/mw690/006e9W8vzy79t4jnIRx5f&690)
