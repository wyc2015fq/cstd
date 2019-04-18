# velocity的toolbox使用方法(含与struts2的集成) - z69183787的专栏 - CSDN博客
2014年05月21日 19:19:01[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6937
1.velocity中的toolbox单独使用(没有与struts2集成)
　1.1.在web项目的web.xml文件中加入以下代码(velocity的jar包配置之类的就不再阐述，本博文默认读者已经搭建好velocity的开发环境),即配置解析velocity的toolbox的servlet
　　<servlet>
　　<servlet-name>velocityView</servlet-name>
　　<servlet-class>org.apache.velocity.tools.view.servlet.VelocityViewServlet</servlet-class>
　　<init-param>
　　<param-name>org.apache.velocity.toolbox</param-name>
　　<param-value>/WEB-INF/toolbox.xml</param-value>
　　</init-param>
　　</servlet>
　　<servlet-mapping>
　　<servlet-name>velocityView</servlet-name>
　　<url-pattern>*.vm</url-pattern>
　　</servlet-mapping>
　1.2.在/WEB-INF/目录下建toolbox.xml文件即toolbox的配置文件
　1.3.toolbox配置文件配置如下
<?xml version="1.0" encoding="UTF-8"?> 
<toolbox>
<tool> 
<key>date</key>
<scope>application</scope> 
<class>org.apache.velocity.tools.generic.DateTool</class> 
<parameter name="format" value="yyyy-M-d"/> 
</tool> 
<tool>
<key>sys1</key>
<scope>application</scope>
<class>com.i2.mailreader.Test</class>
</tool>
<data type="string"> 
<key>sys2</key> 
<scope>request</scope> 
<value>test</value> 
</data> 
</toolbox>
期中toolbox是跟节点，tool节点是每个工具的根节点，每个工具类都有一个key，也就是再vm文件中需要引用的变量，scope是配置这个tool的范围，class就是我们需要制定的类的全路径了，上面的例子中key为date的tool是velocity中的一个工具类，我们可以拿来直接用，其parameter是需要传递给DateTool的参数；key为sys1的tool是我自定义的类com.i2.mailreader.Test,Test类的内容下面会提供出来（很简单）；key为sys2的data是用来配置一些常量，type指定了data的类型，scope是其作用范围，value就是该常量的值了。
com.i2.mailreader.Test类的内容
package com.i2.mailreader;
public class Test {
　　public String getColor(){
　　　　System.out.println("print out Red success !");
　　　　return "red";
　　}
}
1.4.前台vm文件的内容如下
${sys1.getColor()}
<br/>
$date.get('yyyy-M-d H:m:s')
<br/>
$sys2
1.5.启动服务直接访问vm文件
[http://localhost:9001/mailreader/mobile/login5.vm](http://localhost:9001/mailreader/mobile/login5.vm)
即可显示如下结果
red 
2013-7-19 14:46:52 
test
2.velocity与struts2集成时的toolbox使用
　　2.1.在web.xml中只要有关于struts2的配置即可，不需要额外配置velocity的toolbox，struts2的在web.xml中的配置如下
<filter>
<filter-name>Struts2</filter-name>
<filter-class>
org.apache.struts2.dispatcher.ng.filter.StrutsPrepareAndExecuteFilter
</filter-class>
</filter>
<filter-mapping>
<filter-name>Struts2</filter-name>
<url-pattern>*.do</url-pattern>
</filter-mapping>
2.2.在struts.xml中加入以下配置
<constant name="struts.velocity.toolboxlocation" value="/WEB-INF/toolbox.xml"/>
2.3.在/WEB-INF/目录下同样需要有toolbox.xml配置文件；toolbox.xml的内容也同上面toolbox.xml的配置；com.i2.mailreader.Test的内容也同上；
2.4.启动服务，访问url如下
[http://localhost:9001/mailreader/velocity/login5.do](http://localhost:9001/mailreader/velocity/login5.do)
页面显示结果如下
red 
2013-7-19 15:2:49 
test
THE  END !
