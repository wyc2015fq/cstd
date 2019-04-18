# Html5 postmessage 子父窗口传值 - z69183787的专栏 - CSDN博客
2018年08月05日 13:38:28[OkidoGreen](https://me.csdn.net/z69183787)阅读数：222
[https://blog.csdn.net/hjjoe1213123/article/details/51698404](https://blog.csdn.net/hjjoe1213123/article/details/51698404)
最近做一个POS机终端遇到一个问题，子父窗口传值问题，因为POS机是两个屏幕，如果将一个页面拉长投射虽然可以做到两个屏幕显示，但是因为是触摸屏，当第一个屏幕在操作的时候会影响到第二屏幕，反之也是如此，既然需求明确了，问题也知道了，则我们需要两个窗口进行不同的操作
首先是父页面：
- 
`<!DOCTYPE html>`
- 
`<html lang="en">`
- 
`<head>`
- 
`<meta charset="UTF-8">`
- 
`<title>Html5 postMessage</title>`
- 
`<style>`
- 
`#otherWin {`
- 
`width: 600px;`
- 
`height: 400px;`
- 
`background-color: #cccccc;`
- 
`}`
- 
`#txt {`
- 
`width: 500px;`
- 
`height: 300px;`
- 
`background-color: #cccccc;`
- 
`}`
- 
`</style>`
- 
`</head>`
- 
`<body>`
- 
`<button id="btn">open</button>`
- 
`<button id="send">send</button>`
- 
`<input type="text" id="message" />`
- 
`<br/><br/> `
- 
`<div id="txt"></div>`
- 
`<script>`
- 
`window.onload = function() {`
- 
`var btn = document.getElementById('btn');`
- 
`var btn_send = document.getElementById('send');`
- 
`var text = document.getElementById('txt'); `
- 
`var win;`
- 
`btn.onclick = function() {`
- 
`//通过window.open打开接收消息目标窗口`
- 
`win = window.open('http://127.0.0.1:8080/mngapp/chatroom/win.html', 'popUp');`
- 
`}`
- 
`btn_send.onclick = function() { `
- 
`// 通过 postMessage 向子窗口发送数据 `
- 
`win.postMessage( document.getElementById("message").value, 'http://127.0.0.1:8080/');`
- 
`}`
- 
`if (window.addEventListener) {`
- 
`//为window注册message事件并绑定监听函数`
- 
`window.addEventListener('message', receiveMsg, false);`
- 
`}else {`
- 
`window.attachEvent('message', receiveMsg);`
- 
`}`
- 
`//监听函数，接收一个参数--Event事件对象`
- 
`function receiveMsg(e) {`
- 
`console.log("Got a message!");`
- 
`console.log("Message: " + e.data);`
- 
`console.log("Origin: " + e.origin);`
- 
`text.innerHTML = "Got a message!<br>" +`
- 
`"Message: " + e.data +`
- 
`"<br>Origin: " + e.origin;`
- 
`}`
- 
`};`
- 
`</script>`
- 
`</body>`
- 
`</html>`
然后再是子页面：
- 
`<!DOCTYPE html>`
- 
`<html lang="en">`
- 
`<head>`
- 
`<meta charset="UTF-8">`
- 
`<title>Html5 postMessage</title>`
- 
`<style>`
- 
`#txt {`
- 
`width: 500px;`
- 
`height: 300px;`
- 
`background-color: #cccccc;`
- 
`}`
- 
`</style>`
- 
`</head>`
- 
`<body>`
- 
`<h1>The New Window</h1>`
- 
`<div id="txt"></div>`
- 
`<input type="text" id="message" />`
- 
`<button id="send">send</button>`
- 
`<script> `
- 
`window.onload = function() {`
- 
`var text = document.getElementById('txt'); `
- 
`var btn_send = document.getElementById('send');`
- 
`var prent = null;`
- 
`btn_send.onclick = function() { `
- 
`// 通过 postMessage 向父窗口发送数据 `
- 
`freceiveMsg(prent);`
- 
`}`
- 
`//监听函数，接收一个参数--Event事件对象`
- 
`function receiveMsg(e) {`
- 
`console.log("Got a message!");`
- 
`console.log("Message: " + e.data);`
- 
`console.log("Origin: " + e.origin);`
- 
`text.innerHTML = "Got a message!<br>" +`
- 
`"Message: " + e.data +`
- 
`"<br>Origin: " + e.origin;`
- 
`//获取父对象`
- 
`prent = e;`
- 
`}`
- 
`function freceiveMsg(e) {`
- 
`console.log("freceiveMsg:"+e);`
- 
`e.source.postMessage(document.getElementById("message").value,`
- 
`e.origin);`
- 
`}`
- 
`if (window.addEventListener) {`
- 
`//为window注册message事件并绑定监听函数`
- 
`window.addEventListener('message', receiveMsg, false);`
- 
`}else {`
- 
`window.attachEvent('message', receiveMsg);`
- 
`}`
- 
`};`
- 
`</script>`
- 
`</body>`
- 
