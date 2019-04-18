# jsonp jquery jersey 实现跨域访问 - z69183787的专栏 - CSDN博客
2014年02月14日 10:39:38[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2681
服务器端：
```java
@GET
	@Path("addTaskRest")
	@Consumes(MediaType.APPLICATION_FORM_URLENCODED)
	@Produces(MediaType.TEXT_PLAIN)
	public String addTaskRest(@QueryParam("data") String data,@QueryParam("jsonpcallback") String jsonpcallback){
		System.out.println("data="+ data);
		System.out.println("jsonpcallback="+ jsonpcallback);
		String result = "";
		result = this.controller.addTask(data);
		return jsonpcallback+"( {\"id\":\""+result+"\"})";
	}
```
客户端：
```java
$.ajax({
		// url:	"/portal/constructionNotice/showLineInfo.action?random="+Math.random(),
		url :	"http://10.1.14.20:8088/workflowController/service/todo/addTaskRest",
		type:	"get",
		jsonp:'jsonpcallback',
		data:	{
					"data":"{\"app\": \"zhoushuntest\",\"type\": 0,"
						+ "\"occurTime\": \"2013-11-14 11:22:02\",\"title\": \"-------流程标题-------\","
						+ "\"loginName\": \"ST/G01008000311\",\"status\": 0,\"removed\": 0,"
						+ " \"typename\": \"流程名称11\","
						+ "\"url\": \"流程名称11\","
						+ "\"pname\": \"主流程名称\",\"pincident\": 1,"
						+ "\"cname\": \"子流程实例号\",\"cincident\": 1,"
						+ "\"stepName\": \"当前步骤\","
						+ "\"initiator\": \"ST/G01008000311\"}"
				},
		dataType : "jsonp",//跨域必须用jsonp   
	   cache: 	false,
	   error: function(XMLHttpRequest, textStatus, errorThrown) {
           alert(XMLHttpRequest.status);
           alert(XMLHttpRequest.readyState);
           alert(textStatus);
       },
	 success: 	function(obj){	
					alert(obj.id);
				}
	});
```
