# 如何在一台机器上配置多个git的rsa - z69183787的专栏 - CSDN博客
2016年09月21日 12:12:16[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1249
# 如何在一台机器上配置多个git的rsa
## 问题的提出
很多时候，我们一台机器上要使用多个git库，比如 github， csdn 以及 自己公司的。那么 rsa就要有多份。那么该如何让这些共同存在呢？ 
原理就是：建立多个不同的rsa 然后 在ssh config中分别不同的配置。
## 具体步骤
### 1 建立rsa
```
ssh-keygen -t rsa -C "你的邮箱地址"
```
执行完这条命令之后, 会弹出如下提示：
```
Enter file in which to save the key (/Users/aaaa/.ssh/id_rsa):
```
在这里这里就是 “建立多个不同rsa“文件的地方，输入不同的名字，就会产生不同的rsa.例如输入: csdn_rsa。就产生了csdn_rsa.
重复上面的命令，再建立git hub的即可。
### 2 修改ssh config
现在，csdn和git hub都有自己的rsa了。那么，如何引导选择不同的rsa验证呢。这时候需要修改 ssh config文件。 
如果你的 ~/.ssh 目录下没有 请建立一个这样的文件， 文件名: config。 
下面的事情是修改config文件。按照如下方式修改.
```
Host github.com
User 你的名字
IdentityFile ~/.ssh/id_rsa
Host code.csdn.net
User 你的名字
IdentityFile ~/.ssh/csdn_rsa
```
这样就可以愉快的使用多个rsa了。
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
    7.测试设置是否正确：
  至此，设置已经ok，具体后面使用中什么问题，待我探索后再来编写。
