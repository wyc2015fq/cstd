
# 使用Java程序消费SAP Leonardo的机器学习API - 喜欢打酱油的老鸟 - CSDN博客


2018年08月11日 11:20:40[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：67标签：[JAVA																](https://so.csdn.net/so/search/s.do?q=JAVA&t=blog)[SAP 																](https://so.csdn.net/so/search/s.do?q=SAP &t=blog)[Leonard																](https://so.csdn.net/so/search/s.do?q=Leonard&t=blog)[机器学习																](https://so.csdn.net/so/search/s.do?q=机器学习&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=Leonard&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=SAP &t=blog)个人分类：[SAP文章																](https://blog.csdn.net/weixin_42137700/article/category/7781764)[人工智能																](https://blog.csdn.net/weixin_42137700/article/category/7820233)[
							](https://blog.csdn.net/weixin_42137700/article/category/7781764)
[
				](https://so.csdn.net/so/search/s.do?q=SAP &t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=SAP &t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=JAVA&t=blog)
[http://blog.itpub.net/24475491/viewspace-2154590/](http://blog.itpub.net/24475491/viewspace-2154590/)
以sap leonardo作为关键字在微信上搜索，能搜到不少文章。但是我浏览了一下，好像没有发现有从具体编程角度上来介绍的。所以我就贡献一篇。
![](https://upload-images.jianshu.io/upload_images/2085791-725419ae7ffc33c2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
# 需求
开发一个Java程序，用户可以指定一张图片，该Java程序调用SAP Leonardo上训练好的机器学习API，该API会识别该图片，给用户返回一个文本信息，告诉用户识别结果。
访问[https://api.sap.com](https://link.jianshu.com/?t=https%3A%2F%2Fapi.sap.com), 点击API：
![](https://upload-images.jianshu.io/upload_images/2085791-b6e8789b05b4400a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
SAP Leonardo Machine Learning - Functional Services
![](https://upload-images.jianshu.io/upload_images/2085791-30b3ac1bcd62670e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
点击Product Image Classification API：
![](https://upload-images.jianshu.io/upload_images/2085791-e44f07fea9f0e957.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
会看到如下界面。该界面包含了这个API的Model Schema介绍，即您调用API之后，返回的响应结构里包含哪些字段，以及对应的类型（对象或者数组）。界面里包含了一个小型的API调用控制台，您可以直接控制台里选择一个本地图形文件，然后点击Try it out按钮来体验一下API的效果。
![](https://upload-images.jianshu.io/upload_images/2085791-a4e4c4d6275ef908.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
例如我使用了下面这张图片进行测试：
![](https://upload-images.jianshu.io/upload_images/2085791-92283e3633977174.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/474)
在控制台里看到请求处理成功，API判断出来该图片有97%的可能性是一台notebook。
![](https://upload-images.jianshu.io/upload_images/2085791-5066fc5dfb45c937.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/316)
下面我们需要使用Java程序来调用API。
前一步骤的API控制台的右上角有一个按钮Download SDK，点击之后将SDK下载到本地：
![](https://upload-images.jianshu.io/upload_images/2085791-61a3488501ec7012.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
可以看到该SDK是一个基于Gradle的Java项目。您需要下载Maven和Gradle并完成环境变量的配置，这些配置非常基础，在网上有大量资料，这里不再重复。
完成配置后，使用Eclipse的import功能，将SDK导入到Eclipse里。
![](https://upload-images.jianshu.io/upload_images/2085791-82f3abf0a4acef01.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/557)
导入完成后如图。红色区域是SDK自带的代码，蓝色区域是Jerry自己手动创建的代码，用于调用API并打印结果。
![](https://upload-images.jianshu.io/upload_images/2085791-050a6d8390eecc14.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
双击根目录下的pom.xml, 将如下依赖关系维护进去：
![](https://upload-images.jianshu.io/upload_images/2085791-a378cd3c78efed5b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
![](https://upload-images.jianshu.io/upload_images/2085791-8b73c3d9a74b014b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/679)
在项目根目录下执行Maven命令 mvn install, 确保项目成功build。至此，您可以开始编写Java代码使用SAP提供的JDK来调用该机器学习API了。
![](https://upload-images.jianshu.io/upload_images/2085791-431afac991e5928f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
使用下图的Java代码调用SDK去消费API。SDK封装了底层HTTP请求的发送和响应的解析等细节，使用起来非常简捷。
![](https://upload-images.jianshu.io/upload_images/2085791-f4b0108c70769b93.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
为简单起见，上图13行我将本地图片文件的绝对地址硬编码进去。上图第八行硬编码的API key来自API控制台：
![](https://upload-images.jianshu.io/upload_images/2085791-b2a647612e1feb39.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
至此，大功告成，运行程序。。。。。。什么情况？
![](https://upload-images.jianshu.io/upload_images/2085791-b13db25531f6cfbf.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
# 错误分析
仔细看过错误消息，很快Jerry就发现了问题出在哪里。上图蓝色下划线就暗示了错误根源。SAP SDK使用了Google发布的一个开源Java库Gson，将API返回的响应反序列化成Java对象。有了Gson，应用程序开发人员无需重复造轮子，只需要定义一些Java类，作为容器存储Gson反序列化的执行结果即可。
我们再回到API控制台，API响应结构里request字段的类型声明为string。
![](https://upload-images.jianshu.io/upload_images/2085791-298341b5a171a2c5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
然而通过测试发现，request字段的内容实际是一个Json对象：
![](https://upload-images.jianshu.io/upload_images/2085791-fb9b110684b6d111.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/460)
从[http://api.sap.com](https://link.jianshu.com/?t=http%3A%2F%2Fapi.sap.com)下载的SDK里的代码，Gson容器类Response.java里声明的request字段类型为String，而非对象，因此Gson会抛异常：Expected a string but was BEGIN_OBJECT at line 31 column 15 path $.request
![](https://upload-images.jianshu.io/upload_images/2085791-ac17d3dc486aa7bb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
知道了问题根源，改起来就容易了。新建一个Gson容器类Request.java, 然后按照控制台里观察到的request字段的结构，为Request.java维护对应的成员变量。
例如tenantName在API响应结构里是一个String，因此在Java类里的类型是String；files在API响应结构里是一个数组，因此在Java类里的类型为List<String>。
![](https://upload-images.jianshu.io/upload_images/2085791-37339d5d73d0f40b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
调整完毕之后再次执行，能看到期望的输出结果：通过SDK调用API并打印结果。
![](https://upload-images.jianshu.io/upload_images/2085791-b9dd3a42d3d8ec7c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/700)
如果您不愿意自己手动修改，可以直接从我的github下载修改之后的代码，只需要用您自己的API key替换掉代码中硬编码的key即可。
[https://github.com/i042416/ProductImageMLService](https://link.jianshu.com/?t=https%3A%2F%2Fgithub.com%2Fi042416%2FProductImageMLService)
更多Leonardo精彩，尽在[https://api.sap.com](https://link.jianshu.com/?t=https%3A%2F%2Fapi.sap.com)！

