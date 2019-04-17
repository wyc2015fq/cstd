# TortoiseGit创建本地库并提交到远程服务器 - DoubleLi - 博客园






前半部分参考网上的例子：http://www.showerlee.com/archives/1300，但会出现“[Git](http://lib.csdn.net/base/git) did not exit cleanly (exit code 128)”错误



1.在D盘新建一个目录,例如"D:\[git](http://lib.csdn.net/base/git)",并进入目录右键目录空白处选择"Git Create repository here...",弹出对话框点确认,这样即建立了一个本地Git仓库.

![11](http://www.showerlee.com/wp-content/uploads/2014/08/111.jpg)![12](http://www.showerlee.com/wp-content/uploads/2014/08/12.jpg)



2.在该仓库目录下创建一个[测试](http://lib.csdn.net/base/softwaretest)项目文本文件,右键目录空白处,选择Git Commit -> "master" ...,在弹出对话框里输入提示注释,选择要加入的[版本控制](http://lib.csdn.net/base/git)文件,确定即可提交.

![18](http://www.showerlee.com/wp-content/uploads/2014/08/18.jpg)
![13](http://www.showerlee.com/wp-content/uploads/2014/08/13.jpg)



3.现在只是在本地实现使用Git管理项目,在此界面若显示Success则本地提交成功,接下来点击Push..,把我们的改动递交到Git服务器上.
![14](http://www.showerlee.com/wp-content/uploads/2014/08/14.jpg)

4.此时会弹出Push对话框,在Ref - Remote栏里设定当前分支名为master,然后点击Destination - Remote栏的Manage按钮.
![15](http://www.showerlee.com/wp-content/uploads/2014/08/15.jpg)

5.在弹出的设置对话框中按照如图所示,填写服务器Remote名称,URL(git@git.yanwenbo.cn:root/tesproject.git),之前保存的本地私钥testkey.ppk,点击Add New/Save按钮保存这一设置,然后点击确定退出返回之前的对话框.

![10](http://www.showerlee.com/wp-content/uploads/2014/08/101.jpg)



6.最终确认提交

![17](http://www.showerlee.com/wp-content/uploads/2014/08/17.jpg)



7.push成功



![20](http://www.showerlee.com/wp-content/uploads/2014/08/20.jpg)

8.这时会出现“git did not exit cleanly (exit code 128)”错误，解决方法是：在远程服务器上创建testproject.git，然后在push工程。即在gitolite-admin/conf/gitolite.conf文件中增加工程和权限并push到远程服务器。









