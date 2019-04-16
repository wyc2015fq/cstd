# EasyUI学习总结(四)——parser源码分析 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [EasyUI学习总结(四)——parser源码分析](https://www.cnblogs.com/xdp-gacl/p/4082561.html)



　　parser模块是easyloader第一个加载的模块，它的主要作用，就是扫描页面上easyui开头的class标签，然后初始化成easyui控件。

```
1     /**
 2      * parser模块主要是解析页面中easyui的控件
 3      */
 4     $.parser = {
 5         // 是否自动解析
 6         auto: true,
 7 
 8         // 可以被解析的控件
 9         plugins:['linkbutton','menu','menubutton','splitbutton','layout',
10                  'tree','window','dialog','datagrid',
11                  'combobox','combotree','numberbox','validatebox',
12                  'calendar','datebox','panel','tabs','accordion'
13         ],
14 
15         // 解析函数
16         parse: function(context){
17             if ($.parser.auto){
18                 for(var i=0; i<$.parser.plugins.length; i++){
19                     (function(){
20                         // 控件名
21                         var name = $.parser.plugins[i];
22                         // 查找class为easyui-控件名的jq对象，例如，easyui-layout
23                         var r = $('.easyui-' + name, context);
24 
25                         if (r.length){
26                             // 如果有这个对象，那么判断它有没有初始化函数
27                             if (r[name]){
28                                 // 如果有直接调用
29                                 r[name]();
30                             } else if (window.easyloader){
31                                 // 如果没有用easyloader把模块的js文件载入进来，再调用
32                                 easyloader.load(name, function(){
33                                     r[name]();
34                                 })
35                             }
36                         }
37                     })();
38                 }
39             }
40         }
41     };
42 
43     // 调用parse方法，实际上easyloader中已经调用了，我估计这个是给不是easyloader加载时使用的
44     $(function(){
45         $.parser.parse();
46     });
47 })(jQuery);
```











