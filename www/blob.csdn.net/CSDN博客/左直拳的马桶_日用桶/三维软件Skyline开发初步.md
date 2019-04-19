# 三维软件Skyline开发初步 - 左直拳的马桶_日用桶 - CSDN博客
2017年10月11日 19:43:58[左直拳](https://me.csdn.net/leftfist)阅读数：2213
三维软件Skyline做出来的东西有点炫，挺高大上的样子。那么for web的二次开发是啥门道呢？
其实，就是一个ActiveX控件而已。
所以，skyline for web的开发，首先浏览器端要安装一个skyline的软件Skyline TerraExplorer Pro；所以，只能在IE上运行，chrome是不行的。官网上也声称有个啥for web的软件，可以支持谷歌、火狐等主流，但暂时没找到。
因为只是一个ActiveX，不想 Arcgis for javascript那样，有很完整的javascript库，只有一些API而已。并且相关资料非常少，基本上唯一的依靠，只有官网上的这个API说明： 
[http://www.skylineglobe.com/skylineglobe/corporate/developer/dev_api.aspx](http://www.skylineglobe.com/skylineglobe/corporate/developer/dev_api.aspx)
调用起来相对简单，没有什么CSS，JS。就是这样用：
```xml
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
    <title>HTML Example</title>
    <script type="text/javascript">
        function Init() {
            try {
                var flyPath = "http://www.skylineglobe.com/SkylineGlobe/WebClient/PresentationLayer/WebClient/SkyglobeLB.fly";
                // attach callback to the load finished event
                SGWorld66.AttachEvent("OnLoadFinished", OnProjectLoadFinished);
                // Load default developer fly file from www.skylineglobe.com web site.
                // default load is in async mode
                SGWorld66.Project.Open(flyPath);
            }
            catch (e) {
                alert("Error: " + e.description);
            }
        }
        function OnProjectLoadFinished() {
            alert("Received project loaded event. Click OK to fly to Washington DC.");
            var Washington = SGWorld66.Creator.CreatePosition(-77.036667, 38.895111, 1500);
            SGWorld66.Navigate.FlyTo(Washington);
        }
    </script>
</head>
<body onload="Init()">
    <object id="TE" classid="clsid:3a4f9192-65a8-11d5-85c1-0001023952c1" style="width: 600px;height: 600px"></object>
    <object id="SGWorld66" classid="CLSID:3a4f9199-65a8-11d5-85c1-0001023952c1" style="visibility: hidden;height: 0"></object>
</body>
</html>
```
