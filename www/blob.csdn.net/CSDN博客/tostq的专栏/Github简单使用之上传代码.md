# Github简单使用之上传代码 - tostq的专栏 - CSDN博客





2016年05月06日 22:25:56[tostq](https://me.csdn.net/tostq)阅读数：5819









       本人是初次使用Github的小白，原来从Github下载代码一般都是使用Download，所以第一次上传代码的时候就想当然地使用Upload，但是一直折腾都没有成功=_=||，后来才知道Upload早在几年前就已经停用了Orz，于是我在网上搜了些教程，总结了自己的方法。

首先需要：

（1）Github的一个账号

（2）安装Github客户端，我安装的是Github for Windows，安装完后除了一个GitHub界面版，还有一个Git Shell




**一、在GitHub客户端上登录账号**



**二、在GitHub上新建一个仓库repository（如果没有的话），这里的要上传的文件就放在这个仓库里了**

**![](https://img-blog.csdn.net/20160506222213253?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**

**三、通过客户端在本机上复制该仓库**

![](https://img-blog.csdn.net/20160506222234430?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


**四、将你上传的文件复制到当前本机存储仓库目录下**

**五、在当前仓库下打开Git Shell**

![](https://img-blog.csdn.net/20160506222315427?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


**六、在Git Shell下配置仓库地址和账号**



 git config --global user.name "your name"

 git config --global user.name "your name"$ git config --global user.email 

**七、仓库初始化（必要）**

git init

**八、添加文件（夹），这里的文件要保存当仓库目录下**

git add 文件名

**九、添加commit （可选）**

git commit -m "xxx"

**十、把文件上传**

git push

完成了！

如果当前仓库中有未完成修改的没有被Merge的文件，会报错
error: failed to push some refs to

输入 git pull
 自动merge就可以了





