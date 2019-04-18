# Performance — 前端性能监控利器 - z69183787的专栏 - CSDN博客
2017年11月12日 19:08:16[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6810
个人分类：[前端-Html5](https://blog.csdn.net/z69183787/article/category/5816199)
[https://www.cnblogs.com/bldxh/p/6857324.html](https://www.cnblogs.com/bldxh/p/6857324.html)
最近在写一个监控脚本，终于有机会接触到了这一块，整理后写下了本文。
Performance是一个做前端性能监控离不开的API，最好在页面完全加载完成之后再使用，因为很多值必须在页面完全加载之后才能得到。最简单的办法是在window.onload事件中读取各种数据。
## 属性
### timing （PerformanceTiming）
从输入url到用户可以使用页面的全过程时间统计，会返回一个PerformanceTiming对象，单位均为毫秒
![](http://images2015.cnblogs.com/blog/1025895/201705/1025895-20170515171943869-544403857.png)
按触发顺序排列所有属性：(更详细标准的解释请参看：[W3C Editor's Draft](https://w3c.github.io/navigation-timing/#introduction))
`navigationStart`:在同一个浏览器上下文中，前一个网页（与当前页面不一定同域）unload
 的时间戳，如果无前一个网页 unload ，则与 fetchStart 值相等
`unloadEventStart`:前一个网页（与当前页面同域）unload
 的时间戳，如果无前一个网页 unload 或者前一个网页与当前页面不同域，则值为 0
`unloadEventEnd`:和
 unloadEventStart 相对应，返回前一个网页 unload 事件绑定的回调函数执行完毕的时间戳
`redirectStart`:第一个
 HTTP 重定向发生时的时间。有跳转且是同域名内的重定向才算，否则值为 0
`redirectEnd`:最后一个
 HTTP 重定向完成时的时间。有跳转且是同域名内的重定向才算，否则值为 0
`fetchStart`:浏览器准备好使用
 HTTP 请求抓取文档的时间，这发生在检查本地缓存之前
`domainLookupStart`:DNS
 域名查询开始的时间，如果使用了本地缓存（即无 DNS 查询）或持久连接，则与 fetchStart 值相等
`domainLookupEnd`:DNS
 域名查询完成的时间，如果使用了本地缓存（即无 DNS 查询）或持久连接，则与 fetchStart 值相等
`connectStart`:HTTP（TCP）
 开始建立连接的时间，如果是持久连接，则与 fetchStart 值相等,如果在传输层发生了错误且重新建立连接，则这里显示的是新建立的连接开始的时间
`connectEnd`:HTTP（TCP）
 完成建立连接的时间（完成握手），如果是持久连接，则与 fetchStart 值相等,如果在传输层发生了错误且重新建立连接，则这里显示的是新建立的连接完成的时间
> 
**注意:**这里握手结束，包括安全连接建立完成、SOCKS 授权通过
`secureConnectionStart`:HTTPS
 连接开始的时间，如果不是安全连接，则值为 0
`requestStart`:HTTP
 请求读取真实文档开始的时间（完成建立连接），包括从本地读取缓存,连接错误重连时，这里显示的也是新建立连接的时间
`responseStart`:HTTP
 开始接收响应的时间（获取到第一个字节），包括从本地读取缓存
`responseEnd`:HTTP
 响应全部接收完成的时间（获取到最后一个字节），包括从本地读取缓存
`domLoading`:开始解析渲染
 DOM 树的时间，此时 Document.readyState 变为 loading，并将抛出 readystatechange 相关事件
`domInteractive`:完成解析
 DOM 树的时间，Document.readyState 变为 interactive，并将抛出 readystatechange 相关事件
> 
**注意:**只是 DOM 树解析完成，这时候并没有开始加载网页内的资源
`domContentLoadedEventStart`:DOM
 解析完成后，网页内资源加载开始的时间,文档发生 DOMContentLoaded事件的时间
`domContentLoadedEventEnd`:DOM
 解析完成后，网页内资源加载完成的时间（如 JS 脚本加载执行完毕），文档的DOMContentLoaded 事件的结束时间
`domComplete`:DOM
 树解析完成，且资源也准备就绪的时间，Document.readyState 变为 complete，并将抛出 readystatechange 相关事件
`loadEventStart`:load
 事件发送给文档，也即 load 回调函数开始执行的时间,如果没有绑定 load 事件，值为 0
`loadEventEnd`:load
 事件的回调函数执行完毕的时间,如果没有绑定 load 事件，值为 0
**常用计算：**
DNS查询耗时 ：domainLookupEnd - domainLookupStart
TCP链接耗时 ：connectEnd - connectStart
request请求耗时 ：responseEnd - responseStart
解析dom树耗时 ： domComplete - domInteractive
白屏时间 ：responseStart - navigationStart
domready时间(用户可操作时间节点) ：domContentLoadedEventEnd - navigationStart
onload时间(总下载时间) ：loadEventEnd - navigationStart
### navigation
旨在告诉开发者当前页面是通过什么方式导航过来的，只有两个属性：`type`，`redirectCount`
**type:**标志页面导航类型,值如下表
|type常数|枚举值|描述|
|----|----|----|
|TYPE_NAVIGATE|0|普通进入，包括：点击链接、在地址栏中输入 URL、表单提交、或者通过除下表中 TYPE_RELOAD 和 TYPE_BACK_FORWARD 的方式初始化脚本。|
|TYPE_RELOAD|1|通过刷新进入，包括：浏览器的刷新按钮、快捷键刷新、location.reload()等方法。|
|TYPE_BACK_FORWARD|2|通过操作历史记录进入，包括：浏览器的前进后退按钮、快捷键操作、history.forward()、history.back()、history.go(num)。|
|TYPE_UNDEFINED|255|其他非以上类型的方式进入。|
> 
**注意：**稍带个小知识，history.go(url)这种非标准写法目前主流浏览器均不支持，问题可参考http://stackoverflow.com/questions/6277283/history-gourl-issue
**redirectCount:**表示到达最终页面前，重定向的次数，但是这个接口有同源策略限制，即仅能检测同源的重定向。
> 
**注意：**所有前端模拟的重定向都无法统计到，因为不属于HTTP重定向
### memory
描述内存多少，是在Chrome中添加的一个非标准属性。
`jsHeapSizeLimit`:
 内存大小限制
`totalJSHeapSize`:
 可使用的内存
`usedJSHeapSize`:
 JS对象(包括V8引擎内部对象)占用的内存，不能大于totalJSHeapSize，如果大于，有可能出现了内存泄漏
## 方法
### getEntries()
获取所有资源请求的时间数据,这个函数返回一个按startTime排序的对象数组，数组成员除了会自动根据所请求资源的变化而改变以外，还可以用mark(),measure()方法自定义添加，该对象的属性中除了包含资源加载时间还有以下五个属性。
`name`：资源名称，是资源的绝对路径或调用mark方法自定义的名称
`startTime`:开始时间
`duration`：加载时间
`entryType`：资源类型，entryType类型不同数组中的对象结构也不同！具体见下
`initiatorType`：谁发起的请求，具体见下
**entryType**的值：
|值|该类型对象|描述|
|----|----|----|
|mark|PerformanceMark|通过mark()方法添加到数组中的对象|
|measure|PerformanceMeasure|通过measure()方法添加到数组中的对象|
|resource|PerformanceResourceTiming|所有资源加载时间，用处最多|
|navigation|PerformanceNavigationTiming|现除chrome和Opera外均不支持，导航相关信息|
|frame|PerformanceFrameTiming|现浏览器均未支持|
|server|PerformanceServerTiming|未查到相关资料|
**initiatorType**的值：
|发起对象|值|描述|
|----|----|----|
|a Element|`link`/`script`/`img`/`iframe`等|通过标签形式加载的资源，值是该节点名的小写形式|
|a CSS resourc|`css`|通过css样式加载的资源，比如background的url方式加载资源|
|a XMLHttpRequest object|`xmlhttprequest`|通过xhr加载的资源|
|a PerformanceNavigationTiming object|`navigation`|当对象是PerformanceNavigationTiming时返回|
```
//根据entryType类型返回的不同对象
PerformanceMark:{  //通过mark()方法添加的对象
    entryType："mark"
    name：调用mark()方法时自定义的名字
    startTime: 做标记的时间
    duration：0
}
PerformanceMeasure:{  //通过measure()方法添加的对象
    entryType："measure"
    name：调用measure()方法时自定义的名字
    startTime: 开始量的时间
    duration：标记的两个量的时间间隔
}
PerformanceResourceTiming:{  //可以用来做一个精准的进度条
    entryType："resource"
    name：资源的绝对路径,即URL
    startTime: 即将抓取资源的时间，
    duration： responseEnd - startTime
    initiatorType：略！/:傲娇脸
    //其他属性请参考performance.timing
}
PerformanceNavigationTiming:{
    entryType："navigation"
    name：本页路由，即地址栏看到的地址
    startTime: 0
    duration： loadEventEnd - startTime 
    initiatorType："navigation"
    //其他属性请参考performance.timing
}
```
**请注意：**
- 目前通过`<audio>`，`<video>`加载资源,initiatorType还无法返回"audio"和"video"，chrome中只能返回空字符串,firfox返回"other"
- 如果一个图片在页面内既用img引入，又作为背景图片引入，那么initiatorType返回的"img"
- performance.getEntries(params)这种形式仍出于草案阶段，目前所有浏览器均为支持。但是非常有用，期待早些实现。
- 使用该方法统计资源信息的时候首先可以合理利用clearResourceTimings清除已统计过的对象避免重复统计，其次要过滤掉因上报统计数据而产生的对象。
### getEntriesByName(name,type[optional])，getEntriesByType(type)
`name`:想要筛选出的资源名
`type`:entryType的值中一个
返回值仍是一个数组，这个数组相当于getEntries()方法经过所填参数筛选后的一个子集
### clearResourceTimings();
该方法无参数无返回值，可以清楚目前所有entryType为"resource"的数据，用于写单页应用的统计脚本非常有用
### mark(name),measure(name, startMark, endMark),clearMarks(),clearMeasures()
用于做标记和清除标记，供用户自定义统计一些数据，比如某函数运行耗时等
`name`:自定义的名称，**不要和getEntries()返回的数组中其他name重复**
`startMark`:作为开始时间的标记名称或PerformanceTiming的一个属性
`endMark`:作为结束时间的标记名称或PerformanceTiming的一个属性
创建标记：mark(name)；
记录两个标记的时间间隔：measure(name, startMark, endMark);
清除指定标记：window.performance.clearMarks(name);
清除所有标记：window.performance.clearMarks();
清除指定记录间隔数据：window.performance.clearMeasures(name);
清除所有记录间隔数据：window.performance.clearMeasures();
### now()
performance.now()是当前时间与performance.timing.navigationStart的时间差，以微秒（百万分之一秒）为单位的时间，与 Date.now()-performance.timing.navigationStart的区别是不受系统程序执行阻塞的影响，因此更加精准。
## 友情提示
目前主流浏览器虽然都已支持Performance对象，但是并不能支持它上面的全部属性和方法，本文主要依据chrome编写，因此提到的chrome浏览器都是兼容的，其他具体使用时兼容性请自行测试，目前已测如下：
1.safari浏览器(包括mac和ios)只支持navigation,timing,now其余均不支持
2.微信浏览器支持timing,navigation属性，不支持performance.getEntries方法
## 推荐及参考文章
[MDN-Performance](https://developer.mozilla.org/en-US/docs/Web/API/Performance)
[W3C Editor's Draft](https://w3c.github.io/navigation-timing/#introduction)
[初探performance-监控网页与程序性能](http://www.alloyteam.com/2015/09/explore-performance/)
[使用性能API快速分析web前端性能](https://segmentfault.com/a/1190000004010453)
