# kendo AutoComplete实现多筛选条件 - 零度的博客专栏 - CSDN博客
2016年08月11日 12:31:47[零度anngle](https://me.csdn.net/zmx729618)阅读数：1398
kendo autoComplete 原始情况下是不支持多筛选条件的
**[html]**[view plain](http://blog.csdn.net/zhangyuanwei88/article/details/38983751#)[copy](http://blog.csdn.net/zhangyuanwei88/article/details/38983751#)[print](http://blog.csdn.net/zhangyuanwei88/article/details/38983751#)[?](http://blog.csdn.net/zhangyuanwei88/article/details/38983751#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/462192/fork)
- $("#autocomplete").kendoAutoComplete({  
-   filter: "contains"  
- });  
![](http://static.blog.csdn.net/images/save_snippets.png)
```
$("#autocomplete").kendoAutoComplete({
  filter: "contains"
});
```
根据autocomplete 提供的api可以知道使用filter有三种，分别是`startswith`, `endswith` , `contains。但是作为中文使用用户，我们该怎么办呢。很多时候，我们希望搜索出来的东西可以通过汉字或者拼音过滤，但是原始的autocomplete是无法帮助我们实现这以功能的。`
接下来我们改造下，基于autocomplete做一个多过滤条件的autocomplete
具体代码如下：
**[javascript]**[view plain](http://blog.csdn.net/zhangyuanwei88/article/details/38983751#)[copy](http://blog.csdn.net/zhangyuanwei88/article/details/38983751#)[print](http://blog.csdn.net/zhangyuanwei88/article/details/38983751#)[?](http://blog.csdn.net/zhangyuanwei88/article/details/38983751#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/462192/fork)
- kendo.ui.plugin(kendo.ui.ComboBox.extend({  
-     options: {  
-         name: "MultiFilterComboBox"
-     },  
-     _filterSource: function () {  
- this.dataSource.filter({  
-             logic: "or",  
-             filters: [  
-                 { field: "code", operator: "contains", value: this.text() },  
-                 { field: "name", operator: "contains", value: this.text() }  
-             ]  
-         });  
-     }  
- }));  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
kendo.ui.plugin(kendo.ui.ComboBox.extend({
    options: {
        name: "MultiFilterComboBox"
    },
    _filterSource: function () {
        this.dataSource.filter({
            logic: "or",
            filters: [
                { field: "code", operator: "contains", value: this.text() },
                { field: "name", operator: "contains", value: this.text() }
            ]
        });
    }
}));
```
使用方法：
**[javascript]**[view plain](http://blog.csdn.net/zhangyuanwei88/article/details/38983751#)[copy](http://blog.csdn.net/zhangyuanwei88/article/details/38983751#)[print](http://blog.csdn.net/zhangyuanwei88/article/details/38983751#)[?](http://blog.csdn.net/zhangyuanwei88/article/details/38983751#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/462192/fork)
- $("#Nation").kendoMultiFilterComboBox({  
-     placeholder: "民族...",  
-     dataTextField: "name",  
-     dataValueField: "name",  
-     filter: "contains",  
-     dataSource:[{name:'汉族',code:'hanzu'},<pre name="code"class="javascript" style="color: rgb(85, 85, 85); font-size: 14px; line-height: 21px;">name:'苗族',code:'miaozu'}  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
$("#Nation").kendoMultiFilterComboBox({
    placeholder: "民族...",
    dataTextField: "name",
    dataValueField: "name",
    filter: "contains",
    dataSource:[{name:'汉族',code:'hanzu'},<pre name="code" class="javascript" style="color: rgb(85, 85, 85); font-size: 14px; line-height: 21px;">name:'苗族',code:'miaozu'}
```
]});
html标签：
**[html]**[view plain](http://blog.csdn.net/zhangyuanwei88/article/details/38983751#)[copy](http://blog.csdn.net/zhangyuanwei88/article/details/38983751#)[print](http://blog.csdn.net/zhangyuanwei88/article/details/38983751#)[?](http://blog.csdn.net/zhangyuanwei88/article/details/38983751#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/462192/fork)
- <inputid="Nation"/>
![](http://static.blog.csdn.net/images/save_snippets.png)
`<input id="Nation"/>`
现在我们就实现过组合过滤条件的autocomplete,示例只是做了或运算，其他的运算也是可以的，大家可以灵活应用，开发自己的大脑。
