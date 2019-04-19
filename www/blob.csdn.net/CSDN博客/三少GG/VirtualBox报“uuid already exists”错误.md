# VirtualBox报“uuid already exists”错误 - 三少GG - CSDN博客
2013年04月09日 18:23:35[三少GG](https://me.csdn.net/scut1135)阅读数：1608

### [解决导入.vdi文件时报“uuid already exists”错误](http://blog.csdn.net/zhang854429783/article/details/6672199)
分类： [VirtualBox](http://blog.csdn.net/zhang854429783/article/category/859383)
virtualbox导入vdi文件时出现下面的问题：
打开hard disk D:\software\GT5.0.0.vdi 失败
Cannot register the hard disk 'D:\software\GT5.0.0.vdi' withUUID {jfldk-fdjklf-sljk-fdjl-fdjsfld} because a hard disk'D:\software\GT4.3.vdi' with UUID {jfldk-fdjklf-sljk-fdjl-fdjsfld}already exists in the media registry('C:\Users\user\.VirtualBox\VirtualBox.xml')
解决方法：
用命令行进入VirtualBox的安装目录，使用下面的命令：
**在virtualbox4.0.4以上:**
**C:\Program Files\...\VirtualBox>VBoxManage internalcommands sethduuid D:\path\ubuntu.vdi(or .vmdk)**
**然后重新导入即可。**
这是一个重新设置UUID号的命令，VirtualBox没有集成到GUI环境中，只能在命令行中使用。
