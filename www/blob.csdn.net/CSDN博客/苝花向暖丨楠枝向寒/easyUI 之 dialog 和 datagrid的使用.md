# easyUI 之 dialog 和 datagrid的使用 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年04月19日 12:29:59[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：357


`子JSP页面`
```
<table width="100%" id="viewDetailTable">
	<thead>
		<tr>
			<th data-options="field:'obj_code'" style="width:40%;">工作包编号</th>
		</tr>
	</thead>
</table>
```

```java
$(function() {

			var $schcode = $("#toolbarLap_sch_code").combobox('getValue');       //从父jsp页面的input框中获取value值
			var $techcode = $("#toolbarLap_jobtype").combobox('getValue');
			var $TableName = $("#toolbarLap_calendar").combobox('getValue');
		
		$('#viewDetailTable').datagrid({
			url : "assemblePlan/StartqueryScheduleDataList1.do?sch_code="         //get请求方式，通过url传参数
				+ $schcode + "&tech_code=" + $techcode,
			method : 'get',
			rownumbers: true,
			pagination: true,           //显示页码
			pageSize : 10,              //默认一页显示 10条数据
			pageList : [10,20,30,40,50],  //可选 的显示数据条数 的值   
			border : false,
			nowrap : true,
			fit : true,
			singleSelect : false,
		})
	})
```

```
//父jsp页面
	        $('<div></div>').dialog({            //直接通过$('<div><div>') 创建并选择了一个div
                 id : "AssembleDialog", //给div一个id title : '问题工作包', //对话框的标题 
		 width : 600, 
		 height : 450, 
		 closed : false, 
 		 cache : false, 
	         href : 'hull/workAreaPlan/AssembleDialog.jsp', //对话框中显示的内容来源于 另一个jsp页面 
		 modal : true,                 // 弹出对话框以后 后面的内容不允许点击 
		 buttons : [{                  //设置对话框中的按钮 
		 	text : '关闭', //按钮显示的文字 
	         	handler : function() { 
				$('#AssembleDialog').dialog('destroy'); //点击以后 做的事 
			} 
			} ]
		 });
```

细节解释：

datagrid 中，即时不向后台传递参数，也会自动的传递  page （当前页数） 和 rows（每页显示的数据数即pageList所选择的数），所以后端 需要有两个全局变量。 private int page 和 private int rows，如果用的Struts 2  就会被自动赋值。

然后通过page、rows 将从数据库中查询到的结果，进行分页。 

如 返回的结果是 List<Map<String,Object>>  results，  

通过    results.subList((page - 1) * rows, page * rows > totalCount ? totalCount : page * rows))

从  （当前页数-1）*每页数据数   截到   当前页数 * 每页数据数

通过    results.size()方法获取从数据库查出来的数据总数。 然后把 这两个结果 存入 map中 。

key 分别是rows (对应的value是截取的结果集)  和  total (数据总数)，然后转化为json格式，传给前端。

前端会自动接收，并自动生成 table的 tbody 。  

注意 ：  data-options="field:'obj_code'"    这个 filed 的值  必须和 数据库中字段名 一样，否则无法自动生成。


