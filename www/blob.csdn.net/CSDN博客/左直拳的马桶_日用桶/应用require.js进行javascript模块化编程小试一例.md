# 应用require.js进行javascript模块化编程小试一例 - 左直拳的马桶_日用桶 - CSDN博客
2017年03月02日 19:06:28[左直拳](https://me.csdn.net/leftfist)阅读数：360
长久以来都渴望应用javascript的模块化编程。今日紧迫更甚，岁月蹉跎，已经不能再等了。
拜读阮一峰的有关文章已经好几遍，文章写得真好，简洁流畅，头头是道，自觉有点明白了。但经验告诉我们，一定要亲手试一试。
一试之下，果然走了一些弯路，加深了理解。
**一、下载require.js**
[http://requirejs.org/docs/download.html](http://requirejs.org/docs/download.html)
**二、编写相应测试代码。**计有： 
页面：`/index.html`
主体结构定义JS：`/main.js`
符合AMD规范的模块化JS：
```
/js/amd/module-A.js
/js/amd/module-B.js
```
传统意义上的，不符合AMD规范、非模块化的JS
`/js/noamd/file-C.js`
![这里写图片描述](https://img-blog.csdn.net/20170302182005245?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
代码如下：
```java
//module-A.js
//define方法，应该是在require.js定义的。这也是AMD规范的内容之一
define(function (){
    var add = function (x,y){
        return x + y;
    };
    return {
        add: add
    };
});
```
```java
//module-B.js
//依赖于moduleA
define(['moduleA'], function(ma){
    function compute(x,y){
        return ma.add(x,y);
    }
    return {
        compute : compute
    };
});
```
```java
//这是一个传统意义上，非AMD规范的JS；但是，还是要符合一定的规范：需要返回一个对象，以便在main.js里进行exports，然后外部才能进行使用（使用这个对象里面的方法）。
var objectC = (function(){
    function multiply(x,y){
        return x * y;
    }
    return {
        multiply:multiply
    }
})();
```
至关重要的main.js
```java
//main.js
//重点在于如何映射非规范模块
require.config({
    baseUrl: "js",
    shim: {
        'moduleC': {
            deps: [],
            exports: 'objectC'//这个名字，跟file-C.js里的"var objectC=..." 对应
        }
    },
    paths: {
        "moduleA": "amd/module-A",
        "moduleB": "amd/module-B",
        "moduleC": "noamd/file-C"//非规范化文件这一步也是要的，并非单单在shim里定义了就行
    }
});
```
![这里写图片描述](https://img-blog.csdn.net/20170302185858501?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
瞪灯瞪等！且看如何应用： 
index.html
```
<!DOCTYPE html>
<html>
<head>
    <script src="require.js"></script>
    <script src="main.js"></script>
</head>
<body>
    <script type="text/javascript">
        require(['moduleB','moduleC'], function (moduleB,moduleC){
            alert(moduleB.compute(3,4));//7
            alert(moduleC.multiply(3,4));//12
　　  });
    </script>
</body>
</html>
```
参考文章：
[Javascript模块化编程（三）：require.js的用法](http://www.ruanyifeng.com/blog/2012/11/require_js.html)
[基于gulp requirejs rjs的前端自动化构建系列文章(二)](http://hcysun.me/2015/11/14/%E5%9F%BA%E4%BA%8Egulp-requirejs-rjs%E7%9A%84%E5%89%8D%E7%AB%AF%E8%87%AA%E5%8A%A8%E5%8C%96%E6%9E%84%E5%BB%BA%E7%B3%BB%E5%88%97%E6%96%87%E7%AB%A0-%E4%BA%8C/)
