
# [关系图谱] 一.Gephi通过共现矩阵构建知网作者关系图谱 - 杨秀璋的专栏 - CSDN博客

2018年08月16日 20:17:11[Eastmount](https://me.csdn.net/Eastmount)阅读数：1929所属专栏：[知识图谱、web数据挖掘及NLP](https://blog.csdn.net/column/details/eastmount-kgdmnlp.html)



作者最近研究人物关系图谱，准备发表相关的文章，原本是用Python Networkx库绘制，但效果不太理想；故改为Gephi软件，发现其非常好看，特分享几篇文章供大家交流学习，希望对您有所帮助，尤其是引文分析、社交网络、主题分布等方向的同学。后续的文章将尽可能的使用Markdown语法撰写了。
参考文章：[【python数据挖掘课程】十七.社交网络Networkx库分析人物关系（初识篇）](https://blog.csdn.net/eastmount/article/details/78452581)

PSS：2019年1~2月作者参加了CSDN2018年博客评选，希望您能投出宝贵的一票。我是59号，Eastmount，杨秀璋。投票地址：[https://bss.csdn.net/m/topic/blog_star2018/index](https://bss.csdn.net/m/topic/blog_star2018/index)
![](https://img-blog.csdnimg.cn/20190104155648543.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)[
](https://img-blog.csdnimg.cn/20190104155648543.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)五年来写了314篇博客，12个专栏，是真的热爱分享，热爱CSDN这个平台，也想帮助更多的人，专栏包括Python、数据挖掘、网络爬虫、图像处理、C\#、Android等。现在也当了两年老师，更是觉得有义务教好每一个学生，让贵州学子好好写点代码，学点技术，"师者，传到授业解惑也"，提前祝大家新年快乐。2019我们携手共进，为爱而生。
---

# 一.关系图谱结果展示
**1.Fruchterman Reingold布局**
运行结果如下图所示：
![](https://img-blog.csdn.net/20180816174101347?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[
](https://img-blog.csdn.net/20180816174101347?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**2.Force Atlas布局**
总体布局如下图所示：
![](https://img-blog.csdn.net/20180816174906764?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[
](https://img-blog.csdn.net/20180816174906764?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)然后查看图谱的细节如下所示：![](https://img-blog.csdn.net/20180816175441415?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[
](https://img-blog.csdn.net/20180816175441415?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![](https://img-blog.csdn.net/20180816175530250?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[
](https://img-blog.csdn.net/20180816175530250?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
---

# 二.导入共现矩阵
**1.Excel构建共现矩阵**
接下来导入Excel文件的共现矩阵，如下图test.xls文件所示，具体怎么得出这个表格数据，我后续文章将采用倒叙的方式进行讲解，包括抓取知网的论文数据。
![](https://img-blog.csdn.net/20180816175813116?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[
](https://img-blog.csdn.net/20180816175813116?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)部分数据如下所示，其中“赵凌 谢传红”表示两个作者合作完成一篇文章，同理“赵凌 石维武”也一样，这就是所谓的共现矩阵。有的学者针对文学作品或影视作品做关系图谱，则只需将出现在同一章节或同一集电视剧的演员看作存在关系，构建对应的共现矩阵即可。
```python
赵凌	谢传红
赵凌	石维武
张磊	屈国超
张磊	王韶霞
张磊	谢卓潇
屈国超	王韶霞
屈国超	谢卓潇
王韶霞	谢卓潇
高会军	屈植斌
胡翠华	彭馨
张婷	赵孝梅
张婷	何燕
张婷	邓婕
张婷	吴昌学
张婷	禹文峰
张婷	官志忠
```
文章“贵州都匀阳和乡格业寨水族“开控”仪式及仪式音声考述”的作者刚好是“赵凌 谢传红 石维武”三位老师，也对应上述的共现关系。![](https://img-blog.csdn.net/20180816180957777?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[
](https://img-blog.csdn.net/20180816180957777?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**2.Gephi导入数据**
接下来打开Gephi软件，点击“文件”=>“导入电子表格”，选择“test.xls”文件，如下图所示。
![](https://img-blog.csdn.net/201808161819055?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[
](https://img-blog.csdn.net/201808161819055?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)点击“下一步”=>“完成”，需要注意的是：图的类型选择“无向边”，因为论文作者是同时出现的，不存在方向；而如果研究论文引用文献，则建议构建有向图。![](https://img-blog.csdn.net/20180816182013281?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[
](https://img-blog.csdn.net/20180816182013281?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)该图共有497个节点，1085条边，点击确定之后如下图所示，可以看到一堆混乱的关系及散点。![](https://img-blog.csdn.net/20180816193630225?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[
](https://img-blog.csdn.net/20180816193630225?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
---

# 三.设置节点和边
接着点击右下角“统计”，然后点击：
**(1) 平均路径按钮（Betweenness Centrality）**
**(2) 模块化（Modularity Class）**
![](https://img-blog.csdn.net/20180816194013393?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[
](https://img-blog.csdn.net/20180816194013393?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)运行过程中会弹出生成的报告图，点击“关闭”按钮即可，如下图所示：![](https://img-blog.csdn.net/20180816194230251?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[
](https://img-blog.csdn.net/20180816194230251?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)接着设置节点和边的属性。
**(1) 设置节点大小（第二个图标）Ranking为度。点击“运行”后，可以看到节点的大小有所改变，这里的节点大小对应节点的度，例如作者“王二”与五个作者共现，则大小设置为5；也可以设置为平均路径按钮（Betweenness Centrality）。**
![](https://img-blog.csdn.net/20180816194613634?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[
](https://img-blog.csdn.net/20180816194613634?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**(2) 设置节点颜色（第一个图标）Partition为Modularity Class（模块化），为不同的关系设置不同的颜色，如下图所示。**
![](https://img-blog.csdn.net/20180816195043696?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[
](https://img-blog.csdn.net/20180816195043696?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
---

# 四.设置关系图片布局
**1.设置为Fruchterman Reingold类型。FR算法(Fruchterman-Reingold) 属于力引导布局算法类别，网络局部算法。**
![](https://img-blog.csdn.net/20180816195537635?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[
](https://img-blog.csdn.net/20180816195537635?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)输出结果如下图所示：![](https://img-blog.csdn.net/20180816195654372?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[
](https://img-blog.csdn.net/20180816195654372?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)**2.设置为Force Atlas类型。同时将斥力强度从200设置为2000，这样点之间的线会分得更开。**
![](https://img-blog.csdn.net/20180816200013241?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[
](https://img-blog.csdn.net/20180816200013241?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)接着点击预览，点击“显示节点标签”并设置为中文汉字，勾选“比例大小”，否则节点很大的字体很大，可以看到图像如下所示：![](https://img-blog.csdn.net/20180816200222471?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[
](https://img-blog.csdn.net/20180816200222471?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)但是图形中存在很多两两在一起的点，他们是常见的硕士论文和博士论文，导师和学生的合作，或者仅有两个作者的文章。![](https://img-blog.csdn.net/20180816200458886?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[
](https://img-blog.csdn.net/20180816200458886?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)如何过滤这些关系比较孤立的群体呢？
可以通过“过滤”中点击“拓扑”=>“在度的范围”，将关系较少的子图过滤。![](https://img-blog.csdn.net/20180816200717504?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[
](https://img-blog.csdn.net/20180816200717504?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)输出结果如下图所示：![](https://img-blog.csdn.net/20180816201255331?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[
](https://img-blog.csdn.net/20180816201255331?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)最后推荐下面文章给大家学习：
(1) 视频:[http://www.iqiyi.com/w_19ru91d0vt.html](http://www.iqiyi.com/w_19ru91d0vt.html)
(2) 百度文库:[https://wenku.baidu.com/view/29328a9f19e8b8f67d1cb92d.html](https://wenku.baidu.com/view/29328a9f19e8b8f67d1cb92d.html)
(3) 博客文章:[Gephi学习笔记-画出图形基本流程](https://blog.csdn.net/suncherrydream/article/details/62889400)
PS：感谢博友的反馈及相关知识，本文应用的应该是共现矩阵不是共线矩阵吧。英文名是Co-occurrence Matrix，也翻译成共生矩阵

