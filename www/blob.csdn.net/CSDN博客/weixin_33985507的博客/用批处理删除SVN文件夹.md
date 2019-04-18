# 用批处理删除SVN文件夹 - weixin_33985507的博客 - CSDN博客
2011年09月06日 16:22:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
常常因为某种原因需要删除某个项目的svn文件夹，使项目脱离源代码管理的控制。如果一个一个手动删除.SVN文件夹的话，显然很麻烦。于是到网上搜索了一下，找到一个不错的方法。原作者不知道是谁了，还是要感谢他。
将下面的这段文字写在.reg结尾的文本文件中，然后执行。这样你在每个文件夹右击的时候就会出现Delete Svn Folders这个选项了。很是方便！
```
Windows Registry Editor Version 5.00
[HKEY_LOCAL_MACHINE\SOFTWARE\Classes\Folder\shell\DeleteSVN] 
@="Delete SVN Folders"
[HKEY_LOCAL_MACHINE\SOFTWARE\Classes\Folder\shell\DeleteSVN\command] 
@="cmd.exe /c \"TITLE Removing SVN Folders in %1 && COLOR 9A && FOR /r \"%1\" %%f IN (.svn) DO RD /s /q \"%%f\" \""
```
