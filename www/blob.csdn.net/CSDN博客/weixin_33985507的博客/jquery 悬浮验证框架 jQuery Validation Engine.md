# jquery 悬浮验证框架 jQuery Validation Engine - weixin_33985507的博客 - CSDN博客
2017年03月22日 14:42:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：10
中文api 地址  [http://code.ciaoca.com/jquery/validation-engine/](http://code.ciaoca.com/jquery/validation-engine/)   和bootstarp 一起使用不会像easyui  验证那样生硬
![](https://images2015.cnblogs.com/blog/722496/201703/722496-20170322144128174-123695667.png)
修改版
![](https://images2015.cnblogs.com/blog/722496/201703/722496-20170322144128611-1753353396.png)
原版 包涵实例代码
这里记录几点
ps：验证规则 validate[required] 必须要加上required ，其它加的自定义规则才会生效
$('#id').validationEngine('validate') 直接调用验证不会验证异步  一直都是false，如果不考虑使用异步或自己实现异步，可以考虑使用该方法验证
解决方法：
**1.不使用$('#id').validationEngine('validate')的方法**
- 
```java
$addDiv.find('#addForm').submit();
```
提交
- `$addDiv.find('#addForm').validationEngine({`
- `                        maxErrorsPerField:1,`
- `                        ajaxFormValidation: true,`
- `                        onBeforeAjaxFormValidation: function(){`
- `                            $addDiv.find('#addForm').attr("action","#");//这是把action修改为#是防止表单多次提交`
- `                            $.post((baseURL+'/housemanage/nhousegroupdef/add'),$($addDiv.find('#addForm')).serialize(),function(rec){`
- `                                if(rec.code == "0"){`
- `                                    $.msg.success("新增成功！");`
- `                                    nhousegroupdefglobal.sbtable.bootstrapTable("removeAll")`
- `                                    nhousegroupdefglobal.sbtable.bootstrapTable('refresh');`
- `                                    $addDiv.modal("hide")`
- `                                }else{`
- `                                    $.msg.fail(rec.desc);`
- `                                }`
- `                            },"json").error(function(){`
- `                                $.msg.fail("新增失败！");`
- `                            });`
- `                        }`
- `                    });`
在 初始化属性onBeforeAjaxFormValidation 定义提交前并验证完毕的方法即可
自定义规则的方法 
- `$.validationEngineLanguage.allRules.switch={`
- `			"func": function(field,rules,i,options){`
- `				if(!$(field).val()){`
- `					return false;`
- `				}`
- `				return true;`
- `			},`
- `			"alertText": "* 请选择内容"`
- `		}`
- `		$.validationEngineLanguage.allRules.datecompare={`
- `			"func": function(field,rules,i,options){`
- `				if($('#regEndtime').val()&&$('#regBegintime').val()>$('#regEndtime').val()){`
- `					return false;`
- `				}`
- `				return true;`
- `			},`
- `			"alertText": "* 报名开始时间不能大于报名截止时间，请修改！"`
- `		}`
- `		$.validationEngineLanguage.allRules.lookhousedatecompare={`
- `			"func": function(field,rules,i,options){`
- `				if($('#regEndtime').val()&&$('#lookhousetime').val()<$('#regEndtime').val()){`
- `					return false;`
- `				}`
- `				return true;`
- `			},`
- `			"alertText": "* 看房时间必须大于报名截止时间，请修改！"`
- `		}`
- `		$.validationEngineLanguage.allRules.numcompare={`
- `			"func": function(field,rules,i,options){`
- `				if($('#maxNum').val()&&$('#minNum').val()>$('#maxNum').val()){`
- `					return false;`
- `				}`
- `				return true;`
- `			},`
- `			"alertText": "* 成团最低人数必须不能大于成团最高人数，请修改！"`
- `		}`
- ``
- `		$.validationEngineLanguage.allRules.lookhousenameajax={`
- `			"url": baseURL+"/housemanage/nhousegroupdef/vaild/lookhousename",`
- `			"alertText": "* 看房团名称已被其它用户定义过，请修改！",`
- `			"alertTextLoad": "* 正在确认名称是否有其他人使用，请稍等。"`
- `		}`
建议采用上面的方法 ，不用修改 原生的文件，直接在$.validationEngineLanguage.allRules 属性上扩展规则即可
也可采用继承的方法 
- 
```java
$.extend($.validationEngineLanguage.allRules,{
```
- 
```java
switch:{
```
- 
```java
"func": function(field,rules,i,options){
```
- 
```java
if(!$(field).val()){
```
- 
```java
return false;
```
- 
```java
}
```
- 
```java
return true;
```
- 
```java
},
```
- 
```java
"alertText": "* 请选择内容"
```
- 
```java
}
```
- 
```java
});
```
注意如果使用框架自带的ajax验证 后台代码返回
- 
```java
@RequestMapping("/vaild/lookhousename")
```
- 
```java
@ResponseBody
```
- 
```java
public Object vaildLookHouseName()throws Exception{
```
- 
```java
String fieldId = RequestUtil.getRequestParameter("fieldId");
```
- 
```java
String fieldValue = RequestUtil.getRequestParameter("fieldValue");
```
- 
```java
String backstr = RequestUtil.getRequestParameter("backstr");
```
- 
```java
Object[] ret = null;
```
- 
```java
try {
```
- 
```java
if(backstr.equalsIgnoreCase(fieldValue)){
```
- 
```java
ret = new Object[]{fieldId,true};
```
- 
```java
}else{
```
- 
```java
if(nhousegroupdefService.hasLookHouseName(fieldValue)){
```
- 
```java
ret = new Object[]{fieldId,false};
```
- 
```java
}else{
```
- 
```java
ret = new Object[]{fieldId,true};
```
- 
```java
}
```
- 
```java
}
```
- 
```java
```
- 
```java
}catch (Exception e){
```
- 
```java
e.printStackTrace();
```
- 
```java
logger.error(e.getMessage(), e);
```
- 
```java
}
```
- 
```java
return  ret;
```
- 
```java
```
- 
```java
}
```
2..**使用****$('#id').validationEngine('validate')的方法 ， 由于使用自带的ajax需要动态传入两个参数的话**
ps：不使用原框架对ajax自定义验证的时候
1.如果要跟框架一样提示的风格，成功就要消除错误信息，如果要隐藏错误框，使用：$('#ysfyHouse').parent().find("div[class=formError]").css({opacity: 0}) 并不会冲突的话设置透明度，原框架就是这么弄的,找遍了ajax不设置隐藏框的方法，都不合适 ，最后只能妥协加隐藏框，点保存的时候确认隐藏框的值
2.如果直接alert()的话 ，就只需要设置隐藏框就可以了
- `$('#ysfyHouse').on('blur', function(){`
- `			if(!$(this).val()||$('#ysfyHouse').validationEngine('validate')){ //必须等元素验证完毕才能执行下一步`
- `				return;`
- `			}`
- `			$.post((baseURL+'/housemanage/nhousegroupdef/vaild/lookhousejoin'),{houseid:($("#ysfyHouseid").val()),groupid:(page_exchange_moudel.id)},function(rec){`
- `				if(rec.code == "0"){`
- `					if(rec.data.result){`
- `						$('#ysfyHouse').validationEngine('showPrompt','楼盘已重复，请修改！','error');`
- `						$("#ysfyHouseidajaxval").val("1")`
- `					}else{`
- `						$('#ysfyHouse').validationEngine('showPrompt','成功！','pass');`
- `						$("#ysfyHouseidajaxval").val("0")`
- `					}`
- `				}else{`
- `					alert(rec.desc);`
- `				}`
- `			},"json").error(function(){`
- `				alert("请求失败！");`
- `			});`
- ``
- `		})`

### 附件列表
