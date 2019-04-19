# Servlet，Webservice中spring 获取Bean方式 - aisoo的专栏 - CSDN博客
2010年09月02日 10:37:00[aisoo](https://me.csdn.net/aisoo)阅读数：2490
Webservice
// 获取Bean
  MessageContext messageContext = context.getMessageContext();
  ServletContext servletContext = (ServletContext) messageContext.get(MessageContext.SERVLET_CONTEXT);
  WebApplicationContext applicationContext = WebApplicationContextUtils.getWebApplicationContext(servletContext);
  ITerminologyDownloadSrv downloadSrv = (ITerminologyDownloadSrv) applicationContext
    .getBean("rhinTerminologyDownloadSrv");
Servlet中：
// 获取Bean
  WebApplicationContext applicationContext = WebApplicationContextUtils.getWebApplicationContext(this.getServletContext());
  ITerminologyDownloadSrv downloadSrv = (ITerminologyDownloadSrv) applicationContext
    .getBean("rhinTerminologyDownloadSrv");
