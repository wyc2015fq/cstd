# 爬虫必备工具 —— Chrome 开发者工具 - zhusongziye的博客 - CSDN博客





2018年12月20日 21:21:35[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：273








当我们爬取不同的网站是，每个网站页面的实现方式各不相同，我们需要对每个网站都进行分析。那是否有一些通用的分析方法？我分享下自己爬取分析的“套路”。在某个网站上，分析页面以及抓取数据，我用得最多的工具是 **Chrome 开发者工具**。

Chrome 开发者工具是一套内置于 Google Chrome 中的 Web 开发和调试工具，可用来对网站进行迭代、调试和分析。因为国内很多浏览器内核都是基于 Chrome 内核，所以国产浏览器也带有这个功能。例如：UC 浏览器、QQ 浏览器、360 浏览器等。

接下来，我们来看看 Chrome 开发者工具一些比较牛逼的功能。

**01元素面板**

通过元素（Element）面板，我们能查看到想抓取页面渲染内容所在的标签、使用什么 CSS 属性（例如：class="middle"）等内容。例如我想要抓取我知乎主页中的动态标题，在网页页面所在处上右击鼠标，选择“**检查**”，可进入 Chrome 开发者工具的**元素面板**。

![](https://img-blog.csdnimg.cn/20181220211556239.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

通过这种方法，我们能快速定位出页面某个DOM 节点，然后可以提取出相关的解析语句。鼠标移动到节点，然后右击鼠标，选择 “Copy”，能快速复制出 Xpath 、CSS elector 等内容解析库的解析语句。

![](https://img-blog.csdnimg.cn/20181220211618105.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



**02网络面板**

网络（Network）面板记录页面上每个网络操作的相关信息，包括详细的耗时数据、HTTP 请求与响应标头和 Cookie，等等。这就是我们通常说的抓包。

![](https://img-blog.csdnimg.cn/20181220211708642.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

其中的 **Filters** 窗格，我经常使用其来过滤出一些 HTTP 请求，例如过滤出使用 Ajax 发起的异步请求、图片、视频等。

最大的窗格叫 **Requests Table**，此表格会列出了检索的每一个 HTTP 请求。 默认情况下，此表格按时间顺序排序，最早的资源在顶部。点击资源的名称可以显示更多信息。

![](https://img-blog.csdnimg.cn/20181220211929225.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)
- 
**Headers** 是显示 HTTP 请求的 Headers，我们通过这个能看到请求的方式，以及携带的请求参数等。

- 
**Preview** 是请求结果的预览。一般用来查看请求到的图片，对于抓取图片网站比较给力。

- 
**Response** 是请求返回的结果。一般的内容是整个网站的源代码。如果该请求是异步请求，返回的结果内容一般是 Json 文本数据。

- 
**Cookies** 能看到请求携带的 Cookies 以及服务器返回的 Cookies。有时候是需要使用到 Response 的 Cookies。例如，某个页面必须登录才能看到。




