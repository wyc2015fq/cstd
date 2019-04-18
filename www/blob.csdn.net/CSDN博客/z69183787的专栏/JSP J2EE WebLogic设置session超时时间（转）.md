# JSP J2EE WebLogic设置session超时时间（转） - z69183787的专栏 - CSDN博客
2014年03月13日 14:09:25[OkidoGreen](https://me.csdn.net/z69183787)阅读数：815
1 web.xml
设置web应用程序描述符web.xml里的<session-timeout>元素。这个值以分钟为
单位，并覆盖weblogic.xml中的timeoutsecs属性
<session-config>
<session-timeout>24</session-timeout>
</session-config>
此例表示session将在24分钟后过期
当<session-timeout>设置为－2，表示将使用在weblogic.xml中设置的
timeoutsecs这个属性值。
当<session-timeout>设置为－1，表示session将永不过期，而忽略在
weblogic.xml中设置的timeoutsecs属性值。
该属性值可以通过console控制台来设置
2 weblogic.xml
设置weblogic特有部署描述符weblogic.xml的<session-descriptor>元素的
timeoutsecs属性。这个值以秒为单位
<session-descriptor>
<session-param>
<param-name>timeoutsecs</param-name>
<param-value>2600</param-value>
</session-param>
</session-descriptor>
默认值是2600秒
3,jsp中控制
session.setmaxinactiveinterval(7200);
session是默认对象,可以直接引用,单位秒s
4,servlet中控制
httpsession session = request.getsession();
session.setmaxinactiveinterval(7200);
单位秒s  
