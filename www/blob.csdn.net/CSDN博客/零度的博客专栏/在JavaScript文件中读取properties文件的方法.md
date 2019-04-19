# 在JavaScript文件中读取properties文件的方法 - 零度的博客专栏 - CSDN博客
2016年09月21日 17:46:13[零度anngle](https://me.csdn.net/zmx729618)阅读数：25562标签：[javascript](https://so.csdn.net/so/search/s.do?q=javascript&t=blog)
个人分类：[Javascript																[I18N																[Js/JQuery插件](https://blog.csdn.net/zmx729618/article/category/6201133)](https://blog.csdn.net/zmx729618/article/category/6192897)](https://blog.csdn.net/zmx729618/article/category/6156941)
假设有JavaScript文件叫做：readproperties.js，这个文件需要读取config.properties这个配置文件，步骤如下：
1、  下载插件[jquery.i18n.properties-min-1.0.9.js](http://download.csdn.net/download/aj1031689/6480773)，在eclipse中放到合适的目录下。由于需要jQuery的支持，所以也需要jquery插件，在这里选择jquery-1.7.1.min.js（jquery.i18n.properties-min-1.0.9.js这个插件对jQuery没有版本要求，可以使用任何版本的jQuery插件），如下图所示：
![](https://img-blog.csdn.net/20150708181224831?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
2、  在引入readproperties.js的JSP文件中做如下声明：
**[html]**[view plain](http://blog.csdn.net/icat1989/article/details/46806807#)[copy](http://blog.csdn.net/icat1989/article/details/46806807#)[print](http://blog.csdn.net/icat1989/article/details/46806807#)[?](http://blog.csdn.net/icat1989/article/details/46806807#)
- <scriptsrc="js/jquery-1.7.1.min.js"language="javascript">
- </script>
- 
- <scripttype="text/javascript"src="js/jquery.i18n.properties-min-1.0.9.js">
- </script>
![](http://static.blog.csdn.net/images/save_snippets.png)
```
<script src="js/jquery-1.7.1.min.js" language="javascript">
</script>
<script type="text/javascript" src="js/jquery.i18n.properties-min-1.0.9.js"> 
</script>
```
其中的路径根据实际情况作出调整。
3、在readproperties.js中，编写如下函数获取properties文件中的值：
**[javascript]**[view plain](http://blog.csdn.net/icat1989/article/details/46806807#)[copy](http://blog.csdn.net/icat1989/article/details/46806807#)[print](http://blog.csdn.net/icat1989/article/details/46806807#)[?](http://blog.csdn.net/icat1989/article/details/46806807#)
- function loadProperties(){  
-     jQuery.i18n.properties({// 加载properties文件
-     name:'ISPindex', // properties文件名称
-     path:'i18n/', // properties文件路径
-     mode:'map', // 用 Map 的方式使用资源文件中的值
-     callback: function() {// 加载成功后设置显示内容
-         alert($.i18n.prop(“isp_index”));//其中isp_index为properties文件中需要查找到的数据的key值
-     }  
-     });  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
function loadProperties(){
    jQuery.i18n.properties({// 加载properties文件
    name:'ISPindex', // properties文件名称
    path:'i18n/', // properties文件路径
    mode:'map', // 用 Map 的方式使用资源文件中的值
    callback: function() {// 加载成功后设置显示内容
        alert($.i18n.prop(“isp_index”));//其中isp_index为properties文件中需要查找到的数据的key值
    }
    });
}
```
其中properties文件的路径、名称等需要根据实际情况作出调整。本例中properties文件放在如下图所在位置。
![](https://img-blog.csdn.net/20150708181529811?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这样运行该函数时，即可显示需要的数据了。
如果本文说的不够清楚，可以参考API文档：[www.ibm.com/developerworks/cn/web/1305_hezj_jqueryi18n/](http://blog.csdn.net/icat1989/article/details/www.ibm.com/developerworks/cn/web/1305_hezj_jqueryi18n/)
