# freemarker中页面直接可以使用的内置对象 - z69183787的专栏 - CSDN博客
2015年06月11日 17:53:06[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2329
**Request**： 用于获取Request对象中的attribute对象。
例如：${Request["method"]} 这样是直接在页面输出属性值。相当于request.getAtrribute("method");
**如果要对这个值进行判断就必须使用如下格式：<#if Request["method"]="edit">**
**Session**：用于获取Session对象中的attribute对象。
用法参照Request的用法。
**Application**：用于获取Application(ServletContext)对象中的attribute对象。
用法参照Request的用法。
**RequestParameters**：用于获取Request对象的parameter参数（浏览器端发送的请求数据）
例如：${RequestParameters["method"]}等同于request.getParameter("method");
**Parameters**：属性获取，依次从RequestParameters、Request、Session、Application对象中获取对应属性\参数，一旦获取，则不再向下查找。
例如：${Parameters["method"]}
