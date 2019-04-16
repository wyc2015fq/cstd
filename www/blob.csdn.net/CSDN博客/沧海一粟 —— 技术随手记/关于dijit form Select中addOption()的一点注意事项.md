# 关于dijit.form.Select中addOption()的一点注意事项 - 沧海一粟 —— 技术随手记 - CSDN博客





2013年05月11日 10:14:05[慢游](https://me.csdn.net/eengel)阅读数：4686








dijit.form.Select的addOption()函数是一个很好的功能，方便我们用编程的方法动态添加<option>结点到Select中。

不过如果遇到下面这样的问题，希望不要重走本人的老路，花冤枉时间。(Dojo 1.7.3)



HTML代码



```
<table>
		<tr>
			<td><label for="cb1">Check Box 1:</label>
			</td>
			<td><select id="cb1"
				name="cb1" style="width: 100px"
				data-dojo-type="dijit.form.Select">
			</select></td>
		</tr>
		<tr>
			<td><label for="cb2">Check Box 2:</label>
			</td>
			<td><select id="cb2"
				name="cb2" style="width: 100px"
				data-dojo-type="dijit.form.Select">
			</select></td>
		</tr>
	</table>
```




JS代码

```java
require(
				[ "dojo/ready", "dojo/_base/xhr", "dojo/json", "dijit/registry", "dojo/_base/array" ],
				function(ready, xhr, JSON, registry, array) {
					ready(function() {
						var cb1 = registry.byId("cb1");
						var cb2 = registry.byId("cb2");
						
						var options = [
							{
								value: "op1",
								label: "option 1"
							},{
								value: "op2",
								label: "option 2"
							}
						];
						array.forEach(options, function(option){
							cb1.addOption(option);
							cb2.addOption(option);
						});
						
						cb1.set("value", "op1");
						cb2.set("value", "op2");
					});
				});
```


从JS代码来看，cb1当前选中的是op1，cb2当前选中的是op2。但是如果用cb1.get(“value”)取得当前选中的值，得到的却是”op2”！

问题就出在同一个options变量被同时赋给了cb1和cb2，这样做似乎导致它们的option有了关联。

改成下面这种实现方法就可以避免这个问题了：

JS代码

```java
require(
				[ "dojo/ready", "dojo/_base/xhr", "dojo/json", "dijit/registry", "dojo/_base/array" ],
				function(ready, xhr, JSON, registry, array) {
					ready(function() {
						var cb1 = registry.byId("cb1");
						var cb2 = registry.byId("cb2");
						
						var options1 = [
							{
								value: "op1",
								label: "option 1"
							},{
								value: "op2",
								label: "option 2"
							}
						];
						var options2 = [
										{
											value: "op1",
											label: "option 1"
										},{
											value: "op2",
											label: "option 2"
										}
									];
						array.forEach(options1, function(option){
							cb1.addOption(option);
						});
						array.forEach(options2, function(option){
							cb2.addOption(option);
						});
						
						cb1.set("value", "op1");
						cb2.set("value", "op2");
					});
				});
```


有兴趣的可以追溯一下dijit.form.Select的源码。




