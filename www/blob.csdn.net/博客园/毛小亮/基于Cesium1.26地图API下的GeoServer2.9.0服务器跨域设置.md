# 基于Cesium1.26地图API下的GeoServer2.9.0服务器跨域设置 - 毛小亮 - 博客园
# [基于Cesium1.26地图API下的GeoServer2.9.0服务器跨域设置](https://www.cnblogs.com/xianerwonder/p/5980196.html)
**遇到的问题：**
　　最近基于Cesium来做3D模型的地图开发，在访问自己发布的WMS服务之后，遇到了GeoServer跨域问题。
　　调用这个WMS服务的时候，浏览器（我用Chrome）开发者工具报错：
![](https://images2015.cnblogs.com/blog/688944/201610/688944-20161020110300451-215493765.png)
控制台提示如下：
　　Font from origin '[http://](http://rainyin.com/)xxxxxxxxxxxxx' has been blocked from loading by Cross-Origin Resource Sharing policy: No 'Access-Control-Allow-Origin' header is present on the requested resource. Origin 'http://localhost' is therefore not allowed access.
这是因为服务端不允许跨域访问所导致的问题。
**解决方案：**
1、下载官方的cors压缩包，解压得到两个jar包，两个jar包文件放入geoserver目录下web容器的类库文件夹中。
2、打开geoserver目录下web容器的中的xml配置文件。
3、添加过滤器代码和过滤器路由代码。
4、重启geoserver。
ps:如果目录中存在maven，需要在pom.xml中，添加一个依赖标签。
**pss：为了服务大家，本人制作了*[压缩包](http://download.csdn.net/detail/mm190835773/9658758)***
**psss:经本人测试，这个跨域访问方法可以应用于大多数服务器，不仅仅局限于GeoServer**
**CORS(跨域资源共享)小课堂：**
CORS需要浏览器和服务器同时支持。目前，所有浏览器都支持该功能（IE浏览器不能低于IE10）。
　　整个CORS通信过程，都是浏览器自动完成，不需要用户参与。浏览器一旦发现请求跨源，就会自动添加一些附加的头信息，但用户不会有感觉。
　　因此，实现CORS通信的关键是**服务器**。只要服务器实现了CORS接口，就可以跨源通信。
**请求流程：**
　　　　1、在你发送任意请求时，浏览器会将CORS请求合并到http请求之中。具体来说，就是在头信息之中，增加一个`Origin`字段。
![](https://images2015.cnblogs.com/blog/688944/201610/688944-20161020114156498-1499457179.png)
　　　　2、上面的头信息中，`Origin`字段用来说明，本次请求来自哪个源（协议 + 域名 + 端口）。服务器根据这个值，决定是否同意这次请求。
　　　　3、如果`Origin`指定的源，不在许可范围内，服务器会返回一个正常的HTTP回应。
　　　　4.1、浏览器发现，这个回应的头信息没有包含`Access-Control-Allow-Origin`字段，就知道出错了，从而抛出一个错误，被`XMLHttpRequest`的`onerror`回调函数捕获。
　　　　4.2、如果`Origin`指定的域名在许可范围内，服务器返回的响应，会多出几个头信息字段。
![](https://images2015.cnblogs.com/blog/688944/201610/688944-20161020114627857-1608665760.png)
　　　　5、其中最重要的是Access-Control-Allow-Origin这个字段，这个字段规定了可以得到本资源的域，如果头信息有此字段，这个字段与客户端浏览器应该是同域的。

