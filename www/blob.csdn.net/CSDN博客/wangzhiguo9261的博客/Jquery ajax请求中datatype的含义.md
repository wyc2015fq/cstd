
# Jquery ajax请求中datatype的含义 - wangzhiguo9261的博客 - CSDN博客


2018年07月03日 09:02:42[码里偷闲王果果](https://me.csdn.net/wangzhiguo9261)阅读数：69


我的风格就是  列出例子，不过多的讲理论。
一切从一个普通的前端ajax请求jspringMVC后端的例子开始，
前端jquery ajax 请求:
$.ajax({

url: getAbsoluteUrl('score/findScore'),

type:'POST',

dataType:'json',//第1处

success:function(res){

alert(res);

},

error:function(msg){

alert(msg);

}

});

后端 springMVC代码@RequestMapping(value ="findScore", method = RequestMethod.POST, produces ="application/json")

public@ResponseBodyMapfindScore(Model model, ServletRequest request){


Map<String , Object> map =newLinkedHashMap<String, Object>();


map.put("createdUser","jiabaochina");

map.put("score",5);

map.put("status","success");

returnmap;

}

以上的代码是没有问题的，请求后会直接alert一个js对象。这是因为ajax请求dataType值为json，jquery就会把后端返回的字符串尝试通过JSON.parse()尝试解析为js对象。
我又把dataType值改为'text'，结果弹出框直接显示后台返回的json字符串。
我又把dataType值改为'html'，结果弹出框直接显示后台返回的json字符串。

以下是jquery api的解释：
通过dataType选项还可以指定其他不同数据处理方式。除了单纯的XML，还可以指定 html、json、jsonp、script或者text。
其中，text和xml类型返回的数据不会经过处理。数据仅仅简单的将XMLHttpRequest的responseText或responseHTML属性传递给success回调函数，
'''注意'''，我们必须确保网页服务器报告的MIME类型与我们选择的dataType所匹配。比如说，XML的话，服务器端就必须声明 text/xml 或者 application/xml 来获得一致的结果。
如果指定为html类型，任何内嵌的JavaScript都会在HTML作为一个字符串返回之前执行。类似的，指定script类型的话，也会先执行服务器端生成JavaScript，然后再把脚本作为一个文本数据返回。
如果指定为json类型，则会把获取到的数据作为一个JavaScript对象来解析，并且把构建好的对象作为结果返回。为了实现这个目的，他首先尝试使用JSON.parse()。如果浏览器不支持，则使用一个函数来构建。JSON数据是一种能很方便通过JavaScript解析的结构化数据。如果获取的数据文件存放在远程服务器上（域名不同，也就是跨域获取数据），则需要使用jsonp类型。使用这种类型的话，会创建一个查询字符串参数 callback=? ，这个参数会加在请求的URL后面。服务器端应当在JSON数据前加上回调函数名，以便完成一个有效的JSONP请求。如果要指定回调函数的参数名来取代默认的callback，可以通过设置$.ajax()的jsonp参数。








