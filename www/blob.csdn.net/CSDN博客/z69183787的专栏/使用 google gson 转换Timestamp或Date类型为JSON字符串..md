# 使用 google gson 转换Timestamp或Date类型为JSON字符串. - z69183787的专栏 - CSDN博客
2013年10月25日 09:59:37[OkidoGreen](https://me.csdn.net/z69183787)阅读数：10013
创建类型适配类:
Timestamp类型适配类代码  ![收藏代码](http://joesmart.iteye.com/images/icon_star.png)
- import java.lang.reflect.Type;  
- import java.sql.Timestamp;  
- import java.text.DateFormat;  
- import java.text.ParseException;  
- import java.text.SimpleDateFormat;  
- import java.util.Date;  
- 
- import com.google.gson.JsonDeserializationContext;  
- import com.google.gson.JsonDeserializer;  
- import com.google.gson.JsonElement;  
- import com.google.gson.JsonParseException;  
- import com.google.gson.JsonPrimitive;  
- import com.google.gson.JsonSerializationContext;  
- import com.google.gson.JsonSerializer;  
- 
- public class TimestampTypeAdapter implements JsonSerializer<Timestamp>, JsonDeserializer<Timestamp>{  
-     private final DateFormat format = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");  
-     public JsonElement serialize(Timestamp src, Type arg1, JsonSerializationContext arg2) {  
-         String dateFormatAsString = format.format(new Date(src.getTime()));  
-         return new JsonPrimitive(dateFormatAsString);  
-     }  
- 
-     public Timestamp deserialize(JsonElement json, Type typeOfT, JsonDeserializationContext context) throws JsonParseException {  
-         if (!(json instanceof JsonPrimitive)) {  
-             throw new JsonParseException("The date should be a string value");  
-         }  
- 
-         try {  
-             Date date = format.parse(json.getAsString());  
-             return new Timestamp(date.getTime());  
-         } catch (ParseException e) {  
-             throw new JsonParseException(e);  
-         }  
-     }  
- 
- }  
  类型适配类
应用类型适配器 写道
Gson gson = new GsonBuilder().registerTypeAdapter(Timestamp.class,new TimestampTypeAdapter()).setDateFormat("yyyy-MM-dd HH:mm:ss").create();
String jsonString = gson.toJson(resourceInfo,ResourceGeoInfo.class);
输出结果
{"positionTime":"2010-01-07 10:57:27"}
Date 类型的时间转换第二种方式;
Java代码  ![收藏代码](http://joesmart.iteye.com/images/icon_star.png)
- Gson gson = new GsonBuilder().setDateFormat("yyyy-MM-dd HH:mm:ss").create();  
- String jsonString = gson.toJson(new Date(System.currentTimeMillis()),Date.class);  
- System.out.println(jsonString);  
输出结果:
"2010-01-07 12:24:34"
