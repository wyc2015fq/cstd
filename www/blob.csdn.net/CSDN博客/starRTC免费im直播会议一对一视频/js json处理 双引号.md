# js json处理  双引号 - starRTC免费im直播会议一对一视频 - CSDN博客
2018年08月17日 15:23:21[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：1843
在数据传输流程中，json是以文本，即字符串的形式传递的，而JS操作的是JSON对象
  JSON字符串:
    var str1 = '{ "name": "cxh", "sex": "man" }';
    JSON对象:
    var str2 = { "name": "cxh", "sex": "man" };
//使用 JavaScript 内置函数 JSON.parse() 将字符串转换为 JavaScript 对象:
 var obj = JSON.parse(str); //由JSON字符串转换为JSON对象
jQuery插件支持的转换方式
$.parseJSON( jsonstr ); //jQuery.parseJSON(jsonstr),可以将json字符串转换成json对象
[JSON.stringify()](http://www.runoob.com/js/javascript-json-stringify.html) 用于将 JavaScript 对象转换为 JSON 字符串。
拿到一个JSON格式的字符串，我们直接用JSON.parse()把它变成一个JavaScript对象：
JSON.parse('[1,2,3,true]'); // [1, 2, 3, true] JSON.parse('{"name":"小明","age":14}'); // Object {name: '小明', age: 14}
JSON.parse('123.45'); // 123.45
{"quality": "B"A"D"}
实际上要 value.replace("\"","\\\"")
也就是 {"quality": "B\"A\"D"}
