# jenkins github自动化平台搭建 - zhusongziye的博客 - CSDN博客





2017年09月24日 09:00:45[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：405标签：[jenkins																[github自动化平台搭建](https://so.csdn.net/so/search/s.do?q=github自动化平台搭建&t=blog)](https://so.csdn.net/so/search/s.do?q=jenkins&t=blog)
个人分类：[CI持续集成](https://blog.csdn.net/zhusongziye/article/category/7131168)








PS：项目里面要配置github的账户名和密码

请自己申请github账号和创建某个仓库，测试时，可以push一个可执行的python xx.py文件上去




先讲git

① github是一个世界性的代码仓库，开源，免费和收费版两种

② 命令介绍




1. 修改远程代码仓库：

git remote add origin url(这个url填写你的远程代码仓库的地址) 




2. 提交代码的步骤：

1）git add .

2）git commit -m "描述本次提交内容更新内容等等"

3）git push origin master 




3.拉取代码

git pull origin master




4. 切换分支

git checkout xxx(此处填写分支名)




环境：windows操作系统

目的：自动化的脚本存在每次更新都要push（推送到）github（代码仓库）中，我们使用jenkins来完成自动化项目的构建，为了减少每一次手动去将代码添加到jenkins的工作空间下，我们直接将github中的代码pull（拉取）下来，利于版本控制和管理




步骤如下：

1. 首先要有github账户，这个大家自己去注册申请就行

注意的地方：

ssh配置，网上有相关教程，很简单，公钥




2. jenkins环境搭建好，那是必须的（一键式安装）




3. 开始

3.1 新建项目

![](https://img-blog.csdn.net/20170924090116717?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




3.2 配置

GitHub -Porject：

![](https://img-blog.csdn.net/20170924090154542?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




源码管理：

![](https://img-blog.csdn.net/20170924090218217?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




构建触发器：

检测到源码的push操作触发构建

目的是当github代码有更新的时候，每次先clone代码到jenkins本地的工作空间（workspace)

![](https://img-blog.csdn.net/20170924090304712?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




构建：

命令是为了进入py文件中，等同于在cmd下运行python xx.py命令，去执行某个文件

![](https://img-blog.csdn.net/20170924090328046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




4. 这些全部都完毕之后，就可以进行构建了




最后：

有个坑：大家在写代码的时候，如果导入了包，jenkins是不会自动帮你识别的，记得在那个runcase文件下面，用sys模块，引入项目的根目录，将其添加到系统变量中，程序每次运行的时候，就不会报错

![](https://img-blog.csdn.net/20170924090350363?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





