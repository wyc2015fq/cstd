# Jackson 框架，轻易转换JSON （包括时间转换） - z69183787的专栏 - CSDN博客
2015年01月06日 21:53:03[OkidoGreen](https://me.csdn.net/z69183787)阅读数：9886
# [关于jackson中时间字符串的转换](http://blog.csdn.net/masquejava/article/details/10556281)中的错误.
**[java]**[view
 plain](http://blog.csdn.net/masquejava/article/details/10556281#)[copy](http://blog.csdn.net/masquejava/article/details/10556281#)[print](http://blog.csdn.net/masquejava/article/details/10556281#)[?](http://blog.csdn.net/masquejava/article/details/10556281#)
- "yyyy-MM-dd'T'HH:mm:ss.SSSZ"
- "yyyy-MM-dd'T'HH:mm:ss.SSS'Z'"
- "EEE,ddMMMyyyyHH:mm:sszzz"
- "yyyy-MM-dd"
当实体中存在Date类型，但是json字符串中是字符串类型
只支持以上几种格式否则报错
**[java]**[view
 plain](http://blog.csdn.net/masquejava/article/details/10556281#)[copy](http://blog.csdn.net/masquejava/article/details/10556281#)[print](http://blog.csdn.net/masquejava/article/details/10556281#)[?](http://blog.csdn.net/masquejava/article/details/10556281#)
- org.codehaus.jackson.map.JsonMappingException:Cannotconstructinstanceofjava.util.DatefromStringvalue'19802-22T12:00:00.000-0800':notavalidrepresentation(error:Cannotparsedate"19802-22T12:00:00.000-0800":notcompatiblewithanyofstandardforms("yyyy-MM-dd'T'HH:mm:ss.SSSZ","yyyy-MM-dd'T'HH:mm:ss.SSS'Z'","EEE,ddMMMyyyyHH:mm:sszzz","yyyy-MM-dd"))
- at[Source:java.io.StringReader@183f74d;line:1,column:15](throughreferencechain:com.masque.json.Person["birthday"])
- atorg.codehaus.jackson.map.JsonMappingException.from(JsonMappingException.java:163)
- atorg.codehaus.jackson.map.deser.StdDeserializationContext.weirdStringException(StdDeserializationContext.java:220)
- atorg.codehaus.jackson.map.deser.StdDeserializer._parseDate(StdDeserializer.java:511)
- atorg.codehaus.jackson.map.deser.DateDeserializer.deserialize(DateDeserializer.java:26)
- atorg.codehaus.jackson.map.deser.DateDeserializer.deserialize(DateDeserializer.java:17)
- atorg.codehaus.jackson.map.deser.SettableBeanProperty.deserialize(SettableBeanProperty.java:230)
- atorg.codehaus.jackson.map.deser.SettableBeanProperty$MethodProperty.deserializeAndSet(SettableBeanProperty.java:334)
- atorg.codehaus.jackson.map.deser.BeanDeserializer.deserializeFromObject(BeanDeserializer.java:495)
- atorg.codehaus.jackson.map.deser.BeanDeserializer.deserialize(BeanDeserializer.java:351)
- atorg.codehaus.jackson.map.ObjectMapper._readMapAndClose(ObjectMapper.java:2130)
- atorg.codehaus.jackson.map.ObjectMapper.readValue(ObjectMapper.java:1394)
- atcom.masque.json.JacksonTo.main(JacksonTo.java:15)
查了下资料
|Letter|Date or Time Component|Presentation|Examples|
|----|----|----|----|
|G|Era designator|[Text](http://www.oschina.net/question/#text)|AD|
|y|Year|[Year](http://www.oschina.net/question/#year)|1996;96|
|M|Month in year|[Month](http://www.oschina.net/question/#month)|July;Jul;07|
|w|Week in year|[Number](http://www.oschina.net/question/#number)|27|
|W|Week in month|[Number](http://www.oschina.net/question/#number)|2|
|D|Day in year|[Number](http://www.oschina.net/question/#number)|189|
|d|Day in month|[Number](http://www.oschina.net/question/#number)|10|
|F|Day of week in month|[Number](http://www.oschina.net/question/#number)|2|
|E|Day in week|[Text](http://www.oschina.net/question/#text)|Tuesday;Tue|
|a|Am/pm marker|[Text](http://www.oschina.net/question/#text)|PM|
|H|Hour in day (0-23)|[Number](http://www.oschina.net/question/#number)|0|
|k|Hour in day (1-24)|[Number](http://www.oschina.net/question/#number)|24|
|K|Hour in am/pm (0-11)|[Number](http://www.oschina.net/question/#number)|0|
|h|Hour in am/pm (1-12)|[Number](http://www.oschina.net/question/#number)|12|
|m|Minute in hour|[Number](http://www.oschina.net/question/#number)|30|
|s|Second in minute|[Number](http://www.oschina.net/question/#number)|55|
|S|Millisecond|[Number](http://www.oschina.net/question/#number)|978|
|z|Time zone|[General time zone](http://www.oschina.net/question/#timezone)|Pacific Standard Time;PST;GMT-08:00|
|Z|Time zone|[RFC 822 time zone](http://www.oschina.net/question/#rfc822timezone)|-0800|
标准时间格式
|Date and Time Pattern|Result|
|----|----|
|"yyyy.MM.dd G 'at' HH:mm:ss z"|2001.07.04 AD at 12:08:56 PDT|
|"EEE, MMM d, ''yy"|Wed, Jul 4, '01|
|"h:mm a"|12:08 PM|
|"hh 'o''clock' a, zzzz"|12 o'clock PM, Pacific Daylight Time|
|"K:mm a, z"|0:08 PM, PDT|
|"yyyyy.MMMMM.dd GGG hh:mm aaa"|02001.July.04 AD 12:08 PM|
|"EEE, d MMM yyyy HH:mm:ss Z"|Wed, 4 Jul 2001 12:08:56 -0700|
|"yyMMddHHmmssZ"|010704120856-0700|
|"yyyy-MM-dd'T'HH:mm:ss.SSSZ"|2001-07-04T12:08:56.235-0700|
|"yyyy-MM-dd'T'HH:mm:ss.SSSXXX"|2001-07-04T12:08:56.235-07:00|
|"YYYY-'W'ww-u"|2001-W27-3|

Jackson可以轻松的将Java对象转换成json对象和xml文档，同样也可以将json、xml转换成Java对象。
前面有介绍过json-lib这个框架，在线博文：[http://www.cnblogs.com/hoojo/archive/2011/04/21/2023805.html](http://www.cnblogs.com/hoojo/archive/2011/04/21/2023805.html)
相比json-lib框架，Jackson所依赖的jar包较少，简单易用并且性能也要相对高些。而且Jackson社区相对比较活跃，更新速度也比较快。
**一、准备工作**
1、 下载依赖库jar包
Jackson的jar all下载地址：[http://jackson.codehaus.org/1.7.6/jackson-all-1.7.6.jar](http://jackson.codehaus.org/1.7.6/jackson-all-1.7.6.jar)
然后在工程中导入这个jar包即可开始工作
官方示例：[http://wiki.fasterxml.com/JacksonInFiveMinutes](http://wiki.fasterxml.com/JacksonInFiveMinutes)
因为下面的程序是用junit测试用例运行的，所以还得添加junit的jar包。版本是junit-4.2.8
如果你需要转换xml，那么还需要stax2-api.jar
2、 测试类基本代码如下
package com.hoo.test;import java.io.IOException;import java.io.StringWriter;import java.util.ArrayList;import java.util.HashMap;import java.util.Iterator;import java.util.LinkedHashMap;import java.util.List;import java.util.Map;import java.util.Set;import org.codehaus.jackson.JsonEncoding;import org.codehaus.jackson.JsonGenerationException;import org.codehaus.jackson.JsonGenerator;import org.codehaus.jackson.JsonParseException;import org.codehaus.jackson.map.JsonMappingException;import org.codehaus.jackson.map.ObjectMapper;import org.codehaus.jackson.node.JsonNodeFactory;import org.codehaus.jackson.xml.XmlMapper;import org.junit.After;import org.junit.Before;import org.junit.Test;import com.hoo.entity.AccountBean;/** * <b>function:</b>Jackson 将java对象转换成JSON字符串，也可以将JSON字符串转换成java对象 * jar-lib-version: jackson-all-1.6.2 * jettison-1.0.1 * @author hoojo * @createDate 2010-11-23 下午04:54:53 * @file JacksonTest.java * @package com.hoo.test * @project Spring3 * @blog http://blog.csdn.net/IBM_hoojo * @email hoojo_@126.com * @version 1.0 */@SuppressWarnings("unchecked")publicclass JacksonTest {private JsonGenerator jsonGenerator = null;private ObjectMapper objectMapper = null;private AccountBean bean = null;    @Beforepublicvoid init() {        bean = new AccountBean();        bean.setAddress("china-Guangzhou");        bean.setEmail("hoojo_@126.com");        bean.setId(1);        bean.setName("hoojo");        objectMapper = new ObjectMapper();try {            jsonGenerator = objectMapper.getJsonFactory().createJsonGenerator(System.out, JsonEncoding.UTF8);        } catch (IOException e) {            e.printStackTrace();        }    }    @Afterpublicvoid destory() {try {if (jsonGenerator != null) {                jsonGenerator.flush();            }if (!jsonGenerator.isClosed()) {                jsonGenerator.close();            }            jsonGenerator = null;            objectMapper = null;            bean = null;            System.gc();        } catch (IOException e) {            e.printStackTrace();        }    }}
3、 所需要的JavaEntity
package com.hoo.entity;publicclass AccountBean {privateint id;private String name;private String email;private String address;private Birthday birthday;//getter、setter    @Overridepublic String toString() {returnthis.name + "#" + this.id + "#" + this.address + "#" + this.birthday + "#" + this.email;    }}
Birthday
package com.hoo.entity;publicclass Birthday {private String birthday;public Birthday(String birthday) {super();this.birthday = birthday;    }//getter、setterpublic Birthday() {}    @Overridepublic String toString() {returnthis.birthday;    }}
**二、Java对象转换成JSON**
1、 JavaBean(Entity/Model)转换成JSON
/** * <b>function:</b>将java对象转换成json字符串 * @author hoojo * @createDate 2010-11-23 下午06:01:10 */@Testpublicvoid writeEntityJSON() {try {        System.out.println("jsonGenerator");//writeObject可以转换java对象，eg:JavaBean/Map/List/Array等        jsonGenerator.writeObject(bean);            System.out.println();        System.out.println("ObjectMapper");//writeValue具有和writeObject相同的功能        objectMapper.writeValue(System.out, bean);    } catch (IOException e) {        e.printStackTrace();    }}
运行后结果如下：
jsonGenerator{"address":"china-Guangzhou","name":"hoojo","id":1,"birthday":null,"email":"hoojo_@126.com"}ObjectMapper{"address":"china-Guangzhou","name":"hoojo","id":1,"birthday":null,"email":"hoojo_@126.com"}
上面分别利用JsonGenerator的writeObject方法和ObjectMapper的writeValue方法完成对Java对象的转换，二者传递的参数及构造的方式不同；JsonGenerator的创建依赖于ObjectMapper对象。也就是说如果你要使用JsonGenerator来转换JSON，那么你必须创建一个ObjectMapper。但是你用ObjectMapper来转换JSON，则不需要JSONGenerator。
objectMapper的writeValue方法可以将一个Java对象转换成JSON。这个方法的参数一，需要提供一个输出流，转换后可以通过这个流来输出转换后的内容。或是提供一个File，将转换后的内容写入到File中。当然，这个参数也可以接收一个JSONGenerator，然后通过JSONGenerator来输出转换后的信息。第二个参数是将要被转换的Java对象。如果用三个参数的方法，那么是一个Config。这个config可以提供一些转换时的规则，过指定的Java对象的某些属性进行过滤或转换等。
2、 将Map集合转换成Json字符串
/** * <b>function:</b>将map转换成json字符串 * @author hoojo * @createDate 2010-11-23 下午06:05:26 */@Testpublicvoid writeMapJSON() {try {        Map<String, Object> map = new HashMap<String, Object>();        map.put("name", bean.getName());        map.put("account", bean);        bean = new AccountBean();        bean.setAddress("china-Beijin");        bean.setEmail("hoojo@qq.com");        map.put("account2", bean);        System.out.println("jsonGenerator");        jsonGenerator.writeObject(map);        System.out.println("");        System.out.println("objectMapper");        objectMapper.writeValue(System.out, map);    } catch (IOException e) {        e.printStackTrace();    }}
转换后结果如下：
jsonGenerator{"account2":{"address":"china-Beijin","name":null,"id":0,"birthday":null,"email":"hoojo@qq.com"},"name":"hoojo","account":{"address":"china-Guangzhou","name":"hoojo","id":1,"birthday":null,"email":"hoojo_@126.com"}}objectMapper{"account2":{"address":"china-Beijin","name":null,"id":0,"birthday":null,"email":"hoojo@qq.com"},"name":"hoojo","account":{"address":"china-Guangzhou","name":"hoojo","id":1,"birthday":null,"email":"hoojo_@126.com"}}
3、 将List集合转换成json
/** * <b>function:</b>将list集合转换成json字符串 * @author hoojo * @createDate 2010-11-23 下午06:05:59 */@Testpublicvoid writeListJSON() {try {        List<AccountBean> list = new ArrayList<AccountBean>();        list.add(bean);        bean = new AccountBean();        bean.setId(2);        bean.setAddress("address2");        bean.setEmail("email2");        bean.setName("haha2");        list.add(bean);        System.out.println("jsonGenerator");//list转换成JSON字符串        jsonGenerator.writeObject(list);        System.out.println();        System.out.println("ObjectMapper");//用objectMapper直接返回list转换成的JSON字符串        System.out.println("1###" + objectMapper.writeValueAsString(list));        System.out.print("2###");//objectMapper list转换成JSON字符串        objectMapper.writeValue(System.out, list);    } catch (IOException e) {        e.printStackTrace();    }}
结果如下：
jsonGenerator[{"address":"china-Guangzhou","name":"hoojo","id":1,"birthday":null,"email":"hoojo_@126.com"},{"address":"address2","name":"haha2","id":2,"birthday":null,"email":"email2"}]ObjectMapper1###[{"address":"china-Guangzhou","name":"hoojo","id":1,"birthday":null,"email":"hoojo_@126.com"},{"address":"address2","name":"haha2","id":2,"birthday":null,"email":"email2"}]2###[{"address":"china-Guangzhou","name":"hoojo","id":1,"birthday":null,"email":"hoojo_@126.com"},{"address":"address2","name":"haha2","id":2,"birthday":null,"email":"email2"}]
外面就是多了个[]中括号；同样Array也可以转换，转换的JSON和上面的结果是一样的，这里就不再转换了。~.~
4、下面来看看jackson提供的一些类型，用这些类型完成json转换；如果你使用这些类型转换JSON的话，那么你即使没有JavaBean(Entity)也可以完成复杂的Java类型的JSON转换。下面用到这些类型构建一个复杂的Java对象，并完成JSON转换。
@Testpublicvoid writeOthersJSON() {try {        String[] arr = { "a", "b", "c" };        System.out.println("jsonGenerator");        String str = "hello world jackson!";//byte        jsonGenerator.writeBinary(str.getBytes());//boolean        jsonGenerator.writeBoolean(true);//null        jsonGenerator.writeNull();//float        jsonGenerator.writeNumber(2.2f);//char        jsonGenerator.writeRaw("c");//String        jsonGenerator.writeRaw(str, 5, 10);//String        jsonGenerator.writeRawValue(str, 5, 5);//String        jsonGenerator.writeString(str);        jsonGenerator.writeTree(JsonNodeFactory.instance.POJONode(str));        System.out.println();//Object        jsonGenerator.writeStartObject();//{        jsonGenerator.writeObjectFieldStart("user");//user:{        jsonGenerator.writeStringField("name", "jackson");//name:jackson        jsonGenerator.writeBooleanField("sex", true);//sex:true        jsonGenerator.writeNumberField("age", 22);//age:22        jsonGenerator.writeEndObject();//}        jsonGenerator.writeArrayFieldStart("infos");//infos:[        jsonGenerator.writeNumber(22);//22        jsonGenerator.writeString("this is array");//this is array        jsonGenerator.writeEndArray();//]        jsonGenerator.writeEndObject();//}        AccountBean bean = new AccountBean();        bean.setAddress("address");        bean.setEmail("email");        bean.setId(1);        bean.setName("haha");//complex Object        jsonGenerator.writeStartObject();//{        jsonGenerator.writeObjectField("user", bean);//user:{bean}        jsonGenerator.writeObjectField("infos", arr);//infos:[array]        jsonGenerator.writeEndObject();//}    } catch (Exception e) {        e.printStackTrace();    }}
运行后，结果如下：
jsonGenerator"aGVsbG8gd29ybGQgamFja3NvbiE=" true null 2.2c world jac  worl "hello world jackson!""hello world jackson!" {"user":{"name":"jackson","sex":true,"age":22},"infos":[22,"this is array"]} {"user":{"address":"address","name":"haha","id":1,"birthday":null,"email":"email"},"infos":["a","b","c"]}
怎么样？构造的json字符串和输出的结果是一致的吧。关键看懂用JSONGenerator提供的方法，完成一个Object的构建。
**三、JSON转换成Java对象**
1、 将json字符串转换成JavaBean对象
@Testpublicvoid readJson2Entity() {    String json = "{\"address\":\"address\",\"name\":\"haha\",\"id\":1,\"email\":\"email\"}";try {        AccountBean acc = objectMapper.readValue(json, AccountBean.class);        System.out.println(acc.getName());        System.out.println(acc);    } catch (JsonParseException e) {        e.printStackTrace();    } catch (JsonMappingException e) {        e.printStackTrace();    } catch (IOException e) {        e.printStackTrace();    }}
很简单，用到了ObjectMapper这个对象的readValue这个方法，这个方法需要提供2个参数。第一个参数就是解析的JSON字符串，第二个参数是即将将这个JSON解析吃什么Java对象，Java对象的类型。当然，还有其他相同签名方法，如果你有兴趣可以一一尝试使用方法，当然使用的方法和当前使用的方法大同小异。运行后，结果如下：
hahahaha#1#address#null#email
2、 将json字符串转换成List<Map>集合
/** * <b>function:</b>json字符串转换成list<map> * @author hoojo * @createDate 2010-11-23 下午06:12:01 */@Testpublicvoid readJson2List() {    String json = "[{\"address\": \"address2\",\"name\":\"haha2\",\"id\":2,\"email\":\"email2\"},"+"{\"address\":\"address\",\"name\":\"haha\",\"id\":1,\"email\":\"email\"}]";try {        List<LinkedHashMap<String, Object>> list = objectMapper.readValue(json, List.class);        System.out.println(list.size());for (int i = 0; i < list.size(); i++) {            Map<String, Object> map = list.get(i);            Set<String> set = map.keySet();for (Iterator<String> it = set.iterator();it.hasNext();) {                String key = it.next();                System.out.println(key + ":" + map.get(key));            }        }    } catch (JsonParseException e) {        e.printStackTrace();    } catch (JsonMappingException e) {        e.printStackTrace();    } catch (IOException e) {        e.printStackTrace();    }}
尝试过将上面的JSON转换成List，然后List中存放AccountBean，但结果失败了。但是支持Map集合。因为你转成List.class，但是不知道List存放何种类型。只好默然Map类型。因为所有的对象都可以转换成Map结合，运行后结果如下：
2address:address2name:haha2id:2email:email2address:addressname:hahaid:1email:email
3、 Json字符串转换成Array数组，由于上面的泛型转换不能识别到集合中的对象类型。所有这里用对象数组，可以解决这个问题。只不过它不再是集合，而是一个数组。当然这个不重要，你可以用Arrays.asList将其转换成List即可。
/** * <b>function:</b>json字符串转换成Array * @author hoojo * @createDate 2010-11-23 下午06:14:01 */@Testpublicvoid readJson2Array() {    String json = "[{\"address\": \"address2\",\"name\":\"haha2\",\"id\":2,\"email\":\"email2\"},"+"{\"address\":\"address\",\"name\":\"haha\",\"id\":1,\"email\":\"email\"}]";try {        AccountBean[] arr = objectMapper.readValue(json, AccountBean[].class);        System.out.println(arr.length);for (int i = 0; i < arr.length; i++) {            System.out.println(arr[i]);        }    } catch (JsonParseException e) {        e.printStackTrace();    } catch (JsonMappingException e) {        e.printStackTrace();    } catch (IOException e) {        e.printStackTrace();    }}
运行后的结果：
2haha2#2#address2#null#email2haha#1#address#null#email
4、 Json字符串转换成Map集合
/** * <b>function:</b>json字符串转换Map集合 * @author hoojo * @createDate Nov 27, 2010 3:00:06 PM */@Testpublicvoid readJson2Map() {    String json = "{\"success\":true,\"A\":{\"address\": \"address2\",\"name\":\"haha2\",\"id\":2,\"email\":\"email2\"},"+"\"B\":{\"address\":\"address\",\"name\":\"haha\",\"id\":1,\"email\":\"email\"}}";try {        Map<String, Map<String, Object>> maps = objectMapper.readValue(json, Map.class);        System.out.println(maps.size());        Set<String> key = maps.keySet();        Iterator<String> iter = key.iterator();while (iter.hasNext()) {            String field = iter.next();            System.out.println(field + ":" + maps.get(field));        }    } catch (JsonParseException e) {        e.printStackTrace();    } catch (JsonMappingException e) {        e.printStackTrace();    } catch (IOException e) {        e.printStackTrace();    }}
运行后结果如下：
3success:trueA:{address=address2, name=haha2, id=2, email=email2}B:{address=address, name=haha, id=1, email=email}
**四、Jackson对XML的支持**
Jackson也可以完成java对象到xml的转换，转换后的结果要比json-lib更直观，不过它依赖于stax2-api.jar这个jar包。
/** * <b>function:</b>java对象转换成xml文档 * 需要额外的jar包 stax2-api.jar * @author hoojo * @createDate 2010-11-23 下午06:11:21 */@Testpublicvoid writeObject2Xml() {//stax2-api-3.0.2.jar    System.out.println("XmlMapper");    XmlMapper xml = new XmlMapper();try {//javaBean转换成xml//xml.writeValue(System.out, bean);        StringWriter sw = new StringWriter();        xml.writeValue(sw, bean);        System.out.println(sw.toString());//List转换成xml        List<AccountBean> list = new ArrayList<AccountBean>();        list.add(bean);        list.add(bean);        System.out.println(xml.writeValueAsString(list));//Map转换xml文档        Map<String, AccountBean> map = new HashMap<String, AccountBean>();        map.put("A", bean);        map.put("B", bean);        System.out.println(xml.writeValueAsString(map));    } catch (JsonGenerationException e) {        e.printStackTrace();    } catch (JsonMappingException e) {        e.printStackTrace();    } catch (IOException e) {        e.printStackTrace();    }}
运行上面的方法，结果如下：
XmlMapper<unknown><address>china-Guangzhou</address><name>hoojo</name><id>1</id><birthday/><email>hoojo_@126.com</email></unknown><unknown><unknown><address>china-Guangzhou</address><name>hoojo</name><id>1</id><birthday/><email>hoojo_@126.com</email></unknown><email><address>china-Guangzhou</address><name>hoojo</name><id>1</id><birthday/><email>hoojo_@126.com</email></email></unknown><unknown><A><address>china-Guangzhou</address><name>hoojo</name><id>1</id><birthday/><email>hoojo_@126.com</email></A><B><address>china-Guangzhou</address><name>hoojo</name><id>1</id><birthday/><email>hoojo_@126.com</email></B></unknown>
看结果，根节点都是unknown 这个问题还没有解决，由于根节点没有转换出来，所有导致解析xml到Java对象，也无法完成。
