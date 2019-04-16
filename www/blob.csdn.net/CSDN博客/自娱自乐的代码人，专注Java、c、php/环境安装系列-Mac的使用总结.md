# 环境安装系列 - Mac的使用总结 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2015年03月29日 09:14:35[initphp](https://me.csdn.net/initphp)阅读数：858








最近买了一个苹果pro，所以将一些常用的快捷键、开发环境安装等总结在这篇文章中。方便自己的查询

### Mac常用快捷键

Command+M: 最小化窗口

Command+T: 在浏览器中打开新窗口

Command+W: 关闭窗口

Command+Q: 退出程序


Command+C: 复制内容


Command+V: 黏贴内容





### Homebrew安装和使用

brew是类似于Centos下面的yum和Ubuntun下的apt一样的软件包安装工具。


homebrew官网：[brew.sh](http://brew.sh)




#### 常用命令

安装命令：

`ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"`安装软件

`brew  intsall  wget  //安装软件`卸载软件

`brew uninstall wget`搜索软件

`brew search jepg`更新brew

`brew update`列出已经安装的软件

`brew list`显示brew的信息

`brew info`显示依赖信息

`brew deps wget`



#### brew的安装软件目录：


/usr/local/bin  :  可执行文件会安装在这边


/usr/local/lib  :  c语言的动态库和静态库，编译的时候直接通过-l xx  可以依赖


/usr/local/etc:  配置相关文件


/usr/local/include: c语言头文件


/Library/Caches/Homebrew : 你所有安装的软件，都会先下载到这个缓存文件夹中。





#### Jdk 安装

jdk目录：/Library/Java/JavaVirtualMachines


#### Zend studio安装

下载地址：[http://downloads.zend.com/studio-eclipse/10.5.0/ZendStudio-10.5.0-macosx.cocoa.x86_64.dmg](http://downloads.zend.com/studio-eclipse/10.5.0/ZendStudio-10.5.0-macosx.cocoa.x86_64.dmg)

破解包下载地址：com.zend.php.core_10.5.0.v20131105-1526.jar  [http://download.csdn.net/detail/lllkey/6538201](http://download.csdn.net/detail/lllkey/6538201)

下载完成后不能修改文件名, 并且找到zendStudio目录下的plugins文件夹, 并且把上面下载的jar文件替换到里面。对于mac用户这个地方也需要找到plugins文件夹,并且替换以上jar文件.

 mac下找到plugins目录的方法: 

1. 打开dmg文件,把ZendStudio拖入Application文件夹内

2. 在Application文件夹内找到ZendStudio, 右键->显示包内容

3. Contents->Resources->Java->plugins


注册码：

15BD3D9635621E3AD4AE0F243B9E76CCFF6383B0E254EF646FD47B1424832332148D73E944AC95FC2A22EAF9965

FE00D0BBACAD80E4B73BCFD40AADBEBD606EA4C33A74AEBB140CFB189

E545E351774B7A020CDBE4FD159CDFF00F3C6A08C6AAC1A4F811EDDCF59CA

FD09D46C849CB780BF6EF5983D3AFC372FC2021714E6B0B




