# 修改函数原型prototype，使内部函数可被外界使用 - 左直拳的马桶_日用桶 - CSDN博客
2017年11月10日 19:35:28[左直拳](https://me.csdn.net/leftfist)阅读数：641
我现在写javascript，喜欢一块块的，以得到较为明晰的结构和良好的区隔性。比如：
```java
//块一
(function(){
    var a,b;
    function f1(){
        return a + b;
    }
    function f2(){
        return a - b;
    }
})();
//块二
(function(){
    var a,b;
    function f1(){
        return a * b;
    }
    function f2(){
        return a / b;
    }
})();
```
但有时，重用性又会受到些影响，万一其中的子函数要被外部使用呢？比如：
```java
//图例。arcgis for js里面的图例
var legend = (function () {
    var legend = new Legend({
        map: map,
        layerInfos: []
    }, "LegendContDiv");
    legend.startup();
    function legendClose() {
        $("#expandDiv").removeClass("toClose").addClass("toOpen");
        $("#LegendContDiv").css("display", "none");
        $("#divLegend").css("width", "18px");
        $("#divLegend").css("height", "18px");
    }
    return legend;
})();
```
现在，因为某些原因，外部也需要使用 `function legendClose()`  ，怎么办？如何写得简洁漂亮些？
十月革命一声炮响，**送来了prototype。
可以这样：
```java
//图例
var legend = (function () {
    var legend = new Legend({
        map: map,
        layerInfos: []
    }, "LegendContDiv");
    legend.startup();
    function legendClose() {
        $("#expandDiv").removeClass("toClose").addClass("toOpen");
        $("#LegendContDiv").css("display", "none");
        $("#divLegend").css("width", "18px");
        $("#divLegend").css("height", "18px");
    }
    Legend.prototype.close = function () {//<-----------
        legendClose();
    };
    return legend;
})();
legend.close();//呵呵呵呵呵呵
```
不要因为这个legend是个arcgis对象就特别照顾它，修改prototype，它一样乖乖就范。
2017.11.13
其实呢，要扩展对象legend功能，不一定要修改其原型，直接修改它自己就行了。
```java
//图例
var legend = (function () {
    var legend = new Legend({
        map: map,
        layerInfos: []
    }, "LegendContDiv");
    legend.startup();
    function legendClose() {
        $("#expandDiv").removeClass("toClose").addClass("toOpen");
        $("#LegendContDiv").css("display", "none");
        $("#divLegend").css("width", "18px");
        $("#divLegend").css("height", "18px");
    }
    legend.close = function () {//<-----直接修改legend本身
        legendClose();
    };
    return legend;
})();
legend.close();//呵呵呵呵呵呵
```
修改原型，那么所有new出来的对象都会更改；如果不想影响其他对象，直接修改本对象也是可以的。视情况选用。
