# 删除一个目录和其各级子目录下的.svn文件 - DoubleLi - 博客园






两种方法
[1]用find命令和其action来实现
[2]用rm直接实现
$ cd /tmp/xxx
$ rm -rf  `find . -name .svn`
就可以实现了。

删除SVN目录及从服务器端删除SVN版本库文件方法详解


在学习SVN的过程中，你可能会遇到这样的问题,如何删除SVN目录？如何从服务器端彻底删除SVN版本库中部分文件夹或文件夹？本文为你一一解答。


删除SVN目录
当使用了svn版本控制系统后每个目录下都会有一个.svn目录存在，开发完当交付产品或者上传到服务器时一般要把这些目录删除，这里总结了一下在linux和win下的办法。

一、在linux下删除SVN

删除这些目录是很简单的，命令如下
find.-typed-name".svn"|xargsrm-rf
或者
find.-typed-iname".svn"-execrm-rf{}\;
全部搞定。(http://tech.techweb.com.cn/redirect.php?fid=26&tid=205673&goto=nextnewset)

二、在windows下删除SVN用以下法子：

1、在项目平级的目录，执行dos命令：
xcopyproject_dirproject_dir_1/s/i

2、或者在项目根目录执行以下dos命令
for/r.%%ain(.)do@ifexist"%%a\.svn"rd/s/q"%%a\.svn"
其实第二种方法可以用来干很多事的，比如把代码中的.svn替换为任意其他文件名并在硬盘根目录下执行，就可以从硬盘上删除所有的这个文件啦。

3、加注册表
JonGalloway提供了一段注册表代码，可以将”DeleteSVNFolders”命名增加到资源管理器的右键上，这样，鼠标点两下就能把选中目录下的所有.svn目录干掉了。Worksjustgreat!
代码为：
WindowsRegistryEditorVersion5.00[HKEY_LOCAL_MACHINE\SOFTWARE\Classes\Folder\shell\DeleteSVN]@=”DeleteSVNFolders“[HKEY_LOCAL_MACHINE\SOFTWARE\Classes\Folder\shell\DeleteSVN\command]@=”cmd.exe/c\”TITLERemovingSVNFoldersin%1&&COLOR9A&&FOR/r\”%1\”%%fIN(.svn)DORD/s/q\”%%f\”\”“将这段代码保存为一个.reg文件，双击确认导入注册表即可。


从服务器端彻底删除SVN版本库中部分文件夹或文件夹
若要彻底删除SVN版本库某一文件夹或文件,可采取这种方法(举例说明):

例:假设SVN库路径为E:\svn\project，库中的目录结构为
QA/Trunk
Software/Tags/test.exe
删除Software/Tags/目录下的test.exe文件

操作步骤为:
把SVN库dump出来
使用svndumpfilter过滤掉要删除的文件
新建一个SVN库
再将处理好的文件load到新的SVN库里
具体命令为：
>svnadmindumpE:\svn\project>aaa.dump
>typeaaa.dump|svndumpfilterexclude/Software/Tags/test.exe>bbb.dump
>svnadmincreateE:\svn\project_new
>svnadminloadE:\svn\project_new<bbb.dump
然后再将新建的project_new重命名为project,以前的project可以移走,或是另取一个名称.(因为在TRAC中使用的SVN目录是project，用户所使用的SVN目录也是project)

此方法在SVN库里版本不多的情况下完全可以达到彻底删除SVN版本库某一文件夹或文件的效果，但是如果ＳＶＮ库里的版本过多，在dump版本的时候会因存储空间不足，而无法dump版本．也就无法操作了．这时须另挂能满足其存储空间的硬盘操作.

【编辑推荐】
- [技术分享：如何删除SVN备份库](http://developer.51cto.com/art/201006/203233.htm)
- [如何删除SVN及其恢复问题名师指点](http://developer.51cto.com/art/201005/201920.htm)
- [删除SVN版本信息行之有效的办法](http://developer.51cto.com/art/201005/201924.htm)
- [SVN基本操作之新手上路](http://developer.51cto.com/art/201005/202610.htm)
- [Lniux和Windows下如何删除SVN版本信息？](http://developer.51cto.com/art/201006/203226.htm)
- 









