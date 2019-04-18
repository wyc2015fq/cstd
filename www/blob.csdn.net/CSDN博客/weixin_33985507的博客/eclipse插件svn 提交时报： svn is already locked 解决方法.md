# eclipse插件svn 提交时报："svn is already locked"解决方法 - weixin_33985507的博客 - CSDN博客
2016年08月16日 21:08:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
在出错文件夹下，鼠标右键TortoiseSVN->Clean up.
SVN错误：Attempted to lock an already-locked dir
1、出现这个问题后使用“清理”功能，如果还不行，就直接到上一级目录，再执行“清理”，然后再“更新”。
2、有时候如果看到某个包里面的文件夹没有SVN的标志，直接用“Ctrl+Delete”手工删除，然后“清理”，最后“更新”或“提交”。
转载：http://blog.csdn.net/dracotianlong/article/details/8026885
