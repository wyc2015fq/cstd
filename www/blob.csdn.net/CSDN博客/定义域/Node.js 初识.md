# Node.js 初识 - 定义域 - CSDN博客
2016年02月26日 10:47:27[定义域](https://me.csdn.net/u010278882)阅读数：204
# 什么是 NodeJS
- Node 是 JavaScript语言服务器运行环境 
a. Node 作为 JS “虚拟机”，提供 JS 在服务器的运行环境。解释器用 Google V8。 
b. Node 作为 JS “工具库”，提供大量与操作系统互动的工具库。系统调用库用自行开发的 libuv。
# 什么是NVM
NVM(Node Version Manager) Node 版本管理器，是 Node.js 的版本管理工具。
当需要使用多个版本的 Node.js 时可以使用，此外还有 n 这一款 Node 版本管理工具。
# Hello, NodeJS
- 创建名为 “hello.js” 的文件
- 输入内容: `console.log('hello, nodejs')`
- 打开nodejs命令行, 输入: node hello
## 什么是 REPL 环境
除了能够执行脚本文件，命令行还有一种称为 REPL(read-eval-print-loop)，读取-计算-打印-循环的工作模式，可以直接解释执行脚本代码。
输入 node  则执行脚本文件，直接输入 node 则进入REPL环境，然后输入代码即可执行。
# NodeJS 源代码采用模块化结构组织
NodeJS 遵循 CommonJS 规范定义和使用模块。模块与文件一一对应，每个模块就是一个文件。
## 使用模块
通过 require(‘module’) 来引用模块
```java
var circle = require('./circle.js')
```
### 模块名的解析
1.如果指定的模块名包含完整路径, 则直接引用 
2. 如果找寻不到, 则到 Node 的安装目录中寻找已安装模块
### 模块可为目录
目录中包含多个文件, 通过 package.json 文件指定模块入口
```java
// 假设存在一个名为 bar 的模块, 其包含 lib 目录下的文件
// 则 package.json 为
{
    "name": "bar",
    "main": "./lib/bar.js"
}
```
# 参考
[Node.js 概述](http://javascript.ruanyifeng.com/nodejs/basic.html)
[利用 n 和 nvm 管理 Node 的版本](http://weizhifeng.net/node-version-management-via-n-and-nvm.html)
