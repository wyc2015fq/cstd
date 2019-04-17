# Apache Solr复制示例 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年02月12日 10:59:37[boonya](https://me.csdn.net/boonya)阅读数：456










本文英文版地址：[https://examples.javacodegeeks.com/enterprise-java/apache-solr/apache-solr-replication-example/](https://examples.javacodegeeks.com/enterprise-java/apache-solr/apache-solr-replication-example/)


作者姓名：Veeramani
 Kalyanasundaram


作者简介：Veera是在电信领域工作的软件架构师，拥有丰富的Java中间件技术经验。他是一名OOAD从业者，对性能工程感兴趣。
翻译备注：本文采用谷歌翻译，请有时间的同学可以从英文版学习。

在这个Solr复制示例的示例中，我们将向您展示如何在Apache Solr中设置复制，并演示如何将新记录从主内核复制到从属内核。对于这个例子，我们将考虑一个主服务器和两个从服务器。在生产环境中，我们将使用不同的机器来托管主服务器和从服务器。
在这里，我们将使用不同的端口在同一台机器上运行主和从Solr服务器。

我们首选的环境是Windows。
在开始Solr安装之前，请确保已安装JDK并已正确设置Java_Home。


## 1.安装Apache Solr

首先，从以下位置下载最新版本的Apache Solr。

`http://lucene.apache.org/solr/downloads.html`

Apache Solr已经经历了从4.xx到5.0.0的各种更改，因此如果您有不同版本的Solr，则需要下载5.xx版本以遵循此示例。下载Solr zip文件后，将其解压缩到文件夹中。提取的文件夹将如下所示。

![solr_folder](https://examples.javacodegeeks.com/wp-content/uploads/2015/04/solr_folder1.jpg)
Solr文件夹


该`bin`文件夹中包含的脚本来启动和停止服务器。
该`example`文件夹包含几个示例文件。我们将使用其中一个来演示复制的工作原理。该`server`文件夹包含`logs` ，所有的Solr的日志写入文件夹。在索引期间检查日志中的任何错误将是有帮助的。该`solr`下服务器文件夹包含不同的集合或核心。
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


## 2.配置Solr - 主站

在本节中，我们将向您展示如何为Solr实例配置主内核。
Apache Solr附带一个名为Schemaless模式的选项。此选项允许用户构建有效的模式，而无需手动编辑模式文件。在这个例子中，我们将使用参考configset
`sample_techproducts_configs` 。

### 2.1创建主Core

首先，我们需要创建一个用于索引数据的核心。
Solr create命令有以下选项：
- **-c <名称>** -芯或集合的名称来创建（必需）。
- **-d <confdir>** -配置目录，在SolrCloud模式是有用的。
- **-n <CONFIGNAME>** -配置名称。
此默认值与核心或集合的名称相同。
- **-p <端口>** -本地Solr的实例的端口发送创建命令;默认情况下，脚本尝试通过查找正在运行的Solr实例来检测端口。
- **-s <碎片>** -碎片的数量成，缺省值为1分割的集合。
- **-rf <复制品>** -集合中的每个文件的副本的数量。
默认值为1。

在本示例中，我们将使用-c参数作为核心名称，-rf参数用于复制，-d参数用于配置目录。

现在浏览`solr-5.0.0\bin`在命令窗口中的文件夹，并发出以下命令。

`solr create -c master -d sample_techproducts_configs -p 8983 -rf 3`

我们可以在命令窗口中看到以下输出。



|`1`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>Creating new core``'master'``using ``command``:</span>使用命令创建新核心``'master'``：</span>`|
|----|----|


|`2`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"````>[http://localhost:8983/solr/admin/cores?action=CREATE&name=master&instanceDi</span](http://localhost:8983/solr/admin/cores?action=CREATE&name=master&instanceDi%3C/span)> http：// localhost：8983 / solr / admin / cores？action = CREATE＆name = master＆instanceDi</span>```|
|----|----|


|`3`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>r=master</span> r = master</span>`|
|----|----|


|`4`||
|----|----|


|`5`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>{</span> {</span>`|
|----|----|


|`6`|` ``<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>``"responseHeader"``:{</span> “responseHeader”：{</span>`|
|----|----|


|`7`|` ``<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>``"status"``:0,</span> “status”：0，</span>`|
|----|----|


|`8`|` ``<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>``"QTime"``:1563},</span> “QTime”：1563}，</span>`|
|----|----|


|`9`|` ``<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>``"core"``:``"master"````}</span> “core”：“master”}</span>```|
|----|----|




现在我们可以导航到以下URL，并看到主核心被填充在核心选择器中。
您还可以查看核心的统计信息。

`http://localhost:8983/solr/#/master`

![solrreplication_master](https://examples.javacodegeeks.com/wp-content/uploads/2015/07/solrreplication_master1.jpg)
主控台


### 2.2修改solrconfig

打开文件`solrconfig.xml`的文件夹下， `server\solr\master\conf`和添加配置的requestHandler标签下的主。我们将设定值`replicateAfter`和`backAfter`来`optimize` 。在`confFiles`参数值是根据我们要创建从集合名称设置。

*solrconfig.xml*



|`01`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>``<!-- Replication Handler -->``</``span``> <！ - 复制处理程序 - ></``span``>`|
|----|----|


|`02`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``requestHandler``name``=``"/replication"``class``=``"solr.ReplicationHandler"``></``span````> <````requestHandler``name =“/ replication”class =“solr.ReplicationHandler”></``span``>`|
|----|----|


|`03`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``lst``name``=``"master"``></``span````> <````lst``name =“master”></``span``>`|
|----|----|


|`04`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``str``name``=``"replicateAfter"``>optimize</``str``></``span````> <````str``name =“replicateAfter”> optimize </``str``></``span``>`|
|----|----|


|`05`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``str``name``=``"backupAfter"``>optimize</``str``></``span````> <````str````name =“backupAfter”> optimize </````str``></``span``>`|
|----|----|


|`06`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``str``name``=``"confFiles"``>solrconfig_slave.xml:solrconfig.xml,x.xml,y.xml</``str``></``span````> <````str``name =“confFiles”> solrconfig_slave.xml：solrconfig.xml，x.xml，y.xml </``str``></``span``>`|
|----|----|


|`07`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``str``name``=``"commitReserveDuration"``>00:00:10</``str``></``span````> <````str``name =“commitReserveDuration”> 00:00:10 </``str``></``span``>`|
|----|----|


|`08`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````></``lst``></``span````> </````lst``></``span``> `|
|----|----|


|`09`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``int``name``=``"maxNumberOfBackups"``>2</``int``></``span````> <````int````name =“maxNumberOfBackups”> 2 </````int``></``span``>`|
|----|----|


|`10`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``lst``name``=``"invariants"``></``span````> <````lst``name =“invariants”></``span``>`|
|----|----|


|`11`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``str``name``=``"maxWriteMBPerSec"``>16</``str``></``span````> <````str````name =“maxWriteMBPerSec”> 16 </````str``></``span``>`|
|----|----|


|`12`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````></``lst``></``span````> </````lst``></``span``>`|
|----|----|


|`13`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````></``requestHandler``></``span````> </````requestHandler``></``span``>`|
|----|----|




既然我们已经修改了`solrconfig`我们必须重新启动Solr的服务器。发出在命令窗口输入下列命令导航到`solr-5.0.0\bin` 。

`solr stop -all`

`solr start`

## 3.配置Solr - 从站

对于这个例子，我们将创建两个从核。来自主机核心的数据将被复制到两个从机。我们将在具有不同端口的同一机器上与主核心一起运行两个从机。为此，请将另一个solr服务器副本提取到名为solr1的文件夹。
导航到`solr-5.0.0\bin` solr1的文件夹中的命令窗口，并发出以下命令。

`solr start -p 9000`

-p选项将在不同的端口中启动solr服务器。
对于第一个从机，我们将使用端口9000。
现在导航到`solr-5.0.0\bin`在命令窗口中的奴隶的文件夹，并发出以下命令。

`solr create -c slave -d sample_techproducts_configs -p 9000`

我们可以在命令窗口中看到以下输出。



|`1`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>Creating new core``'slave'``using ``command``:</span>使用命令创建新核心“slave”：</span>`|
|----|----|


|`2`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"````>[http://localhost:9000/solr/admin/cores?action=CREATE&name=slave&instanceDir=slave</span](http://localhost:9000/solr/admin/cores?action=CREATE&name=slave&instanceDir=slave%3C/span)> http：// localhost：9000 / solr / admin / cores？action = CREATE＆name = slave＆instanceDir = slave</span>```|
|----|----|


|`3`||
|----|----|


|`4`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>{</span> {</span>`|
|----|----|


|`5`|` ``<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>``"responseHeader"``:{</span> “responseHeader”：{</span>`|
|----|----|


|`6`|` ``<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>``"status"``:0,</span> “status”：0，</span>`|
|----|----|


|`7`|` ``<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>``"QTime"``:1778},</span> “QTime”：1778}，</span>`|
|----|----|


|`8`|` ``<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>``"core"``:``"slave"````}</span> “core”：“slave”}</span>```|
|----|----|




现在打开文件`solrconfig.xml`的文件夹下， `server\solr\slave\conf`和添加配置的requestHandler标签下的奴隶。在配置上，我们将指向站对`masterUrl`进行复制。所述`pollInterval`被设定为20秒。
它是从设备做出的两个轮询请求之间的时间差。

*solrconfig.xml*



|`01`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>``<!-- Replication Handler -->``</``span``> <！ - 复制处理程序 - ></``span``>`|
|----|----|


|`02`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``requestHandler``name``=``"/replication"``class``=``"solr.ReplicationHandler"``></``span````> <````requestHandler``name =“/ replication”class =“solr.ReplicationHandler”></``span``>`|
|----|----|


|`03`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``lst``name``=``"slave"``></``span````> <````lst``name =“slave”></``span``>`|
|----|----|


|`04`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>````<!--fully qualified url for the replication handler of master.</span> <！ -  master的复制处理程序的完全限定URL。</span> <span class="notranslate" onmouseover="_tipon(this)" onmouseout="_tipoff()"><span class="google-src-text" style="direction: ltr; text-align: left">It is possible</span>有可能的</span>```|
|----|----|


|`05`|`<span class="notranslate" onmouseover="_tipon(this)" onmouseout="_tipoff()"><span class="google-src-text" style="direction: ltr; text-align: left">to pass on this as</span>传递这个</span>`|
|----|----|


|`06`|`<span class="notranslate" onmouseover="_tipon(this)" onmouseout="_tipoff()"><span class="google-src-text" style="direction: ltr; text-align: left">a request param for the fetchindex command-->``</``span````> fetchindex命令的请求参数 - ></````span``>`|
|----|----|


|`07`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``str``name``=``"masterUrl"``>[http://localhost:8983/solr/master/replication</](http://localhost:8983/solr/master/replication%3C/)``str``></``span````> <````str``name =“masterUrl”> http：// localhost：8983 / solr / master / replication </``str``></``span``>`|
|----|----|


|`08`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````>````<!--Interval in which the slave should poll master .Format is HH:mm:ss .</span> <！ - 从机应轮询主机的间隔.Format为HH：mm：ss。</span> <span class="notranslate" onmouseover="_tipon(this)" onmouseout="_tipoff()"><span class="google-src-text" style="direction: ltr; text-align: left">If</span>如果</span>```|
|----|----|


|`09`|`<span class="notranslate" onmouseover="_tipon(this)" onmouseout="_tipoff()"><span class="google-src-text" style="direction: ltr; text-align: left">this is absent slave does not</span>这是没有奴隶不</span>`|
|----|----|


|`10`|`<span class="notranslate" onmouseover="_tipon(this)" onmouseout="_tipoff()"><span class="google-src-text" style="direction: ltr; text-align: left">poll automatically.</span>自动轮询。</span>`|
|----|----|


|`11`|`<span class="notranslate" onmouseover="_tipon(this)" onmouseout="_tipoff()"><span class="google-src-text" style="direction: ltr; text-align: left">But a fetchindex can be triggered from the admin or the http API -->``</``span````>但fetchindex可以从admin或http API  - >触发</````span``>`|
|----|----|


|`12`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````><``str``name``=``"pollInterval"``>00:00:20</``str``></``span````> <````str``name =“pollInterval”> 00:00:20 </``str``></``span``>`|
|----|----|


|`13`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````></``lst``></``span````> </````lst``></``span``>`|
|----|----|


|`14`|`<``span``class``=``"notranslate"``onmouseover``=``"_tipon(this)"``onmouseout``=``"_tipoff()"``><``span``class``=``"google-src-text"``style``=````"direction: ltr; text-align: left"````></``requestHandler``></``span````> </````requestHandler``></``span``>`|
|----|----|




因为我们修改了solrconfig，我们必须重新启动solr服务器。
发出在命令窗口输入下列命令导航到`solr-5.0.0\bin` 。

`solr stop -all`

`solr start -p 9000`

现在使用以下URL打开从控制台。复制部分将显示反映我们在solrconfig中进行的配置的配置。

`http://localhost:9000/solr/#/slave/replication`

![solrreplication_slave](https://examples.javacodegeeks.com/wp-content/uploads/2015/07/solrreplication_slave.jpg)
slave-1复制控制台


要创建另一个从服务器，请执行相同的步骤，并在端口9001中配置服务器。我们现在可以使用以下URL打开控制台，并验证复制部分中的配置。

`http://localhost:9001/solr/#/slave/replication`

![solrreplication_slave2](https://examples.javacodegeeks.com/wp-content/uploads/2015/07/solrreplication_slave2.jpg)
slave-2复制控制台


## 4.索引和复制

现在我们将索引指向主核心的示例数据。
Apache Solr附带一个名为SimplePostTool的独立Java程序。该程序打包成JAR，并提供与文件夹下的安装`example\exampledocs` 。

现在，我们定位到`example\exampledocs`文件夹在命令提示符处键入以下命令。您将看到一堆使用该工具的选项。

`java -jar post.jar -h`

使用格式一般如下

```
Usage: java [SystemProperties] -jar post.jar [-h|-] [<file|folder|url|arg> 

[<file|folder|url|arg>...]]
```


正如我们前面所说，我们将索引solr安装附带的“books.csv”文件中的数据。
我们将导航到`solr-5.0.0\example\exampledocs`在命令提示符并发出以下命令。


```bash
java -Dtype=text/csv -Durl=http://localhost:8983/solr/master/update -jar post.jar books.csv
```


这里使用的SystemProperties是：
- -Dtype - 数据文件的类型。
- -Durl - jcg核心的URL。

文件“books.csv”现在将被索引，并且命令提示符将显示以下输出。



|`1`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>SimplePostTool version 5.0.0</span> SimplePostTool版本5.0.0</span>`|
|----|----|


|`2`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"````>Posting files to [base] url[http://localhost:8983/solr/master/update](http://localhost:8983/solr/master/update) using content-````type``text/csv...</span>将文件发布到[base] url http：// localhost：8983 / solr / master / update使用content-``type``text / csv ...</span>`|
|----|----|


|`3`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>POSTing``file``books.csv to [base]</span>将文件books.csv发布到[base]</span>`|
|----|----|


|`4`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>1 files indexed.</span> 1个文件已索引。</span>`|
|----|----|


|`5`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"````>COMMITting Solr index changes to[http://localhost:8983/solr/master/update...</span](http://localhost:8983/solr/master/update...%3C/span)>提交Solr索引更改为http：// localhost：8983 / solr / master / update ...</span>```|
|----|----|


|`6`|`<span class=``"notranslate"``onmouseover=``"_tipon(this)"``onmouseout=``"_tipoff()"````><span class=````"google-src-text"``style=``"direction: ltr; text-align: left"``>Time spent: 0:00:00.604</span>所用时间：0：00：00.604</span>`|
|----|----|




现在打开从核心的控制台，我们可以看到数据自动复制。

`http://localhost:9000/solr/#/slave`

![solrreplication_slave_data](https://examples.javacodegeeks.com/wp-content/uploads/2015/07/solrreplication_slave_data.jpg)
从控制台 - 数据复制


## 5.添加新记录

现在，我们通过向主内核添加记录来进一步验证复制。
为此，打开主控制台URL。

`http://localhost:8983/solr/#/master/documents`

导航到文档部分，然后选择文档类型为CSV，并在文档文本区域中输入以下内容，然后单击提交。


```
id,cat,name,price,inStock,author,series_t,sequence_i,genre_s 

123,book,Apache Solr,6.99,TRUE,Veera,JCG,1,Technical
```


![solrreplication_insert](https://examples.javacodegeeks.com/wp-content/uploads/2015/07/solrreplication_insert.jpg)
主控台 - 添加新记录


数据将被添加到主控核心，并被复制到从属服务器。
要验证它，请导航到从核。我们可以找到文档计数增加到11.我们还可以使用从属管理控制台中的查询部分来验证它。打开以下URL。

`http://localhost:9000/solr/#/slave/query`

输入值`name:apache`在`q`文本区域，然后单击执行查询。我们在主核上插入的新记录将反映在从核中。

![solrreplication_output](https://examples.javacodegeeks.com/wp-content/uploads/2015/07/solrreplication_output.jpg)
从控制台 - 查询


## 6.下载配置

这是Apache Solr复制的一个示例。

**下载**
你可以在这里下载主配置： [**solrconfig主**](http://examples.javacodegeeks.com/wp-content/uploads/2015/07/solrconfig_master.zip)在这里从配置： [**solrconfig奴隶**](http://examples.javacodegeeks.com/wp-content/uploads/2015/07/solrconfig_slave.zip)



