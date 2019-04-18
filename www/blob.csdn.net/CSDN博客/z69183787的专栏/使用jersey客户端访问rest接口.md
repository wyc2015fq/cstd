# 使用jersey客户端访问rest接口 - z69183787的专栏 - CSDN博客
2013年06月27日 16:27:49[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3528
```java
public static void main(String[] args) {
		// TODO Auto-generated method stub
		 		Client c = Client.create();  
		         WebResource r=c.resource("http://10.1.41.252:8080/workflowController/service/todo/stat");
		         MultivaluedMap formData = new MultivaluedMapImpl();
		         formData.add("data", "ST/G001000001612549");
		         String response = r.type("application/x-www-form-urlencoded")
		                                              .post(String.class, formData);
		         System.out.println(response.toString());
	}
```
