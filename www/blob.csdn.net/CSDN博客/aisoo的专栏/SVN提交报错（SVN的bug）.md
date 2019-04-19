# SVN提交报错（SVN的bug） - aisoo的专栏 - CSDN博客
2012年11月20日 09:48:28[aisoo](https://me.csdn.net/aisoo)阅读数：1287
提交的时候报错：
    Failed to execute WebDAV PROPPATCH
svn: Commit failed (details follow):
svn: At least one property change failed; repository is unchanged
RA layer request failed
svn: Error setting property 'log': 
Could not execute PROPPATCH.
原来是我昨天提交的时候，comment太长了。今天再提交，改短了，就可以了。
这是SVN的一个bug，以后注释写短点
