# SSH框架组建时碰到的一些问题 - =朝晖= - 博客园
# [SSH框架组建时碰到的一些问题](https://www.cnblogs.com/dhcn/p/7100215.html)
   以前用[spring](http://lib.csdn.net/base/javaee)+[hibernate](http://lib.csdn.net/base/javaee)的框架解决后台事务，这一次重新组建框架，计划引入Struts,如果方便的话，可能会进一步引入Freemarker。以下记下配置中的一些问题及解决，以供他人参考。
    1、server load时的一个问题：Unable to load configuration. - bean - jar:file:/D:/Dev/resin-3.1.9/webapps/CSD/WEB-INF/lib/struts2-core-2.1.8.1.jar!/struts-default.xml:47:178
          解决：实际上就加上struts-blank这个示例下的commons-fileupload包就可以了。解决参考页面：[http://linres.javaeye.com/blog/352147](http://linres.javaeye.com/blog/352147)
     2、在Sitemesh模板中使用struts标签必须增加一个叫 ActionContextCleanUp 的过滤器。
     3、StrutsTypeConverter类的依赖包有2个：struts2-core-2.1.8.1.jar和xwork-core-2.1.6.jar，注意第一次impor查找找不到的时候，可以删除再添加这两个包然后再重新[测试](http://lib.csdn.net/base/softwaretest)。我个人认为在我这儿刚开始找不到类的原因是初始时，我是先添加struts-core-*.jar,后添加的xwork-core-2.1.6.jar,先后的依存关系导致某些类不可见。
