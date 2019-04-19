# JSON介绍 - xqhrs232的专栏 - CSDN博客
2018年11月02日 17:44:06[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：37
个人分类：[JavaWeb技术](https://blog.csdn.net/xqhrs232/article/category/8231541)
原文地址::[https://www.cnblogs.com/kuikui/p/3176806.html](https://www.cnblogs.com/kuikui/p/3176806.html)
相关文章
1、JSON 教程----[http://www.w3school.com.cn/json/index.asp](http://www.w3school.com.cn/json/index.asp)
2、JSON百度百科----[https://baike.baidu.com/item/JSON/2462549?fr=aladdin](https://baike.baidu.com/item/JSON/2462549?fr=aladdin)
JSON定义
JSON(JavaScript Object Notation) 是一种轻量级的数据交换格式。简单地说，JSON 可以将 JavaScript 对象中表示的一组数据转换为字符串，然后就可以在函数之间轻松地传递这个字符串，或者在异步应用程序中将字符串从 Web 客户机传递给服务器端程序。这个字符串看起来有点儿古怪，但是JavaScript很容易解释它，而且 JSON 可以表示比"名称 / 值对"更复杂的结构。
JSON 语法是 JavaScript 对象表示法语法的子集。
数据在名称/值对中、数据由逗号分隔、花括号保存对象、方括号保存数组
JSON 值可以是：数字（整数或浮点数）、字符串（在双引号中）、逻辑值（true 或 false）、数组（在方括号中）、对象（在花括号中）、null
例如：
{
    "name":"小强",
    "age":16,
    "msg":["a","b"],
    "regex": "^http://.*"
};
JSON.parse是将json格式的字符串转换成json对象。
var str ='{"name":"小强","age":16,"msg":["a","b"],"regex": "^http://.*"}';
var json = JSON.parse(str);
console.log("name:" + json.name);
console.log("msgLen:" + json.msg.length);
// 结果
// name:小强
// msgLen:2
备注：单引号写在{}外，每个属性名都必须用双引号。
JSON.stringify是将json对象转换成json格式的字符串。
var json = {"name":"小强","age":16,"msg":["a","b"],"regex": "^http://.*"};
var str = JSON.stringify(json);
console.log("json:" + str);
console.log("jsonLen:" + str.length);
// 结果
// json:{"name":"小强","age":16,"msg":["a","b"],"regex":"^http://.*"}
// jsonLen:60
备注：JSON.parse和JSON.stringify支持IE8及其以上版本
将JSON转换成数组
function jsonToArray(obj){
    var r = {key:[],value:[]};
    for(var k in obj){
        if(!obj.hasOwnProperty(k)){
            continue;
        }
        r.key.push(k);
        r.value.push(obj[k]);
    }
    return r;
}
var json = {"name":"小强","age":16,"msg":["a","b"],"regex": "^http://.*"};
var arrJson = jsonToArray(json);
console.log("regexKEY:" + arrJson.key[3]);
console.log("regexVALUE:" + arrJson.value[3]);
console.log("KEYLen:" + arrJson.key.length);
// 结果
// regexKEY: regex
// regexVALUE: ^http://.*
// KEYLen:4
关于IE低版本可以使用插件
json官网：[http://www.json.org/json-zh.html](http://www.json.org/json-zh.html)
题外话：IE低版本的浏览器使用率越来越少，建议兼容IE低本的脚本单独提出来，将兼容性代码抽离出来。用IE注释来判断是否引入js插件，js插件最好与标准名称一致，如低版本中引入JSON这个变量，也有个方法JSON.parse等等。可以改善以前的兼容性代码处理方式，将IE那部分的都转成W3C标准事件处理函数，单独为IE引入。个人见解。
