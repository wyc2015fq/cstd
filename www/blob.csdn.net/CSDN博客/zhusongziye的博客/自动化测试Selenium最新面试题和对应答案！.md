# 自动化测试Selenium最新面试题和对应答案！ - zhusongziye的博客 - CSDN博客





2018年05月21日 20:34:18[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：1309








## 问题1：Selenium是什么，流行的版本有哪些？

Selenium是基于Web的最流行的UI自动化测试工具。它提供了一组支持多种平台的公开API（例如Linux，Windows，Mac OS X等）。此外，像Google Chrome，Mozilla Firefox，Internet Explorer和Safari等所有现代浏览器都可以用来运行Selenium测试。它也涵盖了Android平台，其中Appium是实现Selenium Webdriver界面的工具，用于移动自动化。

值得注意的是，除了许多后来的小型版本之外，硒还有三个主要版本：

Selenium 1.0或Selenium RC，于2004年初发布，提供了一个使用服务器与浏览器交换命令和响应的API集。

Selenium 2.0或Selenium Webdriver，在2011年中推出，并在Selenium功能中引入了一系列重大改进。这些新的API完全取代了服务器组件，并与目标浏览器本地交互。

Selenium 3.0，这个版本是在2016年末发布的大版本。它带来的主要变化是引入Webdriver API的W3C规范，用于浏览器自动化。也就是说，每个主要的浏览器都会有自己的Webdriver API来实现功能。

# 问题2：你如何从命令行启动Selenium RC？

> 
// 简单的启动Selenium RC的方法是

java -jar selenium-server.jar

// 在浏览器中运行一套Selenese脚本

java -jar selenium-server.jar -htmlSuite


# 问题3：在我的机器端口4444不是免费的。我怎样才能使用另一个端口？

> 
//你可以在运行selenium服务器时指定端口为 -

Java -jar selenium-server.jar -port 5555


# 问题4：什么是Selenium Server，它与Selenium Hub有什么不同？

Selenium Server是使用单个服务器作为测试节点的一个独立的应用程序。 Selenium hub代理一个或多个Selenium的节点实例。一个hub 和多个node被称为Selenium grid。运行SeleniumServer与在同一主机上用一个hub和单个节点创建de Selenium grid类似。

# 问题5：你如何从Selenium连接到数据库？

Selenium是一个Web UI自动化工具。它不提供任何API来建立数据库连接。这取决于你使用Selenium进行自动化的编程语言。在下面的例子中，我们假设正在使用Java。

一个Connection对象表示与数据库的连接。当我们使用连接方法连接到一个数据库时，我们创建了一个连接对象，它代表了与数据库的连接。单个数据库可能有一个连接或多个连接，还可能有多个连接到不同的数据库上。

我们可以使用Connection对象来做以下事情：
- 
创建用于执行SQL语句的Statement，PreparedStatement和CallableStatement对象。

- 
可以帮助我们提交或回滚一个JDBC事务。

- 
如果你想知道连接到的数据库或数据源信息，Connection对象通过使用DatabaseMetaData就可以收集有关数据库或数据源的信息。

- 
可以帮助我们关闭数据源。Connection.isClosed() 方法只有在调用了Connection.close()时才返回true 。此方法用于关闭所有连接。


首先我们需要通过使用DriverManager.getConnection()方法，建立与数据库的连接。这个方法接受一个包含URL的字符串。DriverManager类尝试查找可以连接到由字符串URL表示的数据库的驱动程序。每当调用getConnection()方法时，DriverManager类都会检查可以连接到URL中指定的数据库的所有已注册的Driver类的列表。

句法：

> 
String url = "jdbc: odbc: makeConnection";

Connection con = DriverManager.getConnection(url, "userID", "password");


# 问题6：Selenium RC中定位器方法有哪些？
- 
ID

- 
Name

- 
CSS (Cascade Stylesheet)

- 
XPATH (Relative XPath and Absolute XPath)

- 
Dom


# 问题7：你如何验证多个页面上存在的一个对象？

可以使用下面的Selenium命令来检查：

> 
assertTrue(selenium.isElementPresent(locator));


# 问题8：XPath中使用单斜杠和双斜杠有什么区别？
- 
如果XPath是从文档节点开始，它将允许创建“绝对”路径表达式。


例如 “/ html / body / p”匹配所有的段落元素。
- 
如果XPath在文档中的任意位置开始进行选择匹配，那么它将允许创建“相对”路径表达式。


例如 “// p”匹配所有的段落元素。

# 问题9：如何编写Selenium IDE / RC的用户扩展？

用户扩展（UX）存储在Selenium IDE或Selenium RC用来激活扩展的单独文件中。它包含用JavaScript编写的函数定义。

因为Selenium的核心是用JavaScript开发的，所以要符合原语言的标准规则来创建扩展。要创建一个扩展，我们必须用下面的设计格式来编写函数。

> 
// 样例

Selenium.prototype.doFunctionName = function(){

}


函数名称前面的“do”告诉Selenium这个函数可以被调用为一个步骤命令，而不是作为内部函数或私有函数被调用。

# 问题10：如何在页面加载成功后验证元素的存在？

它可以通过下面的代码行来实现。

只需一点时间（以秒为单位）来检查元素，如下所示：

> 
public void waitForElementPresent(String element, int timeout) throws Exception {

for (int second = 0;; second++) {

if (second >= timeout)

fail("Timeout. Unable to find the Specified element" + element);

try {

if (selenium.isElementPresent(element))

break;

} catch (Exception e) {

}

Thread.sleep(1000);

}

}


# 问题11：你对Selenium Grid有什么了解？它提供了什么功能？

Selenium Grid是一款利用现有计算基础架构大幅加速Web应用程序功能测试的工具。允许测试者轻松地在多台机器上并行运行多个测试，并且可以在异构环境中运行。

基于优秀的Selenium Web测试工具，Selenium Grid允许测试者并行运行多个Selenium Remote Control实例。更好的是，它集成显示所有Selenium远程控制，所以不必担心实际的基础设施。Selenium Grid将运行Selenium测试套件所需的时间，缩短到Selenium实例的单个实例运行时间的一小点。

# 问题12：如何从你的Java Class启动Selenium服务器？

> 
try {

seleniumServer = new SeleniumServer();

seleniumServer.start();

} catch (Exception e) {

e.printStackTrace();

}


# 问题13：Selenium中有哪些验证点？

Selenium主要有三种验证点 -
- 
检查页面标题

- 
检查某些文字

- 
检查某些元素（文本框，下拉菜单，表等）


# 问题14：什么是XPath？什么时候应该在Selenium中使用XPath？

XPath是一种在HTML / XML文档中定位的方法，可用于识别网页中的元素。如果没有与页面上的元素相关联的名称/ ID，或者名称/ ID的一部分是常量，则必须使用XPath。

绝对路径用 - / 单斜杠

相对路径用 - // 双斜杠

ID，类，名称也可以用于XPath ：
- 
//input[@name=’q’]

- 
//input[@id=’lst-ib’]

- 
//input[@class=’ lst’]


如果id / name / class的一部分是常量 ：
- 
//input[contains(@id,’lst-ib’)


# 问题15：什么是Selenium的CSS定位器策略？用例子来解释。

CSS位置策略可以与Selenium一起使用来定位元素，它使用CSS定位方法，其中 -


绝对路径用 - （空格符号）

相对路径用 - >表示

ID，类，名称也可以用于XPath：
- 
css=input[name=’q’]

- 
css=input[id=’lst-ib’] or input#lst-ib

- 
css=input[class=’lst’] or input.lst


如果id / name / class只有一部分是常量：
- 
css=input[id*=’lst-ib’)]


使用内部文本的元素位置策略：
- 
css = a:contains(‘log out’)


# 问题16：当有很多定位器时，如ID、名称、XPath、CSS定位器，我应该使用哪一个？

如果有唯一的名称或标识符可用，那么应该使用它们来代替XPath和CSS定位器。如果没有，那么CSS定位器应该被优先考虑，因为在大多数现代浏览器中，它们的评估速度比XPath更快。


# 问题17：在硒中处理多个弹出窗口的机制是什么？

可以使用命令getWindowHandles()来处理多个弹出窗口。


然后将所有窗口名称存储到Set变量中并将其转换为数组。

接下来，通过使用数组索引，导航到特定的窗口。

> 
driver.switchTo().window(ArrayIndex);


# 问题18：你如何处理使用Selenium的Ajax控件？

来看一个例子。假如一个文本框是一个Ajax控件，当我们输入一些文本时，它会显示自动建议的值。


处理这样的控件，需要在文本框中输入值之后，捕获字符串中的所有建议值；然后，分割字符串，取值就好了。

> 
Part2. WebDriver相关面试问题和答案


# 问题19：Selenium Webdriver优于Selenium RC的优点是什么？

Selenium RC的架构相当复杂，WebDriver的架构比Selenium RC简单些。

- 
Selenium RC比较慢，因为它使用了另外一个名为Selenium Core的JavaScript程序。相反，WebDriver比Selenium RC更快，因为它直接与浏览器对话，并使用浏览器自己的引擎来进行控制。

- 
像其他JavaScript代码一样，Selenium Core可以访问禁用的元素。Webdriver以更现实的方式与页面元素进行交互。

- 
Selenium RC的API集已经有所改进，但是仍有经常让人困惑的冗余部分。WebDriver API更简单，不包含任何冗余或混淆的命令。

- 
Selenium RC无法支持无头HtmlUnit浏览器。它需要一个真正的、可见的浏览器来操作。Web Driver可以支持无头HtmlUnit浏览器。

- 
Selenium RC内置了测试结果生成器，并自动生成测试结果的HTML文件。Web驱动程序没有自动生成测试结果文件的内置命令。


# 问题20：“GET”和“NAVIGATE”方法的主要区别是什么？

Get方法能获得一个页面进行加载、或获取页面源代码、或获取文本，就这三。而Navigate将通过刷新，回退，前进的方式导航。


例如 -如果我们想要前进，并做一些功能，并返回到主页。

这可以通过调用< navigate()>方法来实现。

driver.get() 方法会等到整个页面被加载后才可以，而driver.navigate()只是重定向到该网页，并不会等待。

# 问题21：隐式等待与显式等待有什么不同？

隐式等待是设置的全局等待，分为1、页面加载超时等待 ；2、页面元素加载超时；3、异步脚本超时。如果是页面元素超时，设置等待时间，是对页面中的所有元素设置加载时间。隐式等待是其实可以理解成在规定的时间范围内，浏览器在不停的刷新页面，直到找到相关元素或者时间结束。

显式等待只是用于特定搜索的一个计时器。它的可扩展性更强，你可以设置它来等待任何条件。通常情况下，可以使用一些预先构建的条件来等待元素变得可点击，可见，不可见等，或者只是编写适合需求的条件。

# 问题22：你将如何处理Selenium WebDriver中的警报/弹出窗口？

有两种类型的警报通常被引用。
- 
基于Windows的警报弹出窗口

- 
基于Web的警报弹出窗口


基于Web的警报弹出窗口。
- 
WebDriver为用户提供了一种使用Alert界面处理这些弹出窗口的非常有效的方法。

- 
void dismiss() - 一旦出现弹出窗口，dismiss()方法就会点击“Cancel”按钮。

- 
void accept() - 只要弹出窗口出现，accept()方法就会点击“Ok”按钮。

- 
String getText() - getText()方法返回警告框中显示的文本。

- 
void sendKeys(String stringToSend) - sendKeys()方法将指定的字符串模式输入到警告框中。


基于Windows的警报弹出窗口。

处理基于windows的弹出窗口总是有点棘手，因为我们知道Selenium是一个自动化测试工具，它只支持Web应用程序测试，也就是说，它不支持基于Windows的应用程序，窗口警报就是其中之一。
- 
Robot class是基于Java的实用程序，它模拟键盘和鼠标操作，并可以有效地用于处理基于windows的弹出与键盘事件的帮助。

- 
KeyPress和KkeyRelease方法可以分别模拟用户按下和释放键盘上某个键的操作。


# 问题23：如何使用Selenium WebDriver截图？

![](https://img-blog.csdn.net/20180520173026793?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 问题24：如何在WebDriver/如何管理HTTPS中的安全连接错误中解决Firefox中的SSL证书问题？

![](https://img-blog.csdn.net/20180520173035223?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

# 问题25：如何解决IE中的SSL认证问题？

> 
// 打开浏览器后添加下面的命令

driver.navigate().to(“javascript:document.getElementById(‘overridelink’).click()”);


# 问题26：Selenium WebDriver中的可用定位器是什么？
- 
ID,

- 
Name,名称

- 
CSS,

- 
XPath,

- 
Class name,

- 
TagName,

- 
LinkText, 链接文本

- 
Partial Link Text.部分链接文本


# 问题27：如何处理WebDriver中的AJAX控件？

AJAX代表异步JavaScript和XML。它不依赖于创建有效的XML所需的打开和关闭标签的额外开销。大部分时间WebDriver自动处理Ajax控件和调用。如果不能处理的话，可以按照下面的方式来处理。

> 
//Waiting for Ajax Control

WebElement AjaxElement = (new WebDriverWait(driver,

10)).until(ExpectedConditions.presenceOfElementLocated(By.("")));


# 问题28：如何在标题菜单的子菜单项上执行鼠标移动操作？

应该首先移动菜单标题，然后移至弹出菜单项并单击它。不要忘记在最后调用actions.perform() 。以下是一些示例Java代码：

![](https://img-blog.csdn.net/20180520173048806?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

> 



Part3. 一般框架面试问题和答案





# 问题29：大致分类和比较TDD/BDD和DDD框架？

你可能听说过所有的这些缩写词。在这里会简要地解释它们，以及它们在系统测试生命周期中如何发挥作用的。


TDD - 测试驱动开发。

也被称为测试驱动设计，是一个软件开发的方法，在源代码上重复进行单元测试。写测试、看它失败、然后重构。这个概念是，先编写测试，然后来检查我们写的代码是否正常工作。每次测试后，重构完成，然后再次执行相同或类似的测试。该过程需要重复多次，直到每个单元在功能上按预期工作。TDD是由XP引入的。

BDD - 行为驱动开发。

行为驱动的开发将TDD的一般技术和原理与领域驱动设计的思想相结合。其目的是帮助人们设计系统（即开发人员）确定合适的测试来编写测试 - 即反映利益相关者所期望行为的测试。

DDD域驱动的开发。

DDD将业务领域概念映射到软件工件中。DDD框架提供以下好处：
- 
帮助团队在业务和IT利益相关者之间建立一个共同的模型

- 
该模型是模块化的，可扩展的，易于维护，该设计反映了一种商业模式。

- 
它提高了业务领域对象的可重用性和可测试性。


# 问题30：什么是数据驱动框架？它与关键字驱动框架有什么不同？

数据驱动框架。

在这个框架中，测试用例逻辑驻留在测试脚本中。测试数据被分离并保存在测试脚本之外。测试数据是从外部文件（Excel文件）中读取的，并被加载到测试脚本中的变量中。变量用于输入值和验证值。

关键字驱动。

关键字/表驱动框架需要开发数据表和关键字。它们独立于执行它们的测试自动化工具。可以使用或不使用应用程序来设计测试。在关键字驱动的测试中，被测试的应用程序的功能记录在一个表格中，以及每个测试的分步说明。

# 问题31：解释使用TestNG而不是JUnit框架的好处？

TestNG相较于Junit的优势：

- 
在JUnit中，我们必须声明@BeforeClass和@AfterClass，这是JUnit中的一个约束，而在TestNG中没有像这样的约束。

- 
TestNG提供了更多的setUp / tearDown级别。1.@ Before/AfterSuite 2.@Before/AfterTest 3.@Before/AfterGroup

- 
TestNG中不需要扩展任何类。

- 
TestNG中没有方法名称约束，就像JUnit一样。

- 
在TestNG中，我们可以告诉测试一个方法依赖于另一个方法，而在JUnit中这是不可能的。

- 
测试用例的分组在TestNG中可用，而JUnit中则不可用。执行可以基于组完成。例如，如果你已经定义了许多案例，并通过将2个组分别定义为“离职“与”回归”隔离。如果你只是想执行“理智”的情况，那就告诉TestNG执行“理智”。TestNG将自动执行属于“离职”组的案例。

- 
另外，TestNG支持并行测试用例执行。


# 问题32：与@Test注释相关的TestNG参数的目的是什么？

在TestNG中，参数是修改注释功能的关键字。

# 问题33：可以使用TestNG运行一组测试用例吗？

是的，TestNG框架支持在测试组的帮助下执行多个测试用例。


它提供了以下选项来运行特定组中的测试用例。

如果想基于回归测试或冒烟测试等其中一个组来执行测试用例，那么：

> 
@Test(groups = {“regression-tests”, “smoke-tests”})


# 问题34：WebDriver哪个实现是最快的，为什么？

WebDriver的最快的实现是 HTMLUnitDriver。

原因是HTMLUnitDriver不会在浏览器中执行测试。相反，它使用简单的HTTP请求 - 响应机制来运行测试用例。

这种方法比需要启动浏览器来测试执行的方式要快得多。

# 问题35：是否可以在Selenium 2.0中使用Selenium RC API？

是的，可以用Selenium 2.0来模拟Selenium 1.0 API（即RC）。但并不是所有的Selenium 1.0方法都支持。


为了达到这个目的，需要从WebDriver获取Selenium实例并使用Selenium方法。

在Selenium 2.0中模拟Selenium 1.0时，方法执行速度也可能会变慢。

# 问题36：可以在Java，Dot Net或Ruby中使用Selenium Grid吗？
- 
使用Java，可以利用TestNG的并行测试功能来驱动Selenium Grid测试。

- 
使用.Net，可以使用“Gallio”并行执行测试。

- 
使用Ruby，可以使用“DeepTest”来分发测试。





