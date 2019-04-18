# Tomcat服务OTA发布J2ME应用(eclipse开发环境) - 专注于数据挖掘算法研究和应用 - CSDN博客





2010年10月27日 12:20:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2562标签：[tomcat																[j2me																[eclipse																[web服务																[java																[网络](https://so.csdn.net/so/search/s.do?q=网络&t=blog)
个人分类：[Linux																[Windows](https://blog.csdn.net/fjssharpsword/article/category/6480443)](https://blog.csdn.net/fjssharpsword/article/category/6480442)





a.部署Tomcat和wtk；


b.在tomcat下conf/web.xml增加：
       <mime-mapping>  
        <extension>jad</extension>  
        <mime-type>text/vnd.sun.J2ME.app-descriptor</mime-type>  
       </mime-mapping>
       <mime-mapping>  
        <extension>jar</extension>  
        <mime-type>application/java-archive</mime-type>  
       </mime-mapping>


c.eclipse开发j2me程序后，将deployed文件夹下的demo.jad和demo.jar部署到tomcat：
       方法一：新增web服务，在webcontent下创建j2medemo文件夹，
               然后将demo.jad和demo.jar拷贝进去，最后部署web服务到tomcat；
       方法二：直接存放在webapps/ROOT目录下，在该目录下创建j2medemo文件夹，
               将demo.jad和demo.jar拷贝进去即可；


 d.发布down.WML页面路径作为WAP下载页面发布，也放在j2medemo目录下，
       <?xml version="1.0"?> 
       <!DOCTYPE wml PUBLIC "-//WAPFORUM//DTD WML 1.1//EN"
           "[http://www.wapforum.org/DTD/wml_1.1.xml](http://www.wapforum.org/DTD/wml_1.1.xml)">
　　   <wml> 
　　     <card id="no1" title="Snake"> 
　　     <a href="[http://127.0.0.1:8080/j2medemo/demo.jad">demo.jad</a](http://127.0.0.1:8080/j2medemo/demo.jad)> 
　　     </card> 
　　   </wml> 


 e. 修改j2medemo目录下demo.jad文件中(txt打开编辑)demo.jar文件的路径，
         MIDlet-Jar-URL: [http://127.0.0.1:8080/j2medemo/demo.jar](http://127.0.0.1:8080/j2medemo/demo.jar)； 


  f.启动tomcat，启动WTK的OTAProvisioning，点击屏幕右下角的apps，点击installapplication，
       输入地址：[http://127.0.0.1:8080/j2medemo/down.WML](http://127.0.0.1:8080/j2medemo/down.WML)，选择demo.jad下载即可安装并运行；


g.用支持java手机，开通数据网络服务，输入部署地址访问即可ota下载；](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=web服务&t=blog)](https://so.csdn.net/so/search/s.do?q=eclipse&t=blog)](https://so.csdn.net/so/search/s.do?q=j2me&t=blog)](https://so.csdn.net/so/search/s.do?q=tomcat&t=blog)




