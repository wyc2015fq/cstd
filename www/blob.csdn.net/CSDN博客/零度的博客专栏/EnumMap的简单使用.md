# EnumMap的简单使用 - 零度的博客专栏 - CSDN博客
2017年03月27日 10:26:21[零度anngle](https://me.csdn.net/zmx729618)阅读数：356
想起前几天有个需求：文件上传如果出错，返回给用户的消息形如为： 错误的行号和原因：
2,4,6  商品id为必填
1,7   应用标识错误
8   时间格式错误
         由于错误原因是有限的，可以用枚举ErrMsgEnum表示。起初利用 HashMap<ErrMsgEnum,String> 来保存，了解了EnumMap内部是利用数组存储后，更改为EnumMap<ErrMsgEnum,String> 效率更高。
**实现**
  1.  ErrMsgEnum.java  
**[java]**[view
 plain](http://blog.csdn.net/cuidiwhere/article/details/21995363#)[copy](http://blog.csdn.net/cuidiwhere/article/details/21995363#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/254592/fork)
- publicenum ErrMsgEnum {  
-     required_item_id("商品id为必填项"),   
-     invalid_app_id("应用标识错误"),   
-     invalid_date("时间格式错误");  
- 
- private String value;  
- 
- private ErrMsgEnum(String value) {  
- this.setValue(value);  
-     }  
- 
- public String getValue() {  
- return value;  
-     }  
- 
- publicvoid setValue(String value) {  
- this.value = value;  
-     }  
- 
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
2. TestEnumMap.java 
**[java]**[view
 plain](http://blog.csdn.net/cuidiwhere/article/details/21995363#)[copy](http://blog.csdn.net/cuidiwhere/article/details/21995363#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/254592/fork)
- import java.util.EnumMap;  
- import java.util.Map;  
- 
- publicclass TestEnumMap {  
- 
- publicstaticvoid main(String[] args) {  
-     EnumMap<ErrMsgEnum,String> errMsgMap = new EnumMap<ErrMsgEnum,String>(ErrMsgEnum.class);  
- 
-     errMsgMap.put(ErrMsgEnum.required_item_id, "2,4,6");  
-     errMsgMap.put(ErrMsgEnum.invalid_app_id, "1,7");  
-     errMsgMap.put(ErrMsgEnum.invalid_date, "8");  
- 
- for(Map.Entry<ErrMsgEnum,String> entry:errMsgMap.entrySet() ){  
-         System.out.println(entry.getValue()+ " " + entry.getKey().getValue());  
-     }     
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
**注意事项:**
  1. 使用EnumMap时，必须指定枚举类型。All of the keys in an enum map must come from a single enum type that is specified, explicitly or implicitly, when the map is created.
  2. key不能为null。 Null keys are not permitted.
  3.  EnumMap内部以数组实现，性能更好。Enum maps are represented internally as arrays.  This representation is extremely compact and efficient.
