# 构建您的第一个Grails 应用程序 - 零度的博客专栏 - CSDN博客
2015年08月14日 09:46:59[零度anngle](https://me.csdn.net/zmx729618)阅读数：622

         Grails 是一种新型 Web 开发框架，它将常见的 Spring 和 Hibernate 等 Java 技术与当前流行的约定优于配置等实践相结合。Grails 是用 Groovy 编写的，它可以提供与遗留 Java 代码的无缝集成，同时还可以加入脚本编制语言的灵活性和动态性。学习完 Grails 之后，您将彻底改变看待 Web 开发的方式。Grails 为您提供 Rails 风格的开发体验，同时以可靠的 Java 技术作为坚强后盾。但是 Grails 不仅仅是 Rails 通往 Java
 平台的简单入口。Grails 吸取了 Rails 的经验，并将它们与现代 Java 开发的意识相结合。可以认为 Grails 是受 Rails *启发*，而不是由 Rails
*转化而来*。本文介绍 Grails 框架，展示它的安装方法，遍览如何构建第一个 Grails 应用程序：介绍本系列后续文章的内容。Java™ 程序员不需要放弃自己喜爱的语言和已有的开发基础设施就可以采纳一种新型的 Web 开发框架。在这个新推出的每月一期的
*精通 Grails* 系列的第一期中，Java 专家 Scott Davis 介绍了 Grails，并演示了如何构建您的第一个 Grails 应用程序。
        请允许我借助另一种开源 Web 开发框架 —— Ruby on Rails —— 来介绍 Grails。Rails 一发布就迷住了开发人员。Rails 的脚手架功能使您可以用以前所需时间的一小部分完成一个新的项目。支持 Rails 的*约定优于配置（convention over configuration）*思想意味着，应用程序可以根据常识性的命名模式自动进行组装（auto-wire），而不必借助繁杂的、容易出错的 XML 配置文件。Ruby 的元编程功能使对象可以神奇地在运行时继承所需的方法和字段，而不会扰乱源代码。
Rails 配得上它所受到的赞美和推崇（现在仍然如此），但是它使 Java 开发人员面临困难的选择。您会因为一个新平台的承诺而放弃自己熟悉的 Java 平台吗？如何处理已有的 Java 代码、已有的生产服务器和经验丰富的 Java 开发人员？
## Groovy 的威力
就像 Rails 与 Ruby 编程语言联系非常紧密一样，Grails 也离不开 Groovy（请参阅 [参考资料](http://www.ibm.com/developerworks/cn/java/j-grails01158/#resources)）。Groovy 是一种动态语言，它在 JVM 上运行，并且与 Java 语言无缝集成。如果阅读了 developerWorks 上的大型 [*实战 Groovy*](http://www.ibm.com/developerworks/cn/views/java/articles.jsp?view_by=search&search_by=%E5%AE%9E%E6%88%98+Groovy%3A) 系列，那么您已经了解了这种语言的威力。如果没有，也不必担心 — 在学习 Grails 的过程中，您将了解到很多关于 Groovy 的知识。Groovy 应该不难学，因为它是特意为 Java 开发人员而设计的。
例如，Groovy 可以大大减少 Java 代码的数量。在 Groovy 中，不再需要为字段编写 getter 和 setter 方法，因为 Groovy 会自动提供它们。不再需要编写
`for Iterator i = list.iterator()` 来循环遍历一系列的项；`list.each` 可以做相同的事情，而且看上去更简洁，表达更清晰。简言之，Groovy 就是 21 世纪的 Java 语言。
如果 Java 开发人员只有重新编写整个应用程序才能利用 Groovy，那么 Groovy 对他们就没有多大的吸引力了。令人高兴的是，Groovy 可以无缝地与已有的代码库集成。Groovy 不会替代 Java 语言 — 它只是提供了增强。您可以很快地掌握 Groovy，因为说到底，Groovy 代码*就是* Java 代码。这两种语言是如此兼容，甚至可以将一个 .java 文件重命名为一个 .groovy 文件 — 例如，将 Person.java 改为 Person.groovy —
 从而得到一个有效的（可执行的）Groovy 文件（虽然这个 Groovy 文件并没有用到 Groovy 提供的任何语法）。 
Groovy 与 Java 语言的深度兼容意味着 Grails 不需要重新创造内部使用的关键技术。相反，您可以以 Groovy 的方式查看熟悉的 Java 库。Groovy 封装了 JUnit
`TestCase` 并以 `GroovyTestCase` 形式提供。Grails 通过 GANT 对 Ant 构建进行了调整，GANT 是 Ant 的一个纯 Groovy 实现。Grails 将 Hibernate 包装在一个小小的 Groovy facade 中，并称之为 GORM — Grails Object/Relational Mapper。Grails 使您在利用已有的 Java 经验的同时，还可以利用最新的 Web 开发实践，以上只是其中的三个例子。
不过，要想全面地鉴赏 Grails，还需要亲身体验一下。现在，让我们来安装 Grails，并创建第一个 Web 应用程序。 
## 安装 Grails
运行 Grails 应用程序所需的一切都在一个 ZIP 文件中。所有的依赖库 — 例如 Groovy、Spring 和 Hibernate — 都已经在那里，随时可以使用。要安装 Grails：
- 从 Grails 站点（见 [参考资料](http://www.ibm.com/developerworks/cn/java/j-grails01158/#resources)）下载并解压 grails.zip。
- 创建一个 `GRAILS_HOME` 环境变量。
- 将 $GRAILS_HOME/bin 添加到 `PATH` 中。
您*的确* 需要安装一个 JDK（Grails 是不错，但是还没有好到 *那种程度*）。Grails 1.0 可在 Java 1.4、1.5 和 1.6 上运行。如果不知道已经安装了哪个版本，可以在命令行提示符下输入
`java -version`。必要时，下载并安装一个与 Grails 兼容的 JDK（见 [参考资料](http://www.ibm.com/developerworks/cn/java/j-grails01158/#resources)）。 
完成安装步骤后，输入 `grails -version` 以进行检查。如果看到以下友好信息，则说明一切都得到正确配置： 
Welcome to Grails 1.0 - http://grails.org/
Licensed under Apache Standard License 2.0
Grails home is set to: /opt/grails
### 附带的 Web 服务器和数据库
有趣的是，不需要单独安装 Web 服务器就可以运行 Grails 应用程序。 Grails 内置了 Jetty servlet 容器。只需输入 
```
grails run-app
```
，就可以使应用程序在 Jetty 容器（见 [参考资料](http://www.ibm.com/developerworks/cn/java/j-grails01158/#resources)）中运行，而不必执行常见的部署过程。在已有的生产服务器上运行 Grails 应用程序也没有问题。通过输入 `grails war` 创建一个标准文件，然后可以将其部署到 Tomcat、JBoss、Geronimo、WebSphere®，或者任何其他遵从 Java EE 2.4 的 servlet 容器。
您也不需要单独安装数据库。Grails 附带了 HSQLDB（见 [参考资料](http://www.ibm.com/developerworks/cn/java/j-grails01158/#resources)），它是一个纯 Java 数据库。通过提供一个随时可用的数据库可以立即提高生产率。由于有了 Hibernate 和 GORM，使用其他数据库（例如 MySQL、PostgreSQL、Oracle Database 或 DB2）也很简单。如果有一个 JDBC driver JAR 再加上通常的连接设置，只需改变一下 DataSource.groovy，就可以立即使用您自己的数据库。
## 编写第一个 Grails 应用程序
我经常旅行 — 一年至少 40 趟。我发现，日程表可以很好地告诉我*何时* 需要达到某个地方，但是不能显示那个地方在*哪里*。而在线地图刚好相反：它们可以解决地点问题，但不能解决时间问题。所以，在本文和本系列接下来的两篇文章中，您将构建一个定制的 Grails 应用程序，在计划旅程时，这个应用程序既可以用于解决时间问题，又可以用于解决地点问题。
## 后续介绍
本系列后面的文章将讨论如何将 Google Calendar 和 Google Maps 与 Grails 相结合。
首先，在一个空白目录下，输入 `grails create-app trip-planner`。稍后，可以看到一个名为 trip-planner 的目录。同 Maven、Rails 和 AppFuse 一样，Grails 会建立一个标准的目录结构。如果您觉得这个目录结构限制了您，并且只有精心设计自己的定制目录树才能使用一个框架，那么这样使用 Grails 不会有多大的乐趣。约定优于配置中的*约定* 部分使您可以拥有任何 Grails 应用程序，并立即知道各个部分之间的联系。
进入 trip-planner 目录，并输入 `grails create-domain-class Trip`。如果一切顺利，将得到两个新的文件：grails-app/domain/Trip.groovy 和 grails-app/test/integration/TripTests.groovy。在后面的文章中，我将谈到测试。目前，我们主要关注域类。一开始，域类看上去如清单 1 所示：
##### 清单 1. Grails 生成的域类
class Trip{
}
看上去没什么内容，对吗？接下来让我们来完善它。为 `Trip` 添加一些字段，如清单 2 所示： 
##### 清单 2. 添加字段后的 `Trip` 类
class Trip { 
  String name
  String city
  Date startDate
  Date endDate
  String purpose
  String notes
}
如前所述，这里不需要创建 getter 和 setter 方法：Groovy 会动态地生成它们。`Trip` 还有很多新的、有用的动态方法，这些方法的名称非常易用理解：
- `Trip.save()` 将数据*保存* 到 HSQLDB 数据库中的 `Trip` 表中。
- `Trip.delete()` 从 `Trip` 表中*删除* 数据。
- `Trip.list()` 返回一个 `Trip` 列表。
- `Trip.get()` 返回一个 `Trip`。
所有这些方法都已经存在，您在需要的时候就可以使用它们。注意，`Trip` 并没有扩展某个父类或者实现某个接口。由于 Groovy 的元编程功能，那些方法只是出现在适当类中的适当位置（只有 grails-app/domain 目录中的类才拥有这些与持久性相关的方法）。
### 构建控制器和视图
创建域类只是成功的一半。每个模型都还需要一个良好的控制器和一些视图（我假设您熟悉 Model-View-Controller 模式；请参阅 [参考资料](http://www.ibm.com/developerworks/cn/java/j-grails01158/#resources)）。输入 `grails generate-all Trip`，以构建一个 grails-app/controllers/TripController.groovy 类，并在 grails-app/views/Trip 中生成一组匹配的 Groovy Server Page（GSP）。对于控制器中的每个
`list` 动作，都有一个相应的 list.gsp 文件。`create` 动作则对应于一个 create.gsp 文件。从这里可以看出约定优于配置的优点：无需 XML 文件就可以匹配这些元素。每个域类根据名称与一个控制器配对。控制器中的每个动作也是根据名称与一个视图配对。如果您愿意，也可以绕开这种基于名称的配置，但是大多数时候只需遵循约定，应用程序自然就可以运行。
看看清单 3 所示的 grails-app/controller/TripController.groovy： 
##### 清单 3. `TripController`
class TripController {
    ...
    def list = {
        if(!params.max) params.max = 10
        [ tripList: Trip.list( params ) ]
    }
    ...
}
Java 开发人员首先会注意到的是，这么少的代码可以实现多少功能。以 `list` 动作为例。起重要作用的是最后一行。Grails 将返回一个 hashmap，其中只有一个名为
`tripList` 的元素。（Groovy 方法的最后一行是一个隐式的 return 语句。如果您愿意，也可以手动地输入单词 
```
return
```
）。`tripList` 元素是 `Trip` 对象的一个 `ArrayList`，`Trip` 对象是通过
`Trip.list()` 方法从数据库中拉出的。通常该方法将返回表中的全部记录。它上面的一行代码表示 “如果 URL 中提供了一个 max 参数，那么使用它来限制返回的
`Trip` 的数量。否则，将 `Trip` 的数量限制为 10”。URL http://localhost:8080/trip-planner/trip/list 将调用这个动作。例如，http://localhost:8080/trip-planner/trip/list?max=3 显示 3 个 trip，而不是通常的 10 个。如果有更多的 trip 要显示，Grails 会自动创建上一页和下一页的分页链接。
那么，如何使用这个 hashmap？看看 grails-app/views/list.gsp，如清单 4 所示：
##### 清单 4. list.gsp
<g:each in="${tripList}" status="i" var="trip">
  <tr class="${(i % 2) == 0 ? 'odd' : 'even'}">
    <td>
      <g:link action="show" id="${trip.id}">${trip.id?.encodeAsHTML()}</g:link>
    </td>
  </tr>
</g:each>
list.gsp 主要是一些老式 HTML 加上少量 GroovyTagLib。以 `g:` 为前缀的就是 GroovyTag。在清单 4 中，`g:each` 遍历
`tripList ArrayList` 中的每个 `Trip`，并构建一个格式良好的 HTML 表格。
对控制器的理解可以归结为三个 *R*：*return*、*redirect* 和 *render*。有些动作利用隐式的 return 语句将数据返回到具有相同名称的 GSP 页面。有些动作进行重定向。例如，如果 URL 中未指定动作，则将调用
`index`：
def index = { redirect(action:list,params:params) }
在此，`TripController` 重定向到 `list` 动作，同时传递 `params` hashmap 中的所有的参数（或
`QueryString`）。
最后，`save` 动作（见清单 5）并没有相应的 save.gsp 页面。如果记录被成功地保存到数据库中，那么该动作会重定向到 
```
show
```
 动作页面。否则，它呈现 create.gsp 页面，以便显示错误，并让您重试。 
##### 清单 5. `save` 动作
def save = {
  def trip = new Trip(params)
  if(!trip.hasErrors() && trip.save()) {
    flash.message = "Trip ${trip.id} created"
    redirect(action:show,id:trip.id)
  }
  else {
    render(view:'create',model:[trip:trip])
  }
}
在此，我们不详细讨论 Grails 是如何工作的，而是看看它的实际效果。
## 应用程序的实际效果
在命令行输入 `grails run-app`。控制台在快速显示一批 Log4j 消息之后，将显示如下所示的消息： 
Server running. Browse to http://localhost:8080/trip-planner
如果端口 8080 上已经有一个服务器在运行，那么将显示一条核心转储信息： 
Server failed to start: java.net.BindException: Address already in use
可以通过两种方法轻松更改 Jetty 所使用的端口。可以通过输入 `grails -Dserver.port=9090 run-app` 临时进行更改。如果要使更改持久，可以从 $GRAILS_HOME/scripts/Init.groovy 中找出以
`serverPort` 开头的那一行，并更改值： 
serverPort = System.getProperty('server.port') ? 
             System.getProperty('server.port').toInteger() : 9090
使 Grails 在您选择的端口上运行之后，在 Web 浏览器中输入 URL。应该可以看到一个欢迎屏幕，其中列出所有的控制器，如图 1 所示：
##### 图 1. Grails 应用程序的欢迎屏幕
![Grails 应用程序的欢迎屏幕](http://www.ibm.com/developerworks/cn/java/j-grails01158/welcome.jpg)
单击 **TripController** 链接。您有一个完整的 CRUD（创建、读取、更新、删除）应用程序可以使用。
使用图 2 所示的页面创建新的 trip：
##### 图 2. Create Trip 页面
![创建 Trip](http://www.ibm.com/developerworks/cn/java/j-grails01158/create.jpg)
使用图 3 所示的页面编辑 trip：
##### 图 3. Trip List 页面
![编辑 Trip](http://www.ibm.com/developerworks/cn/java/j-grails01158/list.jpg)
准备和运行这个应用程序要花多长时间？需要多少代码？下面就是答案：
- 按下 Ctrl-C，关闭 Grails。
- 输入 `grails stats`。
屏幕上将显示输出：
  +----------------------+-------+-------+
  | Name                 | Files |  LOC  |
  +----------------------+-------+-------+
  | Controllers          |     1 |    66 | 
  | Domain Classes       |     1 |     8 | 
  | Integration Tests    |     1 |     4 | 
  +----------------------+-------+-------+
  | Totals               |     3 |    78 | 
  +----------------------+-------+-------+
只需不到 100 行代码，就可以实现应用程序的所有功能。看起来还不错。不过，最后我还要再展示一个窍门。
生成控制器和视图是一项很好的学习体验，而磁盘上的物理文件则有助于说明各个部分是如何连接在一起的。不过在此需要做一件事：删除 `TripController` 类中的内容，并用下面的内容替代：
  class TripController{
    def scaffold = Trip
  }
这行代码告诉 Grails 像对待前一个控制器一样，在运行时在内存中动态地生成所有那些 `list`、`save` 和
`edit` 动作。仅仅 3 行代码就可以产生和 66 行代码一样的行为。 
再次输入 `grails run-app`。是的 — 所有数据都没有了。不必担心。按下 Ctrl-C 关闭 Grails。这一次，输入
`grails prod run-app`。现在处于生产模式下，这意味着在服务器重新启动之前，数据已被保存。通过一连串的单击进入 
```
TripController
```
，保存一些记录。应用程序的行为应该没有什么不同。您已经知道，在浏览器中看到的一切，是由 15 行代码驱动的，可知 Grails 的威力有多大。
## 结束语
希望您对 Grails 的初次体验感到满意。小小一个包中，竟包含了令人惊讶的威力，而您只是看到冰山一角。这个框架的安装非常简单，只需解压一个文件。通过输入几行命令，就可以从头创建一个应用程序。希望这次简单的介绍能勾起您对 Grails 的更大兴趣。当然，本文也为您打好了一个基础，您可以扩展这个例子，尝试各种新的、有趣的方面。
在下个月的文章中，您将专门花一些时间来关注 GORM。您将把日期保存到一个 MySQL 数据库中，进行某些数据验证，并设置一个一对多的关系。不必添加很多代码，就可以明显增强 trip-planner 应用程序的功能。
到那时，好好享受使用 Groovy 和 Grails 的乐趣吧。您对 Web 开发的看法将彻底改变。 
