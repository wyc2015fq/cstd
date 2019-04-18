# Omega Overview - Orisun - 博客园







# [Omega Overview](https://www.cnblogs.com/zhangchaoyang/articles/2041373.html)





Xapian 和 Omega 是独立的两个组件，旨在相互协作来提供索引和搜索功能。Xapian 组件提供了核心的数据库功能（存储信息）以及搜索和检索系统来查找单词和单词组合。

Omega 组件提供了一些工具来将不同格式的信息转化并解析成 Xapian 需要的原始格式以便它可以被索引。Omega 使用了多种工具，比如 `pdftotext`，然后再提交转化并过滤了的基于文本的信息，以便文档的索引和结构可以被标识并存储在 Xapian 数据库。Omega 是 Xapian 下载的一部分。

先来把Omega小用一把，让你对它有个直观的感受，然后我们再来详细介绍Omega。

建立索引数据库：

$ omindex --db db1 --url may9 /home/orisun/master/chsegtxt

db1是你自己取的数据库名，may9是你自己定义的索引信息的URL，

/home/orisun/master/chsegtxt下是你要建立索引的文件。

执行后会在当前路径下生成一个文件夹db1，那may9在哪儿呢？马上就看到它了。

查询数据库：

quest --db=db1 经济

从数据库db1中检索含有“经济“的文档。返回的内容开如：

Parsed Query: Xapian::Query(Z经济:(pos=1))

MSet:

67 [100%]

url=may9/f00153.txt

sample=2011 年 05 月 06 日 周五 出版 * * * * * *  设置 我 的 评论 站 内 信 登出 登入 新手指南 免费注册 您 的 反馈 [ keys ] * # 文章 * o 日期  会员 信息 FT 中文网 iPad 应用程序 全面 升级 ！ 复旦 蓝墨水 活动 - 与 资深 HR 及 高管 会面 “ 世界 因 你 ...

type=text/plain

modtime=1304820622

size=29984

如果想要对一个 web 站点进行爬行并索引，可以使用 `htdig2omega`，它能接受一个 URL 并搜索整个 web 站点。

构建了初始的数据库后，就能向该数据库添加更多的文档和目录了，不过，为了这个目的，应该使用不同 URL 目录，以便能更为显式地在索引内定位文档。您应该使用 `-p` 选项来确保现有的文档没有在添加过程中被删除：
$ omindex -p --db info --url documents /mnt/data0/Documents














