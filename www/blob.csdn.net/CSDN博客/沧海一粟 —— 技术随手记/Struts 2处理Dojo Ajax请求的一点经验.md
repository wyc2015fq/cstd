# Struts 2处理Dojo Ajax请求的一点经验 - 沧海一粟 —— 技术随手记 - CSDN博客





2013年04月11日 14:12:40[慢游](https://me.csdn.net/eengel)阅读数：3458








从Struts 1到Struts 2，从Dojo 1.5到Dojo 1.7、1.8，技术一直在进步，经验却需要时时更新。对于Web应用中最常碰到的Ajax请求处理，新版本的Struts 2和dojo 1.7做法就和以前不一样（[在JAVA servlet中返回不同类型的AJAX response](http://blog.csdn.net/eengel/article/details/6316624)）。现在就拿一个例子说明如何从前台用Dojo通过Ajax请求将Json数据送回后台，后台检测后返回信息；顺便也看看到底新技术进步在哪里。



例子：前台录入人员信息,并保存到后台，后台返回保存后的id。



1. 在主页上增加一个按钮，点击时打开填写人员信息的对话框

main.jsp



```
<div style="width: 100%;">
		<button id="new_person_btn" data-dojo-type="dijit.form.Button"
			type="button">New Person</button>
	</div>
<div class="dijitHidden">
		<div data-dojo-type="test.person" style="width: 400px;"
			id="new_person_dlg"></div>
	</div>
<script>
		require([ "dojo/ready", "dojo/on", "dijit/registry"],
				function(ready, on, registry) {
					ready(function() {

						var new_person_btn = registry.byId("new_person_btn");
						on(new_person_btn, "click", function(evt) {
							registry.byId("new_person_dlg").show();
						});

					});

				});
	</script>
```




2. 对话框页面，可以填写人员姓名、描述、是否正式员工

person.jsp

![](https://img-my.csdn.net/uploads/201304/11/1365660652_3396.JPG)


```
<div data-dojo-type="dijit.form.Form" id="new_person_form"
	data-dojo-id="new_person_form" encType="multipart/form-data">
	<table cellspacing="10" style="width: 100%">
		<tr>
			<td><label for="name">Name:</label></td>
			<td><input type="text" id="new_person_name" name="name"
				style="width: 100%" required="true"
				data-dojo-type="dijit.form.ValidationTextBox" /></td>
		</tr>
		<tr>
			<td><label for="description">Description:</label></td>
			<td><textarea id="new_person_description" name="description"
					data-dojo-type="dijit.form.SimpleTextarea" rows="4"
					style="width: 99%"></textarea></td>
		</tr>
		<tr>
			<td><label for="regular">Regular:</label></td>
			<td>
				<div data-dojo-type="dojo.store.Memory" data-dojo-id="new_person_regular_store"
					data-dojo-props="data: [{id: 'y', name: 'yes'}, {id: 'n', name: 'no'}]"></div>
				<input data-dojo-type="dijit.form.ComboBox" value="yes" style="width: 100%" 
				data-dojo-props="store:new_person_regular_store, searchAttr:'name'" name="regular"
				id="new_person_regular" />
			</td>
		</tr>
	</table>
	<div style="text-align: right; margin-right: 6px;">
		<button id="new_person_submit_btn" data-dojo-type="dijit.form.Button">OK</button>
		<button id="new_person_cancel_btn" data-dojo-type="dijit.form.Button">Cancel</button>
	</div>
</div>
```




3. 对话框对应的JavaScript对象，里面包含了所有对话框相关的JS代码和事件处理。这些代码也可以放在person.jsp中，但鉴于分离视图和控制逻辑有诸多好处（[MVC框架中的模型-视图分离问题（一） —— “你必将业务逻辑由显示中分离”](http://blog.csdn.net/eengel/article/details/6182844)），这儿还是分开吧。

person.js

```java
define([ "dojo/_base/declare", "dijit/Dialog", "dijit/_WidgetBase",
		"dijit/_TemplatedMixin", "dojo/on", "dijit/registry", "dojo/_base/xhr",
		"dijit/form/Form", "dijit/form/ValidationTextBox",
		"dijit/form/SimpleTextarea", "dojo/store/Memory", "dijit/form/ComboBox"], function(declare, Dialog, _WidgetBase,
		_TemplatedMixin, on, registry, xhr) {
	return declare("test.person",
			[ Dialog, _WidgetBase, _TemplatedMixin ], {
				title : "New Person",

				postCreate : function() {
					this.set('href', "dialog_newperson.action");

					this.inherited(arguments);
				},

				onDownloadEnd : function() {
					var new_person_form = registry.byId("new_person_form");
					var new_person_submit_btn = registry
							.byId("new_person_submit_btn");
					on(new_person_submit_btn, "click", function(evt) {
						if (!new_person_form.validate()) {
							return false;
						}

						/*为了让后台认识json格式的数据，这儿有几个地方需要注意的。*/
						
						var data = {
							person : dojo.formToObject("new_person_form")
						};
						// Post the data to the server
						xhr.post({
							url : "submit_newperson.action",
							handleAs : "json",
/*注意点1：json数据必须转成json格式*/
/*注意点2：json数据要放在postData中，而不是content中*/
							postData : dojo.toJson(data),
/*注意点3：设置contentType为application/json */
							contentType : 'application/json; charset=utf-8',
							handle : function(response, ioArgs) {
								return response;
							}
						});
					});

					var new_person_cancel_btn = registry
							.byId("new_person_cancel_btn");
					on(new_person_cancel_btn, "click", function(evt) {
						registry.byId("new_person_dlg").hide();
					});

					this.inherited(arguments);
				}
			});
});
```


4. 为了让Struts 2能认识json格式的数据，需要引入struts2-json-plugin包。这儿用Maven管理项目，就可以直接在pom文件中加入相应dependency。如果不用Maven，则直接将jar包加入到项目buildpath中。

pom.xml

```
<dependency>
			<groupId>org.apache.struts</groupId>
			<artifactId>struts2-json-plugin</artifactId>
			<version>2.3.12</version>
		</dependency>
```


5. 重点！struts.xml中需要进行相应配置

struts.xml

```
<!--注意点1：在package的extends里，必须有json-default -->
<package name="workbench" namespace="/"
		extends="struts-default,json-default">
		<action name="submit_newperson" class="test.SubmitNewPersonAction">
			<!--注意点2：在action里，加入json interceptor信息 -->
			<interceptor-ref name="json">
				<param name="contentType">application/json</param>
			</interceptor-ref>
			<!--注意点3：设置result为json格式 -->
			<result type="json">
				<!--注意点4：设置result包含的内容。这儿只包含result变量的数据，而没有person的数据 -->
				<param name="includeProperties">result</param>
			</result>
		</action>
	</package>
```


6. 保存人员信息的java对象

Person.java


```java
public class Person {
	private String name;
    private String description;
    private String regular;
    
    public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public String getDescription() {
		return description;
	}

	public void setDescription(String description) {
		this.description = description;
	}

	public String getRegular() {
		return regular;
	}

	public void setRegular(String regular) {
		this.regular = regular;
	}
}
```


7. 相应的action。接收传入的人员数据，返回相应的id

TestAction.java

```java
public class TestAction extends ActionSupport {

	private static final long serialVersionUID = 1L;

	private int result;
	 
    private Person person;
    
    public void setPerson(Person person) {
		this.person = person;
	}

	public Person getPerson() {
		return person;
	}

	@Override
	public String execute() {
		System.out.println(person);

		int result = savePerson(person);
		return SUCCESS;
	}

	public int getResult() {
		return result;
	}

}
```


8. 最后返回的Response:
`{"result":1}`

总结

相比较Struts 1而言，Struts 2在处理接收的数据方面，确实有很大的改进。数据传送到后台后，不需要手工写代码解析json格式的数据，只要schema一致，就可以用相应的POJO对象接收。




