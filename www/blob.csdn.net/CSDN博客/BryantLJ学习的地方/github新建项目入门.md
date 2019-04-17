# github新建项目入门 - BryantLJ学习的地方 - CSDN博客





2016年11月30日 19:49:13[遍地流金](https://me.csdn.net/u012177034)阅读数：458








# 新建一个github账号

不赘述

# 采用ssh远程登陆github

由于采用ssh登陆时需要验证，因此需要现在本机上生成密钥验证码，然后在github的账户中添加该密码即可

## 1.终端上创建ssh的密钥：

打开终端检测是否存在ssh：命令cd ~/.ssh 

如果存在，先将已有的ssh备份，或者将新建的ssh生成到另外的目录下 

如果不存在，通过默认的参数直接生成ssh`>>>ssh-keygen -t rsa -C xxxxx@gmail.com#（注册github时的email）`
此时会在~/.ssh/下生成id_rsa.pub文件

## 2.在github上创建ssh-key

登陆github，选择Account Settings–>SSH  Keys 添加ssh 

Title：xxxxx@gmail.com（可随便写） 

Key：打开你生成的id_rsa.pub文件，将其中内容拷贝至此。
## 3.终端ssh登陆测试

采用如下命令即可
`>>>ssh -T git@github.com `
登陆成功后，此时会弹出如下的欢迎信息
`Hi BryantLJ! You've successfully authenticated, but GitHub does not provide shell access.`
# 将本地代码托管到github仓库中

假设本地的代码目录为codedir 

进入当前代码目录

```
>>>cd codedir
>>>git init #在本地创建git仓库，此时会在当前目录生成一个隐藏的.git目录，该目录下记载着本地代码的一些信息
>>>git add .#将当前目录的所有文件加入到仓库缓存区
>>>git commit -a -m 'first commit'#将缓存区中的所有代码信息提交到本地仓库中，-a表示所有缓存区的意思，-m参数表示提交时的说明信息，便于后来查阅修改信息
>>>git remote add origin git@github.com:BryantLJ/py-faster-rcnn-pedestrian.git#为远程github中的某个代码库起一个别名'origin'，并建立远程联系
>>>git push -u origin master#将本地仓库中的所有内容push到github上的远程仓库中
```

第四步执行时可能会出现以下错误

![这里写图片描述](https://img-blog.csdn.net/20161130194700753)

最简单的修改方法是价一个强制命令
`git push -u origin master -f`
但这种方法不是一个好习惯，尤其是当多人合作开发一个项目时会造成历史混乱。推荐的解决方案参见[http://stackoverflow.com/questions/28429819/rejected-master-master-fetch-first](http://stackoverflow.com/questions/28429819/rejected-master-master-fetch-first)





