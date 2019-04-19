# jstree 点箭头有效但点后面item无效的bug - Arthur的随笔 - CSDN博客
2011年10月26日 22:37:32[largetalk](https://me.csdn.net/largetalk)阅读数：1416标签：[class																[django																[function																[html																[list](https://so.csdn.net/so/search/s.do?q=list&t=blog)](https://so.csdn.net/so/search/s.do?q=html&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=django&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)
个人分类：[杂七杂八](https://blog.csdn.net/largetalk/article/category/823511)
                
比如现在有html（django template）如下：
```
<li class="jstree-closed">
<ins class="jstree-icon"> </ins>
<a href="#" name="template_category_item" value="{{key}}"><ins class="jstree-icon"> </ins>{{key}}</a>
<ul>
{% for template in value %}
<li class="jstree-last jstree-leaf">
<ins class="jstree-icon"> </ins>
<a href="#" name="template_list_item" value="{{template.pk}}"><ins class="jstree-icon"> </ins>{{template.module}}</a>
</li>
{% endfor %}
</ul>
</li>
```
为了修复这个bug，只要给template_category_item添加一个事件响应即可，代码如下：
```java
$("a[name=template_category_item]").live("click", function(e){
        var evt = document.createEvent("MouseEvents");
        evt.initEvent("click", true, true);
        $(this).prev()[0].dispatchEvent(evt);
        return false;
    });
```

