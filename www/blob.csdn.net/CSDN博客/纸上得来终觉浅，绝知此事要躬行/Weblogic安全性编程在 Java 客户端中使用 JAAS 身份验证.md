# Weblogic安全性编程在 Java 客户端中使用 JAAS 身份验证 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年01月06日 14:13:59[boonya](https://me.csdn.net/boonya)阅读数：2222








原文地址：[http://edocs.weblogicfans.net/wls/docs92/security/fat_client.html](http://edocs.weblogicfans.net/wls/docs92/security/fat_client.html)




本部分包含下列主题：

- [JAAS
 和 WebLogic Server](http://edocs.weblogicfans.net/wls/docs92/security/fat_client.html#wp1048814)
- [JAAS
 身份验证开发环境](http://edocs.weblogicfans.net/wls/docs92/security/fat_client.html#wp1029274)
- [使用
 JAAS 身份验证编写客户端应用程序](http://edocs.weblogicfans.net/wls/docs92/security/fat_client.html#wp1029379)
- [使用
 JNDI 身份验证](http://edocs.weblogicfans.net/wls/docs92/security/fat_client.html#wp1033403)
- [Java
 客户端 JAAS 身份验证代码示例](http://edocs.weblogicfans.net/wls/docs92/security/fat_client.html#wp1048487)


这些部分引用了可从 BEA 的 [dev2dev](http://dev.bea.com/code) 网站联机获取的示例代码，WebLogic
 Server 分发的以下位置也包括了这些示例代码：

SAMPLES_HOME`\server\examples\src\examples\security\jaas`

`jaas` 目录中包含 `instructions.html` 文件、`ant` 生成文件、`sample_jaas.config` 文件和下列
 Java 文件：

- `BaseClient.java`
- `BaseClientConstants.java`
- `SampleAction.java`
- `SampleCallbackHandler.java`
- `SampleClient.java`
- `TradeResult.java`
- `TraderBean.java`


阅读以下各部分中的信息时，需要查看示例。



## JAAS 和 WebLogic Server

Java 身份验证和授权服务（Java Authentication and Authorization Service，简称 JAAS）是 J2SE Development Kit 5.0 中安全的标准扩展。JAAS 提供强制访问基于用户标识的控件的功能。在 WebLogic Server 中，提供 JAAS 作为 JNDI 身份验证机制的备用服务。

WebLogic Server 客户端只使用标准 JAAS 的身份验证部分。JAAS LoginContext 支持顺序执行所有已配置身份验证提供程序 LoginModule 实例，并负责管理每个已配置提供程序的完成状态。

对 Java 客户端使用 JAAS 身份验证时，注意下列注意事项：

- WebLogic Server 客户端可以使用 JNDI 登录进行身份验证，也可以使用 JAAS 登录进行身份验证，但是 JAAS 登录是首选方法。
- 虽然 JAAS 是身份验证的首选方法，但是 WebLogic 提供的 LoginModule (`weblogic.security.auth.login.UsernamePasswordLoginModule`)
 只支持用户名和密码身份验证。因此，对于客户端证书身份验证（也称作双向 SSL 身份验证），应该使用 JNDI。要使用 JAAS 进行客户端证书身份验证，您必须编写进行证书身份验证的自定义 LoginModule。|**注意：**|如果您编写自己的 LoginModule 用于 WebLogic Server 客户端，则使其调用 `weblogic.security.auth.Authenticate.authenticate()`可执行登录。|
|----|----|

- 要从远程 Java 客户端（即，Java 客户端不是 WebLogic Server 客户端）执行 JAAS 登录，则可使用 WebLogic 提供的 LoginModule 执行登录。但是，如果选择不使用 WebLogic 提供的 LoginModule，而决定自己编写
 LoginModule，则必须使用自己编写的 LoginModule 调用 `weblogic.security.auth.Authenticate.authenticate()`方法以执行登录。
- 如果使用的是远程登录系统或周边登录系统，例如安全声明标记语言 (SAML)，则不需要调用 `weblogic.security.auth.Authenticate.authenticate()`。如果您正在使用
 WebLogic Server 执行登录，则只需要调用`authenticate()`方法。|**注意：**|WebLogic Server 全面支持 JAAS 身份验证的容器，并且支持在应用程序代码中完全使用 JAAS 身份验证和授权。|
|----|----|

- 在 WebLogic Server 中调用 JAAS 以执行登录。每个身份验证提供程序都包含一个 LoginModule。对于通过 JNDI或 JAAS 进行 servlet 登录和 Java 客户端登录的情况，都是如此。WebLogic Server 执行 JAAS 登录内部调用的方法是 `weblogic.security.auth.Authentication.authenticate()`。当使用身份验证类时，`weblogic.security.SimpleCallbackHandler`可能是一种很有用的辅助程序类。
- 虽然 WebLogic Server 不使用 JAAS 授权（而是使用 WebLogic 安全）保护任何资源，但可以在应用程序代码中使用 JAAS 授权来保护应用程序自己的资源。


有关 JAAS 的详细信息，请参阅 JAAS 文档，访问地址为 [http://java.sun.com/products/jaas/reference/docs/index.html](http://java.sun.com/products/jaas/reference/docs/index.html)。



## JAAS 身份验证开发环境

无论客户端是应用程序、applet、Enterprise JavaBean (EJB)，还是需要进行身份验证的 servlet，WebLogic Server 都使用 JAAS 类安全可靠地对服务器进行身份验证。JAAS 实现了 Java 版的可插入身份验证模块（Pluggable Authentication Module，简称 PAM）框架，该框架允许应用程序保持独立于底层身份验证技术。因此，PAM 框架允许使用新的身份验证技术或更新后的身份验证技术，而无需对
 Java 应用程序进行修改。

WebLogic Server 使用 JAAS 对远程 Java 客户端进行身份验证，在内部用于身份验证。因此，只有自定义身份验证提供程序的开发人员和远程 Java 客户端应用程序的开发人员必须与JAAS 直接相关。Web 浏览器客户端的用户，或容器包含的 Java 客户端应用程序（例如，从 servlet 调用 EJB 的应用程序）的开发人员，不需要直接使用 JAAS 或掌握 JAAS 知识。
|**注意：**|为了在 WebLogic 客户端中实现安全，必须在 Java 客户端安装 WebLogic Server 软件分发工具包。|
|----|----|

本部分包含下列主题：

- [JAAS
 身份验证 API](http://edocs.weblogicfans.net/wls/docs92/security/fat_client.html#wp1042108)
- [JAAS
 客户端应用程序组件](http://edocs.weblogicfans.net/wls/docs92/security/fat_client.html#wp1047391)
- [WebLogic
 LoginModule 实现](http://edocs.weblogicfans.net/wls/docs92/security/fat_client.html#wp1048434)


### JAAS 身份验证 API

要在 WebLogic Server 上实现使用 JAAS 身份验证的 Java 客户端，请结合使用 Java J2SE 5.0 应用程序编程接口 (API) 和 WebLogic API。

[表 4-1](http://edocs.weblogicfans.net/wls/docs92/security/fat_client.html#wp1042459) 列出并描述了用于实现 JAAS 身份验证的 Java API 包。[表 4-1](http://edocs.weblogicfans.net/wls/docs92/security/fat_client.html#wp1042459)中的信息来自
 Java API 文档，并且加了批注以添加 WebLogic Server 特定信息。有关 Java API 的详细信息，请参阅 Javadocs，访问地址为[http://java.sun.com/j2se/1.5.0/docs/api/index.html](http://java.sun.com/j2se/1.5.0/docs/api/index.html) 和 [http://java.sun.com/j2ee/1.4/docs/api/index.html](http://java.sun.com/j2ee/1.4/docs/api/index.html)。

[表 4-2](http://edocs.weblogicfans.net/wls/docs92/security/fat_client.html#wp1054786) 列出并描述了用于实现 JAAS 身份验证的 WebLogic API。有关详细信息，请参阅 [Javadocs
 for WebLogic Classes](http://e-docs.bea.com/wls/docs92/javadocs/index.html)。



|Java JAAS API|描述| | |
|----|----|----|----|
|[javax.security.auth.Subject](http://java.sun.com/j2se/1.5.0/docs/api/index.html)|`Subject` 类表示请求源，可以是单个用户或一个组。只有在成功登录主题之后，才创建 `Subject`对象。| | |
|[javax.security.auth.login.LoginContext](http://java.sun.com/j2se/1.5.0/docs/api/index.html)|`LoginContext`类描述了用于对 `Subjects`进行身份验证的基本方法，并且提供了开发独立于底层身份验证技术的应用程序的方法。`Configuration` 指定了将和特定应用程序一起使用的身份验证技术或 LoginModule。所以，在应用程序中可以插入不同的 LoginModule，而无需对应用程序本身做任何修改。调用者在实例化 `LoginContext`之后，将调用 `login`方法对 `Subject`进行身份验证。而 `login`方法则从为调用者指定的名称配置的各个 LoginModule 调用 `login`方法。如果返回 `login`方法，而没有引发异常，则表示整个身份验证成功。然后，调用者可以通过调用 `getSubject`方法，检索近期通过身份验证的 `Subject`。通过分别调用 `Subject`的`getPrincipals`、`getPublicCredentials` 和 `getPrivateCredentials`方法，可以检索与 `Subject`相关联的委托人和凭据。要退出 `Subject`，调用者可以调用 `logout`方法。与 `login`方法一样，此 `logout`方法调用为此 `LoginContext`配置的各个 LoginModule 的 `logout`方法。有关此类的示例实现，请参阅[清单 4-3](http://edocs.weblogicfans.net/wls/docs92/security/fat_client.html#wp1054097)。| | |
|[javax.security.auth.login.Configuration](http://java.sun.com/j2se/1.5.0/docs/api/index.html)|这是用于表示应用程序中 LoginModule 配置的抽象类。`Configuration` 指定了应该用于特定应用程序的 LoginModule，并指定了调用 LoginModule 所应遵循的顺序。需要将该抽象类分成子类，以提供读取和加载实际配置的实现。在 WebLogic Server 中，使用登录配置文件，而不是此类。有关示例配置文件，请参阅[清单 4-2](http://edocs.weblogicfans.net/wls/docs92/security/fat_client.html#wp1054137)。在默认情况下，WebLogic Server 使用从配置文件中读取的 Sun Microsystems 配置类。| | |
|[javax.security.auth.spi.LoginModule](http://java.sun.com/j2se/1.5.0/docs/api/index.html)|`LoginModule` 描述了身份验证技术提供程序实现的接口。在应用程序中插入 `LoginModules`，以提供特定类型的身份验证。当应用程序开发人员写入 `LoginContext` API 时，身份验证技术提供程序实现 LoginModule 接口。配置指定了要和特定登录应用程序一起使用的 LoginModule。所以，可以在应用程序中插入不同的 LoginModule，而无需对应用程序本身做任何修改。|**注意：**|WebLogic Server 提供 LoginModule (```weblogic.security.auth.login.UsernamePasswordLoginModule```) 的实现。BEA 建议您在 WebLogic Server Java 客户端中对 JAAS 身份验证使用该实现；不过，您仍然可以自己开发 LoginModule。||----|----||**注意：**|WebLogic Server 提供 LoginModule (```weblogic.security.auth.login.UsernamePasswordLoginModule```) 的实现。BEA 建议您在 WebLogic Server Java 客户端中对 JAAS 身份验证使用该实现；不过，您仍然可以自己开发 LoginModule。|
|**注意：**|WebLogic Server 提供 LoginModule (```weblogic.security.auth.login.UsernamePasswordLoginModule```) 的实现。BEA 建议您在 WebLogic Server Java 客户端中对 JAAS 身份验证使用该实现；不过，您仍然可以自己开发 LoginModule。| | |
|[javax.security.auth.callback.Callback](http://java.sun.com/j2se/1.5.0/docs/api/index.html)|将此接口的实现传递到 `CallbackHandler`，使底层安全服务与调用应用程序进行交互，检索特定的身份验证数据，如用户名和密码，或者显示某些信息，如错误和警告消息。`Callback` 实现不检索或不显示由底层安全服务请求的信息。`Callback` 实现只提供将这样的请求传递到应用程序的方法，而对应用程序来说，则提供将请求信息返回到底层安全服务的方法。| | |
|[javax.security.auth.callback.CallbackHandler](http://java.sun.com/j2se/1.5.0/docs/api/index.html)|应用程序实现 `CallbackHandler`，并将它传递给底层安全服务，以便它们可以与应用程序进行交互，检索特定的身份验证数据，如用户名和密码，或者显示某些信息，如错误和警告消息。`CallbackHandlers` 以一种依赖于应用程序的方式实现。底层安全服务通过向 `CallbackHandler` 传递单个 `Callbacks`来请求不同类型的信息。`CallbackHandler` 实现根据传递给它的`Callbacks` 决定如何检索和显示信息。例如，如果底层服务需要用户名和密码对用户进行身份验证，则使用 `NameCallback` 和 `PasswordCallback`。`CallbackHandler` 可以选择先后提示用户输入用户名和密码，或者在单独窗口中提示用户输入用户名和密码。| | |






|WebLogic JAAS API|描述| | |
|----|----|----|----|
|```[weblogic.security.auth.Authenticate](http://e-docs.bea.com/wls/docs92/javadocs/weblogic/security/auth/package-summary.html)```|用于对用户凭据进行身份验证的身份验证类。LoginModule 的 WebLogic 实现 (```weblogic.security.auth.login.UsernamePasswordLoginModule```使用此类对用户进行身份验证，并将 `Principals`添加到 `Subject`。编写 LoginModule 的开发人员也必须出于同样的目的使用此类。| | |
|```[weblogic.security.auth.Callback.ContextHandlerCallback](http://e-docs.bea.com/wls/docs92/javadocs/index.html)```|底层安全服务使用此类实例化 `ContextHandlerCallback`，并其传递到 `CallbackHandler`的 `invokeCallback`方法,以检索与该安全操作相关的 ContextHandler。如果没有 ContextHandler 与该操作相关联，则将引发 `javax.security.auth.callback.UnsupportedCallbackexception`。该回调将 ContextHandler 传递到 LoginModule.login() 方法。| | |
|```[weblogic.security.auth.Callback.GroupCallback](http://e-docs.bea.com/wls/docs92/javadocs/index.html)```|底层安全服务使用该类实例化 `GroupCallback`，并将其传递到 `CallbackHandler`的 `invokeCallback`方法，以检索组信息。| | |
|```[weblogic.security.auth.Callback.URLCallback](http://e-docs.bea.com/wls/docs92/javadocs/index.html)```|底层安全服务使用该类实例化 `URLCallback`，并将其传递到 `CallbackHandler`的 `invokeCallback`方法，以检索 URL 信息。`LoginModule,` (```weblogic.security.auth.login.UsernamePasswordLoginModule``` 的 WebLogic 实现要使用该类。|**注意：**|应用程序开发人员不应该使用此类检索 URL 信息，而是应该使用 weblogic.security.URLCallbackHandler 进行检索。||----|----||**注意：**|应用程序开发人员不应该使用此类检索 URL 信息，而是应该使用 weblogic.security.URLCallbackHandler 进行检索。|
|**注意：**|应用程序开发人员不应该使用此类检索 URL 信息，而是应该使用 weblogic.security.URLCallbackHandler 进行检索。| | |
|[weblogic.security.Security](http://e-docs.bea.com/wls/docs92/javadocs/index.html)|此类实现 WebLogic Server 客户端 `runAs` 方法。客户端应用程序使用 `runAs` 方法将它们的 `Subject` 标识与它们执行的 `PrivilegedAction` 或 `PrivilegedExceptionAction` 进行关联。有关示例实现，请参阅[清单 4-5](http://edocs.weblogicfans.net/wls/docs92/security/fat_client.html#wp1051366)。| | |
|[weblogic.security.URLCallbackHandler](http://e-docs.bea.com/wls/docs92/javadocs/index.html)|应用程序开发人员用于返回 `username`、`password` 和 `URL`的类。应用程序开发人员应该使用此类处理 `URLCallback`检索 URL 信息。| | |




### JAAS 客户端应用程序组件

JAAS 身份验证客户端应用程序至少包括下列组件：

- Java 客户端

通过调用对象的 `login()`方法，Java 客户端实例化 `LoginContext` 对象，并调用登录。`login()` 方法调用每个
 LoginModule 中的方法进行登录和身份验证。

LoginContext 也实例化新的空 `javax.security.auth.Subject` 对象（该对象表示正进行身份验证的用户或服务），并构造已配置的 LoginModule，并使用新的 `Subject`和 `CallbackHandler`将该
 LoginModule 实例化。

随后，LoginContext 通过调用 LoginContext 的 `getSubject`方法，检索已通过身份验证的主题。LoginContext使用 `weblogic.security.Security.runAs()` 方法将 `Subject` 标识与将代表用户标识执行的`PrivilegedAction` 或 `PrivilegedExceptionAction` 进行关联。

- LoginModule

LoginModule 使用 `CallbackHandler` 获取用户名和密码，并确定它们是否正是所需的用户名和密码。

如果身份验证成功，LoginModule 就使用代表用户的委托人填充主题。LoginModule 在主题中填充的委托人就是 `Principal` 的实例，而该实例正是实现 `java.security.Principal`接口的类。

您可以编写进行不同类型身份验证的 LoginModule 文件，包括用户名/密码身份验证和证书身份验证。客户端应用程序可以包括一个或多个 LoginModule，但至少要包括一个。
|**注意：**|使用 WebLogic Server 应用程序中的 JAAS `javax.security.auth.Subject.doAs`方法后，并不将主题与客户端操作进行关联。在 WebLogic Server 应用程序中，您可以使用 `doAs`方法实现 J2SE 安全，但是这种用法无需使用 `Security.runAs()`方法。|
|----|----|

- Callbackhandler

`CallbackHandler` 实现 `javax.security.auth.callback.CallbackHandler` 接口。LoginModule
 使用 `CallbackHandler` 与用户进行通信，并获取请求的信息，例如用户名和密码。

- 配置文件

此文件配置在应用程序中使用的 LoginModule。它还指定了 LoginModule 的位置；如果有多个 LoginModule，则指定执行这些 LoginModule 的顺序。此文件能够使 Java 应用程序保持独立于使用 LoginModule 定义和实现的身份验证技术。

- 操作文件

此文件定义客户端应用程序将执行的操作。

- `ant` 生成脚本 (`build.xml`)

此脚本编译应用程序所需的所有文件，并将这些文件部署到 WebLogic Server 应用程序目录。



有关实现此处所描述的组件的完整工作 JAAS 身份验证客户端，请参阅 JAAS 示例应用程序（位于随 WebLogic Server 一起提供的 SAMPLES_HOME`\server\examples\src\examples\security\jaas`目录中）。也可以从
 BEA 的 [dev2dev](http://dev.bea.com/code)站点联机获得此示例。

有关 JAAS 身份验证基础知识的详细信息，请参阅 Sun 的“JAAS Authentication Tutorial”，访问地址为 [http://java.sun.com/j2se/1.5.0/docs/guide/security/jaas/tutorials/GeneralAcnOnly.html](http://java.sun.com/j2se/1.5.0/docs/guide/security/jaas/tutorials/GeneralAcnOnly.html)。

### WebLogic LoginModule 实现

在 `weblogic.jar`文件（位于 `WL_HOME\server\lib`目录中）的 WebLogic
 Server 分发中提供了 `LoginModule`类的 WebLogic 实现。
|**注意：**|WebLogic Server 支持 JAAS 定义的所有回调类型和扩展 JAAS 规范的所有回调类型。|
|----|----|

WebLogic Server `UsernamePasswordLoginModule`在执行操作之前会检查是否已定义现有系统用户身份验证；如果已定义，则不执行任何操作。

有关实现 JAAS LoginModule 的详细信息，请参阅“LoginModule Developer's Guide”，访问地址为 [http://java.sun.com/j2se/1.5.0/docs/guide/security/jaas/JAASLMDevGuide.html](http://java.sun.com/j2se/1.4.2/docs/guide/security/jaas/JAASLMDevGuide.html)。

### JVM 范围的默认用户和 runAs() 方法

当您第一次使用 LoginModule (`weblogic.security.auth.login.UsernamePasswordLoginModule`) 的 WebLogic Server 实现登录时，指定的用户就变成 JVM （Java 虚拟机）的计算机范围默认用户。当您执行`weblogic.security.Security.runAs()` 方法时，该方法将指定的`Subject`与当前线程的访问权限进行关联，然后执行操作。如果指定的`Subject`表示的是没有权限的用户（没有指定给任何组的用户被视作没有权限的用户），则使用
 JVM 范围的默认用户。所以，确保 `runAs()`方法指定所需的`Subject`很重要。为此，您可以使用下列选项之一：

- 选项 1：如果客户端已控制 `main()`，请在客户端代码中实现[清单 4-1](http://edocs.weblogicfans.net/wls/docs92/security/fat_client.html#wp1051872)中显示的包装代码。
- 选项 2：如果客户端没有控制 `main()`，请在每个线程的 `run()`方法中实现[清单 4-1](http://edocs.weblogicfans.net/wls/docs92/security/fat_client.html#wp1051872)中显示的包装代码。

清单 4-1 runAs() 方法包装代码

import java.security.PrivilegedAction;
import javax.security.auth.Subject;
import weblogic.security.Security;

public class client
{
  public static void main(String[] args)
  {
  Security.runAs(new Subject(),
    new PrivilegedAction() {
       public Object run() {
       //
       //如果在客户端代码中实现，则 main() 从此处开始。
 //

```
return null;
       }
    });
  }
}
```






## 使用 JAAS 身份验证编写客户端应用程序

要在 WebLogic Server Java 客户端中使用 JAAS 对主题进行身份验证，请执行下列步骤：

- 为要和 WebLogic Server 结合使用的身份验证机制实现 `LoginModule`类。每种身份验证机制都需要一个 LoginModule 类。单个 WebLogic Server 部署可以有多个 LoginModule 类。|**注意：**|BEA 建议，您应该使用 WebLogic Server` `(`weblogic.security.auth.login.UsernamePasswordLoginModule`) 提供的 LoginModule 的实现对用户名/密码进行身份验证。您可以编写自己的 `LoginModule`对用户名/密码进行身份验证，但是，请不要试图修改 WebLogic Server `LoginModule`后重新使用。如果您编写自己的 LoginModule，则必须使用它调用 `weblogic.security.auth.Authenticate.authenticate()`方法执行登录。如果您使用远程登录机制，例如 SAML，则不需要调用 `authenticate()` 方法。如果您正在使用 WebLogic Server 执行登录，则只需要调用 `authenticate()`。|
|----|----|

`weblogic.security.auth.Authenticate` 类将 [JNDI
 Environment object](http://e-docs.bea.com/wls/docs92/javadocs/weblogic/jndi/Environment.html) 用于初始上下文，如[表 4-3](http://edocs.weblogicfans.net/wls/docs92/security/fat_client.html#wp1033418) 中所述。

- 实现 LoginModule 将用来与用户进行通信的 `CallbackHandler`类，并获取请求的信息，例如用户名、密码和 URL。倘若客户端有服务器故障转移的优势，则该 URL 可能成为 WebLogic 群集的 URL。WebLogic Server
 分发提供在 JAAS 客户端示例中使用的 `SampleCallbackHandler`。`SampleCallbackHandler.java`代码是分发的一部分，可以联机获得。

- 联机：单击 BEA 验证代码链接，访问地址为 [http://dev.bea.com/code](http://dev.bea.com/code)。
- WebLogic Server 分发：在目录 SAMPLES_HOME`\server\examples\src\examples\security\jaas`中|**注意：**|无需实现自己的 `CallbackHandler`类，您就可以使用 WebLogic 提供的两个 `CallbackHandler`类中的任意一个：`weblogic.security.SimpleCallbackHandler` 或`weblogic.security.URLCallbackHandler`。有关这些类的详细信息，请参阅 [Javadocs for WebLogic Classes](http://e-docs.bea.com/wls/docs92/javadocs/index.html)。|
|----|----|



- 编写配置文件，该文件指定了哪些 LoginModule 类用于 WebLogic Server 和 LoginModule 类的调用顺序。有关 JAAS 客户端示例（在 WebLogic Server 分发中提供）中使用的示例配置文件，请参阅[清单 4-2](http://edocs.weblogicfans.net/wls/docs92/security/fat_client.html#wp1054137)。

清单 4-2 sample_jaas.config 代码示例

/** JAAS 示例应用程序的登录配置 **/

Sample {
    weblogic.security.auth.login.UsernamePasswordLoginModule
            required debug=false;
};

- 在 Java 客户端中，编写代码以实例化 `LoginContext`。`LoginContext` 用于查看配置文件 `sample_jaas.config`，以加载为
 WebLogic Server 配置的默认 LoginModule。有关示例 `LoginContext` 的实例化，请参阅[清单 4-3](http://edocs.weblogicfans.net/wls/docs92/security/fat_client.html#wp1054097)。|**注意：**|如果您使用另一种方法对用户（例如标识声明提供程序，或 WebLogic Server 的远程实例）进行身份验证，则由远程源确定默认 LoginModule。|
|----|----|

清单 4-3 LoginContext 代码片段

...
import javax.security.auth.login.LoginContext;
...

    LoginContext loginContext = null;

    try
    {
      // 创建 LoginContext；指定用户名/密码登录模块
      loginContext = new LoginContext("Sample", 
             new SampleCallbackHandler(username, password, url));
    }

- 调用 `LoginContext`实例的 `login()`方法。`login()`方法调用所有已加载的
 LoginModule。每个 LoginModule 都试图对对象进行身份验证。如果没有满足已配置的登录条件，`LoginContext`将引发 `LoginException`。有关`login()`方法示例，请参阅[清单 4-4](http://edocs.weblogicfans.net/wls/docs92/security/fat_client.html#wp1046759)。

清单 4-4 Login() 方法代码片段

...
import javax.security.auth.login.LoginContext;
import javax.security.auth.login.LoginException;
import javax.security.auth.login.FailedLoginException;
import javax.security.auth.login.AccountExpiredException;
import javax.security.auth.login.CredentialExpiredException;
...
/**
     * 尝试进行身份验证
     */
    try
    {
      // 如果返回时没有异常，则身份验证成功
      loginContext.login();

    }

    catch(FailedLoginException fle)

    {
      System.out.println("Authentication Failed, " +
                               fle.getMessage());
      System.exit(-1);
    }
    catch(AccountExpiredException aee)
    {
      System.out.println("Authentication Failed: Account Expired");
      System.exit(-1);
    }
    catch(CredentialExpiredException cee)
    {
      System.out.println("Authentication Failed: Credentials
                               Expired");
      System.exit(-1);
    }
    catch(Exception e)
    {
      System.out.println("Authentication Failed: Unexpected
                               Exception, " + e.getMessage());
      e.printStackTrace();
      System.exit(-1);
    }

- 使用 `javax.security.auth.Subject.getSubject()`方法在 Java 客户端中编写代码，以从 `LoginContext`实例中检索已通过身份验证的主题，并将操作作为主题调用。一旦成功完成对主题的身份验证，就可以通过调用`weblogic.security.Security`.`runAs()`方法访问可置于该主题中的控件。`runAs()`方法将指定的主题与当前线程的访问权限进行关联，然后执行操作。有关 `getSubject()`方法和 `runAs()`方法的示例实现，请参阅[清单 4-5](http://edocs.weblogicfans.net/wls/docs92/security/fat_client.html#wp1051366)。|**注意：**|使用 WebLogic Server 应用程序中的 JAAS `javax.security.auth.Subject.doAs`方法后，并不将主题与客户端操作进行关联。在 WebLogic Server 应用程序中，您可以使用 `doAs`方法实现 J2SE 安全，但是这种用法无需使用 `Security.runAs()` 方法。|
|----|----|

清单 4-5 getSubject() 方法和 runAs() 方法的代码片段

...
/**
 * 检索经过身份验证的主题；作为主题执行 SampleAction
 */
   Subject subject = loginContext.getSubject();
   SampleAction sampleAction = new SampleAction(url);
   Security.runAs(subject, sampleAction);
   System.exit(0);

...

- 如果主题有所需的权限，请编写代码执行操作。BEA 提供 `javax.security.PrivilegedAction`类的示例实现 `SampleAction`，该类可以执行
 EJB 进行股票交易。`SampleAction.java` 代码是分发的一部分，可以联机获得。

- 联机：单击 BEA 验证代码链接，访问地址为 [http://dev.bea.com/code](http://dev.bea.com/code)。
- WebLogic Server 分发：在目录 SAMPLES_HOME`\server\examples\src\examples\security\jaas`中


- 调用 `LoginContext`实例的 `logout()`方法。`logout()` 方法会关闭用户会话，并清除`Subject`。有关 `login()`方法的示例，请参阅[清单 4-6](http://edocs.weblogicfans.net/wls/docs92/security/fat_client.html#wp1051438)。

清单 4-6 logout() 方法代码示例

...
import javax.security.auth.login.LoginContext;
...
try
    {
      System.out.println("logging out...");
      loginContext.logout();
    }



|**注意：**|绝不会为 WebLogic 身份验证提供程序或自定义身份验证提供程序调用 `LoginModule.logout()`方法，因为创建 `Principals` 并将其放入`Subject`后，WebLogic 安全框架就不再控制`Subject`的生命周期。所以，创建 JAAS`LoginContext`进行登录并获取`Subject`的代码，也应该调用 `LoginContext`才能退出。调用 `LoginContext.logout()` 将会清除 `Subject`中的 `Principals`。|
|----|----|



## 使用 JNDI 身份验证

Java 客户端使用 Java 命名和目录接口 (JNDI) 将凭据传递到 WebLogic Server。Java 客户端通过获取 JNDI `InitialContext` 来建立与 WebLogic Server 的连接。然后，Java 客户端使用 `InitialContext` 在
 WebLogic Server JNDI 树中查找它需要的资源。
|**注意：**|JAAS 是进行身份验证的首选方法，但是 WebLogic 身份验证提供程序的 LoginModule 只支持用户名和密码身份验证。因此，对于客户端证书身份验证（也称作双向 SSL 身份验证），应该使用 JNDI。要使用 JAAS 进行客户端证书身份验证，必须编写自定义身份验证提供程序（其 LoginModule 能够进行证书身份验证）。有关如何编写 LoginModule 的信息，请参阅[http://java.sun.com/j2se/1.5.0/docs/guide/security/jaas/JAASLMDevGuide.html](http://java.sun.com/j2se/1.5.0/docs/guide/security/jaas/JAASLMDevGuide.html)。|
|----|----|

要指定用户及其凭据，请设置[表 4-3](http://edocs.weblogicfans.net/wls/docs92/security/fat_client.html#wp1033418)中列出的 JNDI 属性。



|属性|含义|
|----|----|
|`INITIAL_CONTEXT_FACTORY`|提供进入 WebLogic Server 环境的入口点。[weblogic.jndi.WLInitialContextFactory](http://e-docs.bea.com/wls/docs92/javadocs/weblogic/jndi/WLInitialContextFactory.html) 类就是 WebLogic Server 的 JNDI SPI。|
|`PROVIDER_URL`|为提供名称服务的 WebLogic Server 指定主机和端口。例如：` t3://weblogic:7001`。|
|`SECURITY_PRINCIPAL`|当某个用户对默认（活动）安全领域进行身份验证时，指定该用户的标识。|
|`SECURITY_CREDENTIALS`|当某个用户对默认（活动）安全领域进行身份验证时，指定该用户的凭据。|




这些属性存储在要传送到 `InitialContext `构造方法的散列表中。[清单 4-7](http://edocs.weblogicfans.net/wls/docs92/security/fat_client.html#wp1033495)说明了如何在运行于
 WebLogic Server 的 Java 客户端中使用 JNDI 身份验证。

清单 4-7 身份验证示例

...
Hashtable env = new Hashtable();
     env.put(Context.INITIAL_CONTEXT_FACTORY,
              "weblogic.jndi.WLInitialContextFactory");
     env.put(Context.PROVIDER_URL, "t3://weblogic:7001");
     env.put(Context.SECURITY_PRINCIPAL, "javaclient");
     env.put(Context.SECURITY_CREDENTIALS, "javaclientpassword");
     ctx = new InitialContext(env);
|**注意：**|有关 JNDI 上下文、线程以及如何避免出现潜在的 JNDI 上下文问题的信息，请参阅“WebLogic JNDI 编程”中的 [JNDI 上下文和线程](http://edocs.weblogicfans.net/wls/docs92/jndi/jndi.html#jndicontext)和[如何避免潜在的 JNDI 上下文问题](http://edocs.weblogicfans.net/wls/docs92/jndi/jndi.html#contextproblems)。|
|----|----|
|**注意：**|在早于 9.0 版的 WebLogic Server 中，当使用除带有 JNDI 的 IIOP 之外的协议时，将“粘”住第一个用户。也就是说，如果没有其他用户，该用户就成为默认用户。这种做法并不好，因为没有用户名和凭据的任何后续登录都被授予该默认用户的标识。|
|----|----|
|注意：|在 9.0 版本中，不会再出现这种情况，也没有默认用户。|
|----|----|
|注意：|要返回到以前的行为，必须通过命令行或者 `InitialContext`接口设置 `weblogic.jndi.WLContext.ENABLE_DEFAULT_USER`域。|
|----|----|



## Java 客户端 JAAS 身份验证代码示例

WebLogic Server 产品提供了完整的工作 JAAS 身份验证示例。该示例位于 SAMPLES_HOME`\server\examples\src\examples\security\jaas`目录中。有关该示例的描述以及如何生成、配置和运行此示例的说明，请参阅示例目录中的 `package.html`文件。可以修改此代码示例，然后重新使用。




