# 如何在Ubuntu上安装Solr - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年02月12日 11:21:08[boonya](https://me.csdn.net/boonya)阅读数：3012










本文英文版地址：[https://examples.javacodegeeks.com/enterprise-java/apache-solr/install-solr-ubuntu/](https://examples.javacodegeeks.com/enterprise-java/apache-solr/install-solr-ubuntu/)


作者姓名：Veeramani
 Kalyanasundaram


作者简介：Veera是在电信领域工作的软件架构师，拥有丰富的Java中间件技术经验。他是一名OOAD从业者，对性能工程感兴趣。
翻译备注：本文采用谷歌翻译，请有时间的同学可以从英文版学习。

在这个“如何在Ubuntu上安装Solr”的示例中，我们将讨论如何在Ubuntu操作系统中下载和安装Solr。Ubuntu桌面操作系统为世界各地数百万台PC和笔记本电脑供电。所以这个例子是专门为Ubuntu上的用户和希望在Ubuntu上安装Solr。与Solr安装一起，我们还将向您展示如何创建Solr核心并索引与Solr一起提供的示例文件。


我们首选的环境是Ubuntu 14.x和solr-5.x。在开始Solr安装之前，请确保已安装JDK并已正确设置Java_Home。


## 1.安装Apache Solr

首先，让我们从以下位置下载最新版本的Apache Solr：

`http://www.eu.apache.org/dist/lucene/solr/5.3.1/`

文件： `solr-5.3.1.tgz`

一旦文件下载，在/ opt下创建一个名为solr的目录并移动下载的文件。
现在导航到目录`/opt/solr`并使用以下命令解压缩该文件。

`sudo tar -xvf solr-5.3.1.tgz`

Solr命令必须从bin目录中执行，因此请导航到以下路径。

`/opt/solr/solr-5.3.1/bin`

提取的目录将如下所示。

![solr_ubuntu_folder](https://examples.javacodegeeks.com/wp-content/uploads/2015/11/solr_ubuntu_folder.jpg)
Solr Ubuntu文件夹


`bin`文件夹包含用于启动和停止服务器的脚本。`example`文件夹包含几个示例文件。我们将使用其中一个来演示Solr如何对数据进行索引。`server`文件夹包含用于写入所有Solr日志的`logs`文件夹。在索引期间检查日志中的任何错误将有所帮助。服务器下的`solr`文件夹包含不同的集合或核心。
每个核/集合的配置和数据存储在相应的核/集合文件夹中。

Apache Solr附带了一个内置的Jetty服务器。
但在我们启动solr实例之前，我们必须验证JAVA_HOME是否在机器上设置。

现在使用以下命令启动Solr服务器。

`sudo ./solr start`

这将在默认端口8983下启动Solr服务器。我们现在可以在浏览器中打开以下URL，并验证我们的Solr实例正在运行。

`http://localhost:8983/solr/#/`

![Solr Ubuntu控制台](https://examples.javacodegeeks.com/wp-content/uploads/2015/12/solr_ubuntu_console.jpg)
Solr Ubuntu控制台


## 2.配置Apache Solr

当Solr服务器在独立模式下启动时，配置称为核心，当在SolrCloud模式下启动时，配置称为集合。在这个例子中，我们将讨论独立的服务器和核心。我们将暂停SolrCloud讨论以供日后使用。

首先，我们需要创建一个用于索引数据的Core。
Solr create命令有以下选项：
- **-c <name>**
- 要创建的核心或集合的名称（必需）。
- **-d <confdir>**- 配置目录，在SolrCloud模式中有用。
- **-n <configName>**- 配置名称。此默认值与核心或集合的名称相同。
- **-p <port>**
- 发送create命令的本地Solr实例的端口;默认情况下，脚本尝试通过查找正在运行的Solr实例来检测端口。
- **-s <shards>**
- 将集合拆分为多少个分片，默认值为1。
- **-rf <replicas>**- 集合中每个文档的副本数。默认值为1。

在本例中，我们将使用-c参数作为核心名称，使用-d参数作为配置目录。
对于所有其他参数，我们使用默认设置。

现在浏览`solr-5.3.1/bin`目录并发出以下命令

`sudo ./solr create -c jcg -d basic_configs`

我们可以在命令窗口中看到以下输出。



|`01`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>Setup new core instance directory:</span>设置新的核心实例目录：</span>`|
|----|----|


|`02`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>/opt/solr/solr-5.3.1/server/solr/jcg</span> /opt/solr/solr-5.3.1/server/solr/jcg</span>`|
|----|----|


|`03`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>Creating new core``'jcg'``using ``command``:</span>使用命令创建新核心``'jcg'``：</span>`|
|----|----|


|`04`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"````>[http://localhost:8983/solr/admin/cores?action=CREATE&name=jcg&instanceDir=jcg</span](http://localhost:8983/solr/admin/cores?action=CREATE&name=jcg&instanceDir=jcg%3C/span)> http：// localhost：8983 / solr / admin / cores？action = CREATE＆name = jcg＆instanceDir = jcg</span>```|
|----|----|


|`05`||
|----|----|


|`06`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>{</span> {</span>`|
|----|----|


|`07`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>``"responseHeader"``:{</span> “responseHeader”：{</span>`|
|----|----|


|`08`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>``"status"``:0,</span> “status”：0，</span>`|
|----|----|


|`09`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>``"QTime"``:5862},</span> “QTime”：5862}，</span>`|
|----|----|


|`10`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>``"core"``:``"jcg"````}</span> “core”：“jcg”}</span>```|
|----|----|




现在编辑`/server/solr/jcg/conf`文件夹中的schema.xml文件，并在uniqueKey元素后面添加以下内容。

*schema.xml*



|`1`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``><uniqueKey>``id````</uniqueKey></span> <uniqueKey>````id``</ uniqueKey></span>`|
|----|----|


|`2`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``><!-- Fields added``for``books.csv load--></span> <！ - 为books.csv加载添加的字段 - ></span>`|
|----|----|


|`3`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``><field name=``"cat"``type``=``"text_general"``indexed=``"true"``stored=``"true"``/></span> <field name =“``cat``”``type````=“text_general”indexed =“````true``”stored =“``true``”/></span>`|
|----|----|


|`4`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``><field name=``"name"``type``=``"text_general"``indexed=``"true"``stored=``"true"``/></span> <field name =“name”``type``=“text_general”indexed =“``true````”stored =“````true``”/></span>`|
|----|----|


|`5`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``><field name=``"price"``type``=``"tdouble"``indexed=``"true"``stored=``"true"``/></span> <field name =“price”``type``=“tdouble”indexed =“``true````”stored =“````true``”/></span>`|
|----|----|


|`6`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``><field name=``"inStock"``type``=``"boolean"``indexed=``"true"``stored=``"true"``/></span> <field name =“inStock”``type``=“boolean”indexed =“``true````”stored =“````true``”/></span>`|
|----|----|


|`7`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``><field name=``"author"``type``=``"text_general"``indexed=``"true"``stored=``"true"``/></span> <field name =“author”``type``=“text_general”indexed =“``true````”stored =“````true``”/></span>`|
|----|----|




由于我们已经修改了配置，我们必须停止和启动服务器。
为此，我们需要通过命令行从bin目录发出以下命令：

`sudo ./solr stop -all`

服务器将立即停止。现在启动服务器通过命令行从bin目录发出以下命令：

`sudo ./solr start`

## 3.索引数据

Apache Solr附带一个名为SimplePostTool的独立Java程序。
这个程序被打包到JAR中，安装在文件夹example / exampledocs下。

现在，我们导航到命令提示符中的/ example / exampledocs文件夹，并键入以下命令。您将看到一堆使用该工具的选项。

`java -jar post.jar -h`

使用格式一般如下：


```
Usage: java [SystemProperties] -jar post.jar [-h|-] [<file|folder|url|arg>

[<file|folder|url|arg>...]]
```


正如我们前面所说，我们将索引solr安装附带的“books.csv”文件中的数据。
我们将导航到命令提示符中的/ example / exampledocs并发出以下命令。

`java -Dtype=text/csv -Durl=http://localhost:8983/solr/jcg/update -jar post.jar books.csv`

这里使用的SystemProperties是：
- -Dtype - 数据文件的类型。
- -Durl - jcg核心的URL。



|`1`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>SimplePostTool version 5.0.0</span> SimplePostTool版本5.0.0</span>`|
|----|----|


|`2`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"````>Posting files to [base] url[http://localhost:8983/solr/jcg/update](http://localhost:8983/solr/jcg/update) using content-````type``text/csv...</span>将文件发布到[base] url http：// localhost：8983 / solr / jcg / update使用content-``type``text / csv ...</span>`|
|----|----|


|`3`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>POSTing``file``books.csv to [base]</span>将文件books.csv发布到[base]</span>`|
|----|----|


|`4`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>1 files indexed.</span> 1个文件已索引。</span>`|
|----|----|


|`5`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"````>COMMITting Solr index changes to[http://localhost:8983/solr/jcg/update...</span](http://localhost:8983/solr/jcg/update...%3C/span)>提交Solr索引更改为http：// localhost：8983 / solr / jcg / update ...</span>```|
|----|----|


|`6`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>Time spent: 0:00:01.149</span>所用时间：0：00：01.149</span>`|
|----|----|




现在，来自示例文件的数据被索引和存储。
让我们打开以下URL。我们可以看到与示例文件中的数据计数匹配的文档数。

`http://localhost:8983/solr/#/jcg`

![Solr的Ubuntu数据](https://examples.javacodegeeks.com/wp-content/uploads/2015/12/solr_ubuntu_data.jpg)
Solr Ubuntu数据


## 4.下载模式文件

**下载**
您可以下载此示例中使用的模式文件： [**schema.xml**](http://examples.javacodegeeks.com/wp-content/uploads/2015/11/schema.xml_.zip)



