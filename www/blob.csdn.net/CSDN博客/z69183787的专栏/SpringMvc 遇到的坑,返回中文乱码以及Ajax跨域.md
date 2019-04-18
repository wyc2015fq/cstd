# SpringMvc 遇到的坑,返回中文乱码以及Ajax跨域 - z69183787的专栏 - CSDN博客
2016年10月11日 18:00:37[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1540
**前言:** 最近在写一个[Java](http://lib.csdn.net/base/java)Web后台时采用了[spring](http://lib.csdn.net/base/javaee)+MyBatis+[MySQL](http://lib.csdn.net/base/mysql)的方式.记录下遇到的关键问题
# 接口返回数据相关
## 使用@ResponseBody后返回NUll
**说明:**刚把后台运行起来,兴高采烈的测试接口数据,结果无论如何都是返回null,
最终通过各种百度,发现原来是没有引入关键的Jar包.
**解决办法:** 需要引入jackson的jar包(jackson core和jackson mapper),引入后
**图:**
![](https://img-blog.csdn.net/20160130122035632?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## 使用@RequestMapping返回中文乱码
### 原因分析:(网上基本都是一致的答案)
**首先: **确定的是(经过多次测试的结果)只有当返回值是 String时才会出现中文乱码,而当返回值是Map<String, Object>或者是其它类型时,并没有中文乱码的出现.
**然后找原因: **原因是这可以说是spring mvc的一个bug，spring MVC有一系列HttpMessageConverter去处理用@ResponseBody注解的返回值，如返回list或其它则使用 MappingJacksonHttpMessageConverter，返回string，则使用
 StringHttpMessageConverter，而这个convert使用的是字符集是iso-8859-1,而且是final的。所以在当返回json中有中文时会出现乱码。
**[java]**[view
 plain](http://blog.csdn.net/u010979495/article/details/50610856#)[copy](http://blog.csdn.net/u010979495/article/details/50610856#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1570445/fork)
- publicstaticfinal Charset DEFAULT_CHARSET = Charset.forName("ISO-8859-1");      
**解决办法(以及是否尝试成功):**尝试了很多种网上的办法,有一些根本无用,有一些当客户端的Accep是 application/json;时无用.最终也是结合客户端的修改才成功解决问题的。
**环境:** SpringMvc 3.1
**客户端分为三种不同的请求:**
1.浏览器中直接Get访问,Accept是"text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8"
2.用Jquery的jsonp ajax请求,Accept是"*/*"
3.用h5+环境下的mui ajax请求,Accept是"application/json;charset=utf-8"
(PS:h5+是最近做的跨平台移动项目的开发环境,另外就是有试过默认的Accept"application/json"的话不管服务器端用哪种方法,在呢么配置,都会返回乱码,所以最后只得手动加上charset=utf-8了)
**注: **如果什么都不加,第一种和第二种都是返回的乱码,第三种由于手动要求了返回格式,所以没有乱码,这里之所以要第三组做对比是因为有一些方法会造成第三组不能正常访问。
#### 尝试方法一: 在配置文件中的mvc:annotation-driven中添加如下代码
**[html]**[view
 plain](http://blog.csdn.net/u010979495/article/details/50610856#)[copy](http://blog.csdn.net/u010979495/article/details/50610856#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1570445/fork)
- <mvc:annotation-driven>
- <!-- 消息转换器 -->
- <mvc:message-convertersregister-defaults="true">
- <beanclass="org.springframework.http.converter.StringHttpMessageConverter">
- <propertyname="supportedMediaTypes"value="text/html;charset=UTF-8"/>
- </bean>
- </mvc:message-converters>
- </mvc:annotation-driven>
**原理:** StringHttpMessageConverter的父类里有个List<MediaType> supportedMediaTypes属性，用来存放 StringHttpMessageConverter支持需特殊处理的 MediaType 类型，如果需处理的 MediaType 类型不在 supportedMediaTypes列表中，则采用默认字符集。
**最终结果:**
请求方法1中文乱码
请求方法2返回正确的中文
请求方法3返回正确的中文
#### 尝试方法二: 在配置文件中的mvc:annotation-driven中添加如下代码
**[html]**[view
 plain](http://blog.csdn.net/u010979495/article/details/50610856#)[copy](http://blog.csdn.net/u010979495/article/details/50610856#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1570445/fork)
- <mvc:annotation-driven>
- <mvc:message-converters>
- <beanclass="org.springframework.http.converter.StringHttpMessageConverter">
- <propertyname="supportedMediaTypes">
- <list>
- <spanstyle="white-space:pre"></span><value>text/html;charset=UTF-8</value>
- <value>application/json;charset=UTF-8</value>
- <value>*/*;charset=UTF-8</value>
- </list>
- </property>
- </bean>
- </mvc:message-converters>
- </mvc:annotation-driven>
**原理:** 原理同上,但是这里多加了几个value。
**最终结果:**
请求方法1中文乱码
请求方法2返回正确的中文
请求方法3返回正确的中文
关于这种类型的方法为什么不能正确解决,原因网上都是各不相同,这里也不敢轻易做出结论.总之,目前来看这种方法不能解决问题
#### 尝试方法三: 在@RequestMapping里的配置produces="text/html;charset=UTF-8;"
**[java]**[view
 plain](http://blog.csdn.net/u010979495/article/details/50610856#)[copy](http://blog.csdn.net/u010979495/article/details/50610856#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1570445/fork)
- @RequestMapping(value = "***",produces="text/html;charset=UTF-8;")  
**原理:** 手动给对应的Accept返回制定格式编码数据。
**最终结果:**
请求方法1返回正确的中文
请求方法2返回正确的中文
请求方法3无法请求,出了错,因为produces没有添加application/json;对应的头部.
#### 尝试方法四: 在@RequestMapping里的并发配置produces={"application/json;","text/html;charset=UTF-8;"}
**[java]**[view
 plain](http://blog.csdn.net/u010979495/article/details/50610856#)[copy](http://blog.csdn.net/u010979495/article/details/50610856#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1570445/fork)
- @RequestMapping(value = "***",produces={"application/json;","text/html;charset=UTF-8;"})  
**原理:** 手动给对应的Accept返回制定格式编码数据。
**最终结果:**
请求方法1返回中文乱码
请求方法2返回中文乱码
请求方法3返回正确的中文
#### 尝试方法五: 在@RequestMapping里的并发配置produces={"text/html;charset=UTF-8;","application/json;"}
**[java]**[view
 plain](http://blog.csdn.net/u010979495/article/details/50610856#)[copy](http://blog.csdn.net/u010979495/article/details/50610856#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1570445/fork)
- @RequestMapping(value = "***",produces={"text/html;charset=UTF-8;","application/json;"})  
****注意:** 这里和上个方法的区别是,produces里面的顺序对调了**
**原理:** 手动给对应的Accept返回制定格式编码数据。
**最终结果:**
请求方法1返回正确的中文
请求方法2返回正确的中文
请求方法3返回正确的中文
**方法四和方法五对比分析:**
发现produces设置多个Accept只有第一个的charset是有用的,
后面的Accept设置有效(因为不设置就无法接收对应的Accept请求),但是charset设置是无效的.需要客户端手动制定charset才行.
具体原因并不清楚(原谅我并不精通)
**所以得出的结论是:**
#### produces={"text/html;charset=UTF-8;","application/json;"}
这样设置,这样普通浏览器的请求就能正常显示中文,而客户端的模拟请求(可以是ajax或http)则手动指定Accept的charset,即可正常接收中文。
## 客户端使用Ajax请求跨域问题
**说明:**刚刚搭建的Java web后台程序,用Http请求很正常,但是用普通的ajax请求时,出现了一个跨域问题,被拒绝访问.
**解决办法:**
在.net后台布置在IIS服务器上的,直接可以在IIS的应用池中配置Access-Control-Allow-Origin: *就行了.
PhP后台也只需要手动配置
header('Access-Control-Allow-Origin: *');
header('Access-Control-Allow-Headers: Origin, X-Requested-With, Content-Type, Accept');
Java Web后台可以再对应的tomcat服务器上通过CORS配置跨域访问(详情见另一篇),这里暂时只探讨Java Web的JSONP实现
**于是: ** 这里的Java Web后台采用了JSONP传输方式来支持跨域.
(缺点就是只支持Get,而且需要服务器端有JSONP的判断,因为返回参数和普通的不一样)
**JSONP前端代码:(这里是用了JQ的jsonp请求)**
**[javascript]**[view
 plain](http://blog.csdn.net/u010979495/article/details/50610856#)[copy](http://blog.csdn.net/u010979495/article/details/50610856#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1570445/fork)
- <span style="white-space:pre">                    </span>$.ajax(url, {  
-                         data: data,  
-                         dataType: "jsonp",  
-                         jsonpCallback: 'testGetData',  
-                         timeout: "15000", //超时时间设置为3秒；
-                         type: "POST",  
-                         success: function(response) {  
- var result = 'success:' + JSON.stringify(response);  
- //返回的是utf8编码,需要手动转为utf16
-                             console.log(result);  
- //alert(result);
-                         },  
-                         error: function(error) {  
- var result = 'error:' + JSON.stringify(error);  
-                             console.log(result);  
- //alert(result);
-                         }  
-                     });  
JSONP传递Java Web后台代码片段:
1.在方法的最开头获取是否是JSONP请求-通过获取callback参数
**[java]**[view
 plain](http://blog.csdn.net/u010979495/article/details/50610856#)[copy](http://blog.csdn.net/u010979495/article/details/50610856#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1570445/fork)
- //判断是否是jsonp请求
- String jsoncallback = request.getParameter("callback");  
2.判断如果是JSONP的请求则用JSONP的返回参数,否则用正常的(示例里面使用了Jackson来辅助)
**[java]**[view
 plain](http://blog.csdn.net/u010979495/article/details/50610856#)[copy](http://blog.csdn.net/u010979495/article/details/50610856#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1570445/fork)
- publicstatic String getJsonPData(String callbackName,Map<String, Object> data) throws JsonGenerationException, JsonMappingException, IOException{  
-         ObjectMapper mapper = new ObjectMapper();  
- 
-         String json = mapper.writeValueAsString(data);  
-         System.out.println("jsonp回调:"+callbackName);  
-         System.out.println("jackson解析的字符串:"+json);  
-         String result = "";  
- if(callbackName==null||callbackName==""){  
- //普通请求
-             result = json;  
-         }else{  
- //jsonp请求,返回的格式是类似于一个函数的字符串形式(前端再执行这个回调来获取数据)
-             result = callbackName+"("+json+")";  
-         }  
-         System.out.println("最终结果:"+result);  
- return result;    
-     }  
**至此,JSONP的请求就搞定了！**
(原理网上一大堆资料,实现起来也并不难)
