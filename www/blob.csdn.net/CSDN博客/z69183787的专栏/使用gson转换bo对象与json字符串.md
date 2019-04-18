# 使用gson转换bo对象与json字符串 - z69183787的专栏 - CSDN博客
2012年10月22日 10:37:02[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2471
所需jar包 gson-2.2.1.jar .
sample1： json字符串转换bo对象
工具类
```java
import java.io.IOException;
import java.io.StringReader;
import java.lang.reflect.Type;
import java.util.Iterator;
import java.util.LinkedList;
import com.google.gson.Gson;
import com.google.gson.stream.JsonReader;
import com.google.gson.reflect.TypeToken;
public class JsonUtils {    
    public void parseJson(String jsonData){    
        // 解析JSON数据，首先要创建一个JsonReader对象    
        JsonReader reader = new JsonReader(new StringReader(jsonData));    
        try {    
            // 开始解析JSON数组    
            reader.beginArray();    
            // 循环读取JSON中的数组    
            while(reader.hasNext()){    
                // 开始解析JSON数组里的对象    
                reader.beginObject();    
                // 循环读取JSON数组中的对象    
                while(reader.hasNext()){    
                    String tagName = reader.nextName();    
                    if(tagName.equals("name")){    
                        System.out.println("name---->" + reader.nextString());    
                    }    
                    else if(tagName.equals("age")){    
                        System.out.println("age---->" + reader.nextInt());    
                    }    
                }    
                reader.endObject();    
            }    
            reader.endArray();    
        } catch (IOException e) {    
            e.printStackTrace();    
        }       
    }    
    public void parseUserFromJson(String jsonData){    
        // 解析json对象并赋值给User对象    
        Gson gson = new Gson();    
        User user = gson.fromJson(jsonData, User.class);    
        System.out.println("name---->" + user.getName());    
        System.out.println("age---->" + user.getAge());    
    }    
    // 解析JSON数组并赋值给User对象    
    public void parseUserArrayFromJson(String jsonData){    
        // 取得该类型的实体    
        Type listType = new TypeToken<LinkedList<User>>(){}.getType();    
        Gson gson = new Gson();    
        LinkedList<User> users = gson.fromJson(jsonData, listType);    
        for(Iterator<User> iterator = users.iterator(); iterator.hasNext();){    
            User user = (User)iterator.next();    
            System.out.println("name---->" + user.getName());    
            System.out.println("age---->" + user.getAge());    
        }    
    }    
}
```
bo类
```java
public class User {
	private String name;
	private String age;
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getAge() {
		return age;
	}
	public void setAge(String age) {
		this.age = age;
	}
	
	
}
```
main：
```java
public class Test {
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		JsonUtils j = new JsonUtils();
		String jsondata = "[{\"name\":\"kevin\",\"age\":25},{\"name\":\"cissy\",\"age\":24}]";
		String jsondata2 = "{\"name\":\"kevin\",\"age\":25}";
		//System.out.println("----------------------------1");
		//j.parseJson(jsondata);
		//System.out.println("----------------------------2");
		//j.parseUserFromJson(jsondata2);
		//System.out.println("----------------------------3");
		j.parseUserArrayFromJson(jsondata);
	}
}
```
output：
name---->kevin
age---->25
name---->cissy
age---->24
sample2：bo转json
```java
package com.wonders.stpt.util;
import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.io.Writer;
import javax.servlet.http.HttpServletResponse;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
public class ActionWriter {
	private HttpServletResponse response;
	private Gson gsonAnnotation = new GsonBuilder().excludeFieldsWithoutExposeAnnotation().create();
	private Gson gson = new Gson();
	
	public ActionWriter(HttpServletResponse response){
		this.response = response;
	}
	
	/**JSON输出(ajax)
	 * @param response
	 * @param obj
	 */
	public void writeJsonWithAnnotation(Object obj){
		if(response==null) return;
		Writer w = null;
		
		try {
			response.setContentType("text/html");
			response.setCharacterEncoding("UTF-8");
			w = response.getWriter();
			
			String ret = gsonAnnotation.toJson(obj);	
			w.write(ret);		
			w.flush();
		} catch (IOException e) {
			e.printStackTrace();
		}finally{
			if(w!=null){
				try {
					w.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
	}
	
	public void writeJson(Object obj){
		if(response==null) return;
		Writer w = null;
		try {
			response.setContentType("text/html");
			response.setCharacterEncoding("UTF-8");	
			w = response.getWriter();
			String ret = gson.toJson(obj);	
			w.write(ret);		
			w.flush();
		} catch (IOException e) {
			e.printStackTrace();
		}finally{
			if(w!=null){
				try {
					w.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
	}
	
	
	/**字符串输出(ajax)
	 * @param response
	 * @param str
	 */
	public void writeAjax(String str){
		if(response==null) return;
		Writer w = null;
		
		try {
			response.setContentType("text/html");
			response.setCharacterEncoding("UTF-8");
			
			w = response.getWriter();
			
			w.write(str);
			
			w.flush();
		} catch (IOException e) {
			e.printStackTrace();
		}finally{
			if(w!=null){
				try {
					w.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
	}
	
	public void writeJpeg(BufferedInputStream bis){
		if(response==null) return;
		OutputStream output = null;
		
		try {
			response.setContentType("image/jpeg");
	        response.setHeader("Pragma","No-cache");
	        response.setHeader("Cache-Control","no-cache");
	        response.setDateHeader("Expires", 0);
			
	        output = response.getOutputStream();
	        byte[] bytes = new byte[100];
	        int len;
	        while ((len = bis.read(bytes)) > 0) {
	        	output.write(bytes, 0, len);
	        }
	        bis.close();
	        output.flush();
	        output.close();
	        
		} catch (IOException e) {
			e.printStackTrace();
		}finally{
			if(output!=null){
				try {
					output.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
		
		
	}
	
}
```
调用writeJson 返回结果为：
[
{"createTime":"2012-06-04","title":"上海地铁第四届公共安全宣传周总结","picUrl":"","memo":"","SJ_ID":"25","moreUrl":"/node281/node25/index.htm","seq":"","copyTitle":"上海地铁第四届公共安全宣传周总结","partId":"0","copyMemo":"","url":"/node281/node25/201206/con1024263"},
{"createTime":"2011-04-29","title":"第二届“上海地铁公共安全宣传周”活动回顾","picUrl":"","memo":"","SJ_ID":"25","moreUrl":"/node281/node25/index.htm","seq":"","copyTitle":"第二届“上海地铁公共安全宣传周”活动回顾","partId":"0","copyMemo":"","url":"/node281/node25/201104/con1011596"}
]
