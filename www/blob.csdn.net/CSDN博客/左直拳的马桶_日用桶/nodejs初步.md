# nodejs初步 - 左直拳的马桶_日用桶 - CSDN博客
2014年11月26日 09:11:02[左直拳](https://me.csdn.net/leftfist)阅读数：1578
nodejs是啥？
看名字，很容易认为它是一种开发语言，实质上，它更像是一种WEB服务器，一种工具。因为nodejs的作用，在于在服务器端解释、运行javascript。node.js本身不是开发语言，在node.js体系里面，语言说到底就是javascript。
当然这个体系充斥着各种各样的工具，其中包括CoffeeScript，也称得上是一种开发语言，但CoffeeScript最终会被编译成javascript。CoffeeScript其实只是为了方便写javascript而已，因为它比较精简，代码量不用写那么多，据说是借鉴了Ruby和Phyon。传闻Ruby的开发效率比较高，我没用过Ruby，但从CoffeeScript来看，可能是真的，因为代码量大约只占编译后javascript的一半多一点。刚开始的时候，看CoffeeScript会觉得精简过度，造成不好理解，后来习惯了就好了。
以下是跟nodejs有关的一些工具或语言：
npm：NodeJs包管理器
express：服务器端比较流行的MVC框架，处理服务请求，路由转发，逻辑处理
mongoose：mongodb包装，更方便使用数据库
socket.io：实现服务端和客户端socket通信解决方案
backbone：客户端MVC框架，编写客户端应用
coffeescript：用于开发JavaScript的开发语言
zombie：浏览器子集，编写html解析器，轻形javascript客户端测试
