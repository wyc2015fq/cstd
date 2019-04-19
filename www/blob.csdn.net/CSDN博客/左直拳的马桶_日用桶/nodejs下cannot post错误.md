# nodejs下cannot post错误 - 左直拳的马桶_日用桶 - CSDN博客
2014年11月21日 16:52:37[左直拳](https://me.csdn.net/leftfist)阅读数：4577
我写了一段CoffeeScript代码，主要是流程为：
1、当客户端请求方式为GET时，输出页面，页面上有个form，form 里有个submit按钮。form的 action=""，则默认是提交给它自己
2、当客户端请求方式为POST时，则处理相关
代码文件为logout.coffee，代码如下：
```
module.exports = (req, res) ->
  if req.method is 'GET'
    req.session = null
    template  = 'logout.html'
    return res.render(template, null)
  res.writeHead(303, Location: "/login")
  res.end()
```
结果在页面中，按下提交按钮的时候，报错：说不能提交。在浏览器中按F12检查，说是404的错误，页面找不到。真奇怪了，难道是没指定action？事实证明指定其实也无效。
后来网上查了查，醒悟到是路由规则这里的问题：
  app.get '/logout', account.logout
只指定了get。加上post
  app.post '/logout', account.logout
问题解决。也可以这样：
  app.all '/logout', account.logout
node.js比较灵活，精悍，同时也要求开发者对http协议有基本的了解。习惯了微软的IIS，人快变傻逼了。微软体系中，程序员跟一般软件应用人员其实没有多大区别，使用IIS跟使用office差不多，这是我的悲哀。
可惜醒悟得太晚了
