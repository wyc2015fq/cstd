# Jquery EasyUI +Ajax +Json +一般处理程序 实现数据的前台与后台的交互 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年04月10日 20:53:35[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：328


点击前往：[转载地址](https://blog.csdn.net/lishehe/article/details/16848497)

经过上一篇博客，本节主要是来看实现的功能是后台的数据通过json数据传过来，前台修改的数据再传回数据库之后页面再次更新table中的数据；

 图示：

![](https://img-blog.csdn.net/20131120191958875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGlzaGVoZQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

实例：前台的代码

**[html]**[view plain](https://blog.csdn.net/lishehe/article/details/16848497#)[copy](https://blog.csdn.net/lishehe/article/details/16848497#)

- <spanstyle="font-size:18px;"><%--表格显示区--%>
- <tableid="tt" " title="设置打分规则"class="easyui-datagrid"style="width: auto;  
-  height: 400px;" idfield="itemid"pagination="true"iconcls="                                                                icon-save"remoteSort="false"
- data-options="rownumbers:true,  
-         url:'JSONData.ashx/ProcessRequest',pageSize:5,pageList:[5,10,15,20],  
-         method:'get',toolbar:'#tb' ," fitcolumns="true"striped="true">
- <%--url是获取数据的地址--%>
- <%--url:'JSON.ashx/ProcessRequest'--%>
- <%-- 表格标题--%>
- <thead>
- <tr>
- <thdata-options="field:'ScoreStyleID',checkbox:true"> 打分项目类别ID</th>
- <thdata-options="field:'ScoreStyleName',width:100",sortable:"true">打分项目类别名称</th>
- <thdata-options="field:'ScoreStyleState',width:120,align:'right'">打分项目类别描述</th>
- <thdata-options="field:'Score',width:120,align:'right'">打分项目单位分数</th>
- <thdata-options="field:'ScoreItem',width:100">打分项目单位</th>
- <thdata-options="field:'AdminID',width:100">管理员</th>
- <thdata-options="field:'PublishDate',width:100">发布时间</th>
- </tr>
- </thead>
- <%--表格内容--%>
- <tbody>
- 
- </tbody>
- </table></span>

上面主要是设定table的属性，具体每个属性大家可以详细查中文文挡来深入学习

下面是关于增删改查的代码：

**[javascript]**[view plain](https://blog.csdn.net/lishehe/article/details/16848497#)[copy](https://blog.csdn.net/lishehe/article/details/16848497#)

- <span style="font-size:18px;"> var url;  
- //添加管理员
- function newUser() {  
-             $('#dlg').dialog('open').dialog('setTitle', '添加管理员');//设定表头
-             $('#fm').form('clear');//清空窗体数据
-             document.getElementById("test").value = "add";//设定表示为后台调不同方法数据提供接口
- 
-         }  
- //修改管理员
- function editUser() {  
-             $('#fm').form('clear');  
- var row = $('#tt').datagrid('getSelected');  
- if (row) {  
-                 $('#dlg').dialog('open').dialog('setTitle', '修改管理员');  
- //绑定数据列表
-                 $('#firstname').val(row.ScoreStyleName);  
-                 $('#lastname').val(row.Score);  
-                 $('#unit').val(row.ScoreItem);  
-                 $('#message').val(row.ScoreStyleState);  
-                 $('#ID').val(row.ScoreStyleID);  
-                 document.getElementById("test").value = "modify";//设定表示为后台调不同方法数据提供接口
-                 $('#fm').form('load', row);  
- 
-             }  
-         }  
- //保存信息
- function saveUser() {  
- //获取页面中输入的值
- var firstname = document.getElementById("firstname").value;  
- var lastname = document.getElementById("lastname").value;  
- var test = document.getElementById("test").value;  
- var unit = document.getElementById("unit").value;;  
- var message = document.getElementById("message").value;  
- var ID = document.getElementById("ID").value;  
- //var t = "firstname=" + firstname + "&lastname=" + lastname + "&unit=" + unit + "&message=" + message + "&ID=" + ID;
- 
- 
-             $('#fm').form('submit', {  
- //设定地址与传递参数到后台
-                 url: "JSONData.ashx?firstname=" + firstname + "&lastname=" + lastname + "&unit=" + unit + "&message=" + message + "&test=" + test + "&ID=" + ID,  
-                 onSubmit: function () {  
- return $(this).form('validate');  
-                 },  
- //判断结果是否正确
-                 success: function (result) {  
- if (result.indexOf("T") == 0) {  
- 
-                         alert('恭喜您，信息添加成功！')  
-                         $('#dlg').dialog('close');      // 关闭窗口
-                         $('#tt').datagrid('reload');  
-                     }  
- else {  
-                         alert('保存失败，请您核对！')  
-                     }  
- var result = eval('(' + result + ')');  
- if (result.success) {  
-                         $('#dlg').dialog('close');      // close the dialog
-                         $('#tt').datagrid('reload');  
-                         $.message.alert('提示', '保存成功！', 'info');  
- 
- // reload the user data
-                     } else {  
- //$.messager.show({
- //    title: 'Error',
- //    msg: result.msg
- //});
-                     }  
-                 }  
-             });  
-         }  
- //删除管理员
- function removeUser() {  
-             document.getElementById("test").value = "delete";//设定表示为后台调不同方法数据提供接口
- var test = document.getElementById("test").value;  
- var row = $('#tt').datagrid('getSelected');  
- if (row) {  
-                 $.messager.confirm('提示', '你确定要删除这条信息吗？', function (r) {  
- if (r) {  
-                         $('#fm').form('submit', {  
-                             url: "JSONData.ashx?ID=" + row.ScoreStyleID + "&test=" + test,  
-                             onSubmit: function () {  
-                             },  
- //判断结果是否正确
-                             success: function (result) {  
- if (result.indexOf("T") == 0) {  
-                                     $('#dlg').dialog('close');  
-                                     alert('恭喜您，信息删除成功！')  
- // close the dialog
-                                     $('#tt').datagrid('reload');  
- //$('#fm').form('submit');
-                                 }  
- else {  
-                                     alert('添加失败，请重新操作！')  
-                                 }  
- var result = eval('(' + result + ')');  
- 
-                             }  
-                         });  
-                     }  
-                 }  
-                 );  
-             }  
-         }  
- //查询并把数据传递给后台，数组
- function getQueryParams(queryParams) {  
- var StartTime = $("#StartTime").datebox("getValue");  
- var EndTime = $("#EndTime").datebox("getValue");  
- var KeyWord = document.getElementById("KeyWord").value;  
- var Category = $("#Category").combobox("getValue");  
-             queryParams.StartTime = StartTime;  
-             queryParams.EndTime = EndTime;  
-             queryParams.KeyWord = KeyWord;  
-             queryParams.Category = Category;  
- return queryParams;  
-         }  
- 
- //增加查询参数，重新加载表格
- function reloadgrid() {  
- 
- //查询参数直接添加在queryParams中    
- 
- var queryParams = $('#tt').datagrid('options').queryParams;  
-             getQueryParams(queryParams);  
-             $('#tt').datagrid('options').queryParams = queryParams;//传递值
- 
-             $("#tt").datagrid('reload');//重新加载table
- 
-         }</span>  

 接下来是后台中的一般处理程序中的代码（主要是负责读取数据库中的数据）

首先是判断前台请求的是哪种方法：

**[csharp]**[view plain](https://blog.csdn.net/lishehe/article/details/16848497#)[copy](https://blog.csdn.net/lishehe/article/details/16848497#)

- <span style="font-size:18px;">/// <summary>
- /// 条件查询公告
- /// </summary>
- /// <param name="context"></param>
- publicvoid ProcessRequest(HttpContext context)//context中包含前台与后台来回传递的值
-         {//判断前台请求的是增删改查的哪一个
- string command = context.Request.QueryString["test"];//前台传的标示值
- if (command == "add")  
-             {//调用添加方法
-                 Add(context);  
-             }  
- elseif (command == "delete")  
-             {//调用删除方法
-                 Delete(context);  
-             }  
- elseif (command == "modify")  
-             {//调用修改方法
-                 Modify(context);  
-             }  
- else
-             {//调用查询方法
-                 Query(context);  
-             }  
-         }  
- </span>  

 查询中的方法：

**[csharp]**[view plain](https://blog.csdn.net/lishehe/article/details/16848497#)[copy](https://blog.csdn.net/lishehe/article/details/16848497#)

- <span style="font-size:18px;"> //================================================================
- //获取分页和排序信息：页大小，页码，排序方式，排序字段
- int pageRows, page;  
-             pageRows = 10;  
-             page = 1;  
- string order, sort, oderby; order = sort = oderby = "";  
- if (null != context.Request.QueryString["rows"])  
-             {  
-                 pageRows = int.Parse(context.Request.QueryString["rows"].ToString().Trim());  
- 
-             }  
- if (null != context.Request.QueryString["page"])  
-             {  
- 
-                 page = int.Parse(context.Request.QueryString["page"].ToString().Trim());  
- 
-             }  
- if (null != context.Request.QueryString["sort"])  
-             {  
- 
-                 order = context.Request.QueryString["sort"].ToString().Trim();  
- 
-             }  
- if (null != context.Request.QueryString["order"])  
-             {  
- 
-                 sort = context.Request.QueryString["order"].ToString().Trim();  
- 
-             }  
- 
- 
- //===================================================================
- //组合查询语句：条件+排序
-             StringBuilder strWhere = new StringBuilder();  
- if (key != "")  
-             {  
-                 strWhere.AppendFormat(" ScoreStyleName like '%{0}%' and ", key);  
-             }  
- if (category != "")  
-             {  
-                 strWhere.AppendFormat(" AdminID= '{0}' and ", category);  
-             }  
- if (startTime != "")  
-             {  
-                 strWhere.AppendFormat(" PublishDate >= '{0}' and ", startTime);  
-             }  
- if (endTime != "")  
-             {  
-                 strWhere.AppendFormat(" PublishDate <= '{0}' and ", endTime);  
-             }  
- 
- //删除多余的and
- int startindex = strWhere.ToString().LastIndexOf("and");//获取最后一个and的位置
- if (startindex >= 0)  
-             {  
-                 strWhere.Remove(startindex, 3);//删除多余的and关键字
-             }  
- if (sort != "" && order != "")  
-             {  
- //strWhere.AppendFormat(" order by {0} {1}", sort, order);//添加排序
-                 oderby = order + " " + sort;  
-             }  
- //DataSet ds = Bnotice.GetList(strWhere.ToString());  //调用不分页的getlist
- 
- //调用分页的GetList方法
-             DataSet ds = scorebll.GetListByPage(strWhere.ToString(), oderby, (page - 1) * pageRows + 1, page * pageRows);  
- int count = scorebll.GetRecordCount(strWhere.ToString());//获取条数
- string strJson = ToJson.Dataset2Json(ds, count);//DataSet数据转化为Json数据
-             context.Response.Write(strJson);//返回给前台页面
-             context.Response.End();</span>  

      这个过程很是漫长，在调试的过程中，遇到了各种各样的问题，主要是浏览器的缓存问题耽误了好长时间，龙哥给了大力的支持，通过实现此功能，对ajax，一般处理程序的理解深刻多了；框架的使用提高我们开发的效率；

      Json+ajax数据界面不刷新，页面更加的贴近客户的需求，查询效率大大的提升。

  接下来会给大家分享学习分页（自己查找资料中学习总结的知识），查询数据的分页的知识与实例。敬请期待……



