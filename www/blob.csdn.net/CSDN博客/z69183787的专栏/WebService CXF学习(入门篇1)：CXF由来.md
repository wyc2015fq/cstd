# WebService CXF学习(入门篇1)：CXF由来 - z69183787的专栏 - CSDN博客
2014年06月30日 11:21:51[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1055
WebService介绍
   WebService让一个程序可以透明地调用互联网程序，不用管具体的实现细节。只要WebService公开了服务接口，远程客户端就可以调用服务。WebService是基于http协议的组件服务，WebService是分散式应用程序的发展趋势。 
WebService的开源实现
   WebService更多是一种标准，而不是一种具体的技术。不同的平台，不同的语言大都提供WebService的开发实现。在JAVA领域，WebService的框架很多，例如：AXIS，XFire,CXF等。AXIS，XFire相对比较成熟，资料相对较多。在这里我们只对CXF进行讲解，其他大家想学习到互联网找相关资料学习。 
CXF框架由来
   ApacheCXF项目是由ObjectWeb Celtix和CodeHaus XFire合并成立。ObjectWeb Celtix是由IONA公司赞助，于2005年成立的开源Java ESB产品，XFire则是业界知名的SOAP堆栈。合并后的ApacheCXF融合该两个开源项目的功能精华，提供了实现SOA所需要的核心ESB功能框架，包括SOA服务创建，服务路由，及一系列企业级QoS功能。 
ApacheCXF架框的目标
   1.概述 
       <>高性能 
       <>可扩展 
       <>直观易用 
   2.支持标准 
       <> JAX-WS, JSR-181, SAAJ, JAX-RS 
       <> SOAP 1.1, 1.2, WS-I BasicProfile, WS-Security, WS-Addressing, WS-RM, WS-Policy 
       <> WSDL 1.1 
       <> MTOM 
   3.传输方式，绑定，数据绑定，传送数据格式 
       <> 绑定: SOAP, REST/HTTP 
       <> 数据绑定: JAXB 2.x, Aegis, XMLBeans, SDO 
       <> 传送数据格式: XML, JSON, FastInfoset 
       <> 传输方式: HTTP, Servlet, JMS 
   4.部署灵活 
       <> 轻量级: 可以将服务部署到 Tomcat或其支持Spring的容器中 
       <> JBI整合: 部署一个服务引擎到JBI容器，例如： ServiceMix, OpenESB or Petals 
       <> J2EE集成: 可以将服务部署到J2EE应用服务器上，例如：Geronimo, JOnAS, JBoss, WebLogic, 及WebSphere 
       <> Java 客户端/服务端可以独立性 
   5.支持多种编程语言 
       <> 完全支持 JAX-WS 2.x 客户端/服务端模式 
       <> JAX-WS 2.x synchronous, asynchronous and one-way API's 
       <> JAX-WS 2.x Dynamic Invocation Interface (DII) API 
       <> 支持 wrapped and non-wrapped 数据格式 
       <> XML messaging API 
       <> 支持JavaScript 和 ECMAScript 4 XML (E4X) 
       <> 支持CORBA 
       <> 支持JBI及ServiceMix 
   6.可编程环境 
       <> Java to WSDL 
       <> WSDL to Java 
       <> XSD to WSDL 
       <> WSDL to XML 
       <> WSDL to SOAP 
       <> WSDL to service
