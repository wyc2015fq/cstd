# Gson转换bo对象并解析date类型 - z69183787的专栏 - CSDN博客
2013年10月24日 19:18:37[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3896
```java
import java.lang.reflect.Type;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.*;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonDeserializer;
import com.google.gson.JsonElement;
import com.google.gson.JsonParseException;
import com.google.gson.JsonSerializer;
import com.google.gson.reflect.TypeToken;
import com.google.gson.JsonSerializationContext;
import com.google.gson.JsonDeserializationContext;
import com.google.gson.JsonPrimitive;
public class Test {
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		Map<String,String> map = new HashMap<String,String>();
		map.put("123", "asas");
		map.put("vgvg", "rtretr");
		List<Map<String,String>> list = new ArrayList<Map<String,String>>();
		list.add(map);
		Gson gson = new Gson();
		String r = gson.toJson(list);
		
		Type type =  new TypeToken<List<Map<String,String>>>(){}.getType();
		
		List<Map<String,String>> l = gson.fromJson(r, type);
		System.out.println(l.get(0).get("123"));
		
		
		User u = new User();
		u.name = "zs";
		u.date = new Date();
		System.out.println(gson.toJson(u));
		System.out.println(objectToJsonDateSerializer(u,"yyyy-MM-dd"));
		
		System.out.println(((User)jsonToBeanDateSerializer(objectToJsonDateSerializer(u,"yyyy-MM-dd"),User.class,"yyyy-MM-dd")).name);
	}
	
	 /** 
     * 将对象转换成json格式(并自定义日期格式) 
     *  
     * @param ts 
     * @return 
     */  
    public static String objectToJsonDateSerializer(Object ts,  
            final String dateformat) {  
        String jsonStr = null;  
        Gson gson = new GsonBuilder()  
                .registerTypeHierarchyAdapter(Date.class,  
                        new JsonSerializer<Date>() {  
                            public JsonElement serialize(Date src,  
                                    Type typeOfSrc,  
                                    JsonSerializationContext context) {  
                                SimpleDateFormat format = new SimpleDateFormat(  
                                        dateformat);  
                                return new JsonPrimitive(format.format(src));  
                            }  
                        }).setDateFormat(dateformat).create();  
        if (gson != null) {  
            jsonStr = gson.toJson(ts);  
        }  
        return jsonStr;  
    }  
    
	 @SuppressWarnings("unchecked")  
	    public static <T> T jsonToBeanDateSerializer(String jsonStr, Class<T> cl,  
	            final String pattern) {  
	        Object obj = null;  
	        Gson gson = new GsonBuilder()  
	                .registerTypeAdapter(Date.class, new JsonDeserializer<Date>() {  
	                    public Date deserialize(JsonElement json, Type typeOfT,  
	                            JsonDeserializationContext context)  
	                            throws JsonParseException {  
	                        SimpleDateFormat format = new SimpleDateFormat(pattern);  
	                        String dateStr = json.getAsString();  
	                        try {  
	                            return format.parse(dateStr);  
	                        } catch (ParseException e) {  
	                            e.printStackTrace();  
	                        }  
	                        return null;  
	                    }  
	                }).setDateFormat(pattern).create();  
	        if (gson != null) {  
	            obj = gson.fromJson(jsonStr, cl);  
	        }  
	        return (T) obj;  
	    }  
}
```
