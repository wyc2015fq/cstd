# Git学习笔记：中国版GItHub，码云 - 去向前方的博客 - CSDN博客





2018年07月07日 17:20:25[Big_quant](https://me.csdn.net/lvsehaiyang1993)阅读数：288








# 前言

在补习python的时候主要参考的是廖雪峰的教程[Python教程](https://www.liaoxuefeng.com/wiki/0014316089557264a6b348958f449949df42a6d3a2e542c000)，在学习完后准备完成期末作业时，遇到了一个技术难题，需要初步掌握git，因此开始了git的学习。 

本教程参考廖雪峰的[Git教程](https://www.liaoxuefeng.com/wiki/0013739516305929606dd18361248578c67b8067c8c017b000)

Git简介可以参看：[Git简介](https://blog.csdn.net/lvsehaiyang1993/article/details/80820483)

Git学习笔记：版本回滚 [Git:版本回滚](https://blog.csdn.net/lvsehaiyang1993/article/details/80821976)

Git学习笔记：修改[Git学习笔记：修改](https://blog.csdn.net/lvsehaiyang1993/article/details/80834906)

Git学习笔记： 远程仓库[Git学习笔记：远程仓库](https://blog.csdn.net/lvsehaiyang1993/article/details/80880796)

Git学习笔记：分支管理1 [Git学习笔记：分支管理1](https://blog.csdn.net/lvsehaiyang1993/article/details/80893917)

Git学习笔记：分支管理2[Git学习笔记：分支管理2](https://blog.csdn.net/lvsehaiyang1993/article/details/80894648)

Git学习笔记：分支管理3[Git学习笔记：分支管理3](https://blog.csdn.net/lvsehaiyang1993/article/details/80895890)

Git学习笔记：标签管理以及GItHub[Git学习笔记：标签管理以及GIthub](https://blog.csdn.net/lvsehaiyang1993/article/details/80917599)

Git学习笔记：自定义Git[Git学习笔记：自定义Git](https://blog.csdn.net/lvsehaiyang1993/article/details/80921696)
# 目录



- [前言](#前言)
- [目录](#目录)
- [码云](#码云)
- [GitHub导入](#github导入)




# 码云

使用GitHub时，国内的用户经常遇到的问题是访问速度太慢，有时候还会出现无法连接的情况（原因你懂的）。 

如果我们希望体验Git飞一般的速度，可以使用国内的Git托管服务——码云（gitee.com）。 

和GitHub相比，码云也提供免费的Git仓库。此外，还集成了代码质量检测、项目演示等功能。对于团队协作开发，码云还提供了项目管理、代码托管、文档管理的服务，5人以下小团队免费。 
**码云的免费版本也提供私有库功能，只是有5人的成员上限。**

使用码云和使用GitHub类似，我们在码云上注册账号并登录后，需要先上传自己的SSH公钥。选择右上角用户头像 -> 菜单“修改资料”，然后选择“SSH公钥”，填写一个便于识别的标题，然后把用户主目录下的.ssh/id_rsa.pub文件的内容粘贴进去：
![这里写图片描述](https://img-blog.csdn.net/20180707171359666?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

点击“确定”即可完成并看到刚才添加的Key：

![这里写图片描述](https://img-blog.csdn.net/20180707171512986?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

如果我们已经有了一个本地的git仓库（例如，一个名为learngit的本地库），如何把它关联到码云的远程库上呢？

首先，我们在码云上创建一个新的项目，选择右上角用户头像 -> 菜单“控制面板”，然后点击“创建项目”：

![这里写图片描述](https://img-blog.csdn.net/20180707171555527?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

项目名称最好与本地库保持一致：
然后，我们在本地库上使用命令git remote add把它和码云的远程库关联：
`git remote add origin git@gitee.com:challengef/learngit.git`
之后，就可以正常地用git push和git pull推送了！

如果在使用命令git remote add时报错：

```
git remote add origin git@gitee.com:challengef/learngit.git
fatal: remote origin already exists.
```

这说明本地库已经关联了一个名叫origin的远程库，此时，可以先用git remote -v查看远程库信息： 

```
git remote -v 

origin    git@github.com:challengef/learngit.git (fetch) 

origin    git@github.com:challengef/learngit.git (push) 

 

可以看到，本地库已经关联了origin的远程库，并且，该远程库指向GitHub。


我们可以删除已有的GitHub远程库：




`git remote rm origin`
再关联码云的远程库（注意路径中需要填写正确的用户名）：
`git remote add origin git@gitee.com:challengef/learngit.git`
此时，我们再查看远程库信息：

```
git remote -v
origin    git@gitee.com:challengef/learngit.git (fetch)
origin    git@gitee.com:challengef/learngit.git (push)
```

现在可以看到，origin已经被关联到码云的远程库了。通过git push命令就可以把本地库推送到Gitee上。

有的小伙伴又要问了，一个本地库能不能既关联GitHub，又关联码云呢？

答案是肯定的，因为git本身是分布式版本控制系统，可以同步到另外一个远程库，当然也可以同步到另外两个远程库。

使用多个远程库时，我们要注意，git给远程库起的默认名称是origin，如果有多个远程库，我们需要用不同的名称来标识不同的远程库。

仍然以learngit本地库为例，我们先删除已关联的名为origin的远程库：

```
git remote rm origin

然后，先关联GitHub的远程库：

git remote add github git@github.com:challengef/learngit.git

注意，远程库的名称叫github，不叫origin了。

接着，再关联码云的远程库：

git remote add gitee git@gitee.com:challengef/learngit.git
```

同样注意，远程库的名称叫gitee，不叫origin。

现在，我们用git remote -v查看远程库信息，可以看到两个远程库：

```
git remote -v
gitee    git@gitee.com:challengef/learngit.git (fetch)
gitee    git@gitee.com:challengef/learngit.git (push)
github    git@github.com:challengef/learngit.git (fetch)
github    git@github.com:challengef/learngit.git (push)
```

如果要推送到GitHub，使用命令：
`git push github master`
如果要推送到码云，使用命令：

git push gitee master

这样一来，我们的本地库就可以同时与多个远程库互相同步：

```
┌─────────┐ ┌─────────┐
│ GitHub  │ │  Gitee  │
└─────────┘ └─────────┘
     ▲           ▲
     └─────┬─────┘
           │
    ┌─────────────┐
    │ Local Repo  │
    └─────────────┘
```

码云也同样提供了Pull request功能，可以让其他小伙伴参与到开源项目中来。你可以通过Fork我的仓库：[https://gitee.com/liaoxuefeng/learngit](https://gitee.com/liaoxuefeng/learngit)，创建一个your-gitee-id.txt的文本文件， 

写点自己学习Git的心得，然后推送一个pull request给我，这个仓库会在码云和GitHub做双向同步。

# GitHub导入

可以直接把GitHub的项目导入码云 
![这里写图片描述](https://img-blog.csdn.net/20180707171949889?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2x2c2VoYWl5YW5nMTk5Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
```







