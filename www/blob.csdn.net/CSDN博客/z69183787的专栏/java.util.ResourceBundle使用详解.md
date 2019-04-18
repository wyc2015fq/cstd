# java.util.ResourceBundle使用详解 - z69183787的专栏 - CSDN博客
2012年11月06日 16:46:27[OkidoGreen](https://me.csdn.net/z69183787)阅读数：911
java.util.ResourceBundle使用详解
一、认识国际化资源文件
这个类提供软件国际化的捷径。通过此类，可以使您所编写的程序可以：
         轻松地本地化或翻译成不同的语言
         一次处理多个语言环境
         以后可以轻松地进行修改，支持更多的语言环境
说的简单点，这个类的作用就是读取资源属性文件（properties），然后根据.properties文件的名称信息（本地化信息），匹配当前系统的国别语言信息（也可以程序指定），然后获取相应的properties文件的内容。
使用这个类，要注意的一点是，这个properties文件的名字是有规范的：一般的命名规范是： 自定义名_语言代码_国别代码.properties，
如果是默认的，直接写为：自定义名.properties
比如：
myres_en_US.properties
myres_zh_CN.properties
myres.properties
当在中文操作系统下，如果myres_zh_CN.properties、myres.properties两个文件都存在，则优先会使用myres_zh_CN.properties，当myres_zh_CN.properties不存在时候，会使用默认的myres.properties。
没有提供语言和地区的资源文件是系统默认的资源文件。
资源文件都必须是ISO-8859-1编码，因此，对于所有非西方语系的处理，都必须先将之转换为Java Unicode Escape格式。转换方法是通过JDK自带的工具native2ascii.
二、实例
定义三个资源文件，放到src的根目录下面（必须这样，或者你放到自己配置的calsspath下面。
myres.properties
aaa=good 
bbb=thanks
myres_en_US.properties
aaa=good 
bbb=thanks
myres_zh_CN.properties
aaa=\u597d 
bbb=\u591a\u8c22
import java.util.Locale; 
import java.util.ResourceBundle; 
/** 
* 国际化资源绑定测试 
* 
* @author leizhimin 2009-7-29 21:17:42 
*/
publicclass TestResourceBundle
 { 
publicstaticvoid main(String[]
 args) { 
                Locale locale1 = new Locale("zh", "CN"); 
                ResourceBundle resb1 = ResourceBundle.getBundle("myres", locale1); 
                System.out.println(resb1.getString("aaa")); 
                ResourceBundle resb2 = ResourceBundle.getBundle("myres", Locale.getDefault()); 
                System.out.println(resb1.getString("aaa")); 
                Locale locale3 = new Locale("en", "US"); 
                ResourceBundle resb3 = ResourceBundle.getBundle("myres", locale3); 
                System.out.println(resb3.getString("aaa")); 
        } 
}
运行结果：
好 
好 
good 
Process finished with exit code 0
如果使用默认的Locale，那么在英文操作系统上，会选择myres_en_US.properties或myres.properties资源文件。
三、认识Locale
Locale 对象表示了特定的地理、政治和文化地区。需要 Locale 来执行其任务的操作称为语言环境敏感的 操作，它使用 Locale 为用户量身定制信息。例如，显示一个数值就是语言环境敏感的操作，应该根据用户的国家、地区或文化的风俗/传统来格式化该数值。
使用此类中的构造方法来创建 Locale：
 Locale(String language)
 Locale(String language, String country)
 Locale(String language, String country, String variant)
创建完 Locale 后，就可以查询有关其自身的信息。使用 getCountry 可获取 ISO 国家代码，使用 getLanguage 则获取 ISO 语言代码。可用使用 getDisplayCountry 来获取适合向用户显示的国家名。同样，可用使用 getDisplayLanguage 来获取适合向用户显示的语言名。有趣的是，getDisplayXXX 方法本身是语言环境敏感的，它有两个版本：一个使用默认的语言环境作为参数，另一个则使用指定的语言环境作为参数。 
语言参数是一个有效的 ISO 语言代码。这些代码是由 ISO-639 定义的小写两字母代码。在许多网站上都可以找到这些代码的完整列表，如： 
http://www.loc.gov/standards/iso639-2/englangn.html。    
国家参数是一个有效的 ISO 国家代码。这些代码是由 ISO-3166 定义的大写两字母代码。在许多网站上都可以找到这些代码的完整列表，如： 
http://www.iso.ch/iso/en/prods-services/iso3166ma/02iso-3166-code-lists/list-en1.html。    
四、中文资源文件的转码 native2ascii
这个工具用法如下:
![](http://img1.51cto.com/attachment/200907/200907291248879241218.png)
如果觉得麻烦，可以直接将中文粘贴到里面，回车就可以看到转码后的结果了。
![](http://img1.51cto.com/attachment/200907/200907291248879324359.png)
看明白这个了，就不在为struts等web框架的国际化而感到稀奇了。
**1. [Java](http://lib.csdn.net/base/javase)国际化简介**
Java既然作为一个跨平台的语言就必然要在各种不同的语言环境中使用, 为了解决这个问题Java给我们提供了一个工具类ResourceBundle, 帮助我们实现Java的国际化, 核心的思想就是, 对不同的语言提供一个不同的资源文件.
**2. Java国际化的步骤**
(1) 当然是编写要实现国际化的主程序.
(2) 定义资源文件, 注意资源文件的定义要符合一定的规范, 规范如下: 如果我们将缺省的资源文件的文件名取名为message.properties, 那么相应的其他语言的资源文件名就是message_语言代码_国家代码.properties. 例如: 我们的语言汉语的语言代码是zh, 国家代码是CN, 所以简体中文的资源文件名就是: message_zh_CN.properties,
 英语的语言代码是en, 美国的国家代码是US, 所以美国英语的资源文件就是: message_en_US.properties.
(3) 根据软件的使用环境取得相应的资源文件
(4) 在取得的资源文件中取得Key对应的值.
**3. 例子(注意这里的资源文件必须放在classpath的根路径下)**
(1) 主程序ParamFormat.java
package test.abu.csdn.tmp;
import java.text.MessageFormat;
import java.util.Date;
import java.util.Locale;
import java.util.ResourceBundle;
/**
 * 演示Java的国际化
 * @author Abu
 *
 */
public class ParamFormat {
    /**
     * @param args
     */
    public static void main(String[] args) {
// [测试](http://lib.csdn.net/base/softwaretest)缺省的资源文件
        testCustom();
 // 测试美国英语的资源文件
        testDefault();
    }
    public static void testDefault() {
 // 获得缺省的系统区域
        Locale locale = Locale.getDefault();
   // 获得资源文件
        ResourceBundle rb = ResourceBundle.getBundle("message", locale);
        // 获得相应的key值    
        String greeting = rb.getString("greeting");
        String userInfo = rb.getString("userinfo");
        String name= "liky";
        int age = 18;
        Date birth = new Date();
 // 格式化参数,注意是一个Object的数组,这意味着可以将任何类型的对象来格式化模板.
        Object[] params = {name,age,birth}; 
    // 格式化参数,返回格式后的字符串
        String result = MessageFormat.format(userInfo, params);
        System.err.println(greeting + result);    
    }
    public static void testCustom() {
    // 设置定制的语言国家代码
        Locale locale = new Locale("en_US");
    // 获得资源文件
        ResourceBundle rb = ResourceBundle.getBundle("message", locale);
        // 获得相应的key值
        String greeting = rb.getString("greeting");
        String userInfo = rb.getString("userinfo");
        String name= "liky";
        int age = 18;
        Date birth = new Date();
  // 格式化参数,注意是一个Object的数组,这意味着可以将任何类型的对象来格式化模板.
        Object[] params = {name,age,birth}; 
        // 格式化参数,返回格式后的字符串
        String result = MessageFormat.format(userInfo, params);
        System.err.println(greeting + result);    
    }    
}
(2) 缺省的资源文件message.properties(这里使用native2ascii将中文转化成了ISO-8859-1编码.)
greeting /u6B22/u8FCE/u60A8/u7684/u5230/u6765, 
userinfo=/u59D3/u540D/: {0}, /u5E74/u9F84/: {1}, /u751F/u65E5/: {2}.
(3) 美国英语的资源文件message_en_US.properties
greeting Welcome my boy, 
userinfo=name: {0}, age: {1}, birthday: {2}.
**4.总结**
其实这就是Java的i18n的本质了, 其他的一些框架例如struts, webwork等的国际化核心都是如此.
