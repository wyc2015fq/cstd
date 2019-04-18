# RESTful Web Service(二):使用eclipse创建restful webservice 工程 - z69183787的专栏 - CSDN博客
2012年10月27日 12:24:12[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2421

使用环境:jdk1.5+eclispe3.4+myeclispe7
File=>New=>Web Service Project。
project name: restblog,选择REST(JAX-RS) 下一步。
勾选core,jaxb,client,json然后下一步。
之所以创建servlet是因为会请求一些物理不存在的URI，事实上就是这个servlet在处理这些resource。
jaxb(Java Architecture for XML Binding)可以将java class序列号成xml，请参考[http://java.sun.com/developer/technicalArticles/WebServices/jaxb/](http://java.sun.com/developer/technicalArticles/WebServices/jaxb/)
json就不多说了，这个包可以实现java class 转化成json。
点击finish完成工程创建。
这样创建的工程与通常的web工程的区别就是添加了servlet在web.xml，这里贴出servlet代码：后续的文章中也会提到他。
<servlet>
<display-name>JAX-RS REST Servlet</display-name>
<servlet-name>JAX-RS REST Servlet</servlet-name>
<servlet-class>
com.sun.jersey.spi.container.servlet.ServletContainer
</servlet-class>
<load-on-startup>1</load-on-startup>
</servlet>
<servlet-mapping>
<servlet-name>JAX-RS REST Servlet</servlet-name>
<url-pattern>/services/*</url-pattern>
</servlet-mapping>[](http://11011.net/software/vspaste)
在工程的classpath下会引用下列包：
![11](http://www.blogjava.net/images/blogjava_net/Hafeyang/WindowsLiveWriter/9d8ea9e6231f_10562/11_thumb.jpg)
这些包可以从[http://download.java.net/maven/2/com/sun/jersey/](http://download.java.net/maven/2/com/sun/jersey/) 下载(使用maven2)
首先创建一个简单的类Category
![12](http://www.blogjava.net/images/blogjava_net/Hafeyang/WindowsLiveWriter/9d8ea9e6231f_10562/12_thumb.jpg)
使用@XmlRootElement标注该类
点击![13](http://www.blogjava.net/images/blogjava_net/Hafeyang/WindowsLiveWriter/9d8ea9e6231f_10562/13_thumb.jpg) 图标创建webservice,选择restblog工程，Framework选择REST(JAX-RS)
![14](http://www.blogjava.net/images/blogjava_net/Hafeyang/WindowsLiveWriter/9d8ea9e6231f_10562/14_thumb.jpg)
Java class : CategoryService
URL path: 资源路径 填写category ,这样所有与category相关的资源都使用[http://yourpath/services/category](http://yourpath/services/category)访问
LIfecycle: Pre-request(JAX-RS default)每次请求时生成一个服务类实例(这里指CategoryService),singleton:单例，只存在一个实例
Consumes: 接受的contentType 包括application/xml ,application/json 通俗来说就是接受提交数据的格式。
Produces: 产生的数据格式选项同样为applicatin/xml,application/json 等。
注意这里的Consumes/Produces是指请求[http://yourpath/services/category](http://yourpath/services/category)的数据格式。
点击 Add 按钮添加服务方法，界面如下：
![16](http://www.blogjava.net/images/blogjava_net/Hafeyang/WindowsLiveWriter/9d8ea9e6231f_10562/16_thumb.jpg)
这是一个构造服务方法的界面，下面有代码预览。在添加参数的时候，选择Param Type选项的意义：
Context: 这是一个标识该参数为请求上下文。可以直接获得request的参数。
QueryParam:uri?之后的参数
PathParam:uri中的参数如：/category/{id}中的id就是PathParam
FormParam:使用post提交的参数。
CookieParam: Cookie参数。
HeaderParam: 请求的头部信息。
也许你已经注意到没有session参数，是的，所谓的"无状态stateless"多少就体现在这里，web服务不保存请求相关的信息。
综合上面的用法，下面的例子涵盖了常用的服务方法的写法，之中的处理过程暂时用硬编码。后面的文章会介绍HIbernate,Spring的应用。
package com.dawnpro.restblog;
import java.util.ArrayList;
import java.util.List;
import javax.ws.rs.Consumes;
import javax.ws.rs.DELETE;
import javax.ws.rs.DefaultValue;
import javax.ws.rs.FormParam;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.PUT;
import javax.ws.rs.Path;
import javax.ws.rs.PathParam;
import javax.ws.rs.Produces;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.UriInfo;
import org.codehaus.jettison.json.JSONException;
import org.codehaus.jettison.json.JSONObject;
@Path("category")
publicclass CategoryService {
    @GET
    @Produces( {"application/json","application/xml"})
/**
     * getCategories 产生json,xml两种数据格式，具体那种格式取决于contentType
*/
public List<Category> getCategories() {
        List<Category> result =new ArrayList<Category>();
        result.add(new Category(1,"第一个分类"));
        result.add(new Category(2,"第二个分类"));
return result;
    }
    @GET
    @Path("{id}")
public Category getCategory(@PathParam("id") int id) {
returnnew Category(id,"id为"+id+"的类别");
    }
    @GET
    @Path("json/{id}")
    @Produces( "application/json")
public JSONObject getCategoryJson(@PathParam("id") int id){//产生json
        JSONObject o =new JSONObject();
try {
            o.put("id", id);
            o.put("name", "id为"+id+"的category");
        } catch (JSONException e) {
            e.printStackTrace();
        }
return o;
    }
    @PUT
    @Path("add")
    @Produces("text/html")
    @Consumes( { "application/xml", "application/json" })
public String addCategory(Category category) {
        System.out.println("处理添加类别逻辑，接受的数据为id:"+category.getId()+",name:"+category.getName());
return "ok";
    }
    @POST
    @Path("addbyname")
public String addCategory(@FormParam("categoryname") @DefaultValue("[未命名]") String cateogryname) {
        System.out.println("处理添加类别逻辑，接受的数据为name:"+cateogryname);
return "添加类别"+cateogryname+"成功";
    }
    @POST
    @Produces("text/html")
    @Path("updatecategory")
    @Consumes( { "application/xml", "application/json" })
public String updateCategory(Category category) {
        System.out.println("处理更新类别逻辑，接受的数据为id:"+category.getId()+",name:"+category.getName());
return "ok";
    }
    @DELETE
    @Path("delete/{id}")
public String deleteCategory(@PathParam("id") int id) {
        System.out.println("处理删除类别逻辑，接受的数据为id:"+id);
return "ok";
    }
    @GET
    @Path("commonProcess")
public String commonProcess(@Context UriInfo info){//@Context 参数标识UriInfo
         StringBuilder buf = new StringBuilder();  
for (String param: info.getQueryParameters().keySet()) {  
             buf.append(param+" : "+info.getQueryParameters().get(param));  
             buf.append("\n");  
         }  
         System.out.println(buf.toString());
return "ok";
    }
}
你可以在CategoryService.java编辑中 右键菜单=>MyEclipse=>Add REST Method添加服务方法。
右键工程根目录restblog选择MyEclipse=>Test with RESTful WebServices Explorer 将会打开测试窗口。[](http://www.blogjava.net/images/blogjava_net/Hafeyang/WindowsLiveWriter/9d8ea9e6231f_10562/17.jpg)浏览中的地址为:http://localhost:8080/restblog/services/application.wadl 首先/services是由servlet拦截。如果在浏览器中浏览该地址，将得到一个xml文件。这个文件叫web service application description language。顾名思义他是一个服务描述文件。需要说明的是当调用方法的参数为Category类型，调用时传递的是xml同时contentType设置为application/xml这样jersey会将xml转化为Category类型的对象。如果转化出错则服务调用失败，会返回错误信息。
### 总结
本节主要介绍在Myeclipse中创建RESTful web service project 以及测试。到目前为止，只写了两个类就轻松的实现了RESTful Web service。参考资料：介绍rest:[http://java.sun.com/developer/technicalArticles/WebServices/restful/](http://java.sun.com/developer/technicalArticles/WebServices/restful/)介绍使用myeclipse rest 支持[http://www.myeclipseide.com/documentation/quickstarts/webservices_rest/](http://www.myeclipseide.com/documentation/quickstarts/webservices_rest/)RESTful Web Services Developer's Guide[http://docs.sun.com/app/docs/doc/820-4867?l=zh](http://docs.sun.com/app/docs/doc/820-4867?l=zh)netbeans的restful webservice 支持：[http://www.netbeans.org/kb/60/websvc/rest_zh_CN.html](http://www.netbeans.org/kb/60/websvc/rest_zh_CN.html)
posted on 2009-02-05 22:19 [衡锋](http://www.blogjava.net/Hafeyang/) 阅读(6044) [评论(3)](http://www.blogjava.net/Hafeyang/archive/2009/02/05/253458.html#Post)[编辑](http://www.blogjava.net/Hafeyang/admin/EditPosts.aspx?postid=253458)[收藏](http://www.blogjava.net/Hafeyang/AddToFavorite.aspx?id=253458) 所属分类:[restful
 webservice](http://www.blogjava.net/Hafeyang/category/37449.html)
![](http://www.blogjava.net/Hafeyang/aggbug/253458.html?webview=1)
