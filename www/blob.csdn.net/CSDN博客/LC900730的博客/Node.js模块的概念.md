# Node.js模块的概念 - LC900730的博客 - CSDN博客
2017年08月26日 17:14:35[lc900730](https://me.csdn.net/LC900730)阅读数：211
## 模块：原生模块和文件模块
原生模块：启动的时候就被加载 
文件模块：动态加载模块。加载文件模块的 工作主要由原生模块module来实现和完成。原生模块在启动时候已经被加载，而文件模块则需要通过调用require方法来加载。
### 1.原生模块的调用
require加载成功后返回一个node.js模块的对象，该对象拥有该模块的所有属性和方法。
```
var httpModule=require('http');
"http":HTTP是Node.js提供的原生模块，该模块中有createServer、request和get等多个方法和属性
```
### 2.文件模块调用方法
原生模块不需要指定模块路径。原生模块不需要指定模块路径，而文件模块加载时必须指定文件路径
```
var test=require('/path/../test.js');
var test=require('/path/../test');
/开始代表着该test文件模块的绝对路径，相对路径则./开始
```
在Node.js中明确指出，在文件模块中，只有exports和module.exports对象暴露给该外部的属性和方法，才能够通过返回的require对象进行调用，其他对象和属性是无法获取的。 
### 文件处理
File System模块介绍 
文件IO是由标准POSIX函数封装而成的，需要使用require(‘fs’)访问这个模块。所有方法都提供了同步和异步2种方式 
fs.rename(path1,path2,[callback])异步调用函数对应fs.renameSync(path1,path2)同步调用函数，作用功能都是相同的 ，但是接口是一个同步接口，因此调用方式有所区别。
### 路由处理
根据不同客户端的请求资源路径，来分配服务器处理逻辑。
#### 特定规则
可以根据用户请求的url，依据特定的规则得到执行函数。
### GET和POST
Node.js中HTTP客户端发送的GET请求参数都存储在request对象中的url属性中。
