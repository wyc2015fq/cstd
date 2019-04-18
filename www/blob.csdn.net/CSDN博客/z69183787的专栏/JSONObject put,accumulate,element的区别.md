# JSONObject put,accumulate,element的区别 - z69183787的专栏 - CSDN博客
2014年01月22日 10:09:16[OkidoGreen](https://me.csdn.net/z69183787)阅读数：45462
public Object put (Object key, Object value) 将value映射到key下。如果此JSONObject对象之前存在一个value在这个key下，当前的value会替换掉之前的value
Associates the specified value with the specified key in this map(optional operation). If the map previously contained . a mapping for this key, the old value is replaced by the specified value. (A map m is said
 to contain a mapping for a key k if and only if m.containsKey(k) would return true .))
public JSONObject accumulate (String key, Object value) 累积value到这个key下。这个方法同element()方法类似，特殊的是，如果当前已经存在一个value在这个key下那么一个JSONArray将会存储在这个key下来保存所有累积的value。如果已经存在一个JSONArray，那么当前的value就会添加到这个JSONArray中
。相比之下replace方法会替代先前的value
Accumulate values under a key. It is similar to the element method except that if there is already an object stored 
under the key then a JSONArray is stored under the key to hold all of the accumulated values. If there is already a 
JSONArray, then the new value is appended to it. In contrast, the replace method replaces the previous value.
public JSONObject element (String key, Object value) 将键/值对放到这个JSONObject对象里面。如果当前value为空(null)，那么如果这个key存在的话，这个key就会移除掉。如果这
个key之前有value值，那么此方法会调用accumulate()方法。
Put a key/value pair in the JSONObject. If the value is null, then the key will be removed from the JSONObject if it is 
present. If there is a previous value assigned to the key, it will call accumulate.
