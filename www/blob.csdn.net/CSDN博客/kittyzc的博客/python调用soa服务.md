# python调用soa服务 - kittyzc的博客 - CSDN博客
2018年10月15日 09:32:21[IE06](https://me.csdn.net/kittyzc)阅读数：215所属专栏：[python小tips](https://blog.csdn.net/column/details/31935.html)
公司项目需要调用基于soa接口的服务，几个技术人员用java配置了一整天都没有搞定，最后一怒之后我自己把要调用的功能重写了一遍。最近用python试了一下，结果一行代码就搞定了，真是感慨万千。

# 1. SOA

SOA是service-oriented architecture（面向服务的架构）的简写，主要是通过通用接口将服务包装并独立化。

常见的几种SOA规范有：
- SOAP、XMLRPC：http+xml。目前XMLPRC已经逐渐被SOAP取代。
- REST：http+json。SOAP使用xml，安全性更高；REST使用json，性能更好。有时候把REST和SOAP都称为REST，特点为：应用层协议（http）+序列化协议。
- RPC：底层协议（通常是tcp）+序列化协议。比如gRPC=http2+ProtoBuf，由谷歌开发。

下面是常见RPC的对比：
|*|Motan|Dubbox|thrift|gRPC|rpcx|
|----|----|----|----|----|----|
|开发语言|Java|Java|跨语言|跨语言|go|
|底层协议|motan|Dubbo、 Rmi 、 Hessian、 http、 WebService 、Dubbo|Thrift 、Memcached|tpc/http/frame|http2|
|消息序列化|hessian2,json|hessian2,json,resr,kyro,FST等，可扩展protobuf等|thrift|protobuf|Gob、Json、MessagePack、gencode、ProtoBuf等|

# 2. python启用SOA

这里举两个例子：

# 2.1 基于SOAP

安装soappy：

```
pip install soappy
```

建立一个python文件，内容如下，然后运行启用服务：

```python
import SOAPpy
def hello(a):
    return "Hello "+a
server = SOAPpy.SOAPServer(("localhost", 7788))
server.registerFunction(hello)
server.serve_forever()
```

# 2.2 基于XMLRPC

使用自带的SimpleXMLRPCServer：

```python
from SimpleXMLRPCServer import SimpleXMLRPCServer
def add(a , b):
	return a+b
server = SimpleXMLRPCServer(("localhost", 8123))
server.register_function(add)
server.serve_forever()
```

# 3. python调用SOA

这里依旧举两个例子：

# 3.1 基于SOAP

使用方法如下：

```python
import SOAPpy  
server = SOAPpy.SOAPProxy("http://localhost:7788")  
print server.hello('chen')
```

输出

```
Hello chen
```

# 3.2 基于XMLRPC

使用方法为：

```python
import xmlrpclib
server = xmlrpclib.Server('http://localhost:8123/')
print server.add(1,2)
```

输出

```
3
```
