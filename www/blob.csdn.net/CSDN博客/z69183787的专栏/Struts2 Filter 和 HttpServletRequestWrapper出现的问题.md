# Struts2 Filter 和 HttpServletRequestWrapper出现的问题 - z69183787的专栏 - CSDN博客
2014年07月02日 10:10:17[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1272
**Struts2 是通过request.getParameterMap()来获取属性的，要重写这个方法**
 Struts2 的Action中定义了一个属性：
public Class LoginAction {
    private String redirectionURL；
}
现在定义了一个XSSFilter用来过滤掉url参数中的特殊信息（这个Filter配置在Struts2之前），通过一个XSSRequestWrapper包装器对request对象进行包装，在浏览器中输入：
![](http://static.oschina.net/uploads/space/2013/0503/151636_fNvi_260828.jpg)
在filter这样测试：
![](http://static.oschina.net/uploads/space/2013/0503/151420_7OsF_260828.jpg)
输出的内容是这样的：
![](http://static.oschina.net/uploads/space/2013/0503/152337_fq1W_260828.jpg)
这个说明过滤是成功的了，但是在action中进行这样测试，
![](http://static.oschina.net/uploads/space/2013/0503/152424_BtIo_260828.jpg)
输出却是这样的：
![](http://static.oschina.net/uploads/space/2013/0503/152500_5Fh4_260828.jpg)
action中redirectionURL，使用的还是为包装前的request的属性，这是为什么呢？我filter是定义在Struts2之前嘛，在Struts2通过注入方式将request对象的参数redirectionURL注入到LoginAction的redirectionURL之前，这个request对象应该是我包装过的对象嘛，包装过的话参数redirectionURL的值应该就是过滤后的，为什么这里还是没过滤的呢？
