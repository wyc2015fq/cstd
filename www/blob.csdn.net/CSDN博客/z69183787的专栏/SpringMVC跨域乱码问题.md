# SpringMVC跨域乱码问题 - z69183787的专栏 - CSDN博客
2016年10月11日 17:59:51[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1366
最近的工作重点转移至web开发,使用框架主要还是spring mvc, jackson, jQuery,但在开发中又遭遇了n年前的跨域问题,jsonp乱码问题,其实这些问题产生原因很简单,但实际上解决起来却需要了解各种机制运行内幕.
#### 解决方案
- 在web.xml中设置CharacterEncodingFilter,相信绝大多数乱码问题均可以解决,这其中的原理不用再讲解,毕竟这个filter的代码非常简单.
```
```
|123456789101112131415161718192021|<!--CharacterEncodingfilter--><filter><filter-name>encodingFilter</filter-name><filter-class>org.springframework.web.filter.CharacterEncodingFilter</filter-class><init-param><param-name>encoding</param-name><param-value>UTF-8</param-value></init-param><init-param><param-name>forceEncoding</param-name><param-value>true</param-value></init-param></filter><filter-mapping><filter-name>encodingFilter</filter-name><url-pattern>/*</url-pattern><dispatcher>REQUEST</dispatcher><dispatcher>FORWARD</dispatcher></filter-mapping>|
- 由于我使用的是spring mvc并且提供ajax访问,但在部署服务时出现了跨域问题,好在我使用的jQuery已经提供了解决跨域访问的方案,你只需要在原来的访问地址上加上?callback=?即可
当ajax请求发起时,jQuery会将callback赋值,而服务端也需要在业务处理完毕后将此参数做为方法名,而json串做为方法参数一起返回给jQuery,让其回调.
```
```
|12345|$.getJSON(host+"api/service/a.json?callback=?",function(data){// TODO callback方法}|
这里JSON_FORMAT = “.json”,整个value等同于请求地址,method表示请求方式,由于jsonp只支持get方法,这里将设置成GET方法,最后将文档类型设定为json,而jsonMapper则是springside的一个工具类[JsonMapper](https://github.com/springside/springside4/blob/V4.0.0.GA/modules/core/src/main/java/org/springside/modules/mapper/JsonMapper.java),这也算是jsonp最简单的封装方式了.
测试通过成功跨域调用,但很不幸出现了乱码.那么为什么在不跨域时对象成功返回,而跨域后却出现乱码呢?这还是需要对照两者的实现方式
```
```
|123456789|// 跨域请求@RequestMapping(value="a"+JSON_FORMAT,method=RequestMethod.GET,produces=MediaType.APPLICATION_JSON_VALUE)@ResponseBodypublicStringdoit(@RequestParam(@RequestParam("callback")Stringcallback){returnjsonMapper.toJsonP(callback,newObject());}|
```
```
|123456789|// 非跨域请求@RequestMapping(value="a"+JSON_FORMAT,method=RequestMethod.GET,produces=MediaType.APPLICATION_JSON_VALUE)@ResponseBodypublicObjectdoit(){returnnewObject();}|
在非跨域请求中,它会默认使用MappingJackson2HttpMessageConverter将对象转成json串,而它使用的编码格式是utf-8,所以这里不会出现乱码问题.
而当我们使用成jsonp后,由于返回对象是String对象,那么它将使用StringHttpMessageConverter进行转换,但这里的编码格式是ISO-8859-1,所以会出现乱码.
所以第一种最容易想到的方案就出来了,即按照ISO-8859-1的方式构造一个新的字符串
```
```
|12345678|@RequestMapping(value="a"+JSON_FORMAT,method=RequestMethod.GET,produces=MediaType.APPLICATION_JSON_VALUE)@ResponseBodypublicStringdoit(@RequestParam(@RequestParam("callback")Stringcallback){returnnewString(jsonMapper.toJsonP(callback,newObject()).getBytes("UTF-8"),Charset.forName("ISO-8859-1"));}|
虽然这种方式可以解决乱码,但明显创建了很多无用对象,并且每一次都要这样转换很麻烦,那么有没有更简单的方式呢,答案是肯定的
```
```
|12345678|@RequestMapping(value="a"+JSON_FORMAT,method=RequestMethod.GET,produces=MediaType.APPLICATION_JSON_VALUE+CHARSET)@ResponseBodypublicStringdoit(@RequestParam(@RequestParam("callback")Stringcallback){returnjsonMapper.toJsonP(callback,newObject());}|
只需要在produces后指定编码格式即可,为了方便使用我将它定义成常量,方便后期使用
```
```
|123|privatefinalstaticStringCHARSET=";charset=UTF-8";|
这样就完美解决了乱码问题.
当然,网上还有不少自定义Converter的方法,这些方法可解决乱码问题,但不能解决跨域问题,因为它将整个字符串都加上了””,而jQuey不会认为其是方法只以为是普通字符串从而不能调用,针对不同场景还需要使用不同方式进行处理.
