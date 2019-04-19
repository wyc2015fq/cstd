# 基于Gson库的json/bean解析工具 - 零度的博客专栏 - CSDN博客
2016年04月25日 10:38:12[零度anngle](https://me.csdn.net/zmx729618)阅读数：1190
```
package zmx.gson.test;
import java.io.Serializable;
import java.util.Date;
public class UserInfo implements Serializable{
	private static final long serialVersionUID = -7371753338546349542L;
	
    private String id;  
    
    private String username; 
    
    private String face;
    
    private Date birthday;
    
    
	public String getId() {
		return id;
	}
	public void setId(String id) {
		this.id = id;
	}
	public String getUsername() {
		return username;
	}
	public void setUsername(String username) {
		this.username = username;
	}
	public String getFace() {
		return face;
	}
	public void setFace(String face) {
		this.face = face;
	}
	public Date getBirthday() {
		return birthday;
	}
	public void setBirthday(Date birthday) {
		this.birthday = birthday;
	}
    
    
    
	
	
	
	
}
```
```
package zmx.gson.test;
import java.lang.reflect.Type;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;
import java.util.Map;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import com.google.gson.JsonDeserializationContext;
import com.google.gson.JsonDeserializer;
import com.google.gson.JsonElement;
import com.google.gson.JsonParseException;
import com.google.gson.JsonPrimitive;
import com.google.gson.JsonSerializationContext;
import com.google.gson.JsonSerializer;
/**
 * 
 * @author zhangwenchao
 *
 */
public class GsonUtil {
	
	private static Gson gson = new Gson();
  
    private GsonUtil() {  
      
    } 
    
    /**
     * 将对象转换成json格式 
     * @param obj
     * @return String
     */
    public static String objectToJsonStr(Object obj){
    	
    	String jsonStr = null;
    	jsonStr = gson.toJson(obj);
		return jsonStr;
    	
    }
    
    
    /** 
     * 将对象转换成json格式(并自定义日期格式) 
     *  
     * @param obj 
     * @return String
     */  
    public static String objectToJsonDateSerializer(Object obj,  
            final String dateformat) {  
        String jsonStr = null;  
        gson = new GsonBuilder()  
                .registerTypeHierarchyAdapter(Date.class,  
                        new JsonSerializer<Date>() {
                	        @Override
                            public JsonElement serialize(Date src,Type typeOfSrc,  JsonSerializationContext context) {  
                                SimpleDateFormat format = new SimpleDateFormat(dateformat);  
                                return new JsonPrimitive(format.format(src));  
                            }}
                ).setDateFormat(dateformat).create();  
        if (gson != null) {  
            jsonStr = gson.toJson(obj);  
        }  
        return jsonStr;  
    }
    
    
    /** 
     * 将json格式转换成list对象  
     * @param jsonStr 
     * @return 
     */  
    public static List<?> jsonToList(String jsonStr) {  
        List<?> objList = null;  
        if (gson != null) {  
            java.lang.reflect.Type type = new com.google.gson.reflect.TypeToken<List<?>>() {}.getType();  
            objList = gson.fromJson(jsonStr, type);  
        }  
        return objList;  
    }
    
    
    /** 
     * 将json格式转换成list对象，并准确指定类型 
     * @param jsonStr 
     * @param type 
     * @return 
     */  
    public static List<?> jsonToList(String jsonStr, java.lang.reflect.Type type) {  
        List<?> objList = null;  
        if (gson != null) {  
            objList = gson.fromJson(jsonStr, type);  
        }  
        return objList;  
    }
    
    
 /** 
  * 将json格式转换成map对象 
  *  
  * @param jsonStr 
  * @return 
  */  
  public static Map<?, ?> jsonToMap(String jsonStr) {  
        Map<?, ?> objMap = null;  
        if (gson != null) {  
            java.lang.reflect.Type type = new com.google.gson.reflect.TypeToken<Map<?, ?>>() {  
            }.getType();  
            objMap = gson.fromJson(jsonStr, type);  
        }  
        return objMap;  
  }  
  
    /** 
     * 将json转换成bean对象 
     *  
     * @param jsonStr 
     * @return 
     */  
    public static Object jsonToBean(String jsonStr, Class<?> cl) {  
        Object obj = null;  
        if (gson != null) {  
            obj = gson.fromJson(jsonStr, cl);  
        }  
        return obj;  
    }  
  
    /** 
     * 将json转换成bean对象 
     *  
     * @param jsonStr 
     * @param cl 
     * @return 
     */  
    @SuppressWarnings("unchecked")  
    public static <T> T jsonToBeanDateSerializer(String jsonStr, Class<T> cl,  
            final String pattern) {  
        Object obj = null;  
        gson = new GsonBuilder()  
                .registerTypeAdapter(Date.class, new JsonDeserializer<Date>() {
                	@Override
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
  
    /** 
     * 根据 json串中的key获取 对应的对象 
     * @param jsonStr 
     * @param key 
     * @return 
     */  
    public static Object getJsonValue(String jsonStr, String key) {  
        Object rulsObj = null;  
        Map<?, ?> rulsMap = jsonToMap(jsonStr);  
        if (rulsMap != null && rulsMap.size() > 0) {  
            rulsObj = rulsMap.get(key);  
        }  
        return rulsObj;  
    }  
    public static void main(String[] args) {
    	
    	UserInfo userInfo =  new UserInfo();
    	userInfo.setId("10001");
    	userInfo.setUsername("张三");
    	userInfo.setFace("common");
    	userInfo.setBirthday(new Date());
    	String  json =  GsonUtil.objectToJsonDateSerializer(userInfo, "yyyy-MM-dd");
		System.out.println(json);
		
		//String jsonStr = "[{\"id\":\"10001\",\"username\":\"张三\",\"face\":\"common\",\"birthday\":\"2016-04-25\"},{\"id\":\"10002\",\"username\":\"lisi\",\"face\":\"common\",\"birthday\":\"2016-04-25\"}]";
		String jsonStr = "[{id:'10001',username:'张三',face:'common',birthday:'2016-04-25'},{id:\"10002\",username:\"lisi\",face:\"common\",birthday:\"2016-04-25\"}]";
		List<UserInfo> ll =  GsonUtil.jsonToBeanDateSerializer(jsonStr, List.class, "yyyy-MM-dd");
		
		System.out.println(ll);
		
		
	}
    
    
    
    
    
	
	
	
}
```
输出：
```
{"id":"10001","username":"张三","face":"common","birthday":"2016-04-25"}
[{id=10001, username=张三, face=common, birthday=2016-04-25}, {id=10002, username=lisi, face=common, birthday=2016-04-25}]
```
