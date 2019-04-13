
# js 使用技巧 - [近几年工作中的经验总结的技巧] - 萌萌的It人 www.itmmd.com - CSDN博客


2015年01月16日 13:21:04[Jlins](https://me.csdn.net/dyllove98)阅读数：1569


1、如果 ajax 返回单一的 json 格式，接收方需要这样再格式化一下赋值： var str = eval("(" + msg + ")");
开发引用： /// <reference path="http://x.autoimg.cn/as/static/js/jquery-1.7.2.min.js" />
2、如果 ajax 同发出两个以上的请求时，返回的状态会串，不能用异步，用同步可以解决问题；
3、navigator.plugins["Shockwave Flash"] 检查 当前机器 flash 版本
4、删除前后空格 String.prototype.trim = function () { return this.replace(/(^[ |　]*)|([ |　]*$)/g, ""); }
5、IE6 下 JS 在执行一个方法后，当前方法内的代码执行完后，此次的单线程就会停止，当前的方法里还有其它的方法也不会再执行；如果要执行的话，需要加 setTimeout(); 事件再执行；
6、把 document.getElementById(id) 转换成 $("id")
function $(id) { return typeof (id) == 'string' ? document.getElementById(id) : id }
7、图片加载失败，并防止死循环 onerror="this.src=aaa.jpg;this.onerror=null;"
8、document.getElementsByTagName('*').length 查看页面有多少个 Dom 元素；
9、parseInt() 只会返回整数部分；一个完整的parseInt应该是这样的：parseInt(string, radix)，其中radix指定数字的进制（十进制，二进制，十六进制etc.） parseInt("f",16): 15
把加号放在包含合法数字的字符串前面会将字符串转化为数字；
Null 用成数字时会表现为0，做布尔时表现为false.
声明一个变量但没有赋值，此时这个变量的值为undefined. Undefined用作数字时类型表现为NaN, 用作布尔时表现为false.
10、各种正则验证规则数字验证规则：
"^\\d+$"　　//非负整数（正整数 + 0）
"^[0-9]*[1-9][0-9]*$"　　//正整数
"^((-\\d+)|(0+))$"　　//非正整数（负整数 + 0）
"^-[0-9]*[1-9][0-9]*$"　　//负整数
"^-?\\d+$"　　　　//整数
"^\\d+("　　//非负浮点数（正浮点数 + 0）
"^(([0-9]+\\.[0-9]*[1-9][0-9]*)|([0-9]*[1-9][0-9]*\\.[0-9]+)|([0-9]*[1-9][0-9]*))$"　　//正浮点数
"^((-\\d+("　　//非正浮点数（负浮点数 + 0）
"^(-(([0-9]+\\.[0-9]*[1-9][0-9]*)|([0-9]*[1-9][0-9]*\\.[0-9]+)|([0-9]*[1-9][0-9]*)))$"　　//负浮点数
"^(-?\\d+)("　　//浮点数
var r = /^\+?[1-9][0-9]*$/;　　//正整数
r.test(str);

11、按照 json 的属性值排序
var cc=[
{ name: "a", age: 30},
{ name: "c", age: 24},
{ name: "b", age: 28},
{ name: "e", age: 18},
{ name: "d", age: 38}
].sort(function(obj1, obj2) {
return obj1.age - obj2.age;
});
for(var i=0;i<cc.length;i++){
alert(cc[i]['age']); //依次显示 18,24,28,30,38
}

12、合并数组：
var mergeTo = [4,5,6];
var mergeFrom = [7,8,9];
Array.prototype.push.apply(mergeTo, mergeFrom);
mergeTo; // is: [4, 5, 6, 7, 8, 9]
13、多点击事件获取点击的是哪个
$('\#IndexLink,\#IndexLink1').on('click', function (e) {
var id=e.target.id;
> id 取到的就是被点击的ID值
}
14、zepto 里面或者 jquery 里返回 ajax json 串时，如果你的 json 串不规范（里面的键名，值，必须是双引号），就会跳出 success 方法，而且不执行里面的所有语句；
15、再JavaScript中下面做法会声明全局变量
在function外使用声明变量（无论是否使用var） ；
在function内不是用var 声明变量 ；
直接赋值于window属性；
16、锚点定位方法
window.location.hash = 'm001';
<a name="m001">&nbsp;</a>
17、window.history.forward(1); 阻止页面后退；
18、 JS call 与aplly 用法
function Person(name, age) {
this.name = name;
this.age = age;
};
function Student(name, age, grade) {
Person.apply(this, arguments);
this.grade = grade;
};
var student = new Student('qian', 21, '一年级');
alert('name:' + student.name + '\n' + 'age:' + student.age + '\n' + 'grade:' + student.grade);
//也就是通俗一点讲就是:用student去执行Person这个类里面的内容,在Person这个类里面存在this.name等之类的语句,
//这样就将属性创建到了student对象里面

**18）页面到底部自动加载内容：**
var divH = document.body.scrollHeight,top = document.body.scrollTop,windowH = window.screen.availHeight;
if ((top + windowH) >divH) {
console.log('该他妈的加载内容了。');
}
console.log('网页正文全文高：' + document.body.scrollHeight + ' 网页被卷去的高： ' + document.body.scrollTop + ' 屏幕可用工作区高度:' + window.screen.availHeight);

**其他精彩文章**
## [jQuery教程(19)-jquery ajax操作之序列化表单](http://www.itmmd.com/201501/497.html)
## [jQuery教程(18)-ajax操作之执行POST请求](http://www.itmmd.com/201501/496.html)
## [jQuery教程(20)-jquery ajax + php 操作之为Ajax请求提供不同...](http://www.itmmd.com/201501/499.html)
## [jQuery教程(21)-jquery ajax 回调函数](http://www.itmmd.com/201501/500.html)
## [jQuery教程(22)-ajax操作之错误处理](http://www.itmmd.com/201501/503.html)

## [jQuery教程(24)-ajax操作之Ajax和事件](http://www.itmmd.com/201501/504.html)

更多关于[android开发](http://www.itmmd.com/mobile.html)文章


