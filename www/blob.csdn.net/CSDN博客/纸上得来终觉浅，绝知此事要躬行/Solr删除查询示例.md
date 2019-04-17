# Solr删除查询示例 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年02月12日 11:12:25[boonya](https://me.csdn.net/boonya)阅读数：1989










本文英文版地址：[https://examples.javacodegeeks.com/enterprise-java/apache-solr/solr-delete-query-example/](https://examples.javacodegeeks.com/enterprise-java/apache-solr/solr-delete-query-example/)


作者姓名：Veeramani
 Kalyanasundaram


作者简介：Veera是在电信领域工作的软件架构师，拥有丰富的Java中间件技术经验。他是一名OOAD从业者，对性能工程感兴趣。
翻译备注：本文采用谷歌翻译，请有时间的同学可以从英文版学习。

在Solr删除查询示例的示例中，我们将讨论Solr中可用的各种选项，以删除索引文档。我们还将讨论Solr中提供的不同客户端选项。为了演示删除查询用法，我们将使用基本配置在Solr中创建一个核心，并索引与Solr安装一起提供的示例文件。

这个例子的首选环境是solr-5.0.0。在开始Solr安装之前，请确保已安装JDK并已正确设置Java_Home。我们还将使用Eclipse和Maven来使用SolrJ构建一个客户端。


## 1.安装Apache Solr

首先，让我们从以下位置下载最新版本的Apache Solr：

`http://lucene.apache.org/solr/downloads.html`

Apache Solr已经经历了从4.xx到5.0.0的各种更改，因此如果您有不同版本的Solr，则需要下载5.xx版本以遵循此示例。

下载Solr zip文件后，将其解压缩到文件夹中。
提取的文件夹将如下所示：

![solr_folder](https://examples.javacodegeeks.com/wp-content/uploads/2015/04/solr_folder1.jpg)
Solr文件夹


`bin`文件夹包含用于启动和停止服务器的脚本。`example`文件夹包含几个示例文件。我们将使用其中一个来演示Solr如何对数据进行索引。`server`文件夹包含用于写入所有Solr日志的`logs`文件夹。在索引期间检查日志中的任何错误将有所帮助。服务器下的`solr`文件夹包含不同的集合或核心。
每个核/集合的配置和数据存储在相应的核/集合文件夹中。

Apache Solr附带了一个内置的Jetty服务器。
但在我们启动solr实例之前，我们必须验证JAVA_HOME是否在机器上设置。

我们可以使用命令行脚本启动服务器。让我们从命令提示符转到bin目录并发出以下命令：

`solr start`

这将在默认端口8983下启动Solr服务器。

我们现在可以在浏览器中打开以下URL，并验证我们的Solr实例正在运行。
solr管理工具的具体细节超出了示例的范围。

`http://localhost:8983/solr/`

![Solr管理控制台](https://examples.javacodegeeks.com/wp-content/uploads/2015/04/solr_admin_console1.jpg)
Solr管理控制台


## 2.配置Apache Solr

在本节中，我们将向您展示如何配置solr实例的核心/集合以及如何定义字段。
Apache Solr附带一个名为Schemaless模式的选项。此选项允许用户构建有效的模式，而无需手动编辑模式文件。但是对于这个例子，我们将使用Schema配置来理解Solr的内部。

### 2.1创建核心

当Solr服务器在独立模式下启动时，配置称为核心，当在SolrCloud模式下启动时，配置称为集合。在这个例子中，我们将讨论独立的服务器和核心。我们将暂停SolrCloud讨论以供日后使用。

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

现在在命令窗口中导航`solr-5.0.0\bin`文件夹，并发出以下命令：

`solr create -c jcg -d basic_configs`

我们可以在命令窗口中看到以下输出。



|`1`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>Creating new core``'jcg'``using ``command``:</span>使用命令创建新核心``'jcg'``：</span>`|
|----|----|


|`2`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"````>[http://localhost:8983/solr/admin/cores?action=CREATE&name=jcg&instanceDir=jcg</span](http://localhost:8983/solr/admin/cores?action=CREATE&name=jcg&instanceDir=jcg%3C/span)> http：// localhost：8983 / solr / admin / cores？action = CREATE＆name = jcg＆instanceDir = jcg</span>```|
|----|----|


|`3`||
|----|----|


|`4`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>{</span> {</span>`|
|----|----|


|`5`|` ``<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>``"responseHeader"``:{</span> “responseHeader”：{</span>`|
|----|----|


|`6`|` ``<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>``"status"``:0,</span> “status”：0，</span>`|
|----|----|


|`7`|` ``<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>``"QTime"``:663},</span> “QTime”：663}，</span>`|
|----|----|


|`8`|` ``<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>``"core"``:``"jcg"````}</span> “core”：“jcg”}</span>```|
|----|----|




现在我们导航到以下URL，我们可以看到jcg core被填充在核心选择器中。
您还可以查看核心的统计信息。

`http://localhost:8983/solr`

![Solr jcg core](https://examples.javacodegeeks.com/wp-content/uploads/2015/04/solr_jcg_core.jpg)
Solr jcg core


### 2.2修改schema.xml文件

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

由于我们已经修改了配置，我们必须停止和启动服务器。
为此，我们需要通过命令行从bin目录发出以下命令：

`solr stop -all`

服务器将立即停止。现在启动服务器通过命令行从bin目录发出以下命令：

`solr start`

## 3.索引数据

Apache Solr附带一个名为SimplePostTool的独立Java程序。
该程序打包到JAR中，安装在`example\exampledocs`文件夹下。

现在，我们导航到命令提示符中的`example\exampledocs`文件夹，并键入以下命令。您将看到一堆使用该工具的选项。

`java -jar post.jar -h`

使用格式一般如下：


```
Usage: java [SystemProperties] -jar post.jar [-h|-] [<file|folder|url|arg>

[<file|folder|url|arg>...]]
```


正如我们前面所说，我们将索引solr安装附带的“books.csv”文件中的数据。
我们将在命令提示符中导航到`solr-5.0.0\example\exampledocs`并发出以下命令。

`java -Dtype=text/csv -Durl=http://localhost:8983/solr/jcg/update -jar post.jar books.csv`

这里使用的SystemProperties是：
- **-Dtype** - 数据文件的类型。
- **-Durl** - jcg核心的URL。

文件“books.csv”现在将被索引，并且命令提示符将显示以下输出。



|`1`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>SimplePostTool version 5.0.0</span> SimplePostTool版本5.0.0</span>`|
|----|----|


|`2`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"````>Posting files to [base] url[http://localhost:8983/solr/jcg/update](http://localhost:8983/solr/jcg/update) using content-</span>将文件发布到[base] url http：// localhost：8983 / solr / jcg / update using content-</span>```|
|----|----|


|`3`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>``type````text/csv...</span>键入text / csv ...</span>```|
|----|----|


|`4`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>POSTing``file``books.csv to [base]</span>将文件books.csv发布到[base]</span>`|
|----|----|


|`5`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>1 files indexed.</span> 1个文件已索引。</span>`|
|----|----|


|`6`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"````>COMMITting Solr index changes to[http://localhost:8983/solr/jcg/update...</span](http://localhost:8983/solr/jcg/update...%3C/span)>提交Solr索引更改为http：// localhost：8983 / solr / jcg / update ...</span>```|
|----|----|


|`7`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>Time spent: 0:00:00.647</span>所用时间：0：00：00.647</span>`|
|----|----|




## 4.使用SimplePostTool

有两种方法从索引中删除文档。
- **“按ID删除”** - 删除具有指定ID的文档，并且只有在模式中定义了唯一ID字段时才能使用。
- **“按查询删除”** - 删除与指定查询匹配的所有文档。

单个删除消息还可以包含多个删除操作，并且可以具有“按ID删除”和“按查询删除”的组合。
在本节中，我们将向您展示如何使用与Solr一起提供的SimplePostTool来执行删除操作。

### 4.1按ID删除

现在，在命令提示符中导航到路径`solr-5.0.0\example\exampledocs` ，并发出以下命令：

`java -Ddata=args -Dcommit=true -Durl=http://localhost:8983/solr/jcg/update -jar post.jar "<delete><id>0553573403</id></delete>"`

注意，我们已将`commit`参数设置为true。
如果我们将值设置为false，那么我们必须再次显式地执行提交。打开以下URL并记下记录数。记录的总数将减少一个。

`http://localhost:8983/solr/jcg/select?q=*`

![solr_delete_post_id](https://examples.javacodegeeks.com/wp-content/uploads/2015/09/solr_delete_post_id.jpg)
SimplePostTool - 按ID


### 4.2按查询删除

现在，在命令提示符中导航到路径`solr-5.0.0\example\exampledocs` ，并发出以下命令：

`java -Ddata=args -Dcommit=true -Durl=http://localhost:8983/solr/jcg/update -jar post.jar "<delete><query>name:Clash</query></delete>"`

在这里，我们使用查询而不是id。此查询将删除给定条件的所有匹配记录。在我们的例子中，我们只有一个记录匹配名称`Clash` 。打开以下URL并记下记录数。记录的总数将进一步减少1。

`http://localhost:8983/solr/jcg/select?q=*`

![solr_delete_post_query](https://examples.javacodegeeks.com/wp-content/uploads/2015/09/solr_delete_post_query.jpg)
SimplePostTool - 按查询


## 5.使用SolrJ

在本节中，我们将讨论如何使用SolrJ客户端对文档执行删除操作。
让我们创建一个名为`solrdelete`的maven项目， `solrdelete`包含archetype *maven* - *archetype* - *quickstart* 。
我们将为SolrJ的pom.xml添加以下依赖关系。

*pom.xml*



|`01`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``dependency``></``span``> <``dependency``></``span``>`|
|----|----|


|`02`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``groupId``>org.apache.solr</``groupId``></``span````> <````groupId``> org.apache.solr </``groupId``></``span``>`|
|----|----|


|`03`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``artifactId``>solr-solrj</``artifactId``></``span````> <````artifactId``> solr-solrj </``artifactId``></``span``>`|
|----|----|


|`04`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``version``>5.3.0</``version``></``span``> <``version````> 5.3.0 </````version``></``span``>`|
|----|----|


|`05`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````></``dependency``></``span````> </````dependency``></``span``>`|
|----|----|


|`06`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``dependency``></``span``> <``dependency``></``span``>`|
|----|----|


|`07`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``groupId``>commons-logging</``groupId``></``span````> <````groupId``> commons-logging </``groupId``></``span``>`|
|----|----|


|`08`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``artifactId``>commons-logging</``artifactId``></``span````> <````artifactId``> commons-logging </``artifactId``></``span``>`|
|----|----|


|`09`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``version``>1.2</``version``></``span``> <``version````> 1.2 </````version``></``span``>`|
|----|----|


|`10`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````></``dependency``></``span````> </````dependency``></``span``>`|
|----|----|




### 5.1按ID删除

SolrClient提供了两种方法`deleteById()`和`deleteByQuery()`从索引中删除文档。让我们在包中创建一个名为`SolrDelete`的类`com.javacodegeeks.snippets.enterprise.solrdelete`

代码很简单，我们创建了一个`HttpSolrClient`并使用`deleteById`方法来执行删除操作，基于作为参数传递给函数的ID。

*SolrDelete.java*



|`01`|`<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>``package````com.javacodegeeks.snippets.enterprise.solrdelete;</span>````package``com.javacodegeeks.snippets.enterprise.solrdelete;</span>`|
|----|----|


|`02`|`<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>``import````java.io.IOException;</span>````import``java.io.IOException;</span>`|
|----|----|


|`03`|`<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>``import````org.apache.solr.client.solrj.SolrClient;</span>````import``org.apache.solr.client.solrj.SolrClient;</span>`|
|----|----|


|`04`|`<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>``import````org.apache.solr.client.solrj.SolrServerException;</span>````import``org.apache.solr.client.solrj.SolrServerException;</span>`|
|----|----|


|`05`|`<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>``import````org.apache.solr.client.solrj.impl.HttpSolrClient;</span>````import``org.apache.solr.client.solrj.impl.HttpSolrClient;</span>`|
|----|----|


|`06`|`<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>``public``class``SolrDelete {</span> ``public``class``SolrDelete {</span>`|
|----|----|


|`07`|`<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>``public``static``void````main(String args[]) {</span>````public``static````void````main（String args []）{</span>`|
|----|----|


|`08`|` ``<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>String urlString =``"[http://localhost:8983/solr/jcg](http://localhost:8983/solr/jcg)"````;</span> String urlString =“http：// localhost：````8983``/ solr / jcg”;</span>`|
|----|----|


|`09`|` ``<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>SolrClient solr =``new``HttpSolrClient(urlString);</span> SolrClient solr =``new``HttpSolrClient（urlString）;</span>`|
|----|----|


|`10`|` ``<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>``try``{</span>尝试{</span>`|
|----|----|


|`11`|` ``<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>solr.deleteById(``"0553293354"``);</span> solr.deleteById（“``0553293354``”）;</span>`|
|----|----|


|`12`|` ``<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>solr.commit();</span> solr.commit（）;</span>`|
|----|----|


|`13`|` ``<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>}``catch``(SolrServerException e) {</span> }``catch``（SolrServerException e）{</span>`|
|----|----|


|`14`|` ``<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>``// TODO Auto-generated catch block</span> // TODO自动生成的catch块</span>`|
|----|----|


|`15`|` ``<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>e.printStackTrace();</span> e.printStackTrace（）</span>`|
|----|----|


|`16`|` ``<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>}``catch``(IOException e) {</span> }``catch``（IOException e）{</span>`|
|----|----|


|`17`|` ``<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>``// TODO Auto-generated catch block</span> // TODO自动生成的catch块</span>`|
|----|----|


|`18`|` ``<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>e.printStackTrace();</span> e.printStackTrace（）</span>`|
|----|----|


|`19`|` ``<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>}</span> }}</span>`|
|----|----|


|`20`|` ``<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>}</span> }}</span>`|
|----|----|


|`21`|`<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>}</span> }}</span>`|
|----|----|




现在运行类作为Java应用程序。打开以下URL并记下记录数。记录的总数将进一步减少1。

`http://localhost:8983/solr/jcg/select?q=*`

![solr_delete_solrj_id](https://examples.javacodegeeks.com/wp-content/uploads/2015/09/solr_delete_solrj_id.jpg)
SolrJ - 按Id


### 5.2按查询删除

我们将使用`deleteByQuery()`方法删除与查询匹配的文档。我们将修改上面的类如下所示。查询`name:Black`将删除具有匹配名称Black的所有文档。

*SolrDelete.java*



|`01`|`<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>``package````com.javacodegeeks.snippets.enterprise.solrdelete;</span>````package``com.javacodegeeks.snippets.enterprise.solrdelete;</span>`|
|----|----|


|`02`|`<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>``import````java.io.IOException;</span>````import``java.io.IOException;</span>`|
|----|----|


|`03`|`<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>``import````org.apache.solr.client.solrj.SolrClient;</span>````import``org.apache.solr.client.solrj.SolrClient;</span>`|
|----|----|


|`04`|`<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>``import````org.apache.solr.client.solrj.SolrServerException;</span>````import``org.apache.solr.client.solrj.SolrServerException;</span>`|
|----|----|


|`05`|`<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>``import````org.apache.solr.client.solrj.impl.HttpSolrClient;</span>````import``org.apache.solr.client.solrj.impl.HttpSolrClient</span>`|
|----|----|


|`06`|`<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>``public``class``SolrDelete {</span> ``public``class``SolrDelete {</span>`|
|----|----|


|`07`|`<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>``public``static``void````main(String args[]) {</span>````public``static````void````main（String args []）{</span>`|
|----|----|


|`08`|`<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>String urlString =``"[http://localhost:8983/solr/jcg](http://localhost:8983/solr/jcg)"````;</span>字符串urlString =“http：// localhost：````8983``/ solr / jcg”;</span>`|
|----|----|


|`09`|`<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>SolrClient solr =``new``HttpSolrClient(urlString);</span> SolrClient solr =``new``HttpSolrClient（urlString）;</span>`|
|----|----|


|`10`|`<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>``try``{</span>尝试{</span>`|
|----|----|


|`11`|`<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>solr.deleteByQuery(``"name:Black"``);</span> solr.deleteByQuery（“name：Black”）;</span>`|
|----|----|


|`12`|`<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>solr.commit();</span> solr.commit（）;</span>`|
|----|----|


|`13`|`<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>}``catch``(SolrServerException e) {</span> }``catch``（SolrServerException e）{</span>`|
|----|----|


|`14`|`<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>``// TODO Auto-generated catch block</span> // TODO自动生成的catch块</span>`|
|----|----|


|`15`|`<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>e.printStackTrace();</span> e.printStackTrace（）</span>`|
|----|----|


|`16`|`<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>}``catch``(IOException e) {</span> }``catch``（IOException e）{</span>`|
|----|----|


|`17`|`<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>``// TODO Auto-generated catch block</span> // TODO自动生成的catch块</span>`|
|----|----|


|`18`|`<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>e.printStackTrace();</span> e.printStackTrace（）</span>`|
|----|----|


|`19`|`<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>}</span> }}</span>`|
|----|----|


|`20`|`<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>}</span> }}</span>`|
|----|----|


|`21`|`<span ``class``=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"``><span``class``=``"google-src-text"``style=````"direction: ltr; text-align: left"````>}</span> }}</span>`|
|----|----|




现在运行类作为Java应用程序。打开以下URL并记下记录数。在这种情况下，记录的总数将进一步减少两个，因为我们在索引记录中有两个Black实例。

`http://localhost:8983/solr/jcg/select?q=*`

![solr_delete_solrj_query](https://examples.javacodegeeks.com/wp-content/uploads/2015/09/solr_delete_solrj_query.jpg)
SolrJ - 按查询


## 6.下载源代码

这是一个solr删除查询的示例。

**下载**
您可以在这里下载此示例的完整源代码： **[solrdelete](http://examples.javacodegeeks.com/wp-content/uploads/2015/09/solrdelete.zip)**并在此下载配置**[模式](http://examples.javacodegeeks.com/wp-content/uploads/2015/09/schema.zip)**



