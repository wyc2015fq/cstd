# Android hybrid App项目构建和部分基本开发问题 - =朝晖= - 博客园
# [Android hybrid App项目构建和部分基本开发问题](https://www.cnblogs.com/dhcn/p/7115413.html)
      1、首先是选型：Cordova+Ionic Framework，调试[测试](http://lib.csdn.net/base/softwaretest)环境是Ripple Emulator.开发环境其实可以随便选，我个人选择了Eclipse，当然[Android](http://lib.csdn.net/base/android) SDK+ADT也是必须的.
      2、安装：在这个[js](http://lib.csdn.net/base/javascript)横行的时代，Cordova、IonicFramework、Ripple这三者都可以通过[Node.js](http://lib.csdn.net/base/nodejs)中的npm安装，非常方便。[android](http://lib.csdn.net/base/android) SDK和ADT的安装都比较大众，不过Cordova除了npm install还要做一些设置完整文档见其[官方文档](http://cordova.apache.org/docs/en/3.5.0/guide_platforms_android_index.md.html#Android%20Platform%20Guide),Cordova搞定，Ionic就方便了。
      3、新建项目：用ionic的指令新建项目，用Cordova添加platform，然后用ripple就可以模拟终端调试，这种方式相对比较方便，不像真正的AVD，那个光启动就要老命了。可以界面整体搞定以后，在启用AVD Emulator测试，这样启动的次数减少，节省时间。
      4、说说Ripple的作用,其实hybrid App 的WebApp部分主要是本地页面+远程Restful Json数据，由于普通浏览器有跨域限制，于是乎本地页面无法访问远程服务，于是Ripple先是架一个localhost服务，开放本地文件，其次它把那部分远程需求通过中间代理的方式转发给真正远程服务。之前好像也有一套DNS之类的方法解决这个问题，不过没有工具这么简便易用。
      5、补充库，在PCWeb上，一般[jQuery](http://lib.csdn.net/base/jquery)可以和[AngularJS](http://lib.csdn.net/base/angularjs)一起用，不过在ionic前提下，[jquery](http://lib.csdn.net/base/jquery)好像和ionic有一定不兼容，而angular.elementopen的jqlite函数也很有限，比如典型的post config用到的$.param(),这个时候就可以用zepto.js取代，轻量级jQ语法兼容库，专为移动打造，且和ionic不会冲突。
      6、关于CSRF的问题,由于本地HTML不可能有服务器动态标签，所以使用Ajax请求的CSRF Token传递方法,也就是说通过header传递，这方面[angularjs](http://lib.csdn.net/base/angularjs) $http的Config提供对应的支持，意味着你不需要每次post的时候还要去读Cookie，这些AngularJS都替你提供横向支持。不过这种支持方法因跨域原因无法在hybridApp中使用，你得用直接的请求来得到csrftoken，然后用angularjs拦截器注入到每个POST请求里面，因为这实际上是异步跨请求的数据依赖，这种依赖很难保证肯定达成，所以最好不要用CRSF机制，这样比较干净一点，毕竟是hybridApp，界面简化以后，可能很少用到用户输入的富文本。
      7、关于跨域的Cookie问题，hybrid app的本地脚本和远程服务不在同一个域，意味着无法跨域访问。远程服务的Cookie只能用来它自己从Request数据里面读，本地代码是无法读，当然这个问题，可以通过一些跨域设置来解决，不过那个可能会存在安全问题，之前本地需要读的Cookie数据，可以直接放在返回JSON数据里面，这样就可以实现之前的数据传递了。
     8、本地数据缓存问题，hybrid App要达到原生App的体验，则必须有一个SQLite级的东西来用，这就是localStorage或者sessionStorage，Storage目前只能存放字符串,不过比Cookie的解析序列化要方便多了。当然还有写WebDB级的东西来用，我个人觉得其操作没有直接JS数据操作方便。
     9、ionic的项目，AVD测试必须用ionic的bulid命令构建、Run命令测试。在Eclipse里面run as是不行的，我估计主要是build的原因。
     10、发布前的处理，ionic官方文档GuideBook部分有这个内容，其中少了一条，就是要修改Package Name，这个东西是发布后APK的一个标识性的东西，要改成自己的，其实很简单，新建一个自己产品的com.xxx.xxx的包，把原先的那个移到这个包里面，改一下config就Ok了。
