# 《Training:Supporting Different Languages》 - 一世豁然的专栏 - CSDN博客





2017年04月13日 22:53:11[一世豁然](https://me.csdn.net/Explorer_day)阅读数：187标签：[android](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android](https://blog.csdn.net/Explorer_day/article/category/6289318)









注：本人非专业翻译人员，编写此博客只是为了自学使用，如有疑问，请参考官方文档。




从您的应用程序代码中提取UI字符串并将其保存在外部文件中是一个很好的做法。 Android使每个在Android项目中的资源目录轻松实现。




如果您使用Android SDK工具（请参阅创建Android项目）创建了项目，那么这些工具会在项目的顶层创建一个res /目录。 在这个res /目录中有各种资源类型的子目录。 还有一些默认文件，如res / values / strings.xml，其中包含您的字符串值。





Create Locale Directories and String Files


要添加对更多语言的支持，请在res /中创建附加值目录，其中包含连字符和目录名末尾的ISO语言代码。 例如，值-es /是包含语言代码为“es”的区域设置的简单资源的目录。 Android将在运行时根据设备的区域设置加载相应的资源。 有关更多信息，请参阅Providing Alternative Resources。





一旦您决定了将支持的语言，请创建资源子目录和字符串资源文件。 例如：



MyProject/
    res/
       values/
           strings.xml
       values-es/
           strings.xml
       values-fr/
           strings.xml
将每个区域设置的字符串值添加到相应的文件中。






在运行时，Android系统会根据当前为用户设备设置的区域设置使用适当的字符串资源集。



例如，以下是不同语言的一些不同的字符串资源文件。





英文（默认语言环境），/values/strings.xml：




```
<?xml version="1.0" encoding="utf-8"?>
<resources>
    <string name="title">My Application</string>
    <string name="hello_world">Hello World!</string>
</resources>
```


西班牙语，/values-es/strings.xml：





```
<?xml version="1.0" encoding="utf-8"?>
<resources>
    <string name="title">Mi Aplicación</string>
    <string name="hello_world">Hola Mundo!</string>
</resources>
```


法语，/values-fr/strings.xml：





```
<?xml version="1.0" encoding="utf-8"?>
<resources>
    <string name="title">Mon Application</string>
    <string name="hello_world">Bonjour le monde !</string>
</resources>
```






注意：您可以使用任何资源类型的区域设置限定符（或任何配置限定符），例如，提供位图drawable的本地化版本。 有关详细信息，请参阅Localization







Use the String Resources

您可以使用由<string>元素的name属性定义的资源名称来引用源代码和其他XML文件中的字符串资源。




在您的源代码中，您可以使用语法R.string.<string_name>引用字符串资源。 有多种方法可以通过这种方式接受字符串资源。




例如：



```java
// Get a string resource from your app's Resources
String hello = getResources().getString(R.string.hello_world);

// Or supply a string resource to a method that requires a string
TextView textView = new TextView(this);
textView.setText(R.string.hello_world);
```


在其他XML文件中，只要XML属性接受字符串值，就可以引用一个带有@ string / <string_name>的字符串资源。






例如：



```
<TextView
    android:layout_width="wrap_content"
    android:layout_height="wrap_content"
    android:text="@string/hello_world" />
```








