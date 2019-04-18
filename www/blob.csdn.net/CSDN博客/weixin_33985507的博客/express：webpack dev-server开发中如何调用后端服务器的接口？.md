# express：webpack dev-server开发中如何调用后端服务器的接口？ - weixin_33985507的博客 - CSDN博客
2016年08月24日 17:32:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
**开发环境：**
    前端：webpack + vue + vue-resource，基于如下模板创建的开发环境： [https://github.com/vuejs-templates/webpack](https://github.com/vuejs-templates/webpack)
    后端：java, tomcat
**问题：**
    前端开发人员在使用webpack + vue的开发环境调试时，发现在所有需要调用后端接口的地方就很麻烦，目前处理如下：
    1. 将每个接口的返回值放在一个json文件中，这些json文件统一放到static目录下
    2. 开发的时候，然后所有需要调用后端接口的地方，就先把路径指向static下对应的json文件名，获取数据；
    3. 发布的时候，将所有接口的路径改为正常的路径。
    这样每次发布都很麻烦，而且很容易出错。
**分析：**
    通过分析webpack的build/dev-server.js文件，发现这个是使用express来实现的一个简单的web服务器，那么就考虑是否可以参考自己写一个express的中间件，将所有对于后端接口的调用都转为调用真正的后端接口，这样在发布的时候就不需要修改路径了。
**解决方法：**
    通过分析 build/dev-server.js 源码，发现里面有用到http-proxy-middleware（[https://npm.taobao.org/package/http-proxy-middleware）](https://npm.taobao.org/package/http-proxy-middleware%EF%BC%89)，并且在代码中有注释：*Define HTTP proxies to your custom API backend * ，这不就是我想要的吗？
**    然后看后面的代码中去读取config/index.js内的dev.proxyTable来对http-proxy-middleware做初始化，所以我就在config/index.js的proxyTable中添加了如下内容：**
***    proxyTable: {***
***        '/appserver/SinglePowerStation': 'http://192.168.1.82:8899',***
***        '/appserver/powerStationManage': 'http://192.168.1.82:8899',***
***    },***
   这里有几点要说明：
    0）我要实现的目的是把所有以 *SinglePowerStation、**powerStationManage*开头的接口调用都转到后端服务器上；
    1）我的后端服务器地址是[http://192.168.1.82:8899](http://192.168.1.82:8899)
     2）后端接口前缀只有*SinglePowerStation、**powerStationManage*两种；
    至于proxyTable的格式是通过分析build/dev-server.js内的如下代码总结出来的：
> 
*Object.keys(proxyTable).forEach(function (context) {*
*  var options = proxyTable[context]*
*  if (typeof options === 'string') {*
*    options = { target: options }*
*  }  *
*  app.use(proxyMiddleware(context, options))*
*})*
——————完———————
[来自为知笔记(Wiz)](http://www.wiz.cn/i/7ebd14db)
