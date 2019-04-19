# Service Worker初体验(Progressive Web Apps) - game3108的专栏 - CSDN博客
2016年11月02日 15:36:00[game3108](https://me.csdn.net/game3108)阅读数：801
Progressive Web Apps 其实是一系列的 Web 功能和技术的集合，其目的在于增强 Web Apps 的功能和体验，使之更像 Native Apps。这边转载一篇Progressive
 Web Apps的技术核心的文章，关于Progressive Web Apps可以了解2016Qcon的ppt：https://huangxuan.me/pwa-qcon2016/#/
转载自AlloyTeam：[http://www.alloyteam.com/2016/01/9274/](http://www.alloyteam.com/2016/01/9274/)
在2014年，W3C公布了service worker的草案，service worker提供了很多新的能力，使得web app拥有与native app相同的离线体验、消息推送体验。
service worker是一段脚本，与web worker一样，也是在后台运行。作为一个独立的线程，运行环境与普通脚本不同，所以不能直接参与web交互行为。native app可以做到离线使用、消息推送、后台自动更新，service worker的出现是正是为了使得web app也可以具有类似的能力。
service worker可以：
- 后台消息传递
- 
网络代理，转发请求，伪造响应
- 
离线缓存
- 
消息推送
- 
 … …
本文以资源缓存为例，说明一下service worker是如何工作的。
## 生命周期
先来看一下一个service worker的运行周期
![](http://segmentfault.com/img/bVrKGT)
上图是service worker生命周期，出处[http://www.html5rocks.com/en/tutorials/service-worker/introduction/](http://www.html5rocks.com/en/tutorials/service-worker/introduction/)
图中可以看到，一个service worker要经历以下过程：
    1.  安装
    2.  激活，激活成功之后，打开chrome://inspect/#service-workers可以查看到当前运行的service worker
![](http://segmentfault.com/img/bVrKH9)
    3. 监听fetch和message事件，下面两种事件会进行简要描述
    4. 销毁，是否销毁由浏览器决定，如果一个service worker长期不使用或者机器内存有限，则可能会销毁这个worker
## fetch事件
在页面发起http请求时，service worker可以通过fetch事件拦截请求，并且给出自己的响应。
w3c提供了一个新的fetch api，用于取代XMLHttpRequest，与XMLHttpRequest最大不同有两点：
     1. fetch()方法返回的是Promise对象，通过then方法进行连续调用，减少嵌套。ES6的Promise在成为标准之后，会越来越方便开发人员。
      2. 提供了Request、Response对象，如果做过后端开发，对Request、Response应该比较熟悉。前端要发起请求可以通过url发起，也可以使用Request对象发起，而且Request可以复用。但是Response用在哪里呢？在service worker出现之前，前端确实不会自己给自己发消息，但是有了service worker，就可以在拦截请求之后根据需要发回自己的响应，对页面而言，这个普通的请求结果并没有区别，这是Response的一处应用。
下面是在[http://www.sitepoint.com/introduction-to-the-fetch-api/](http://www.sitepoint.com/introduction-to-the-fetch-api/)中，作者利用fetch api通过fliker的公开api获取图片的例子，注释中详细解释了每一步的作用：
```
```
|12345678910111213141516|/* 由于是get请求，直接把参数作为query string传递了 */varURL='https://api.flickr.com/services/rest/?method=flickr.photos.search&api_key=your_api_key&format=json&nojsoncallback=1&tags=penguins';functionfetchDemo(){// fetch(url, option)支持两个参数，option中可以设置header、body、method信息fetch(URL).then(function(response){// 通过promise 对象获得相应内容，并且将响应内容按照json格式转成对象，json()方法调用之后返回的依然是promise对象// 也可以把内容转化成arraybuffer、blob对象returnresponse.json();}).then(function(json){// 渲染页面insertPhotos(json);});}fetchDemo();|
fetch api与XMLHttpRequest相比，更加简洁，并且提供的功能更全面，资源获取方式比ajax更优雅。兼容性方面：chrome 42开始支持，对于旧浏览器，可以通过官方维护的polyfill支持。
## message事件
页面和serviceWorker之间可以通过posetMessage()方法发送消息，发送的消息可以通过message事件接收到。
这是一个双向的过程，页面可以发消息给service worker，service worker也可以发送消息给页面，由于这个特性，可以将service worker作为中间纽带，使得一个域名或者子域名下的多个页面可以自由通信。
这里是一个小的页面之间通信demo[https://nzv3tos3n.qnssl.com/message/msg-demo.html](https://nzv3tos3n.qnssl.com/message/msg-demo.html)
## 利用service workder缓存文件
下面介绍一个利用service worker缓存离线文件的例子
准备index.js，用于注册service-worker
```
```
|1234567|if(navigator.serviceWorker){navigator.serviceWorker.register('service-worker.js').then(function(registration){console.log('service worker 注册成功');}).catch(function(err){console.log('servcie worker 注册失败')});}|
在上述代码中，注册了service-worker.js作为当前路径下的service worker。由于service worker的权限很高，所有的代码都需要是安全可靠的，所以只有https站点才可以使用service worker，当然localhost是一个特例。
注册完毕，现在开始写service-worker.js代码。
根据前面的生命周期图，在一个新的service worker被注册以后，首先会触发install事件，在service-workder.js中，可以通过监听install事件进行一些初始化工作，或者什么也不做。
因为我们是要缓存离线文件，所以可以在install事件中开始缓存，但是只是将文件加到caches缓存中，真正想让浏览器使用缓存文件需要在fetch事件中拦截
```
```
|1234567891011|varcacheFiles=['about.js','blog.js'];self.addEventListener('install',function(evt){evt.waitUntil(caches.open('my-test-cahce-v1').then(function(cache){returncache.addAll(cacheFiles);}));});|
首先定义了需要缓存的文件数组cacheFile，然后在install事件中，缓存这些文件。
evt是一个InstallEvent对象,继承自ExtendableEvent，其中的waitUntil()方法接收一个promise对象，直到这个promise对象成功resolve之后，才会继续运行service-worker.js。
caches是一个CacheStorage对象，使用open()方法打开一个缓存，缓存通过名称进行区分。
获得cache实例之后，调用addAll()方法缓存文件。
这样就将文件添加到caches缓存中了，想让浏览器使用缓存，还需要拦截fetch事件
```
```
|123456789101112131415161718192021|// 缓存图片self.addEventListener('fetch',function(evt){evt.respondWith(caches.match(evt.request).then(function(response){if(response){returnresponse;}varrequest=evt.request.clone();returnfetch(request).then(function(response){if(!response&&response.status!==200&&!response.headers.get('Content-type').match(/image/)){returnresponse;}varresponseClone=response.clone();caches.open('my-test-cache-v1').then(function(cache){cache.put(evt.request,responseClone);});returnresponse;});}))});|
通过监听fetch事件，service worker可以返回自己的响应。
首先检缓存中是否已经缓存了这个请求，如果有，就直接返回响应，就减少了一次网络请求。否则由service workder发起请求，这时的service workder起到了一个中间代理的作用。
service worker请求的过程通过fetch api完成，得到response对象以后进行过滤，查看是否是图片文件，如果不是，就直接返回请求，不会缓存。
如果是图片，要先复制一份response，原因是request或者response对象属于stream，只能使用一次，之后一份存入缓存，另一份发送给页面。
这就是service worker的强大之处：拦截请求，伪造响应。fetch api在这里也起到了很大的作用。
service worker的更新很简单，只要service-worker.js的文件内容有更新，就会使用新的脚本。但是有一点要注意：旧缓存文件的清除、新文件的缓存要在activate事件中进行，因为可能旧的页面还在使用之前的缓存文件，清除之后会失去作用。
在初次使用service worker的过程中，也遇到了一些问题，下面是其中两个
### 问题1. 运行时间
service worker并不是一直在后台运行的。在页面关闭后，浏览器可以继续保持service worker运行，也可以关闭service worker，这取决与浏览器自己的行为。所以不要定义一些全局变量，例如下面的代码(来自[https://jakearchibald.com/2014/service-worker-first-draft/](https://jakearchibald.com/2014/service-worker-first-draft/)):
```
```
|12345678|varhitCounter=0;this.addEventListener('fetch',function(event){hitCounter++;event.respondWith(newResponse('Hit number '+hitCounter));});|
返回的结果可能是没有规律的：1,2,1,2,1,1,2….，原因是hitCounter并没有一直存在，如果浏览器关闭了它，下次启动的时候hitCounter就赋值为0了
这样的事情导致调试代码困难，当你更新一个service worker以后，只有在打开新页面以后才可能使用新的service worker，在调试过程中经常等上一两分钟才会使用新的，比较抓狂。
### 问题2. 权限太大
当service worker监听fetch事件以后，对应的请求都会经过service worker。通过chrome的network工具，可以看到此类请求会标注：from service worker。如果service worker中出现了问题，会导致所有请求失败，包括普通的html文件。所以service worker的代码质量、容错性一定要很好才能保证web app正常运行。
参考文章:
 1. http://www.html5rocks.com/en/tutorials/service-worker/introduction/
 2. http://www.sitepoint.com/introduction-to-the-fetch-api/
 3. https://developer.mozilla.org/en-US/docs/Web/API/InstallEvent
 4. https://developer.mozilla.org/en-US/docs/Web/API/ExtendableEvent
 5. https://developer.mozilla.org/en-US/docs/Web/API/CacheStorage
