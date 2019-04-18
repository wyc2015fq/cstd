# Struts2 无后缀action请求 - z69183787的专栏 - CSDN博客
2014年08月25日 18:37:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：10150
struts.properties 中默认 设置为：
### Used by the DefaultActionMapper
### You may provide a comma separated list, e.g. struts.action.extension=action,jnlp,do
struts.action.extension=action
以action为后缀，注释掉，
web.xml中配置拦截为 /* 即可无后缀访问
struts2是可以配置默认的后缀名的，如http://localhost:8080/test.action，这个是默认的，但是也可以通过配置去修改这个.action为别的。
这里是通过一个常量配置改变的。
Java代码  ![收藏代码](http://irfen.iteye.com/images/icon_star.png)
- <constant name="struts.action.extension" value="do"/>  
 这样的就会变为http://localhost:8080/test.do来访问。
但是我很不喜欢有后缀，百度了很多大家的结论一般都是使用SmartUrls和urlrewritefilter这两个解决，但是我只是想把后缀去掉而已，就让我增加别的组件这个实在很不爽，最后终于找到了个方法：
Java代码  ![收藏代码](http://irfen.iteye.com/images/icon_star.png)
- <constant name="struts.action.extension" value=","/>  
 在这里把value配置为,就可以使用http://localhost:8080/test完成访问了
把这段注释加上吧，大家可以看看
Java代码  ![收藏代码](http://irfen.iteye.com/images/icon_star.png)
- ### Used by the DefaultActionMapper  
- ### You may provide a comma separated list, e.g. struts.action.extension=action,jnlp,do
- ### The blank extension allows you to match directory listings as well as pure action names  
- ### without interfering with static resources, which can be specified as an empty string  
- ### prior to a comma e.g. struts.action.extension=, or struts.action.extension=x,y,z,,  
- struts.action.extension=action,,  
如果将Struts2的filter-mapping配置成
```
<filter-mapping>  
    <filter-name>struts2</filter-name>  
    <url-pattern>/*</url-pattern>  
</filter-mapping>
```
的话，struts2将可以处理无后缀的action请求，如
/demo/hello.action 和/demo/hello 是同样的效果。
这样有时会出现一些问题，将有一些组件映射为无后缀的请求拦截了，如/demo/dwr 请显示对外公布的js方法列表，在struts2拦截后确显示
```
There is no Action mapped for namespace / and action name dwr
```
这种情况下可以在struts.properties中配置
```
struts.action.excludePattern = /dwr.*,/xxx.*
```
属性值是以逗号分隔的正则表达式。
