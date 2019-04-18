# 通过Jersey客户端API调用REST风格的Web服务 - z69183787的专栏 - CSDN博客
2013年07月15日 13:04:21[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1680
Jersey 客户端 API 基础 
要开始使用 Jersey 客户端 API，你首先需要创建一个 com.sun.jersey .api.client.Client 类的实例。下面是最简单的方法：
import com.sun.jersey .api.client.Client;
Client client = Client.create();
Client 类是创建一个 RESTful Web Service 客户端的主要配置点。你可以使用它来配置不同的客户端属性和功能，并且指出使用哪个资源提供者。创建一个 Client 类的实例是一个比较昂贵的操作，所以尽量避免创建一些不需要的客户端实例。比较好的方式是尽可能地复用已经存在的实例。
    当你创建完一个 Client 类的实例后，你可以开始使用它。无论如何，在发出请求前，你需要创建一个 Web Resource 对象来封装客户端所需要的 Web 资源。
Web 资源创建了一个 WebResponse 对象：
import com.sun.jersey .api.client.WebResource;
Web Resource webResource = c.resource("[http://example.com/base](http://example.com/base)");
通过使用 WebResource 对象来创建要发送到 Web 资源的请求，以及处理从 Web 资源返回的响应。例如，你可以使用 WebResource 对象来发送 HTTP GET、PUT、POST 以及 DELETE 请求。
    GET 请求：使用 WebResource 类的 get() 方法来提交一个 HTTP GET请求到 Web 资源：
String s = webResource.get(String.class);
    这表示如果 WebResource 对象的 URL 是 [http://example.com/base](http://example.com/base)，那么一个 HTTP
 GET 请求将会发送到地址为 [http://example.com/base](http://example.com/base) 的资源。
String s = webResource.get(String.class);
   你还可以指定 get() 请求时的查询参数。例如，下面的代码在 get() 请求中指定了两个查询参数：
MultivaluedMap queryParams = new MultivaluedMapImpl();
queryParams.add("param1", "val1");
queryParams.add("param2", "val2");
String s = webResouce.queryParams(queryParams).get(String.class);
  你还可以指定响应所能接受的 MIME 类型。例如，下面的代码指定了响应的 MIME 类型只能为文本：
String s = webResource.accept("text/plain").get(String.class);
你还可以获取对应请求的 HTTP 状态码，例如下面这个例子展示获取一个请求所返回的文本实体与状态码：   
ClientResponse response = webResource.accept("text/plain")
                                     .get(ClientResponse.class);
int status = response.getStatus();
String textEntity = response.getEntity(String.class);     
ClientResponse 对象代表了一个客户端收到的 HTTP 响应。   
PUT 请求 ：使用 WebResource 类的 put() 方法来提交一个 HTTP PUT 请求到 Web 资源。例如下面的代码展示了请求发送一个文本实体 foo:bar 到指定的 Web 资源:
ClientResponse response = webResource.type("text/plain")
                                     .put(ClientResponse.class, "foo:bar");      
同样，你也可以在使用 put() 方法发送请求时指定查询参数，方法与使用 get() 方法时指定查询参数一样。在下面的例子中，把在之前 get() 方法示例中使用过的两个同样的查询参数指定到了一个 put() 请求中：
MultivaluedMap queryParams = new MultivaluedMapImpl();
queryParams.add("param1", "val1");
queryParams.add("param2", "val2");
ClientResponse response = webResource.queryParams(queryParams)
                                     .put(ClientResponse.class, "foo:bar");
 POST 请求 ：一个 POST 请求相当于一个 GET 请求和一个 PUT 请求的综合，也就意味着，你可以使用 POST 请求来发送一个实体到指定的 Web 资源并且接收另一个实体。使用 WebResource 类的 post() 方法来发送一个 HTTP POST 请求到指定的 Web 资源。下面的例子展示了发送一个带有查询参数以及进行了 URL 编码的表单数据的 POST 请求：
MultivaluedMap formData = new MultivaluedMapImpl();
formData.add("name1", "val1");
formData.add("name2", "val2");
ClientResponse response = webResource.type("application/x-www-form-urlencoded")
                                     .post(ClientResponse.class, formData);
 DELETE 请求：使用 Web Resource 类的 delete() 方法来发送珍上 HTTP DELETE 请求到指定的 Web 资源。例如，下面的例子展示删除一个 URI 为 [http://example.com/base/user/123](http://example.com/base/user/123) 资源：
ClientResponse response = webResource.path("user/123")
                                     .delete(ClientResponse.class);
  另外，Web Resource.path() 方法可以在所有 HTTP 请求中使用，它可以让你给要请求的 Web 资源指定一个额外的路径。另一个 WebResouce 类的方法 header() 可以给你的请求添加 HTTP 头部信息。
另外如果表单提交的话，需要new Form来作为参数提交。
 一个基于 Jersey 客户端的示例
package com.trend.vmware.client;
import javax.ws.rs.core.MediaType;
import com.sun.jersey.api.client.Client;
import com.sun.jersey.api.client.ClientResponse;
import com.sun.jersey.api.client.WebResource;
import com.sun.jersey.api.representation.Form;
import com.trend.vmware.model.TaskResponse;
public class VMOVFRestClient {
 public static void main(String[] args) {
  String restURI = "[http://localhost:8080/VmService](http://localhost:8080/VmService)";
  Client c = Client.create();
  WebResource webResource = c.resource(restURI+"/rest/vsphere/template");
  exportOVF(webResource,"AutoLab/Test_VM3/action/exportOVF");
   // importOVF(webResource,"10.64.3.188/action/importOVF");
 }
 public static void exportOVF(WebResource r, String pathParam){
  postForm(r.path(pathParam),"datastore1","D:/ovf");
  System.out.println(" -------------------------"+r.path(pathParam));
 }
  public static void importOVF(WebResource r, String pathParam){
   postFormForImportOVF(r.path(pathParam),"E:/ovf","datastore1","TEST_VM");
   System.out.println(" -------------------------"+r.path(pathParam));
  }
 //TO POST FORM
 public static void postForm(WebResource r, String dataStoreName,String localPath) {
  Form form = new Form();
  form.add("dataStoreName", dataStoreName);
  form.add("localPath",localPath);
  ClientResponse response = r.type(MediaType.APPLICATION_FORM_URLENCODED)
           .post(ClientResponse.class, form);
  //TODO handler response.getEntity
  System.out.println(response.getEntity(String.class));
 }
 //TO POST FORM FOR importOVF
 public static void postFormForImportOVF(WebResource r, String localPath,String dataStoreName,String newVMName) {
  Form form = new Form();
  form.add("localPath",localPath);
  form.add("dataStoreName", dataStoreName);
  form.add("newVMName", newVMName);
  ClientResponse response = r.type(MediaType.APPLICATION_FORM_URLENCODED)
           .post(ClientResponse.class, form);
  //TODO handler response.getEntity
  TaskResponse res = response.getEntity(TaskResponse.class);
  System.out.println("res ---"+res.getMessage()+"- "+res.getTaskStatus());
 }
}
