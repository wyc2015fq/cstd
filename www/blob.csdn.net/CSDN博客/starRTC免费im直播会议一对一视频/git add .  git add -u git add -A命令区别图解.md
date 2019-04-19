# git add .   git add -u git add -A命令区别图解 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年07月15日 09:18:14[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：295
git版本不同会有所区别：
**Git Version 1.x**: 
**Git Version 2.x**: 
git add . 
修改(modified)以及新文件(new)，但不包括被删除的文件。
git add -u
仅监控**已经被add的文件**（即tracked file），会将修改的文件提交到暂存区。
-u
不会提交新文件（untracked file）。（git add --update的缩写）
即： -u  提交被修改(modified)和被删除(deleted)文件，不包括新文件(new)
git add -A ：是上面两个功能的合集（git add --all的缩写）

详细请参见git help add
参考：
[http://www.cnblogs.com/skura23/p/5859243.html](http://www.cnblogs.com/skura23/p/5859243.html)
