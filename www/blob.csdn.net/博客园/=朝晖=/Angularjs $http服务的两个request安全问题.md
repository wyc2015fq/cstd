# Angularjs $http服务的两个request安全问题 - =朝晖= - 博客园
# [Angularjs $http服务的两个request安全问题](https://www.cnblogs.com/dhcn/p/7115442.html)
      今天为了hybrid app和后端restful服务的安全认证问题，又翻了一下$http的文档，$http服务文档页面两个安全问题是json和XSRF，JSON那个比较好理解，就不补充什么了，说说XSRF和CORS(跨域HTTP请求)的访问凭证问题
      1、XSRF问题又叫CSRF问题跨站请求伪造，一般来讲实在cookie放一个Token，当提交post请求时取出token，把Token放到post的请求data或者header里面，一般Ajax请求会放在header里面，这样比较容易横向设置，这个token传到后端，再由后端取出来和后端session里面的数据做对比验证。因为[AngularJS](http://lib.csdn.net/base/angularjs)的$http发送请求，其数据都是传object或者自己组object，不像[jQuery](http://lib.csdn.net/base/jquery)，直接取整个form的所有数据，而像有些框架的CSRF Token，hacking直接绑定model好像不太nice，所以设置XSRF Cookie Name的方式，$http会自己直接从Cookie里面去取这个token，放到header里面，这样就不用每次发送post请求时，自己去想法设法拿那个值了。
      2、跨域HTTP请求(CORS)的访问控制问题，目前的Ajax XMLHTTPRequest标准已经允许一定条件下的跨域请求，hybrid的页面等资源都在apk里面，所以其数据请求肯定是跨域的，这个问题我说起来有点话长，我就不详述了,[angularjs](http://lib.csdn.net/base/angularjs)文档给了个链接：https://developer.mozilla.org/en-US/docs/Web/HTTP/Access_control_CORS#Requests_with_credentials ,目前一些比较激进的[HTML5](http://lib.csdn.net/base/html5)或者[js](http://lib.csdn.net/base/javascript)书籍里面也有跨域请求的介绍，比如《[html5](http://lib.csdn.net/base/html5)程序设计》

