# Winsows下SVN版本控制器的安装与使用 - xiahouzuoxin - CSDN博客





2013年03月17日 16:24:01[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：6269








**Winsows下SVN版本控制器的安装与使用**

**1.   下载（这里都是windows版本）**

（1）SVN服务器安装包：[http://sourceforge.net/projects/win32svn/?source=dlp](http://sourceforge.net/projects/win32svn/?source=dlp)

（2）TortoiseSVN安装包：[http://sourceforge.net/projects/tortoisesvn/?source=dlp](http://sourceforge.net/projects/tortoisesvn/?source=dlp)

TortoiseSVN包的作用是为了在windows任意目录下都能识别SVN服务器所在路径。

此处的2个包名为：Setup-Subversion-1.7.8.msi、TortoiseSVN-1.7.11.23600-win32-svn-1.7.8

**2.   安装**

运行Setup-Subversion-1.7.8.msi和TortoiseSVN-1.7.11.23600-win32-svn-1.7.8，按正常安装软件的默认方式安装即可。

**3.   建立版本库（Repository）**

（1）新建一个目录。比如准备为“MP3编码器”项目建立版本库，建立目录“F:/SVN/mp3prj” ；



（2）进入mp3prj目录，"右键->TortoiseSVN->CreateRepository here...“，则在mp3prj下产生如下文件



![](https://img-my.csdn.net/uploads/201303/17/1363508682_1513.png)



**4.   配置用户和权限**

（1）打开mp3prj下的conf文件夹，下面有3个文件

![](https://img-my.csdn.net/uploads/201303/17/1363508690_3339.png)




（2）修改svnserve.conf

将“# password-db =passwd”去掉’#’号注释，变成成“password-db = passwd”；

将“#[general]”前如果有注释也去掉

（3）修改passwd

原文为：

[users]

# harry =harryssecret

# sally =sallyssecret

改为（当然，密码可以改成自己需要的）：

[users]

# 2个用户harry,sally的用户密码为harryssecret和sallyssecret

harry = harryssecret

sally = sallyssecret

svnadmin = admin

（4）修改authz

原文为：

[aliases]

# joe =/C=XZ/ST=Dessert/L=Snake City/O=Snake Oil, Ltd./OU=Research Institute/CN=JoeAverage



[groups]

# harry_and_sally =harry,sally

# harry_sally_and_joe= harry,sally,&joe



# [/foo/bar]

# harry = rw

# &joe = r

# * =



#[repository:/baz/fuz]

# @harry_and_sally =rw

# * = r

整个修改为

[groups]

harry_and_sally=harry,sally



# 设定组权限

# 根目录

[/]

svnadmin = rw

# truck目录

[/truck]

harry = rw

sally = r

* = r

# sanguo目录

[/sanguo]

harry = r

sally = rw

* = r

# ts目录

[/ts]

@harry_and_sally = rw  # 以@为键值意味着对前面定义的组进行授权

* = r

注：从对authz的配置可以看出，svnadmin具有最大的对/目录的读写权限，harry只对/truck目录有读写权限。（这很重要，因为我们这里的版本信息将打算保存到/truck目录，因此只能使用harry用户）

**5.   运行SVN服务器**

打开cmd，运行“svnserve -d -r F:/SVN/mp3prj/”，这里将路径改成新建版本库所在的路径。

运行之后

![](https://img-my.csdn.net/uploads/201303/17/1363508699_8871.png)




打开后在需要使用SVN时千万不要关闭cmd窗口，每次需要备份时都要使用上面的命令。



注：以上步骤按理是在版本控制服务器上完成的，只不过我们这里将自己的电脑用作主机了。下面的过程都要保证SVN服务器已经打开才能操作。



**6.   将开发的第一个版本放进版本库中**

（1）切换到自己的需要备份的工程目录，“右键->TortoiseSVN->Import... ”，



![](https://img-my.csdn.net/uploads/201303/17/1363508710_6300.png)


上图中的路径可替换为svn://localhost/truck，对于工作杂服务器上SNV，则改为svn://[服务器ip地址]/truck。

（2）完成后点OK，首次使用会出现填写用户名和密码的界面，

用户名：harry

密码：harryssecrect

确定



![](https://img-my.csdn.net/uploads/201303/17/1363508716_2091.png)


若出现类似如上的界面说明备份成功。



**7.   从版本库中copy出最新的版本**

（1）确定需要将从SVN上下载的版本保存目录并进入，这也可以在第（2）步中配置

（2）“右键->Checkout”



![](https://img-my.csdn.net/uploads/201303/17/1363508722_9751.png)


以上填写完后点OK则导出文件在对应的目录下。



**8.   将更改后的版本放入版本库中**

在更改的目录下，“右键/SVN Commit…”，添加描述信息，这时成新版本(Revision2)。

注：在更改的目录下，“右键/SVN Update…”，这时会更新旧版本，但不会生成新版本，小心使用。



**9.   对比2个版本的不同**

“右键->TortoiseSVN->Showlog”，弹出如下的框图，

![](https://img-my.csdn.net/uploads/201303/17/1363508729_6993.png)




在Revision1上“右键->Compare with working copy”，可以查看每个文件的修改部分。

![](https://img-my.csdn.net/uploads/201303/17/1363508735_4591.png)






