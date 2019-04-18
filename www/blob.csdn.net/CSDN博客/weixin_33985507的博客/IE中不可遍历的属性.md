# IE中不可遍历的属性 - weixin_33985507的博客 - CSDN博客
2010年01月09日 15:49:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
IE的for...in循环存在严重的缺陷，除了性能低下外，有许多属性不可遍历，著名有这三兄弟：constructor  ,toString ,valueOf。但实际情况更恐怖。下面是来自火狐官网的测试代码：
```java
var ret = (function(){
var obj = {
    constructor : function() { return "0"; }
    ,toString : function() { return "1"; }
    ,valueOf : function() { return "2"; }
    ,toLocaleString : function() { return "3"; }
    ,prototype : function() { return "4"; }
    ,isPrototypeOf : function() { return "5"; }
    ,propertyIsEnumerable : function() { return "6"; }
    ,hasOwnProperty : function() { return "7"; }
    ,length: function() { return "8"; }
    ,unique : function() { return "9" }
};
 
var result = [];
for(var prop in obj) {
	result.push(obj[ prop ]());
}
return result.join("");
})();
```
 
var ret = (function(){
var obj = {
    constructor : function() { return "0"; }
    ,toString : function() { return "1"; }
    ,valueOf : function() { return "2"; }
    ,toLocaleString : function() { return "3"; }
    ,prototype : function() { return "4"; }
    ,isPrototypeOf : function() { return "5"; }
    ,propertyIsEnumerable : function() { return "6"; }
    ,hasOwnProperty : function() { return "7"; }
    ,length: function() { return "8"; }
    ,unique : function() { return "9" }
};
 
var result = [];
for(var prop in obj) {
	result.push(obj[ prop ]());
}
return result.join("");
})(); 
alert(ret);
运行代码
结果：
|IE6|IE8|FF3.6|opera10|chrome4|safari4|
|----|----|----|----|----|----|
|489|489|0123456789|0123456789|0123456789|0123456789|
