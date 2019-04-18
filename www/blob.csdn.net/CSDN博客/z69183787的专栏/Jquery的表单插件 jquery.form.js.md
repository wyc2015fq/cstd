# Jquery的表单插件 jquery.form.js - z69183787的专栏 - CSDN博客
2013年12月14日 13:12:28[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1126
1. 本篇文章是从我网上搜集整理的有关Jquery Form的用法。
Jquery Form Plugin是jquery最重要的插件之一，它利用AJAX技术在不引起页面刷新的情况下POST表单。主要有两个方法：ajaxform和ajaxsubmit。它会自动收集表单元素内容，决定如何管理提交进程。这两个方法支持多重选择。我想AJAX方式的表单应用再没有比这个插件更简单易用的了。 
先在页面里设置一个普通的表单：
Html代码  ![收藏代码](http://yaofeng911.iteye.com/images/icon_star.png)
- <formid=”myForm” action=”comment.php” method=”post”>
-     姓名: <inputtype=”text” name=”name” id=”name”/>
-     评论: <textareaname=”comment” id=”comment”></textarea>
- <inputtype=”submit” value=”提交评论” />
- </form>
- [/html]   
加载jquery库和插件脚本库。然后简单地写一段初始化脚本，这段脚本将在DOM加载完成时执行： 
Html代码  ![收藏代码](http://yaofeng911.iteye.com/images/icon_star.png)
- <head>
- <scripttype=”text/javascript” src=”jquery.js”></script>  //加载jquery   
- <scripttype=”text/javascript” src=”form.js”></script>    //加载插件   
- <scripttype=”text/javascript”>
-         // DOM加载完毕后执行   
-         $(document).ready(function() {   
-             // 绑定’myForm’并定义一个简单的回调函数   
-             $('#myForm').ajaxForm(function() {   
-                 alert(”评论提交完成!”);   
-             });   
-         });   
- </script>
- </head>
- [/html]  
OK，就是这么简单。当点击“提交评论”按钮后，表单的数据就会POST到comment.php脚本，并且返回“评论提交完成”信息（如果提交成功）。页面并没有刷新，非常AJAX……
让我们来看看这个插件还有什么强大的功能。 
## ajaxForm
1. 该方法须预先绑定表单，所以它一般在$(document).ready(function() { …}里定义。它能让表单在不刷新页面的情况下POST到目标。可以为该方法提供一个参数，参数一般是一个回调函数用于返回信息给用户。当然如果不提供参数也行，只是表单提在无声无息地提交后无法得到确认了。
2. 增 加所有需要的事件监听器，为AJAX提交表单做好准备。ajaxForm不能提交表单。在document的ready函数中，使用ajaxForm来为 AJAX提交表单进行准备。ajaxForm接受0个或1个参数。这个单个的参数既可以是一个回调函数，也可以是一个Options对象。
可链接（Chainable）：可以。
实例：
Js代码  ![收藏代码](http://yaofeng911.iteye.com/images/icon_star.png)
- $('#myFormId').ajaxForm();  
## ajaxSubmit
1. 该方法是以响应事件来通用AJAX方式提交表单。比如点击某个按钮或改变了某个下拉框的值，在触发事件函数里可以设置该方法。如：
Js代码  ![收藏代码](http://yaofeng911.iteye.com/images/icon_star.png)
- $(”#clickme”).click(function(){  
-   $(”#myForm”).ajaxSubmit();  
- });  
或者这样：
Html代码  ![收藏代码](http://yaofeng911.iteye.com/images/icon_star.png)
- <selectid=”mySelect” onchange=”$(‘#myForm’).ajaxSubmit();”>
2. 马上由AJAX来提交表单。大多数情况下，都是调用ajaxSubmit来对用户提交表单进行响应。ajaxSubmit接受0个或1个参数。这个单个的参数既可以是一个回调函数，也可以是一个**Options**对象。
可链接（Chainable）：可以。 
实例：
Js代码  ![收藏代码](http://yaofeng911.iteye.com/images/icon_star.png)
- // 绑定表单提交事件处理器
- $('#myFormId').submit(function() {  
- // 提交表单
-     $(this).ajaxSubmit();  
- // 为了防止普通浏览器进行表单提交和产生页面导航（防止页面刷新？）返回false
- returnfalse;  
-    });  
## formSerialize
1. 该方法可以表单域的名称和值连接起来，形成一个字符串，字符串格式为name1=value1&name2=value2…比如：
Js代码  ![收藏代码](http://yaofeng911.iteye.com/images/icon_star.png)
- var queryString=$(”#myForm”).formSerialize();  
变量queryString的值将为变成name=xxx&comment=xxx。得到这个字符串后，可以用$.post来提交，
如：
Js代码  ![收藏代码](http://yaofeng911.iteye.com/images/icon_star.png)
- $.post(”comment.php”,queryString);  
2.**将表单串行化（或序列化）成一个查询字符串。这个方法将返回以下格式的字符串：name1=value1&name2=value2。**可链接（Chainable）：不能， 这个方法返回一个字符串。 
实例：
Js代码  ![收藏代码](http://yaofeng911.iteye.com/images/icon_star.png)
- var queryString = $('#myFormId').formSerialize();  
- 
- // 现在可以使用$.get、$.post、$.ajax等来提交数据
- $.post('myscript.php', queryString);  
## fieldSerialize
1. 和formSerialize一样，返回一个字符串，但返回的是表单指定元素或特定元素类型域的字符串。返回字符串格式也是一样的。如：var queryString = $(’#myForm :text’).fieldSerialize();//返回myForm表单内所有文本框的字符串值。
2.将表单的字段元素串行化（或序列化）成一个查询字符串。当只有部分表单字段需要进行串行化（或序列化）时，这个就方便了。这个方法将返回以下格式的字符串：**name1=value1&name2=value2**。
可链接（Chainable）：不能，这个方法返回一个字符串。 
实例：
Js代码  ![收藏代码](http://yaofeng911.iteye.com/images/icon_star.png)
- var queryString = $('#myFormId .specialFields').fieldSerialize();   
## fieldValue
1.返回一个数组，记录表单元素的值。如果表单没有值则对应数组值为空。如：
Js代码  ![收藏代码](http://yaofeng911.iteye.com/images/icon_star.png)
- var data=$(’:text’).fieldValue();//返回所有表单中所有文本框类型的值。
变量data为[”,”,”]这样的一个数组，数组元素对应表单元素的值。事实上我们可以这样来得到“姓名”文本框里的值：var data=$(”#name”).val();（这个元素要有ID属性）这样的作法不需要jquery form插件也可以得到值。但使用插件可以更方便地取得单个或多个表单中特定元素域的值。比如取得所有文本框或所有复选框的值。只要加个“:”就可以。这种方法也可以用到formSerialize或fieldSerialize方法上。
Js代码  ![收藏代码](http://yaofeng911.iteye.com/images/icon_star.png)
- var data=$(”#myForm :text”).fieldValue();//取得在myForm表单里所有文本框的值。
2.返回匹配插入数组中的表单元素值。从0.91版起，该方法将总是以数组的形式返回数据。如果元素值被判定可能无效，则数组为空，否则它将包含一个或多于一个的元素值。
可链接（Chainable）：不能，该方法返回数组。
实例：
Js代码  ![收藏代码](http://yaofeng911.iteye.com/images/icon_star.png)
- // 取得密码输入值
- var value = $('#myFormId :password').fieldValue();   
- alert('The password is: ' + value[0]);   
## formToArray
同样也返回一个数组，这个数组是对象数组。对象有name和value两个属性，分别记录表单中的元素名称和元素值。如：
Js代码  ![收藏代码](http://yaofeng911.iteye.com/images/icon_star.png)
- var data=$(”#myForm”).formToArray();  
 变量data得到的是一个对象数组。data[0].name保存myForm表单中第一个表单元素的名称属性值，这里是”name”。data[0].value保存myForm表单中第一个表单元素的值，这里就是填了什么值就是什么。
## resetForm
1.该方法很简单啦，恢复表单初始状态。也就恢复到DOM加载完成时的表单状态。类似“重设表单”
2.清除表单元素。该方法将所有的文本（text）输入字段、密码（password）输入字段和文本区域（textarea）字段置空，清除任何select元素中的选定，以及将所有的单选（radio）按钮和多选（checkbox）按钮重置为非选定状态。
可链接（Chainable）：可以。
Js代码  ![收藏代码](http://yaofeng911.iteye.com/images/icon_star.png)
- $('#myFormId').clearForm();  
## clearFields
1.清除表单域元素。可以清除特定类型的域元素，比如清除所有文本框的，或所有复选框的域。
2.清除字段元素。只有部分表单元素需要清除时才方便使用。
可链接（Chainable）：可以。
Js代码  ![收藏代码](http://yaofeng911.iteye.com/images/icon_star.png)
- $('#myFormId .specialFields').clearFields();   
**Options对象**ajaxForm和ajaxSubmit都支持众多的选项参数，这些选项参数可以使用一个Options对象来提供。Options只是一个JavaScript对象，它包含了如下一些属性与值的集合：
target
指明页面中由服务器响应进行更新的元素。元素的值可能被指定为一个jQuery选择器字符串，一个jQuery对象，或者一个DOM元素。
默认值：null。
url
指定提交表单数据的URL。
默认值：表单的action属性值
type
指定提交表单数据的方法（method）：“GET”或“POST”。
默认值：表单的method属性值（如果没有找到默认为“GET”）。 
beforeSubmit
表 单提交前被调用的回调函数。“beforeSubmit”回调函数作为一个钩子（hook），被提供来运行预提交逻辑或者校验表单数据。如果 “beforeSubmit”回调函数返回false，那么表单将不被提交。“beforeSubmit”回调函数带三个调用参数：数组形式的表单数据， jQuery表单对象，以及传入ajaxForm/ajaxSubmit中的Options对象。表单数组接受以下方式的数据：
[ { name: 'username', value: 'jresig' }, { name: 'password', value: 'secret' } ]
默认值：null
success
表单成功提交后调用的回调函数。如果提供“success”回调函数，当从服务器返回响应后它被调用。然后由dataType选项值决定传回responseText还是responseXML的值。
默认值：null
dataType
期望返回的数据类型。null、“xml”、“script”或者“json”其中之一。dataType提供一种方法，它规定了怎样处理服务器的响应。这个被直接地反映到jQuery.httpData方法中去。下面的值被支持：
'**xml**'：如果dataType == 'xml'，将把服务器响应作为XML来对待。同时，如果“success”回调方法被指定， 将传回responseXML值。
'**json**'：如果dataType == 'json'， 服务器响应将被求值，并传递到“success”回调方法，如果它被指定的话。
'**script**'：如果dataType == 'script'， 服务器响应将求值成纯文本。
默认值：null（服务器返回responseText值） 
semantic
Boolean flag indicating whether data must be submitted in strict semantic order (slower). Note that the normal form serialization is done in semantic order with the exception of input elements of type="image". You should only set the semantic option to true
 if your server has strict semantic requirements and your form contains an input element of type="image".
布 尔标志，表示数据是否必须严格按照语义顺序（slower？）来进行提交。注意：一般来说，表单已经按照语义顺序来进行了串行化（或序列化），除了 type="image"的input元素。如果你的服务器有严格的语义要求，以及表单中包含有一个type="image"的input元素，就应该将 semantic设置为true。（译注：这一段由于无法理解，翻译出来可能语不达意，但请达人指正。）
默认值：false 
resetForm
布尔标志，表示如果表单提交成功是否进行重置。
Default value: null 
clearForm
布尔标志，表示如果表单提交成功是否清除表单数据。
默认值：null 
实例：
Js代码  ![收藏代码](http://yaofeng911.iteye.com/images/icon_star.png)
- // 准备好Options对象
- var options = {  
-     target:     '#divToUpdate',  
-     url:        'comment.php',  
-     success: function() {  
-       alert('Thanks for your comment!');  
-     } };  
- 
- // 将options传给ajaxForm
- $('#myForm').ajaxForm(options);  
注意：Options对象还可以用来将值传递给jQuery的$.ajax方法。如果你熟悉$.ajax所支持的options，你可以利用它们来将Options对象传递给ajaxForm和ajaxSubmit。
参考实例 [http://www.malsup.com/jquery/form/#api](http://www.malsup.com/jquery/form/#api)
