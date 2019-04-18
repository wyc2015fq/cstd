# EL表达式取Map,List值的总结 - z69183787的专栏 - CSDN博客
2014年04月17日 15:55:04[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1352

**[java]**[view
 plain](http://blog.csdn.net/han_dongwei/article/details/7988386#)[copy](http://blog.csdn.net/han_dongwei/article/details/7988386#)[print](http://blog.csdn.net/han_dongwei/article/details/7988386#)[?](http://blog.csdn.net/han_dongwei/article/details/7988386#)
- 
EL表达式取Map中的值：
后台action 中：
**[java]**[view
 plain](http://blog.csdn.net/han_dongwei/article/details/7988386#)[copy](http://blog.csdn.net/han_dongwei/article/details/7988386#)[print](http://blog.csdn.net/han_dongwei/article/details/7988386#)[?](http://blog.csdn.net/han_dongwei/article/details/7988386#)
- Map map = new HashMap();  
- map.put(key1,value1);  
- map.put(key2,value2);  
- map.put(key3,value3);  
前台jsp 中使用EL获取Map：
1 ：  ${map[key1]}   ---- 直接取map中key=key1 的value;  例：  map .put("a","b"),  ${map["a"]}  就可以
注意：如果key1 是数值，例如; 1
后台 map.put(1, value1) , 前台 ${map[1]}将取不到值。原因：el表达式中数字1是Long类型，无法匹配后台map中的int。 修改 map.put(0L, value);  前台 ：${map[1]}.
2 
**[java]**[view
 plain](http://blog.csdn.net/han_dongwei/article/details/7988386#)[copy](http://blog.csdn.net/han_dongwei/article/details/7988386#)[print](http://blog.csdn.net/han_dongwei/article/details/7988386#)[?](http://blog.csdn.net/han_dongwei/article/details/7988386#)
- <forEachitems="${map}" var="item">  
- <c:outvalue="${item.key}"/>  
- <c:outvalue="${item.value}"/>  
- </forEach>  
## EL 表达式取List的值：
后台action ：
**[java]**[view
 plain](http://blog.csdn.net/han_dongwei/article/details/7988386#)[copy](http://blog.csdn.net/han_dongwei/article/details/7988386#)[print](http://blog.csdn.net/han_dongwei/article/details/7988386#)[?](http://blog.csdn.net/han_dongwei/article/details/7988386#)
- List list = new ArrayList();   
-          list.add("abc");  
-           list.add("123");  
-           list.add("haha");  
-           list.add("hehe");  
前台jsp中：
${list[0]}，         ${list[1]}，          ${list[2]}，          ${list[3]}
## 练习例子：
**[java]**[view
 plain](http://blog.csdn.net/han_dongwei/article/details/7988386#)[copy](http://blog.csdn.net/han_dongwei/article/details/7988386#)[print](http://blog.csdn.net/han_dongwei/article/details/7988386#)[?](http://blog.csdn.net/han_dongwei/article/details/7988386#)
- List<Map<String,Object>> students = new ArrayList<Map<String,Object>>();   
-         Map<String,Object> s1 = new HashMap<String,Object>();   
-         s1.put("name","jim");   
-         s1.put("age","15");   
-         students.add(s1);   
-            Map<String,Object> s2 = new HashMap<String,Object>();   
-         s2.put("name","lucy");   
-         s2.put("age","12");   
-         students.add(s2);   
前台：
**[java]**[view
 plain](http://blog.csdn.net/han_dongwei/article/details/7988386#)[copy](http://blog.csdn.net/han_dongwei/article/details/7988386#)[print](http://blog.csdn.net/han_dongwei/article/details/7988386#)[?](http://blog.csdn.net/han_dongwei/article/details/7988386#)
- <c:forEach var="student" items="$ {students}" >   
-           <tr>   
-           <span style="white-space:pre">    </span><td> <c:out value="$ {student.name}"default="wang"/> </td>   
-          <span style="white-space:pre"> </span><td> <c:out value="$ {student.age}"default="wang"/> </td>   
-           </tr>   
-  </c:forEach>   
- 
