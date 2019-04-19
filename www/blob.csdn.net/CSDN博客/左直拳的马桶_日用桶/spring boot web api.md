# spring boot web api - 左直拳的马桶_日用桶 - CSDN博客
2019年03月28日 19:43:58[左直拳](https://me.csdn.net/leftfist)阅读数：41
个人分类：[Java](https://blog.csdn.net/leftfist/article/category/2412493)
spring boot内置了tomcat，可以很方便的对外提供Web Api。有时实在是很方便，比方说，我写个后台程序，可以处理一些比较耗时的工作。而这个功能，将会开放给WEB前端调用，由WEB前端触发。
用spring boot会很方便。但如果是用C#来弄，后台是后台，还要另外写个WEB程序来提供接口，后台与WEB接口如何交互？鉴于是两个不同的进程，估计需要用到消息中间件。或者是后台这里要支持RPC，远程调用。
言归正传。spring boot项目里，如何提供一个WEB API?
上代码：
```java
@RestController
@RequestMapping(value="/api")
public class ApiController {
	//提交的参数为json
    @RequestMapping(value = "/authors/json/{id}", method = RequestMethod.POST)
    public String test(@PathVariable int id, @RequestParam("t") int type,@RequestBody Author auth){
        return String.format("id:%1$d,type:%2$d,name:%3$s;params type:json",id,type,auth.getName());
    }
	//提交的参数为键值对
    @RequestMapping(value = "/authors/kv/{id}", method = RequestMethod.POST)
    public String test2(@PathVariable int id, @RequestParam("t") int type,Author auth){
        return String.format("id:%1$d,type:%2$d,name:%3$s;params type:key-value",id,type,auth.getName());
    }
	
}
```
这段代码虽然短小，但蕴含的信息却比较丰富。
1、这是一个POST方式的API，GET的比较简单，不必多说。
2、参数通过3种方式传递：
1）路径，对应`{id}`
```java
@PathVariable int id
```
2）querystring，对应 `?t=`
```java
@RequestParam("t") int type
```
3）提交，对应 `Author auth`
```java
@RequestBody Author auth
```
@RequestBody 注解表明提交参数形式为json，对应的contentType为`application/json`
如果不加则为默认的键值对，对应的contentType为`application/x-www-form-urlencoded`
前端对应访问代码：
```
//提交键值对
var url1 = "http://localhost:8085/api/authors/kv/1?t=2";
var data1 = "name=chenqu&desc=foolish";
var ctype1 = "application/x-www-form-urlencoded;charset=utf-8";
//提交json
var url2 = "http://localhost:8085/api/authors/json/1?t=2";
var data2 = JSON.stringify({
	"name":"chenqu",
	"desc":"foolish"
});
var ctype2 = "application/json; charset=utf-8";
$.ajax({
	url: url2,
	data: data2,
	contentType: ctype2,		
	//dataType: "json",
	type: "POST",
	success: function (data) {
		alert(data);
	},
	error: function (rq, status, thrown) {
		alert(rq.responseText + "," + status + ": " + thrown);
	}
});
```
