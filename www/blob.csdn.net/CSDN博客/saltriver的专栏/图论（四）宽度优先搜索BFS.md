
# 图论（四）宽度优先搜索BFS - saltriver的专栏 - CSDN博客


2017年01月14日 21:22:04[saltriver](https://me.csdn.net/saltriver)阅读数：4466标签：[搜索																](https://so.csdn.net/so/search/s.do?q=搜索&t=blog)[遍历																](https://so.csdn.net/so/search/s.do?q=遍历&t=blog)[数据结构与算法																](https://so.csdn.net/so/search/s.do?q=数据结构与算法&t=blog)[bfs																](https://so.csdn.net/so/search/s.do?q=bfs&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=数据结构与算法&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=遍历&t=blog)个人分类：[数据结构与算法																](https://blog.csdn.net/saltriver/article/category/6506969)
[
																								](https://so.csdn.net/so/search/s.do?q=遍历&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=搜索&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=搜索&t=blog)

宽度优先搜索(BFS, Breadth First Search)是一个针对图和树的遍历算法。发明于上世纪50年代末60年代初，最初用于解决迷宫最短路径和网络路由等问题。
对于下面的树而言，BFS方法首先从根节点1开始，其搜索节点顺序是1,2,3,4,5,6,7,8。
![这里写图片描述](https://img-blog.csdn.net/20170114211759962?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170114211759962?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
BFS使用队列(queue)来实施算法过程，队列(queue)有着先进先出FIFO(First Input First Output)的特性，BFS操作步骤如下：
1、把起始点放入queue；
2、重复下述2步骤，直到queue为空为止：
1) 从queue中取出队列头的点；
2) 找出与此点邻接的且尚未遍历的点，进行标记，然后全部放入queue中。
下面结合一个图(graph)的实例，说明BFS的工作过程和原理：
（1）将起始节点1放入队列中，标记为已遍历：
![这里写图片描述](https://img-blog.csdn.net/20170114211828038?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170114211828038?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
（2）从queue中取出队列头的节点1，找出与节点1邻接的节点2,3，标记为已遍历，然后放入queue中。
![这里写图片描述](https://img-blog.csdn.net/20170114211840976?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170114211840976?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
（3）从queue中取出队列头的节点2，找出与节点2邻接的节点1,4,5，由于节点1已遍历，排除；标记4,5为已遍历，然后放入queue中。
![这里写图片描述](https://img-blog.csdn.net/20170114211850086?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170114211850086?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
（4）从queue中取出队列头的节点3，找出与节点3邻接的节点1,6,7，由于节点1已遍历，排除；标记6,7为已遍历，然后放入queue中。
![这里写图片描述](https://img-blog.csdn.net/20170114211907775?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170114211907775?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
（5）从queue中取出队列头的节点4，找出与节点4邻接的节点2,8，2属于已遍历点，排除；因此标记节点8为已遍历，然后放入queue中。
![这里写图片描述](https://img-blog.csdn.net/20170114211918416?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170114211918416?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
（6）从queue中取出队列头的节点5，找出与节点5邻接的节点2,8，2,8均属于已遍历点，不作下一步操作。
![这里写图片描述](https://img-blog.csdn.net/20170114211927463?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170114211927463?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
（7）从queue中取出队列头的节点6，找出与节点6邻接的节点3,8,9，3,8属于已遍历点，排除；因此标记节点9为已遍历，然后放入queue中。
![这里写图片描述](https://img-blog.csdn.net/20170114211935619?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170114211935619?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
（8）从queue中取出队列头的节点7，找出与节点7邻接的节点3, 9，3,9属于已遍历点，不作下一步操作。
![这里写图片描述](https://img-blog.csdn.net/20170114211944588?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170114211944588?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
（9）从queue中取出队列头的节点8，找出与节点8邻接的节点4,5,6，4,5,6属于已遍历点，不作下一步操作。
![这里写图片描述](https://img-blog.csdn.net/20170114211952072?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170114211952072?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
（10）从queue中取出队列头的节点9，找出与节点9邻接的节点6,7，6,7属于已遍历点，不作下一步操作。
![这里写图片描述](https://img-blog.csdn.net/20170114211959744?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170114211959744?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
（11）queue为空，BFS遍历结束。
[            ](https://img-blog.csdn.net/20170114211959744?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

[
  ](https://img-blog.csdn.net/20170114211952072?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2FsdHJpdmVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)