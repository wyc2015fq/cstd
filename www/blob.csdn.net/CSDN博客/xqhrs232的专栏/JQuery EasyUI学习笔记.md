# JQuery EasyUI学习笔记 - xqhrs232的专栏 - CSDN博客
2018年11月02日 15:21:39[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：49
个人分类：[JavaWeb技术](https://blog.csdn.net/xqhrs232/article/category/8231541)
原文地址::[https://www.cnblogs.com/ygj0930/p/6690888.html](https://www.cnblogs.com/ygj0930/p/6690888.html)
相关文章
1、[jQuery UI 之 EasyUI 快速入门](https://www.cnblogs.com/soulaz/p/5586613.html)----[https://www.cnblogs.com/soulaz/p/5586613.html](https://www.cnblogs.com/soulaz/p/5586613.html)
2、jQueryEasyUI百度百科----[https://baike.baidu.com/item/jQueryEasyUI/1733327?fr=aladdin](https://baike.baidu.com/item/jQueryEasyUI/1733327?fr=aladdin)
3、EasyUI 简介----[http://www.jeasyui.net/](http://www.jeasyui.net/)
4、TopJUI前端框架----[http://demo.topjui.com/?s=bdtg](http://demo.topjui.com/?s=bdtg)
**转载请注明原文地址：[http://www.cnblogs.com/ygj0930/p/6690888.html](http://www.cnblogs.com/ygj0930/p/6690888.html)**
**简介与准备**
jQuery EasyUI 是一个基于 jQuery 的框架，集成了各种用户界面插件。
准备：下载：http://www.jeasyui.com/download/index.php
导入：
<link rel="stylesheet" type="text/css" href="easyui/themes/default/easyui.css">
<link rel="stylesheet" type="text/css" href="easyui/themes/icon.css">
<script type="text/javascript" src="easyui/jquery-1.7.2.min.js"></script>
<script type="text/javascript" src="easyui/jquery.easyui.min.js"></script>
使用插件：如使用EasyUI提供的面板：
<div id="p" class="easyui-panel" style="width:500px;height:200px;padding:10px;"
    title="My Panel" iconCls="icon-save" collapsible="true">
    This is EasyUI！
</div>
一：拖动效果
$(选择元素).draggable();
二：菜单
创建菜单：一个div包含一个菜单项，div嵌套div就是子菜单，并列的div是同级的菜单项。
    <div id="mm" class="easyui-menu" style="width:120px;">//使用easyui-menu菜单插件创建菜单
        <div>New</div>
        <div>
            <span>Open</span>//包含子菜单
            <div style="width:150px;">
                <div>Word</div>
                <div>Excel</div>
                <div>PowerPoint</div>
            </div>
        </div>
        <div>Save</div>
        <div>Exit</div>
    </div>
显示与隐藏菜单：调用 'show' 方法显示或者调用 'hide' 方法隐藏，可以设置为右键事件show。
三：按钮
链接按钮：把普通的链接以按钮图标显示。
使用<a>标签创建链接，href属性为链接url，class 属性为'easyui-linkbutton' ，iconCls属性为按钮图标，<a>标签之间值为按钮文本。
<a href="#" class="easyui-linkbutton" iconCls="icon_cancel">Cancel</a>
菜单按钮：点击该按钮时弹出一个下拉菜单。
定义一个<a>标签和一个菜单，href属性为连接url，class属性为"easyui-menubutton",iconCls属性为按钮图标，menu属性为菜单id，标签之间值为按钮文本。
<a href="#" class="easyui-menubutton" menu="#menu_help" iconCls="icon_help">Help</a>
分割按钮：与菜单按钮相似，不同的是，分割按钮被分割为两部分。当移动鼠标到分割按钮上时，将显示一条分割线。只有当移动鼠标在分割按钮的右侧部分时才显示菜单。
定义同菜单按钮，只不过class属性为easyui-splitbutton。
<a href="#" class="easyui-splitbutton" menu="#menu" iconCls="icon_edit">Edit</a>
四：网格
1：创建表格
表格插件：<table id="table" class="easyui-datagrid" style="">
表头：列的名称信息定义在 <thead> 标签之间，<tr>为一行，<th>标签间为列名， field属性指定列id。
表数据：定义在 <tbody> 标签中。<tr>为一行，<td>之间为一列值。
<table id="tt" class="easyui-datagrid" style="width:400px;height:auto;">
        <thead>//定义表头
            <tr>
                <th field="name1" width="50">Col 1</th>//列名
                <th field="name2" width="50">Col 2</th>
                <th field="name3" width="50">Col 3</th>
            </tr>                          
        </thead>                           
        <tbody>   //表内容                         
            <tr>                           
                <td>Data 1</td>            
                <td>Data 2</td>            
                <td>Data 3</td>                    
            </tr>                          
            <tr>                           
                <td>Data 1</td>            
                <td>Data 2</td>            
                <td>Data 3</td>                   
            </tr>                                               
        </tbody>                           
    </table>
2：获得表格选中行
var rows = $('#tableid').datagrid('getSelections');
3：添加工具栏 
定义一个工具类div并指定id，然后集成到网格中。
 <table id="tid" class="easyui-datagrid" style="width:600px;height:250px"，**toolbar="#toolbarid">**
4:格式化列数据
<th  formatter="formatfunction">列值</th>
定义一个formatfunction函数对列值的格式化显示，比如：红色字体，下划线。
5：设置按列值排序
<th field="列名" sortable="true">列值</th>
6：添加复选框
表头创建一列为复选框列即可：<th field="check_box" checkbox="true"></th>
7：分页显示
<table id="tid" class="easyui-datagrid" **pagination****=****"true"**>
设置 'pagination' 属性为 true，它将在数据网格的底部生成一个分页工具栏。pagination将发送两个参数到服务器：
- page：页码，起始值 1。
- rows：每页显示行。
8：属性表格
<table id="tid" class="easyui-propertygrid" style="width:300px" url="properties.json" showGroup="true"scrollbarSize="0"></table>
9：滚动条实现分页查询
 当滚动垂直滚动条时，数据网格（datagrid）执行 ajax 请求来加载和刷新现有的记录，整个刷新的行为过程平稳没有闪烁。
<table id="tid" class="easyui-datagrid" style="width:700px;height:300px" data-options="view:scrollview,rownumbers:true,singleSelect:true,
**url:'ajax.do'**,**pageSize:50**">
不需要使用 pagination 属性，而用pageSize 属性。滚动时执行 ajax 请求向url申请数据来显示。
五：树形菜单
1：创建树形菜单
树形菜单可以定义在 <ul> 元素中，设置class="easyui-tree"。一个 <li> 元素产生一个树节点，子 <ul> 元素将产生一子树菜单。
<ul class="easyui-tree">
        <li>
            <span>Folder</span>
            <ul>
                <li>
                    <span>Sub Folder 1</span>
                    <ul>//子树
                        <li><span>File 11</span></li>
                        <li><span>File 12</span></li>
                        <li><span>File 13</span></li>
                    </ul>
                </li>
                <li><span>File 2</span></li>
                <li><span>File 3</span></li>
            </ul>
        </li>
        <li><span>File21</span></li>
    </ul>
2：创建带复选框的树形菜单，可以从JSON文件提取数据根据层级转化成树形菜单
<ul id="tt" class="easyui-tree"
            url="tree_data.json"
            checkbox="true">//开启复选框功能
</ul>
3：创建树形网格
<table id="test"** class="easyui-treegrid"** style="width:400px;height:300px"
            url="tree_data.json"//从JSON文件加载树形菜单数据
            rownumbers="true"
            idField="id" treeField="name">
        <thead>//创建网格列头信息
            <tr>
                <th field="name" width="160">Name</th>
                <th field="size" width="60" align="right">Size</th>
                <th field="date" width="100">Modified Date</th>
            </tr>
        </thead>
    </table>
六：表单
 1：Ajax表单
//创建普通表单
<form id="formid" action="servlet.do" method="post">
    <input type="submit" value="Submit"></input>
</form>
//为表单添加回调函数成为Ajax表单
$('#formid').form({
        success:function(data){
            //data为返回结果
        }
    });
2：表单验证
为input标签指定class="easyui-validatebox"验证插件，type=""指定输入数据类型即可。
<input class="easyui-validatebox" type="text" name="subject" required="true"></input>
七：窗口
1：普通窗口
创建一个div，指定class属性为easyui-window即可，div标签之间为窗口内容。
2：窗口的打开与隐藏
$('#win').window('open');
$('#win').window('close');
3：复杂窗口
Layout可以嵌套到窗口div中，从而构建复杂的窗口。
4：普通对话框
创建一个div，指定class属性为easyui-dialog即可，div标签之间为对话框内容，可以通过toolbar="#"属性自定义对话框的工具栏，buttons属性指定对话框按钮。
    <div id="dd" class="easyui-dialog" style="padding:5px;width:400px;height:200px;"
            title="My Dialog" iconCls="icon-ok"**            toolbar="#dlg-toolbar" buttons="#dlg-buttons"**>
        Dialog Content.
    </div>
八：布局
1：边框布局
边框布局提供五个区域：east、west、north、south、center。
- north 区域可以用来显示网站的标语。
- south 区域可以用来显示版权以及一些说明。
- west 区域可以用来显示导航菜单。
- east 区域可以用来显示一些推广的项目。
- center 区域可以用来显示主要的内容。
应用布局，只需要创建一个div指定class="easyui-layout"作为布局容器，然后内嵌套各区域即可。各个区域的创建：<divregion="east、west、north、south、center">
2：标签面板：**easyui-tabs**
    <div **class="easyui-tabs"** style="width:400px;height:100px;">
       //一个div一个标签页
        <div title="First Tab" style="padding:10px;">
            First Tab
        </div>
        <div title="Second Tab" closable="true" style="padding:10px;">
            Second Tab
        </div>
        <div title="Third Tab" iconCls="icon-reload" closable="true" style="padding:10px;">
            Third Tab
        </div>
    </div>
