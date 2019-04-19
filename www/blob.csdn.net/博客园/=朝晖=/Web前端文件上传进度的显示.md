# Web前端文件上传进度的显示 - =朝晖= - 博客园
# [Web前端文件上传进度的显示](https://www.cnblogs.com/dhcn/p/7417832.html)
      跟后台关系不大，主要是前端js实现，具体使用了XMLHttpRequest的ProgressEvent事件，可以参考MDN中的[Using XMLHttpRequest](https://developer.mozilla.org/cn/docs/Web/API/XMLHttpRequest/Using_XMLHttpRequest#Submitting_forms_and_uploading_files)
　　https://developer.mozilla.org/cn/docs/Web/API/XMLHttpRequest/Using_XMLHttpRequest#Submitting_forms_and_uploading_files
[`XMLHttpRequest`](https://developer.mozilla.org/zh-cn/DOM/XMLHttpRequest) 让发送一个HTTP请求变得非常容易。你只需要简单的创建一个请求对象实例，打开一个URL，然后发送这个请求。当传输完毕后，结果的[HTTP状态](https://developer.mozilla.org/zh-CN/docs/HTTP/HTTP_response_codes)以及返回的响应内容也可以从请求对象中获取。本页把这个强大的JavaScript对象的一些常用的甚至略有晦涩的使用案例进行了一下概述。
``` js  language-js
```
function reqListener () {
  console.log(this.responseText);
}
var oReq = new XMLHttpRequest();
oReq.onload = reqListener;
oReq.open("get", "yourFile.txt", true);
oReq.send();
```
```
## 请求类型[Edit](https://developer.mozilla.org/zh-CN/docs/Web/API/XMLHttpRequest/Using_XMLHttpRequest$edit#请求类型)
通过XMLHttpRequest生成的请求可以有两种方式来获取数据，异步模式或同步模式。请求的类型是由这个XMLHttpRequest对象的[open()](https://developer.mozilla.org/zh-cn/DOM/XMLHttpRequest#open())方法的第三个参数`async`的值决定的。如果该参数的值为false，则该XMLHttpRequest请求以同步模式进行，否则该过程将以异步模式完成。这两种类型请求的详细讨论和指南可以在[同步和异步请求](https://developer.mozilla.org/zh-cn/DOM/XMLHttpRequest/Synchronous_and_Asynchronous_Requests)页找到。
注意：由于对用户体验的糟糕效果，从Gecko 30.0(Firefox 30.0 / Thunderbird 30.0 / SeaMonkey 2.27)版本开始，在主线程上的同步请求已经被弃用。
## 处理响应[Edit](https://developer.mozilla.org/zh-CN/docs/Web/API/XMLHttpRequest/Using_XMLHttpRequest$edit#处理响应)
W3C规范定义了XMLHttpRequest对象的几种类型的[响应属性](http://www.w3.org/TR/XMLHttpRequest2/#response)。这些属性告诉客户端关于XMLHttpRequest返回状态的重要信息。一些处理非文本返回类型的用例可能包含一些下面章节描述的操作和分析。
### 分析并操作 responseXML属性
如果你使用 `XMLHttpRequest 来获得一个远程的 XML 文档的内容，responseXML 属性将会是一个由 XML 文档解析而来的 DOM 对象，这很难被操作和分析。这里有五种主要的分析 XML 文档的方式：`
- 使用 [XPath](https://developer.mozilla.org/zh-cn/XPath) 定位到文档的制定部分。
- 使用 [JXON](https://developer.mozilla.org/zh-cn/JXON) 将其转换成 JavaScript 对象树。
- 手工的 [解析和序列化 XML](https://developer.mozilla.org/zh-cn/Parsing_and_serializing_XML) 为字符串或对象。
- 使用 [XMLSerializer](https://developer.mozilla.org/zh-cn/XMLSerializer) 把 DOM 树序列化成字符串或文件。
- 如果你预先知道 XML 文档的内容，你可以使用 [RegExp](https://developer.mozilla.org/zh-cn/JavaScript/Reference/Global_Objects/RegExp)。如果你用 RegExp 扫描时受到换行符的影响，你也许想要删除所有的换行符。然而，这种方法是"最后手段"，因为如果XML代码发生轻微变化，该方法将可能失败。
### 解析和操作包含 HTML 文档的 `responseText` 属性
**注意:** 在 W3C [XMLHttpRequest](http://dvcs.w3.org/hg/xhr/raw-file/tip/Overview.html) 规范中允许 HTML 通过 XMLHttpRequest.responseXML 属性进行解析。更多详细内容请阅读 [HTML in XMLHttpRequest](https://developer.mozilla.org/zh-cn/HTML_in_XMLHttpRequest) 。
如果使用 `XMLHttpRequest` 从远端获取一个 HTML 页面，则所有 HTML 标记会以字符串的形式存放在responseText 属性里，这样就使得操作和解析这些标记变得困难。解析这些HTML标记主要有三种方式：
- 使用 XMLHttpRequest.responseXML 属性。
- 将内容通过 fragment.body.innerHTML 注入到一个 [文档片段](https://developer.mozilla.org/zh-CN/docs/Web/API/DocumentFragment) 中，并遍历 DOM 中的片段。
- 如果你预先知道 HTML 文档的内容，你可以使用 [RegExp](https://developer.mozilla.org/zh-cn/JavaScript/Reference/Global_Objects/RegExp)。如果你用 RegExp 扫描时受到换行符的影响，你也许想要删除所有的换行符。 然而，这种方法是"最后手段"，因为如果HTML代码发生轻微变化，该方法将可能失败。
## 处理二进制数据[Edit](https://developer.mozilla.org/zh-CN/docs/Web/API/XMLHttpRequest/Using_XMLHttpRequest$edit#处理二进制数据)
尽管 `XMLHttpRequest` 一般用来发送和接收文本数据，但其实也可以发送和接受二进制内容。有许多经过良好测试的方法来强制使用 XMLHttpRequest 发送二进制数据。利用 XMLHttpRequest 的  .overrideMimeType() 方法是一个解决方案，虽然它并不是一个标准方法。
``` js line-numbers  language-js
```
var oReq = new XMLHttpRequest();
oReq.open("GET", url, true);
// retrieve data unprocessed as a binary string
oReq.overrideMimeType("text/plain; charset=x-user-defined");
/* ... */
```
```
在 XMLHttpRequest Level 2 规范中新加入了 [responseType 属性](http://www.w3.org/TR/XMLHttpRequest2/#the-responsetype-attribute) ，使得发送和接收二进制数据变得更加容易。
``` js line-numbers  language-js
```
var oReq = new XMLHttpRequest();
oReq.onload = function(e) {
  var arraybuffer = xhr.response; // not responseText
  /* ... */
}
oReq.open("GET", url, true);
oReq.responseType = "arraybuffer";
oReq.send();
```
```
更多示例请参考 [发送和接收二进制数据](https://developer.mozilla.org/zh-cn/DOM/XMLHttpRequest/Sending_and_Receiving_Binary_Data) 。
## 监测进度[Edit](https://developer.mozilla.org/zh-CN/docs/Web/API/XMLHttpRequest/Using_XMLHttpRequest$edit#监测进度)
`XMLHttpRequest` 提供了各种在请求被处理期间发生的事件以供监听。这包括定期进度通知、错误通知，等等。
支持 DOM 的 progress 事件监测之于 `XMLHttpRequest` 传输，遵循 Web API [进度事件规范](http://dev.w3.org/2006/webapi/progress/Progress.html) : 这些事件实现了 [`ProgressEvent`](https://developer.mozilla.org/zh-CN/docs/Web/API/ProgressEvent) 接口。
``` js line-numbers  language-js
```
var req = new XMLHttpRequest();
req.addEventListener("progress", updateProgress, false);
req.addEventListener("load", transferComplete, false);
req.addEventListener("error", transferFailed, false);
req.addEventListener("abort", transferCanceled, false);
req.open();
...
// progress on transfers from the server to the client (downloads)
function updateProgress(evt) {
  if (evt.lengthComputable) {
    var percentComplete = evt.loaded / evt.total;
    ...
  } else {
    // Unable to compute progress information since the total size is unknown
  }
}
function transferComplete(evt) {
  alert("The transfer is complete.");
}
function transferFailed(evt) {
  alert("An error occurred while transferring the file.");
}
function transferCanceled(evt) {
  alert("The transfer has been canceled by the user.");
}
```
```
第 3-6 行为多种事件添加了事件监听，这些事件在使用 `XMLHttpRequest` 执行数据传输时被发出。
**注意：** 你需要在请求调用 `open()` 之前添加事件监听。否则 progress 事件将不会被触发。
在上个例子中，progress 事件被指定由 `updateProgress()` 函数处理，并接收到传输的总字节数和已经传输的字节数，它们分别在事件对象的 `total` 和 `loaded` 属性里。但是如果 `lengthComputable` 属性的值是 false，那么意味着总字节数是未知并且 total 的值为零。
progress 事件同时存在于下载和上传的传输。下载相关事件在 `XMLHttpRequest` 对象上被触发，就像上面的例子一样。上传相关事件在 `XMLHttpRequest.upload` 对象上被触发，像下面这样：
``` js line-numbers  language-js
```
var req = new XMLHttpRequest();
req.upload.addEventListener("progress", updateProgress);
req.upload.addEventListener("load", transferComplete);
req.upload.addEventListener("error", transferFailed);
req.upload.addEventListener("abort", transferCanceled);
req.open();
```
```
**注意：**progress 事件在使用 `file:` 协议的情况下是无效的。
**注意：**从 Gecko 9.0 开始，进度事件现在可以依托于每一个传入的数据块，包括进度事件被触发前在已经接受了最后一个数据包且连接已经被关闭的情况下接收到的最后一个块。这种情况下，当该数据包的 load 事件发生时 progress 事件会被自动触发。这使你可以只关注 progress 事件就可以可靠的监测进度。
**注意：**在 Gecko 12.0 中，当 `responseType` 为 "moz-blob" 时，如果你的 progress 事件被触发，则响应的值是一个包含了接收到的数据的 [`Blob`](https://developer.mozilla.org/zh-CN/docs/Web/API/Blob) 。
使用 `loadend` 事件可以侦测到所有的三种加载结束条件（abort、load、error）：
``` js line-numbers  language-js
```
req.addEventListener("loadend", loadEnd, false);
function loadEnd(evt) {
  alert("The transfer finished (although we don't know if it succeeded or not).");
}
```
```
需要注意的是，没有方法可以确切的知道 `loadend` 事件接收到的信息是来自何种条件引起的操作终止；但是你可以在所有传输结束的时候使用这个事件处理。
## 提交表单和上传文件[Edit](https://developer.mozilla.org/zh-CN/docs/Web/API/XMLHttpRequest/Using_XMLHttpRequest$edit#提交表单和上传文件)
`XMLHttpRequest` 的实例有两种方式提交表单：
- 使用 AJAX
- 使用 `[FormData](https://developer.mozilla.org/zh-CN/docs/DOM/XMLHttpRequest/FormData)` API
第二种方式（ 使用 `[FormData](https://developer.mozilla.org/zh-CN/docs/DOM/XMLHttpRequest/FormData)` API ）是最简单最快捷的，但是缺点是被收集的数据不是字符串形式的。
第一种方式反而是最复杂的但也是最灵活和最强大。
### 只使用 XMLHttpRequest
在大多数用例中，提交表单时即便不使用 `[FormData](https://developer.mozilla.org/zh-CN/docs/DOM/XMLHttpRequest/FormData)` API 也不会要求其他的 API。唯一的例外情况是，如果你要上传一个或多个文件，你需要额外的 `[FileReader](https://developer.mozilla.org/zh-CN/docs/Web/API/FileReader)` API。
#### 提交方法简介
一个 html [`<form>`](https://developer.mozilla.org/zh-CN/docs/Web/HTML/Element/form) 可以用以下四种方式发送：
- 使用 `POST` 方法，并设置 `enctype` 属性为 `application/x-www-form-urlencoded` (默认)
- 使用 `POST` 方法，并设置 `enctype` 属性为 `text/plain`
- 使用 `POST` 方法，并设置 `enctype` 属性为 `multipart/form-data`
- 使用 `GET` 方法（这种情况下 `enctype` 属性会被忽略）
现在，我们提交一个表单，它里面有两个字段，分别被命名为 `foo` 和 `baz`。如果你用 `POST` 方法，那么服务器将会接收到一个字符串类似于下面三种情况之一，其中的区别依赖于你采用何种编码类型：
- 方法: `POST`;
- 编码类型: `application/x-www-form-urlencoded` (default):
```
Content-Type: application/x-www-form-urlencoded
foo=bar&baz=The+first+line.%0D%0AThe+second+line.%0D%0A
```
- 方法: `POST`;
- 编码类型: `text/plain`:
```
Content-Type: text/plain
foo=bar
baz=The first line.
The second line.
```
- 方法: `POST`;
- 编码类型: `multipart/form-data`:
```
Content-Type: multipart/form-data; boundary=---------------------------314911788813839
-----------------------------314911788813839
Content-Disposition: form-data; name="foo"
bar
-----------------------------314911788813839
Content-Disposition: form-data; name="baz"
The first line.
The second line.
-----------------------------314911788813839--
```
相反的，如果你用 `GET` 方法，一个像下面这样的字符串将被简单的附加到 URL：
`?foo=bar&baz=The%20first%20line.%0AThe%20second%20line.`
#### 一个小的普通的框架
所有这些事情都是由浏览器在你提交一个 [`<form>`](https://developer.mozilla.org/zh-CN/docs/Web/HTML/Element/form) 的时候自动完成的。但是如果你想要用 JavaScript 做同样的事情，你不得不告诉解释器所有的事。那么，如何发送表单这件事在使用纯粹的 AJAX 时会复杂到无法在这里解释清楚。基于这个原因，我们提供一个完整的（但仍然教条的）框架，它可以使用所有的四种提交方式，甚至上传文件：
``` html line-numbers  language-html
```
<!doctype html>
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<title>Sending forms with pure AJAX – MDN</title>
<script type="text/javascript">
"use strict";
/*\
|*|
|*|  :: XMLHttpRequest.prototype.sendAsBinary() Polyfill ::
|*|
|*|  https://developer.mozilla.org/en-US/docs/DOM/XMLHttpRequest#sendAsBinary()
\*/
if (!XMLHttpRequest.prototype.sendAsBinary) {
  XMLHttpRequest.prototype.sendAsBinary = function(sData) {
    var nBytes = sData.length, ui8Data = new Uint8Array(nBytes);
    for (var nIdx = 0; nIdx < nBytes; nIdx++) {
      ui8Data[nIdx] = sData.charCodeAt(nIdx) & 0xff;
    }
    /* send as ArrayBufferView...: */
    this.send(ui8Data);
    /* ...or as ArrayBuffer (legacy)...: this.send(ui8Data.buffer); */
  };
}
/*\
|*|
|*|  :: AJAX Form Submit Framework ::
|*|
|*|  https://developer.mozilla.org/en-US/docs/DOM/XMLHttpRequest/Using_XMLHttpRequest
|*|
|*|  This framework is released under the GNU Public License, version 3 or later.
|*|  http://www.gnu.org/licenses/gpl-3.0-standalone.html
|*|
|*|  Syntax:
|*|
|*|   AJAXSubmit(HTMLFormElement);
\*/
var AJAXSubmit = (function () {
  function ajaxSuccess () {
    /* console.log("AJAXSubmit - Success!"); */
    alert(this.responseText);
    /* you can get the serialized data through the "submittedData" custom property: */
    /* alert(JSON.stringify(this.submittedData)); */
  }
  function submitData (oData) {
    /* the AJAX request... */
    var oAjaxReq = new XMLHttpRequest();
    oAjaxReq.submittedData = oData;
    oAjaxReq.onload = ajaxSuccess;
    if (oData.technique === 0) {
      /* method is GET */
      oAjaxReq.open("get", oData.receiver.replace(/(?:\?.*)?$/, oData.segments.length > 0 ? "?" + oData.segments.join("&") : ""), true);
      oAjaxReq.send(null);
    } else {
      /* method is POST */
      oAjaxReq.open("post", oData.receiver, true);
      if (oData.technique === 3) {
        /* enctype is multipart/form-data */
        var sBoundary = "---------------------------" + Date.now().toString(16);
        oAjaxReq.setRequestHeader("Content-Type", "multipart\/form-data; boundary=" + sBoundary);
        oAjaxReq.sendAsBinary("--" + sBoundary + "\r\n" + oData.segments.join("--" + sBoundary + "\r\n") + "--" + sBoundary + "--\r\n");
      } else {
        /* enctype is application/x-www-form-urlencoded or text/plain */
        oAjaxReq.setRequestHeader("Content-Type", oData.contentType);
        oAjaxReq.send(oData.segments.join(oData.technique === 2 ? "\r\n" : "&"));
      }
    }
  }
  function processStatus (oData) {
    if (oData.status > 0) { return; }
    /* the form is now totally serialized! do something before sending it to the server... */
    /* doSomething(oData); */
    /* console.log("AJAXSubmit - The form is now serialized. Submitting..."); */
    submitData (oData);
  }
  function pushSegment (oFREvt) {
    this.owner.segments[this.segmentIdx] += oFREvt.target.result + "\r\n";
    this.owner.status--;
    processStatus(this.owner);
  }
  function plainEscape (sText) {
    /* how should I treat a text/plain form encoding? what characters are not allowed? this is what I suppose...: */
    /* "4\3\7 - Einstein said E=mc2" ----> "4\\3\\7\ -\ Einstein\ said\ E\=mc2" */
    return sText.replace(/[\s\=\\]/g, "\\$&");
  }
  function SubmitRequest (oTarget) {
    var nFile, sFieldType, oField, oSegmReq, oFile, bIsPost = oTarget.method.toLowerCase() === "post";
    /* console.log("AJAXSubmit - Serializing form..."); */
    this.contentType = bIsPost && oTarget.enctype ? oTarget.enctype : "application\/x-www-form-urlencoded";
    this.technique = bIsPost ? this.contentType === "multipart\/form-data" ? 3 : this.contentType === "text\/plain" ? 2 : 1 : 0;
    this.receiver = oTarget.action;
    this.status = 0;
    this.segments = [];
    var fFilter = this.technique === 2 ? plainEscape : escape;
    for (var nItem = 0; nItem < oTarget.elements.length; nItem++) {
      oField = oTarget.elements[nItem];
      if (!oField.hasAttribute("name")) { continue; }
      sFieldType = oField.nodeName.toUpperCase() === "INPUT" ? oField.getAttribute("type").toUpperCase() : "TEXT";
      if (sFieldType === "FILE" && oField.files.length > 0) {
        if (this.technique === 3) {
          /* enctype is multipart/form-data */
          for (nFile = 0; nFile < oField.files.length; nFile++) {
            oFile = oField.files[nFile];
            oSegmReq = new FileReader();
            /* (custom properties:) */
            oSegmReq.segmentIdx = this.segments.length;
            oSegmReq.owner = this;
            /* (end of custom properties) */
            oSegmReq.onload = pushSegment;
            this.segments.push("Content-Disposition: form-data; name=\"" + oField.name + "\"; filename=\""+ oFile.name + "\"\r\nContent-Type: " + oFile.type + "\r\n\r\n");
            this.status++;
            oSegmReq.readAsBinaryString(oFile);
          }
        } else {
          /* enctype is application/x-www-form-urlencoded or text/plain or method is GET: files will not be sent! */
          for (nFile = 0; nFile < oField.files.length; this.segments.push(fFilter(oField.name) + "=" + fFilter(oField.files[nFile++].name)));
        }
      } else if ((sFieldType !== "RADIO" && sFieldType !== "CHECKBOX") || oField.checked) {
        /* field type is not FILE or is FILE but is empty */
        this.segments.push(
          this.technique === 3 ? /* enctype is multipart/form-data */
            "Content-Disposition: form-data; name=\"" + oField.name + "\"\r\n\r\n" + oField.value + "\r\n"
          : /* enctype is application/x-www-form-urlencoded or text/plain or method is GET */
            fFilter(oField.name) + "=" + fFilter(oField.value)
        );
      }
    }
    processStatus(this);
  }
  return function (oFormElement) {
    if (!oFormElement.action) { return; }
    new SubmitRequest(oFormElement);
  };
})();
</script>
</head>
<body>
<h1>Sending forms with pure AJAX</h1>
<h2>Using the GET method</h2>
<form action="register.php" method="get" onsubmit="AJAXSubmit(this); return false;">
  <fieldset>
    <legend>Registration example</legend>
    <p>
      First name: <input type="text" name="firstname" /><br />
      Last name: <input type="text" name="lastname" />
    </p>
    <p>
      <input type="submit" value="Submit" />
    </p>
  </fieldset>
</form>
<h2>Using the POST method</h2>
<h3>Enctype: application/x-www-form-urlencoded (default)</h3>
<form action="register.php" method="post" onsubmit="AJAXSubmit(this); return false;">
  <fieldset>
    <legend>Registration example</legend>
    <p>
      First name: <input type="text" name="firstname" /><br />
      Last name: <input type="text" name="lastname" />
    </p>
    <p>
      <input type="submit" value="Submit" />
    </p>
  </fieldset>
</form>
<h3>Enctype: text/plain</h3>
<form action="register.php" method="post" enctype="text/plain" onsubmit="AJAXSubmit(this); return false;">
  <fieldset>
    <legend>Registration example</legend>
    <p>
      Your name: <input type="text" name="user" />
    </p>
    <p>
      Your message:<br />
      <textarea name="message" cols="40" rows="8"></textarea>
    </p>
    <p>
      <input type="submit" value="Submit" />
    </p>
  </fieldset>
</form>
<h3>Enctype: multipart/form-data</h3>
<form action="register.php" method="post" enctype="multipart/form-data" onsubmit="AJAXSubmit(this); return false;">
  <fieldset>
    <legend>Upload example</legend>
    <p>
      First name: <input type="text" name="firstname" /><br />
      Last name: <input type="text" name="lastname" /><br />
      Sex:
      <input id="sex_male" type="radio" name="sex" value="male" /> <label for="sex_male">Male</label>
      <input id="sex_female" type="radio" name="sex" value="female" /> <label for="sex_female">Female</label><br />
      Password: <input type="password" name="secret" /><br />
      What do you prefer:
      <select name="image_type">
        <option>Books</option>
        <option>Cinema</option>
        <option>TV</option>
      </select>
    </p>
    <p>
      Post your photos:
      <input type="file" multiple name="photos[]">
    </p>
    <p>
      <input id="vehicle_bike" type="checkbox" name="vehicle[]" value="Bike" /> <label for="vehicle_bike">I have a bike</label><br />
      <input id="vehicle_car" type="checkbox" name="vehicle[]" value="Car" /> <label for="vehicle_car">I have a car</label>
    </p>
    <p>
      Describe yourself:<br />
      <textarea name="description" cols="50" rows="8"></textarea>
    </p>
    <p>
      <input type="submit" value="Submit" />
    </p>
  </fieldset>
</form>
</body>
</html>
```
```
要测试它的话，创建一个页面名为 `register.php` （作为示例表单的 `action` 属性）并且只输入以下内容：
``` php line-numbers  language-php
```php
<?php
/* register.php */
header("Content-type: text/plain");
/*
NOTE: You should never use `print_r()` in production scripts, or
otherwise output client-submitted data without sanitizing it first.
Failing to sanitize can lead to cross-site scripting vulnerabilities.
*/
echo ":: data received via GET ::\n\n";
print_r($_GET);
echo "\n\n:: Data received via POST ::\n\n";
print_r($_POST);
echo "\n\n:: Data received as \"raw\" (text/plain encoding) ::\n\n";
if (isset($HTTP_RAW_POST_DATA)) { echo $HTTP_RAW_POST_DATA; }
echo "\n\n:: Files received ::\n\n";
print_r($_FILES);
```
```
## 跨站的 XMLHttpRequest[Edit](https://developer.mozilla.org/zh-CN/docs/Web/API/XMLHttpRequest/Using_XMLHttpRequest$edit#跨站的_XMLHttpRequest)
现代浏览器可以通过执行 WebApps 工作小组通过的 [Access Control for Cross-Site Requests](https://developer.mozilla.org/zh-CN/docs/Web/HTTP/Access_control_CORS) 标注来支持跨站请求。只要服务器端的配置允许您从您的 Web 应用发送请求，就可以使用 `XMLHttpRequest` 。  否则，会抛出一个 `INVALID_ACCESS_ERR` 异常
## 绕过缓存[Edit](https://developer.mozilla.org/zh-CN/docs/Web/API/XMLHttpRequest/Using_XMLHttpRequest$edit#绕过缓存)
一般地，如果缓存中有相应内容， `XMLHttpRequest` 会试图从缓存中读取内容。绕过缓存的方法见下述代码：
``` js line-numbers  language-js
```
var req = new XMLHttpRequest();
req.open('GET', url, false);
req.channel.loadFlags |= Components.interfaces.nsIRequest.LOAD_BYPASS_CACHE;
req.send(null);
```
```
**Note:** This approach will only work in Gecko-based software, as the `channel` attribute is Gecko-specific.
或者还有一个跨浏览器兼容的方法，就是给 URL 添加时间戳。请确保你酌情地添加了 "?" or "&" 。例如，将：
`http://foo.com/bar.html`
改为
`http://foo.com/bar.html?12345`
以及将
`http://foo.com/bar.html?foobar=baz`
改为
`http://foo.com/bar.html?foobar=baz&12345`
因为本地缓存都是以 URL 作为索引的，这样就可以使每个请求都是唯一的，也就可以这样来绕开缓存。
你也可以用下面的方法自动更改缓存：
``` js line-numbers  language-js
```
var req = new XMLHttpRequest();
req.open("GET", url += ((/\?/).test(url) ? "&" : "?") + (new Date()).getTime(), false);
req.send(null);
```
```
## 安全性[Edit](https://developer.mozilla.org/zh-CN/docs/Web/API/XMLHttpRequest/Using_XMLHttpRequest$edit#安全性)
[Firefox 3 note](https://developer.mozilla.org/zh-CN/docs/Mozilla/Firefox/Releases/3)
[Firefox 5 note](https://developer.mozilla.org/zh-CN/docs/Mozilla/Firefox/Releases/5)
要启用跨站脚本，推荐的做法是对 XMLHttpRequest 的响应使用 the `Access-Control-Allow-Origin `的 HTTP 头。
### XMLHttpRequests being stopped
If you end up with an XMLHttpRequest having `status=0` and `statusText=null`, it means that the request was not allowed to be performed. It was `[UNSENT](http://www.w3.org/TR/XMLHttpRequest/#dom-xmlhttprequest-unsent)`. A likely cause for this is when the [`XMLHttpRequest` origin](http://www.w3.org/TR/XMLHttpRequest/#xmlhttprequest-origin) (at the creation of the XMLHttpRequest) has changed when the XMLHttpRequest is then `open()`. This case can happen for example when one has an XMLHttpRequest that gets fired on an onunload event for a window: the XMLHttpRequest gets in fact created when the window to be closed is still there, and then the request is sent (ie `open()`) when this window has lost its focus and potentially different window has gained focus. The way to avoid this problem is to set a listener on the new window "activate" event that gets set when the old window has its "unload" event fired.
## Downloading JSON and JavaScript from extensions[Edit](https://developer.mozilla.org/zh-CN/docs/Web/API/XMLHttpRequest/Using_XMLHttpRequest$edit#Downloading_JSON_and_JavaScript_from_extensions)
For security reasons, extensions should never use `[eval()](https://developer.mozilla.org/zh-cn/JavaScript/Reference/Global_Objects/eval)` to parse JSON or JavaScript code downloaded from the web.  See [Downloading JSON and JavaScript in extensions](https://developer.mozilla.org/zh-cn/Downloading_JSON_and_JavaScript_in_extensions) for details.
## Using XMLHttpRequest from JavaScript modules / XPCOM components[Edit](https://developer.mozilla.org/zh-CN/docs/Web/API/XMLHttpRequest/Using_XMLHttpRequest$edit#Using_XMLHttpRequest_from_JavaScript_modules_XPCOM_components)
Instantiating `XMLHttpRequest` from a [JavaScript module](https://developer.mozilla.org/zh-cn/JavaScript_code_modules/Using) or an XPCOM component works a little differently; it can't be instantiated using the `XMLHttpRequest()` constructor. The constructor is not defined inside components and the code results in an error. The easiest way to work around this is to use the XMLHttpRequest constructor associated with the [hidden DOM window](https://developer.mozilla.org/zh-cn/XPCOM_Interface_Reference/nsIAppShellService#Attributes):
``` js line-numbers  language-js
```
const { XMLHttpRequest } = Components.classes["@mozilla.org/appshell/appShellService;1"]
                                     .getService(Components.interfaces.nsIAppShellService)
                                     .hiddenDOMWindow;
var req = XMLHttpRequest();
```
```
Warning: The following code will create an XMLHttpRequest associated with the nearest available window object. If this window is closed then it will cancel your XMLHttpRequest.
Gecko 16 note
(Firefox 16 / Thunderbird 16 / SeaMonkey 2.13)
This was fixed by [bug 756277](https://bugzilla.mozilla.org/show_bug.cgi?id=756277), so starting from this version it is always safe to create an XMLHttpRequest this way.
``` js line-numbers  language-js
```
const XMLHttpRequest = Components.Constructor("@mozilla.org/xmlextras/xmlhttprequest;1");
var req = XMLHttpRequest();
```
```
For C++ code, you would need to `QueryInterface` the component to an `nsIEventTarget` in order to add event listeners, but chances are that in C++, using a channel directly would be better.
      参考：https://segmentfault.com/q/1010000005955853

