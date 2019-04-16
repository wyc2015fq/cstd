# EasyUI学习总结(二)——easyloader分析与使用(转载) - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [EasyUI学习总结(二)——easyloader分析与使用(转载)](https://www.cnblogs.com/xdp-gacl/p/4079802.html)



　　本文转载自：http://www.cnblogs.com/haogj/archive/2013/04/22/3036685.html

　　使用脚本库总要加载一大堆的样式表和脚本文件，在easyui 中，除了可以使用通常的方式加载之外，还提供了使用 easyloader 加载的方式。这个组件主要是为了按需加载组件而诞生。什么情况下使用它呢？
- 你觉得一次性导入 easyui 的核心 min js 和 css 太大
- 你只用到 easyui 的其中几个组件
- 你想使用其中的一个组件，但是你又不知道这个组件依赖了那些组件。

　　如果你有以上三中情况，那么推荐你使用easyLoader。它可以帮你解决这些问题。

　　easyloader 用来帮助我们自动加载所需的脚本文件和样式文件，这样，我们只需要在页面中引用 jquery 脚本 和 easyloader 脚本，easyloader 就可以帮助我们分析模块的依赖关系，先加载依赖项。模块加载好了会调用parse模块来解析页面。把class是easyui开头的标签都转化成 easyui的控件。

　　下面我们，以使用messager和dialog模块为例，使用easyloader加载所需的模块。

　　我们的页面可以简单的仅仅写入下面的内容。注意，并不需要通常的样式表和一大堆的脚本引用。

```
1 <%@ page language="java" pageEncoding="UTF-8"%>
 2 <!DOCTYPE HTML>
 3 <html>
 4   <head>
 5     <title>EasyUI入门——EasyUI的easyloader的使用</title>
 6   <!-- 引入JQuery -->
 7   <script type="text/javascript" src="${pageContext.request.contextPath}/jquery-easyui-1.4.1/jquery.min.js"></script>
 8   <!-- 引入easyloader.js -->
 9   <script type="text/javascript" src="${pageContext.request.contextPath}/jquery-easyui-1.4.1/easyloader.js"></script>
10   
11   <script type="text/javascript">
12       $(function(){
13           //使用easyloader加载dialog模块使用到的相关js和css样式
14           easyloader.load('dialog',function(){
15               /*使用JavaScript动态创建EasyUI的Dialog的步骤：
16                 1、定义一个div，并给div指定一个id
17                 2、使用Jquery选择器选中该div，然后调用dialog()方法就可以创建EasyUI的Dialog
18                 */
19                 //使用自定义参数创建EasyUI的Dialog
20                 $('#dd2').dialog({
21                     title: '使用JavaScript创建的Dialog',
22                     width: 400,
23                     height: 200,
24                     closed: false,
25                     cache: false,
26                     modal: true
27                 });
28           });
29           
30           easyloader.locale = "zh_CN";
31           //easyloader.load 还有一个别名 using 定义在 window 对象上
32           //使用easyloader加载messager模块使用到的相关js和css样式
33         using("messager", function () {
34             alert("加载成功！");
35             $("#btnAlert").click(function () {
36                 $.messager.alert('Warning', 'The warning message');
37             });
38         });
39       });
40   </script>
41   
42   </head>
43   
44   <body>
45       <%--使用纯html的方式创建创建EasyUI的Dialog的步骤：
46       1、定义一个div
47       2、将div的class样式属性设置成easyui-dialog，这样就可以将普通的div变成EasyUI的Dialog了
48        --%>
49     <div class="easyui-dialog" id="dd1" title="EasyUI Dialog" style="width: 500px;height: 300px;">
50         Hello World!
51     </div>
52     <div id="dd2">Dialog Content</div>
53     <a id="btnAlert" class="easyui-linkbutton">弹出提示框</a>
54   </body>
55 </html>
```

　　注意看！只有 jquery 的脚本和 easyloader 的脚本，完全没有一大堆的样式和其他脚本文件。

　　页面运行效果如下：

![](https://images0.cnblogs.com/blog/289233/201411/062024441426311.gif)

　　load 用来使用代码来说明需要加载的模块，这是在 easyloader 中定义的一个函数，函数的第一个参数为准备加载的模块名称，第二个参数为加载成功之后的回调函数。这里用来提示已经加载成功。

　　load 加载的模块有两种格式，即可以是一个字符串表示的单个模块，也可以是一个字符串的数组，同时加载多个模块。

```
1 //name有两种，一种是string ,一种是string array,这样一次可以加载多个plugin,都是调用add方法进行添加  
2 if (typeof name == 'string') {
3     add(name);
4 } else {
5     for (var i = 0; i < name.length; i++) {
6         add(name[i]);
7     }
8 }
```

　　easyloader.load 还有一个别名 using 定义在 window对象上，如下所示：

```
window.using = easyloader.load;
```

　　所以，加载的代码也可以这样写。

```
1 using("messager", function () {
2     alert("加载成功！");
3 });
```

　　加载成功之后，我们就可以在代码中使用已经加载的模块了。

　　页面中还使用 class 说明了一个按钮，这里使用了 class="easyui-linkbutton"，easyloader 还可以帮助我们解析元素中的特殊类名，直接就在页面中使用过的模块。

　　easyloader 会在它所在文件夹中，寻找 plugins 子文件夹中的脚本，和 themes 文件夹中的样式表。所以需要保证文件保存在正确的位置。不过，easyloader 还提供了一个 base 属性，用来指定寻找插件和脚本的起点。

```
1 // jquery-easyui的根目录，在加载easyloader时，会自动根据你放置的位置而改变  
2 base:'.',
```

　　除了 base , 还有几个重要的属性

```
1 base:'.',//该属性是为了加载js,记录文件夹路径的  
2 theme:'default', //默认主题  
3 css:true,    
4 locale:null,
```

　　比如，希望使用中文语言包，则可以如下使用。你会看到提示框的按钮中的文字已经从 Ok 转换为了 "确定"。

```
1 easyloader.locale = "zh_CN";
2 using("messager", function () {
3     alert("加载成功！");
4 });
```

 　　以上就是关于easyloader分析与使用。









