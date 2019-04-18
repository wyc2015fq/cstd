# struts2 注解配置 - z69183787的专栏 - CSDN博客
2015年06月11日 15:09:56[OkidoGreen](https://me.csdn.net/z69183787)阅读数：694
个人分类：[Struts2](https://blog.csdn.net/z69183787/article/category/2175433)
1、web.xml 
Java代码  ![收藏代码](http://zhaozhi3758.iteye.com/images/icon_star.png)
- <!-- struts2 配置 -->  
-     <filter>  
-         <filter-name>struts2</filter-name>  
-         <filter-class>  
-             org.apache.struts2.dispatcher.ng.filter.StrutsPrepareAndExecuteFilter  
-         </filter-class>  
-         <init-param>  
-             <param-name>actionPackages</param-name>  
-             <param-value>com.yz.webapp.action</param-value>  
-         </init-param>  
-     </filter>  
-     <filter-mapping>  
-         <filter-name>struts2</filter-name>  
-         <url-pattern>/*</url-pattern>  
-     </filter-mapping>   
struts.properties 
Java代码  ![收藏代码](http://zhaozhi3758.iteye.com/images/icon_star.png)
- #修改时重新加载  
- struts.configuration.xml.reload = true
- #打印更多错误信息  
- struts.devMode = true
- #注解扫描的包结尾名  
- struts.convention.package.locators = action  
- #映射扩展名  
- struts.action.extension = html  
- #结果资源所在路径  
- #struts.convention.result.path = /WEB-INF/pages  
2、action 类名上加注解 
Java代码  ![收藏代码](http://zhaozhi3758.iteye.com/images/icon_star.png)
- @Namespace("/ssi")  
- @ParentPackage("json-default")  
- @Action(value = "admin", results = {  
- @Result(name = "success", location = "/WEB-INF/pages/admin.jsp"),  
- @Result(name = "json", type = "json", params = { "excludeProperties","adminMgr" }) })  
- publicclass AdminAction extends BaseAction{}  
多个Action 
Java代码  ![收藏代码](http://zhaozhi3758.iteye.com/images/icon_star.png)
- @Namespace("/msa")  
- @Result(name = "json", type = "json", params = { "excludeProperties",  
- ".*Manager,.*\\.authorities,.*\\.roles,.*\\.fileCon" })  
- @Actions(value = {  
- @Action(value = "foreignship", results = { @Result(name = "success", location = "foreignship/foreignship.jsp") }),  
- @Action(value = "foreignshipsee", results = { @Result(name = "success", location = "foreignship/foreignshipsee.jsp") }) })  
- publicclass TMsaForeignShipArchivesAction extends BaseAction{}  
在类方法上加注解 
Java代码  ![收藏代码](http://zhaozhi3758.iteye.com/images/icon_star.png)
- //@Action(value = "add", results = { @Result(name = "success", location = "/index.jsp") })
- @Action(value = "save")  
- public String save() {  
- try{  
-         adminMgr.insert("insertYz_admin", admin);  
-     }catch(Exception e){  
-         msg = e.toString();  
-         success = false;  
-     }  
- returnthis.SUCCESS;  
- }  
