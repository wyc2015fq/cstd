# 八爪鱼采集器︱加载更多、再显示20条图文教程（Xpatth、Ajax） - 素质云笔记/Recorder... - CSDN博客





2016年10月19日 18:40:19[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：3662












**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)


———————————————————————————


由于代码布置采集器比较麻烦，又很早知道八爪鱼采集器的强大，所以把一些常规的采集内容贴成图文教程，供以后使用。




八爪鱼采集器官方视频教程见：http://www.bazhuayu.com/tutorial/zxs.aspx?t=0




采集流程：

1、打开网页

2、寻找“加载更多”、“在显示20条”的Xpath的路径

3、设置循环、Ajax

4、循环-提取内容

5、内容正则化

6、布置采集




注意是：先循环完列表，然后再提取数据。




——————————————————————————————




# 1、打开网页




打开网页部分，就是你自己想要的网页。

![](https://img-blog.csdn.net/20161019181108772?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





——————————————————————————————





# 2、寻找“加载更多”、“在显示20条”的Xpath的路径




官方认为火狐浏览器，加载一些插件就可以了。

![](https://img-blog.csdn.net/20161019181224071?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


在附加组件里面。需要加载两个插件：firebug、firePath。自己搜索一下，就能下载得到。

![](https://img-blog.csdn.net/20161019181424740?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


右键：“使用Firebug查看元素”，

点击“FirePath”

点击原页面的“加载页面”，Xpath就会显示出来了。

![](https://img-blog.csdn.net/20161019181631905?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





——————————————————————————————


# 3、设置循环、Ajax




然后填写在“单个元素“里面，如果上面出现字样——”加载更多“，那么就代表这个步骤设置成功。

![](https://img-blog.csdn.net/20161019182221930?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





同时，拖拽一个点击元素在里面，并在”高级选项“设置两个步骤：

勾选“使用循环”；

勾选Ajax加载，设置2s。

![](https://img-blog.csdn.net/20161019182412587?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





——————————————————————————————


# 4、循环-提取内容




这个步骤，跟常规的设置一样。

直接点击元素——创建一个元素列表以处理一组元素——添加列表——继续编辑列表——再点击——添加到列表——创建列表完成——循环

同时注意的是：如果是加载更多、再显示的问题，不能放在上述的循环之上，要放在之下。




5、提取数据——正则化

提取数据的时候，会出现需要只选择一部分，需要选择：

![](https://img-blog.csdn.net/20161019183650983?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


格式化数据-添加步骤，然后进行步骤的选择。




——————————————————————————————





最终的流程表：

![](https://img-blog.csdn.net/20161019183800155?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)






**每每以为攀得众山小，可、每每又切实来到起点，大牛们，缓缓脚步来俺笔记葩分享一下吧，please~**![](https://img-blog.csdn.net/20161213101203247)


———————————————————————————




