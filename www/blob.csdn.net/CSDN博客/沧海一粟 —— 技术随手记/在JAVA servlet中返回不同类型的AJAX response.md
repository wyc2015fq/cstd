# 在JAVA servlet中返回不同类型的AJAX response - 沧海一粟 —— 技术随手记 - CSDN博客





2011年04月11日 21:37:00[慢游](https://me.csdn.net/eengel)阅读数：19941








    废话不多说，直接上例子。
**String**    后台返回一段文字到前台是最常见的Ajax应用。比如后台返回一个名字或一条警告信息，让前台显示在页面上。
**前台请求**var deferred = dojo.xhrGet({
     url: "/test_submit.do",
     handleAs: "text",
     load: function(response, ioArgs){
      alert(response);
      return response;
     },
     error: function(response, ioArgs){
      return response;
     }
    });

**后台返回**public ActionForward execute(ActionMapping mapping, ActionForm form,
   HttpServletRequest request, HttpServletResponse response) {
response.getWriter().write("butterfly"); //alert "butterfly"
return null;
}



**Number**    如果从后台返回一个数字，在本文的例子里，前台dojo.xhrGet只能选择接收类型为”text”，因此其实后台的数字发送到前台后会被转化成string类型。值得注意的是后台写数字到response中的方式，会有些微的不同。
**前台请求**var deferred = dojo.xhrGet({
     url: "/login_submit.do",
     handleAs: "text",
     load: function(response, ioArgs){
      alert(response);
      alert(response+1);
      return response;
     },
     error: function(response, ioArgs){
      return response;
     }
    });

**后台返回**public ActionForward execute(ActionMapping mapping, ActionForm form,
   HttpServletRequest request, HttpServletResponse response) {
// response.getWriter().print(100);   //"100", 101
// response.getWriter().print("100");   //"100", 101
// response.getWriter().write(100);   //"d", "NaN"
 response.getWriter().write("100");   //"100", 101
return null;
}
    当用response.getWriter().write(100)直接返回一个数字时，会被当作是ascii码，并转换成相应的字符。



**JSON**
    JSON可以说是AJAX请求中使用最普遍的一种格式了。几乎可以称得上是万金油，可以到处抹（再次膜拜一下Douglas Crockford）。
**前台请求**    这儿前台也需要将dojo.xhrGet的handleAs参数改成”json”。
var deferred = dojo.xhrGet({
     url: "/login_submit.do",
     handleAs: "json",
     load: function(response, ioArgs){
      alert(response.type);
      return response;
     },
     error: function(response, ioArgs){
      return response;
     }
    });

**后台返回**    为了支持后台返回JSON格式，必须先将数据包装到专门的JSON对象中。这儿采用的是json-lib包。
public ActionForward execute(ActionMapping mapping, ActionForm form,
   HttpServletRequest request, HttpServletResponse response) {
JSONObject jsonObject = new JSONObject();
 jsonObject.put("name", "fly");
 jsonObject.put("type", "虫子");
 response.setCharacterEncoding("UTF-8");//这儿是为了返回中文格式的response。如果都是英文就不用了。
response.getWriter().print(jsonObject); //{"name":"fly","type":"虫子"}
return null;
}



**List**    在AJAX应用中，经常会碰到需要后台返回一个列表，比如一组数据、一整张表格等。
**前台请求**    这儿dojo.xhrGet的handleAs参数依然为”json”。
var deferred = dojo.xhrGet({
     url: "/login_submit.do",
     handleAs: "json",
     load: function(response, ioArgs){
      alert(response);
alert(response[0]+1);
      return response;
     },
     error: function(response, ioArgs){
      return response;
     }
    });

**后台返回**    为了返回List，首先容易想到的是用List对象包装数据。
public ActionForward execute(ActionMapping mapping, ActionForm form,
   HttpServletRequest request, HttpServletResponse response) {
List list = new ArrayList();
list.add("100"); // list.add(100);
list.add("200");  // list.add(200);
response.getWriter().print(list); //100, 200
return null;
}
    这儿无论100和200加不加双引号，前台得到的都会是number而不是string。但是，如果数据不是数字而是String，或者List里面的项也是比较复杂的格式如JSON等，则需要用到json-lib包中的JSONArray对象了。
public ActionForward execute(ActionMapping mapping, ActionForm form,
   HttpServletRequest request, HttpServletResponse response) {
List list = new ArrayList();
 list.add("butter");
 list.add("fly");
 JSONArray ja = JSONArray.fromObject(list);
response.getWriter().print(ja);  //"butter", "fly"
return null;
}



    上面列出了最常用的集中AJAX response格式。从这些出发，如果想要返回更复杂的response，可以通过组合这几种格式来实现。比如返回复杂的表格数据可以使用List+JSON的组合。等等。



