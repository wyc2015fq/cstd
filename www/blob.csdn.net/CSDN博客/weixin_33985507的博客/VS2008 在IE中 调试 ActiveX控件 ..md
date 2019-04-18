# VS2008 在IE中 调试 ActiveX控件 . - weixin_33985507的博客 - CSDN博客
2012年07月16日 14:54:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
不知道是不是微软的粗心大意，VS2008中竟然没有ActiveX控件测试容器。
幸好需要在IE中测试控件，就不需要那个测试容器啦。
1. 生产测试控件的HTML：
<HTML>
<HEAD>
<TITLE>Test</TITLE>
</HEAD>
<BODY>
<OBJECT ID="MyActiveX" WIDTH=800 HEIGHT=600
 CLASSID="CLSID:5228A02F-8FBD-424C-B457-EEBD4E748504">
    <PARAM NAME="_Version" VALUE="65536">
    <PARAM NAME="_ExtentX" VALUE="4657">
    <PARAM NAME="_ExtentY" VALUE="4075">
    <PARAM NAME="_StockProps" VALUE="0">
</OBJECT>
</BODY>
</HTML>
2. 设置debug选项
command: C:/Program Files/Internet Explorer/iexplore.exe
command arguments: F:/test.htm
3. 全路径
注册ocx时，需要使用全路径。 否则就有错误：The sepcified module could not be found.
4. 在Vista下的问题
由于UAC的控制，需要用"Run as Admin"的方式运行
否则就有“1>Project : error PRJ0050: Failed to register output.”错误。
