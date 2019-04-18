# hive中UDF开发：解析json对象和解析json数组对象 - z69183787的专栏 - CSDN博客
2018年04月03日 14:26:13[OkidoGreen](https://me.csdn.net/z69183787)阅读数：474
[https://blog.csdn.net/oTengYue/article/details/70255202](https://blog.csdn.net/oTengYue/article/details/70255202)
hive默认函数：
+-------------------------------------------------------------------+
                                json
 +-------------------------------------------------------------------+
 {"store":
   {"fruit":[{"weight":8,"type":"apple"},{"weight":9,"type":"pear"}],
    "bicycle":{"price":19.95,"color":"red"}
   },
  "email":"amy@only_for_json_udf_test.net",
  "owner":"amy"
 }
 +-------------------------------------------------------------------+
 json内容可以用以下查询语句解析
[sql][view
 plain](https://blog.csdn.net/u010670689/article/details/43703497#)[copy](https://blog.csdn.net/u010670689/article/details/43703497#)
- hive> SELECT get_json_object(src_json.json, '$.owner') FROM src_json;  
- amy  
- hive> SELECT get_json_object(src_json.json, '$.store.fruit[0]') FROM src_json;  
- {"weight":8,"type":"apple"}  
- hive> SELECT get_json_object(src_json.json, '$.non_exist_key') FROM src_json;  
- NULL

查阅hive的UDF函数指南可知，虽然udf中的get_json_object和json_tuple能对json解析，但有时候没法实现复杂业务扩展，同时也没有UDF对JSON数组的解析，因此，很多时候需要自己实现解析JSON的UDF，写UDF中如果使用大量开源库（例如fastjson或gson）则会直接导致生成的jar依赖较大，并不是最理想的，本博文查阅了hive源码，发现hive内部用JSONObject和JSONArray解析json对象，则我们同样可以使用这两个对象解析即可，本文旨在提供一个可调用json对象和数组的解析udf，并提供复杂json的解析，而不是替代get_json_object等内置函数。 
一、JSONObject解析JSON对象
```java
package com.jd.bdp.util.udf;
import org.apache.commons.lang.StringUtils;
import org.apache.hadoop.hive.ql.exec.UDF;
import org.json.JSONException;
import org.json.JSONObject;
import org.json.JSONTokener;
/**
 * JSON对象解析UDF
 * date:2017-04-20
 */
public class GetJsonObject extends UDF {
    /**
     * 解析json并返回对应的值。例如
     add jar jar/bdp_udf_demo-1.0.0.jar;
     create temporary function getJsonObject as 'com.jd.bdp.util.udf.GetJsonObject';
     select getJsonObject(json字符串,key值)
     * @param jsonStr
     * @param objName
     * @return
     */
    public String evaluate(String jsonStr,String objName) throws JSONException {
        if(StringUtils.isBlank(jsonStr)|| StringUtils.isBlank(objName)){
            return null;
        }
        JSONObject jsonObject = new JSONObject(new JSONTokener(jsonStr));
        Object objValue = jsonObject.get(objName);
        if(objValue==null){
            return null;
        }
        return objValue.toString();
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
二、JSONArray解析JSON数组对象
　　对json数组字符串的解析，最关键的是生成子json字符串对象，之后往往需要配合上文中getJsonObject或get_json_object进行子json字符串对象读取数据了。下面代码实现了解析json字符串数组对象生成子json字符串对象数组。
```java
package com.jd.bdp.util.udf;
import org.apache.commons.lang.StringUtils;
import org.apache.hadoop.hive.ql.exec.UDF;
import org.apache.hadoop.hive.ql.metadata.HiveException;
import org.apache.hadoop.io.Text;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONTokener;
import java.util.ArrayList;
/**
 * JSON数组对象解析UDF
 * date:2017-04-20
 */
public class GetJsonArray extends UDF {
    /**
     * 解析json并返回对应子json字符串数组,例如
     add jar jar/bdp_udf_demo-1.0.0.jar;
     create temporary function getJsonArray as 'com.jd.bdp.util.udf.GetJsonArray';
     select getJsonArray(json字符串)
     * @param jsonArrayStr
     * @return
     * @throws HiveException
     */
    public ArrayList<Text> evaluate(String jsonArrayStr) throws JSONException {
        if(StringUtils.isBlank(jsonArrayStr)||StringUtils.isBlank(jsonArrayStr)){
            return null;
        }
        ArrayList<Text> textList = new ArrayList<Text>();
        if(!jsonArrayStr.trim().startsWith("[")){
            textList.add(new Text(jsonArrayStr));
        }else{
            JSONArray jsonArray = new JSONArray(new JSONTokener(jsonArrayStr));
            Text[] jsonTexts = new Text[jsonArray.length()];
            for(int i=0;i<jsonArray.length();i++){
                String json = jsonArray.getJSONObject(i).toString();
                textList.add(new Text(json));
            }
        }
        return textList;
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
三、两个UDF的配合使用过程
添加自定义函数
```
add jar jar/bdp_udf_demo-1.0.0.jar;
create temporary function getJsonObject as 'com.jd.bdp.util.udf.GetJsonObject';
create temporary function getJsonArray as 'com.jd.bdp.util.udf.GetJsonArray';
```
- 1
- 2
- 3
在hql中使用自定义UDF函数 
本场景是对fdm_janc2_product_1_chain表中的productioninfo中json字符串数组解析，并使用lateral view explode对生成的子json字符串数组列转行，最后对子json字符串解析读取数据。
```
select 
        getJsonObject(adtable.info,'factoryAddress') factoryAddress,
        getJsonObject(adtable.info,'factoryName') factoryName,
        getJsonObject(adtable.info,'factoryQs') factoryQs,
        *
from
(
        select * from fdm.fdm_janc2_product_1_chain where dp = 'ACTIVE'
)dtl lateral view explode(getJsonArray(productioninfo)) adtable as info
```
