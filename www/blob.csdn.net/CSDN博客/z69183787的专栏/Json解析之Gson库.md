# Json解析之Gson库 - z69183787的专栏 - CSDN博客
2013年07月10日 20:00:02[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3389
Json（JavaScript Object Notation）是一种轻量级的数据交换格式，类似XML，但比XML更小更快更容易解析。当前各种流行的web应用框架都对Json提供良好的支持，各种流行开发语言也支持Json解析。
Java中解析Json的方式也很多，根据官方的JSONObject库，自己设计的一个抽象工具类：
Java代码  ![收藏代码](http://zhiweiofli.iteye.com/images/icon_star.png)
- import java.io.Serializable;  
- import java.util.List;  
- 
- import org.json.JSONException;  
- import org.json.JSONObject;  
- 
- publicabstractclass JsonParser <T extends Serializable>{  
- 
- abstract List<T> getListFromJson(String str);  
- 
- abstract T getObjectFromJson(JSONObject jobj);  
- 
- public T getObjectFromJson(String str) {  
- try {  
-             JSONObject jsonObject = new JSONObject(str);  
- return getObjectFromJson(jsonObject);  
-         } catch (JSONException e) {  
-             e.printStackTrace();   
-         } catch(Exception e){  
-             e.printStackTrace();  
-         }  
- returnnull;  
-     }  
- 
- protected String getValueByName(JSONObject jobj, String name)  
- throws JSONException{  
- if(jobj.has(name)){  
- return jobj.getString(name);  
-         }  
- returnnull;  
-     }  
- 
- }  
具体实现的子类如下：
Java代码  ![收藏代码](http://zhiweiofli.iteye.com/images/icon_star.png)
- publicclass UserInfoParser extends JsonParser<UserInfo> {  
- 
- private UserInfoParser() {  
- 
-     }  
- 
- @Override
-     List<UserInfo> getListFromJson(String str) {  
- try {  
-             JSONArray array = new JSONArray(str);  
- int len = array.length();  
-             ArrayList<UserInfo> list = new ArrayList<UserInfo>(len);  
- for(int i=0;i<len;i++) {  
-                 UserInfo info = getObjectFromJson(array.getJSONObject(i));  
-                 list.add(info);  
-             }  
- return list;  
-         } catch (JSONException e) {  
-             e.printStackTrace();  
-         }  
- returnnull;  
-     }  
- 
- @Override
-     UserInfo getObjectFromJson(JSONObject jobj) {  
- try {  
-             UserInfo info = new UserInfo();  
-             info.setId(getValueByName(jobj, UserInfo.NODE_ID));  
-             info.setFace(getValueByName(jobj,UserInfo.NODE_FACE));  
-             info.setUsername(getValueByName(jobj,UserInfo.NODE_USER_NAME));  
- return info;  
-         } catch (JSONException e) {  
-             e.printStackTrace();  
-         }  
- returnnull;  
-     }  
- 
- }  
当泛型值对象T，对应的数据结构比较简单，层次比较少的时候，官方的JSONObject库解析还过得去。当遇到层次较多（Json里面包含N个Json数组），数据结构复杂（Json由对个复杂数据的Json组成）的Json，解析速度就会大大降低！
在处理复杂Json结构时，我推荐使用Google的Gson解析库。刚刚接触Gson时，我有以下疑虑：
1、Gson对Json的支持度如何，能不能支持所有的Json结构？
2、由于Gson是基于Java的反射原理来实现的，解析的效率如何保证？
3、上手难度如何？
当我在项目实验性地引入Gson后，我o嘴了~~真不愧是Google出品，实属佳品！
再我的项目是基于android平台的App，使用Gson之前，不仅http请求和Json数据解析的耗时太长，而且内存占有一直居高不下。
使用Gson后，解析的时间缩短了30%（这只是Json的解析时间，不算上http请求的时间），内存占用足足减少了一半！！！最重要的是，开发效率还提高不少，何以见得，请看：
1、值对象必须实现序列化接口，成员属性的名称必须与Json数据的key一致，建议遵从J2EE的标准，使用get-set方法控制属性的访问，因为Json的key是后台应用定义的，假如后台与前台的开发语言不同，命名规范也不一致，使用get-set能有效分离这些不规范的命名到其他模块代码中去。
Java代码  ![收藏代码](http://zhiweiofli.iteye.com/images/icon_star.png)
- publicclass UserInfo implements Serializable {  
- 
- privatestaticfinallong serialVersionUID = 1050128890144400614L;  
- 
- private String id;  
- private String username;  
- private String face;  
- 
- public String getId() {  
- return id;  
-     }  
- 
- publicvoid setId(String id) {  
- this.id = id;  
-     }  
- 
- public String getUsername() {  
- return username;  
-     }  
- 
- publicvoid setUsername(String username) {  
- this.username = username;  
-     }  
- 
- public String getFace() {  
- return face;  
-     }  
- 
- publicvoid setFace(String face) {  
- this.face = face;  
-     }  
- 
- }  
2、奉上一个基于Gson的Json解析封装工具：
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
```
Java代码  ![收藏代码](http://zhiweiofli.iteye.com/images/icon_star.png)
- /**
-  * Gson类库的封装工具类，专门负责解析json数据</br>
-  * 内部实现了Gson对象的单例
-  * @author zhiweiofli
-  * @version 1.0
-  * @since 2012-9-18
-  */
- publicclass JsonUtil {  
- 
- privatestatic Gson gson = null;  
- 
- static {  
- if (gson == null) {  
-             gson = new Gson();  
-         }  
-     }  
- 
- private JsonUtil() {  
- 
-     }  
- 
- /**
-      * 将对象转换成json格式
-      * 
-      * @param ts
-      * @return
-      */
- publicstatic String objectToJson(Object ts) {  
-         String jsonStr = null;  
- if (gson != null) {  
-             jsonStr = gson.toJson(ts);  
-         }  
- return jsonStr;  
-     }  
- 
- /**
-      * 将对象转换成json格式(并自定义日期格式)
-      * 
-      * @param ts
-      * @return
-      */
- publicstatic String objectToJsonDateSerializer(Object ts,  
- final String dateformat) {  
-         String jsonStr = null;  
-         gson = new GsonBuilder()  
-                 .registerTypeHierarchyAdapter(Date.class,  
- new JsonSerializer<Date>() {  
- public JsonElement serialize(Date src,  
-                                     Type typeOfSrc,  
-                                     JsonSerializationContext context) {  
-                                 SimpleDateFormat format = new SimpleDateFormat(  
-                                         dateformat);  
- returnnew JsonPrimitive(format.format(src));  
-                             }  
-                         }).setDateFormat(dateformat).create();  
- if (gson != null) {  
-             jsonStr = gson.toJson(ts);  
-         }  
- return jsonStr;  
-     }  
- 
- /**
-      * 将json格式转换成list对象
-      * 
-      * @param jsonStr
-      * @return
-      */
- publicstatic List<?> jsonToList(String jsonStr) {  
-         List<?> objList = null;  
- if (gson != null) {  
-             java.lang.reflect.Type type = new com.google.gson.reflect.TypeToken<List<?>>() {  
-             }.getType();  
-             objList = gson.fromJson(jsonStr, type);  
-         }  
- return objList;  
-     }  
- 
- /**
-      * 将json格式转换成list对象，并准确指定类型
-      * @param jsonStr
-      * @param type
-      * @return
-      */
- publicstatic List<?> jsonToList(String jsonStr, java.lang.reflect.Type type) {  
-         List<?> objList = null;  
- if (gson != null) {  
-             objList = gson.fromJson(jsonStr, type);  
-         }  
- return objList;  
-     }  
- 
- /**
-      * 将json格式转换成map对象
-      * 
-      * @param jsonStr
-      * @return
-      */
- publicstatic Map<?, ?> jsonToMap(String jsonStr) {  
-         Map<?, ?> objMap = null;  
- if (gson != null) {  
-             java.lang.reflect.Type type = new com.google.gson.reflect.TypeToken<Map<?, ?>>() {  
-             }.getType();  
-             objMap = gson.fromJson(jsonStr, type);  
-         }  
- return objMap;  
-     }  
- 
- /**
-      * 将json转换成bean对象
-      * 
-      * @param jsonStr
-      * @return
-      */
- publicstatic Object jsonToBean(String jsonStr, Class<?> cl) {  
-         Object obj = null;  
- if (gson != null) {  
-             obj = gson.fromJson(jsonStr, cl);  
-         }  
- return obj;  
-     }  
- 
- /**
-      * 将json转换成bean对象
-      * 
-      * @param jsonStr
-      * @param cl
-      * @return
-      */
- @SuppressWarnings("unchecked")  
- publicstatic <T> T jsonToBeanDateSerializer(String jsonStr, Class<T> cl,  
- final String pattern) {  
-         Object obj = null;  
-         gson = new GsonBuilder()  
-                 .registerTypeAdapter(Date.class, new JsonDeserializer<Date>() {  
- public Date deserialize(JsonElement json, Type typeOfT,  
-                             JsonDeserializationContext context)  
- throws JsonParseException {  
-                         SimpleDateFormat format = new SimpleDateFormat(pattern);  
-                         String dateStr = json.getAsString();  
- try {  
- return format.parse(dateStr);  
-                         } catch (ParseException e) {  
-                             e.printStackTrace();  
-                         }  
- returnnull;  
-                     }  
-                 }).setDateFormat(pattern).create();  
- if (gson != null) {  
-             obj = gson.fromJson(jsonStr, cl);  
-         }  
- return (T) obj;  
-     }  
- 
- /**
-      * 根据
-      * 
-      * @param jsonStr
-      * @param key
-      * @return
-      */
- publicstatic Object getJsonValue(String jsonStr, String key) {  
-         Object rulsObj = null;  
-         Map<?, ?> rulsMap = jsonToMap(jsonStr);  
- if (rulsMap != null && rulsMap.size() > 0) {  
-             rulsObj = rulsMap.get(key);  
-         }  
- return rulsObj;  
-     }  
- 
- }  
引用方式十分简单：
Java代码  ![收藏代码](http://zhiweiofli.iteye.com/images/icon_star.png)
- (UserInfo)JsonUtil.jsonToBean(jsonString, UserInfo.class);  
3、解析复杂Json的方式
Gson支持解析多层结构的Json，当然对于多层解析的效率，暂时还没测试过，估计效率下降不会超过JSONObject...
想解析Json中的Json，Json中的Json数组，Gson提供InstanceCreator结构，来实例化对应的对象，用来加载已解析的json数据。估计解析的流程是，Gson反射属性名称前，先考量能否获得其对应的实例，再对属性对象进行反射、赋值，如此迭代进行...
例如，解析Json数组的方式，实现：
Java代码  ![收藏代码](http://zhiweiofli.iteye.com/images/icon_star.png)
- publicclass UserInfoResult extends ResponseState implements InstanceCreator<List<UserInfo>>{  
- 
- privatestaticfinallong serialVersionUID = -8701527648781449574L;  
- 
- public List<UserInfo> userlist;  
- 
- @Override
- public List<UserInfo> createInstance(Type arg0) {  
- returnnew ArrayList<UserInfo>(2);  
-     }  
- }  
对比JSONObject，Gson的好处在于：
1、高效，安全的反射，带来高效的解析速度
2、简化的开发流程，对比JSONObject每次都要设计解析的对象，Gson只需要设计值对象，由此带来的就是敏捷的开发
3、良好的支持，Gson属于开源项目，项目位于[http://code.google.com/p/google-gson/](http://code.google.com/p/google-gson/)，现在版本还在不断升级中，建议使用最新的稳定版。
最后，本文只对比介绍了Gson对Json解析的方面，对于Json的自动化生成，注释的使用，容以后再分享...
