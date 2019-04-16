# Java学习笔记26：webx安全配置问题 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年01月06日 15:13:17[initphp](https://me.csdn.net/initphp)阅读数：1755
所属专栏：[Java学习笔记](https://blog.csdn.net/column/details/javago.html)









1、默认拦截器

即使你不加说明，BasicRequestContext也总是会启用一个默认的interceptor实

现：ResponseHeaderSecurityFilter。这个类实现了下列功能：

• 避免header name和value中出现CRLF字符 ── 在header中嵌入CRLF（回车换行）字符

是一种常见的攻击手段。攻击者嵌入CRLF以后，使服务器对HTTP请求发生错误判断，从而执行攻击者的恶意代码。事实上，现在的servlet引擎如tomcat已经可以防御这种攻击。但作为框架，并不能依赖于特定的servlet引擎，所以加上这个额外的安全检查，确保万无一失。

• 将status message用HTML entity编码重写 ── 通常status message会被显示在HTML页面中。攻击者可以利用这一点在页面中嵌入恶意代码。将status message以HTML entity编码重写以后，就可以避免这个问题。

• 限制cookie的总大小 ── 过大的cookie可能使WEB服务器拒绝响应请求。攻击者同样可以利用这一点使用户无法正常访问网站。限制cookie的总大小可以部分地解决这种危机。

如果需要，你可以对ResponseHeaderSecurityFilter指定一些参数。

例 7.2. 配置ResponseHeaderSecurityFilter（/WEB-INF/webx.xml）

<request-contexts:interceptors

xmlns="http://www.alibaba.com/schema/services/request-contexts/basic/interceptors">

<interceptor class="...Interceptor1" />

<interceptor class="...Interceptor2" />

<response-header-security-filter maxSetCookieSize="5K" />

</request-contexts:interceptors>

2、过滤参数

出于安全的考虑，<parser>还支持对输入参数进行过滤。请看示例：

例 7.31. 配置过滤参数
- <parser>
- 
- <filters>
- 
- <parser-filters:uploaded-file-whitelistextensions="jpg, gif, png"/>
- 
- </filters>
- 
- </parser>

上面的配置将会禁止文件名后缀不在列表中的文件被上传到服务器上。如果做得更好一点，你甚至可以对上传文件进行病毒扫描。

目前，<parser>支持两种过滤器接口：ParameterValueFilter和UploadedFileFilter。前者用来对普通的参数值进行过滤（例如排除可能造成攻击的HTML代码）；后者用来对上传文件的file item对象进行过滤，就像刚才的uploaded-file-whitelist的例子。

3、增加Fasttext的宏

在webx-component-and-root.xml中配置
- <!-- Template渲染服务。 -->
- <services:template
- xmlns="http://www.alibaba.com/schema/services/template/engines"
- searchExtensions="true">
- <velocity-enginetemplateEncoding="GBK"
- strictReference="false"path="/${component}/templates">
- <global-macros>
- <name>global/*Macros.vm</name>
- <name>global/*macros.vm</name>
- <name>classpath/unictemplate/macros.vm</name>
- </global-macros>
- <plugins>
-                 <!-- <vm-plugins:escape-supportdefaultEscape="html">
- <vm-plugins:noescape>
- <vm-plugins:if-matchespattern="^control\."/>
- <vm-plugins:if-matchespattern="^screen_placeholder"/>
- <vm-plugins:if-matchespattern="^stringEscapeUtil\.escape"/>
- <vm-plugins:if-matchespattern="^csrfToken\.(get)?hiddenField"/>
- </vm-plugins:noescape>
- </vm-plugins:escape-support> -->
- <vm-plugins:fasttext-support/>
- <vm-plugins:renderable-support/>
- </plugins>
- </velocity-engine>

注意：

(1)b2b使用的是fasttext解决方案，而不用eacape，所以添加<vm-plugins:fasttext-support/>

(2)为了解决二房库的宏引用问题，使用<name>classpath/unictemplate/macros.vm</name>进行配置

顺便说说资源的load，在webx-component-and-root.xml中配置的资源，例如：
- <global-macros>
- <name>global/*Macros.vm</name>
- <name>global/*macros.vm</name>
- <name>classpath/unictemplate/macros.vm</name>
- </global-macros>

他们都是在中相应的classloader去加载的，这些loader在resources.xml中进行了说明，比如：res-loaders:classpath-loader， res-loaders:webapp-loader



