# Solr自动完成示例 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年02月12日 10:51:13[boonya](https://me.csdn.net/boonya)阅读数：750










本文英文版地址：[https://examples.javacodegeeks.com/enterprise-java/apache-solr/solr-autocomplete-example/](https://examples.javacodegeeks.com/enterprise-java/apache-solr/solr-autocomplete-example/)


作者姓名：Veeramani
 Kalyanasundaram


作者简介：Veera是在电信领域工作的软件架构师，拥有丰富的Java中间件技术经验。他是一名OOAD从业者，对性能工程感兴趣。
翻译备注：本文采用谷歌翻译，请有时间的同学可以从英文版学习。

在这个Solr自动完成示例的示例中，我们将讨论如何为任何UI组件实现自动完成功能。我们将使用jQuery自动完成功能以及Solr索引数据来实现自动完成功能。

我们对这个例子的首选环境是solr-5.0.0，Eclipse Luna，JDK 8u25和Tomcat 8应用服务器。话虽如此，我们也测试了JDK 1.7和Tomcat 7的代码。

在您开始安装Solr的确保你已经安装了JDK和`Java_Home`设置为合适。


## 1.安装Apache Solr

首先，从以下位置下载最新版本的Apache Solr。

`http://lucene.apache.org/solr/downloads.html`

截至本文，稳定版本可用是5.0.0。
Apache Solr已经经历了从4.xx到5.0.0的各种更改，因此如果您有不同版本的Solr，则需要下载5.xx版本以遵循此示例。

下载Solr zip文件后，将其解压缩到文件夹中。
提取的文件夹将如下所示。

![solr_folder](https://examples.javacodegeeks.com/wp-content/uploads/2015/04/solr_folder1.jpg)
Solr文件夹


该`bin`文件夹中包含的脚本来启动和停止服务器。
该`example`文件夹包含几个示例文件。我们将使用其中一个来演示Solr如何对数据进行索引。该`server`文件夹包含`logs` ，所有的Solr的日志写入文件夹。在索引期间检查日志中的任何错误将是有帮助的。该`solr`下服务器文件夹包含不同的集合或核心。
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


## 2.配置Apache Solr

在本节中，我们将向您展示如何配置solr实例的核心/集合以及如何定义字段。
Apache Solr附带一个名为Schemaless模式的选项。此选项允许用户构建有效的模式，而无需手动编辑模式文件。但是对于这个例子，我们将使用Schema配置来理解Solr的内部。

### 2.1创建核心

当Solr服务器在独立模式下启动时，配置称为核心，当它在SolrCloud模式下启动时，配置称为集合。在这个例子中，我们将讨论独立的服务器和核心。我们将暂停SolrCloud讨论以供日后使用。

首先，我们需要创建一个用于索引数据的Core。
Solr create命令有以下选项：
- **-c <名称>** -芯或集合的名称来创建（必需）。
- **-d <confdir>** -配置目录，在SolrCloud模式是有用的。
- **-n <CONFIGNAME>** -配置名称。
此默认值与核心或集合的名称相同。
- **-p <端口>** -本地Solr的实例的端口发送创建命令;默认情况下，脚本尝试通过查找正在运行的Solr实例来检测端口。
- **-s <碎片>** -碎片的数量成，缺省值为1分割的集合。
- **-rf <复制品>** -集合中的每个文件的副本的数量。
默认值为1。

在本例中，我们将使用-c参数作为核心名称，使用-d参数作为配置目录。
对于所有其他参数，我们使用默认设置。

现在浏览`solr-5.0.0\bin`在命令窗口中的文件夹，并发出以下命令。

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

我们需要修改`schema.xml`文件的文件夹下， `server\solr\jcg\conf`包含的字段。我们将使用随同Solr安装一起提供的示例文件“books.csv”中的一个进行索引。该文件所在的文件夹下`solr-5.0.0\example\exampledocs`

现在，我们定位到该文件夹`server\solr`目录。
你会看到一个文件夹，名为`jcg`创建。子文件夹即`conf`和`data`分别拥有核心的配置和索引数据。

现在编辑`schema.xml`在文件`\server\solr\jcg\conf`文件夹和唯一键元素后添加以下内容。

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


|`6`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``field``name``=``"inStock"``type``=``"boolean"``indexed``=``"true"``stored``=``"true"``/></``span````> <````field``name =“inStock”type =“boolean”indexed =“true”saved =“true”/></``span``>`|
|----|----|


|`7`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``field``name``=``"author"``type``=``"text_general"``indexed``=``"true"``stored``=``"true"``/></``span````> <````field``name =“author”type =“text_general”indexed =“true”stored =“true”/></``span``>`|
|----|----|




我们已经设置的属性`indexed`为true。
这指定了用于索引的字段，并且可以使用索引检索记录。将值设置为false将使该字段只存储，但不能查询。

另外请注意，我们叫另一个属性`stored` ，并将其设置为true。这指定字段存储并可以在输出中返回。将此字段设置为false将使该字段仅编入索引，并且无法在输出中检索。

我们已经为“books.csv”文件中的字段分配了类型。
CSV文件“id”中的第一个字段由schema.xml文件的uniqueKey元素自动处理，用于建立索引。

由于我们已经修改了配置，我们必须停止和启动服务器。
为此，我们需要通过命令行从bin目录发出以下命令。

`solr stop -all`

服务器将立即停止。现在启动服务器通过命令行从bin目录发出以下命令。

`solr start`

## 3.索引数据

Apache Solr附带一个名为SimplePostTool的独立Java程序。
该程序打包成JAR，并提供与文件夹下的安装`example\exampledocs` 。

现在，我们定位到`example\exampledocs`文件夹在命令提示符处键入以下命令。您将看到一堆使用该工具的选项。

`java -jar post.jar -h`

使用格式一般如下

```
Usage: java [SystemProperties] -jar post.jar [-h|-] [<file|folder|url|arg> 

[<file|folder|url|arg>...]]
```

正如我们前面所说，我们将索引solr安装附带的“books.csv”文件中的数据。
我们将导航到`solr-5.0.0\example\exampledocs`在命令提示符并发出以下命令。

`java -Dtype=text/csv -Durl=http://localhost:8983/solr/jcg/update -jar post.jar books.csv`

这里使用的SystemProperties是：
- -Dtype - 数据文件的类型。
- -Durl - jcg核心的URL。

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




## 4.设置Web项目

我们将使用jQuery自动完成小部件来消费来自Solr的数据。
首先，我们将为一个简单的Web应用程序设置maven项目。

在eclipse去文件 - >新建 - >其他 - > Maven项目。

![solrauto_maven1](https://examples.javacodegeeks.com/wp-content/uploads/2015/05/solrauto_maven1.jpg)
Maven - 步骤1


在向导的“选择项目名称和位置”页中，确保“创建一个简单的项目（跳过原型选择）”选项**未被选中** ，点击“下一步”继续使用默认值。

![solrauto_maven2](https://examples.javacodegeeks.com/wp-content/uploads/2015/05/solrauto_maven2.jpg)
Maven - 步骤2


在这里选择“maven-archetype-webapp”，然后单击下一步。

![solrauto_maven_webapp](https://examples.javacodegeeks.com/wp-content/uploads/2015/05/solrauto_maven_webapp.jpg)
Maven - 步骤3


在向导的“输入工件ID”页面中，您可以定义项目的名称和主包。
设置“组ID”变量`"com.javacodegeeks.snippets.enterprise"`和“工件ID”变量`"solrautocomplete"` 。对于包进入`"com.javacodegreeks.solrautocomplete"` ，然后点击“完成”退出向导并创建项目。

![solrauto_maven4_1](https://examples.javacodegeeks.com/wp-content/uploads/2015/05/solrauto_maven4_1.jpg)
Maven - 步骤4


如果在index.jsp中看到任何错误，请为项目设置目标运行时。

![solrauto_maven_target](https://examples.javacodegeeks.com/wp-content/uploads/2015/05/solrauto_maven_target.jpg)
Maven - 步骤5


现在创建一个名为`search.html`在WebApp文件夹。
我们使用托管在云上的jQuery。我们将使用jQuery AJAX从Solr获取数据并绑定到自动完成功能的源。

*search.html*



|`01`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><!DOCTYPE html></``span``> <！DOCTYPE html></``span``>`|
|----|----|


|`02`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``html``></``span``> <``html``></``span``>`|
|----|----|


|`03`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``head``></``span``> <``head``></``span``>`|
|----|----|


|`04`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``meta``charset``=``"ISO-8859-1"``></``span````> <````meta``charset =“ISO-8859-1”></``span``>`|
|----|----|


|`05`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``title``>Solr auto complete</``title``></``span````> <````title``> Solr自动完成</ ``title``></``span``>`|
|----|----|


|`06`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``link``</span> <``link``</span>`|
|----|----|


|`07`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>href="[http://code.jquery.com/ui/1.10.4/themes/ui-lightness/jquery-ui.css](http://code.jquery.com/ui/1.10.4/themes/ui-lightness/jquery-ui.css)"</``span````> href =“[http://code.jquery.com/ui/1.10.4/themes/ui-lightness/jquery-ui.css](http://code.jquery.com/ui/1.10.4/themes/ui-lightness/jquery-ui.css)”</````span``>`|
|----|----|


|`08`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>rel="stylesheet"></``link``></``span````> rel =“stylesheet”> </````link``></``span``>`|
|----|----|


|`09`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``script``src``=``"[http://code.jquery.com/jquery-1.10.2.js](http://code.jquery.com/jquery-1.10.2.js)"``></``script``></``span````> <````script``src =“[http://code.jquery.com/jquery-1.10.2.js](http://code.jquery.com/jquery-1.10.2.js)”> </``script``></``span``>`|
|----|----|


|`10`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``script``src``=``"[http://code.jquery.com/ui/1.10.4/jquery-ui.js](http://code.jquery.com/ui/1.10.4/jquery-ui.js)"``></``script``></``span````> <````script``src =“[http://code.jquery.com/ui/1.10.4/jquery-ui.js](http://code.jquery.com/ui/1.10.4/jquery-ui.js)”> </``script``></``span``>`|
|----|----|


|`11`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``script``></``span``> <``script``></``span``>`|
|----|----|


|`12`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>$(function() {</``span``> $（function（）{</``span``>`|
|----|----|


|`13`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>var URL_PREFIX = "[http://localhost:8983/solr/jcg/select?q=name:](http://localhost:8983/solr/jcg/select?q=name:)";</``span````> var URL_PREFIX =“http：// localhost：8983 / solr / jcg / select？q = name：”;</````span``>`|
|----|----|


|`14`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>var URL_SUFFIX = "&wt=json";</``span``> var URL_SUFFIX =“＆wt = json”;</``span``>`|
|----|----|


|`15`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>$("#searchBox").autocomplete({</``span``> $（“＃searchBox”）。autocomplete（{</``span``>`|
|----|----|


|`16`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>source : function(request, response) {</``span``> source：function（request，response）{</``span``>`|
|----|----|


|`17`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>var URL = URL_PREFIX + $("#searchBox").val() + URL_SUFFIX;</``span``> var URL = URL_PREFIX + $（“＃searchBox”）。val（）+ URL_SUFFIX;</``span``>`|
|----|----|


|`18`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>$.ajax({</``span``> $ .ajax（{</``span``>`|
|----|----|


|`19`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>url : URL,</``span``> url：URL，</``span``>`|
|----|----|


|`20`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>success : function(data) {</``span``> success：function（data）{</``span``>`|
|----|----|


|`21`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>var docs = JSON.stringify(data.response.docs);</``span``> var docs = JSON.stringify（data.response.docs）;</``span``>`|
|----|----|


|`22`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>var jsonData = JSON.parse(docs);</``span``> var jsonData = JSON.parse（docs）;</``span``>`|
|----|----|


|`23`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>response($.map(jsonData, function(value, key) {</``span``> response（$ .map（jsonData，function（value，key）{</``span``>`|
|----|----|


|`24`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>return {</``span``> return {</``span``>`|
|----|----|


|`25`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>label : value.name</``span``> label：value.name</``span``>`|
|----|----|


|`26`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>}</``span``> }}</``span``>`|
|----|----|


|`27`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>}));</``span``> }））;</``span``>`|
|----|----|


|`28`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>},</``span``> }，</``span``>`|
|----|----|


|`29`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>dataType : 'jsonp',</``span``> dataType：'jsonp'，</``span``>`|
|----|----|


|`30`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>jsonp : 'json.wrf'</``span``> jsonp：'json.wrf'</``span``>`|
|----|----|


|`31`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>});</``span``> }）;</``span``>`|
|----|----|


|`32`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>},</``span``> }，</``span``>`|
|----|----|


|`33`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>minLength : 1</``span``> minLength：1</``span``>`|
|----|----|


|`34`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>})</``span``> }）</``span``>`|
|----|----|


|`35`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>});</``span``> }）;</``span``>`|
|----|----|


|`36`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````></``script``></``span````> </````script``></``span``>`|
|----|----|


|`37`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````></``head``></``span````> </````head``></``span``>`|
|----|----|


|`38`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``body``></``span``> <``body``></``span``>`|
|----|----|


|`39`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``div``></``span``> <``div``></``span``>`|
|----|----|


|`40`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``p``>Type The or A</``p``></``span``> <``p````>键入或A </````p``></``span``>`|
|----|----|


|`41`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``label``for``=``"searchBox"``>Tags: </``label````> <````input``id``=``"searchBox"``></``input``></``span````> <````label````for =“searchBox”>标签：</````label``> <``input````id =“searchBox”> </````input``></``span``>`|
|----|----|


|`42`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````></``div``></``span````> </````div``></``span``>`|
|----|----|


|`43`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````></``body``></``span````> </````body``></``span``>`|
|----|----|


|`44`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````></``html``></``span````> </````html``></``span``>`|
|----|----|




由于Solr在不同的端口上运行，请求（网页）从另一个端口启动，因此我们可能会遇到跨域问题。为了克服这个问题，我们必须使用`jsonp` 。minLength参数指定搜索要输入的字符数量。
在这里，我们将值指定为1，这意味着当单个字符键入时，结果被绑定。

现在我们可以使用Run as - > Maven clean然后Run as - > Maven install来创建部署包。这将在目标文件夹中创建一个war文件。在`war`产生的文件必须放在`webapps`的tomcat的文件夹中。现在我们可以启动服务器了。

打开以下URL并键入“A”。这将带来的书籍有标题A的结果。

`http://localhost:8080/solrautocomplete/search.html`

![solrauto_output1](https://examples.javacodegeeks.com/wp-content/uploads/2015/05/solrauto_output1.jpg)
输出1


现在在搜索框中键入“The”。这将返回有词的书。

![solrauto_output2](https://examples.javacodegeeks.com/wp-content/uploads/2015/05/solrauto_output2.jpg)
输出2


上述索引技术的问题是我们不能基于短语获得结果。
说如果我们键入“黑色”，它不会获取任何结果。此外，当我们键入'bla'没有结果绑定。为了克服这个问题，我们将使用NGramFilterFactory并重新索引数据。

## 5.使用NGramFilterFactory进行索引

我们将字段复制`name`到一个所谓的新领域`name_ngram` 。在将文档添加到索引时，copyField命令将一个字段复制到另一个字段。它用于以不同方式对同一字段建立索引，或者将多个字段添加到同一字段，以便更容易/更快速地搜索。

现在，修改`schema.xml`文件中的`\server\solr\jcg\conf`文件夹，并添加以下突出显示的内容。

*schema.xml*



|`1`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>``<!--</span> <！ - </span>`|
|----|----|


|`2`|```<span class="notranslate" onmouseover="_tipon(this)" onmouseout="_tipoff()"><span class="google-src-text" style="direction: ltr; text-align: left"><copyField source="title" dest="text"/></span> <copyField source =“title”dest =“text”/></span>```|
|----|----|


|`3`|```<span class="notranslate" onmouseover="_tipon(this)" onmouseout="_tipoff()"><span class="google-src-text" style="direction: ltr; text-align: left"><copyField source="body" dest="text"/></span> <copyField source =“body”dest =“text”/></span>```|
|----|----|


|`4`|`<span class="notranslate" onmouseover="_tipon(this)" onmouseout="_tipoff()"><span class="google-src-text" style="direction: ltr; text-align: left">-->``</``span````> - ></````span``>`|
|----|----|


|`5`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``copyField``source``=``"name"``dest``=``"name_ngram"``/></``span````> <````copyField``source =“name”dest =“name_ngram”/></``span``>`|
|----|----|




在同一文件中，我们需要添加一个字段名为`name_ngram`并将其标记索引。对于它，我们需要添加突出显示的行。

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


|`8`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``field``name``=``"name_ngram"``type``=``"text_ngram"``indexed``=``"true"``stored``=``"true"``/></``span````> <````field``name =“name_ngram”type =“text_ngram”indexed =“true”stored =“true”/></``span``>`|
|----|----|




就拿我们已经改变了新的领域作为类型的说明`text_ngram` 。我们将定义类型`text_ngram`其后。

现在我们添加了定义字段`text_ngram`中`schema.xml`文件。我们已将最小ngram大小设置为2，最大ngram大小设置为10。

*schema.xml*



|`01`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>``<!-- Added for NGram field-->``</``span``> <！ - 为NGram字段添加 - ></``span``>`|
|----|----|


|`02`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``fieldType``name``=``"text_ngram"``class``=``"solr.TextField"``positionIncrementGap``=``"100"``></``span````> <````fieldType``name =“text_ngram”class =“solr.TextField”positionIncrementGap =“100”></``span``>`|
|----|----|


|`03`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``analyzer``type``=``"index"``></``span````> <````analyzer``type =“index”></``span``>`|
|----|----|


|`04`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``tokenizer``class``=``"solr.NGramTokenizerFactory"``minGramSize``=``"2"``maxGramSize``=``"10"``/></``span````> <````tokenizer``class =“solr.NGramTokenizerFactory”minGramSize =“2”maxGramSize =“10”/></``span``>`|
|----|----|


|`05`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``filter``class``=``"solr.LowerCaseFilterFactory"``/></``span````> <````filter``class =“solr.LowerCaseFilterFactory”/></``span``>`|
|----|----|


|`06`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````></``analyzer``></``span````> </````analyze``></``span``>`|
|----|----|


|`07`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``analyzer``type``=``"query"``></``span````> <````analyzer``type =“query”></``span``>`|
|----|----|


|`08`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``tokenizer``class``=``"solr.EdgeNGramTokenizerFactory"``minGramSize``=``"2"``maxGramSize``=``"10"``/></``span````> <````tokenizer``class =“solr.EdgeNGramTokenizerFactory”minGramSize =“2”maxGramSize =“10”/></``span``>`|
|----|----|


|`09`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``filter``class``=``"solr.LowerCaseFilterFactory"``/></``span````> <````filter``class =“solr.LowerCaseFilterFactory”/></``span``>`|
|----|----|


|`10`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````></``analyzer``></``span````> </````analyze``></``span``>`|
|----|----|


|`11`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````></``fieldType``></``span````> </````fieldType``></``span``>`|
|----|----|




我们结合的特点`NGramTokenizerFactory`和`EdgeNGramTokenizerFactory`达到最佳的索引。由于我们已经修改了配置，我们必须停止和启动服务器。要做到这一点，我们需要从发出以下命令`bin`通过命令行目录。

`solr stop -all`

服务器将立即停止。现在启动服务器从bin目录通过命令行发出以下命令。

`solr start`

我们将重新索引存在于数据`books.csv`文件。
我们将导航到`solr-5.0.0\example\exampledocs`在命令提示符并发出以下命令。

`java -Dtype=text/csv -Durl=http://localhost:8983/solr/jcg/update -jar post.jar books.csv`

该文件`books.csv`现在将重新索引和命令提示将显示以下的输出。



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


|`6`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>Time spent: 0:00:02.325</span>所用时间：0：00：02.325</span>`|
|----|----|




## 6.修改search.html

现在，我们将修改`search.html`包含另一个搜索框测试NGRAM索引。我们将创建一个ID搜索框`ngrambox`和写入新的搜索框，另一个JavaScript函数。

*search.html*



|`01`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><!DOCTYPE html></``span``> <！DOCTYPE html></``span``>`|
|----|----|


|`02`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``html``></``span``> <``html``></``span``>`|
|----|----|


|`03`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``head``></``span``> <``head``></``span``>`|
|----|----|


|`04`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``meta``charset``=``"ISO-8859-1"``></``span````> <````meta``charset =“ISO-8859-1”></``span``>`|
|----|----|


|`05`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``title``>Solr auto complete</``title``></``span````> <````title``> Solr自动完成</ ``title``></``span``>`|
|----|----|


|`06`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``link``</span> <``link``</span>`|
|----|----|


|`07`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>href="[http://code.jquery.com/ui/1.10.4/themes/ui-lightness/jquery-ui.css](http://code.jquery.com/ui/1.10.4/themes/ui-lightness/jquery-ui.css)"</``span````> href =“[http://code.jquery.com/ui/1.10.4/themes/ui-lightness/jquery-ui.css](http://code.jquery.com/ui/1.10.4/themes/ui-lightness/jquery-ui.css)”</````span``>`|
|----|----|


|`08`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>rel="stylesheet"></``link``></``span````> rel =“stylesheet”> </````link``></``span``>`|
|----|----|


|`09`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``script``src``=``"[http://code.jquery.com/jquery-1.10.2.js](http://code.jquery.com/jquery-1.10.2.js)"``></``script``></``span````> <````script``src =“[http://code.jquery.com/jquery-1.10.2.js](http://code.jquery.com/jquery-1.10.2.js)”> </``script``></``span``>`|
|----|----|


|`10`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``script``src``=``"[http://code.jquery.com/ui/1.10.4/jquery-ui.js](http://code.jquery.com/ui/1.10.4/jquery-ui.js)"``></``script``></``span````> <````script``src =“[http://code.jquery.com/ui/1.10.4/jquery-ui.js](http://code.jquery.com/ui/1.10.4/jquery-ui.js)”> </``script``></``span``>`|
|----|----|


|`11`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``script``></``span``> <``script``></``span``>`|
|----|----|


|`12`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>$(function() {</``span``> $（function（）{</``span``>`|
|----|----|


|`13`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>var URL_PREFIX = "[http://localhost:8983/solr/jcg/select?q=name:](http://localhost:8983/solr/jcg/select?q=name:)";</``span````> var URL_PREFIX =“http：// localhost：8983 / solr / jcg / select？q = name：”;</````span``>`|
|----|----|


|`14`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>var URL_SUFFIX = "&wt=json";</``span``> var URL_SUFFIX =“＆wt = json”;</``span``>`|
|----|----|


|`15`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>$("#searchBox").autocomplete({</``span``> $（“＃searchBox”）。autocomplete（{</``span``>`|
|----|----|


|`16`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>source : function(request, response) {</``span``> source：function（request，response）{</``span``>`|
|----|----|


|`17`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>var URL = URL_PREFIX + $("#searchBox").val() + URL_SUFFIX;</``span``> var URL = URL_PREFIX + $（“＃searchBox”）。val（）+ URL_SUFFIX;</``span``>`|
|----|----|


|`18`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>$.ajax({</``span``> $ .ajax（{</``span``>`|
|----|----|


|`19`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>url : URL,</``span``> url：URL，</``span``>`|
|----|----|


|`20`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>success : function(data) {</``span``> success：function（data）{</``span``>`|
|----|----|


|`21`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>var docs = JSON.stringify(data.response.docs);</``span``> var docs = JSON.stringify（data.response.docs）;</``span``>`|
|----|----|


|`22`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>var jsonData = JSON.parse(docs);</``span``> var jsonData = JSON.parse（docs）;</``span``>`|
|----|----|


|`23`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>response($.map(jsonData, function(value, key) {</``span``> response（$ .map（jsonData，function（value，key）{</``span``>`|
|----|----|


|`24`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>return {</``span``> return {</``span``>`|
|----|----|


|`25`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>label : value.name</``span``> label：value.name</``span``>`|
|----|----|


|`26`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>}</``span``> }}</``span``>`|
|----|----|


|`27`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>}));</``span``> }））;</``span``>`|
|----|----|


|`28`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>},</``span``> }，</``span``>`|
|----|----|


|`29`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>dataType : 'jsonp',</``span``> dataType：'jsonp'，</``span``>`|
|----|----|


|`30`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>jsonp : 'json.wrf'</``span``> jsonp：'json.wrf'</``span``>`|
|----|----|


|`31`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>});</``span``> }）;</``span``>`|
|----|----|


|`32`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>},</``span``> }，</``span``>`|
|----|----|


|`33`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>minLength : 1</``span``> minLength：1</``span``>`|
|----|----|


|`34`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>})</``span``> }）</``span``>`|
|----|----|


|`35`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>});</``span``> }）;</``span``>`|
|----|----|


|`36`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>$(function() {</``span``> $（function（）{</``span``>`|
|----|----|


|`37`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>var URL_PREFIX = "[http://localhost:8983/solr/jcg/select?q=name:](http://localhost:8983/solr/jcg/select?q=name:)";</``span````> var URL_PREFIX =“http：// localhost：8983 / solr / jcg / select？q = name：”;</````span``>`|
|----|----|


|`38`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>var URL_MIDDLE = "OR name_ngram:";</``span``> var URL_MIDDLE =“OR name_ngram：”;</``span``>`|
|----|----|


|`39`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>var URL_SUFFIX = "&wt=json";</``span``> var URL_SUFFIX =“＆wt = json”;</``span``>`|
|----|----|


|`40`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>$("#ngramBox").autocomplete(</``span``> $（“＃ngramBox”）。autocomplete（</``span``>`|
|----|----|


|`41`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>{</``span``> {</``span``>`|
|----|----|


|`42`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>source : function(request, response) {</``span``> source：function（request，response）{</``span``>`|
|----|----|


|`43`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>var searchString = "\"" + $("#ngramBox").val() + "\"";</``span``> var searchString =“\”“+ $（”＃ngramBox“）。val（）+”\“”;</``span``>`|
|----|----|


|`44`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>var URL = URL_PREFIX + searchString + URL_MIDDLE</``span``> var URL = URL_PREFIX + searchString + URL_MIDDLE</``span``>`|
|----|----|


|`45`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>+ searchString + URL_SUFFIX;</``span``> + searchString + URL_SUFFIX;</``span``>`|
|----|----|


|`46`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>$.ajax({</``span``> $ .ajax（{</``span``>`|
|----|----|


|`47`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>url : URL,</``span``> url：URL，</``span``>`|
|----|----|


|`48`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>success : function(data) {</``span``> success：function（data）{</``span``>`|
|----|----|


|`49`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>var docs = JSON.stringify(data.response.docs);</``span``> var docs = JSON.stringify（data.response.docs）;</``span``>`|
|----|----|


|`50`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>var jsonData = JSON.parse(docs);</``span``> var jsonData = JSON.parse（docs）;</``span``>`|
|----|----|


|`51`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>response($.map(jsonData, function(value, key) {</``span``> response（$ .map（jsonData，function（value，key）{</``span``>`|
|----|----|


|`52`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>return {</``span``> return {</``span``>`|
|----|----|


|`53`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>label : value.name</``span``> label：value.name</``span``>`|
|----|----|


|`54`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>}</``span``> }}</``span``>`|
|----|----|


|`55`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>}));</``span``> }））;</``span``>`|
|----|----|


|`56`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>},</``span``> }，</``span``>`|
|----|----|


|`57`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>dataType : 'jsonp',</``span``> dataType：'jsonp'，</``span``>`|
|----|----|


|`58`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>jsonp : 'json.wrf'</``span``> jsonp：'json.wrf'</``span``>`|
|----|----|


|`59`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>});</``span``> }）;</``span``>`|
|----|----|


|`60`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>},</``span``> }，</``span``>`|
|----|----|


|`61`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>minLength : 1</``span``> minLength：1</``span``>`|
|----|----|


|`62`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>})</``span``> }）</``span``>`|
|----|----|


|`63`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>});</``span``> }）;</``span``>`|
|----|----|


|`64`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````></``script``></``span````> </````script``></``span``>`|
|----|----|


|`65`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````></``head``></``span````> </````head``></``span``>`|
|----|----|


|`66`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``body``></``span``> <``body``></``span``>`|
|----|----|


|`67`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``div``></``span``> <``div``></``span``>`|
|----|----|


|`68`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``p``>Type 'A' or 'The'</``p``></``span``> <``p````>输入“A”或“The”</````p``></``span``>`|
|----|----|


|`69`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``label``for``=``"searchBox"``>Tags: </``label````> <````input``id``=``"searchBox"``></``input``></``span````> <````label````for =“searchBox”>标签：</````label``> <``input````id =“searchBox”> </````input``></``span``>`|
|----|----|


|`70`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````></``div``></``span````> </````div``></``span``>`|
|----|----|


|`71`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``div``></``span``> <``div``></``span``>`|
|----|----|


|`72`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``p``>Type 'Th' or 'Bla' or 'The Black'</``p``></``span````> <````p``>输入“Th”或“Bla”或“Black”</``p``></``span``>`|
|----|----|


|`73`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``label``for``=``"ngramBox"``>Tags: </``label````> <````input``id``=``"ngramBox"``></``input``></``span````> <````label````for =“ngramBox”>标签：</````label``> <``input````id =“ngramBox”> </````input``></``span``>`|
|----|----|


|`74`|` ``<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````></``div``></``span````> </````div``></``span``>`|
|----|----|


|`75`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````></``body``></``span````> </````body``></``span``>`|
|----|----|


|`76`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````></``html``></``span````> </````html``></``span``>`|
|----|----|




现在，再次打包使用maven和复制`war`到的Apache Tomcat 
```
webapps
```
文件夹中。在浏览器中打开以下URL并键入'Bla'。

`http://localhost:8080/solrautocomplete/search.html`

![solrauto_output3_1](https://examples.javacodegeeks.com/wp-content/uploads/2015/05/solrauto_output3_1.jpg)
输出3


## 7.下载Eclipse Project

这是Solr自动完成的一个例子。

**下载**
你可以在这里下载这个例子的完整源代码： [**Solr的自动完成**](http://examples.javacodegeeks.com/wp-content/uploads/2015/05/solrautocomplete.zip) ，请下载模式文件： [**架构文件**](http://examples.javacodegeeks.com/wp-content/uploads/2015/05/schema.zip) 。




