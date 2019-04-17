# Latex: 调整文档的行距？ - 家家的专栏 - CSDN博客





2011年12月27日 15:19:47[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：13914







|若在文档中使用两倍行距，要在导言区中加入\linespread{1.6}或等价地\renewcommand{\baselinestretch}{1.6}这时，除了使得正文中行距为缺省值的两倍外，脚注和浮动对象中标题的行距 也扩大为原来的两倍。要想在正文中使用两倍行距，而在标题中使用单倍行距， 可由 *setspace* 宏包来完成这一任务。\usepackage{setspace} \linestretch{1.6}\linestrech 的值为 1 时为单倍行距， 1.2 时是一倍半行距， 而为 1.6 时是双倍行距。 无论 *setspace* 使用与否， \captionfont 命令都可以用来 调节标题文本的行距。例如：\renewcommand{\captionfont}{\linespread{1.6}\normalsize}使得无论正文中的行距是多少，标题标题文本为双倍行距。|
|----|




转自：http://blog.sina.com.cn/s/blog_5e16f1770100gcpg.html



