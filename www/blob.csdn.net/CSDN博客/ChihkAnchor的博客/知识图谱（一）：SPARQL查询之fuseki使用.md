# 知识图谱（一）：SPARQL查询之fuseki使用 - ChihkAnchor的博客 - CSDN博客





2019年02月26日 17:12:10[Chihk-Anchor](https://me.csdn.net/weixin_40871455)阅读数：84








Fuseki是一个SPARQL服务器。它通过HTTP提供使用SPARQL协议的REST式SPARQL HTTP更新，SPARQL查询和SPARQL更新。 下载地址：[清华镜像站](https://mirrors.tuna.tsinghua.edu.cn/apache/jena/binaries/)

![](https://img-blog.csdnimg.cn/20190226165209350.png)

下载完了之后解压缩 

![](https://img-blog.csdnimg.cn/20190226165252464.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

启动方式：双击fuseki-server.bat,出现如下界面：

![](https://img-blog.csdnimg.cn/20190226165409558.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

使用浏览器访问：http://localhost:3030

![](https://img-blog.csdnimg.cn/20190226165944981.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

然后添加数据，上传我们已有的rdf文件，例如xxx.ttl、xxx.nt等

![](https://img-blog.csdnimg.cn/2019022617031919.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/2019022617060259.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)

文件上传成功！可以上传多个文件。文件格式可以是多个支持RDF文件的格式。上传到我们后台数据库中了。 再看看命令终端显示Fuseki的信息，也是成功了.  

![](https://img-blog.csdnimg.cn/20190226170702108.png)

输入sparql语句进行查询：

![](https://img-blog.csdnimg.cn/20190226171031478.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)





