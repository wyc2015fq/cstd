# Spring 3 mvc中返回pdf,json,xml等不同的view - z69183787的专栏 - CSDN博客
2014年12月01日 22:42:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3989
个人分类：[SpringMvc](https://blog.csdn.net/z69183787/article/category/2176803)
   spring mvc就是好，特别是rest风格的话，一个 
org.springframework.web.servlet.view.ContentNegotiatingViewResolver就可以根据不同的场景返回不同的view了，比如返回json,xml,pdf等.马上看代码实现 
先是一个简单的pojo 
Java代码  ![收藏代码](http://image68.360doc.com/DownloadImg/2014/02/0822/38859034_1.png)
- package com.hmkcode.vo;  
- 
- import javax.xml.bind.annotation.XmlRootElement;  
- 
- @XmlRootElement(name = "person")  
- publicclass Person {  
- 
- privateint id;  
- private String name;  
- publicint getId() {  
- return id;  
-     }  
- publicvoid setId(int id) {  
- this.id = id;  
-     }  
- public String getName() {  
- return name;  
-     }  
- publicvoid setName(String name) {  
- this.name = name;  
-     }  
- public String toString(){  
- return"ID: "+id+" - Name: "+name;  
-     }  
- 
- }  
   注意使用@XmlRootElement标签之。 
然后是controller: 
Java代码  ![收藏代码](http://image68.360doc.com/DownloadImg/2014/02/0822/38859034_1.png)
- @Controller
- publicclass MyController {  
- 
- @RequestMapping("/view")  
- public Person getPerson(Model model){  
-         Person person = new Person();  
-         person.setId(1);  
-         person.setName("test");  
-         model.addAttribute("person",person);  
- return person;  
-     }  
  然后分别假设要导出为PDF的view，引入著名的itext了； 
Java代码  ![收藏代码](http://image68.360doc.com/DownloadImg/2014/02/0822/38859034_1.png)
- publicclass PdfView extends AbstractPdfView {  
- 
- protectedvoid buildPdfDocument(  
-         Map model,  
-         Document doc,  
-         PdfWriter writer,  
-         HttpServletRequest req,  
-         HttpServletResponse resp)  
- throws Exception {  
- 
- 
-         Person person = (Person) model.get("person");  
-         doc.add( new Paragraph(person.toString()));  
- 
-     }  
   这里演示简单往itext中写入一个输出而已； 
然后是web.xml: 
Java代码  ![收藏代码](http://image68.360doc.com/DownloadImg/2014/02/0822/38859034_1.png)
- <servlet>  
-         <servlet-name>springmvc</servlet-name>  
-         <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>  
-         <load-on-startup>1</load-on-startup>  
-     </servlet>  
- 
-     <servlet-mapping>  
-         <servlet-name>springmvc</servlet-name>  
-         <url-pattern>/rest/*</url-pattern>  
-     </servlet-mapping>  
  最核心的配置来了： 
Java代码  ![收藏代码](http://image68.360doc.com/DownloadImg/2014/02/0822/38859034_1.png)
-    <bean class="org.springframework.web.servlet.view.ContentNegotiatingViewResolver">  
- <property name="order" value="1" />  
- <property name="mediaTypes">  
-   <map>  
-      <entry key="json" value="application/json" />  
-      <entry key="xml" value="application/xml" />  
-      <entry key="pdf" value="application/pdf" />  
- 
-   </map>  
- </property>  
- 
- <property name="defaultViews">  
-   <list>  
-     <!-- JSON View -->  
-     <bean  
- class="org.springframework.web.servlet.view.json.MappingJacksonJsonView">  
-     </bean>  
-     <!--  XML view -->  
-       <bean class="org.springframework.web.servlet.view.xml.MarshallingView">  
-       <constructor-arg>  
-           <bean class="org.springframework.oxm.jaxb.Jaxb2Marshaller">  
-              <property name="classesToBeBound">  
-               <list>  
-                  <value>com.test.Person</value>  
-               </list>  
-              </property>  
-           </bean>  
-       </constructor-arg>  
-     </bean>  
-     <!--  PDF view -->  
-     <bean class="com.test.PdfView">  
- 
-       </bean>  
-    </list>  
- </property>  
- <property name="ignoreAcceptHeader" value="true" />  
- 
- t;/bean>  
   可以看到，其中使用的是： 
org.springframework.web.servlet.view.ContentNegotiatingViewResolver，对其pdf,xml.json进行分别的view设置，JSON用的是著名的jackyson去解析了； 
最后，通过rest风格的地址，比如http://localhost/view/view.json,返回json； 
http://localhost/view/view.pdf,则导出到pdf,http://localhost/view/view.xml， 
返回前端就xml了，真的十分方便
            
