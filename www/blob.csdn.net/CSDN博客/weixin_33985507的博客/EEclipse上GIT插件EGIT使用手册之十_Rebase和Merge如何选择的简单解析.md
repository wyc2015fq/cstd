# EEclipse上GIT插件EGIT使用手册之十_Rebase和Merge如何选择的简单解析 - weixin_33985507的博客 - CSDN博客
2013年05月10日 11:55:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
此小结为什么说是简单解析呢，因为rebase和merge的选择问题讨论比较激烈，笔者也没有一个定论，而且git也处于研究发展阶段，很多理论还没有完全的纯熟。
对于一个多人开发团队频繁提交更新的情况，如果使用merge会使得历史线图非常复杂，并且merge一次就会新增一个记录点，如果使用rebase就是完全的线性开发。
![EEclipse上GIT插件EGIT使用手册之十_Rebase和Merge如何选择的简单解析](http://static.open-open.com/lib/uploadImg/20120205/20120205175837_791.gif)
上图所示是Merge和Rebase的两个结果，显然你不想要merge的混乱结果吧，你能告诉我merge图中那条线是master分支吗？
所以给出如下建议，如果同一文件反复修改或提交次数比较多，预期会出现很多的conflict，那么可以使用merge合并，仅需要解决一次冲突即 可（不过，大范围主题式的修改，是不是应该事先就新开一个分支呢？）；如果修改范围小，预期conflict少，则建议使用rebase。
EGIT中默认的pull操作是Fetch+Merge，如果要用rebase，可以分开操作。先执行Fetch更新remote tracking，再执行rebase进行合并（下一小节将介绍rebase操作）。或者修改pull的默认操作，在.git/config文件中配置：
![EEclipse上GIT插件EGIT使用手册之十_Rebase和Merge如何选择的简单解析](http://static.open-open.com/lib/uploadImg/20120205/20120205175837_754.gif)
上述配置只对mirror分支有效，也可做全局配置，在$HOME/.gitconfig中配置，windows系统如果没有配置HOME变量的话就默认在$documents and settings/ USER目录下：
![EEclipse上GIT插件EGIT使用手册之十_Rebase和Merge如何选择的简单解析](http://static.open-open.com/lib/uploadImg/20120205/20120205175838_870.gif)
