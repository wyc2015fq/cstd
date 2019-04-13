
# Java json转Map，转bean，转List - 无界 - CSDN博客

2016年08月19日 16:02:00[21aspnet](https://me.csdn.net/21aspnet)阅读数：3071个人分类：[Java																](https://blog.csdn.net/21aspnet/article/category/1877583)



引用jackson
```python
/**
*  json转Map，转bean，转List<bean> by http://blog.csdn.net/21aspnet/
*  需要jackjson jar包
*/
public class JsonUtil {
    /**
     * Object转Json
     */
    public static String ObjectToJson(Object value) {
        try {
            ObjectMapper mapper = new ObjectMapper();
            String js = mapper.writeValueAsString(value);
            return js;
        } catch (Exception e) {
            System.out.println(e.toString());
            return "Error";
        }
    }
    /**
     * Json转Object
     */
    public static Object JsonToList(String json, Class<?> bean) throws Exception {
        ObjectMapper mapper = new ObjectMapper();
        JavaType javaType = mapper.getTypeFactory().constructParametricType(List.class, bean);
        return mapper.readValue(json, javaType);
    }
    /**
     * Json转Object
     */
    public static Object JsonToBean(String json, Class<?> bean) throws Exception {
        ObjectMapper mapper = new ObjectMapper();
        JavaType javaType = mapper.getTypeFactory().uncheckedSimpleType(bean);
        return mapper.readValue(json, javaType);
    }
    /**
     * Json转Map
     */
    public static Map<String, Map<String, String>> JsonToMap(String json) {
        try {
            ObjectMapper mapper = new ObjectMapper();
            Map<String, Map<String, String>> maps = mapper.readValue(json, Map.class);                      
            return maps;
        } catch (Exception e) {
            System.out.println(e.toString());
            return null;
        }
    }
}
```


