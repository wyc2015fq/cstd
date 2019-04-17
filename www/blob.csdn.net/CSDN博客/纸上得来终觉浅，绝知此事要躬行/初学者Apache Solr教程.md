# 初学者Apache Solr教程 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年02月12日 10:46:14[boonya](https://me.csdn.net/boonya)阅读数：1635








本文英文版地址：[https://examples.javacodegeeks.com/enterprise-java/apache-solr/apache-solr-tutorial-beginners/](https://examples.javacodegeeks.com/enterprise-java/apache-solr/apache-solr-tutorial-beginners/)


作者姓名：Veeramani
 Kalyanasundaram


作者简介：Veera是在电信领域工作的软件架构师，拥有丰富的Java中间件技术经验。他是一名OOAD从业者，对性能工程感兴趣。

翻译备注：本文采用谷歌翻译，请有时间的同学可以从英文版学习。






在Apache Solr Tutorial for Beginners示例中，我们将讨论如何安装最新版本的Apache Solr并告诉您如何配置它。此外，我们将向您展示如何使用示例数据文件执行索引。Apache Solr支持从不同的源格式索引，包括各种数据库，PDF文件，XML文件，CSV文件等。在这个例子中，我们将研究如何从CSV文件索引数据。

我们首选的环境是Windows。在开始Solr安装之前，请确保已安装JDK并已正确设置Java_Home。



## 为什么是Apache Solr

Apache Solr是一个强大的搜索服务器，它支持像API一样的REST。
Solr由Lucene提供支持，可以实现强大的匹配功能，例如短语，通配符，连接，分组和更多的各种数据类型。
它是高度优化的高流量使用Apache Zookeeper。Apache Solr提供了大量的功能，我们列出了一个高影响功能的子集。
- 高级全文搜索功能。
- 基于开放接口的标准 - XML，JSON和Http。
- 高度可扩展和容错。
- 支持Schema和Schemaless配置。
- 分面搜索和过滤。
- 支持主要语言，如英语，德语，中文，日语，法语等
- 富文档解析。

## 2.安装Apache Solr

首先，从以下位置下载最新版本的Apache Solr：

`http://lucene.apache.org/solr/downloads.html`

截至本文，稳定版本可用是5.0.0。
Apache Solr已经经历了从4.xx到5.0.0的各种更改，因此如果您有不同版本的Solr，则需要下载5.xx版本以遵循此示例。

下载Solr zip文件后，将其解压缩到文件夹中。
提取的文件夹将如下所示。

![solr_folder](https://examples.javacodegeeks.com/wp-content/uploads/2015/04/solr_folder1.jpg)
Solr文件夹


`bin`文件夹包含用于启动和停止服务器的脚本。`example`文件夹包含几个示例文件。我们将使用其中一个来演示Solr如何对数据进行索引。`server`文件夹包含用于写入所有Solr日志的`logs`文件夹。在索引期间检查日志中的任何错误将有所帮助。服务器下的`solr`文件夹包含不同的集合或核心。
每个核/集合的配置和数据存储在相应的核/集合文件夹中。

Apache Solr附带了一个内置的Jetty服务器。
但在我们启动solr实例之前，我们必须验证JAVA_HOME是否在机器上设置。

我们可以使用命令行脚本启动服务器。让我们从命令提示符转到bin目录并发出以下命令

`solr start`

这将在默认端口8983下启动Solr服务器。

我们现在可以在浏览器中打开以下URL，并验证我们的Solr实例正在运行。
solr管理工具的具体细节超出了示例的范围。

`http://localhost:8983/solr/`

![Solr管理控制台](https://examples.javacodegeeks.com/wp-content/uploads/2015/04/solr_admin_console1.jpg)
Solr管理控制台


## 3.配置Apache Solr

在本节中，我们将向您展示如何配置solr实例的核心/集合以及如何定义字段。
Apache Solr附带一个名为Schemaless模式的选项。此选项允许用户构建有效的模式，而无需手动编辑模式文件。但是对于这个例子，我们将使用Schema配置来理解Solr的内部。

### 3.1创建核心

当Solr服务器在独立模式下启动时，配置称为核心，当它在SolrCloud模式下启动时，配置称为集合。在这个例子中，我们将讨论独立的服务器和核心。我们将暂停SolrCloud讨论以供日后使用。

首先，我们需要创建一个用于索引数据的Core。
Solr create命令有以下选项：
- **-c <name>** - 要创建的核心或集合的名称（必需）。
- **-d <confdir>** - 配置目录，在SolrCloud模式下有用。
- **-n <configName>** - 配置名称。
此默认值与核心或集合的名称相同。
- **-p <port>** - 要向其发送create命令的本地Solr实例的端口;默认情况下，脚本尝试通过查找正在运行的Solr实例来检测端口。
- **-s <shards>** - 将集合拆分为的分片数，默认值为1。
- **-rf <replicas>** - 集合中每个文档的副本数。默认值为1。

在本例中，我们将使用-c参数作为核心名称，使用-d参数作为配置目录。
对于所有其他参数，我们使用默认设置。

现在在命令窗口中导航`solr-5.0.0\bin`文件夹，并发出以下命令。

`solr create -c jcg -d basic_configs`

我们可以在命令窗口中看到以下输出。



|`1`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>Creating new core 'jcg' using command:</``span``>使用命令创建新核心'jcg'：</``span``>`|
|----|----|


|`2`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>[http://localhost:8983/solr/admin/cores?action=CREATE&name=jcg&instanceDir=jcg</](http://localhost:8983/solr/admin/cores?action=CREATE&name=jcg&instanceDir=jcg%3C/)``span````> http：// localhost：8983 / solr / admin / cores？action = CREATE＆name = jcg＆instanceDir = jcg</````span``>`|
|----|----|


|`3`||
|----|----|


|`4`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>{</``span``> {</``span``>`|
|----|----|


|`5`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>"responseHeader":{</``span``> “responseHeader”：{</``span``>`|
|----|----|


|`6`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>"status":0,</``span``> “status”：0，</``span``>`|
|----|----|


|`7`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>"QTime":663},</``span``> “QTime”：663}，</``span``>`|
|----|----|


|`8`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>"core":"jcg"}</``span``> “core”：“jcg”}</``span``>`|
|----|----|




现在我们导航到以下URL，我们可以看到jcg core被填充在核心选择器中。
您还可以查看核心的统计信息。

`http://localhost:8983/solr`

![Solr jcg core](https://examples.javacodegeeks.com/wp-content/uploads/2015/04/solr_jcg_core.jpg)
Solr JCG核心


### 3.2修改schema.xml文件

我们需要修改文件夹`server\solr\jcg\conf`下的`schema.xml`文件，以包含字段。我们将使用随同Solr安装一起提供的示例文件“books.csv”中的一个进行索引。该文件位于文件夹`solr-5.0.0\example\exampledocs`

现在我们导航到文件夹`server\solr`目录。
您将看到一个名为`jcg`的文件夹。子文件夹即`conf`和`data`分别具有核心的配置和索引数据。

现在编辑`\server\solr\jcg\conf`文件夹中的schema.xml文件，并在uniqueKey元素后面添加以下内容。

*schema.xml*



|`1`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``uniqueKey``>id</``uniqueKey``></``span``> <``uniqueKey````> id </````uniqueKey``></``span``>`|
|----|----|


|`2`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>``<!-- Fields added for books.csv load-->``</``span``> <！ - 为books.csv加载添加的字段 - ></``span``>`|
|----|----|


|`3`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``field``name``=``"cat"``type``=``"text_general"``indexed``=``"true"``stored``=``"true"``/></``span````> <````field``name =“cat”type =“text_general”indexed =“true”stored =“true”/></``span``>`|
|----|----|


|`4`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``field``name``=``"name"``type``=``"text_general"``indexed``=``"true"``stored``=``"true"``/></``span````> <````field``name =“name”type =“text_general”indexed =“true”stored =“true”/></``span``>`|
|----|----|


|`5`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``field``name``=``"price"``type``=``"tdouble"``indexed``=``"true"``stored``=``"true"``/></``span````> <````field``name =“price”type =“tdouble”indexed =“true”stored =“true”/></``span``>`|
|----|----|


|`6`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``field``name``=``"inStock"``type``=``"boolean"``indexed``=``"true"``stored``=``"true"``/></``span````> <````field``name =“inStock”type =“boolean”indexed =“true”stored =“true”/></``span``>`|
|----|----|


|`7`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``field``name``=``"author"``type``=``"text_general"``indexed``=``"true"``stored``=``"true"``/></``span````> <````field``name =“author”type =“text_general”indexed =“true”stored =“true”/></``span``>`|
|----|----|




我们已将`indexed`的属性设置为true。
这指定了用于索引的字段，并且可以使用索引检索记录。将值设置为false将使该字段只存储，但不能查询。

还要注意，我们有另一个属性叫`stored`并设置为true。
这指定字段存储并可以在输出中返回。将此字段设置为false将使该字段仅编入索引，并且无法在输出中检索。

我们已经为“books.csv”文件中的字段分配了类型。
CSV文件“id”中的第一个字段由schema.xml文件的uniqueKey元素自动处理，用于建立索引。
如果你注意到，我们错过了字段series_t，sequence_i和genre_s，不做任何输入。
但是，当我们执行索引时，所有这些字段也被索引而没有任何问题。如果你想知道这种情况，请仔细看看schema.xml文件中的dynamicField部分。

*schema.xml*



|`01`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``dynamicField``name``=``"*_i"``type``=``"int"``indexed``=``"true"``stored``=``"true"``/></``span````> <````dynamicField``name =“* _ i”type =“int”indexed =“true”stored =“true”/></``span``>`|
|----|----|


|`02`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``dynamicField``name``=``"*_is"``type``=``"ints"``indexed``=``"true"``stored``=``"true"``/></``span````> <````dynamicField``name =“* _ is”type =“ints”indexed =“true”stored =“true”/></``span``>`|
|----|----|


|`03`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``dynamicField``name``=``"*_s"``type``=``"string"``indexed``=``"true"``stored``=``"true"``/></``span````> <````dynamicField``name =“* _ s”type =“string”indexed =“true”stored =“true”/></``span``>`|
|----|----|


|`04`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``dynamicField``name``=``"*_ss"``type``=``"strings"``indexed``=``"true"``stored``=``"true"``/></``span````> <````dynamicField``name =“* _ ss”type =“strings”indexed =“true”stored =“true”/></``span``>`|
|----|----|


|`05`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``dynamicField``name``=``"*_l"``type``=``"long"``indexed``=``"true"``stored``=``"true"``/></``span````> <````dynamicField``name =“* _ l”type =“long”indexed =“true”stored =“true”/></``span``>`|
|----|----|


|`06`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``dynamicField``name``=``"*_ls"``type``=``"longs"``indexed``=``"true"``stored``=``"true"``/></``span````> <````dynamicField``name =“* _ ls”type =“longs”indexed =“true”stored =“true”/></``span``>`|
|----|----|


|`07`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``dynamicField``name``=``"*_t"``type``=``"text_general"``indexed``=``"true"``stored``=``"true"``/></``span````> <````dynamicField``name =“* _ t”type =“text_general”indexed =“true”stored =“true”/></``span``>`|
|----|----|


|`08`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``dynamicField``name``=``"*_txt"``type``=``"text_general"``indexed``=``"true"``stored``=``"true"``/></``span````> <````dynamicField``name =“* _ txt”type =“text_general”indexed =“true”stored =“true”/></``span``>`|
|----|----|


|`09`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``dynamicField``name``=``"*_b"``type``=``"boolean"``indexed``=``"true"``stored``=``"true"``/></``span````> <````dynamicField``name =“* _ b”type =“boolean”indexed =“true”stored =“true”/></``span``>`|
|----|----|


|`10`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``dynamicField``name``=``"*_bs"``type``=``"booleans"``indexed``=``"true"``stored``=``"true"``/></``span````> <````dynamicField``name =“* _ bs”type =“booleans”indexed =“true”stored =“true”/></``span``>`|
|----|----|


|`11`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``dynamicField``name``=``"*_f"``type``=``"float"``indexed``=``"true"``stored``=``"true"``/></``span````> <````dynamicField``name =“* _ f”type =“float”indexed =“true”stored =“true”/></``span``>`|
|----|----|


|`12`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``dynamicField``name``=``"*_fs"``type``=``"floats"``indexed``=``"true"``stored``=``"true"``/></``span````> <````dynamicField``name =“* _ fs”type =“floats”indexed =“true”stored =“true”/></``span``>`|
|----|----|


|`13`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``dynamicField``name``=``"*_d"``type``=``"double"``indexed``=``"true"``stored``=``"true"``/></``span````> <````dynamicField``name =“* _ d”type =“double”indexed =“true”stored =“true”/></``span``>`|
|----|----|


|`14`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``dynamicField``name``=``"*_ds"``type``=``"doubles"``indexed``=``"true"``stored``=``"true"``/></``span````> <````dynamicField``name =“* _ ds”type =“doubles”indexed =“true”stored =“true”/></``span``>`|
|----|----|




由于我们已经修改了配置，我们必须停止和启动服务器。
为此，我们需要通过命令行从bin目录发出以下命令。

`solr stop -all`

服务器将立即停止。现在启动服务器通过命令行从bin目录发出以下命令。

`solr start`

## 4.索引数据

Apache Solr附带一个名为SimplePostTool的独立Java程序。
该程序打包到JAR中，安装在`example\exampledocs`文件夹下。

现在，我们导航到命令提示符中的`example\exampledocs`文件夹，并键入以下命令。您将看到一堆使用该工具的选项。

`java -jar post.jar -h`

使用格式一般如下

```
Usage: java [SystemProperties] -jar post.jar [-h|-] [<file|folder|url|arg> 

[<file|folder|url|arg>...]]
```


正如我们前面所说，我们将索引solr安装附带的“books.csv”文件中的数据。
我们将在命令提示符中导航到`solr-5.0.0\example\exampledocs`并发出以下命令。

`java -Dtype=text/csv -Durl=http://localhost:8983/solr/jcg/update -jar post.jar books.csv`

这里使用的SystemProperties是：
- -Dtype - 数据文件的类型。
- -Durl - jcg核心的URL。

文件“books.csv”现在将被索引，并且命令提示符将显示以下输出。



|`1`|`<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>SimplePostTool version``5.0``.``0``</span> SimplePostTool版本``5.0``.``0``</span>`|
|----|----|


|`2`|`<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>Posting files to [base] url http:``//localhost:8983/solr/jcg/update using content-</span>将文件发布到[base] url http：// localhost：8983 / solr / jcg / update using content-</span>`|
|----|----|


|`3`|`<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>type text/csv...</span>键入text / csv ...</span>`|
|----|----|


|`4`|`<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>POSTing file books.csv to [base]</span>将文件books.csv发布到[base]</span>`|
|----|----|


|`5`|`<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>``1````files indexed.</span>````1``个文件已索引。</span>`|
|----|----|


|`6`|`<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>COMMITting Solr index changes to http:``//localhost:8983/solr/jcg/update...</span>提交Solr索引更改为http：// localhost：8983 / solr / jcg / update ...</span>`|
|----|----|


|`7`|`<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>Time spent:``0``:``00``:``00.647``</span>所用时间：``0``：``00``：``00.647``</span>`|
|----|----|




现在我们导航到以下URL并选择核心jcg。

`http://localhost:8983/solr`

![Solr JCG核心数据](https://examples.javacodegeeks.com/wp-content/uploads/2015/04/solr_jcg_core_data.jpg)
Solr JCG核心数据


仔细看看统计部分，Num Docs参数将显示索引的行数。

## 5.访问索引的文档

Apache Solr提供了基于REST的API来访问数据，并且还提供了不同的参数来检索数据。我们将向您展示几个基于场景的查询。

### 5.1按名称搜索

我们将通过它的名字检索书的细节。为此，我们将使用以下语法。URL中的参数“q”是查询事件。

在浏览器中打开以下URL。

`http://localhost:8983/solr/jcg/select?q=name:"A Clash of Kings"`

输出将如下所示。

![solr_output1](https://examples.javacodegeeks.com/wp-content/uploads/2015/04/solr_output1.jpg)
Solr按名称


### 5.2按起始字母搜索

现在，我们将告诉你如果我们只知道起始字母或词，并且不记得完整的标题，如何搜索记录。
我们可以使用以下查询来检索结果。

`http://localhost:8983/solr/jcg/select?q=name:"A"`

输出将列出所有用字母A盯着的书。

![Solr开头字母](https://examples.javacodegeeks.com/wp-content/uploads/2015/04/solr_starting_letter.jpg)
Solr开头字母


### 5.3使用通配符搜索

Solr支持通配符搜索。我们将在以下查询中显示如何检索名称中包含单词“of”的所有书籍。

`http://localhost:8983/solr/jcg/select?q=name:"*of"`

输出将列出所有带有“of”字样的书。

![Solr通配符搜索](https://examples.javacodegeeks.com/wp-content/uploads/2015/04/solr_wildcard_search.jpg)
Solr通配符搜索


### 5.4使用条件搜索

Solr支持条件搜索。它提供了“fq”参数，使用它可以为我们的查询设置条件。我们将向您展示如何在以下查询中查找价格低于$ 6的图书。

`http://localhost:8983/solr/jcg/select?q=*&fq=price:[0 TO 6]`

输出将仅列出少于6美元的图书。

![solr_output4](https://examples.javacodegeeks.com/wp-content/uploads/2015/04/solr_output4.jpg)
Solr搜索条件


## 6. Solr客户端API

有不同的客户端API可用于连接到Solr服务器。
我们列出了一些广泛使用的Solr客户端API。
- SolRuby - 从Ruby连接
- SolPHP - 从PHP连接
- PySolr - 从Python连接
- SolPerl - 从Perl连接
- SolrJ - 从Java连接
- SolrSharp - 从C＃

此外，Solr提供了基于REST的API，可以使用JavaScript直接使用。

## 7.下载模式文件

这是一个Apache Solr的教程为初学者。

**下载**
您可以在这里下载模式文件： [**Solr模式文件**](http://examples.javacodegeeks.com/wp-content/uploads/2015/04/schema.zip)



