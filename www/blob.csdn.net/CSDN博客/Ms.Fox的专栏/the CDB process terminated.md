# the CDB process terminated - Ms.Fox的专栏 - CSDN博客
2018年01月02日 17:00:19[lulu-lu](https://me.csdn.net/smbluesky)阅读数：3873标签：[qt																[CDB process terminat](https://so.csdn.net/so/search/s.do?q=CDB process terminat&t=blog)](https://so.csdn.net/so/search/s.do?q=qt&t=blog)
个人分类：[qt](https://blog.csdn.net/smbluesky/article/category/7377296)
在调试qt的时候，出现了the CDB process terminated 。
首先，要确定qt creator的配置是对的。
1、qt的环境变量的设置
2、在qt creator 中的工具 -> 选项 ->构建和运行，要确定这些选项配置对。
1）安装windbgd 32位，64位的安装
2）qt 版本的设置
3）Kit 页面的设置
3 这个时候再调试程序的时候，还是出现the CDB process terminated 。这个时候，需要去构建这个项目，打开这个项目生成exe文件的目录，执行。
我这边的问题是发现缺少ucrtbased.dll库文件。
可以从以下链接找到详解。
https://www.cnblogs.com/findumars/p/6110973.html
把库文件添加到对应的目录，就可以了
