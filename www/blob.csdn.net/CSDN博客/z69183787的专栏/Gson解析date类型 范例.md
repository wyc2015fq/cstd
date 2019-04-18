# Gson解析date类型 范例 - z69183787的专栏 - CSDN博客
2013年10月25日 09:58:50[OkidoGreen](https://me.csdn.net/z69183787)阅读数：10764
```java
/**
 * 
 */
package zs;
import java.lang.reflect.Type;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.LinkedList;
import java.util.List;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.reflect.TypeToken;
/** 
 * @ClassName: GsonTest 
 * @Description: TODO(这里用一句话描述这个类的作用) 
 * @author zhoushun 
 * @date 2013-10-25 上午9:51:05 
 *  
 */
public class GsonTest {
	/** 
	 * @Title: main 
	 * @Description: TODO(这里用一句话描述这个方法的作用) 
	 * @param @param args    设定文件 
	 * @return void    返回类型 
	 * @throws 
	 */
	public static void main(String[] args) {
		List<User> list = new ArrayList<User>();
		Gson gson = new GsonBuilder().setDateFormat("yyyy-MM-dd HH:mm:ss").create();  
		User u = new User();
		u.name = "12321";
		u.date = new Date();
		User u1 = new User();
		u1.name = "12322";
		u1.date = new Date();
		User u2 = new User();
		u2.name = "12323";
		u2.date = new Date();
		list.add(u);list.add(u1);list.add(u2);
		System.out.println(gson.toJson(list));
		Type listType = new TypeToken<List<User>>(){}.getType();      
  
		List<User> users = gson.fromJson(gson.toJson(list), listType);      
        for(User m : users){
        	System.out.println(new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(m.date));
        }
		 String json = "{\"name\":\"12345\",\"date\":\"2013-01-04 10:30:30\"}";
		//Gson gson = new Gson();
		
		System.out.println(gson.toJson(u));
		User uu = gson.fromJson(json, User.class);
		System.out.println(new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(uu.date));
	}
}
```
