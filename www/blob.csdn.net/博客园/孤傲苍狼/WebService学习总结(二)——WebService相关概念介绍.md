# WebService学习总结(二)——WebService相关概念介绍 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [WebService学习总结(二)——WebService相关概念介绍](https://www.cnblogs.com/xdp-gacl/p/4259109.html)



## 一、WebService是什么？

　　1. 基于Web的服务：服务器端整出一些资源让客户端应用访问（获取数据）

　　2. 一个跨语言、跨平台的规范（抽象）

　　3. 多个跨平台、跨语言的应用间通信整合的方案（实际）

## 二、为什么要用Web service？

　　web service能解决：
- 跨平台调用
- **跨语言调用**
- 远程调用

## 三、什么时候使用web Service?

　　1. 同一家公司的新旧应用之间

　　2. 不同公司的应用之间

　　3. 一些提供数据的内容聚合应用：天气预报、股票行情

## 四、Web Service中的几个重要术语

### 4.1、WSDL(web service definition language)

WSDL是webservice定义语言, 对应.wsdl文档, 一个webservice会对应一个唯一的wsdl文档, 定义了客户端与服务端发送请求和响应的数据格式和过程

### 4.2、SOAP(simple object  access protocal)

　　SOAP是"**简单对象访问协议**"
- 是一种简单的、基于**HTTP****和**[**XML**](http://baike.baidu.com/view/63.htm)**的协议**, 用于在WEB上交换**结构化的数据**
- soap消息：**请求消息**和**响应消息**

### 4.3、SEI(WebService EndPoint Interface)

**　　SEI是web service****的终端接口，**就是WebService服务器端用来处理请求的接口

### 4.4、CXF(Celtix + XFire)

　　一个apache的用于开发webservice服务器端和客户端的框架。









