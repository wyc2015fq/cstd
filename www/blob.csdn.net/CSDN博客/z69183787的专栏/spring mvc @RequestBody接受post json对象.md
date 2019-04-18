# spring mvc @RequestBody接受post json对象 - z69183787的专栏 - CSDN博客
2016年04月06日 10:59:28[OkidoGreen](https://me.csdn.net/z69183787)阅读数：15709
jq ajax通过post json对象
遇到的HTTP 415/400问题错误解决方案：http://www.linuxidc.com/Linux/2014-04/99928.htm
客户端：
[?](http://my.oschina.net/freegeek/blog/299134#)
```
`<html>`
`<head>`
`<meta http-equiv=``"Content-Type"``content=``"text/html; charset=UTF-8"``>`
`<title>Insert title here</title>`
`<script type=``"text/javascript"`
`    ``src=``"<%=basePath%>resources/scripts/jquery-2.0.3.min.js"``></script>`
`<script type=``"text/javascript"``>`
`$(``function``(){`
`    ``$(``"#send"``).click(``function``(){`
`        ``var``jsondata = $(``"#jsondata"``).val();`
`        ``var``jsonobj = JSON.parse(jsondata)`
`        `
`        ``var``callback = ``function``(data) {`
`            ``$(``"#result"``).html(JSON.stringify(data))`
`        ``}`
`        `
`        ``$.postJSON(``'<%=basePath%>api/user/test'``, jsonobj, callback)`
`        ``});`
`    ``})`
`    ``$.postJSON = ``function``(url, data, callback) {`
`        ``return``jQuery.ajax({`
`            ``'type'``: ``'POST'``,`
`            ``'url'``: url,`
`            ``'contentType'``: ``'application/json'``,`
`            ``'data'``: JSON.stringify(data),`
`            ``'dataType'``: ``'json'``,`
`            ``'success'``: callback`
`        ``});`
`    ``};`
`</script>`
`</head>`
`<body>`
`    ``JSON对象`
`    ``<br>`
`    ``<textarea id=``"jsondata"``cols=``"60"``rows=``"5"``>`
`{``"id"``:2,``"gender"``:0,``"mobile"``:``"15072370640"``,``"password"``:``"123456"``,``"accessToken"``:``"fd6bf3dd3cca4b0ca7c9099447994dba"``}`
`    ``</textarea><br>`
`    ``<button id=``"send"``>POST</button><br>`
`    ``<font color=``"red"``id=``"result"``></font>`
`</body>`
`</html>`
```
spring mvc 服务端：
[?](http://my.oschina.net/freegeek/blog/299134#)
```
```java
@RequestMapping
```
```java
(value=
```
```java
"/test"
```
```java
,method=RequestMethod.POST)
```
```java
public
```
```java
@ResponseBody
```
```java
Map<String, Object> testPostJson(
```
```java
```
```java
@RequestBody
```
```java
UserForm userForm,
```
```java
```
```java
BindingResult bindingResult) {
```
```java
```
```java
```
```java
Map<String, Object> map =
```
```java
new
```
```java
HashMap<String, Object>();
```
```java
```
```java
if
```
```java
(bindingResult.hasErrors()) {
```
```java
```
```java
map.put(
```
```java
"errorCode"
```
```java
,
```
```java
"40001"
```
```java
);
```
```java
```
```java
map.put(
```
```java
"errorMsg"
```
```java
, bindingResult.getFieldError().getDefaultMessage());
```
```java
```
```java
}
```
```java
```
```java
```
```java
map.put(
```
```java
"user"
```
```java
, userForm);
```
```java
```
```java
return
```
```java
map;
```
```java
}
```
```
测试：
![](http://static.oschina.net/uploads/space/2014/0808/010512_2Wul_782574.jpg)
结果
[?](http://my.oschina.net/freegeek/blog/299134#)
```
`{``"user"``:{``"name"``:``null``,``"id"``:2,``"password"``:``"123456"``,``"avatar"``:``null``,``"gender"``:0,``"mobile"``:``"15072370640"``,``"accessToken"``:``"fd6bf3dd3cca4b0ca7c9099447994dba"``}}`
```
注意：
@ResponseBody 导致的中文乱码问题见：http://my.oschina.net/freegeek/blog/287127
