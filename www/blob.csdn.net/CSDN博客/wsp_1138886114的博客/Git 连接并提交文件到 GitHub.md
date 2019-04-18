# Git 连接并提交文件到 GitHub - wsp_1138886114的博客 - CSDN博客





2018年08月14日 18:08:34[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：109








### 一、准备工作

Git是分布式版本控制系统，同一个Git仓库，可以分布到不同的机器上。  

gitHub是一个面向开源及私有软件项目的托管平台，因为只支持git 作为唯一的版本库格式进行托管，故名gitHub。

###### 1.1 创建github账号

登录[https://github.com/](https://github.com/)，用邮箱注册账号，然后github会给你发送邮件，让你激活邮箱账号，然后就可以创建代码仓库了！ 

###### 1.2 安装 git

官网：[https://git-scm.com/downloads](https://git-scm.com/downloads)

###### 1.3 本地Git仓库和GitHub仓库之间进行传输
- 创建项目的SSH Key：
- 
一般在 C:\Users\Administrator 目录下打开gitBash执行： 
`ssh-keygen -t rsa -C "youremail@example.com`
- 
终端询问：是否选择当前目录在当前目录创建，是否添加密码，确认密码，三个问题（默认回车）

- 创建完成后，在用户主目录里找到.ssh目录，里面有id_rsa和id_rsa.pub两个文件。 

这两个就是SSH Key的秘钥对，id_rsa是私钥，不能泄露出去，id_rsa.pub是公钥，可以放心地告诉任何人.
- 登录github注册或登录账号：
打开“settings”的“SSH Keys”页面，然后，点“New SSH Key”，填上任意Title。 

在Key文本框里粘贴id_rsa.pub文件的内容，点“Add Key”，你就应该看到已经添加的Key- 验证是否成功：
在git bash里输入下面的命令: `$ ssh -T git@github.com`
###### 1.4 开始设置username和email，因为github每次commit都会记录他们

```
$ git config --global user.name     "name"       //你的GitHub登陆名
$ git config --global user.email    "123@126.com"//你的GitHub注册邮箱
```

###### 1.5 接下来就是把本地仓库传到github上去

```
git status                         # 查看修改文件
git add -A .                       # 添加要上传的:当前目录所有文件
git commit -m"对上传文件命名"       # 提交上传的文件 
git push                           # （推送）上传 


git add /目录/文件名                # 添加要上传的:指定文件
```



