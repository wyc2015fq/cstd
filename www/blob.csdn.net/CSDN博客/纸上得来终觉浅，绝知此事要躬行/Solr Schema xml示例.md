# Solr Schema.xml示例 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年02月12日 11:08:33[boonya](https://me.csdn.net/boonya)阅读数：1432










本文英文版地址：[https://examples.javacodegeeks.com/enterprise-java/apache-solr/solr-schema-xml-example/](https://examples.javacodegeeks.com/enterprise-java/apache-solr/solr-schema-xml-example/)


作者姓名：Veeramani
 Kalyanasundaram


作者简介：Veera是在电信领域工作的软件架构师，拥有丰富的Java中间件技术经验。他是一名OOAD从业者，对性能工程感兴趣。
翻译备注：本文采用谷歌翻译，请有时间的同学可以从英文版学习。

在Solr Schema.xml示例的示例中，我们将讨论Apache Solr提供的schema.xml文件中的不同配置参数。schema.xml描述了我们希望Solr索引的文档。我们可以定义字段类型和字段本身。字段类型定义很强大，包括有关Solr如何处理传入字段值和查询值的信息。我们将讨论如何创建字段，创建自定义字段类型，在此示例中使用复制字段和使用动态字段。

为了演示schema.xml文件的使用，我们将使用基本配置在Solr中创建一个核心，并索引与Solr安装一起提供的示例文件。

这个例子的首选环境是solr-5.0.0。在开始Solr安装之前，请确保已安装JDK并已正确设置Java_Home。


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


## 2.创建Solr核心

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


## 3.使用字段类型

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

## 4.定义字段类型

在创建核心时，默认情况下会创建许多字段类型。
字段类型定义了Solr应如何解释字段中的数据以及如何查询字段。我们将创建一个字段类型“ 
text_uppercase”，它将数据转换为大写字母值并对其进行索引。字段类型定义可以包括四种类型的信息：
- 实现类名称（强制）
- 字段类型的名称（强制）
- 如果字段类型为TextField，则说明字段类型的字段分析
- 字段类型属性 - 根据实现类，一些属性可能是必需的。

字段类型确定字段类型的大多数行为，但也可以定义可选属性。

现在编辑`\server\solr\jcg\conf`文件夹中的schema.xml文件，并在小写字段类型后添加以下内容。以下配置将创建一个字段类型“text_uppercase”。

*schema.xml*



|`01`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>````<!-- lowercases the entire field value, keeping it as a single token.</span> <！ - 降低整个字段值，将其保留为单个令牌。</span> <span class="notranslate" onmouseover="_tipon(this)" onmouseout="_tipoff()"><span class="google-src-text" style="direction: ltr; text-align: left">-->````</``span``> - ></``span``>`|
|----|----|


|`02`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``fieldType``name``=``"lowercase"``class``=``"solr.TextField"``positionIncrementGap``=``"100"``></``span````> <````fieldType``name =“lowercase”class =“solr.TextField”positionIncrementGap =“100”></``span``>`|
|----|----|


|`03`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``analyzer``></``span``> <``analyzer``></``span``>`|
|----|----|


|`04`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``tokenizer``class``=``"solr.KeywordTokenizerFactory"``/></``span````> <````tokenizer``class =“solr.KeywordTokenizerFactory”/></``span``>`|
|----|----|


|`05`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``filter``class``=``"solr.LowerCaseFilterFactory"``/></``span````> <````filter``class =“solr.LowerCaseFilterFactory”/></``span``>`|
|----|----|


|`06`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````></``analyzer``></``span````> </````analyze``></``span``>`|
|----|----|


|`07`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````></``fieldType``></``span````> </````fieldType``></``span``>`|
|----|----|


|`08`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``fieldType``name``=``"text_uppercase"``class``=``"solr.TextField"``positionIncrementGap``=``"100"``></``span````> <````fieldType``name =“text_uppercase”class =“solr.TextField”positionIncrementGap =“100”></``span``>`|
|----|----|


|`09`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``analyzer``></``span``> <``analyzer``></``span``>`|
|----|----|


|`10`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``tokenizer``class``=``"solr.WhitespaceTokenizerFactory"``/></``span````> <````tokenizer``class =“solr.WhitespaceTokenizerFactory”/></``span``>`|
|----|----|


|`11`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``filter``class``=``"solr.UpperCaseFilterFactory"``/></``span````> <````filter``class =“solr.UpperCaseFilterFactory”/></``span``>`|
|----|----|


|`12`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````></``analyzer``></``span````> </````analyze``></``span``>`|
|----|----|


|`13`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````></``fieldType``></``span````> </````fieldType``></``span``>`|
|----|----|




## 5.使用复制字段

复制字段命令将文档添加到索引时将一个字段复制到另一个字段。
它用于以不同方式对同一字段建立索引，或者将多个字段添加到同一字段，以便更容易/更快速地搜索。

在我们的示例中，我们将为名称创建一个复制字段。
我们将使用复制字段以大写形式对名称建立索引。

现在编辑`\server\solr\jcg\conf`文件夹中的schema.xml文件，并在小写字段类型后添加以下内容。以下配置将创建一个字段类型“text_uppercase”。

*schema.xml*



|`1`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>``<!--</span> <！ - </span>`|
|----|----|


|`2`|```<span class="notranslate" onmouseover="_tipon(this)" onmouseout="_tipoff()"><span class="google-src-text" style="direction: ltr; text-align: left"><copyField source="title" dest="text"/></span> <copyField source =“title”dest =“text”/></span>```|
|----|----|


|`3`|```<span class="notranslate" onmouseover="_tipon(this)" onmouseout="_tipoff()"><span class="google-src-text" style="direction: ltr; text-align: left"><copyField source="body" dest="text"/></span> <copyField source =“body”dest =“text”/></span>```|
|----|----|


|`4`|`<span class="notranslate" onmouseover="_tipon(this)" onmouseout="_tipoff()"><span class="google-src-text" style="direction: ltr; text-align: left">-->``</``span````> - ></````span``>`|
|----|----|


|`5`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``copyField``source``=``"name"``dest``=``"uname"``/></``span````> <````copyField``source =“name”dest =“uname”/></``span``>`|
|----|----|




现在我们必须将字段类型分配给目标。我们将字段类型添加为“text_uppercase”，如下所示。

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


|`8`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``field``name``=``"uname"``type``=``"text_uppercase"``indexed``=``"true"``stored``=``"true"``/></``span````> <````field``name =“uname”type =“text_uppercase”indexed =“true”stored =“true”/></``span``>`|
|----|----|




## 6.使用动态字段

动态字段通过在配置原理上应用约定来创建。
当我们使用基本配置创建核心时，默认情况下会创建以下动态字段。

*schema.xml*



|`01`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``dynamicField``name``=``"*_i"``type``=``"int"``indexed``=``"true"``stored``=``"true"``/></``span````> <````dynamicField``name =“* _ i”type =“int”indexed =“true”stored =“true”/></``span``>`|
|----|----|


|`02`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``dynamicField``name``=``"*_is"``type``=``"int"``indexed``=``"true"``stored``=``"true"``multiValued``=``"true"``/></``span````> <````dynamicField``name =“* _ is”type =“int”indexed =“true”stored =“true”multiValued =“true”/></``span``>`|
|----|----|


|`03`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``dynamicField``name``=``"*_s"``type``=``"string"``indexed``=``"true"``stored``=``"true"``/></``span````> <````dynamicField``name =“* _ s”type =“string”indexed =“true”stored =“true”/></``span``>`|
|----|----|


|`04`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``dynamicField``name``=``"*_ss"``type``=``"string"``indexed``=``"true"``stored``=``"true"``multiValued``=``"true"``/></``span````> <````dynamicField``name =“* _ ss”type =“string”indexed =“true”stored =“true”multiValued =“true”/></``span``>`|
|----|----|


|`05`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``dynamicField``name``=``"*_l"``type``=``"long"``indexed``=``"true"``stored``=``"true"``/></``span````> <````dynamicField``name =“* _ l”type =“long”indexed =“true”stored =“true”/></``span``>`|
|----|----|


|`06`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``dynamicField``name``=``"*_ls"``type``=``"long"``indexed``=``"true"``stored``=``"true"``multiValued``=``"true"``/></``span````> <````dynamicField``name =“* _ ls”type =“long”indexed =“true”stored =“true”multiValued =“true”/></``span``>`|
|----|----|


|`07`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``dynamicField``name``=``"*_t"``type``=``"text_general"``indexed``=``"true"``stored``=``"true"``/></``span````> <````dynamicField``name =“* _ t”type =“text_general”indexed =“true”stored =“true”/></``span``>`|
|----|----|


|`08`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``dynamicField``name``=``"*_txt"``type``=``"text_general"``indexed``=``"true"``stored``=``"true"``multiValued``=``"true"``/></``span````> <````dynamicField``name =“* _ txt”type =“text_general”indexed =“true”stored =“true”multiValued =“true”/></``span``>`|
|----|----|


|`09`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``dynamicField``name``=``"*_en"``type``=``"text_en"``indexed``=``"true"``stored``=``"true"``multiValued``=``"true"``/></``span````> <````dynamicField``name =“* _ en”type =“text_en”indexed =“true”stored =“true”multiValued =“true”/></``span``>`|
|----|----|


|`10`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``dynamicField``name``=``"*_b"``type``=``"boolean"``indexed``=``"true"``stored``=``"true"``/></``span````> <````dynamicField``name =“* _ b”type =“boolean”indexed =“true”stored =“true”/></``span``>`|
|----|----|


|`11`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``dynamicField``name``=``"*_bs"``type``=``"boolean"``indexed``=``"true"``stored``=``"true"``multiValued``=``"true"``/></``span````> <````dynamicField``name =“* _ bs”type =“boolean”indexed =“true”stored =“true”multiValued =“true”/></``span``>`|
|----|----|


|`12`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``dynamicField``name``=``"*_f"``type``=``"float"``indexed``=``"true"``stored``=``"true"``/></``span````> <````dynamicField``name =“* _ f”type =“float”indexed =“true”stored =“true”/></``span``>`|
|----|----|


|`13`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``dynamicField``name``=``"*_fs"``type``=``"float"``indexed``=``"true"``stored``=``"true"``multiValued``=``"true"``/></``span````> <````dynamicField``name =“* _ fs”type =“float”indexed =“true”stored =“true”multiValued =“true”/></``span``>`|
|----|----|


|`14`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``dynamicField``name``=``"*_d"``type``=``"double"``indexed``=``"true"``stored``=``"true"``/></``span````> <````dynamicField``name =“* _ d”type =“double”indexed =“true”stored =“true”/></``span``>`|
|----|----|


|`15`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``dynamicField``name``=``"*_ds"``type``=``"double"``indexed``=``"true"``stored``=``"true"``multiValued``=``"true"``/></``span````> <````dynamicField``name =“* _ ds”type =“double”indexed =“true”stored =“true”multiValued =“true”/></``span``>`|
|----|----|




在books.csv文件中，我们有以下字段动态映射到字段类型。
- **series_t** - 映射到动态字段名称“* _t”并分配类型text_general
- **sequence_i** - 映射到动态字段名称“* _i”，并分配类型int
- **genre_s** - 映射到动态字段名称“* _s”并分配类型字符串

## 7.索引数据

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




## 8.下载配置

这是schema.xml配置的一个示例。

**下载**
您可以在这里下载此示例的模式文件： **[schema.xml](http://examples.javacodegeeks.com/wp-content/uploads/2015/08/schema.xml_.zip)**



