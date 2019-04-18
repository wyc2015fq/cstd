# Spring 的优秀工具类盘点，第 2 部分: 特殊字符转义和方法入参检测工具类 - z69183787的专栏 - CSDN博客
2014年04月17日 13:19:22[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2197
## 特殊字符转义
由于 Web 应用程序需要联合使用到多种语言，每种语言都包含一些特殊的字符，对于动态语言或标签式的语言而言，如果需要动态构造语言的内容时，一个我们经常会碰到的问题就是特殊字符转义的问题。下面是 Web 开发者最常面对需要转义的特殊字符类型：
- 
HTML 特殊字符；
- 
JavaScript 特殊字符；
- 
SQL 特殊字符；
如果不对这些特殊字符进行转义处理，则不但可能破坏文档结构，还可以引发潜在的安全问题。Spring 为 HTML 和 JavaScript 特殊字符提供了转义操作工具类，它们分别是 HtmlUtils 和 JavaScriptUtils。
### HTML 特殊字符转义
HTML 中 <，>，& 等字符有特殊含义，它们是 HTML 语言的保留字，因此不能直接使用。使用这些个字符时，应使用它们的转义序列：
- 
&：&
- 
" ："
- 
< ：<
- 
> ：>
由于 HTML 网页本身就是一个文本型结构化文档，如果直接将这些包含了 HTML 特殊字符的内容输出到网页中，极有可能破坏整个 HTML 文档的结构。所以，一般情况下需要对动态数据进行转义处理，使用转义序列表示 HTML 特殊字符。下面的 JSP 网页将一些变量动态输出到 HTML 网页中：
##### 清单 1. 未进行 HTML 特殊字符转义处理网页
<%@ page language="java" contentType="text/html; charset=utf-8"%>
<%!
   String userName = "</td><tr></table>";
   String address = " \" type=\"button";
 %>
<table border="1">
   <tr>
     <td>姓名：</td><td><%=userName%></td> ①
   </tr>
   <tr>
     <td>年龄：</td><td>28</td>
   </tr>
</table>
 <input value="<%=address%>"  type="text" /> ②
在 ① 和 ② 处，我们未经任何转义处理就直接将变量输出到 HTML 网页中，由于这些变量可能包含一些特殊的 HTML 的字符，它们将可能破坏整个 HTML 文档的结构。我们可以从以上 JSP 页面的一个具体输出中了解这一问题：
<table border="1">
   <tr>
     <td>姓名：</td><td></td><tr></table></td> 
     ① 破坏了 <table> 的结构
   </tr>
   <tr>
     <td>年龄：</td><td>28</td>
   </tr>
</table>
 <input value=" " type="button"  type="text" /> 
 ② 将本来是输入框组件偷梁换柱为按钮组件
融合动态数据后的 HTML 网页已经面目全非，首先 ① 处的 <table> 结构被包含 HTML 特殊字符的 userName 变量截断了，造成其后的 <table> 代码变成无效的内容；其次，② 处 <input> 被动态数据改换为按钮类型的组件（type="button"）。为了避免这一问题，我们需要事先对可能破坏 HTML 文档结构的动态数据进行转义处理。Spring 为我们提供了一个简单适用的 HTML 特殊字符转义工具类，它就是 HtmlUtils。下面，我们通过一个简单的例子了解 HtmlUtils
 的具体用法：
##### 清单 2. HtmpEscapeExample
package com.baobaotao.escape;
import org.springframework.web.util.HtmlUtils;
public class HtmpEscapeExample {
    public static void main(String[] args) {
        String specialStr = "<div id=\"testDiv\">test1;test2</div>";
        String str1 = HtmlUtils.htmlEscape(specialStr); ①转换为HTML转义字符表示
        System.out.println(str1);
       
        String str2 = HtmlUtils.htmlEscapeDecimal(specialStr); ②转换为数据转义表示
        System.out.println(str2);
       
        String str3 = HtmlUtils.htmlEscapeHex(specialStr); ③转换为十六进制数据转义表示
        System.out.println(str3);
       
        ④下面对转义后字符串进行反向操作
        System.out.println(HtmlUtils.htmlUnescape(str1));
        System.out.println(HtmlUtils.htmlUnescape(str2));
        System.out.println(HtmlUtils.htmlUnescape(str3));
    }
}
HTML 不但可以使用通用的转义序列表示 HTML 特殊字符，还可以使用以 # 为前缀的数字序列表示 HTML 特殊字符，它们在最终的显示效果上是一样的。HtmlUtils 提供了三个转义方法：
|方法|说明|
|----|----|
|```static String htmlEscape(String input)```|将 HTML 特殊字符转义为 HTML 通用转义序列；|
|```static String htmlEscapeDecimal(String input)```|将 HTML 特殊字符转义为带 # 的十进制数据转义序列；|
|```static String htmlEscapeHex(String input)```|将 HTML 特殊字符转义为带 # 的十六进制数据转义序列；|
此外，HtmlUtils 还提供了一个能够将经过转义内容还原的方法：htmlUnescape(String input)，它可以还原以上三种转义序列的内容。运行以上代码，您将可以看到以下的输出：
str1:<div id="testDiv">test1;test2</div>
str2:<div id="testDiv">test1;test2</div>
str3:<div id="testDiv">test1;test2</div>
<div id="testDiv">test1;test2</div>
<div id="testDiv">test1;test2</div>
<div id="testDiv">test1;test2</div>
您只要使用 HtmlUtils 对代码 [清单
 1](http://www.ibm.com/developerworks/cn/java/j-lo-spring-utils2/#list1) 的 userName 和 address 进行转义处理，最终输出的 HTML 页面就不会遭受破坏了。
### JavaScript 特殊字符转义
JavaScript 中也有一些需要特殊处理的字符，如果直接将它们嵌入 JavaScript 代码中，JavaScript 程序结构将会遭受破坏，甚至被嵌入一些恶意的程序。下面列出了需要转义的特殊 JavaScript 字符：
- 
' ：\'
- 
" ：\"
- 
\ ：\\
- 
走纸换页： \f
- 
换行：\n
- 
换栏符：\t
- 
回车：\r
- 
回退符：\b

我们通过一个具体例子演示动态变量是如何对 JavaScript 程序进行破坏的。假设我们有一个 JavaScript 数组变量，其元素值通过一个 Java List 对象提供，下面是完成这一操作的 JSP 代码片断：
##### 清单 3. jsTest.jsp：未对 JavaScript 特殊字符进行处理
<%@ page language="java" contentType="text/html; charset=utf-8"%>
<jsp:directive.page import="java.util.*"/>
<%
  List textList = new ArrayList();
  textList.add("\";alert();j=\"");
%>
<script>
  var txtList = new Array();
   <% for ( int i = 0 ; i < textList.size() ; i++) { %>
     txtList[<%=i%>] = "<%=textList.get(i)%>"; 
	 ① 未对可能包含特殊 JavaScript 字符的变量进行处理
   <% } %>
</script>
当客户端调用这个 JSP 页面后，将得到以下的 HTML 输出页面：
<script>
  var txtList = new Array();
   txtList[0] = "";alert();j=""; ① 本来是希望接受一个字符串，结果被植入了一段JavaScript代码
</script>
由于包含 JavaScript 特殊字符的 Java 变量直接合并到 JavaScript 代码中，我们本来期望 ① 处所示部分是一个普通的字符串，但结果变成了一段 JavaScript 代码，网页将弹出一个 alert 窗口。想像一下如果粗体部分的字符串是“";while(true)alert();j="”时会产生什么后果呢？
因此，如果网页中的 JavaScript 代码需要通过拼接 Java 变量动态产生时，一般需要对变量的内容进行转义处理，可以通过 Spring 的 JavaScriptUtils 完成这件工作。下面，我们使用 JavaScriptUtils 对以上代码进行改造：
<%@ page language="java" contentType="text/html; charset=utf-8"%>
<jsp:directive.page import="java.util.*"/>
<jsp:directive.page import="org.springframework.web.util.JavaScriptUtils"/>
<%
  List textList = new ArrayList();
  textList.add("\";alert();j=\"");
%>
<script>
   var txtList = new Array();
   <% for ( int i = 0 ; i < textList.size() ; i++) { %>
   ① 在输出动态内容前事先进行转义处理
   txtList[<%=i%>] = "<%=JavaScriptUtils.javaScriptEscape(""+textList.get(i))%>";
   <% } %>
</script>
通过转义处理后，这个 JSP 页面输出的结果网页的 JavaScript 代码就不会产生问题了：
<script>
   var txtList = new Array();
   txtList[0] = "\";alert();j=\"";
   ① 粗体部分仅是一个普通的字符串，而非一段 JavaScript 的语句了
</script>
### SQL特殊字符转义
应该说，您即使没有处理 HTML 或 JavaScript 的特殊字符，也不会带来灾难性的后果，但是如果不在动态构造 SQL 语句时对变量中特殊字符进行处理，将可能导致程序漏洞、数据盗取、数据破坏等严重的安全问题。网络中有大量讲解 SQL 注入的文章，感兴趣的读者可以搜索相关的资料深入研究。
虽然 SQL 注入的后果很严重，但是只要对动态构造的 SQL 语句的变量进行特殊字符转义处理，就可以避免这一问题的发生了。来看一个存在安全漏洞的经典例子：
SELECT COUNT(userId) 
FROM t_user 
WHERE userName='"+userName+"' AND password ='"+password+"';
以上 SQL 语句根据返回的结果数判断用户提供的登录信息是否正确，如果 userName 变量不经过特殊字符转义处理就直接合并到 SQL 语句中，黑客就可以通过将 userName 设置为 “1' or '1'='1”绕过用户名/密码的检查直接进入系统了。
所以除非必要，一般建议通过 PreparedStatement 参数绑定的方式构造动态 SQL 语句，因为这种方式可以避免 SQL 注入的潜在安全问题。但是往往很难在应用中完全避免通过拼接字符串构造动态 SQL 语句的方式。为了防止他人使用特殊 SQL 字符破坏 SQL 的语句结构或植入恶意操作，必须在变量拼接到 SQL 语句之前对其中的特殊字符进行转义处理。Spring 并没有提供相应的工具类，您可以通过 jakarta commons lang 通用类包中（spring/lib/jakarta-commons/commons-lang.jar）的
 StringEscapeUtils 完成这一工作：
##### 清单 4. SqlEscapeExample
package com.baobaotao.escape;
import org.apache.commons.lang.StringEscapeUtils;
public class SqlEscapeExample {
    public static void main(String[] args) {
        String userName = "1' or '1'='1";
        String password = "123456";
        userName = StringEscapeUtils.escapeSql(userName);
        password = StringEscapeUtils.escapeSql(password);
        String sql = "SELECT COUNT(userId) FROM t_user WHERE userName='"
            + userName + "' AND password ='" + password + "'";
        System.out.println(sql);
    }
}
事实上，StringEscapeUtils 不但提供了 SQL 特殊字符转义处理的功能，还提供了 HTML、XML、JavaScript、Java 特殊字符的转义和还原的方法。如果您不介意引入 jakarta commons lang 类包，我们更推荐您使用 StringEscapeUtils 工具类完成特殊字符转义处理的工作。
[回页首](http://www.ibm.com/developerworks/cn/java/j-lo-spring-utils2/#ibm-pcon)
## 方法入参检测工具类
Web 应用在接受表单提交的数据后都需要对其进行合法性检查，如果表单数据不合法，请求将被驳回。类似的，当我们在编写类的方法时，也常常需要对方法入参进行合法性检查，如果入参不符合要求，方法将通过抛出异常的方式拒绝后续处理。举一个例子：有一个根据文件名获取输入流的方法：InputStream getData(String file)，为了使方法能够成功执行，必须保证 file 入参不能为 null 或空白字符，否则根本无须进行后继的处理。这时方法的编写者通常会在方法体的最前面编写一段对入参进行检测的代码，如下所示：
public InputStream getData(String file) {
    if (file == null || file.length() == 0|| file.replaceAll("\\s", "").length() == 0) {
        throw new IllegalArgumentException("file入参不是有效的文件地址");
    }
…
}
类似以上检测方法入参的代码是非常常见，但是在每个方法中都使用手工编写检测逻辑的方式并不是一个好主意。阅读 Spring 源码，您会发现 Spring 采用一个 org.springframework.util.Assert 通用类完成这一任务。
Assert 翻译为中文为“断言”，使用过 JUnit 的读者都熟知这个概念，它断定某一个实际的运行值和预期想一样，否则就抛出异常。Spring 对方法入参的检测借用了这个概念，其提供的 Assert 类拥有众多按规则对方法入参进行断言的方法，可以满足大部分方法入参检测的要求。这些断言方法在入参不满足要求时就会抛出 IllegalArgumentException。下面，我们来认识一下 Assert 类中的常用断言方法：
|断言方法|说明|
|----|----|
|```notNull(Object object)```|当 object 不为 null 时抛出异常，notNull(Object object, String message) 方法允许您通过 message 定制异常信息。和 notNull() 方法断言规则相反的方法是 isNull(Object object)/isNull(Object object, String message)，它要求入参一定是 null；|
|```isTrue(boolean expression) / isTrue(boolean expression, String message)```|当 expression 不为 true 抛出异常；|
|```notEmpty(Collection collection) / notEmpty(Collection collection, String message)```|当集合未包含元素时抛出异常。notEmpty(Map map) / notEmpty(Map map, String message) 和 notEmpty(Object[] array, String message) / notEmpty(Object[] array, String message) 分别对 Map 和 Object[] 类型的入参进行判断；|
|```hasLength(String text) / hasLength(String text, String message)```|当 text 为 null 或长度为 0 时抛出异常；|
|```hasText(String text) / hasText(String text, String message)```|text 不能为 null 且必须至少包含一个非空格的字符，否则抛出异常；|
|```isInstanceOf(Class clazz, Object obj) / isInstanceOf(Class type, Object obj, String message)```|如果 obj 不能被正确造型为 clazz 指定的类将抛出异常；|
|```isAssignable(Class superType, Class subType) / isAssignable(Class superType, Class subType, String message)```|subType 必须可以按类型匹配于 superType，否则将抛出异常；|
使用 Assert 断言类可以简化方法入参检测的代码，如 InputStream getData(String file) 在应用 Assert 断言类后，其代码可以简化为以下的形式：
public InputStream getData(String file){
    Assert.hasText(file,"file入参不是有效的文件地址"); 
    ① 使用 Spring 断言类进行方法入参检测
…
}
可见使用 Spring 的 Assert 替代自编码实现的入参检测逻辑后，方法的简洁性得到了不少的提高。Assert 不依赖于 Spring 容器，您可以大胆地在自己的应用中使用这个工具类。
[回页首](http://www.ibm.com/developerworks/cn/java/j-lo-spring-utils2/#ibm-pcon)
## 小结
本文介绍了一些常用的 Spring 工具类，其中大部分 Spring 工具类不但可以在基于 Spring 的应用中使用，还可以在其它的应用中使用。
对于 Web 应用来说，由于有很多关联的脚本代码，如果这些代码通过拼接字符串的方式动态产生，就需要对动态内容中特殊的字符进行转义处理，否则就有可能产生意想不到的后果。Spring 为此提供了 HtmlUtils 和 JavaScriptUtils 工具类，只要将动态内容在拼接之前使用工具类进行转义处理，就可以避免类似问题的发生了。如果您不介意引入一个第三方类包，那么 jakarta commons lang 通用类包中的 StringEscapeUtils 工具类可能更加适合，因为它提供了更加全面的转义功能。
最后我们还介绍了 Spring 的 Assert 工具类，Assert 工具类是通用性很强的工具类，它使用面向对象的方式解决方法入参检测的问题，您可以在自己的应用中使用 Assert 对方法入参进行检查。
