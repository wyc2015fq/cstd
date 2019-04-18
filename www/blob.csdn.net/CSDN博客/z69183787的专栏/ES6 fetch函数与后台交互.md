# ES6 fetch函数与后台交互 - z69183787的专栏 - CSDN博客
2017年11月30日 12:56:05[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2615
最近在学习react-native,遇到调用后端接口的问题.看了看官方文档,推荐使用es6的fetch来与后端进行交互,在网上找了一些资料.在这里整理,方便以后查询.
1.RN官方文档中,可使用XMLHttpRequest
```
var request = new XMLHttpRequest(); request.onreadystatechange = (e) => { if (request.readyState !== 4) { return; } if (request.status === 200) { console.log('success', request.responseText); } else { console.warn('error'); } }; request.open('GET', 'https://mywebsite.com/endpoint.php');
 request.send();
这是http的原生方法,这里不做多的介绍.
2.RN官方文档中,推荐使用fetch
```
```
fetch('https://mywebsite.com/endpoint/', { method: 'POST', headers: { 'Accept': 'application/json', 'Content-Type': 'application/json', }, body: JSON.stringify({ firstParam: 'yourValue', secondParam: 'yourOtherValue', }) }).then(function(res){
　　console.log(res)
})
```
body中的数据就是我们需要向服务器提交的数据,比如用户名,密码等;如果上述body中的数据提交失败,那么你可能需要把数据转换成如下的表单提交的格式:
`fetch('https://mywebsite.com/endpoint/', { method: 'POST', headers: { 'Content-Type': 'application/x-www-form-urlencoded', }, body: 'key1=value1&key2=value2' })``.then(function(res){`
```
console.log(res)
})
这样可以获取纯文本的返回数据.
如果你需要返回json格式的数据:
`fetch(``'`
```
`https://mywebsite.com/endpoint/```'``).then(``function``(res) {``
`    ``if``(res.ok) {`
`        ``res.json().then(``function``(obj) {`
`            ``// 这样数据就转换成json格式的了`
`        ``})`
`    ``}`
`}, ``function``(ex) {`
`    ``console.log(ex)`
`})`
fetch模拟表单提交:
`fetch(``'doAct.action'``, { `
`    ``method: ``'post'``, `
`    ``headers: { `
`      ``"Content-type"``: ``"application/x-www-form-urlencoded; charset=UTF-8"`
`    ``}, `
`    ``body: ``'foo=bar&lorem=ipsum'`
`  ``})`
`  ``.then(json) `
`  ``.then(``function``(data) { `
`    ``console.log(``'Request succeeded with JSON response'``, data); `
`  ``}) `
`  ``.``catch``(``function``(error) { `
`    ``console.log(``'Request failed'``, error); `
`  ``});`
不过无论是ajax还是fetch,都是对http进行了一次封装,大家各取所好吧.
参考文档:https://developer.mozilla.org/zh-CN/docs/Web/API/GlobalFetch/fetch
