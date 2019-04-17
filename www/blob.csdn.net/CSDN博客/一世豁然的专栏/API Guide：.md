# API Guide： - 一世豁然的专栏 - CSDN博客





2017年04月23日 20:36:32[一世豁然](https://me.csdn.net/Explorer_day)阅读数：245标签：[android](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android](https://blog.csdn.net/Explorer_day/article/category/6289318)









语法：


`<action android:name="string" />`


包含在：


[<intent-filter>](https://developer.android.google.cn/guide/topics/manifest/intent-filter-element.html)





描述：


将一个操作添加到意图过滤器。 一个<intent-filter>元素必须包含一个或多个<action>元素。 如果intent过滤器中没有<action>元素，则过滤器不接受任何Intent对象。 有关意图过滤器的详细信息以及过滤器中操作规范的作用，请参阅意向和意图过滤器。





属性：


android:name


动作的名称。 一些标准动作在Intent类中定义为ACTION_string常量。 要将这些操作之一分配给此属性，请先添加“android.intent.action”到AC TION_之后的字符串。 例如，对于ACTION_MAIN，使用“android.intent.action.MAIN”。对于ACTION_WEB_SEARCH，使用“android.intent.action.
 WEB_SEARCH”。对于您定义的操作，最好使用应用程序的程序包名称作为前缀来确保唯一性。 例如，TRANSMOGRIFY操作可能指定如下：


`	  <action android：name =“com.example.project.TRANSMOGRIFY”/>`

介绍在：


API Level 1






