# 零基础学习GitHub桌面版-3 团队协作流程 - 博客堂 - CSDN博客





2017年09月03日 14:25:31[最小森林](https://me.csdn.net/u012052268)阅读数：1073
所属专栏：[零基础学习GitHub桌面版](https://blog.csdn.net/column/details/17109.html)









![](http://ww1.sinaimg.cn/large/006qDjsOgy1g0tdplblplj30ne07q7cc.jpg)




### 文章目录
- [团队协作流程](#_4)
- [1为团队成员写入权限](#1_21)
- [2创建分支并修改版本](#2_37)
- [3打开一个拉取请求Pull Request](#3Pull_Request_60)
- [4讨论和审核你的代码](#4_78)
- [5合并Merge](#5Merge_84)
- [5.1 可能遇到的问题](#51__93)




# 团队协作流程

GitHub Flow是一个轻量级的，基于分支的工作流程，支持团队和部署在那里的定期做项目。

这个应用适用于这样的情境中：同公司的几个同事共同开发一款应用，大家拥有相同的权限，相同的目标，相同的责任。这种情况下，可以开启团队合作模式。

团队成员可以各自修改改进代码，然后发起讨论，讨论通过后，合并成一个总分支，流程如下图：

![image](http://www.ruanyifeng.com/blogimg/asset/2015/bg2015122302.png)

团队合作共有以下步骤
- 赋予权限，fork项目
- 创建分支并在自己的分支上修改版本
- 打开一个拉取请求Pull Request。晒出自己的代码。
- 团队成员讨论和审核你的代码
- 部署Deploy
- 合并Merge

## 1为团队成员写入权限

在我们的队友添加一个写的权限，这样我们的队友才能很好的修改代码。

我们打开网页上的GitHub,点击settings,

![image](https://upload-images.jianshu.io/upload_images/1159344-b391edfb2e257975.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/283/format/webp)

之后我们找到 “Organizations” ，点击右上角 “New organization”，这里会让我们验证密码，之后就有添加合作者的选项。这样我们就能添加我们的小伙伴了！

![image](https://upload-images.jianshu.io/upload_images/1159344-0d863da02802360c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)

这样我们就添加了新的小伙伴，新的小伙伴有着同样的权限去修改和管理代码。

此时我们就会看到我的小伙伴的github主页上就会出现关于我创建的First的各种通知。
![image](https://upload-images.jianshu.io/upload_images/1159344-a06ac6fd36d0d989.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
## 2创建分支并修改版本

![image](https://upload-images.jianshu.io/upload_images/1159344-b68cac6fd42d971f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)

在我们创建一个叫add new function的分支。

> 
Create a branch


fork 到个人仓库:
![image](https://upload-images.jianshu.io/upload_images/1159344-e85dbdfa65a6ce84.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
clone 项目到本地:

点击菜单栏 “File” -> “Clone Repo”， 选择本地路径、输入自己的远程仓库 URL （如：[https://github.com/thousfeet/Test），clone](https://github.com/thousfeet/Test%EF%BC%89%EF%BC%8Cclone) 完成后点击 “Open Now” 。此时左侧的 “REMOTE” 下方的 “origin” 会显示你的远程仓库信息。

![image](https://upload-images.jianshu.io/upload_images/1159344-1e576a3415989bbf.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)

修改新的版本,

填写好新的Summary和Description，提交新的版本并同步。

这样其他小伙伴登陆到GitHub上就看到了就可以清楚的看到一切的修改。

## 3打开一个拉取请求Pull Request

这个是整个流程中最关键的一步，发布Pull Request。意思是告诉小伙伴我做了一些修改，大家看看怎么样？可以的话就作为主分支了哦~

点击客户端或者网页上的Pull Request发布。

我们这里点击Pull Request

![image](https://upload-images.jianshu.io/upload_images/1159344-2dceb44ec74cf361.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)

我们填写好必要的说明性文字，界面如下：

![image](https://user-gold-cdn.xitu.io/2018/7/10/16482da223eef961?imageView2/0/w/1280/h/960/format/webp/ignore-error/1)

点击Send Pull Request

这样分其他小伙伴就看到了你的请求，他们会审核你写的代码，也许会提出修改意见，也许会直接同意，并同意发布

。

## 4讨论和审核你的代码

你的小伙伴收到系统提醒并开始对你的代码讨论，您还可以继续推送到你的分支在你提交的讨论和反馈光。如果有人评论说，你忘了做某件事，或者如果在代码中的错误，你可以在你的分支修复它，进行版本的更新。直到达成一个大家都满意的状态。

## 5合并Merge

一旦你拉的请求进行了审查，并且大家通过你的测试，您可以部署您的更改。如果你的分支造成的问题，您可以通过部署现有的主投产回滚。

现在，您的更改在生产中得到了验证，现在是时候你的代码合并到主分支。将其合并到Merged主分支中，然后删除这个分支。

merge的具体流程参照上篇日志：[零基础学习GitHub桌面版-2分支的使用](http://blog.csdn.net/u012052268/article/details/77778629/)

### 5.1 可能遇到的问题

> 
解决 merge 到本地时的冲突


当你在本地修改了代码文件时，队友可能修改了同一份代码，这时候从团队项目分支 merge 到本地时，就可能会产生冲突。

此时当在团队远程分支右键点击 merge 时，Gitkraken会检测出 conflict ，这时候只要选择 “View conflict file” 就可以打开冲突文件的 diff 视图，

通过勾选方框选择保留冲突部分的哪个版本，确定后就可以得到最下方的 Output 示意的最终合并后的文件。

![](https://upload-images.jianshu.io/upload_images/1159344-9411ccdb4f9b73de.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)





