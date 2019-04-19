# ResourceBundle.getBundle("baseName")的使用 - 零度的博客专栏 - CSDN博客
2016年05月17日 14:24:15[零度anngle](https://me.csdn.net/zmx729618)阅读数：5259
1、使用ResourceBundle访问本地资源    
        在设计时，我们往往需要访问一些适合本地修改的配置信息，如果作为静态变量，那么每次修改都需要重新编译一个class，.config保存此类信息并不适合，这时我们需要ResourceBundle。   通过ResourceBundle，我们需要访问位于/WEB-INF/classes目录下的一个后缀名为properties的文本类型文件，从里面读取我们需要的值。 
```
public static String getValueByKey(String propertyBaseName, String key) {
		Locale locale = Locale.getDefault();    
		ResourceBundle localResource = ResourceBundle.getBundle(propertyBaseName, locale);    
		String value = localResource.getString(key);    
		return value;
	}
```
       使用：System.out.println(PropertyUtil.getValueByKey("propertyBaseName.properties","test"));
       这里对应了/WEB-INF/class/propertyBaseName.properties文件内容为： test=hello world，   则打印出来的结果就是hello world。        
       请注意，这里我们可以利用Locale和ResourceBundle的这个组合创建国际化的java程序。我们可以把locale实例化为new Locale("zh","CN");    通过ResourceBundle.getBundle("MessagesBundle", locale);    系统将自动寻找MessagesBundle_zh_CN，即定义为中国大陆地区简体中文。如果没有该文件，则会依次寻找MessagesBundle_zh，MessagesBundle，直到找到为止。
2、读取国际化文件
         通过此类，可以使您所编写的程序可以：轻松地本地化或翻译成不同的语言； 一次处理多个语言环境；以后可以轻松地进行修改，支持更多的语言环境
         说的简单点，这个类的作用就是读取资源属性文件（properties），然后根据.properties文件的名称信息（本地化信息），匹配当前系统的国别语言信息（也可以程序指定），然后获取相应的properties文件的内容。
         使用这个类，要注意的一点是，这个properties文件的名字是有规范的：一般的命名规范是： 自定义名_语言代码_国别代码.properties，
如果是默认的，直接写为：自定义名.properties。
        比如：
                message_en_US.properties
                message_zh_CN.properties
                message.properties
        当在中文操作系统下，如果message_zh_CN.properties、mesage.properties两个文件都存在，则优先会使用myres_zh_CN.properties，当myres_zh_CN.properties不存在时候，会使用默认的myres.properties。没有提供语言和地区的资源文件是系统默认的资源文件。
         注意：资源文件都必须是ISO-8859-1编码，因此，对于所有非西方语系的处理，都必须先将之转换为Java Unicode Escape格式。转换方法是通过JDK自带的工具native2ascii.
        实例：定义三个资源文件，放到src的根目录下面（必须这样，或者你放到自己配置的calsspath下面。
```
1、myres.properties
aaa=good 
bbb=thanks
```
```
2、myres_en_US.properties
aaa=good 
bbb=thanks
```
```
3、myres_zh_CN.properties
aaa=\u597d 
bbb=\u591a\u8c22
```
` `
```
import java.util.Locale; 
import java.util.ResourceBundle; 
/** 
* 国际化资源绑定测试 
* @author 
*/ 
public class TestResourceBundle { 
        public static void main(String[] args) { 
                Locale locale1 = new Locale("zh", "CN"); 
                ResourceBundle resb1 = ResourceBundle.getBundle("myres", locale1); 
                System.out.println(resb1.getString("aaa")); 
                ResourceBundle resb2 = ResourceBundle.getBundle("myres", Locale.getDefault()); 
                System.out.println(resb1.getString("aaa")); 
                Locale locale3 = new Locale("en", "US"); 
                ResourceBundle resb3 = ResourceBundle.getBundle("myres", locale3); 
                System.out.println(resb3.getString("aaa")); 
        } 
}
```
        运行结果：
```
好 
好 
good
```
        如果使用默认的Locale，那么在英文操作系统上，会选择myres_en_US.properties或myres.properties资源文件。
3、认识Locale
        Locale 对象表示了特定的地理、政治和文化地区。需要 Locale 来执行其任务的操作称为语言环境敏感的 操作，它使用 Locale 为用户量身定制信息。例如，显示一个数值就是语言环境敏感的操作，应该根据用户的国家、地区或文化的风俗/传统来格式化该数值。
       使用此类中的构造方法来创建 Locale：
 Locale(String language)
 Locale(String language, String country)
 Locale(String language, String country, String variant)
       创建完 Locale 后，就可以查询有关其自身的信息。使用 getCountry 可获取 ISO 国家代码，使用 getLanguage 则获取 ISO 语言代码。可用使用 getDisplayCountry 来获取适合向用户显示的国家名。同样，可用使用 getDisplayLanguage 来获取适合向用户显示的语言名。有趣的是，getDisplayXXX 方法本身是语言环境敏感的，它有两个版本：一个使用默认的语言环境作为参数，另一个则使用指定的语言环境作为参数。
