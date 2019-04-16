# jQuery 表单验证插件 jQuery Validation Engine 使用 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月10日 17:32:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：22
个人分类：[HTML和Javascript](https://blog.csdn.net/leixiaohua1020/article/category/1678189)












jQuery 表单验证插件 jQuery Validation Engine 使用方式如下：

1.引入头文件（注意一定要把jQuery放在前面），指定使用jQuery Validation Engine的<form>的ID：



```
<script src="lib/jquery-1.8.3.min.js"></script>

<!-- validationEngine -->
<link rel="stylesheet" href="css/validationEngine.jquery.css" type="text/css"/>
<script src="lib/jquery.validationEngine-zh_CN.js" type="text/javascript" charset="utf-8">
</script>
<script src="lib/jquery.validationEngine.js" type="text/javascript" charset="utf-8">
</script>
<script>
	$(document).ready(function(){
		// binds form submission and fields to the validation engine
		$("#formular").validationEngine('attach');
	});
</script>
```


2.在需要使用jQuery Validation Engine的<form>代码如下所示：





```
<form action="UserAdd.action" method="post" id="formular">
               <table class="tablesorter">
            	<tbody>
            		<tr><td><s:property value="%{getText('user.name')}"/>：</td><td><input value=""  name="name" type="text" /></td></tr>
                  	<tr><td><s:property value="%{getText('user.loginname')}"/>：</td><td><input value="" class="validate[required,minSize[4]] text-input validate[optional,maxSize[20]] text-input" name="loginname" type="text" /></td></tr>
                  	<tr><td><s:property value="%{getText('user.password')}"/>：</td><td><input value="" class="validate[required] text-input" name="password" type="password" id="password"/></td></tr>
                 	<tr><td><s:property value="%{getText('user.repassword')}"/>：</td><td><input value="" class="validate[required,equals[password]] text-input" name="repassword" type="password" /></td></tr>
                  	<tr><td><s:property value="%{getText('user.email')}"/>：</td><td><input class="validate[required,custom[email]] text-input" value="" name="email" type="text" /></td></tr>
	                <tr><td><s:property value="%{getText('user.sex')}"/>：</td><td><input id="sex_man" type="radio" name="sex" value="1" /><s:property value="%{getText('user.sex.man')}"/>
              		<input id="sex_woman" type="radio" name="sex" value="0"/><s:property value="%{getText('user.sex.woman')}"/></td></tr>
                  	<tr><td><s:property value="%{getText('user.birthday')}"/>：</td><td><input class="validate[required,custom[date]] text-input" value="" name="birthday" id="birthday" type="text" /></td></tr>
                  	<tr><td><s:property value="%{getText('user.telephone')}"/>：</td><td><input class="validate[custom[phone]] text-input" value="" name="telephone" type="text" /></td></tr>
                  	<tr><td><s:property value="%{getText('user.userrolename')}"/>：</td><td><select name="userroleid" id="userroleid" class="input_select2">
                   <c:forEach items="${alluserrole}" var="userrole">
                  	<option value="${userrole.id}"}>${userrole.name}</option>
                  </c:forEach>	
                  </select></td></tr>
            </tbody>
         	</table>
                  <div><s:property value="%{getText('global.remark')}"/>：<br/>
                  <textarea class="text_area2" cols="32" rows="3" name="remark"><s:property value="%{getText('user.value')}"/><s:property value="%{getText('global.remark')}"/></textarea></div><div style="height:10px"></div>
              <div>
                <input class="submit2" name="Submit" type="submit" value="提交" />
                
              	<input class="submit2" name="Submit2" type="reset" value="重置" />
              </div>
          	</form>
```


 代码有些乱，是直接从项目工程中粘贴出来的，用到了Struts2标签，JSTL，以及EL等。但是决定表单如何验证的关键在于每个输入框的“class”属性。



例如name为loginname的<input>的class属性如下：

class="validate[required,minSize[4]] text-input validate[optional,maxSize[20]] text-input"


就规定了输入内容长度为4-20字符，如果为空，效果如下：

![](https://img-blog.csdn.net/20131010172654593?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




再比如name为repassword的<input>的class属性如下：

class="validate[required,equals[password]] text-input"


则会比较id为password的<input>的内容与本<input>是否相同，即确认密码与密码是否相同。



再比如name为email的<input>的class属性如下：


class="validate[required,custom[email]] text-input"


则验证本<input>的内容是否符合Email格式




下载地址：[http://download.csdn.net/detail/leixiaohua1020/6376529](http://download.csdn.net/detail/leixiaohua1020/6376529)




