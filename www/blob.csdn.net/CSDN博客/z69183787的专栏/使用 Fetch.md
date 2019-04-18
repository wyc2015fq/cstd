# 使用 Fetch - z69183787的专栏 - CSDN博客
2017年11月30日 12:47:37[OkidoGreen](https://me.csdn.net/z69183787)阅读数：279
[Fetch API](https://developer.mozilla.org/en-US/docs/Web/API/Fetch_API)  提供了一个 JavaScript接口，用于访问和操纵HTTP管道的部分，例如请求和响应。它还提供了一个全局 [`fetch()`](https://developer.mozilla.org/zh-CN/docs/Web/API/GlobalFetch/fetch)方法，该方法提供了一种简单，合乎逻辑的方式来跨网络异步获取资源。
这种功能以前是使用  [`XMLHttpRequest`](https://developer.mozilla.org/zh-CN/docs/Web/API/XMLHttpRequest)实现的。Fetch提供了一个更好的替代方法，可以很容易地被其他技术使用，例如 [```
Service
 Workers
```](https://developer.mozilla.org/zh-CN/docs/Web/API/ServiceWorker_API)。Fetch还提供了单个逻辑位置来定义其他HTTP相关概念，例如 CORS和HTTP的扩展。
请注意，fetch 规范与 jQuery.ajax() 主要有两种方式的不同，牢记：
- 当接收到一个代表错误的 HTTP 状态码时，从 `fetch()`返回的
 Promise 不会被标记为 reject， 即使该 HTTP 响应的状态码是 404 或 500。相反，它会将 Promise 状态标记为 resolve （但是会将 resolve 的返回值的 ok 属性设置为 false ），  仅当网络故障时或请求被阻止时，才会标记为 reject。
- 默认情况下, `fetch`不会从服务端发送或接收任何 cookies,
 如果站点依赖于用户 session，则会导致未经认证的请求（要发送 cookies，必须设置 [credentials](https://developer.mozilla.org/zh-CN/docs/Web/API/GlobalFetch/fetch#参数) 选项）.
## 进行 fetch 请求 
一个基本的 fetch请求设置起来很简单。看看下面的代码：
```
let myImage = document.querySelector('img');
fetch('flowers.jpg')
.then(function(response) {
    return response.blob();
})
.then(function(myBlob) {
    let objectURL = URL.createObjectURL(myBlob);
    myImage.src = objectURL;
});
```
这里我们通过网络获取一个图像并将其插入到一个 [`<img>`](https://developer.mozilla.org/zh-CN/docs/Web/HTML/Element/img) 元素中。最简单的用法是只提供一个参数用来指明想fetch到的资源路径，然后返回一个包含响应结果的promise(一个 [`Response`](https://developer.mozilla.org/zh-CN/docs/Web/API/Response) 对象)。
当然它只是一个 HTTP 响应，而不是真的图片。为了获取图片的内容，我们需要使用 [`blob()`](https://developer.mozilla.org/zh-CN/docs/Web/API/Body/blob) 方法（在[`Body`](https://developer.mozilla.org/zh-CN/docs/Web/API/Body)mixin
 中定义，被 [`Request`](https://developer.mozilla.org/zh-CN/docs/Web/API/Request) 和 [`Response`](https://developer.mozilla.org/zh-CN/docs/Web/API/Response) 对象实现）。
## 浏览器支持情况
Fetch 的支持目前还处于早期的阶段，不过正在取得良好的进展。它目前在 Firefox 39 以上，和 Chrome 42 以上都被支持了。
![](https://mdn.mozillademos.org/files/15083/fetch-api.png)
如果你现在就想使用它，还可以用 [Fetch Polyfill](https://github.com/github/fetch)，用于支持那些还未支持
 Fetch 的浏览器。记住它目前还支持在实验阶段。
注意：目前可能有些担忧，[Fetch
 spec](https://fetch.spec.whatwg.org/) 并不符合 [Streams spec](https://streams.spec.whatwg.org/)。但是，将来的计划有将
 Streams 和 Fetch 整合的趋势：参考 [Fetch
 API integrated with Streams](https://github.com/yutakahirano/fetch-with-streams/)。
## 功能检测
Fetch API 的支持情况，可以通过检测 [`Headers`](https://developer.mozilla.org/zh-CN/docs/Web/API/Headers)、[`Request`](https://developer.mozilla.org/zh-CN/docs/Web/API/Request)、[`Response`](https://developer.mozilla.org/zh-CN/docs/Web/API/Response) 或 [`fetch()`](https://developer.mozilla.org/zh-CN/docs/Web/API/GlobalFetch/fetch) 是否在 [`Window`](https://developer.mozilla.org/zh-CN/docs/Web/API/Window) 或 [`Worker`](https://developer.mozilla.org/zh-CN/docs/Web/API/Worker) 域中。比如你可以这样做：
```
if(self.fetch) {
    // run my fetch request here
} else {
    // do something with XMLHttpRequest?
}
```
## 发起 fetch 请求
发起一个基本的 fetch 请求很简单。看如下代码：
```
var myImage = document.querySelector('img');
fetch('flowers.jpg')
.then(function(response) {
  return response.blob();
})
.then(function(myBlob) {
  var objectURL = URL.createObjectURL(myBlob);
  myImage.src = objectURL;
});
```
以上代码中，我们通过网络获取了一个图片，然后将它插入到一个 [`<img>`](https://developer.mozilla.org/zh-CN/docs/Web/HTML/Element/img) 标签中。这个最简单的用法中，`fetch()` 接受了一个参数——请求的地址——然后返回一个包含
 response（一个 [`Response`](https://developer.mozilla.org/zh-CN/docs/Web/API/Response) 对象）的 promise
 对象。
当然它只是一个 HTTP 响应，而不是真的图片。为了获取图片的内容，我们需要使用 [`blob()`](https://developer.mozilla.org/zh-CN/docs/Web/API/Body/blob)方法（在 [`Body`](https://developer.mozilla.org/zh-CN/docs/Web/API/Body) mixin
 中定义，被 [`Request`](https://developer.mozilla.org/zh-CN/docs/Web/API/Request) 和 [`Response`](https://developer.mozilla.org/zh-CN/docs/Web/API/Response) 对象实现）。
注意：Body mixin 还有其他相似的方法，用于获取其他类型的内容。参考 [Body](https://developer.mozilla.org/zh-CN/docs/Web/API/Fetch_API/Using_Fetch#Body)。
`接着，从 `[`Blob`](https://developer.mozilla.org/zh-CN/docs/Web/API/Blob) 中获取 `objectURL，之后再插入到 `[`img`](https://developer.mozilla.org/zh-CN/docs/Web/API/Img) 中。
最好使用符合[内容安全策略 (CSP)](https://developer.mozilla.org/zh-CN/docs/Web/Security/CSP/CSP_policy_directives)的链接而不是使用直接指向资源地址的方式来进行Fetch的请求。
### 自定义请求的参数
`fetch()` 接受第二个可选参数，一个可以控制不同配置的 `init` 对象：
```
var myHeaders = new Headers();
var myInit = { method: 'GET',
               headers: myHeaders,
               mode: 'cors',
               cache: 'default' };
fetch('flowers.jpg',myInit)
.then(function(response) {
  return response.blob();
})
.then(function(myBlob) {
  var objectURL = URL.createObjectURL(myBlob);
  myImage.src = objectURL;
});
```
参考 [`fetch()`](https://developer.mozilla.org/zh-CN/docs/Web/API/GlobalFetch/fetch)，查看所有可选的配置和更多描述。
### 检测请求是否成功
如果遇到网络故障，[`fetch()`](https://developer.mozilla.org/zh-CN/docs/Web/API/GlobalFetch/fetch) promise
 将会 reject，带上一个 [`TypeError`](https://developer.mozilla.org/zh-CN/docs/Web/JavaScript/Reference/Global_Objects/TypeError) 对象。虽然这个情况经常是遇到了权限问题或类似问题——比如
 404 不是一个网络故障。想要精确的判断 `fetch()` 是否成功，需要包含 promise resolved 的情况，此时再判断 [`Response.ok`](https://developer.mozilla.org/zh-CN/docs/Web/API/Response/ok) 是不是为
 true。类似一下代码：
```
fetch('flowers.jpg').then(function(response) {
  if(response.ok) {
    response.blob().then(function(myBlob) {
      var objectURL = URL.createObjectURL(myBlob);
      myImage.src = objectURL;
    });
  } else {
    console.log('Network response was not ok.');
  }
})
.catch(function(error) {
  console.log('There has been a problem with your fetch operation: ' + error.message);
});
```
### 自定义请求对象
除了传给 `fetch()` 一个资源的地址，你还可以通过使用 [`Request()`](https://developer.mozilla.org/zh-CN/docs/Web/API/Request/Request) 构造函数来创建一个
 request 对象，然后再作为参数传给 `fetch()：`
```
var myHeaders = new Headers();
var myInit = { method: 'GET',
               headers: myHeaders,
               mode: 'cors',
               cache: 'default' };
var myRequest = new Request('flowers.jpg', myInit);
fetch(myRequest).then(function(response) {
  return response.blob();
}).then(function(myBlob) {
  var objectURL = URL.createObjectURL(myBlob);
  myImage.src = objectURL;
});
```
`Request()` 和 `fetch()` 接受同样的参数。你甚至可以传入一个已存在的
 request 对象来创造一个拷贝：
`var anotherRequest = new Request(myRequest,myInit);`
这个很有用，因为 request 和 response bodies 只能被使用一次（译者注：这里的意思是因为设计成了 stream 的方式，所以它们只能被读取一次）。创建一个拷贝就可以再次使用 request/response 了，当然也可以使用不同的 `init` 参数。
注意：[`clone()`](https://developer.mozilla.org/zh-CN/docs/Web/API/Request/clone) 方法也可以用于创建一个拷贝。它在语义上有一点不同于其他拷贝的方法。其他方法（比如拷贝一个
 response）中，如果 request 的 body 已经被读取过，那么将执行失败，然而 `clone()` 则不会失败。
## Headers
使用 [`Headers`](https://developer.mozilla.org/zh-CN/docs/Web/API/Headers) 的接口，你可以通过 [`Headers()`](https://developer.mozilla.org/zh-CN/docs/Web/API/Headers/Headers) 构造函数来创建一个你自己的
 headers 对象。一个 headers 对象是一个简单的多名值对：
```
var content = "Hello World";
var myHeaders = new Headers();
myHeaders.append("Content-Type", "text/plain");
myHeaders.append("Content-Length", content.length.toString());
myHeaders.append("X-Custom-Header", "ProcessThisImmediately");
```
也可以传一个多维数组或者对象字面量：
```
myHeaders = new Headers({
  "Content-Type": "text/plain",
  "Content-Length": content.length.toString(),
  "X-Custom-Header": "ProcessThisImmediately",
});
```
它的内容可以被获取：
```
console.log(myHeaders.has("Content-Type")); // true
console.log(myHeaders.has("Set-Cookie")); // false
myHeaders.set("Content-Type", "text/html");
myHeaders.append("X-Custom-Header", "AnotherValue");
 
console.log(myHeaders.get("Content-Length")); // 11
console.log(myHeaders.getAll("X-Custom-Header")); // ["ProcessThisImmediately", "AnotherValue"]
 
myHeaders.delete("X-Custom-Header");
console.log(myHeaders.getAll("X-Custom-Header")); // [ ]
```
虽然一些操作只能在 [`ServiceWorkers`](https://developer.mozilla.org/zh-CN/docs/Web/API/ServiceWorker_API) 中使用，但是它提供了更方便的操作
 Headers 的 API。
如果使用了一个不合法的HTTP Header属性名，那么Headers的方法通常都抛出 TypeError 异常。如果不小心写入了一个不可写的属性，也会抛出一个 TypeError 异常。除此以外的情况，失败了并不抛出异常。例如：
```
var myResponse = Response.error();
try {
  myResponse.headers.set("Origin", "http://mybank.com");
} catch(e) {
  console.log("Cannot pretend to be a bank!");
}
```
最佳实践是在使用之前检查 content type 是否正确，比如：
```
fetch(myRequest).then(function(response) {
  if(response.headers.get("content-type") === "application/json") {
    return response.json().then(function(json) {
      // process your JSON further
    });
  } else {
    console.log("Oops, we haven't got JSON!");
  }
});
```
### Guard
由于 Headers 可以在 request 请求中被发送或者在 response 请求中被接收，并且规定了哪些参数是可写的，Headers 对象有一个特殊的 guard 属性。这个属性没有暴露给 Web，但是它影响到哪些内容可以在 Headers 对象中被操作。
可能的值如下：
- `none`：默认的
- `request：`从 request 中获得的
 headers（[`Request.headers`](https://developer.mozilla.org/zh-CN/docs/Web/API/Request/headers)）只读
- `request-no-cors`：从不同域（[`Request.mode`](https://developer.mozilla.org/zh-CN/docs/Web/API/Request/mode)`no-cors`）的
 request 中获得的 headers 只读
- `response`：从 response
 中获得的 headers（[`Response.headers`](https://developer.mozilla.org/zh-CN/docs/Web/API/Response/headers)）只读
- `immutable：`在 ServiceWorkers
 中最常用的，所有的 headers 都只读
注意：你不可以添加或者修改一个 guard 属性是 `request` 的
 Request Headers 的 `Content-Length` 属性。同样地，插入 `Set-Cookie` 属性到一个
 response headers 是不允许的，因此 ServiceWorkers 是不能给合成的 Response 的 headers 添加一些 cookies。
## Response 对象
如上述, [`Response`](https://developer.mozilla.org/zh-CN/docs/Web/API/Response) 实例是在fetch()处理完promises之后返回的.
它的实例也可用通过JavaScript来创建, 但只有在[`ServiceWorkers`](https://developer.mozilla.org/zh-CN/docs/Web/API/ServiceWorker_API)中才真正有用,当使用[`respondWith()`](https://developer.mozilla.org/zh-CN/docs/Web/API/FetchEvent/respondWith)方法并提供了一个自定义的response来接受request时:
```
var myBody = new Blob();
addEventListener('fetch', function(event) {
  event.respondWith(new Response(myBody, {
    headers: { "Content-Type" : "text/plain" }
  });
});
```
[`Response()`](https://developer.mozilla.org/zh-CN/docs/Web/API/Response/Response) 构造方法接受两个可选参数—response的数据体和一个初始化对象(与[`Request()`](https://developer.mozilla.org/zh-CN/docs/Web/API/Request/Request)所接受的init参数类似.)
你会用到的最常见的response属性有:
- [`Response.status`](https://developer.mozilla.org/zh-CN/docs/Web/API/Response/status) —
 整数(默认值为200) 为response的状态码.
- [`Response.statusText`](https://developer.mozilla.org/zh-CN/docs/Web/API/Response/statusText) —
 字符串(默认值为"OK"),该值与HTTP状态码消息对应.
- [`Response.ok`](https://developer.mozilla.org/zh-CN/docs/Web/API/Response/ok) —
 如上所示, 该属性是来检查response的状态是否在200-299(包括200,299)这个范围内.该属性返回一个[`Boolean`](https://developer.mozilla.org/zh-CN/docs/Web/API/Boolean)值.
注意: 静态方法[`error()`](https://developer.mozilla.org/zh-CN/docs/Web/API/Response/error)只是返回了一个错误的response.
 与此类似地, [`redirect()`](https://developer.mozilla.org/zh-CN/docs/Web/API/Response/redirect) 只是返回了一个可以重定向至某URL的response.
 这些也只与Service Workers才有关.
## Body
不管是请求还是响应都能够包含body对象. body也可以是以下任意类型的实例.
- [`ArrayBuffer`](https://developer.mozilla.org/zh-CN/docs/Web/API/ArrayBuffer)
- [`ArrayBufferView`](https://developer.mozilla.org/zh-CN/docs/Web/API/ArrayBufferView) (Uint8Array
 and friends)
- [`Blob`](https://developer.mozilla.org/zh-CN/docs/Web/API/Blob)/File
- string
- [`URLSearchParams`](https://developer.mozilla.org/zh-CN/docs/Web/API/URLSearchParams)
- [`FormData`](https://developer.mozilla.org/zh-CN/docs/Web/API/FormData)
[`Body`](https://developer.mozilla.org/zh-CN/docs/Web/API/Body) 类定义了以下方法 (这些方法都被 [`Request`](https://developer.mozilla.org/zh-CN/docs/Web/API/Request) 和[`Response`](https://developer.mozilla.org/zh-CN/docs/Web/API/Response)所实现)以获取body内容.
 这些方法都会返回一个被解析后的promise对象和数据.
- [`arrayBuffer()`](https://developer.mozilla.org/zh-CN/docs/Web/API/Body/arrayBuffer)
- [`blob()`](https://developer.mozilla.org/zh-CN/docs/Web/API/Body/blob)
- [`json()`](https://developer.mozilla.org/zh-CN/docs/Web/API/Body/json)
- [`text()`](https://developer.mozilla.org/zh-CN/docs/Web/API/Body/text)
- [`formData()`](https://developer.mozilla.org/zh-CN/docs/Web/API/Body/formData)
比起XHR来，这些方法让非文本化的数据使用起来更加简单。
请求体可以由传入body参数来进行设置:
```
```
var form = new FormData(document.getElementById('login-form'));
fetch("/login", {
  method: "POST",
  body: form
})
```
```
request 和response (也包括`fetch()` 方法)都会试着自动设置content type.如果没有设置`Content-Type值，发送的请求也会自动设值`.
