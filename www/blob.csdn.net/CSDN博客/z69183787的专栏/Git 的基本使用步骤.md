# Git 的基本使用步骤 - z69183787的专栏 - CSDN博客
2015年01月25日 14:25:19[OkidoGreen](https://me.csdn.net/z69183787)阅读数：851
我是在win7下使用Git，具体步骤总结如下：
（1） 下载Git（我的下载Git-1.8.3-preview20130601.exe），具体下载地址：http://code.google.com/p/msysgit/downloads/list（[msysgit](http://code.google.com/p/msysgit/)：[Git](http://baike.baidu.com/view/1531489.htm) 版本控制系统在
 Windows 下的版本）或者 http://git-scm.com/download（Git官方网站吧）。下载完成自行安装，很简单的.
（2）在GitHub创建账户，然后创建一个 Repository（仓库、储存室），创建过程有提示，这里就不再赘述。
（3）Git安装完成应该有Git Bash和Git Gui（可视化操作），我使用的是Git Bash；打开Git bash进行命令操作（Linux下的命令在Git Bash下大都能用）
（4）输入命令：git config --global user.name "usernme" （username：GitHub的用户名）
（5）git
 config --global user.email emailName （emailName：我写的是GitHub的注册邮箱）
（6）在本地创建一个Git的工作空间，在里面创建一个工程（如Hello），在工程 目录下随意创建一个文件（如readme.txt，在文件中写点内容，最好是英文）；git
 bash里面 进入工程目录（如我的”cd ./F:\GitOpenSource“到Git工作空间，需要一”. /“开始， 然后cd到工程目录下），输入命令：git init（初始化工程）
（7）使用git add filename 将文件添加到库（如"git add readme.txt"）
（8）使用git commit -m "first commit" 进行提交（"first commit" 相当于一个描述）
（9）使用ssh-keygen -C 'email' -t rsa （我的emali：EmailName@163.com）
（10）在pub文件中copy key，添加当GitHub中的SSH keys
（11）ssh -T git@github.com进行验证
（12）使用远程连接：git remote add origin git@github.com:username/Hello.git
 （username：GitHub上的用户名，Hello：我在GitHub上创建的仓库repository名）
（13）输入命令“git
 push origin master”将源码推送到GitHub
（14）在GitHub上仓库页面刷新即可看到你在本地创建并提交的文件
推荐各位使用GitHub的客户端工具，界面做得确实不错。下载地址：[http://github-windows.s3.amazonaws.com/setup.exe](http://github-windows.s3.amazonaws.com/setup.exe)
