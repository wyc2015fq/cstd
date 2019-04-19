# 如何在一台机器上配置多个Git的rsa - 零度的博客专栏 - CSDN博客
2017年01月11日 09:26:53[零度anngle](https://me.csdn.net/zmx729618)阅读数：889
        说说关于一台机器如何支持多个Git帐号，我的情况是这样。通常，我开发的项目都是托管在Oschina上，因为感觉Github有时候网络不是很好，所以方便起见，就申请了一个Oschina的帐号，平时也可以去上面看看优秀的项目。但是，自己还有一个Github的帐号。这个时候在本地使用git时就非常尴尬了，很久之前配置过同时支持两个Git帐号的，最新实验室给配置了台新电脑，就得重新折腾一下，现在做个记录。当前电脑环境为Win7具体ubuntu上应该和这个类似。
    1.如果本机上已经有添加过自己的git帐号，找到git生成的密钥的目录,如果没有调到第三步：
           C:\Users\WJ\.ssh
     2.修改当前目录下的密钥文件的名字，
            id_rsa_oschina 
            id_rsa_oschina.pub
     3. 为自己的github帐号创建相应的密钥
           ssh-keygen -t rsa -C "my_github_emial"
           Enter file in which to save the key (/c/Users/WJ/.ssh/id_rsa): id_rsa_github  (这里输入我们定义的名字以便和oschina的有区别, 然后一路回车)
     4. 生成完成后，会在本地的ssh目录下出现刚刚生成的密钥文件:
![](https://img-blog.csdn.net/20150705154217750?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
     可以看到名字是有相应的区别。
    5. 编写配置文件，支持两个帐号自动登录
        在改目录下创建config文件，并编写以下内容：
![](https://img-blog.csdn.net/20150705154552695?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       可以参考我的内容，填写自己的相应的个人信息。
    6.删除本地全局设置
      如果之前使用过程中使用过git config --global user.name "" 或者 git config --global user.email 命令，git会在C盘目下产生一个.gitconfig文件，这个文件中保存了全局的git帐号信息，应该删除掉。
     至此，设置已经ok。
