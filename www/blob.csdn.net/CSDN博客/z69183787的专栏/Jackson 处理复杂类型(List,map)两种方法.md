# Jackson 处理复杂类型(List,map)两种方法 - z69183787的专栏 - CSDN博客
2018年10月30日 11:51:59[OkidoGreen](https://me.csdn.net/z69183787)阅读数：194
[https://blog.csdn.net/zhuyijian135757/article/details/38269715](https://blog.csdn.net/zhuyijian135757/article/details/38269715)
方法一:
String jsonString="[{'id':'1'},{'id':'2'}]";
ObjectMapper mapper = new ObjectMapper();
JavaType javaType = mapper.getTypeFactory().constructParametricType(List.class, Bean.class);
//如果是Map类型  mapper.getTypeFactory().constructParametricType(HashMap.class,String.class, Bean.class);
List<Bean> lst =  (List<Bean>)mapper.readValue(jsonString, javaType); 
方法二:
String jsonString="[{'id':'1'},{'id':'2'}]";
ObjectMapper mapper = new ObjectMapper();
List<Bean> beanList = mapper.readValue(jsonString, new TypeReference<List<Bean>>() {}); 
