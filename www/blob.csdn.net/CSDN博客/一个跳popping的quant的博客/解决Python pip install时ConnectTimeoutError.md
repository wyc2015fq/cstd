# 解决Python pip install时ConnectTimeoutError - 一个跳popping的quant的博客 - CSDN博客





2018年05月29日 16:39:57[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：2645
所属专栏：[学python的路上](https://blog.csdn.net/column/details/20561.html)









今天在用anaconda3安装tensorflow时，出现了ConnectTimeoutError的错误：

![](https://img-blog.csdn.net/2018052916351140)


原因是pip的源地址是官方网址：pypi.python.org，网络协议：HTTPS，通过这些国外站点下载时就会特别慢。因而可以通过国内的一些镜像网站：

阿里云 [http://mirrors.aliyun.com/pypi/simple/](http://mirrors.aliyun.com/pypi/simple/)


中国科技大学 [https://pypi.mirrors.ustc.edu.cn/simple/](https://pypi.mirrors.ustc.edu.cn/simple/%20)

豆瓣(douban) [http://pypi.douban.com/simple/](http://pypi.douban.com/simple/)


清华大学 [https://pypi.tuna.tsinghua.edu.cn/simple/](https://pypi.tuna.tsinghua.edu.cn/simple/)


中国科学技术大学 [http://pypi.mirrors.ustc.edu.cn/simple/](http://pypi.mirrors.ustc.edu.cn/simple/)

在pip时，只需要在后面加上镜像地址就可以了：

```python
#例如安装tensorflow时使用豆瓣的源  
    pip install --index-url https://pypi.douban.com/simple tensorflow
```
![](https://img-blog.csdn.net/20180529163911568)



