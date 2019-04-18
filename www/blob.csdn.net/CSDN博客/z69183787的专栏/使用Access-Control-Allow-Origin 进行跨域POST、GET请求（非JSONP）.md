# 使用Access-Control-Allow-Origin 进行跨域POST、GET请求（非JSONP） - z69183787的专栏 - CSDN博客
2017年05月25日 17:37:02[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6077
以下为struts2 配置下，ajax接口输出 response
reponse headr进行如下设置
发起ajax请求的地址 origin 可由 document.origin得到，
Access-Control-Allow-Origin 设置为 * ，则认为对所有 origin开放，
若设置为指定值，则只对指定origin开放，
例：如设置为 http://aaa.com，则此跨域只接受 http://aaa.com 站下发起的ajax请求
请求如下：
```java
$.ajax({
     url:"http://localhost:8010/activities/prize/searchShopInfoByName?keyword=%E5%B0%8F%E8%82%A5%E7%BE%8A",
     dataType:'json',
     processData: false, 
     type:'get',
     data:"{}",
     success:function(data){
alert(1);
      //code
     },
     error:function(XMLHttpRequest, textStatus, errorThrown) {
       console.log(XMLHttpRequest.status);
       console.log(XMLHttpRequest.readyState);
       console.log(textStatus);
     }});
```
cors跨域时，所有ajax请求方式与同域请求一致，并非jsonp的请求形式，在chrome的network中也可看出，请求的initiator为 xmlHttpRequest；
jsonp 的为 script，即 <script> 请求。
![](https://img-blog.csdn.net/20170525174329329?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
```java
HttpServletResponse response = ServletActionContext.getResponse();
        response.setHeader("Access-Control-Allow-Origin", "http://event.dianping.com");
        response.setHeader("Access-Control-Allow-Credentials", "true");
        response.setContentType("text/plain;charset=UTF-8");
        PrintWriter writer = null;
        try {
            writer = response.getWriter();
        } catch (IOException ex) {
            logger.error("call Response.getWriter() failed", ex);
            return;
        }
        try {
            writer.write(content);
        }catch (Exception e){
            logger.error(e);
        }finally {
            if(writer != null){
                try {
                    writer.close();
                }catch (Exception e){
                    logger.error(e);
                }
            }
        }
```
springmvc 同理或可以使用 mvc cors注解
