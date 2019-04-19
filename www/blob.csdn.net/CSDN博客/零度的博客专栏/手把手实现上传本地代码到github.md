# 手把手实现上传本地代码到github - 零度的博客专栏 - CSDN博客
2017年07月18日 14:33:32[零度anngle](https://me.csdn.net/zmx729618)阅读数：289标签：[git																[github](https://so.csdn.net/so/search/s.do?q=github&t=blog)](https://so.csdn.net/so/search/s.do?q=git&t=blog)
个人分类：[Git](https://blog.csdn.net/zmx729618/article/category/6358095)
**最近有人在群里问怎么将新创建的本地代码上传到github上，这里简单的记录一下，我喜欢使用命令行，这里全用命令行来实现，不了解git命令的可以去了解下。**
**第一步：建立git仓库**
打开git shell 客户端，cd到你的本地项目根目录下，执行git命令：
`git init`
第二步：将项目的所有文件添加到仓库中
`git add . `
**如果想添加某个特定的文件，只需把.换成特定的文件名即可**
**第三步：将add的文件commit到仓库**
`git commit -m "注释语句"`
第四步：去github上创建自己的Repository，创建页面如下图所示：
![这里写图片描述](https://img-blog.csdn.net/20150703103054152)
**点击下面的Create repository，就会进入到类似下面的一个页面，拿到创建的仓库的https地址，红框标示的就是**
![这里写图片描述](https://img-blog.csdn.net/20150703103443338)
**第五步：重点来了，将本地的仓库关联到github上**
`git remote add origin https://github.com/hanhailong/CustomRatingBar`
**后面的https链接地址换成你自己的仓库url地址，也就是上面红框中标出来的地址**
**第六步：上传github之前，要先pull一下，执行如下命令：**
`git pull origin master`
**敲回车后，会执行输出类似如下** ：
![这里写图片描述](https://img-blog.csdn.net/20150703104116678)
**第七步，也就是最后一步，上传代码到github远程仓库**
`git push -u origin master`
**执行完后，如果没有异常，等待执行完就上传成功了，中间可能会让你输入Username和Password，你只要输入github的账号和密码就行了**
**最后附上代码上传成功后的截图：**
![这里写图片描述](https://img-blog.csdn.net/20150703104457421)
