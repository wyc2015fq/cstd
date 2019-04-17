# Zabbix二次开发_01基础 - DoubleLi - 博客园






最近有个想法：想做一个zabbix数据的二次呈现，所以来写一下Zabbix的api的内容。

先说下zabbix api的认证基础。

#  Zabbix API简介

Zabbix API开始扮演着越来越重要的角色，尤其是在集成第三方软件和自动化日常任务时。。Zabbix API为批量操作、第三方软件集成以及其他作用提供可编程接口。

Zabbix API是在1.8版本中开始引进并且已经被广泛应用。所有的Zabbix移动客户端都是基于API，甚至原生的WEB前端部分也是建立在它之上。Zabbix API 中间件使得架构更加模块化也避免直接对数据库进行操作。它允许你通过JSON
RPC协议来创建、更新和获取Zabbix对象并且做任何你喜欢的操作。

Zabbix API提供两项主要功能：
- 远程管理Zabbix配置
- 远程检索配置和历史数据



**使用JSON**

API 采用JSON-RPC实现。这意味着调用任何函数，都需要发送POST请求，输入输出数据都是以JSON格式。大致工作流如下：
- 
准备JSON对象，它描述了你想要做什么（创建主机，获取图像，更新监控项等）。

- 
采用POST方法向 http://example.com/zabbix/api_jsonrpc.php发送此JSON对象，http://example.com/zabbix/是Zabbix前端地址 ，api_jsonrpc.php是调用API的PHP脚本。可在安装可视化前端的目录下找到。

- 
获取JSON格式响应。

- 
注：请求除了必须是POST方法之外，HTTP Header Content-Type必须为【application/jsonrequest，application/json-rpc，application/json】其中之一。


可以采用脚本或者任何"手动"支持JSON RPC的工具来使用API。而首先需要了解的就是如何验证和如何使用验证ID来获取想要的信息。后面的演示会以Python脚本和基于Curl的例子来呈现API的基本使用。

**基本请求格式**

Zabbix API 简化的JSON请求如下：

```
![复制代码](https://common.cnblogs.com/images/copycode.gif)

 1 {
 2 "jsonrpc": "2.0",
 3 "method": "method.name",
 4 "params": {
 5 "param_1_name": "param_1_value",
 6 "param_2_name": "param_2_value"
 7 },
 8 "id": 1,
 9 "auth": "159121b60d19a9b4b55d49e30cf12b81",
10 }

![复制代码](https://common.cnblogs.com/images/copycode.gif)
```



下面一行一行来看：
- 
"jsonrpc": "2.0"-这是标准的JSON RPC参数以标示协议版本。所有的请求都会保持不变。

- 
"method": "method.name"-这个参数定义了真实执行的操作。例如：host.create、item.update等等

- 
"params"-这里通过传递JSON对象来作为特定方法的参数。如果你希望创建监控项，"name"和"key_"参数是需要的，每个方法需要的参数在Zabbix API文档中都有描述。

- 
"id": 1-这个字段用于绑定JSON请求和响应。响应会跟请求有相同的"id"。在一次性发送多个请求时很有用，这些也不需要唯一或者连续

- 
"auth": "159121b60d19a9b4b55d49e30cf12b81"-这是一个认证令牌【authentication token】用以鉴别用户、访问API。这也是使用API进行相关操作的前提-获取认证ID。




#  API 使用
- **环境准备**

Zabbix API是基于JSON-RPC 2.0规格，具体实现可以选择任何你喜欢的编程语言或者手动方式。这里我们采用的Python和基于Curl的方式来做示例。Python 2.7版本已经支持JSON，所以不再需要其他模块组件。当然可以采用Perl、Ruby、PHP之类的语言，使用前先确保相应JSON模块的安装。
- **身份验证**

任何Zabbix API客户端在真正工作之前都需要验证它自身。在这里是采用User.login方法。这个方法接受一个用户名和密码作为参数并返回验证ID，一个安全哈希串用于持续的API调用（在使用User.logout之前该验证ID均有效）。

# 认证获取简例

**1.     user.login方法获取zabbix server的认证结果****官方地址：**[https://www.zabbix.com/documentation/2.2/manual/api/reference/user/login](https://www.zabbix.com/documentation/2.2/manual/api/reference/user/login)
**python****脚本：**
|1234567891011121314151617181920212223242526272829303132|[iyunv@yang python]# cat auth.py#!/usr/bin/env python2.7#coding=utf-8import jsonimport urllib2# based url and required headerurl = "http://1.1.1.1/zabbix/api_jsonrpc.php"header = {"Content-Type":"application/json"}# auth user and passworddata = json.dumps({   "jsonrpc": "2.0",   "method": "user.login",   "params": {   "user": "Admin",   "password": "zabbix"},"id": 0})# create request objectrequest = urllib2.Request(url,data)for key in header:   request.add_header(key,header[key])# auth and get authidtry:   result = urllib2.urlopen(request)except URLError as e:   print "Auth Failed, Please Check Your Name AndPassword:",e.codeelse:   response = json.loads(result.read())   result.close()print"Auth Successful. The Auth ID Is:",response['result']|
|----|----|



**python****脚本运行结果：**
|12|[iyunv@yang python]# python auth.pyAuth Successful. The Auth ID Is: a0b82aae0842c2041386a61945af1180|
|----|----|



**curl****命令：**
|123|curl -i -X POST -H 'Content-Type:application/json' -d '{"jsonrpc":"2.0","method":"user.login","params":{"user":"admin","password":"zabbix"},"auth":null,"id":0}' [http://1.1.1.1/zabbix/api_jsonrpc.php](http://1.1.1.1/zabbix/api_jsonrpc.php)|
|----|----|



**curl****命令运行结果：**


|1|{"jsonrpc":"2.0","result":"b895ce91ba84fe247e444817c6773cc3","id":0}|
|----|----|

比较来看，采用脚本可以有更多的灵活性，而基于CURL的方式，对结果的处理不是很方便。原理则都是相通的。



除了这些获取信息以外，采用API调用同样可以进行创建操作，更新操作和删除操作等等。这也很容易让我们联想起数据库操作，当然比较这些采用API

调用获取结果的方式，也不能忘掉这种最直接而危险的方式。在开始讨论中已经提到，Zabbix现在自带的前端实现部分是采用数据库操作，部分是基于API
调用。



# API调用数据流程
- **数据流程**

下面的流程图代表了Zabbix API
工作的典型工作流。验证（方法user.login）是获取验证ID的强制步骤。这个ID又允许我们调用API提供的任何权限允许的方法来进行操作。在之
前的例子中没有提到user.logout方法，这也是一次验证ID能够重复使用的原因所在。使用user.logout方法后将会使验证ID失效，后面
的操作将不能再使用此ID。

![](https://images2015.cnblogs.com/blog/938959/201702/938959-20170213173522285-404655921.jpg)

from:http://www.cnblogs.com/yangliheng/p/6394573.html









