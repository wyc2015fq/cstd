# Java构造和解析Json数据的两种方法详解二 - z69183787的专栏 - CSDN博客
2015年07月16日 17:13:17[OkidoGreen](https://me.csdn.net/z69183787)阅读数：918
个人分类：[数据转换-Json](https://blog.csdn.net/z69183787/article/category/2175427)
 在[www.json.org](http://www.json.org/)上公布了很多JAVA下的json构造和解析工具，其中org.json和json-lib比较简单，两者使用上差不多但还是有些区别。下面接着介绍用org.json构造和解析Json数据的方法示例。
      用json-lib构造和解析Json数据的方法详解请参见我上一篇博文：[Java构造和解析Json数据的两种方法详解一](http://www.cnblogs.com/lanxuezaipiao/archive/2013/05/23/3096001.html)
# 一、介绍
org.json包是另一个用来beans,collections,maps,java arrays 和XML和JSON互相转换的包，主要就是用来解析Json数据，在其官网[http://www.json.org/上](http://www.json.org/)有详细讲解，有兴趣的可以去研究。
# 二、下载jar依赖包
      可以去[这里](http://download.csdn.net/detail/lanxuezaipiao/5440983)下载
# 三、基本方法介绍
      由于org.json不能直接与bean进行转换，需要通过map进行中转，为了方便，我这里写了一个工具类JsonHelper，用于Json与Map、Bean的相互转换
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
package com.json;
import java.lang.reflect.Method;
import java.text.ParseException;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import org.json.JSONException;
import org.json.JSONObject;
/**
 * 
 * 1:将JavaBean转换成Map、JSONObject
 * 2:将Map转换成Javabean
 * 3:将JSONObject转换成Map、Javabean
 * 
 * @author Alexia
 */
public class JsonHelper {
    
    /**
     * 将Javabean转换为Map
     * 
     * @param javaBean
     *            javaBean
     * @return Map对象
     */
    public static Map toMap(Object javaBean) {
        Map result = new HashMap();
        Method[] methods = javaBean.getClass().getDeclaredMethods();
        for (Method method : methods) {
            try {
                if (method.getName().startsWith("get")) {
                    String field = method.getName();
                    field = field.substring(field.indexOf("get") + 3);
                    field = field.toLowerCase().charAt(0) + field.substring(1);
                    Object value = method.invoke(javaBean, (Object[]) null);
                    result.put(field, null == value ? "" : value.toString());
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        return result;
    }
    /**
     * 将Json对象转换成Map
     * 
     * @param jsonObject
     *            json对象
     * @return Map对象
     * @throws JSONException
     */
    public static Map toMap(String jsonString) throws JSONException {
        JSONObject jsonObject = new JSONObject(jsonString);
        
        Map result = new HashMap();
        Iterator iterator = jsonObject.keys();
        String key = null;
        String value = null;
        
        while (iterator.hasNext()) {
            key = (String) iterator.next();
            value = jsonObject.getString(key);
            result.put(key, value);
        }
        return result;
    }
    /**
     * 将JavaBean转换成JSONObject（通过Map中转）
     * 
     * @param bean
     *            javaBean
     * @return json对象
     */
    public static JSONObject toJSON(Object bean) {
        return new JSONObject(toMap(bean));
    }
    /**
     * 将Map转换成Javabean
     * 
     * @param javabean
     *            javaBean
     * @param data
     *            Map数据
     */
    public static Object toJavaBean(Object javabean, Map data) {
        Method[] methods = javabean.getClass().getDeclaredMethods();
        for (Method method : methods) {
            try {
                if (method.getName().startsWith("set")) {
                    String field = method.getName();
                    field = field.substring(field.indexOf("set") + 3);
                    field = field.toLowerCase().charAt(0) + field.substring(1);
                    method.invoke(javabean, new Object[] {
                    data.get(field)
                    });
                }
            } catch (Exception e) {
            }
        }
        return javabean;
    }
    /**
     * JSONObject到JavaBean
     * 
     * @param bean
     *            javaBean
     * @return json对象
     * @throws ParseException
     *             json解析异常
     * @throws JSONException
     */
    public static void toJavaBean(Object javabean, String jsonString)
            throws ParseException, JSONException {
        JSONObject jsonObject = new JSONObject(jsonString);
    
        Map map = toMap(jsonObject.toString());
        
        toJavaBean(javabean, map);
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
# 四、演示示例
这里以基本的几个常用方法进行测试
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
package com.json;
import java.text.ParseException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
/**
 * 使用json-lib构造和解析Json数据
 * 
 * @author Alexia
 * @date 2013/5/23
 * 
 */
public class OrgJsonTest {
    /**
     * 构造Json数据
     * 
     * @return
     * @throws JSONException
     */
    public static String BuildJson() throws JSONException {
        // JSON格式数据解析对象
        JSONObject jo = new JSONObject();
        // 下面构造两个map、一个list和一个Employee对象
        Map<String, String> map1 = new HashMap<String, String>();
        map1.put("name", "Alexia");
        map1.put("sex", "female");
        map1.put("age", "23");
        Map<String, String> map2 = new HashMap<String, String>();
        map2.put("name", "Edward");
        map2.put("sex", "male");
        map2.put("age", "24");
        List<Map> list = new ArrayList<Map>();
        list.add(map1);
        list.add(map2);
        Employee employee = new Employee();
        employee.setName("wjl");
        employee.setSex("female");
        employee.setAge(24);
        // 将Map转换为JSONArray数据
        JSONArray ja = new JSONArray();
        ja.put(map1);
        System.out.println("JSONArray对象数据格式：");
        System.out.println(ja.toString());
        // 将Javabean转换为Json数据（需要Map中转）
        JSONObject jo1 = JsonHelper.toJSON(employee);
        System.out.println("\n仅含Employee对象的Json数据格式：");
        System.out.println(jo1.toString());
        // 构造Json数据，包括一个map和一个含Employee对象的Json数据
        jo.put("map", ja);
        jo.put("employee", jo1.toString());
        System.out.println("\n最终构造的JSON数据格式：");
        System.out.println(jo.toString());
        return jo.toString();
    }
    /**
     * 解析Json数据
     * 
     * @param jsonString
     *            Json数据字符串
     * @throws JSONException
     * @throws ParseException
     */
    public static void ParseJson(String jsonString) throws JSONException,
            ParseException {
        JSONObject jo = new JSONObject(jsonString);
        JSONArray ja = jo.getJSONArray("map");
        System.out.println("\n将Json数据解析为Map：");
        System.out.println("name: " + ja.getJSONObject(0).getString("name")
                + " sex: " + ja.getJSONObject(0).getString("sex") + " age: "
                + ja.getJSONObject(0).getInt("age"));
        String jsonStr = jo.getString("employee");
        Employee emp = new Employee();
        JsonHelper.toJavaBean(emp, jsonStr);
        System.out.println("\n将Json数据解析为Employee对象：");
        System.out.println("name: " + emp.getName() + " sex: " + emp.getSex()
                + " age: " + emp.getAge());
    }
    /**
     * @param args
     * @throws JSONException
     * @throws ParseException
     */
    public static void main(String[] args) throws JSONException, ParseException {
        // TODO Auto-generated method stub
        ParseJson(BuildJson());
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
运行结果如下
# 五、与json-lib比较
      json-lib和org.json的使用几乎是相同的，我总结出的区别有两点：
      1. org.json比json-lib要轻量得多，前者没有依赖任何其他jar包，而后者要依赖ezmorph和commons的lang、logging、beanutils、collections等组件
      2. json-lib在构造bean和解析bean时比org.json要方便的多，json-lib可直接与bean互相转换，而org.json不能直接与bean相互转换而需要map作为中转，若将bean转为json数据，首先需要先将bean转换为map再将map转为json，比较麻烦。
      总之，还是那句话—适合自己的才是最好的，大家要按需选取使用哪种方法进行解析。最后给大家介绍两款解析Json数据的工具：一是在线工具JSON[Edit（](http://braincast.nl/samples/jsoneditor/)http://braincast.nl/samples/jsoneditor/）；另一个是Eclipse的插件JSON
 Tree Analyzer，都很好用，推荐给大家使用！
