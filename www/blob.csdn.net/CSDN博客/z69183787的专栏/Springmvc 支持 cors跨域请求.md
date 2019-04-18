# Springmvc 支持 cors跨域请求 - z69183787的专栏 - CSDN博客
2016年11月09日 17:55:33[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6033
1、spring版本 4.3.2 + 
2、xml配置：
<?xml version="1.0" encoding="UTF-8"?>
<beansxmlns="http://www.springframework.org/schema/beans"
xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
xmlns:context="http://www.springframework.org/schema/context"
xmlns:mvc="http://www.springframework.org/schema/mvc"
xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd
       http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context.xsd
       http://www.springframework.org/schema/mvc http://www.springframework.org/schema/mvc/spring-mvc.xsd">
<context:component-scanbase-package="xxxx.action"/>
<context:annotation-config/>
<mvc:annotation-driven/>
<mvc:cors>
<mvc:mappingpath="/xxxx/xxxx"/>
</mvc:cors>
<beanid="multipartResolver"class="org.springframework.web.multipart.commons.CommonsMultipartResolver"/>
<!-- 静态资源映射 -->
<mvc:resourcesmapping="/xxxx/img/**"location="/WEB-INF/img/"/>
</beans>
更复杂的配置：
```
<mvc:cors>  
  
    <mvc:mapping path="/api/**"  
        allowed-origins="http://domain1.com, http://domain2.com"  
        allowed-methods="GET, PUT"  
        allowed-headers="header1, header2, header3"  
        exposed-headers="header1, header2" allow-credentials="false"  
        max-age="123" />  
  
    <mvc:mapping path="/resources/**"  
        allowed-origins="http://domain1.com" />  
  
</mvc:cors>
```
http://www.springframework.org/schema/mvc/spring-mvc.xsd
xsd中可以看到，cors配置各个配置项的默认取值
<xsd:elementname="cors">
<xsd:annotation>
<xsd:documentation>
<![CDATA[ Configure cross origin requests processing. ]]>
</xsd:documentation>
</xsd:annotation>
<xsd:complexType>
<xsd:sequence>
<xsd:elementname="mapping"minOccurs="1"maxOccurs="unbounded">
<xsd:annotation>
<xsd:documentation>
<![CDATA[
Enable cross origin requests processing on the specified path pattern. By default, all origins, GET HEAD POST methods, all headers and credentials are allowed and max age is set to
 30 minutes.
]]>
</xsd:documentation>
</xsd:annotation>
<xsd:complexType>
<xsd:attributename="path"type="xsd:string"use="required">
<xsd:annotation>
<xsd:documentation>
<![CDATA[
A path into the application that should handle CORS requests. Exact path mapping URIs (such as "/admin") are supported as well as Ant-stype path patterns (such as /admin/**).
]]>
</xsd:documentation>
</xsd:annotation>
</xsd:attribute>
<xsd:attributename="allowed-origins"type="xsd:string">
<xsd:annotation>
<xsd:documentation>
<![CDATA[
Comma-separated list of origins to allow, e.g. "http://domain1.com, http://domain2.com". The special value "*" allows all domains (default).
]]>
</xsd:documentation>
</xsd:annotation>
</xsd:attribute>
<xsd:attributename="allowed-methods"type="xsd:string">
<xsd:annotation>
<xsd:documentation>
<![CDATA[
Comma-separated list of HTTP methods to allow, e.g. "GET, POST". The special value "*" allows all method. By default GET, HEAD and POST methods are allowed.
]]>
</xsd:documentation>
</xsd:annotation>
</xsd:attribute>
<xsd:attributename="allowed-headers"type="xsd:string">
<xsd:annotation>
<xsd:documentation>
<![CDATA[
Comma-separated list of headers that a pre-flight request can list as allowed for use during an actual request. The special value of "*" allows actual requests to send any header (default).
]]>
</xsd:documentation>
</xsd:annotation>
</xsd:attribute>
<xsd:attributename="exposed-headers"type="xsd:string">
<xsd:annotation>
<xsd:documentation>
<![CDATA[
Comma-separated list of response headers other than simple headers (i.e. Cache-Control, Content-Language, Content-Type, Expires, Last-Modified, Pragma) that an actual response might
 have and can be exposed. Empty by default.
]]>
</xsd:documentation>
</xsd:annotation>
</xsd:attribute>
<xsd:attributename="allow-credentials"type="xsd:boolean">
<xsd:annotation>
<xsd:documentation>
<![CDATA[
Whether user credentials are supported (true by default).
]]>
</xsd:documentation>
</xsd:annotation>
</xsd:attribute>
<xsd:attributename="max-age"type="xsd:long">
<xsd:annotation>
<xsd:documentation>
<![CDATA[
How long, in seconds, the response from a pre-flight request can be cached by clients. 1800 seconds (30 minutes) by default.
]]>
</xsd:documentation>
</xsd:annotation>
</xsd:attribute>
</xsd:complexType>
</xsd:element>
</xsd:sequence>
</xsd:complexType>
</xsd:element>
