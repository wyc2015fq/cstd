# Spring JdbcTemplate 查询出的Map，是如何产生大小写忽略的Key的？ - z69183787的专栏 - CSDN博客
2014年08月05日 11:14:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：14349
Java 是区分大小写的，普通的Map例如HashMap如果其中的key="ABC" value="XXX"
那么map.get("Abc") 或 map.get("abc")是获取不到值得。
但Spring中产生了一个忽略大小写的map使我产生了好奇
例如 jdbcTemplate.queryForList(sql)产生的 List<Map>
key="BID" value="STR1"
其中key都是大写的
但如果用
System.out.println("map.get(BILLIDS):" + map.get("BID") );
System.out.println("map.get(billids):" + map.get("bid") );
System.out.println("map.get(bIlLIds):" + map.get("bId") );
都可以查出=STR1
请问这是怎么做到的？
**Spring用的是自己封装的Map：org.springframework.util.LinkedCaseInsensitiveMap，基于java.util.LinkedHashMap封装的。**
**LinkedCaseInsensitiveMap有一个成员变量Map caseInsensitiveKeys，小写后的Key与真实key的映射关系，get的时候先把你传入的key转小写得到真实的KEY，然后得到对应value，所以大小写不敏感了。但其实key还保持本色的，所以如果序列化为json或者xml格式，key是大写，这是需要注意的。**
**  public Object put(String key, Object value)    {        caseInsensitiveKeys.put(convertKey(key), key);        return super.put(key, value);    }    public Object get(Object key)    {        if(key instanceof String)            return super.get(caseInsensitiveKeys.get(convertKey((String)key)));        else            return null;    }**
