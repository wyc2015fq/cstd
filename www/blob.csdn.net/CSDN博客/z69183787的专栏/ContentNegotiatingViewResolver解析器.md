# ContentNegotiatingViewResolver解析器 - z69183787的专栏 - CSDN博客
2014年12月01日 22:21:31[OkidoGreen](https://me.csdn.net/z69183787)阅读数：27031
RESTful服务中很重要的一个特性即是同一资源,多种表述我们使用ContentNegotiatingViewResolver就可以做到，这个视图解析器允许你用同样的内容数据来呈现不同的view
如下面描述的三种方式:
方式1  使用扩展名
[http://www.test.com/user.xml](http://www.test.com/user.xml)    呈现xml文件
[http://www.test.com/user.json](http://www.test.com/user.json)    呈现json格式
[http://www.test.com/user](http://www.test.com/user)       使用默认view呈现，比如jsp等
-----------------------------------------------------------------------------------------------------
方式2  使用http request header的Accept
GET /user HTTP/1.1
Accept:application/xml
GET /user HTTP/1.1
Accept:application/json
….
-----------------------------------------------------------------------------------------------------
方式3  使用参数
[http://www.test.com/user?format=xml](http://www.test.com/user?format=xml)
[http://www.test.com/user?format=json](http://www.test.com/user?format=json)
这三种方式各自的优缺点这里就不再介绍了
如何使用ContentNegotiatingViewResolver？
假设我们有这么一个目标：
/user/{userid}.json    用于返回一个描述User的JSON
/user/{userid}        用于返回一个展示User的JSP页面
/user/{userid}.xml     用于返回一个展示User的XML文件
配置文件说明   （具体例子下篇文章放上）
我们知道有accept header,扩展名以及参数这三种方式，配置文件中
这里是解析器的执行顺序，如果有多个的话（前面多次解释过）
<property name=*"order"* value=*"1"*></property>
--------------------------------------------------------------------------------------------------------------
如果所有的mediaType都没匹配上，就会使用defaultContentType
<property name=*"defaultContentType"* value=*"text/html"* />
这里是是否启用扩展名支持，默认就是true
例如  /user/{userid}.json
<property name=*"favorPathExtension"* value=*"true"*></property>
这里是是否启用参数支持，默认就是true
例如  /user/{userid}?format=json
<property name=*"favorParameter" *value=*"false"*></property>
这里是否忽略掉accept header，默认就是false
例如     GET /user HTTP/1.1
Accept:application/json
<property name=*"ignoreAcceptHeader"* value=*"true"*></property>
我们的例子是采用.json , .xml结尾的,所以关掉两个
--------------------------------------------------------------------------------------------------------------
这里是扩展名到mimeType的映射,
例如 /user/{userid}.json  中的   .json  就会映射到   application/json
<property name=*"mediaTypes"*>
           <map>
              <entry key=*"json" *value=*"*application/json*"* />
              <entry key=*"xml" *value=*"*application/xml*"*/>                     </map>
</property>
注:
ContentNegotiatingViewResolver是根据客户提交的MimeType(如 text/html,application/xml)来跟服务端的一组viewResover的MimeType相比较,如果符合,即返回viewResover的数据.
而 /user/123.xml, ContentNegotiatingViewResolver会首先将 .xml 根据mediaTypes属性将其转换成 application/xml,然后完成前面所说的比较.
# 使用内容协商实现多视图例
根据前篇文件的介绍，这里直接给出例子
配置xml
<context:component-scan base-package=*"com.controls"* />
    <context:annotation-config />
    <bean class=*"org.springframework.web.servlet.view.ContentNegotiatingViewResolver"*>
        <property name=*"order"* value=*"1"* />
        <property name=*"favorParameter" *value=*"false"* />
        <property name=*"ignoreAcceptHeader" *value=*"true"* />
        <property name=*"mediaTypes"*>
            <map>
                <entry key=*"json"* value=*"application/json"* />
                <entry key=*"xml"* value=*"application/xml"* />        
            </map>
        </property>
        <property name=*"defaultViews"*>
            <list>
                <bean class=*"org.springframework.web.servlet.view.json.MappingJacksonJsonView"*></bean>
                <bean class=*"org.springframework.web.servlet.view.xml.MarshallingView"*>
                    <constructor-arg>
                        <bean class=*"org.springframework.oxm.jaxb.Jaxb2Marshaller"*>
                             <property name=*"classesToBeBound"*>
                                <list>
                                   <value>com.model.User</value>
                                </list>
                             </property>
                        </bean>
                    </constructor-arg>
                </bean>
            </list>
        </property>
    </bean>
    <!-- 上面没匹配到则会使用这个视图解析器 -->
    <bean class=*"org.springframework.web.servlet.view.InternalResourceViewResolver"*>
        <property name=*"order"* value=*"2"* />
        <property name=*"prefix"* value=*"/WEB-INF/views/"* />
        <property name=*"suffix"* value=*".jsp"* />
        <property name=*"viewClass"* value=*"org.springframework.web.servlet.view.JstlView"* />
    </bean>
Model
@XmlRootElement
**public class** User {
**private long** userID;
**private** String userName;
**private** Date birth;
**public** String getUserName() {
**return** userName;
    }
**public void** setUserName(String userName) {
**this**.userName = userName;
    }
**public** Date getBirth() {
**return** birth;
    }
**public void** setBirth(Date birth) {
**this**.birth = birth;
    }
**public long** getUserID() {
**return** userID;
    }
**public void** setUserID(**long** userID) {
**this**.userID = userID;
    }
}
Contoller
@RequestMapping(value="/user/{userid}")
**public** String queryUser(@PathVariable("userid") **long** userID, ModelMap model)
{
       User u = **new** User();
       u.setUserID(userID);
       u.setUserName("zhaoyang");
       model.addAttribute("User", u);
**return** "User";
}
如果是返回text/html，还需要建立个jsp文件
<body>
    UserName: ${requestScope.User.userID } <br />
    Age: ${requestScope.User.userName }
  </body>
测试结果
json
![](http://pic002.cnblogs.com/images/2012/267998/2012010700214742.png)
xml
![](http://pic002.cnblogs.com/images/2012/267998/2012010700215983.png)
jsp
![](http://pic002.cnblogs.com/images/2012/267998/2012010700221564.png)
