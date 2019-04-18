# spring boot 使用FreeMarker模板 - z69183787的专栏 - CSDN博客
2017年06月28日 16:55:39[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2128
在[spring](http://lib.csdn.net/base/javaee) boot中使用FreeMarker模板非常简单方便，只需要简单几步就行：
1、引入依赖：
**[html]**[view
 plain](http://blog.csdn.net/clementad/article/details/51942629#)[copy](http://blog.csdn.net/clementad/article/details/51942629#)
- <dependency>
- <groupId>org.springframework.boot</groupId>
- <artifactId>spring-boot-starter-freemarker</artifactId>
- </dependency>
2、创建模板：
**[html]**[view
 plain](http://blog.csdn.net/clementad/article/details/51942629#)[copy](http://blog.csdn.net/clementad/article/details/51942629#)
- <!DOCTYPE html>
- <html>
- <body>
- <h4>亲爱的${toUserName}，你好！</h4>
- 
- <pstyle="color:blue;"> ${message}</p>
- 
- 祝：开心！  
- </br>
- ${fromUserName}  
- </br>
- ${time?date}  
- 
- </body>
- </html>
其中，${time?date}表示time是日期类型的变量，只取date部分。“?date”还可以使用“?datetime”或“?time”。
3、使用模板，测试用例：
**[java]**[view
 plain](http://blog.csdn.net/clementad/article/details/51942629#)[copy](http://blog.csdn.net/clementad/article/details/51942629#)
- @Autowired
- Configuration configuration; //freeMarker configuration
- 
- @Test
- publicvoid sendHtmlMailUsingFreeMarker() throws Exception {  
-     Map<String, Object> model = new HashMap<String, Object>();  
-     model.put("time", new Date());  
-     model.put("message", "这是测试的内容。。。");  
-     model.put("toUserName", "张三");  
-     model.put("fromUserName", "老许");  
- 
-     Template t = configuration.getTemplate("welcome.ftl"); // freeMarker template
-     String content = FreeMarkerTemplateUtils.processTemplateIntoString(t, model);  
- 
-     logger.debug(content);  
- //mailService.sendHtmlMail(to, "主题：html邮件", content);
- }  
4、测试结果：
![](https://img-blog.csdn.net/20160718155242465?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
源代码参考：[https://github.com/xujijun/my-spring-boot](https://github.com/xujijun/my-spring-boot)
FreeMarker官网：[http://freemarker.org/](http://freemarker.org/)
**附：**使用循环遍历一个List的模板：
**[html]**[view
 plain](http://blog.csdn.net/clementad/article/details/51942629#)[copy](http://blog.csdn.net/clementad/article/details/51942629#)
- <html>
- <body>
- <h3>发现错误！<ahref="${errorLogLink}"target="_blank">点击这里查看详情</a></h3>
- 
- <h3>错误列表：</h3>
- 
- <tableborder="1px solid #8968CD"style="border-collapse: collapse;"><tr><th>错误位置</th><th>数量</th><th>错误信息</th><th>错误类名</th><th>更多信息</th></tr>
- <#list errorList as error>
- <tr>
- <td>${error.pos}</td>
- <td>${error.count}</td>
- <td>${error.msg}</td>
- <td>${error.eName!}</td>
- <td>${error.details!}</td>
- </tr>
- </#list>
- </table>
- 
- </body>
- </html>
注意：最后那个两个感叹号表示：如果error.eName/error.details的值为空，则用空格代替。感叹号后面也可以加一个缺省字符串，在空值的时候代替空值。如果不加感叹号会报错。
