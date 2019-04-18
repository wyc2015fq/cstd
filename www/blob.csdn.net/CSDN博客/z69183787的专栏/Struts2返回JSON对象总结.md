# Struts2返回JSON对象总结 - z69183787的专栏 - CSDN博客
2015年06月27日 09:22:05[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1036
1.导入jsonplugin包
  Struts2.1.6:导入jsonplugin-0.34.jar包(下载包)和commons-logging-1.0.4.jar(Struts2 lib下有) 
  Struts2.1.8导入struts2-json-plugin-2.1.8.1.jar(Struts2 lib下有) 
  Struts2.2.1导入struts2-json-plugin-2.2.1.jar(Struts2 lib下有) 
2.struts.xml中package 中extends="json-default"
Java代码  ![收藏代码](http://ryanjoy.iteye.com/images/icon_star.png)
- <package name="json" namespace="/json"extends="json-default">  
3.result 中type="json"
Java代码  ![收藏代码](http://ryanjoy.iteye.com/images/icon_star.png)
- <!-- 封装所有的get开头的方法 -->  
- <result type="json" name="user">  
- </result>  
- 
- <!-- 只包含user.id属性 -->  
- <result type="json" name="user">  
-     <param name="includeProperties">                 
-         user\.id  
-     </param>  
- </result>  
- 
- <!-- 返回user的JSON List列表，其中userInfosList是action中的一个List类型的属性，userInfosList \[\d+\]\. userName表示，userInfosList中存储的对象0..end的userName属性（list中存储的对象必须有userName属性）  
- -->  
- <result  name="success" type="json">  
- <param name="includeProperties">  
-     userInfosList\[\d+\]\.userName,userInfosList\[\d+\]\.password  
- </param>  
- </result>  
- 
- <!-- 不包含user属性 -->  
- <result type="json" name="list">  
-     <param name="excludeProperties">                 
-         user  
-     </param>  
- </result>  
- 
- <!-- 根对象只包含user -->  
- <result type="json">    
-     <param name="root">    
-         user  
-     </param>    
- </result>   
- 
- <!-- "root"对象中父类的field(属性)不会(会？) 默认存放到 JSON数据中，如果不想这样做，需要在配置时指定 ignoreHierarchy 为 false:  -->  
- <result type="json">    
-     <param name="ignoreHierarchy">false</param>    
- </result>   
- 
4.避免使用get开头的action方法
  在属性get方法上面加 
  @JSON(name="newName")json中的名称 
  @JSON(serialize=false) 该属性不被加入json 
  @JSON(format="yyyy-MM-dd") 格式化日期 
5.在action中赋值,返回对应的result字符串
说明：
为什么要用includeProperties或excludeProperties 参数：主要是为了过滤掉接口，pojo的set、list、其它对象等不需要的数据防止循环取其它关联对象或找不到接口。如果不配置,默认是处理action中的所有属性，如果action中有接口注入，json拦截器可能找不到接口而返回不了结果，还有如果action中有一个对象，这个对象与好多对象都有关联，json拦截器会将相关联的所有对象的属性全部转换成json格式，如果其它对象有list、set,其返回结果相当庞大，有可能是死循环而无法返回
 。如果不用<param name="includeProperties">或其他方式进行json数据过滤，通过debug你会发现前台返回的json字符串，是把action中的所有属性全部转化成json字符串返回给客户端（包括service接口、pojo所有属性及有关联的pojo。有时候根本返回不了结果，也不报错，后台执行了，但前台执行不到callback function，这主要是因为找不到接口或者关联的pojo太多，造成死循环），一般情况下用的最多的就是root、includeProperties 和excludeNullProperties参数。当然还有其他的方法，如给pojo的属性加json注解。 
总结：
action中避免使用get开头的action方法,去掉action中的接口的get方法。 为json类型的result配置includeProperties, excludeProperties等参数. 
