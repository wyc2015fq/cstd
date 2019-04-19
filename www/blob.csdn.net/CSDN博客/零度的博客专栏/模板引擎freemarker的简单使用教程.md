# 模板引擎freemarker的简单使用教程 - 零度的博客专栏 - CSDN博客
2016年10月14日 14:02:06[零度anngle](https://me.csdn.net/zmx729618)阅读数：1374
      freemarker十分强大，而且不依赖web容器，个人感觉十分好用。下面直接进主题，freemarker还有什么特性，请找度娘或谷哥，下面实现通过freemarker生成word的例子。
### 1、创建模板
我创建模板的方法比较简单，首先是新建一个word文档，按照内容格式排好版，然后在需要注入信息的位置先写上占位置的数据，如图1，然后另存为xml文件（我是存为2003版本的xml），然后用文本编辑器把xml打开，在xml中把对应的数据改为freemarker的输出表达式，如图2，然后保存，把xml的后缀名改为freemarker的文件后缀名**ftl**，便是一个freemarker模板了。如下图
图1：word文档：
![](https://img-blog.csdn.net/20140518223309250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc3Rvcm13eQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
图2：通过word改写的freemarker模板：
![](https://img-blog.csdn.net/20140518223739531)
### 2、代码实现
**[java]**[view
 plain](http://blog.csdn.net/stormwy/article/details/26172353#)[copy](http://blog.csdn.net/stormwy/article/details/26172353#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/352307/fork)
- import java.io.BufferedWriter;  
- import java.io.File;  
- import java.io.FileNotFoundException;  
- import java.io.FileOutputStream;  
- import java.io.IOException;  
- import java.io.OutputStreamWriter;  
- import java.io.UnsupportedEncodingException;  
- import java.io.Writer;  
- import java.text.SimpleDateFormat;  
- import java.util.Date;  
- import java.util.HashMap;  
- import java.util.Map;  
- 
- import freemarker.template.Configuration;  
- import freemarker.template.Template;  
- import freemarker.template.TemplateException;  
- 
- /**
-  * 使用freemark生成word
-  *
-  */
- publicclass Freemark {  
- 
- publicstaticvoid main(String[] args){  
-         Freemark freemark = new Freemark("template/");  
-         freemark.setTemplateName("wordTemplate.ftl");  
-         freemark.setFileName("doc_"+new SimpleDateFormat("yyyy-MM-dd hh-mm-ss").format(new Date())+".doc");  
-         freemark.setFilePath("bin\\doc\\");  
- //生成word
-         freemark.createWord();  
-     }  
- 
- privatevoid createWord(){  
- 
-         Template t = null;  
- try {  
- //获取模板信息
-             t = configuration.getTemplate(templateName);  
-         } catch (IOException e) {  
-             e.printStackTrace();  
-         }  
- 
-         File outFile = new File(filePath+fileName);  
-         Writer out = null;  
- try {  
-             out = new BufferedWriter(new OutputStreamWriter(new FileOutputStream(outFile), "UTF-8"));  
-         } catch (UnsupportedEncodingException e) {  
-             e.printStackTrace();  
-         } catch (FileNotFoundException e) {  
-             e.printStackTrace();  
-         }  
- 
-         Map map = new HashMap<String, Object>();  
-         map.put("name", "蒙奇·D·路飞");  
-         map.put("country", "日本");  
-         map.put("city", "东京");  
-         map.put("time",new SimpleDateFormat("yyyy-MM-dd hh-mm-ss").format(new Date()));  
- try {  
- //输出数据到模板中，生成文件。
-             t.process(map, out);  
-             out.close();  
-         } catch (TemplateException e) {  
-             e.printStackTrace();  
-         } catch (IOException e) {  
-             e.printStackTrace();  
-         }  
- 
-     }  
- /**
-      * freemark初始化
-      * @param templatePath 模板文件位置
-      */
- public Freemark(String templatePath) {  
-         configuration = new Configuration();  
-         configuration.setDefaultEncoding("utf-8");  
-         configuration.setClassForTemplateLoading(this.getClass(),templatePath);       
-     }     
- /**
-      * freemark模板配置
-      */
- private Configuration configuration;  
- /**
-      * freemark模板的名字
-      */
- private String templateName;  
- /**
-      * 生成文件名
-      */
- private String fileName;  
- /**
-      * 生成文件路径
-      */
- private String filePath;  
- 
- public String getFileName() {  
- return fileName;  
-     }  
- 
- publicvoid setFileName(String fileName) {  
- this.fileName = fileName;  
-     }  
- 
- public String getFilePath() {  
- return filePath;  
-     }  
- 
- publicvoid setFilePath(String filePath) {  
- this.filePath = filePath;  
-     }  
- 
- public String getTemplateName() {  
- return templateName;  
-     }  
- 
- publicvoid setTemplateName(String templateName) {  
- this.templateName = templateName;  
-     }  
- 
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
      程序运行后，会在bin的doc目录下生成doc文件，效果图：
![](https://img-blog.csdn.net/20140518224217640)
![](https://img-blog.csdn.net/20140518224224000)
