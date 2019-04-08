# 最近整理的一些常见的面试题，面试大全，黑马程序员面试宝典题库---JavaWEE高级篇

2018年09月03日 16:21:37 [高数老师](https://me.csdn.net/qq_29073921) 阅读数：94



 版权声明：代码共享，欢迎copy	https://blog.csdn.net/qq_29073921/article/details/82350213

## 一、 Filter 和 Listener（评论留言获取原件）


可参见：<https://www.cnblogs.com/libingbin/p/5985647.html>

## 二、 AJAX

###  1. 谈谈你对 ajax 的认识？


Ajax 是一种创建交互式网页应用的的网页开发技术； Asynchronous JavaScript and XML”的缩写。
**Ajax 的优势：**
    通过异步模式，提升了用户体验。
    优化了浏览器和[服务器](https://www.baidu.com/s?wd=%E6%9C%8D%E5%8A%A1%E5%99%A8&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)之间的传输，减少不必要的数据往返，减少了带宽占用。
    Ajax [引擎](https://www.baidu.com/s?wd=%E5%BC%95%E6%93%8E&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)在客户端运行，承担了一部分本来由服务器承担的工作，从而减少了大用户量下的服务器负载。
**Ajax 的最大特点：**
    可以实现局部刷新，在不更新整个页面的前提下维护数据，提升用户体验度。
注意：
    ajax 在 实 际 项 目 开 发 中 使 用 率 非 常 高 （ 牢 固 掌 握 ） ， 针 对 ajax 的 详 细 描 述 ：
   <https://www.jb51.net/article/93258.htm>

### 2. jsonp 原理


JavaScript 是一种在 Web 开发中经常使用的前端动态脚本技术。在 JavaScript 中，有一个很重要的安全性限制，被称为“Same-Origin Policy”（同源策略）。这一策略对于 JavaScript 代码能够访问的页面内容做了很重要的限制，即 JavaScript 只能访问与包含它的文档在同一域下的内容。
JavaScript 这个安全策略在进行多 iframe 或多窗口编程、以及 Ajax 编程时显得尤为重要。根据这个策略，在 baidu.com 下的页面中包含的 JavaScript 代码，不能访问在 google.com 域名下的页面内容；甚至不同的子域名之间的页面也不能通过 JavaScript 代码互相访问。对于 Ajax 的影响在于，通过 XMLHttpRequest 实现的Ajax 请求，不能向不同的域提交请求，例如，在 abc.example.com 下的页面，不能向 def.example.com 提交Ajax 请求，等等。
然而，当进行一些比较深入的前端编程的时候，[不可避免](https://www.baidu.com/s?wd=%E4%B8%8D%E5%8F%AF%E9%81%BF%E5%85%8D&tn=24004469_oem_dg&rsv_dl=gh_pl_sl_csd)地需要进行跨域操作，这时候“同源策略”就显得过于苛刻。 JSONP 跨域 GET 请求是一个常用的解决方案，下面我们来看一下 JSONP 跨域是如何实现的，并且探讨下 JSONP 跨域的原理。
jsonp 的最基本的原理是：动态添加一个<script>标签， 使用 script 标签的 src 属性没有跨域的限制的特点实现跨域。首先在客户端注册一个 callback, 然后把 callback 的名字传给服务器。此时，服务器先生成 json 数据。 然后以 javascript 语法的方式，生成一个 function , function 名字就是传递上来的参数 jsonp。最后将json 数据直接以入参的方式，放置到 function 中，这样就生成了一段 js 语法的文档，返回给客户端。
客户端浏览器，解析 script 标签，并执行返回的 javascript 文档，此时数据作为参数，传入到了客户端预先定义好的 callback 函数里。
参考资料： <http://www.nowamagic.net/librarys/veda/detail/224>

## 三、 Linux

###  4. 说一下常用的 Linux 命令


列出文件列表： ls 【参数 -a -l】
创建目录和移除目录： mkdir rmdir
用于显示文件后几行内容： tail
打包： tar -xvf
打包并压缩： tar -zcvf
查找字符串： grep
显示当前所在目录： pwd
创建空文件： touch
编辑器： vim vi
列出文件列表： ls 【参数 -a -l】
创建目录和移除目录： mkdir rmdir
用于显示文件后几行内容： tail
打包： tar -xvf
打包并压缩： tar -zcvf
查找字符串： grep
显示当前所在目录： pwd
创建空文件： touch
编辑器： vim vi
 

### 5. Linux 中如何查看日志？


动态打印日志信息： tail –f 日志文件
参考资料： <https://www.cnblogs.com/zdz8207/p/linux-log-tail-cat-tac.html>

### 6. Linux 怎么关闭进程


通常用 ps 查看进程 PID ，用 kill 命令终止进程。
ps 命令用于查看当前正在运行的进程。
grep 是搜索
例如： ps -ef | grep java
表示查看所有进程里 CMD 是 java 的进程信息。
ps -aux | grep java
-aux 显示所有状态
kill 命令用于终止进程。
例如： kill -9 [PID]
-9 表示强迫进程立即停止。

##  四、 常见的前端框架有哪些

###  1. EasyUI（2017-11-23-lyq）


EasyUI 是一种基于 jQuery 的用户界面插件集合。 easyui 为创建现代化，互动， JavaScript 应用程序，提供
必要的功能。使用 easyui 你不需要写很多代码，你只需要通过编写一些简单 HTML 标记，就可以定义用户界面。
优势：开源免费，页面也还说的过去。

###  2. MiniUI（2017-11-23-lyq）


基于 jquery 的框架，开发的界面功能都很丰富。 jQuery MiniUI - 快速开发 WebUI。它能缩短开发时间，减少
代码量，使开发者更专注于业务和服务端，轻松实现界面开发，带来绝佳的用户体验。使用 MiniUI，开发者可以快速
创建 Ajax 无刷新、 B/S 快速录入数据、 CRUD、 Master-Detail、菜单工具栏、弹出面板、布局导航、数据验证、
分页表格、树、树形表格等典型 WEB 应用系统界面。 缺点：收费，没有源码，基于这个开发如果想对功能做扩展就
需要找他们的团队进行升级！

###  3. jQueryUI（2017-11-23-lyq）


jQuery UI 是一套 jQuery 的页面 UI 插件，包含很多种常用的页面空间，例如 Tabs（如本站首页右
上角部分） 、拉帘效果（本站首页左上角）、对话框、拖放效果、日期选择、颜色选择、数据排序、窗体大小调整等
等非常多的内容。

###  4. Vue.js（2017-11-23-lyq）


参考原文：<https://cn.vuejs.org/v2/guide/>

### 5. AngularJS （2017-11-23-lyq）


参考原文： <http://www.angularjs.net.cn/api/>

 