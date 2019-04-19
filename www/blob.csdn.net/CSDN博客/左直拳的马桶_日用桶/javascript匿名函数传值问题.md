# javascript匿名函数传值问题 - 左直拳的马桶_日用桶 - CSDN博客
2017年11月10日 18:57:44[左直拳](https://me.csdn.net/leftfist)阅读数：553
匿名函数里的对象，跟定义匿名函数页面的上下文有关，而与调用匿名函数的页面无关。
很拗口。举例说明：
```xml
//在某个“容器”页面
<div id="map_workspace"></div>
<script>
function showWorkSpace(callback, h) {
    var height = 500;
    callback("map_workspace",height);
}
</script>
//在该“容器”页面中的某个iframe页面
<script>
parent.showWorkSpace(function (containerId, h) {
    var container = $("#" + containerId);
    container.empty().html("<iframe class='frame frameMap' style='width:100%;height:" + h + ";' src='" + url + "'></iframe>");
});
</script>
```
匿名函数在iframe里定义，而在“容器”页面执行，而$(“#map_workspace”)对象很明显在“容器”页面上，但偏偏，没有获得我们预期的结果。改为：
```php
//在某个“容器”页面
function showWorkSpace(callback, h) {
    var height = 500;
    callback($("#map_workspace"),height);
}
//在该“容器”页面中的某个iframe页面
parent.showWorkSpace(function (container, h) {
    container.empty().html("<iframe class='frame frameMap' style='width:100%;height:" + h + ";' src='" + url + "'></iframe>");
});
```
效果如愿。
什么道理，我也说不上来，仅作记录。
