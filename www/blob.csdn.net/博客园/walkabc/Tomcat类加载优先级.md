# Tomcat类加载优先级 - walkabc - 博客园
# [Tomcat类加载优先级](https://www.cnblogs.com/hitfire/articles/4567900.html)
如前所述,web应用程序类装入器的发散 默认的Java授权模型(按照建议的 Servlet规范,版本2.4,部分9.7.2 Web应用程序类加载器)。 当一个请求加载 从web应用程序的类 *WebappX *类装入器处理, 这类加载器将在本地存储库 **第一个 **, 而不是委托前看。 也有例外。 类的 JRE不能覆盖基类的一部分。 对于一些类(如 XML解析器组件在J2SE 1.4 +)、J2SE 1.4支持功能 使用。 最后,web应用程序类加载器将为JavaEE总是代表第一 为规范由Tomcat实现API类 (Servlet、JSP EL WebSocket)。 所有其他类装入器在Tomcat中遵循 通常的代表团的模式。
因此,从一个web应用程序的角度来看,类或资源 加载看起来在接下来的存储库,按此顺序:
- 引导您的JVM类
- */ web - inf /类 *您的web应用程序
- */ web - inf / lib / * . jar *您的web应用程序
- 系统类装入器类(如上所述)
- 常见的类装入器的类(如上所述)
如果web应用程序类装入器 [配置](http://localhost:8080/docs/config/loader.html)与 `<装载机代表= " true " / > `然后订单就变成:
- 引导您的JVM类
- 系统类装入器类(如上所述)
- 常见的类装入器的类(如上所述)
- */ web - inf /类 *您的web应用程序
- */ web - inf / lib / * . jar *您的web应用程序

