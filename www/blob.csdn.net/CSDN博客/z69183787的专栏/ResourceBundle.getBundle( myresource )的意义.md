# ResourceBundle.getBundle("myresource")的意义 - z69183787的专栏 - CSDN博客
2012年12月07日 14:25:49[OkidoGreen](https://me.csdn.net/z69183787)阅读数：954
使用ResourceBundle访问本地资源    
    在设计时，我们往往需要访问一些适合本地修改的配置信息，如果作为静态变量，那么每次修改都需要重新编译一个class，.config保存此类信息并不适合，这时我们需要ResourceBundle。   通过ResourceBundle，我们需要访问位于/WEB-INF/classes目录下的一个后缀名为properties的文本类型文件，从里面读取我们需要的值。    Locale locale = Locale.getDefault();    ResourceBundle localResource
 = ResourceBundle.getBundle("ConnResource", locale);    String value = localResource.getString("test");    System.out.println("ResourceBundle: " + value);    这里对应了/WEB-INF/class/ConnResource.properties文件内容为：    test=hello world    打印出来的结果就是hello world       
 请注意，这里我们可以利用Locale和ResourceBundle的这个组合创建国际化的java程序。我们可以把locale实例化为new Locale("zh","CN");    通过ResourceBundle.getBundle("MessagesBundle", locale);    系统将自动寻找MessagesBundle_zh_CN，即定义为中国大陆地区简体中文。如果没有该文件，则会依次寻找MessagesBundle_zh，MessagesBundle，直到找到为止。
