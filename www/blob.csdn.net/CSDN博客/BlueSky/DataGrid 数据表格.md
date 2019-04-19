# DataGrid  数据表格 - BlueSky - CSDN博客
2016年01月20日 14:52:50[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：575
扩展自 $.fn.panel.defaults ，用 $.fn.datagrid.defaults 重写了 defaults 。
**依赖**
- panel
- resizable
- linkbutton
- pagination
**用法**
1.  **<table**id="tt"**></table>**
1.  $('#tt').datagrid({   
2.      url:'datagrid_data.json',   
3.      columns:[[   
4.          {field:'code',title:'Code',width:100},
   
5.          {field:'name',title:'Name',width:100},
   
6.          {field:'price',title:'Price',width:100,align:'right'}
   
7.      ]]   
8.  });  
**数据表格（DataGrid）的特性**
其特性扩展自 panel，下列是为 datagrid 增加的特性。
|**名称**|**类型**|**说明**|**默认值**|
|----|----|----|----|
|columns|array|datagrid 的 column 的配置对象，更多详细请参见 column 的特性。|null|
|frozenColumns|array|和列的特性一样，但是这些列将被冻结在左边。|null|
|fitColumns|boolean|True 就会自动扩大或缩小列的尺寸以适应表格的宽度并且防止水平滚动。|false|
|striped|boolean|True 就把行条纹化。（即奇偶行使用不同背景色）|false|
|method|string|请求远程数据的 method 类型。|post|
|nowrap|boolean|True 就会把数据显示在一行里。|true|
|idField|string|标识字段。|null|
|url|string|从远程站点请求数据的 URL。|null|
|loadMsg|string|当从远程站点加载数据时，显示的提示信息。|Processing, please wait …|
|pagination|boolean|True 就会在 datagrid 的底部显示分页栏。|false|
|rownumbers|boolean|True 就会显示行号的列。|false|
|singleSelect|boolean|True 就会只允许选中一行。|false|
|pageNumber|number|当设置了 pagination 特性时，初始化页码。|1|
|pageSize|number|当设置了 pagination 特性时，初始化页码尺寸。|10|
|pageList|array|当设置了 pagination 特性时，初始化页面尺寸的选择列表。|[10,20,30,40,50]|
|queryParams|object|当请求远程数据时，发送的额外参数。|{}|
|sortName|string|定义可以排序的列。|null|
|sortOrder|string|定义列的排序顺序，只能用 'asc' 或 'desc'。|asc|
|remoteSort|boolean|定义是否从服务器给数据排序。|true|
|showFooter|boolean|定义是否显示一行页脚。|false|
|rowStyler|function|返回例如 'background:red' 的样式，该函数需要两个参数：rowIndex： 行的索引，从 0 开始。rowData： 此行相应的记录。||
|loadFilter|function|返回过滤的数据去显示。这个函数需要一个参数 'data' ，表示原始数据。你可以把原始数据变成标准数据格式，此函数必须返回标准数据对象，含有 'total' 和 'rows'特性。||
|editors|object|定义编辑行时的 editor 。|预定义的 editor|
|view|object|定义 datagrid 的 view 。|默认的 view|
**列（Column）的特性**
DataGrid 的 Column 是一个数组对象，它的每个元素也是一个数组。数组元素的元素是一个配置对象，它定义了每个列的字段。
代码示例：
1.  columns:[[   
2.      {field:'itemid',title:'Item ID',rowspan:2,width:80,sortable:**true**},
   
3.      {field:'productid',title:'Product ID',rowspan:2,width:80,sortable:**true**},
   
4.      {title:'Item Details',colspan:4}   
5.  ],[   
6.      {field:'listprice',title:'List Price',width:80,align:'right',sortable:**true**},
   
7.      {field:'unitcost',title:'Unit Cost',width:80,align:'right',sortable:**true**},
   
8.      {field:'attr1',title:'Attribute',width:100},
   
9.      {field:'status',title:'Status',width:60}
   
10. ]]  
|**名称**|**类型**|**说明**|**默认值**|
|----|----|----|----|
|title|string|列的标题文字。|undefined|
|field|string|列的字段名。|undefined|
|width|number|列的宽度。|undefined|
|rowspan|number|指一个单元格占据多少行。|undefined|
|colspan|number|指一个单元格占据多少列。|undefined|
|align|string|指如何对齐此列的数据，可以用 'left'、'right'、'center'。|undefined|
|sortable|boolean|True 就允许此列被排序。|undefined|
|resizable|boolean|True 就允许此列被调整尺寸。|undefined|
|hidden|boolean|True 就隐藏此列。|undefined|
|checkbox|boolean|True 就显示 checkbox。|undefined|
|formatter|function|单元格的格式化函数，需要三个参数： value： 字段的值。rowData： 行的记录数据。rowIndex： 行的索引。|undefined|
|styler|function|单元格的样式函数，返回样式字符串来自定义此单元格的样式，例如 'background:red' 。此函数需要三个参数：value： 字段的值。rowData： 行的记录数据。rowIndex： 行的索引。|undefined|
|sorter|function|自定义字段的排序函数，需要两个参数：a： 第一个字段值。b： 第二个字段值。|undefined|
|editor|string,object|指编辑类型。当是 string 时指编辑类型，当 object 时包含两个特性： type：string，编辑类型，可能的类型是： text、textarea、checkbox、numberbox、validatebox、datebox、combobox、combotree。options：对象，编辑类型对应的编辑器选项。|undefined|
**编辑器（Editor）**
用 $.fn.datagrid.defaults.editors 重写了 defaults。
每个编辑器有下列行为：
|**名称**|**参数**|**说明**|
|----|----|----|
|init|container, options|初始化编辑器并且返回目标对象。|
|destroy|target|如果必要就销毁编辑器。|
|getValue|target|从编辑器的文本返回值。|
|setValue|target , value|给编辑器设置值。|
|resize|target , width|如果必要就调整编辑器的尺寸。|
例如，文本编辑器（text editor）像下面这样定义：
1.  $.extend($.fn.datagrid.defaults.editors, {   
2.      text: {   
3.          init: **function**(container, options){   
4.  **var** input = $('<input type="text" class="datagrid-editable-input">').appendTo(container);
   
5.  **return** input;   
6.          },   
7.          getValue: **function**(target){   
8.  **return** $(target).val();   
9.          },   
10.         setValue: **function**(target, value){   
11.             $(target).val(value);   
12.         },   
13.         resize: **function**(target, width){   
14. **var** input = $(target);   
15. **if** ($.boxModel == **true**){
   
16.                 input.width(width - (input.outerWidth() - input.width()));   
17.             } **else** {   
18.                 input.width(width);   
19.             }   
20.         }   
21.     }   
22. });  
**数据表格视图（DataGrid View）**
用 $.fn.datagrid.defaults.view 重写了 defaults。
view 是一个对象，它告诉 datagrid 如何呈现行。这个对象必须定义下列方法。
|**名称**|**参数**|**说明**|
|----|----|----|
|render|target, container, frozen|当数据加载时调用。target：DOM 对象，datagrid 对象。container：行的容器。frozen：表示是否呈现冻结容器。|
|renderFooter|target, container, frozen|这是呈现行脚选项的函数。|
|renderRow|target, fields, frozen, rowIndex, rowData|这是选项的函数，将会被 render 函数调用。|
|refreshRow|target, rowIndex|定义如何刷新指定的行。|
|onBeforeRender|target, rows|视图被呈现前触发。|
|onAfterRender|target|视图被呈现后触发。|
**事件**
其事件扩展自 panel，下列是为 datagrid 增加的事件。
|**名称**|**参数**|**说明**|
|----|----|----|
|onLoadSuccess|data|当数据加载成功时触发。|
|onLoadError|none|加载远程数据发生某些错误时触发。|
|onBeforeLoad|param|发送加载数据的请求前触发，如果返回 false加载动作就会取消。|
|onClickRow|rowIndex, rowData|当用户点击一行时触发，参数包括： rowIndex：被点击行的索引，从 0 开始。rowData：被点击行对应的记录。|
|onDblClickRow|rowIndex, rowData|当用户双击一行时触发，参数包括： rowIndex：被双击行的索引，从 0 开始。rowData：被双击行对应的记录。|
|onClickCell|rowIndex, field, value|当用户单击一个单元格时触发。|
|onDblClickCell|rowIndex, field, value|当用户双击一个单元格时触发。|
|onSortColumn|sort, order|当用户对一列进行排序时触发，参数包括： sort：排序的列的字段名order：排序的列的顺序|
|onResizeColumn|field, width|当用户调整列的尺寸时触发。|
|onSelect|rowIndex, rowData|当用户选中一行时触发，参数包括： rowIndex：选中行的索引，从 0 开始rowData：选中行对应的记录|
|onUnselect|rowIndex, rowData|当用户取消选择一行时触发，参数包括： rowIndex：取消选中行的索引，从 0 开始rowData：取消选中行对应的记录|
|onSelectAll|rows|当用户选中全部行时触发。|
|onUnselectAll|rows|当用户取消选中全部行时触发。|
|onBeforeEdit|rowIndex, rowData|当用户开始编辑一行时触发，参数包括： rowIndex：编辑行的索引，从 0 开始rowData：编辑行对应的记录|
|onAfterEdit|rowIndex, rowData, changes|当用户完成编辑一行时触发，参数包括： rowIndex：编辑行的索引，从 0 开始rowData：编辑行对应的记录changes：更改的字段/值对|
|onCancelEdit|rowIndex, rowData|当用户取消编辑一行时触发，参数包括： rowIndex：编辑行的索引，从 0 开始rowData：编辑行对应的记录|
|onHeaderContextMenu|e, field|当 datagrid 的头部被右键单击时触发。|
|onRowContextMenu|e, rowIndex, rowData|当右键点击行时触发。|
**方法**
|**名称**|**参数**|**说明**|
|----|----|----|
|options|none|返回 options 对象。|
|getPager|none|返回 pager 对象。|
|getPanel|none|返回 panel 对象。|
|getColumnFields|frozen|返回列的字段，如果 frozen 设定为 true，冻结列的字段被返回。|
|getColumnOption|field|返回指定列的选项。|
|resize|param|调整尺寸和布局。|
|load|param|加载并显示第一页的行，如果指定 param 参数，它将替换 queryParams 特性。|
|reload|param|重新加载行，就像 load 方法一样，但是保持在当前页。|
|reloadFooter|footer|重新加载脚部的行。|
|loading|none|显示正在加载状态。|
|loaded|none|隐藏正在加载状态。|
|fitColumns|none|使列自动展开/折叠以适应 datagrid 的宽度。|
|fixColumnSize|none|固定列的尺寸。|
|fixRowHeight|index|固定指定行的高度。|
|loadData|data|加载本地数据，旧的行会被移除。|
|getData|none|返回加载的数据。|
|getRows|none|返回当前页的行。|
|getFooterRows|none|返回脚部的行。|
|getRowIndex|row|返回指定行的索引，row 参数可以是一个行记录或者一个 id 字段的值。|
|getSelected|none|返回第一个选中的行或者 null。|
|getSelections|none|返回所有选中的行，当没有选中的记录时，将返回空数组。|
|clearSelections|none|清除所有的选择。|
|selectAll|none|选中当前页所有的行。|
|unselectAll|none|取消选中当前页所有的行。|
|selectRow|index|选中一行，行索引从 0 开始。|
|selectRecord|idValue|通过 id 的值做参数选中一行。|
|unselectRow|index|取消选中一行。|
|beginEdit|index|开始对一行进行编辑。|
|endEdit|index|结束对一行进行编辑。|
|cancelEdit|index|取消对一行进行编辑。|
|getEditors|index|获取指定行的编辑器们。每个编辑器有下列特性：actions：编辑器能做的动作们。target：目标编辑器的 jQuery 对象。field：字段名。type：编辑器的类型。|
|getEditor|options|获取指定的编辑器， options 参数包含两个特性： index：行的索引。field：字段名。|
|refreshRow|index|刷新一行。|
|validateRow|index|验证指定的行，有效时返回 true。|
|updateRow|param|更新指定的行， param 参数包含下列特性：index：更新行的索引。row：行的新数据。|
|appendRow|row|追加一个新行。|
|insertRow|param|插入一个新行， param 参数包括下列特性：index：插入进去的行的索引，如果没有定义，就追加此新行。row：行的数据。|
|deleteRow|index|删除一行。|
|getChanges|type|获取最后一次提交以来更改的行，type 参数表示更改的行的类型，可能的值是：inserted、deleted、updated，等等。当 type 参数没有分配时，返回所有改变的行。|
|acceptChanges|none|提交自从被加载以来或最后一次调用acceptChanges以来所有更改的数据。|
|rejectChanges|none|回滚自从创建以来或最后一次调用acceptChanges以来所有更改的数据。|
|mergeCells|options|把一些单元格合并为一个单元格，options 参数包括下列特性：index：列的索引。field：字段名。rowspan：合并跨越的行数。colspan：合并跨越的列数。|
|showColumn|field|显示指定的列。|
|hideColumn|field|隐藏指定的列。|
﻿
