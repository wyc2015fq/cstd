
# Git的使用 -- 将本地项目上传到Github - 追求卓越,做到专业 - CSDN博客


2019年04月09日 00:01:29[Waldenz](https://me.csdn.net/enter89)阅读数：9标签：[本地项目上传到Github																](https://so.csdn.net/so/search/s.do?q=本地项目上传到Github&t=blog)[git本地仓库到Github																](https://so.csdn.net/so/search/s.do?q=git本地仓库到Github&t=blog)[git github																](https://so.csdn.net/so/search/s.do?q=git github&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=git本地仓库到Github&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=本地项目上传到Github&t=blog)个人分类：[Git																](https://blog.csdn.net/enter89/article/category/8837998)
[
																								](https://so.csdn.net/so/search/s.do?q=本地项目上传到Github&t=blog)


## 前期准备工作：
Git安装，Github账号的申请，cd到要上传的项目所在的目录
## git 初始化
在要上传的文件夹下，通过命令 git init 把这个文件夹变成Git可管理的仓库，执行命令后，会生成一个.git的隐藏文件夹，它是用来跟踪和管理版本库的。
`git init`
## git 状态查看
`git status`通过git status查看当前目录下的文件状态。
如下图：git status 显示，没任何需要提交的，但是存在没有被add的文件。
![](https://img-blog.csdnimg.cn/20190408225842487.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)
## git 添加文件到仓库
通过 git add把项目添加到仓库。
或者用 git add .   把目录下的所有文件添加到仓库。
执行 git add tf.ipynb后，再次查看状态
![](https://img-blog.csdnimg.cn/2019040823032741.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)
## git commit 把项目提交到仓库
![](https://img-blog.csdnimg.cn/20190408230951762.png)
-m 后面引号中是本次提交的注释内容。
## 创建SSH key，在Github上创建repository
详见另一篇文章：[https://blog.csdn.net/enter89/article/details/89096144](https://blog.csdn.net/enter89/article/details/89096144)
## Github上的仓库和本地库关联
`git remote add origin https://github.com/Waldenz/JupterSrc.git`origin 后为Github上创建好的仓库的地址。
## 本地库推送到Github库
`git push -u origin master`* 注意，当在Github上创建repository时，如果勾选了”Initialize this repository with a README“,即创建仓库的时候自动给你创建一个README.md的Markdown说明文件。执行上述命令会出现failed to push some refs to *******，是由于本地仓库中不存在README文件，这是需要先执行以下命令进行内容合并
`git pull --rebase origin master`这时再执行 git push -u origin master命令，就可以push成功了。
git push -u origin master：第一次推送master分支时，加上了-u参数，把本地的master分支内容推送的远程新的master分支，还会把本地的master分支和远程的master分支关联起来，在以后的推送或者拉取时就可以简化命令。
git push origin master ：本地master分支的最新修改推送至GitHub

# 总结：
1、本地创建文件夹，通过git init把它变成Git仓库；
2、将需要上传的文件复制到该文件夹中，通过git add把文件添加到仓库；
3、通过git commit -m "comment log"把项目提交到仓库；
4、在Github上设置SSH秘钥；创建repository仓库，通过git remote add originhttps://github.com/Waldenz/JupterSrc.git 将本地仓库和远程仓库进行关联；
5、通过git push -u origin master将本地仓库推送到Github上。

