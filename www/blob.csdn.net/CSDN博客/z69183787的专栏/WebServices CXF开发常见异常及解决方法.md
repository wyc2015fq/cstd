# WebServices CXF开发常见异常及解决方法 - z69183787的专栏 - CSDN博客
2014年06月30日 11:25:50[OkidoGreen](https://me.csdn.net/z69183787)阅读数：35176
======================================================异常1=================================================================
2011-7-14 9:55:15 org.apache.cxf.service.factory.ReflectionServiceFactoryBean buildServiceFromClass
信息: Creating Service {http://services.ws.com/}IHelloServicesService from class com.ws.services.IHelloServices
Exception in thread "main" javax.xml.parsers.FactoryConfigurationError: Provider org.apache.xerces.jaxp.DocumentBuilderFactoryImpl not found
 at javax.xml.parsers.DocumentBuilderFactory.newInstance(Unknown Source)
 at org.apache.cxf.helpers.XMLUtils.getDocumentBuilderFactory(XMLUtils.java:109)
 at org.apache.cxf.helpers.XMLUtils.getParser(XMLUtils.java:133)
 at org.apache.cxf.helpers.XMLUtils.parse(XMLUtils.java:151)
 at org.apache.cxf.jaxb.JAXBDataBinding.<clinit>(JAXBDataBinding.java:111)
 at org.apache.cxf.service.factory.ReflectionServiceFactoryBean.createDefaultDataBinding(ReflectionServiceFactoryBean.java:214)
 at org.apache.cxf.service.factory.AbstractServiceFactoryBean.getDataBinding(AbstractServiceFactoryBean.java:109)
 at org.apache.cxf.service.factory.AbstractServiceFactoryBean.getDataBinding(AbstractServiceFactoryBean.java:105)
 at org.apache.cxf.jaxws.support.JaxWsServiceFactoryBean.generatedWrapperBeanClass(JaxWsServiceFactoryBean.java:654)
 at org.apache.cxf.jaxws.support.JaxWsServiceFactoryBean.getExtraClass(JaxWsServiceFactoryBean.java:630)
 at org.apache.cxf.service.factory.ReflectionServiceFactoryBean.buildServiceFromClass(ReflectionServiceFactoryBean.java:432)
 at org.apache.cxf.jaxws.support.JaxWsServiceFactoryBean.buildServiceFromClass(JaxWsServiceFactoryBean.java:682)
 at org.apache.cxf.service.factory.ReflectionServiceFactoryBean.initializeServiceModel(ReflectionServiceFactoryBean.java:501)
 at org.apache.cxf.service.factory.ReflectionServiceFactoryBean.create(ReflectionServiceFactoryBean.java:241)
 at org.apache.cxf.jaxws.support.JaxWsServiceFactoryBean.create(JaxWsServiceFactoryBean.java:202)
 at org.apache.cxf.frontend.AbstractWSDLBasedEndpointFactory.createEndpoint(AbstractWSDLBasedEndpointFactory.java:101)
 at org.apache.cxf.frontend.ServerFactoryBean.create(ServerFactoryBean.java:157)
 at org.apache.cxf.jaxws.JaxWsServerFactoryBean.create(JaxWsServerFactoryBean.java:202)
 at com.test.ServerTest.<init>(ServerTest.java:19)
 at com.test.ServerTest.main(ServerTest.java:23)
**缺少xercesImpl.jar**
======================================================异常2=================================================================
2011-7-14 10:10:59 org.apache.cxf.service.factory.ReflectionServiceFactoryBean buildServiceFromClass
信息: Creating Service {http://services.ws.com/}IHelloServicesService from class com.ws.services.IHelloServices
Exception in thread "main" java.lang.LinkageError: JAXB 2.1 API is being loaded from the bootstrap classloader, but this RI (from jar:file:/E:/Project/workspace/CxfServices/libs/jaxb-impl-2.2.1.1.jar!/com/sun/xml/bind/v2/model/impl/ModelBuilder.class) needs
 2.2 API. Use the endorsed directory mechanism to place jaxb-api.jar in the bootstrap classloader. (See[http://java.sun.com/j2se/1.6.0/docs/guide/standards/](http://java.sun.com/j2se/1.6.0/docs/guide/standards/))
 at com.sun.xml.bind.v2.model.impl.ModelBuilder.<clinit>(ModelBuilder.java:173)
 at com.sun.xml.bind.v2.runtime.JAXBContextImpl.getTypeInfoSet(JAXBContextImpl.java:456)
 at com.sun.xml.bind.v2.runtime.JAXBContextImpl.<init>(JAXBContextImpl.java:302)
 at com.sun.xml.bind.v2.runtime.JAXBContextImpl$JAXBContextBuilder.build(JAXBContextImpl.java:1140)
 at com.sun.xml.bind.v2.ContextFactory.createContext(ContextFactory.java:154)
 at com.sun.xml.bind.v2.ContextFactory.createContext(ContextFactory.java:121)
 at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
 at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:39)
 at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:25)
 at java.lang.reflect.Method.invoke(Method.java:597)
 at javax.xml.bind.ContextFinder.newInstance(ContextFinder.java:210)
 at javax.xml.bind.ContextFinder.find(ContextFinder.java:368)
 at javax.xml.bind.JAXBContext.newInstance(JAXBContext.java:574)
 at org.apache.cxf.jaxb.JAXBContextCache.createContext(JAXBContextCache.java:258)
 at org.apache.cxf.jaxb.JAXBContextCache.getCachedContextAndSchemas(JAXBContextCache.java:167)
 at org.apache.cxf.jaxb.JAXBDataBinding.createJAXBContextAndSchemas(JAXBDataBinding.java:418)
 at org.apache.cxf.jaxb.JAXBDataBinding.initialize(JAXBDataBinding.java:290)
 at org.apache.cxf.service.factory.AbstractServiceFactoryBean.initializeDataBindings(AbstractServiceFactoryBean.java:86)
 at org.apache.cxf.service.factory.ReflectionServiceFactoryBean.buildServiceFromClass(ReflectionServiceFactoryBean.java:438)
 at org.apache.cxf.jaxws.support.JaxWsServiceFactoryBean.buildServiceFromClass(JaxWsServiceFactoryBean.java:682)
 at org.apache.cxf.service.factory.ReflectionServiceFactoryBean.initializeServiceModel(ReflectionServiceFactoryBean.java:501)
 at org.apache.cxf.service.factory.ReflectionServiceFactoryBean.create(ReflectionServiceFactoryBean.java:241)
 at org.apache.cxf.jaxws.support.JaxWsServiceFactoryBean.create(JaxWsServiceFactoryBean.java:202)
 at org.apache.cxf.frontend.AbstractWSDLBasedEndpointFactory.createEndpoint(AbstractWSDLBasedEndpointFactory.java:101)
 at org.apache.cxf.frontend.ServerFactoryBean.create(ServerFactoryBean.java:157)
 at org.apache.cxf.jaxws.JaxWsServerFactoryBean.create(JaxWsServerFactoryBean.java:202)
 at com.test.ServerTest.<init>(ServerTest.java:19)
 at com.test.ServerTest.main(ServerTest.java:23)
原因：jdk1.6.0_02中的jaxws-api-2.0.jar及jaxb-api-2.0.jar，而cxf需要jaxws-api-2.1.jar及jaxb-api-2.1.jar的支持。
解决方法：1、将cxf所需的2.1的jar复制一份到jdk目录下的jre\lib\endorsed文件夹中。如果endorsed文件不存在，可新建。
          2、jdk升级到1.6.0_22版本以上。
======================================================异常3=================================================================
2011-7-14 10:25:40 org.apache.cxf.service.factory.ReflectionServiceFactoryBean buildServiceFromClass
信息: Creating Service {http://services.ws.com/}IHelloServicesService from class com.ws.services.IHelloServices
Exception in thread "main" java.lang.NoClassDefFoundError: org/eclipse/jetty/server/Connector
 at org.apache.cxf.transport.http_jetty.JettyHTTPServerEngineFactory.createJettyHTTPServerEngine(JettyHTTPServerEngineFactory.java:237)
 at org.apache.cxf.transport.http_jetty.JettyHTTPDestination.retrieveEngine(JettyHTTPDestination.java:113)
 at org.apache.cxf.transport.http_jetty.JettyHTTPDestination.finalizeConfig(JettyHTTPDestination.java:146)
 at org.apache.cxf.transport.http.HTTPTransportFactory.getDestination(HTTPTransportFactory.java:278)
 at org.apache.cxf.binding.soap.SoapTransportFactory.getDestination(SoapTransportFactory.java:134)
 at org.apache.cxf.endpoint.ServerImpl.initDestination(ServerImpl.java:92)
 at org.apache.cxf.endpoint.ServerImpl.<init>(ServerImpl.java:71)
 at org.apache.cxf.frontend.ServerFactoryBean.create(ServerFactoryBean.java:158)
 at org.apache.cxf.jaxws.JaxWsServerFactoryBean.create(JaxWsServerFactoryBean.java:202)
 at com.test.ServerTest.<init>(ServerTest.java:19)
 at com.test.ServerTest.main(ServerTest.java:23)
缺少jar包：jetty-server-7.4.2.v20110526.jar
======================================================异常4=================================================================
2011-7-14 10:31:02 org.apache.cxf.service.factory.ReflectionServiceFactoryBean buildServiceFromClass
信息: Creating Service {http://services.ws.com/}IHelloServicesService from class com.ws.services.IHelloServices
Exception in thread "main" java.lang.NoClassDefFoundError: org/eclipse/jetty/http/HttpBuffers
 at java.lang.ClassLoader.defineClass1(Native Method)
 at java.lang.ClassLoader.defineClass(ClassLoader.java:620)
 at java.security.SecureClassLoader.defineClass(SecureClassLoader.java:124)
 at java.net.URLClassLoader.defineClass(URLClassLoader.java:260)
 at java.net.URLClassLoader.access$100(URLClassLoader.java:56)
 at java.net.URLClassLoader$1.run(URLClassLoader.java:195)
 at java.security.AccessController.doPrivileged(Native Method)
 at java.net.URLClassLoader.findClass(URLClassLoader.java:188)
 at java.lang.ClassLoader.loadClass(ClassLoader.java:306)
 at sun.misc.Launcher$AppClassLoader.loadClass(Launcher.java:268)
 at java.lang.ClassLoader.loadClass(ClassLoader.java:251)
 at java.lang.ClassLoader.loadClassInternal(ClassLoader.java:319)
 at org.apache.cxf.transport.http_jetty.JettyHTTPServerEngine.getHTTPConnectorFactory(JettyHTTPServerEngine.java:620)
 at org.apache.cxf.transport.http_jetty.JettyHTTPServerEngine.retrieveListenerFactory(JettyHTTPServerEngine.java:609)
 at org.apache.cxf.transport.http_jetty.JettyHTTPServerEngine.finalizeConfig(JettyHTTPServerEngine.java:659)
 at org.apache.cxf.transport.http_jetty.JettyHTTPServerEngineFactory.createJettyHTTPServerEngine(JettyHTTPServerEngineFactory.java:239)
 at org.apache.cxf.transport.http_jetty.JettyHTTPDestination.retrieveEngine(JettyHTTPDestination.java:113)
 at org.apache.cxf.transport.http_jetty.JettyHTTPDestination.finalizeConfig(JettyHTTPDestination.java:146)
 at org.apache.cxf.transport.http.HTTPTransportFactory.getDestination(HTTPTransportFactory.java:278)
 at org.apache.cxf.binding.soap.SoapTransportFactory.getDestination(SoapTransportFactory.java:134)
 at org.apache.cxf.endpoint.ServerImpl.initDestination(ServerImpl.java:92)
 at org.apache.cxf.endpoint.ServerImpl.<init>(ServerImpl.java:71)
 at org.apache.cxf.frontend.ServerFactoryBean.create(ServerFactoryBean.java:158)
 at org.apache.cxf.jaxws.JaxWsServerFactoryBean.create(JaxWsServerFactoryBean.java:202)
 at com.test.ServerTest.<init>(ServerTest.java:19)
 at com.test.ServerTest.main(ServerTest.java:23)
缺少jetty-http-7.4.2.v20110526.jar
======================================================异常5=================================================================
Caused by: com.sun.xml.bind.v2.runtime.IllegalAnnotationsException: 2 counts of IllegalAnnotationExceptions 
java.util.Map is an interface, and JAXB can't handle interfaces. 
        this problem is related to the following location: 
                at java.util.Map 
                at private java.util.Map com.cecltd.service.jaxws_asm.SayHi.arg0 
                at com.cecltd.service.jaxws_asm.SayHi 
java.util.Map does not have a no-arg default constructor.         this problem is related to the following location: 
                at java.util.Map 
                at private java.util.Map com.cecltd.service.jaxws_asm.SayHi.arg0 
                at com.cecltd.service.jaxws_asm.SayHi 
解决方法：将服务端的services接口返回的Map类型的值，改成HashMap.
结论：在做webServices复杂类型值传递时，返回值的类型不要用接口类型。例如(List应该改为ArrayList,Map改为HashMap等)
