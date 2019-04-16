# javaweb基础第二十四课：文件上传之准备工作 - PeterBishop - CSDN博客





2019年01月12日 22:15:15[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：26
个人分类：[JavaWeb](https://blog.csdn.net/qq_40061421/article/category/8534452)









大纲：
- 文件表单域
- 上传文件要注意的事项



  这节课我们开始学文件的上传和下载，我先准备好文件。OK。

  然后我们学习下文件上传的前端注意事项。


- 文件表单域

我们要上传文件首先要写个文件表单域，写法如下

<input type=”file”></input>

这个文件表单域可以让我们选择文件，来看下效果。

![](https://img-blog.csdnimg.cn/20190112221254217.png)



这个就是文件表单域的效果了

选好文件后效果如下:

![](https://img-blog.csdnimg.cn/20190112221302875.png)





2.上传文件要注意的事项
- 可以看到如果是get请求是不行的![](https://img-blog.csdnimg.cn/20190112221308673.png)



              2.我们发现一个问题，这个怎么只是上传了一个文件名啊，我们要的是一个文件啊，或者说是这个文件的二进制流啊，注意，上传文件时，要设置form标签的enctype属性
enctype="multipart/form-data"

3.最终效果
![](https://img-blog.csdnimg.cn/20190112221443302.png)

这个就代表正确的上传了文件，当然这只是前端部分。下节课我们讲后端部分。





4.总结：

文件上传前端部分注意事项
- - 要有文件表单域，且必须设置name属性
- 提交请求的方式必须是post
- 必须设置form标签的enctype属性为multipart/form-data




