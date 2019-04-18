# struts2 在result type = json 时 ，兼容json与jsonp 请求 - z69183787的专栏 - CSDN博客
2015年05月21日 20:57:47[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4051
<result type="json">
<param name="callbackParameter">callback</param>
</result>
即可，查看 struts2 JSONResult 源代码后，发现兼容json和jsonp
```java
protected String createJSONString(HttpServletRequest request, Object rootObject) throws JSONException {
    String json = JSONUtil.serialize(rootObject, this.excludeProperties, this.includeProperties, this.ignoreHierarchy, this.enumAsBean, this.excludeNullProperties);
    json = addCallbackIfApplicable(request, json);
    return json;
  }
```
```java
protected String addCallbackIfApplicable(HttpServletRequest request, String json) {
    if ((this.callbackParameter != null) && (this.callbackParameter.length() > 0)) {
      String callbackName = request.getParameter(this.callbackParameter);
      if ((callbackName != null) && (callbackName.length() > 0))
        json = callbackName + "(" + json + ")";
    }
    return json;
  }
```
在项目中需要添加一个数据分析模块，为了不影响现有业务，需要在另一台服务器上面进行相应的分析。首先想到的就是将分析结果保存为JSON格式，页面通过AJAX获取这些数据。那么自然就涉及到AJAX跨域的问题，而使用JSONP解决跨域问题是最简单的。
Struts 2支持JSONP，只需要在result中配置相应的属性即可。
<actionname="saleRank.*"class="saleRankAction"method="{1}">
<resultname="rank"type="json">
<paramname="callbackParameter">callback</param>
<paramname="includeProperties">
                        saleRank.*
</param>
</result>
</action>
指定callbackParameter参数即可。 页面上面的调用代码为：
$(function(){
    $.ajax({
         type: "get",
         async: true,
         url: "http://remote-server/saleRank.rank.action",
         data: {'nick': 'www.darrenfang.com'},
         dataType: "jsonp",
         jsonp: "callback",
         jsonpCallback:"handle"
     })
     .done(function(data){
// 调用成功处理逻辑
     })
     .fail(function(){
// 调用失败处理逻辑
     });
});
需要注意的是Struts2中配置的callbackParameter值要和页面AJAX调用参数中的jsonp的值一样（代码中高亮的行）。
第9行的jsonpCallback这个参数可以不用加上，jQuery会自动生成一个名称。特别注意，如果同一个页面中有多次JSONP调用，最好将这个参数设置成不同的值，或者不加上这个参数。如果这个参数的值是一样的，那么后面的回调函数会覆盖前面的。
