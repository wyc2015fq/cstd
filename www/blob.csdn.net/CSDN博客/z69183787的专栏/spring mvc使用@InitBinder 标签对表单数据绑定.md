# spring mvc使用@InitBinder 标签对表单数据绑定 - z69183787的专栏 - CSDN博客
2015年01月06日 20:55:06[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1593
在SpringMVC中，bean中定义了Date，double等类型，如果没有做任何处理的话，日期以及double都无法绑定。
解决的办法就是使用spring mvc提供的@InitBinder标签
在我的项目中是在BaseController中增加方法initBinder，并使用注解@InitBinder标注，那么spring m[vc](http://www.2cto.com/kf/ware/vc/)在绑定表单之前，都会先注册这些编辑器，当然你如果不嫌麻烦，你也可以单独的写在你的每一个controller中。剩下的控制器都继承该类。spring自己提供了大量的实现类，诸如CustomDateEditor
 ，CustomBooleanEditor，CustomNumberEditor等许多，基本上够用。
当然，我们也可以不使用他自己自带这些编辑器类，那下面我们自己去构造几个
**[java]**[view
 plain](http://blog.csdn.net/axin66ok/article/details/17938095#)[copy](http://blog.csdn.net/axin66ok/article/details/17938095#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/145249/fork)
- import org.springframework.beans.propertyeditors.PropertiesEditor;  
- 
- publicclass DoubleEditor extends PropertiesEditor {    
- @Override
- publicvoid setAsText(String text) throws IllegalArgumentException {    
- if (text == null || text.equals("")) {    
-             text = "0";    
-         }    
-         setValue(Double.parseDouble(text));    
-     }    
- 
- @Override
- public String getAsText() {    
- return getValue().toString();    
-     }    
- }   
**[java]**[view
 plain](http://blog.csdn.net/axin66ok/article/details/17938095#)[copy](http://blog.csdn.net/axin66ok/article/details/17938095#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/145249/fork)
- import org.springframework.beans.propertyeditors.PropertiesEditor;  
- 
- publicclass IntegerEditor extends PropertiesEditor {    
- @Override
- publicvoid setAsText(String text) throws IllegalArgumentException {    
- if (text == null || text.equals("")) {    
-             text = "0";    
-         }    
-         setValue(Integer.parseInt(text));    
-     }    
- 
- @Override
- public String getAsText() {    
- return getValue().toString();    
-     }    
- }   
**[java]**[view
 plain](http://blog.csdn.net/axin66ok/article/details/17938095#)[copy](http://blog.csdn.net/axin66ok/article/details/17938095#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/145249/fork)
- import org.springframework.beans.propertyeditors.PropertiesEditor;  
- 
- publicclass FloatEditor extends PropertiesEditor {    
- @Override
- publicvoid setAsText(String text) throws IllegalArgumentException {    
- if (text == null || text.equals("")) {    
-             text = "0";    
-         }    
-         setValue(Float.parseFloat(text));    
-     }    
- 
- @Override
- public String getAsText() {    
- return getValue().toString();    
-     }    
- }    
**[java]**[view
 plain](http://blog.csdn.net/axin66ok/article/details/17938095#)[copy](http://blog.csdn.net/axin66ok/article/details/17938095#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/145249/fork)
- import org.springframework.beans.propertyeditors.PropertiesEditor;  
- 
- publicclass LongEditor extends PropertiesEditor {    
- @Override
- publicvoid setAsText(String text) throws IllegalArgumentException {    
- if (text == null || text.equals("")) {    
-             text = "0";    
-         }    
-         setValue(Long.parseLong(text));    
-     }    
- 
- @Override
- public String getAsText() {    
- return getValue().toString();    
-     }    
- }    
在BaseController中
**[java]**[view
 plain](http://blog.csdn.net/axin66ok/article/details/17938095#)[copy](http://blog.csdn.net/axin66ok/article/details/17938095#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/145249/fork)
- @InitBinder
- protectedvoid initBinder(WebDataBinder binder) {    
-        binder.registerCustomEditor(Date.class, new CustomDateEditor(new SimpleDateFormat("yyyy-MM-dd HH:mm:ss"), true));    
- /        binder.registerCustomEditor(int.class, new CustomNumberEditor(int.class, true));    
-        binder.registerCustomEditor(int.class, new IntegerEditor());    
- /        binder.registerCustomEditor(long.class, new CustomNumberEditor(long.class, true));  
-        binder.registerCustomEditor(long.class, new LongEditor());    
-        binder.registerCustomEditor(double.class, new DoubleEditor());    
-        binder.registerCustomEditor(float.class, new FloatEditor());    
-    }   
**[java]**[view
 plain](http://blog.csdn.net/axin66ok/article/details/17938095#)[copy](http://blog.csdn.net/axin66ok/article/details/17938095#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/145249/fork)
- publicclass org.springframework.beans.propertyeditors.PropertiesEditor extends java.beans.PropertyEditorSupport {  
看到没？如果你的编辑器类直接继承PropertyEditorSupport也可以。
先就这样吧，洗洗啪啪啪。。。
