# javaWeb安全漏洞修复总结 - =朝晖= - 博客园
# [javaWeb安全漏洞修复总结](https://www.cnblogs.com/dhcn/p/7746434.html)
1Web安全介绍**1**
2SQL注入、盲注**1**
2.1SQL注入、盲注概述 1
2.2安全风险及原因 2
2.3AppScan扫描建议 2
2.4应用程序解决方案 4
3会话标识未更新**7**
3.1会话标识未更新概述 7
3.2安全风险及原因分析 7
3.3AppScan扫描建议 8
3.4应用程序解决方案 8
4已解密登录请求**8**
4.1已解密登录请求概述 8
4.2安全风险及原因分析 8
4.3AppScan扫描建议 9
4.4应用程序解决方案 9
5跨站点请求伪造**11**
5.1跨站点请求伪造概述 11
5.2安全风险及原因分析 12
5.3AppScan扫描建议 12
5.4应用程序解决方案 12
6不充分账户封锁**13**
6.1不充分账户封锁概述 13
6.2安全风险及原因分析 13
6.3AppScan扫描建议 13
6.4应用程序解决方案 13
7启用不安全HTTP方法**14**
7.1启用不安全HTTP方法概述 14
7.2安全风险及原因分析 14
7.3AppScan扫描建议 15
7.4应用程序解决方案 15
8HTTP注释敏感信息**16**
8.1HTTP注释敏感信息概述 16
8.2安全风险及原因分析 16
8.3AppScan扫描建议 16
8.4应用程序解决方案 16
9发现电子邮件地址模式**16**
9.1发现电子邮件地址模式概述 16
9.2安全风险及原因分析 17
9.3AppScan扫描建议 17
9.4应用程序解决方案 17
10通过框架钓鱼**20**
10.1通过框架钓鱼概述 20
10.2安全风险及原因分析 20
10.3AppScan扫描建议 20
10.4应用程序解决方案 23
11检查到文件替代版本**25**
11.1检查到文件替代版本概述 25
11.2安全风险及原因分析 25
11.3AppScan扫描建议 25
11.4应用程序解决方案 26
# 1 **W****eb安全介绍**
目前很多业务都依赖于互联网，例如说网上银行、网络购物、网游等，很多恶意攻击者出于不良的目的对Web 服务器进行攻击，想方设法通过各种手段获取他人的个人账户信息谋取利益。正是因为这样，Web业务平台最容易遭受攻击。同时，对Web服务器的攻击也可以说是形形色色、种类繁多，常见的有挂马、SQL注入、缓冲区溢出、嗅探、利用IIS等针对Webserver漏洞进行攻击。
　　一方面，由于TCP/IP的设计是没有考虑安全问题的，这使得在网络上传输的数据是没有任何安全防护的。攻击者可以利用系统漏洞造成系统进程缓冲区溢出，攻击者可能获得或者提升自己在有漏洞的系统上的用户权限来运行任意程序，甚至安装和运行恶意代码，窃取机密数据。而应用层面的软件在开发过程中也没有过多考虑到安全的问题，这使得程序本身存在很多漏洞，诸如缓冲区溢出、SQL注入等等流行的应用层攻击，这些均属于在软件研发过程中疏忽了对安全的考虑所致。
另一方面，用户对某些隐秘的东西带有强烈的好奇心，一些利用木马或病毒程序进行攻击的攻击者，往往就利用了用户的这种好奇心理，将木马或病毒程序捆绑在一些艳丽的图片、音视频及免费软件等文件中，然后把这些文件置于某些网站当中，再引诱用户去单击或下载运行。或者通过电子邮件附件和QQ、MSN等即时聊天软件，将这些捆绑了木马或病毒的文件发送给用户，利用用户的好奇心理引诱用户打开或运行这些文件、
# 2 **SQL注入、盲注**
## 2.1 **SQL注入、盲注概述**
Web 应用程序通常在后端使用数据库，以与企业数据仓库交互。查询数据库事实上的标准语言是 SQL（各大数据库供应商都有自己的不同版本）。Web 应用程序通常会获取用户输入（取自 HTTP 请求），将它并入 SQL 查询中，然后发送到后端数据库。接着应用程序便处理查询结果，有时会向用户显示结果。 
如果应用程序对用户（攻击者）的输入处理不够小心，攻击者便可以利用这种操作方式。在此情况下，攻击者可以注入恶意的数据，当该数据并入 SQL 查询中时，就将查询的原始语法更改得面目全非。例如，如果应用程序使用用户的输入（如用户名和密码）来查询用户帐户的数据库表，以认证用户，而攻击者能够将恶意数据注入查询的用户名部分（和/或密码部分），查询便可能更改成完全不同的数据复制查询，可能是修改数据库的查询，或在数据库服务器上运行 Shell 命令的查询。
## 2.2 **安全风险及原因**
高风险漏洞，攻击者可能会查看、修改或删除数据库条目和表
原因：未对用户输入正确执行危险字符清理
## 2.3 **AppScan扫描建议**
若干问题的补救方法在于对用户输入进行清理。
通过验证用户输入未包含危险字符，便可能防止恶意的用户导致应用程序执行计划外的任务，例如：启动任意 SQL 查询、嵌入将在客户端执行的 Javascript 代码、运行各种操作系统命令，等等。 
建议过滤出所有以下字符：
[1] |（竖线符号）
[2] & （& 符号）
[3];（分号）
[4] $（美元符号）
[5] %（百分比符号）
[6] @（at 符号）
[7] '（单引号）
[8] "（引号）
[9] \'（反斜杠转义单引号）
[10] \"（反斜杠转义引号）
[11] <>（尖括号）
[12] ()（括号）
[13] +（加号）
[14] CR（回车符，ASCII 0x0d）
[15] LF（换行，ASCII 0x0a）
[16] ,（逗号）
[17] \（反斜杠）
以下部分描述各种问题、问题的修订建议以及可能触发这些问题的危险字符：
SQL 注入和 SQL 盲注： 
A. 确保用户输入的值和类型（如 Integer、Date 等）有效，且符合应用程序预期。 
B. 利用存储过程，将数据访问抽象化，让用户不直接访问表或视图。当使用存储过程时，请利用 ADO 命令对象来实施它们，以强化变量类型。 
C. 清理输入以排除上下文更改符号，例如： 
[1] '（单引号）
[2] "（引号）
[3] \'（反斜线转义单引号）
[4] \"（反斜杠转义引号）
[5] )（结束括号）
[6] ;（分号）
跨站点脚本编制：
A. 清理用户输入，并过滤出 JavaScript 代码。我们建议您过滤下列字符： 
[1] <>（尖括号）
[2] "（引号）
[3] '（单引号）
[4] %（百分比符号）
[5] ;（分号）
[6] ()（括号）
[7] &（& 符号）
[8] +（加号）
B. 如果要修订 <%00script> 变体，请参阅 MS 文章 821349 
C. 对于 UTF-7 攻击： [-] 可能的话，建议您施行特定字符集编码（使用 'Content-Type' 头或 <meta> 标记）。 
HTTP 响应分割：清理用户输入（至少是稍后嵌入在 HTTP 响应中的输入）。 
请确保输入未包含恶意的字符，例如：
[1] CR（回车符，ASCII 0x0d）
[2] LF（换行，ASCII 0x0a）远程命令执行：清理输入以排除对执行操作系统命令有意义的符号，例如：
[1] |（竖线符号）
[2] & （& 符号）
[3];（分号）
执行 shell 命令： 
A. 绝不将未检查的用户输入传递给 eval()、open()、sysopen()、system() 之类的 Perl 命令。 
B. 确保输入未包含恶意的字符，例如： 
[1] $（美元符号）
[2] %（百分比符号）
[3] @（at 符号）
XPath 注入：清理输入以排除上下文更改符号，例如： 
[1] '（单引号）
[2] "（引号） 等
LDAP 注入： 
A. 使用正面验证。字母数字过滤（A..Z,a..z,0..9）适合大部分 LDAP 查询。 
B. 应该过滤出或进行转义的特殊 LDAP 字符： 
[1] 在字符串开头的空格或“#”字符
[2] 在字符串结尾的空格字符
[3] ,（逗号）
[4] +（加号）
[5] "（引号）
[6] \（反斜杠）
[7] <>（尖括号）
[8] ;（分号）
[9] ()（括号）
MX 注入： 
应该过滤出特殊 MX 字符： 
[1] CR（回车符，ASCII 0x0d）
[2] LF（换行，ASCII 0x0a）记录伪造：
应该过滤出特殊记录字符：
[1] CR（回车符，ASCII 0x0d）
[2] LF（换行，ASCII 0x0a）
[3] BS（退格，ASCII 0x08）
ORM 注入： 
A. 确保用户输入的值和类型（如 Integer、Date 等）有效，且符合应用程序预期。 
B. 利用存储过程，将数据访问抽象化，让用户不直接访问表或视图。 
C. 使用参数化查询 API 
D. 清理输入以排除上下文更改符号，例如： (*)： 
[1] '（单引号）
[2] "（引号）
[3] \'（反斜线转义单引号）
[4] \"（反斜杠转义引号）
[5] )（结束括号）
[6] ;（分号）
## 2.4 **应用程序解决方案**
1、我们为了调试方便，在页面上会抛出数据库异常信息，如果入侵工具获取了这些信息，就可以获取系统的一些配置信息，如web系统框架、采用的数据库等，从而找出系统漏洞。所以不要在页面上抛出异常的详细信息，这些信息对客户并没有用，只是方便技术人员调试罢了，处理方法是在异常处理页面把打印异常代码删除即可；
2、新建一个过滤器，通过过滤器过滤SQL注入特殊字符，配置成功后，重启服务，用Appsan工具扫描，漏洞得到解决，通过过滤器可以解决SQL注入、跨站点脚本编制及通过框架钓鱼等问题，具体实现方式如下：
**1、在web.xml文件中配置过滤器**
	<filter-mapping>
		<filter-name>requestEncodingFilter</filter-name>
		<url-pattern>/*</url-pattern>
	</filter-mapping>
    <filter>
		<filter-name>InjectFilter</filter-name>
		<filter-class>com.sitech.ismp.util.context.InjectFilter</filter-class>
	</filter>
**2、过滤器过滤代码**
**public****class** InjectFilter **extends** IsmpServletFilter {
**private** String failPage = "/loginout.jsp";//发生注入时，跳转页面
**public****void** doFilter(ServletRequest request,ServletResponse response,
    		FilterChain filterchain)**throws** IOException, ServletException {
	    //判断是否有注入攻击字符
	    HttpServletRequest req = (HttpServletRequest) request;
	 	String inj = injectInput(req);
**if** (!inj.equals("")) {
			 request.getRequestDispatcher(failPage).forward(request, response);
**return**;
	    } **else** {
		    // 传递控制到下一个过滤器
	    	filterchain.doFilter(request, response);
	    }
    }
    /**
     * 判断request中是否含有注入攻击字符
     * **@param** request
     * **@return**
     */
**public** String injectInput(ServletRequest request) {
	    Enumeration e = request.getParameterNames();
	    String attributeName;
	    String attributeValues[];
	    String inj = "";
**while** (e.hasMoreElements()) { 
	    	attributeName = (String)e.nextElement();
	    	//不对密码信息进行过滤，一般密码中可以包含特殊字符
**if**(attributeName.equals("userPassword")||attributeName.equals("confirmPassword")||attributeName.equals("PASSWORD")
	    			||attributeName.equals("password")||attributeName.equals("PASSWORD2")||attributeName.equals("valiPassword")){
**continue**;
	    	}
	    	attributeValues = request.getParameterValues(attributeName);
**for** (**int** i = 0; i < attributeValues.length; i++) {
**if**(attributeValues[i]==**null**||attributeValues[i].equals(""))
**continue**;
	    		inj = injectChar(attributeValues[i]);
**if** (!inj.equals("")) {
**return** inj;
	    		}
	    	}
	    }   
**return** inj;
    }
    /**
     * 判断字符串中是否含有注入攻击字符
     * **@param** str
     * **@return**
     */
**public** String injectChar(String str) {
       String inj_str = "\" ) \' * %";
       String inj_stra[] = inj_str.split(" ");
**for** (**int** i = 0 ; i < inj_stra.length ; i++ )
       {
**if** (str.indexOf(inj_stra[i])>=0)
           {
**return** inj_stra[i];
           }
       }
**return** "";
    }
}
# 3 **会话标识未更新**
## 3.1 **会话标识未更新概述**
“会话固定”是一种攻击技术，会强制用户的会话标识变成显式值。 固定会话标识值的技术有许多种，会随着目标 Web 站点的功能而不同。从利用“跨站点脚本编制”到向 Web 站点密集发出先前生成的 HTTP 请求，都在这些技术范围内。用户的会话标识固定之后，攻击者会等待用户登录，然后利用预定义的会话标识值来假定用户的联机身份。 
一般而言，对于标识值的会话管理系统有两种类型。第一种类型是“宽容”系统，可让 Web 浏览器指定任何标识。第二种类型是“严格”系统，只接受服务器端生成的值。当使用宽容系统时，不需要联系 Web 站点，便可以维护任何会话标识。在严格系统中，攻击者需要维护“陷阱会话”并且必须定期联系 Web 站点，才能防止闲置超时。 对于会话固定，倘若没有活动保护，使用会话来识别已认证的用户的任何 Web 站点都可能受到攻击。使用会话标识的 Web 站点通常都是基于 cookie 的站点，但也会使用 URL 和隐藏的表单字段。不幸的是，基于 cookie 的会话最容易受到攻击。 目前已识别的大多数攻击方法都是针对 cookie 的固定。 相对于在用户登录 Web 站点之后，再窃取用户的会话标识，会话固定提供的机会多得多。 
在用户登录之前，攻击的活动部分便已启动。
会话固定攻击过程通常由三个步骤组成：
1) 安装会话 
攻击者针对目标 Web 站点设下“陷阱会话”，并获取这个会话的标识，攻击者也可以选择攻击中所用的任意会话标识。在某些情况下，必须反复联系 Web 站点，才能维护确定好的陷阱会话值。 
2) 固定会话 
攻击者将陷阱会话值引进用户的浏览器中，固定用户的会话标识。
3) 进入会话 
用户登录目标 Web 站点之后，当使用固定会话标识值时，攻击者便可加以接管。”
**修改**
**对于这类问题解决方案为在用户进入登录页面时清空session让cookie过期**
request.getSession(**true**).invalidate();//清空session
	Cookie cookie = request.getCookies()[0];//获取cookie
	cookie.setMaxAge(0);//让cookie过期
另外一种方式利用JSP的一些特性，不让登录页面产生Session 
<% page session=”false” %>
## 3.2 **安全风险及原因分析**
高风险漏洞，可能会窃取或操纵客户会话和 cookie，它们可能用于模仿合法用户，从而使黑客能够以该用户身份查看或变更用户记录以及执行事务
原因：Web 应用程序编程或配置不安全
## 3.3 **AppScan扫描建议**
始终生成新的会话，供用户成功认证时登录。防止用户操纵会话标识。
请勿接受用户浏览器登录时所提供的会话标识
## 3.4 **应用程序解决方案**
 会话标识未更新，Appscan给出的描述是建议用户每次登录时需使用新的会话标识。应用程序实现上就是在登录模块，添加以下代码，即用户登录后，重新生成会话。
HttpSession session = request.getSession(**false**);
**if**(session!=**null**){ //让cookie过期
		  session.invalidate();
		  Cookie cookie = request.getCookies()[0];//获取cookie
		  cookie.setMaxAge(0);//让cookie过期
}
request.getSession(**true**);//生成新会话
经过测试，这段代码只在weblogic和tomcat下才有效，在公司中间件webspeed及jboss6.0下问题都依然存在，但从扫描的结果信息分析看，漏洞已经解决，分析判断应该只是session处理机制不同，AppScan工具仍认为存在漏洞风险。在与电信沟通中我们存在一个经验教训大家一定要吸取，不能过渡迷信流行的自动化测试工具，尤其是对于Appscan这种判断防御行为的复杂软件，仅靠有限的规则设置就当做是web安全的唯一标准这显然不太合理，这种情况一定要与测试方沟通解释。
另一方面，对于公司的产品webspeed，也想提点建议，商务项目采用公司的产品为公司节约了不少成本，但是我们产品后续升级维护也必须重视起来，当确认出是webspeed本身问题后，联系vasg相关人员进行协调解决，根本没有非常了解该产品技术人员支持，只是一个刚入职的同事在配合测试。调试了一周时间仍不能解决，最后只能作为一个遗留问题搁置。公司一直在向产品化转变，但是自身的产品维护、升级、管理仍然需要改进。
# 4 **已解密登录请求**
## 4.1 **已解密登录请求概述**
在应用程序测试过程中，检测到将未加密的登录请求发送到服务器。由于登录过程所用的部分输入字段（例如：用户名、密码、电子邮件地址、社会保险号码，等等）是个人敏感信息，建议通过加密连接（如 SSL）将其发送到服务器。任何以明文传给服务器的信息都可能被窃，稍后可用来电子欺骗身份或伪装用户。 此外，若干隐私权法规指出，用户凭证之类的敏感信息一律以加密方式传给网站。 
## 4.2 **安全风险及原因分析**
安全风险中，可能会窃取诸如用户名和密码等未经加密即发送了的用户登录信息
原因：诸如用户名、密码和信用卡号之类的敏感输入字段未经加密即进行了传
递
## 4.3 **AppScan扫描建议**
1. 确保所有登录请求都以加密方式发送到服务器。 
2. 请确保敏感信息，例如： 
- 用户名
- 密码
- 社会保险号码
- 信用卡号码
- 驾照号码
- 电子邮件地址
- 电话号码
- 邮政编码
一律以加密方式传给服务器。
## 4.4 **应用程序解决方案**
 已解密的登录请求，要求就是数据要加密传输。最简单有效的解决方式采用SSL加密协议传输，但是由于EMA服务管理平台业务的特殊性，采用SSL加密方式对现有的业务影响太大，所以最终没有采用此种方式解决该问题，但个人在进行测试过程中也尝试在tomcat和jboss下SSL方式配置，写下来供参考。
Jboss内核也是tomcat，所以两者配置基本都是一样，都是在生成证书文件后，在service.xml 进行配置：
- 进入到cmd 进入到jdk bin目录下执行keytool -genkey -alias tomcat -keyalg RSA -keystore webspeed.keystore 生成证书
- 在service.xml配置SSL
    <Connector port="8443" maxHttpHeaderSize="8192"
               maxThreads="150" minSpareThreads="25" maxSpareThreads="75"
               enableLookups="false" disableUploadTimeout="true"
               acceptCount="100" scheme="https" secure="true"
               clientAuth="false" sslProtocol="TLS" 
               keystoreFile="C:\tomcat-5.5.26\conf\webspeed.keystore"  keystorePass="1111aaaa"/> 
这样配置后虽然可以通过https访问，但仍然还可以通过8080使用普通的http访问，所以还必须禁止普通模式登录。所以还得在web.xml添加配置。
|01020304050607080910111213141516171819202122232425262728293031|<security-constraint> <!-- Authorization setting for SSL --> <web-resource-collection > <web-resource-name >SSL</web-resource-name> <url-pattern>*.jsp</url-pattern> <url-pattern>*.action</url-pattern> </web-resource-collection> <user-data-constraint> <transport-guarantee>CONFIDENTIAL</transport-guarantee> </user-data-constraint> </security-constraint> <login-config> <!-- Authorization setting for SSL --> <auth-method>CLIENT-CERT</auth-method> <realm-name>Client Cert Users-only Area</realm-name> </login-config>|
 应注意，由于项目的一些组件无法通过https，因此url-pattern字段只对.jsp和.action进行了限制，如果不做特定限制，则系统默认是全部使用https传输。而且上述设置一旦在某个工程中出现，那么当前tomcat将全局采用这一配置。
# 5 **跨站点请求伪造**
## 5.1 **跨站点请求伪造概述**
“跨站点伪造请求 (CSRF)”攻击可让黑客以受害者的名义在易受攻击的站点上运行操作。当易受攻击的站点未适当验证请求来源时，便可能出现这个攻击。这个漏洞的严重性取决于受影响的应用程序的功能，例如，对搜索页面的 CSRF 攻击，严重性低于对转帐页面或概要更新页面的 CSRF 攻击。 
这项攻击的执行方式，是强迫受害者的浏览器向易受攻击的站点发出 HTTP 请求。如果用户目前已登录受害者站点，请求会自动使用用户的凭证（如会话 Cookie、用户的 IP 地址，以及其他浏览器认证方法）。攻击者利用这个方法来伪造受害者的身份，再代替他来提交操作。换句话来说，易受攻击的站点未采取适当措施来验证用户实际是否想执行特定操作。 
强迫受害者发送非预期的请求，方法有许多种：
- 通过电子邮件向受害者发送易受攻击应用程序的恶意链接。 - 在黑客的 Web 页面上，放置一个易受攻击的 Web 站点的热链接（如图像或帧）。 - 在公共论坛中，张贴易受攻击站点的链接。 
- 利用站点（或另一个站点）的“跨站点脚本编制”或“链接注入”漏洞，将浏览器自动重定向到易受攻击的站点。 
如果攻击者利用易受攻击的站点本身的“链接注入”漏洞，可以增加用户通过站点认证的可能性，进而增加攻击成功的可能性。 
例如，攻击者可以利用上述任何选项来诱惑受害者查看含有下列条目的页面：
<img src="http://bank/transfer?destination=John&money=1000" style='visibility:hidden'> 
这会使受害者的浏览器自动请求 URL 及浏览器的当前凭证。如果这个银行业站点易受到 CSRF 攻击，它会根据应用程序逻辑，从受害者的帐户中，将 1000 美元转账到 John 的银行帐户。“跨站点伪造请求”攻击也称为 CSRF（发音为 C-Serf）、XSRF、“跨站点伪造引用”、“单键攻击”以及“会话骑乘”。 
您可以利用下列方式来验证您的应用程序是否易受到 CSRF 攻击： 
[1] 检查易受攻击的链接/请求是否未包括攻击者难以猜中的参数 
[2] 检查易受攻击的链接/请求是否会执行只应自愿执行的操作 
含有用户在不知不觉中提交的请求所能直接访问的敏感操作的应用程序，被视为很容易遭受 CSRF 攻击。CSRF 也可能出现在登录页面和注销页面上。由于攻击者可以伪造来自受害者的连续注销请求，因此 CSRF 可能导致服务拒绝。在登录页面上，CSRF 可以允许攻击者使用包含攻击者用户名和密码的伪造请求来将客户机登录到攻击者的账户中。登录 CSRF 攻击会带有严重的后果，这取决于其他站点行为。例如，如果站点保留了用户操作的历史记录（例如搜索历史记录），那么攻击者将能够在易受攻击的站点上查看受害者之前执行的操作。
## 5.2 **安全风险及原因分析**
安全风险中，可能会窃取或操纵客户会话和 cookie，它们可能用于模仿合法用户，从而使黑客能够以该用户身份查看或变更用户记录以及执行事务
原因：应用程序使用的认证方法不充分
## 5.3 **AppScan扫描建议**
如果要避免 CSRF 攻击，每个请求都应该包含唯一标识，它是攻击者所无法猜测的参数。建议的选项之一是添加取自会话 cookie 的会话标识，使它成为一个参数。服务器必须检查这个参数是否符合会话 cookie，若不符合，便废弃请求。 攻击者无法猜测这个参数的原因是应用于 cookie 的“同源策略”，因此，攻击者无法伪造一个虚假的请求，让服务器误以为真。攻击者难以猜测且无法访问的任何秘密（也就是无法从其他域访问），都可用来替换会话标识。 这可以防止攻击者设计看似有效的请求。 
## 5.4 **应用程序解决方案**
 已解密的登录请求，要求就是数据要加密传输。最简单有效的解决方式采用SSL加密协议传输，但是由于EMA服务管理平台业务的特殊性，采用SSL加密方式对现有的业务影响太大，所以最终没有采用此种方式解决该问题，但个人在进行测试过程中也尝试在tomcat和jboss下SSL方式配置，写下来供参考。
# 6 **不充分账户封锁**
## 6.1 **不充分账户封锁概述**
蛮力攻击是指恶意用户发送大量可能的密码和/或用户名以访问应用程序的尝试。 由于该技术包含大量登录尝试，未限制允许的错误登录请求次数的应用程序很容易遭到这类攻击。 因此，强烈建议您对帐户限制允许的错误登录尝试次数，超过该次数，便锁定该帐户。样本利用： 
下列请求说明密码猜测请求：
http://site/login.asp?username=EXISTING_USERNAME&password=GUESSED_PASSWORD 如果站点在若干次错误尝试之后并不锁定测试的帐户，攻击者最终可能会发现帐户密码，并使用它来假冒帐户的合法用户。
## 6.2 **安全风险及原因分析**
安全风险高，可能会升级用户特权并通过 Web 应用程序获取管理许可权
原因：Web 应用程序编程或配置不安全
## 6.3 **AppScan扫描建议**
请确定允许的登录尝试次数（通常是 3-5 次），确保超出允许的尝试次数之后，便锁定帐户。 为了避免真正的用户因帐户被锁定而致电支持人员的麻烦，可以仅临时性暂挂帐户活动，并在特定时间段之后启用帐户。帐户锁定大约 10 分钟，通常便足以阻止蛮力攻击。 
## 6.4 **应用程序解决方案**
根据扫描建议，web应用程序设定允许登录尝试次数，登录连续失败超过设定次数，就锁定用户，失败次数灵活配置。
在用户登录时进行验证：
**if** (!encrypter.encrypt(userPassword).equalsIgnoreCase(
				user.getLOGIN_PASSWD() == **null** ? "" : user.getLOGIN_PASSWD())) 
{
			//更新此用户登录失败次数
**this**.updateLoginFailTimes(userCode);
			//如果用户连续登录失败次数超过配置值则将其锁定
**int** loginLockTimes=**this**.getLoginLockTimes();
**if**(**this**.getLoginFailTimes(userCode)>=loginLockTimes){
**this**.lockUser(userCode);
			}
**throw****new** MySecurityException("密码不正确! 用户:" + userCode);
		}
# 7 **启用不安全HTTP方法**
## 7.1 **启用不安全HTTP方法概述**
似乎 Web 服务器配置成允许下列其中一个（或多个）HTTP 方法（动词）： 
- DELETE 
- SEARCH 
- COPY 
- MOVE 
- PROPFIND 
- PROPPATCH 
- MKCOL 
- LOCK 
- UNLOCK 
这些方法可能表示在服务器上启用了 WebDAV，可能允许未授权的用户对其进行利用。
## 7.2 **安全风险及原因分析**
原因：Web 服务器或应用程序服务器是以不安全的方式配置的
## 7.3 **AppScan扫描建议**
如果服务器不需要支持 WebDAV，请务必禁用它，或禁止不必要的 HTTP 方法（动词）。 
## 7.4 **应用程序解决方案**
修改web工程中web.xml,增加安全配置信息，禁用不必要HTTP方法
   <security-constraint>  
   <web-resource-collection>  
   	<web-resource-name>HtmlAdaptor</web-resource-name>  
  	 	<description>test</description>  
   		<url-pattern>*.jsp</url-pattern>
   		<url-pattern>*.do</url-pattern>  
   		<http-method>GET</http-method>  
   		<http-method>POST</http-method>
   		<http-method>PUT</http-method>  
		<http-method>DELETE</http-method>  
		<http-method>HEAD</http-method>  
		<http-method>OPTIONS</http-method>  
		<http-method>TRACE</http-method>    
   </web-resource-collection><!--  
   <auth-constraint>  
   		<role-name>JBossAdmin</role-name>  
   </auth-constraint>  
   --></security-constraint>。
# 8 **HTTP注释敏感信息**
## 8.1 **HTTP注释敏感信息概述**
很多 Web 应用程序程序员使用 HTML 注释，以在需要时帮助调试应用程序。尽管添加常规注释有助于调试应用程序，但一些程序员往往会遗留重要数据（例如：与 Web 应用程序相关的文件名、旧的链接或原非供用户浏览的链接、旧的代码片段等）。
## 8.2 **安全风险及原因分析**
安全风险低，能会收集有关 Web 应用程序的敏感信息，如用户名、密码、机器名和/或敏感文件位置
原因：程序员在 Web 页面上留下调试信息
## 8.3 **AppScan扫描建议**
[1] 请勿在 HTML 注释中遗留任何重要信息（如文件名或文件路径）。 
[2] 从生产站点注释中除去以前（或未来）站点链接的跟踪信息。 
[3] 避免在 HTML 注释中放置敏感信息。 
[4] 确保 HTML 注释不包括源代码片段。 
[5] 确保程序员没有遗留重要信息。 
。
## 8.4 **应用程序解决方案**
虽然这个漏洞为低级别漏洞，但电信方也是要求必须修复，要修改此漏洞需要检查工程中的每一个jsp页面，工作量还是挺大。所以在后续开发过程中注释尽量写英文注释，尽量不要遗留敏感注释信息在jsp代码中，养成良好的编码习惯才是解决问题根本。
# 9 **发现电子邮件地址模式**
## 9.1 **发现电子邮件地址模式概述**
Spambot 搜寻因特网站点，开始查找电子邮件地址来构建发送自发电子邮件（垃圾邮件）的邮件列表。 AppScan 检测到含有一或多个电子邮件地址的响应，可供利用以发送垃圾邮件。 而且，找到的电子邮件地址也可能是专用电子邮件地址，对于一般大众应是不可访问的。
## 9.2 **安全风险及原因分析**
安全风险低，能会收集有关 Web 应用程序的敏感信息，如用户名、密码、机器名和/或敏感文件位置
原因：Web 应用程序编程或配置不安全
## 9.3 **AppScan扫描建议**
从 Web 站点中除去任何电子邮件地址，使恶意的用户无从利用。
## 9.4 **应用程序解决方案**
根据扫描建议删除注释中出现email地址信息，如果页面中要显示mail地址转为图片形式展示。如：ema服务管理平台首页需要展示客户联系方式，并且联系方式、email等信息，这些信息用户都是可以自行修改的，因为包含了email地址，所以联系方式就转为图片形式：
<%@ page language="java" contentType="text/html; charset=gb2312"%>
<%@ include file="/common/taglib.jsp" %>
<%@ include file="/common/chart.jsp" %>
<%@ page import="java.util.List,java.util.*,java.awt.*,java.awt.image.*,com.sun.image.codec.jpeg.*,java.util.*" %>
<%@ page import="com.sitech.ismp.informationService.publish.dao.TB_SYS_SUPPORT_STAFFDao" %>
<html:html locale="true">
<head>  
<%@ include file="/common/link.jsp" %> 
<%@ page import="com.sitech.ismp.util.context.CommUtil" %>
</head>
<link href="/css/style.css" rel="stylesheet" type="text/css">
<script type="text/javascript" src="/js/pub.js"></script>
<link rel="stylesheet" type="text/css" media="all" href="/css/calendar-win2k-cold-1.css" title="win2k-cold-1" />
<body>
<html:form action="/homeContactShow" method="post" styleId="theForm">
 <bean:define id="theForm" name="supportStaffForm"/>
 <DIV class="bg" id=left>
 <div>
<table width="100%" border="0" cellpadding="0" cellspacing="0" class="tablebg">
<TBODY>
		<tr>
        <td class="tableheadbg">运营中心联系方式</td>
      </tr>
      <tr>
      <td class="tablewhitebg"> 
     	<%  
   			List typeList = (List)request.getAttribute("typeList");
   			out.clear();
   			out = pageContext.pushBody(); 
        	response.setContentType("image/jpeg");
         	response.addHeader("pragma","NO-cache");
         	response.addHeader("Cache-Control","no-cache");
         	response.addDateHeader("Expries",0);
**int** rowheight=20;	 
**int** width=135,height=rowheight*typeList.size();		 
   			TB_SYS_SUPPORT_STAFFDao dao= **new** TB_SYS_SUPPORT_STAFFDao();
   			String ty= "";
   			String mob="";
**for**(**int** i=0;i<typeList.size();i++){
   				HashMap hm=(HashMap)typeList.get(i);
   			 	ty=(String)hm.get("TYPE_ID");
   				List sta =(List)dao.findSupportStaffByTypeId(ty);
**for**(**int** k=0;k<sta.size();k++){
   					HashMap map = (HashMap)sta.get(k);
   					mob = (String)map.get("MOBILE");
   					height+=3*rowheight;
**if**(mob!=**null**)height+=rowheight;
   				}
   			}
   			BufferedImage image = **new** BufferedImage(width, height, BufferedImage.TYPE_INT_RGB);
   			Graphics g = image.getGraphics();
         	g.setColor(Color.white);
         	g.fillRect(0, 0, width, height);
         	g.setColor(Color.BLUE);
         	Font font=**new** Font("宋体",Font.PLAIN,13);
         	g.setFont(font);
**int** row=0;
   			String typeid = "";
   			String typename="";
   			String name = "";
   			String tel="";
   			String mail = "";
   			String mobile="";
**for**(**int** i=0;i<typeList.size();i++){
   			HashMap hm=(HashMap)typeList.get(i);
   			 typeid=(String)hm.get("TYPE_ID");
   			 typename=(String)hm.get("TYPE_NAME");
   			 row++;
   			 g.drawString(typename,0,(row-1)*rowheight+10);
   			List staffs =(List)dao.findSupportStaffByTypeId(typeid);
**for**(**int** k=0;k<staffs.size();k++){
   				HashMap map = (HashMap)staffs.get(k);
   				name = (String)map.get("NAME");
   				 tel = (String)map.get("TEL");
   				 mail = (String)map.get("MAIL");
   				 mobile = (String)map.get("MOBILE");
   				 row++;
   				 g.drawString(name+":"+tel,0,(row-1)*rowheight+10);
   				 row++;
   				 g.drawString(mail,0,(row-1)*rowheight+10);
**if**(mobile !=**null**){
   				 	row++;
   				 	g.drawString(mail,0,(row-1)*rowheight+10);
   				 }
           }
           }
           g.dispose();
           ServletOutputStream outStream = response.getOutputStream();
         	JPEGImageEncoder encoder =JPEGCodec.createJPEGEncoder(outStream);
         	encoder.encode(image);
         	outStream.close();           
            %>
        </td>
      </tr>
 </TBODY>   
 </table>
 </div>
 </DIV>  
</html:form>
</body>
</html:html>
# 10 **通过框架钓鱼**
## 10.1 **通过框架钓鱼****概述**
网络钓鱼是一个通称，代表试图欺骗用户交出私人信息，以便电子欺骗身份。
攻击者有可能注入 frame 或 iframe 标记，其中含有类似受攻击之网站的恶意属性。不小心的用户有可能浏览它，但并不知道他正在离开原始网站，冲浪到恶意的网站。之后，攻击者便可以诱惑用户重新登录，然后获取他的登录凭证。 
伪造的网站嵌入在原始网站中，这个情况对攻击者有帮助，因为他的网络钓鱼企图会披上更可信赖的外表。
样本利用：
如果参数值未经适当清理，便反映在响应中，下列请求：http://[SERVER]/script.aspx?parameter=<frame name="evil" src="www.evil.com"> 
会使响应含有通往这个邪恶站点的框架。
## 10.2 **安全风险及原因分析**
安全风险中，可能会劝说初级用户提供诸如用户名、密码、信用卡号、社会保险号等敏感信息
原因：对用户输入正确执行危险字符清理
## 10.3 **AppScan扫描建议**
若干问题的补救方法在于对用户输入进行清理。
通过验证用户输入未包含危险字符，便可能防止恶意的用户导致应用程序执行计划外的任务，例如：启动任意 SQL 查询、嵌入将在客户端执行的 Javascript 代码、运行各种操作系统命令，等等。 
建议过滤出所有以下字符：
[1] |（竖线符号）
[2] & （& 符号）
[3];（分号）
[4] $（美元符号）
[5] %（百分比符号）
[6] @（at 符号）
[7] '（单引号）
[8] "（引号）
[9] \'（反斜杠转义单引号）
[10] \"（反斜杠转义引号）
[11] <>（尖括号）
[12] ()（括号）
[13] +（加号）
[14] CR（回车符，ASCII 0x0d）
[15] LF（换行，ASCII 0x0a）
[16] ,（逗号）
[17] \（反斜杠）
以下部分描述各种问题、问题的修订建议以及可能触发这些问题的危险字符：
SQL 注入和 SQL 盲注： 
A. 确保用户输入的值和类型（如 Integer、Date 等）有效，且符合应用程序预期。 
B. 利用存储过程，将数据访问抽象化，让用户不直接访问表或视图。当使用存储过程时，请利用 ADO 命令对象来实施它们，以强化变量类型。 
C. 清理输入以排除上下文更改符号，例如： 
[1] '（单引号）
[2] "（引号）
[3] \'（反斜线转义单引号）
[4] \"（反斜杠转义引号）
[5] )（结束括号）
[6] ;（分号）
跨站点脚本编制：
A. 清理用户输入，并过滤出 JavaScript 代码。我们建议您过滤下列字符： 
[1] <>（尖括号）
[2] "（引号）
[3] '（单引号）
[4] %（百分比符号）
[5] ;（分号）
[6] ()（括号）
[7] &（& 符号）
[8] +（加号）
B. 如果要修订 <%00script> 变体，请参阅 MS 文章 821349 
C. 对于 UTF-7 攻击： [-] 可能的话，建议您施行特定字符集编码（使用 'Content-Type' 头或 <meta> 标记）。 
HTTP 响应分割：清理用户输入（至少是稍后嵌入在 HTTP 响应中的输入）。 
请确保输入未包含恶意的字符，例如：
[1] CR（回车符，ASCII 0x0d）
[2] LF（换行，ASCII 0x0a）远程命令执行：清理输入以排除对执行操作系统命令有意义的符号，例如：
[1] |（竖线符号）
[2] & （& 符号）
[3];（分号）
执行 shell 命令： 
A. 绝不将未检查的用户输入传递给 eval()、open()、sysopen()、system() 之类的 Perl 命令。 
B. 确保输入未包含恶意的字符，例如： 
[1] $（美元符号）
[2] %（百分比符号）
[3] @（at 符号）
XPath 注入：清理输入以排除上下文更改符号，例如： 
[1] '（单引号）
[2] "（引号） 等
LDAP 注入： 
A. 使用正面验证。字母数字过滤（A..Z,a..z,0..9）适合大部分 LDAP 查询。 
B. 应该过滤出或进行转义的特殊 LDAP 字符： 
[1] 在字符串开头的空格或“#”字符
[2] 在字符串结尾的空格字符
[3] ,（逗号）
[4] +（加号）
[5] "（引号）
[6] \（反斜杠）
[7] <>（尖括号）
[8] ;（分号）
[9] ()（括号）
MX 注入： 
应该过滤出特殊 MX 字符： 
[1] CR（回车符，ASCII 0x0d）
[2] LF（换行，ASCII 0x0a）记录伪造：
应该过滤出特殊记录字符：
[1] CR（回车符，ASCII 0x0d）
[2] LF（换行，ASCII 0x0a）
[3] BS（退格，ASCII 0x08）
ORM 注入： 
A. 确保用户输入的值和类型（如 Integer、Date 等）有效，且符合应用程序预期。 
B. 利用存储过程，将数据访问抽象化，让用户不直接访问表或视图。 
C. 使用参数化查询 API 
D. 清理输入以排除上下文更改符号，例如： (*)： 
[1] '（单引号）
[2] "（引号）
[3] \'（反斜线转义单引号）
[4] \"（反斜杠转义引号）
[5] )（结束括号）
[6] ;（分号）
(*) 这适用于 SQL。高级查询语言可能需要不同的清理机制。
。
## 10.4 **应用程序解决方案**
新建一个过滤器，通过过滤器过滤SQL注入特殊字符，配置成功后，重启服务，用Appsan工具扫描，漏洞得到解决，具体实现方式如下：
**1、在web.xml文件中配置过滤器**
	<filter-mapping>
		<filter-name>requestEncodingFilter</filter-name>
		<url-pattern>/*</url-pattern>
	</filter-mapping>
    <filter>
		<filter-name>InjectFilter</filter-name>
		<filter-class>com.sitech.ismp.util.context.InjectFilter</filter-class>
	</filter>
**2、过滤器过滤代码**
**public****class** InjectFilter **extends** IsmpServletFilter {
**private** String failPage = "/loginout.jsp";//发生注入时，跳转页面
**public****void** doFilter(ServletRequest request,ServletResponse response,
    		FilterChain filterchain)**throws** IOException, ServletException {
	    //判断是否有注入攻击字符
	    HttpServletRequest req = (HttpServletRequest) request;
	 	String inj = injectInput(req);
**if** (!inj.equals("")) {
			 request.getRequestDispatcher(failPage).forward(request, response);
**return**;
	    } **else** {
		    // 传递控制到下一个过滤器
	    	filterchain.doFilter(request, response);
	    }
    }
    /**
     * 判断request中是否含有注入攻击字符
     * **@param** request
     * **@return**
     */
**public** String injectInput(ServletRequest request) {
	    Enumeration e = request.getParameterNames();
	    String attributeName;
	    String attributeValues[];
	    String inj = "";
**while** (e.hasMoreElements()) { 
	    	attributeName = (String)e.nextElement();
	    	//不对密码信息进行过滤，一般密码中可以包含特殊字符
**if**(attributeName.equals("userPassword")||attributeName.equals("confirmPassword")||attributeName.equals("PASSWORD")
	    			||attributeName.equals("password")||attributeName.equals("PASSWORD2")||attributeName.equals("valiPassword")){
**continue**;
	    	}
	    	attributeValues = request.getParameterValues(attributeName);
**for** (**int** i = 0; i < attributeValues.length; i++) {
**if**(attributeValues[i]==**null**||attributeValues[i].equals(""))
**continue**;
	    		inj = injectChar(attributeValues[i]);
**if** (!inj.equals("")) {
**return** inj;
	    		}
	    	}
	    }   
**return** inj;
    }
    /**
     * 判断字符串中是否含有注入攻击字符
     * **@param** str
     * **@return**
     */
**public** String injectChar(String str) {
       String inj_str = "\" ) \' * %";
       String inj_stra[] = inj_str.split(" ");
**for** (**int** i = 0 ; i < inj_stra.length ; i++ )
       {
**if** (str.indexOf(inj_stra[i])>=0)
           {
**return** inj_stra[i];
           }
       }
**return** "";
    }
}
# 11 **检查到文件替代版本**
## 11.1 **检查到文件替代版本概述**
开发者往往会将脚本文件的备用版本留在虚拟根目录中，例如，开头是“Copy of”、“_”、“.”、“~”和“Old”的文件。当请求这些文件时，它们会显示在浏览器中。这些文件可能包含现有脚本的备用版本或旧版本。 
请务必从虚拟目录下除去这些文件，因为它们可能包含用于调试的敏感信息，也可能指向通过当前脚本不可查看的站点区域。
## 11.2 **安全风险及原因分析**
安全风险低，能会收集有关 Web 应用程序的敏感信息，如用户名、密码、机器名和/或敏感文件位置
原因：在生产环境中留下临时文件
## 11.3 **AppScan扫描建议**
请勿将文件的备用版本存放在虚拟 Web 服务器的根目录下。相反地，当更新站点时，请将文件删除或移动到虚拟根目录以外的目录、在这个目录中编辑文件，然后再将文件移动（或拷贝）回虚拟根目录。请确保，在虚拟根目录下，只有实际在使用的文件 。
## 11.4 **应用程序解决方案**
这个问题反映出我们编码习惯和文档版本管理方面的问题，从我们工程中，可以看到历史遗留的一些“copy of”、”_”开头jsp和java文件，甚至有些正式的文件也有命名成例如：a1.jsp,a2.jsp的，这些明显不符合文件的命名规范，也会给系统带来一些潜在漏洞，所以开发人员一定要注意命名规范性和历史版本信息清除，养成良好的编码习惯。
写在最后：这是最近从过往文件存档里面找到的一份12年的文档，是不是自己写的，也记不清了。share出来

