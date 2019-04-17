# 新版本PyCharm Debug mode off解决方案 - zhusongziye的博客 - CSDN博客





2018年11月10日 21:38:50[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：402








新版的PyCharm在执行时会出现一些问题，不知道大家有没有遇到：

![](https://img-blog.csdnimg.cn/20181110213454295.png)

就是即使代码中增加了`app.debug=True`执行时这里仍然是`off`，网上答案说在`Edit Configurations`中勾选`FLASK_DEBUG`：

![](https://img-blog.csdnimg.cn/20181110213538349.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181110213604754.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)





但是船长试过以后发现出现了另一个问题，就是在右键运行的时候：

![](https://img-blog.csdnimg.cn/20181110213617658.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

后面这个数字会不停的增加。这显然不是我们希望看到的。

现在给出船长的解决方式：

一、在`Edit Configurations`中，先复制这个`Target`

![](https://img-blog.csdnimg.cn/20181110213655933.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

二、然后点击`-`将下面两个`Flask server`文件删掉：

![](https://img-blog.csdnimg.cn/20181110213713581.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

三、删除后会跳到下图页面，点击`+`，然后不要选择Flask server而是选择Python，注意，必须点击`+`增加，要不然直接选择`Python`增加不了：

![](https://img-blog.csdnimg.cn/20181110213728391.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

四、选择了`Python`后将复制来的文件路径粘贴到`Script path`中，并修改上面的`Name`为`app.py`，点击`ok`保存：

![](https://img-blog.csdnimg.cn/20181110213744134.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

此时运行这里变成`Python`来执行：

![](https://img-blog.csdnimg.cn/20181110213754931.png)

右键也变为：

![](https://img-blog.csdnimg.cn/20181110213803119.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

执行文件：

![](https://img-blog.csdnimg.cn/20181110213811950.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



