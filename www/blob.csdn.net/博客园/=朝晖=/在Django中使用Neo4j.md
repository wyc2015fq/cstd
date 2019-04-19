# 在Django中使用Neo4j - =朝晖= - 博客园
# [在Django中使用Neo4j](https://www.cnblogs.com/dhcn/p/7124786.html)
      重要的先说在前面吧，最后的选型结构是安装了最新的neo4j版本3.0.3，使用了neo4j-rest-client客户端库。主要原因是更适用于django的neomodel库目前只支持neo4j2.2.
     来自官方的文档--Using Neo4j from Python：https://neo4j.com/developer/python/   这篇官方文档为Django开发推荐NeoModel(http://neomodel.readthedocs.io/en/latest/)。另外这个文档的末尾有多篇推荐阅读，会列在最后:
     neo4j在debian系上的安装：http://debian.neo4j.org/?_ga=1.23681983.403762323.1467875632
     安装命令可能遇到以下错误：
- The following packages have unmet dependencies:  
-  neo4j : Depends: java8-runtime but it is not installable or  
-                   j2re1.8 but it is not installable  
- E: Unable to correct problems, you have held broken packages.  
采用以下命令解决以上问题：
- sudo add-apt-repository ppa:webupd8team/java  
- sudo apt-get update  
- sudo apt-get install oracle-java8-installer  
上面是安装官方JDK，也可以安装OpenJDK：
- sudo add-apt-repository ppa:openjdk-r/ppa  
- sudo apt-get update  
- sudo apt-get install openjdk-8-jdk  
- sudo update-alternatives --config java  
- sudo update-alternatives --config javac  
安装以后启动失败提示：
- Setting up neo4j (2.2.10) ...  
-  System start/stop links for /etc/init.d/neo4j-service already exist.  
- Starting Neo4j Server...WARNING: not changing user  
- process [6234]... waiting for server to be ready.. Failed to start within 120 seconds.  
- Neo4j Server may have failed to start, please check the logs.  
- 
StackOverflow(http://stackoverflow.com/questions/28995662/what-does-warning-not-changing-user-mean-in-neo4j)提示
- sudo su   
切换成root用户做操作就Ok了。
安装时出现问题如下：
- The following actions will resolve these dependencies:  
- 
-      Keep the following packages at their current version:  
- 1)     neo4j [Not Installed]                                
- 2)     oracle-java9-installer [Not Installed]               
可以把安装指令改为
- apt-get -f install neo4j  
Neo4j官方文档入口：
[http://neo4j.com/docs/](http://neo4j.com/docs/)
The Neo4j Manual v2.2.10：http://neo4j.com/docs/2.2.10/
Neo4j Cypher Refcard 2.2.10：http://neo4j.com/docs/2.2.10/cypher-refcard/
Neo4j Bolt Driver for [Python](http://lib.csdn.net/base/python)：http://neo4j.com/docs/api/[python](http://lib.csdn.net/base/python)-driver/current/
The Neo4j [Java](http://lib.csdn.net/base/java)Developer Reference v3.0：http://neo4j.com/docs/[Java](http://lib.csdn.net/base/java)-reference/current/
The Neo4j REST API documentation v3.0:http://neo4j.com/docs/rest-docs/current/
Neo4j OGM - An Object Graph Mapping Library for Neo4j:http://neo4j.com/docs/ogm-manual/current/
The Neo4j Operations Manual v3.0:http://neo4j.com/docs/operations-manual/current/
Neo4j Cypher Refcard 3.0.3:http://neo4j.com/docs/cypher-refcard/current/
Neo4jBooks:https://neo4j.com/books/
官方文档Using Neo4j from Python的推荐阅读：
[Using Neo4j from Python](https://blog.safaribooksonline.com/2013/07/23/using-neo4j-from-python/)
[Using Neo4j from Kivy](http://www.tsartsaris.gr/kivy2neo-sending-cypher-queries-from-a-kivy-app-to-a-neo4j-database)
[A script to automatically migrate relational databases to Neo4J](https://github.com/lycofron/pysql2neo4j)
[Py2neo Spatial](http://tech.onefinestay.com/post/107881172672/py2neo-spatial)
[Holger Spill: An introduction to Python and graph databases with Neo4j](https://www.youtube.com/watch?v=ps5RtmWKwLY&feature=youtu.be&noredirect=1)
[Python NLTK/Neo4j: Analysing the transcripts of How I Met Your Mother](http://www.markhneedham.com/blog/2015/01/10/python-nltkneo4j-analysing-the-transcripts-of-how-i-met-your-mother/)
[Flask and Neo4j](http://nicolewhite.github.io/neo4j-flask/index.html)
[Using Jupyter and Neo4j with Docker](https://blog.ouseful.info/2016/04/11/getting-started-with-the-neo4j-graph-database-linking-neo4j-and-jupyter-scipy-docker-containers-using-docker-compose/)
[Neo4j Jupyter Visualization Notebook](http://nicolewhite.github.io/neo4j-jupyter/hello-world.html)
一些中文参考资料：
图形[数据库](http://lib.csdn.net/base/mysql) Neo4j 开发实战:http://www.ibm.com/developerworks/cn/java/j-lo-neo4j/index.html
neo4j使用指南(官方文档概要):http://blog.csdn[.NET](http://lib.csdn.net/base/dotnet)/gtuu0123/article/details/6384375
图形数据库、NOSQL和Neo4j:http://www.infoq.com/cn/articles/graph-nosql-neo4j
图数据库实践系列:http://www.linuxidc.com/[Linux](http://lib.csdn.net/base/linux)/2013-08/88766.htm
[spring](http://lib.csdn.net/base/javaee) Data Neo4j简介:http://www.infoq.com/cn/news/2013/11/spring-data-neo4j-intro
Neo4J High Availability 设置向导:http://www.68idc.cn/help/mobilesys/J2ME/20150730468082.html
Neo4j集群安装实践:http://blog.fens.me/nosql-neo4j-intro/
参考资料：https://github.com/neo4j/neo4j/issues/7031
http://www.webupd8.org/2012/09/install-[Oracle](http://lib.csdn.net/base/oracle)-java-8-in-ubuntu-via-ppa.html

