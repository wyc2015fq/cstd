# Jackson处理json转换map时遇到的问题 - z69183787的专栏 - CSDN博客
2016年11月11日 14:15:52[OkidoGreen](https://me.csdn.net/z69183787)阅读数：8769
调用第三方的http接口，返回的是 json字符串
类似如下格式：
![](https://img-blog.csdn.net/20161111141028907?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
原先的处理方式，使用jackson 的objectmapper 转换成 Map<String,Object> 形式处理
```java
private static final ObjectMapper objectMapper;
    static {
        objectMapper = new ObjectMapper();
        //去掉默认的时间戳格式
        objectMapper.configure(SerializationFeature.WRITE_DATES_AS_TIMESTAMPS, false);
        //设置为中国上海时区
        objectMapper.setTimeZone(TimeZone.getTimeZone("GMT+8"));
        objectMapper.configure(SerializationFeature.WRITE_NULL_MAP_VALUES, false);
        //空值不序列化
        objectMapper.setSerializationInclusion(JsonInclude.Include.NON_NULL);
        //反序列化时，属性不存在的兼容处理
        objectMapper.getDeserializationConfig().withoutFeatures(DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES);
        //序列化时，日期的统一格式
        objectMapper.setDateFormat(new SimpleDateFormat(DatePatternEnum.TimePattern.pattern));
        objectMapper.configure(SerializationFeature.FAIL_ON_EMPTY_BEANS, false);
        objectMapper.configure(DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES, false);
        //单引号处理
        objectMapper.configure(com.fasterxml.jackson.core.JsonParser.Feature.ALLOW_SINGLE_QUOTES, true);
    }
```
```java
Map<String, Object> map = objectMapper.readValue(result, Map.class);
            int code = (Integer)map.get("code");
            if(code == 1){
                List<Map<String,Object>> coupons = (List)(((Map<String, Object>) map.get("data")).get("coupons"));
                if(CollectionUtils.isNotEmpty(coupons)){
                    //codeMap.put("code",Long.valueOf(String.valueOf(coupons.get(0).get("id"))));
                    codeMap.put("code",(Long)(coupons.get(0).get("id")));
                }else{
                    codeMap.put("msg",code + " coupons[] 为空");
                }
            }
```
这样会导致的问题是 若存在一个数值型的 值，在转换过程中会根据值得大小自动 映射成 Integer 或 Long，导致解析时不能使用强转，发生转换异常
**解决方式：**
1、使用com.alibaba.fastjson，JsonObject处理http接收的返回json字符串
```java
JSONObject jsonObject = JSON.parseObject(result);
            Integer code = jsonObject.getInteger("code");
            if(code != null && code == 1){
                JSONArray coupons = jsonObject.getJSONObject("data").getJSONArray("coupons");
                if(CollectionUtils.isNotEmpty(coupons)){
                    //codeMap.put("code",Long.valueOf(String.valueOf(coupons.get(0).get("id"))));
                    codeMap.put("code",coupons.getJSONObject(0).getLong("id"));
                }else{
                    codeMap.put("msg",code + " coupons[] 为空");
                }
            }
```
2、还是使用jackson ，但复杂类型时 ，用 java对象进行转换，而非map，Integer的类型会自动映射为 类中 Long
