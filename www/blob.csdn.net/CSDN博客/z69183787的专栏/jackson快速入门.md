# jackson快速入门 - z69183787的专栏 - CSDN博客
2014年12月19日 14:50:18[OkidoGreen](https://me.csdn.net/z69183787)阅读数：711
个人分类：[Json转换-Jackson](https://blog.csdn.net/z69183787/article/category/2750399)
**1. 背景**
目前维护的产品使用jackson处理json，现整理一下jackson相关资料，希望对初次接触jackson的新手有所帮助。
jackson主页: [http://jackson.codehaus.org/](http://jackson.codehaus.org/)
jackson document: [http://wiki.fasterxml.com/JacksonJavaDocs](http://wiki.fasterxml.com/JacksonJavaDocs)
JacksonInFiveMinutes: [http://wiki.fasterxml.com/JacksonInFiveMinutes](http://wiki.fasterxml.com/JacksonInFiveMinutes)
本文主要内容译自JacksonInFiveMinutes，增加了部分示例，转载请注明出处。
受java平台上各种处理xml的类库(StAX, JAXB等)启发，Jackson提供一种处理json的java类库。Jackson的目标是为开发者提供快速、准确、轻量级和用起来最爽的json处理类库。本文将概括介绍Jackson的主要功能和相关功能的使用示例。
**2.  使用方式**
Jackson提供三种可选的json处理方式：
**1） Streaming API**
又称Incremental parsing/generation， 受StAX API启发，以非关联递增方式读写json内容。 通过org.codehaus.jackson.JsonParser读取，通过org.codehaus.jackson.JsonGenerator写入。
**2） Tree Model**
通过基于内存的树形结构来描述json数据，和XML
 DOM类似。通过org.codehaus.jackson.map.ObjectMapper构建树，树由JsonNode节点组成。
**3）Data Binding**
基于属性访问或注解的方式将json和POJO对象互相转换， 受JAXB基于注解的处理方式启发。通过org.codehaus.jackson.map.ObjectMapper读写json数据。它包含两种类型:
3.1 Simple Data Binding
用于json和Java Maps, Lists, Strings, Numbers, Booleans and nulls的相互转换。
3.2 Full Data Binding
用于json和Java Bean的相互转换。
下面从使用的角度上比较三种处理方式：
**Streaming API **执行效率最高，读写速度最快，另外两种方式都基于它实现；
**Tree Model **是最灵活的；
**Data Binding **通常是最方便使用的；
**3.  示例**
快速上手最好的方式就是看示例，下面的例子演示了上面三种方式的用法，建议通过附件下载完整的示例工程文件。
Java代码  ![收藏代码](http://learnworld.iteye.com/images/icon_star.png)
- package com.learnworld.jackson.main;  
- 
- import java.io.File;  
- import java.util.Date;  
- import java.util.HashMap;  
- import java.util.Map;  
- 
- import org.codehaus.jackson.JsonEncoding;  
- import org.codehaus.jackson.JsonFactory;  
- import org.codehaus.jackson.JsonGenerator;  
- import org.codehaus.jackson.JsonNode;  
- import org.codehaus.jackson.JsonParser;  
- import org.codehaus.jackson.JsonToken;  
- import org.codehaus.jackson.map.ObjectMapper;  
- import org.codehaus.jackson.node.ObjectNode;  
- import org.codehaus.jackson.type.TypeReference;  
- 
- import com.learnworld.jackson.annotation.Account;  
- import com.learnworld.jackson.pojo.Name;  
- import com.learnworld.jackson.pojo.User;  
- import com.learnworld.jackson.pojo.User.Gender;  
- 
- publicclass JacksonMain {  
- 
- /**
-      * @param args
-      * @throws Exception
-      */
- publicstaticvoid main(String[] args) throws Exception {  
-         ObjectMapper mapper = new ObjectMapper();  
- 
- // 1.1 Simple Data Binding
-         simpleDataBinding(mapper);  
- 
- // 1.2 Full Data Binding
-         fullDataBinding(mapper);  
- 
- // 1.3 Data Binding with Generics
-         dataBindingWithGenerics(mapper);  
- 
- // 1.4 Data Binding with Annotation
-         dataBindingWithAnnotation(mapper);  
- 
- // 2.1 Tree Model
-         treeModel(mapper);  
- 
- // 2.2 Construct a Tree
-         constructTreeModel(mapper);  
- 
- // 3.1 Streaming API (write Json)
-         streamingAPIWrite();  
- 
- // 3.2 Streaming API (read Json)
-         streamingAPIRead();  
-     }  
- 
- @SuppressWarnings("unchecked")  
- publicstaticvoid simpleDataBinding(ObjectMapper mapper) throws Exception {  
- // json -> Map
-         Map<String, Object> userDataRead = mapper.readValue(new File("user.json"),  
-                 Map.class);  
-         System.out.println("simpleDataBinding(): " + userDataRead);  
- 
- // Map -> json
-         Map<String,Object> userData = new HashMap<String,Object>();  
-         Map<String,String> nameStruct = new HashMap<String,String>();  
-         nameStruct.put("first", "Joe");  
-         nameStruct.put("last", "Sixpack");  
-         userData.put("name", nameStruct);  
-         userData.put("gender", "MALE");  
-         userData.put("verified", Boolean.FALSE);  
-         userData.put("userImage", "Rm9vYmFyIQ==");  
-         mapper.writeValue(new File("user-modified.json"), userData);  
-     }  
- 
- publicstaticvoid fullDataBinding(ObjectMapper mapper) throws Exception {  
- // json -> Object
-         User user = mapper.readValue(new File("user.json"), User.class);  
-         System.out.println("fullDataBinding(): " + user);  
- 
- // Object -> json
-         mapper.writeValue(new File("user-modified.json"), user);  
-     }  
- 
- publicstaticvoid dataBindingWithGenerics(ObjectMapper mapper) throws Exception {  
- // json -> Map
-         Map<String, Name> genericData = mapper.readValue(new File("generic.json"),  
- new TypeReference<Map<String,Name>>() { });  
-         System.out.println("dataBindingWithGenerics():" + genericData);  
-     }  
- 
- publicstaticvoid dataBindingWithAnnotation(ObjectMapper mapper) throws Exception {  
- // json -> Object
-         Account account = mapper.readValue(new File("account.json"), Account.class);  
-         System.out.println("dataBindingWithAnnotation(): " + account);  
- 
-         account.setGmtCreate(new Date());  
- // Object -> json
-         mapper.writeValue(new File("account.json"), account);  
-     }  
- 
- publicstaticvoid treeModel(ObjectMapper mapper) throws Exception {  
- // can either use mapper.readTree(JsonParser), or bind to JsonNode
-         JsonNode rootNode = mapper.readValue(new File("user.json"), JsonNode.class);  
- 
- // ensure that "last name" isn't "Xmler"; if is, change to "Jsoner"
-         JsonNode nameNode = rootNode.path("name");  
-         String lastName = nameNode.path("last").getTextValue();  
- if ("xmler".equalsIgnoreCase(lastName)) {  
-           ((ObjectNode)nameNode).put("last", "Jsoner");  
-         }  
- // write it out
-         mapper.writeValue(new File("user-modified.json"), rootNode);  
-     }  
- 
- publicstaticvoid constructTreeModel(ObjectMapper mapper) throws Exception {  
-         ObjectMapper objectMapper = new ObjectMapper();  
-         ObjectNode userOb = objectMapper.createObjectNode();  
-         ObjectNode nameOb = userOb.putObject("name");  
-         nameOb.put("first", "Thomas");  
-         nameOb.put("last", "Zhou");  
-         userOb.put("gender", User.Gender.MALE.toString());  
-         userOb.put("verified", false);  
-         userOb.put("userImage", "Foobar!".getBytes());  
- // write it out
-         mapper.writeValue(new File("user-modified.json"), userOb);  
-     }  
- 
- publicstaticvoid streamingAPIRead() throws Exception {  
-         JsonFactory f = new JsonFactory();  
-         JsonGenerator g = f.createJsonGenerator(new File("user.json"), JsonEncoding.UTF8);  
- 
-         g.writeStartObject();  
-         g.writeObjectFieldStart("name");  
-         g.writeStringField("first", "Thomas");  
-         g.writeStringField("last", "Zhou");  
-         g.writeEndObject(); // for field 'name'
-         g.writeStringField("gender", Gender.MALE.name());  
-         g.writeBooleanField("verified", false);  
-         g.writeFieldName("userImage"); // no 'writeBinaryField' (yet?)
- byte[] binaryData = "Foobar!".getBytes();  
-         g.writeBinary(binaryData);  
-         g.writeEndObject();  
-         g.close(); // important: will force flushing of output, close underlying output stream
-     }  
- 
- publicstaticvoid streamingAPIWrite() throws Exception {  
-         JsonFactory f = new JsonFactory();  
-         JsonParser jp = f.createJsonParser(new File("user.json"));  
-         User user = new User();  
-         jp.nextToken(); // will return JsonToken.START_OBJECT (verify?)
- while (jp.nextToken() != JsonToken.END_OBJECT) {  
-           String fieldname = jp.getCurrentName();  
-           jp.nextToken(); // move to value, or START_OBJECT/START_ARRAY
- if ("name".equals(fieldname)) { // contains an object
-             Name name = new Name();  
- while (jp.nextToken() != JsonToken.END_OBJECT) {  
-               String namefield = jp.getCurrentName();  
-               jp.nextToken(); // move to value
- if ("first".equals(namefield)) {  
-                 name.setFirst(jp.getText());  
-               } elseif ("last".equals(namefield)) {  
-                 name.setLast(jp.getText());  
-               } else {  
- thrownew IllegalStateException("Unrecognized field '"+fieldname+"'!");  
-               }  
-             }  
-             user.setName(name);  
-           } elseif ("gender".equals(fieldname)) {  
-             user.setGender(User.Gender.valueOf(jp.getText()));  
-           } elseif ("verified".equals(fieldname)) {  
-             user.setVerified(jp.getCurrentToken() == JsonToken.VALUE_TRUE);  
-           } elseif ("userImage".equals(fieldname)) {  
-             user.setUserImage(jp.getBinaryValue());  
-           } else {  
- thrownew IllegalStateException("Unrecognized field '"+fieldname+"'!");  
-           }  
-         }  
-         jp.close(); // ensure resources get cleaned up timely and properly
-         System.out.println("streamingAPIWrite(): " + user);  
-     }  
- }  
**4.  其他**
除了使用上文中演示的ObjectMapper方式使用data binding和tree model，以下方法也可以满足你的需求：
- 
JsonParser.readValueAs()
- 
JsonParser.readValueAsTree()
- 
JsonGenerator.writeObject()
- 
JsonGenerator.writeTree()
