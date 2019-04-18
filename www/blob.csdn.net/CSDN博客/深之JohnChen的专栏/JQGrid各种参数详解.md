# JQGrid各种参数详解 - 深之JohnChen的专栏 - CSDN博客

2017年01月20日 11:42:19[byxdaz](https://me.csdn.net/byxdaz)阅读数：13274


# jqGrid皮肤

从3.5版本开始，jqGrid完全支持[jQuery](http://lib.csdn.net/base/jquery) UI的theme。我们可以从http://jqueryui.com/themeroller/下载我们所需要的theme。当然，你也可以编辑自己的theme。jqGrid 

也并不需要把所有的css文件都引入进来，只需导入核心css文件“ui.theme.css ” 以及“ui.core.css”即可，文件位于目录development-bundle/themes下。

# jqGrid原理

jqGrid是典型的B/S[架构](http://lib.csdn.net/base/architecture)，服务器端只是提供数据管理，客户端只提供数据显示。换句话说，jqGrid可以以一种更加简单的方式来展现你[数据库](http://lib.csdn.net/base/mysql)的信息，而且也可以把客户端数据传回给服务器端。 

对于jqGrid我们所关心的就是：必须有一段代码把一些页面信息保存到数据库中，而且也能够把响应信息返回给客户端。jqGrid是用ajax来实现对请求与响应的处理。

# jqGrid参数
|名称|类型|描述|默认值|可修改|
|----|----|----|----|----|
|url|string|获取数据的地址|||
|datatype|string|从服务器端返回的数据类型，默认xml。可选类型：xml，local，json，jsonnp，script，xmlstring，jsonstring，clientside|||
|mtype|string|ajax提交方式。POST或者GET，默认GET|||
|colNames|Array|列显示名称，是一个数组对象|||
|colModel|Array|常用到的属性：**name**列显示的名称；**index**传到服务器端用来排序用的列名称；**width**列宽度；**align**对齐方式；**sortable**是否可以排序|||
|pager|string|定义翻页用的导航栏，必须是有效的html元素。翻页工具栏可以放置在html页面任意位置|||
|rowNum|int|在grid上显示记录条数，这个参数是要被传递到后台|||
|rowList|array|一个下拉选择框，用来改变显示记录数，当选择时会覆盖rowNum参数传递到后台|||
|sortname|string|默认的排序列。可以是列名称或者是一个数字，这个参数会被提交到后台|||
|viewrecords|boolean|定义是否要显示总记录数|||
|caption|string|表格名称|||
|[a1]|object|对ajax参数进行全局设置，可以覆盖ajax事件|null|是|
|[a2]|object|对ajax的select参数进行全局设置|null|是|
|altclass|String|用来指定行显示的css，可以编辑自己的css文件，只有当altRows设为 ture时起作用|ui-priority-secondary||
|altRows|boolean|设置表格 zebra-striped 值|||
|autoencode|boolean|对url进行编码|false|是|
|autowidth|boolean|如果为ture时，则当表格在首次被创建时会根据父元素比例重新调整表格宽度。如果父元素宽度改变，为了使表格宽度能够自动调整则需要实现函数：setGridWidth|false|否|
|cellLayout|integer|定义了单元格padding + border 宽度。通常不必修改此值。初始值为|5|是|
|cellEdit|boolean|启用或者禁用单元格编辑功能|false|是|
|cellsubmit|String|定义了单元格内容保存位置|‘remote’|是|
|cellurl|String|单元格提交的url|空值|是|
|datastr|String|xmlstring或者jsonstring|空值|是|
|deselectAfterSort|boolean|只有当datatype为local时起作用。当排序时不选择当前行|true|是|
|direction|string|表格中文字的显示方向，从左向右（ltr）或者从右向左（rtr）|ltr|否|
|editurl|string|定义对form编辑时的url|空值|是|
|emptyrecords|string|当返回的数据行数为0时显示的信息。只有当属性 viewrecords 设置为ture时起作用||是|
|ExpandColClick|boolean|当为true时，点击展开行的文本时，treeGrid就能展开或者收缩，不仅仅是点击图片|true|否|
|ExpandColumn|string|指定那列来展开tree grid，默认为第一列，只有在treeGrid为true时起作用|空值|否|
|[a3]|boolean|当为true时，会在翻页栏之上增加一行|false|否|
|forceFit|boolean|当为ture时，调整列宽度不会改变表格的宽度。当shrinkToFit 为false时，此属性会被忽略|false|否|
|gridstate|string|定义当前表格的状态：'visible' or 'hidden'|visible|否|
|gridview|boolean|构造一行数据后添加到grid中，如果设为true则是将整个表格的数据都构造完成后再添加到grid中，但treeGrid, subGrid, or afterInsertRow 不能用|false|是|
|height|mixed|表格高度，可以是数字，像素值或者百分比|150|否|
|hiddengrid|boolean|当为ture时，表格不会被显示，只显示表格的标题。只有当点击显示表格的那个按钮时才会去初始化表格数据。|false|否|
|hidegrid|boolean|启用或者禁用控制表格显示、隐藏的按钮，只有当caption 属性不为空时起效|true|否|
|hoverrows|boolean|当为false时mouse hovering会被禁用|false|是|
|jsonReader|array|描述json 数据格式的数组||否|
|lastpage|integer|只读属性，定义了总页数|0|否|
|lastsort|integer|只读属性，定义了最后排序列的索引，从0开始|0|否|
|loadonce|boolean|如果为ture则数据只从服务器端抓取一次，之后所有操作都是在客户端执行，翻页功能会被禁用|false|否|
|loadtext|string|当请求或者排序时所显示的文字内容|Loading....|否|
|loadui|string|当执行ajax请求时要干什么。**disable**禁用ajax执行提示；**enable**默认，当执行ajax请求时的提示；**block**启用Loading提示，但是阻止其他操作|enable|是|
|multikey|string|只有在multiselect设置为ture时起作用，定义使用那个key来做多选。**shiftKey**，**altKey**，**ctrlKey**|空值|是|
|multiboxonly|boolean|只有当multiselect = true.起作用，当multiboxonly 为ture时只有选择checkbox才会起作用|false|是|
|multiselect|boolean|定义是否可以多选|false|否|
|multiselectWidth|integer|当multiselect为true时设置multiselect列宽度|20|否|
|page|integer|设置初始的页码|1|是|
|pagerpos|string|指定分页栏的位置|center|否|
|pgbuttons|boolean|是否显示翻页按钮|true|否|
|pginput|boolean|是否显示跳转页面的输入框|true|否|
|pgtext|string|当前页信息||是|
|prmNames|array|Default valuesprmNames: {page:“page”,rows:“rows”, sort: “sidx”,order: “sord”, search:“_search”, nd:“nd”, npage:null} 当参数为null时不会被发到服务器端|none|是|
|postData|array|此数组内容直接赋值到url上，参数类型：{name1:value1…}|空array|是|
|reccount|integer|只读属性，定义了grid中确切的行数。通常情况下与records属性相同，但有一种情况例外，假如rowNum=15，但是从服务器端返回的记录数是20，那么records值是20，但reccount值仍然为15，而且表格中也只显示15条记录。|0|否|
|recordpos|string|定义了记录信息的位置： left, center, right|right|否|
|records|integer|只读属性，定义了返回的记录数|none|否|
|recordtext|string|显示记录数信息。{0} 为记录数开始，{1}为记录数结束。viewrecords为ture时才能起效，且总记录数大于0时才会显示此信息|||
|resizeclass|string|定义一个class到一个列上用来显示列宽度调整时的效果|空值|否|
|rowList|array|一个数组用来调整表格显示的记录数，此参数值会替代rowNum参数值传给服务器端。|[]|否|
|rownumbers|boolean|如果为ture则会在表格左边新增一列，显示行顺序号，从1开始递增。此列名为'rn'.|false|否|
|rownumWidth|integer|如果rownumbers为true，则可以设置column的宽度|25|否|
|savedRow|array|只读属性，只用在编辑模式下保存数据|空值|否|
|scroll|boolean|创建一个动态滚动的表格，当为true时，翻页栏被禁用，使用垂直滚动条加载数据，且在首次访问服务器端时将加载所有数据到客户端。当此参数为数字时，表格只控制可见的几行，所有数据都在这几行中加载|false|否|
|scrollOffset|integer|设置垂直滚动条宽度|18|否|
|scrollrows|boolean|当为true时让所选择的行可见|false|是|
|selarrrow|array|只读属性，用来存放当前选择的行|array|否|
|selrow|string|只读属性，最后选择行的id|null|否|
|shrinkToFit|boolean|此属性用来说明当初始化列宽度时候的计算类型，如果为ture，则按比例初始化列宽度。如果为false，则列宽度使用colModel指定的宽度|true|否|
|sortable|boolean|是否可排序|false|否|
|sortname|string|排序列的名称，此参数会被传到后台|空字符串|是|
|sortorder|string|排序顺序，升序或者降序（asc or desc）|asc|是|
|subGrid|boolean|是否使用suggrid|false|否|
|subGridModel|array|subgrid模型|array|否|
|subGridType|mixed|如果为空则使用表格的dataType|null|是|
|subGridUrl|string|加载subgrid数据的url，jqGrid会把每行的id值加到url中|空值|是|
|subGridWidth|integer|subgrid列的宽度|20|否|
|toolbar|array|表格的工具栏。数组中有两个值，第一个为是否启用，第二个指定工具栏位置（相对于body layer），如：[true,”both”] 。工具栏位置可选值：“top”,”bottom”, “both”. 如果工具栏在上面，则工具栏id为“t_”+表格id；如果在下面则为 “tb_”+表格id；如果只有一个工具栏则为 “t_”+表格id|[false,'']|否|
|totaltime|integer|只读属性，计算加载数据的时间。目前支持xml跟json数据|0|否|
|treedatatype|mixed|数据类型，通常情况下与datatype相同，不会变|null|否|
|treeGrid|boolean|启用或者禁用treegrid模式|false|否|
|treeGridModel|string|treeGrid所使用的方法|Nested|否|
|treeIcons|array|树的图标，默认值：{plus:'ui-icon-triangle-1-e',minus:'ui-icon-triangle-1-s',leaf:'ui-icon-radio-off'}||否|
|treeReader|array|扩展表格的colModel且加在colModel定义的后面||否|
|tree_root_level|numeric|r oot元素的级别，|0|否|
|userData|array|从request中取得的一些用户信息|array|否|
|userDataOnFooter|boolean|当为true时把userData放到底部，用法：如果userData的值与colModel的值相同，那么此列就显示正确的值，如果不等那么此列就为空|false|是|
|viewrecords|boolean|是否要显示总记录数|false|否|
|viewsortcols|array|定义排序列的外观跟行为。数据格式：[false,'vertical',true].第一个参数是说，是否都要显示排序列的图标，false就是只显示当前排序列的图标；第二个参数是指图标如何显示，vertical：排序图标垂直放置，horizontal：排序图标水平放置；第三个参数指单击功能，true：单击列可排序，false：单击图标排序。说明：如果第三个参数为false则第一个参数必须为ture否则不能排序||否|
|width|number|如果设置则按此设置为主，如果没有设置则按colModel中定义的宽度计算|none|否|
|xmlReader|array|对xml[数据结构](http://lib.csdn.net/base/datastructure)的描述||否|

# Jqgrid学习 -------ColModel API

ColModel 是jqGrid里最重要的一个属性，设置表格列的属性。
|属性|数据类型|备注|默认值|
|----|----|----|----|
|align|string|left, center, right.|left|
|classes|string|设置列的css。多个class之间用空格分隔，如：'class1 class2' 。表格默认的css属性是ui-ellipsis|empty string|
|datefmt|string|”/”, ”-”, and ”.”都是有效的日期分隔符。y,Y,yyyy 年YY, yy 月m,mm for monthsd,dd 日.|ISO Date (Y-m-d)|
|defval|string|查询字段的默认值|空|
|editable|boolean|单元格是否可编辑|false|
|editoptions|array|编辑的一系列选项。{name:’__department_id’,index:’__department_id’,width:200,editable:true,edittype:’select’,editoptions: {dataUrl:”${ctx}/admin/deplistforstu.action”}},这个是演示动态从服务器端获取数据。|empty|
|editrules|array|编辑的规则{name:’age’,index:’age’, width:90,editable:true,editrules: {edithidden:true,required:true,number:true,minValue:10,maxValue:100}},设定年龄的最大值为100，最小值为10，而且为数字类型，并且为必输字段。|empty|
|edittype|string|可以编辑的类型。可选值：text, textarea, select, checkbox, password, button, image and file.|text|
|fixed|boolean|列宽度是否要固定不可变|false|
|formoptions|array|对于form进行编辑时的属性设置|empty|
|formatoptions|array|对某些列进行格式化的设置|none|
|formatter|mixed|对列进行格式化时设置的函数名或者类型{name:’sex’,index:’sex’, align:’center’,width:60,editable:true,edittype:’select’,editoptions: {value:’0:待定;1:男;2:女’},formatter:function(cellvalue, options, rowObject){var temp = “<img src=’${ctx}/jquery-ui-1.7.2.custom/css/img/”if(cellvalue==1){temp = temp +”user-white.png”;} else if(cellvalue==2){temp = temp +”user-white-female.png”;} else {temp = temp + “user-silhouette.png”;}temp = temp + “‘ border=’0′ />”return temp;}},//返回性别的图标。|none|
|hidedlg|boolean|是否显示或者隐藏此列|false|
|hidden|boolean|在初始化表格时是否要隐藏此列|false|
|index|string|索引。其和后台交互的参数为sidx|empty|
|jsonmap|string|定义了返回的json数据映射|none|
|key|boolean|当从服务器端返回的数据中没有id时，将此作为唯一rowid使用只有一个列可以做这项设置。如果设置多于一个，那么只选取第一个，其他被忽略|false|
|label|string|如果colNames为空则用此值来作为列的显示名称，如果都没有设置则使用name 值|none|
|name|string|表格列的名称，所有关键字，保留字都不能作为名称使用包括subgrid, cb and rn.|Required|
|resizable|boolean|是否可以被resizable|true|
|search|boolean|在搜索模式下，定义此列是否可以作为搜索列|true|
|searchoptions|array|设置搜索参数|empty|
|sortable|boolean|是否可排序|true|
|sorttype|string|用在当datatype为local时，定义搜索列的类型，可选值：int/integer - 对integer排序float/number/currency - 排序数字date - 排序日期text - 排序文本|text|
|stype|string|定义搜索元素的类型|text|
|surl|string|搜索数据时的url|empty|
|width|number|默认列的宽度，只能是象素值，不能是百分比|150|
|xmlmap|string|定义当前列跟返回的xml数据之间的映射关系|none|
|unformat|function|‘unformat’单元格值|null|

# Jqgrid学习 -------数据

jqGrid可支持的数据类型：xml、json、jsonp、local or clientSide、xmlstring、jsonstring 

、script、function (…)。

## Json数据

需要定义jsonReader来跟服务器端返回的数据做对应，其默认值：

jsonReader : { 

 root: "rows", 

 page: "page", 

 total: "total", 

 records: "records", 

 repeatitems: true, 

 cell: "cell", 

 id: "id", 

 userdata: "userdata", 

 subgrid: {root:"rows", 

repeatitems: true, 

 cell:"cell" 

 } 

这样服务器端返回的数据格式：

{ 

 total: "xxx", 

 page: "yyy", 

 records: "zzz", 

 rows : [ 

{id:"1", cell:["cell11", "cell12", "cell13"]}, 

{id:"2", cell:["cell21", "cell22", "cell23"]}, 

 ... 

 ] 

}

jsonReader的属性
|total|总页数|
|----|----|
|page|当前页|
|records|查询出的记录数|
|rows|包含实际数据的数组|
|id|行id|
|cell|当前行的所有单元格|

自定义：

jQuery("#gridid").jqGrid({ 

... 

 jsonReader : { 

 root:"invdata", 

 page: "currpage", 

 total: "totalpages", 

 records: "totalrecords", 

 cell: "invrow" 

 }, 

... 

}); 

totalpages: "xxx", 

 currpage: "yyy", 

 totalrecords: "zzz", 

 invdata : [ 

{id:"1", invrow:["cell11", "cell12", "cell13"]}, 

{id:"2", invrow:["cell21", "cell22", "cell23"]}, 

 ... 

 ] 

repeatitems 

  指明每行的数据是可以重复的，如果设为false，则会从返回的数据中按名字来搜索元素，这个名字就是colModel中的名字

jsonReader : { 

 root:"invdata", 

 page: "currpage", 

 total: "totalpages", 

 records: "totalrecords", 

 repeatitems: false, 

 id: "0" 

 }

 totalpages: "xxx", 

 currpage: "yyy", 

 totalrecords: "zzz", 

 invdata : [ 

{invid:"1",invdate:"cell11", amount:"cell12", tax:"cell13", total:"1234", note:"somenote"}, 

{invid:"2",invdate:"cell21", amount:"cell22", tax:"cell23", total:"2345", note:"some note"}, 

 ... 

 ] 

此例中，id属性值为“invid”。 

一旦当此属性设为false时，我们就不必把所有在colModel定义的name值都赋值。因为是按name来进行搜索元素的，所以他的排序也不是按colModel中指定的排序结果。

用户数据（user data）

在某些情况下，我们需要从服务器端返回一些参数但并不想直接把他们显示到表格中，而是想在别的地方显示，那么我们就需要用到userdata标签

jsonReader: { 

 ... 

 userdata: "userdata", 

 ... 

} 

{ 

 total: "xxx", 

 page: "yyy", 

 records: "zzz", 

userdata: {totalinvoice:240.00, tax:40.00},

 rows : [ 

{id:"1", cell:["cell11", "cell12", "cell13"]}, 

{id:"2", cell:["cell21", "cell22", "cell23"]}, 

... 

 ] 

}

在客户端我们可以有下面两种方法得到这些额外信息：

1. jQuery("grid_id").getGridParam('userData') 

2. jQuery("grid_id").getUserData() 

3. jQuery("grid_id").getUserDataItem( key ) 

# Jqgrid学习 -------事件

var lastSel; 

jQuery("#gridid").jqGrid({ 

... 

onSelectRow: function(id){ 

 if(id && id!==lastSel){ 

 jQuery('#gridid').restoreRow(lastSel); 

 lastSel=id; 

 } 

 jQuery('#gridid').editRow(id, true); 

 }, 

... 

})

|事件|参数|备注|
|----|----|----|
|afterInsertRow|rowidrowdatarowelem|当插入每行时触发。rowid插入当前行的id；rowdata插入行的数据，格式为name: value，name为colModel中的名字|
|beforeRequest|none|向服务器端发起请求之前触发此事件但如果datatype是一个function时例外|
|beforeSelectRow|rowid, e|当用户点击当前行在未选择此行时触发。rowid：此行id；e：事件对象。返回值为ture或者false。如果返回true则选择完成，如果返回false则不会选择此行也不会触发其他事件|
|gridComplete|none|当表格所有数据都加载完成而且其他的处理也都完成时触发此事件，排序，翻页同样也会触发此事件|
|loadComplete|xhr|当从服务器返回响应时执行，xhr：XMLHttpRequest 对象|
|loadError|xhr,status,error|如果请求服务器失败则调用此方法。xhr：XMLHttpRequest 对象；satus：错误类型，字符串类型；error：exception对象|
|onCellSelect|rowid,iCol,cellcontent,e|当点击单元格时触发。rowid：当前行id；iCol：当前单元格索引；cellContent：当前单元格内容；e：event对象|
|ondblClickRow|rowid,iRow,iCol,e|双击行时触发。rowid：当前行id；iRow：当前行索引位置；iCol：当前单元格位置索引；e:event对象|
|onHeaderClick|gridstate|当点击显示/隐藏表格的那个按钮时触发；gridstate：表格状态，可选值：visible or hidden|
|onPaging|pgButton|点击翻页按钮填充数据之前触发此事件，同样当输入页码跳转页面时也会触发此事件|
|onRightClickRow|rowid,iRow,iCol,e|在行上右击鼠标时触发此事件。rowid：当前行id；iRow：当前行位置索引；iCol：当前单元格位置索引；e：event对象|
|onSelectAll|aRowids,status|multiselect为ture，且点击头部的checkbox时才会触发此事件。aRowids：所有选中行的id集合，为一个数组。status：boolean变量说明checkbox的选择状态，true选中false不选中。无论checkbox是否选择，aRowids始终有值|
|onSelectRow|rowid,status|当选择行时触发此事件。rowid：当前行id；status：选择状态，当multiselect 为true时此参数才可用|
|onSortCol|index,iCol,sortorder|当点击排序列但是数据还未进行变化时触发此事件。index：name在colModel中位置索引；iCol：当前单元格位置索引；sortorder：排序状态：desc或者asc|
|resizeStart|event, index|当开始改变一个列宽度时触发此事件。event：event对象；index：当前列在colModel中位置索引|
|resizeStop|newwidth, index|当列宽度改变之后触发此事件。newwidth：列改变后的宽度；index：当前列在colModel中的位置索引|
|serializeGridData|postData|向服务器发起请求时会把数据进行序列化，用户自定义数据也可以被提交到服务器端|

# Jqgrid学习 -------方法

jqGrid的方法，从3.6开始已经完全兼容jQuery UI库。

jQuery("#grid_id").jqGridMethod( parameter1,...parameterN );

jQuery("#grid_id").setGridParam({...}).hideCol("somecol").trigger("reloadGrid");

如果使用新的API：

jQuery("#grid_id").jqGrid('method', parameter1,...parameterN );

jQuery("#grid_id").jqGrid('setGridParam',{...}).jqGrid('hideCol',"somecol").trigger("reloadGrid");

jqGrid配置使用新的api

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd"> 

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en"> 

<head> 

<meta http-equiv="Content-Type" content="text/html; charset=utf-8" /> 

<title>My First Grid</title> 

<link rel="stylesheet" type="text/css" media="screen" href="css/ui-lightness/jquery-ui-1.7.1.custom.css" /> 

<link rel="stylesheet" type="text/css" media="screen" href="css/ui.jqgrid.css" /> 

<script src="js/jquery-1.3.2.min.js" type="text/[JavaScript](http://lib.csdn.net/base/javascript)"></script> 

<script src="js/i18n/grid.locale-en.js" type="text/javascript"></script> 

<script type="text/javascript"> 

jQuery.jgrid.no_legacy_api = true; 

</script> 

<script src="js/jquery.jqGrid.min.js" type="text/javascript"></script> 

</head> 

<body> 

... 

</body> 

</html>

jqGrid方法
|方法名|参数|返回值|说明|
|----|----|----|----|
|addJSONData|data|none|使用传来的data数据填充表格。使用方法：var mygrid = jQuery(”#”+grid_id)[0];var myjsongrid = eval(”(”+jsonresponse.responseText+”)”); mygrid.addJSONData(myjsongrid);myjsongrid = null;jsonresponse =null;|
|addRowData|rowid,data, position, srcrowid|成功为true, 否则为false|根据参数插入一行新的数据，rowid为新行的id，data为新行的数据，position为新增行的位置，srcrowid为新增行的参考位置。data数据格式：{name1:value1,name2: value2…} name为在colModel中指定的名称|
|addXMLData|data|none|根据传来的数据填充表格。用法：var mygrid = jQuery(”#”+grid_id)[0]; mygrid.addXmlData(xmlresponse.responseXML);|
|clearGridData|clearfooter|jqGrid对象|清除表格当前加载的数据。如果clearfooter为true时则此方法删除表格最后一行的数据|
|delRowData|rowid|成功为true否则为false|根据rowid删除行，但不会从服务器端删除数据|
|footerData|action,data, format|jgGrid对象|设置或者取得底部数据。action：“get”或者“set”，默认为“get”，如果为“get”返回值为name:value，name为colModel中名称。如果为“set”则值为name：value，name是colModel中的名称。format：默认为true，当为 true时，在设置新值时会调用formatter格式化数值|
|getCell|rowid, iCol|单元格内容|返回指定rowid，iCol的单元格内容，iCol既可以是当前列在colModel中的位置索引也可以是name值。注意：在编辑行或者单元格时不能使用此方法，此时返回的并不是改变的值，而是原始值|
|getCol|colname, returntype, mathoperation|array[] or value|返回列的值。colname既可以是当前列在colModel中的位置索引也可以是name值。returntype指定返回数据的类型，默认为false。当为false时，返回的数组中只包含列的值，当为true时返回数组是对象数组，具体格式 {id:rowid, value:cellvalue} ，id为行的id，value为列的值。如： [{id:1,value:1},{id:2,value:2}…]。mathoperation 可选值为'sum, 'avg', 'count'|
|getDataIDs|none|array[]|返回当前grid里所有数据的id|
|getGridParam|name|mixed value|返回请求的参数信息|
|getInd|rowid,rowcontent|mixed|如果rowcontent为false，返回行所在的索引位置，id为行id。rowcontent默认为false。如果rowconent为ture则返回的为行对象，如果找不到行则返回false|
|getRowData|rowid or none|array[]|返回指定行的数据，返回数据类型为name:value，name为colModel中的名称，value为所在行的列的值，如果根据rowid找不到则返回空。在编辑模式下不能用此方法来获取数据，它得到的并不是编辑后的值|
|hideCol|colnameor[colnames]|jqGrid对象|如果参数为一个列名则隐藏此列，如果给定的是数组则隐藏指定的所有列。格式： [“name1”,”name2”]|
|remapColumns|permutation, updateCells, keepHeader|none|调整表格列的显示顺序,permutation为当前列的顺序，假如值是[1,0,2]，那么第一列就会在第二位显示。如果updateCells为ture则是对单元格数据进行重新排序，如果keepHeader为true则对header数据显示位置进行调整|
|resetSelection|none|jqGrid对象|选择或者反选行数据，在多选模式下也同样起作用|
|setCaption|caption|jqGrid对象|设置表格的标题|
|setCell|rowid,colname, data, class, properties|jqGrid对象|改变单元格的值。rowid：当前行id；colname：列名称，也可以是列的位置索引，从0开始；data：改变单元格的内容，如果为空则不更新；class：如果是string则会使用addClass方法将其加入到单元格的css中，如果是array则会直接加到style属性中；properties：设置单元格属性|
|setGridParam|object|jqGrid对象|设置grid的参数。有些参数的修改必须要重新加载grid才可以生效，这个方法可以覆盖事件|
|setGridHeight|new_height|jqGrid对象|动态改变grid的高度，只能对单元格的高度进行设置而不能对表格的高度进行动态修改。new_height：可以是象素值，百分比或者"auto"|
|setGridWidth|new_width,shrink|jqGrid对象|动态改变表格的宽度。new_width:表格宽度，象素值；shrink：true或者false，作用同shrinkToFit|
|setLabel|colname, data, class, properties|jqGrid对象|给指定列设置一个新的显示名称。colname：列名称，也可以是列的位置索引，从0开始；data：列显示名称，如果为空则不修改；class：如果是 string则会使用addClass方法将其加入到单元格的css中，如果是array则会直接加到style属性中；properties：设置 label的属性|
|setRowData|rowid,data, cssprop|成功true否则false|更新行的值，rowid为行id。data值格式：{name1:value1,name2: value2…} name为colModel中名称；cssprop：如果是string则会使用addClass方法将其加入到行的css中，如果是array或者对象则会直接加到style属性中|
|setSelection|rowid,onselectrow|jqGrid对象|选择或反选指定行。如果onselectrow为ture则会触发事件onSelectRow，onselectrow默认为ture|
|showCol|colname|jqGrid|显示列。colname可以是数组[“name1”,”name2”],但是name1或者name2必须是colModel中的name|
|trigger(“reloadGrid”)|none|jqGrid对象|重新加载当前表格，也会向服务器发起新的请求|
|updateColumns|none|none|同步表格的宽度，用在表格拖拽时，用法：var mygrid=jQuery(”#grid_id”)[0];mygrid.updateColumns();|

jqGrid的通用方法和设置 

这些方法并不和jqGrid对象绑定，可以随意使用：

jQuery.jgrid.jqGridFunction( parameter1,...parameterN );
|函数名|参数|返回值|说明|
|----|----|----|----|
|ajaxOptions|空对象|none|这个函数可以改变jqgrid中用到的ajax请求参数，这个函数可以覆盖当前所有ajax请求设置。从3.6版本开始起有3个级别的ajax设置：第一个级别就是在模块中设置ajax请求；第二个级别就是通过此函数设置；第三级别的设置是控制全局ajax请求的设置：jQuery.extend(jQuery.ajax({method specific options}, ajaxOptions, ThirdLevelajaxSettinds)); 当然我们也可以单独设置ajax的参数|
|jqID|string|解析后的string|转义字符串，把两个反斜杠(\\)转化为单个反斜杠(\)|
|jgrid.htmlDecode|string|转换后string|把转码后的字符串还原|
|jgrid.htmlEncode|string|编码后的string|把字符串编码|
|jgrid.format|string|格式化后string|简单字符串模板。用法：Example jQuery.jqgformat(“Please enter a value between {0} and {1}.”, 4,result : “Please enter a value between 4 and 8.”|
|jgrid.getCellIndex|cell|index|这个方法是用来修复在ie7里的一个bug|
|jgrid.stringToDoc|xmlstring|xmlDoc|把xmlstring转换为dom对象|
|jgrid.stripHtml|content|new_content|去掉html标签返回标签中内容|
|jgrid.parse|jsonstring|对象|把一个jsonstring转换为json对象|

jqGrid的方法
|方法名|参数|返回值|说明|
|----|----|----|----|
|filterGrid|grid_id,params|HTML对象|构造jqGrid的查询界面。grid_id:表格id；params：参数|
|filterToolbar|params|jqGrid对象|同上。不同的是搜索输入框在header层下方|
|getColProp|colname|array|返回指定列的属性集合。name为colModel中名称|
|GridDestroy|grid_id|boolean|从dom上删除此grid|
|GridUnload|grid_id|boolean|跟GridDestroy不同的是table对象跟pager对象并不会被删除，以便下次使用|
|setGridState|state|jqGrid对象|设置grid的状态，显示或者隐藏。这个方法不会触发onHeaderClick 事件。|
|setColProp|colname, properties|jqGrid对象|设置新的属性，对于动态改变列属性是非常有用的，但是有些属性的修改并不会起效。用法：jQuery(”#grid_id”).setColProp('colname',{editoptions:{value:“True:False”}});|
|sortGrid|colname, reload|jqGrid对象|按指定列进行排序。效果同setGridParam({sortname:'myname'}).trigger('reloadGrid'). 如果reload为true则会重新加载数据|
|updateGridRows|data,rowidname,jsonreader|boolean|修改表格中某行的数据，data数据格式：[{name:value,name1:value1…}, {name:value,name2:value2…}]，name为colModel中的名称；rowidname某行的名称。 jsonreader：boolean值，默认false。如果为true则是定义数据的格式，data的值并不是name：value形式而只是 value|

# Jqgrid学习 -------配置 json

IE8，FF3以及Chrome 3已经支持JSON，配置：

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd"> 

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en"> 

<head> 

<meta http-equiv="Content-Type" content="text/html; charset=utf-8" /> 

<title>My First Grid</title> 

<link rel="stylesheet" type="text/css" media="screen" href="css/ui-lightness/jquery-ui-1.7.1.custom.css" /> 

<link rel="stylesheet" type="text/css" media="screen" href="css/ui.jqgrid.css" /> 

<script src="js/jquery-1.3.2.min.js" type="text/javascript"></script> 

<script src="js/i18n/grid.locale-en.js" type="text/javascript"></script> 

<script type="text/javascript"> 

jQuery.jgrid.useJSON = true; 

</script> 

<script src="js/jquery.jqGrid.min.js" type="text/javascript"></script> 

</head> 

<body> 

... 

</body> 

</html>

这段代码要放到语言包之后jqGrid.js文件之前。 

如果浏览器不支持JSON，那么我们只能用eval函数解析json。

除了jqGrid本身提供对json的类库外，我们可以使用JSON.parse来处理JSON，配置如下：

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd"> 

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en"> 

<head> 

<meta http-equiv="Content-Type" content="text/html; charset=utf-8" /> 

<title>My First Grid</title> 

<link rel="stylesheet" type="text/css" media="screen" href="css/ui-lightness/jquery-ui-1.7.1.custom.css" /> 

<link rel="stylesheet" type="text/css" media="screen" href="css/ui.jqgrid.css" /> 

<script src="js/json2.js" type="text/javascript"></script> 

<script src="js/jquery-1.3.2.min.js" type="text/javascript"></script> 

<script src="js/i18n/grid.locale-en.js" type="text/javascript"></script> 

<script src="js/jquery.jqGrid.min.js" type="text/javascript"></script> 

<script type="text/javascript"> 

 jQuery.extend(jQuery.jgrid,{ 

 parse:function(jsstring) { 

 return JSON.parse(jsstring); 

 } 

 }); 

</script> 

</head> 

<body> 

... 

</body> 

</html>

# Jqgrid学习 -------翻页

jqGrid的翻页要定义在html里，通常是在grid的下面，且是一个div对象：

<table id="list"></table> 

 <div id="gridpager"></div>

jQuery("#grid_id").jqGrid({ 

... 

 pager : '#gridpager', 

... 

});

不必给翻页设置任何的css属性。在jqGrid里定义的翻页可以是：:pager : '#gridpager', pager : 'gridpager' or pager : jQuery('#gridpager'). 推荐使用前两个，当使用其他方式时jqGrid的导入导出功能时会引起错误。

导航栏的属性：

$.jgrid = { 

defaults : { 

recordtext: "View {0} - {1} of {2}", 

emptyrecords: "No records to view", 

loadtext: "Loading...", 

pgtext : "Page {0} of {1}" 

}, 

... 

}

如果想改变这些设置：

1． jQuery.extend(jQuery.jgrid.defaults,{emptyrecords: "Nothing to display",...});

2．jQuery("#grid_id").jqGrid({ 

... 

 pager : '#gridpager', 

 emptyrecords: "Nothing to display", 

... 

});

导航栏的属性：
|属性名|类型|说明|默认值|是否可以被修改|
|----|----|----|----|----|
|lastpage|integer|只读属性，总页数|0|NO|
|pager|mixed|导航栏对象，必须是一个有效的html元素，位置可以随意|空字符串|NO|
|pagerpos|string|定义导航栏的位置，默认分为三部分：翻页，导航工具及记录信息|center|NO|
|pgbuttons|boolean|是否显示翻页按钮|true|NO|
|pginput|boolean|是否显示跳转页面的输入框|true|NO|
|pgtext|string|页面信息，第一个值是当前页第二个值是总页数|语言包|YES|
|reccount|integer|只读属性，实际记录数，千万不能跟records 参数搞混了，通常情况下他们是相同的，假如我们定义rowNum=15，但我们从服务器端返回的记录为20即records=20，而reccount=15，表格中也显示15条记录|0|NO|
|recordpos|string|定义记录信息的位置，可选值：left, center, right|right|NO|
|records|integer|只读属性，从服务器端返回的记录数|none|NO|
|recordtext|string|显示记录的信息，只有当viewrecords为true时起效，且记录数必须大于0|语言包|yes|
|rowList|array|可以改变表格可以显示的记录数，格式为[10,20,30]|array|no|
|rowNum|integer|设置表格可以显示的记录数|20|yes|
|viewrecords|boolean|是否要显示总记录数信息|false|no|

jQuery("#grid_id").setGridParam({rowNum:10}).trigger("reloadGrid"); 

跟翻页相关的事件只有一个：onPaging
|onPaging|pgButton|当点击翻页按钮但还为展现数据时触发此事件，当然这跳转栏输入页码改变页时也同样触发此事件。参数pgButton可选值： first,last,prev,next|||

jqGrid的翻页导航是一个方法，你可以事先定义一些其他操作，比如：编辑、新增、删除及搜索。也可以增加自定义的函数。导航工具栏是定义到翻页控件上的。定义如下：

<body> 

... 

 <table id="list"></table> 

 <div id="gridpager"></div> 

... 

</body>

jQuery("#grid_id").jqGrid({ 

... 

 pager : '#gridpager', 

... 

}); 

jQuery("#grid_id").navGrid('#gridpager',{parameters},prmEdit, prmAdd, prmDel, prmSearch, prmView); 

jQuery("#grid_id").jqGrid({ 

... 

 pager : '#gridpager', 

... 

}); 

jQuery("#grid_id").jqGrid('navGrid','#gridpager',{parameters},prmEdit, prmAdd, prmDel, prmSearch, prmView); 

jQuery("#grid_id").jqGrid({ 

... 

 pager : '#gridpager', 

... 

}).navGrid('#gridpager',{parameters}, prmEdit, prmAdd, prmDel, prmSearch, prmView); 

...

grid_id ：表格id 

gridpager ：导航栏id 

parameters ：参数列表 

prmEdit, prmAdd, prmDel, prmSearch, prmView ：事件

$.jgrid = { 

... 

 search : { 

 caption: "Search...", 

 Find: "Find", 

 Reset: "Reset", 

 odata : ['equal', 'not equal', 'less', 'less or equal','greater','greater or equal', 'begins with','does not begin with','is in','is not in','ends with','does not end with','contains','does not contain'], 

 groupOps: [ { op: "AND", text: "all" }, { op: "OR", text: "any" } ], 

 matchText: " match", 

 rulesText: " rules" 

 }, 

 edit : { 

 addCaption: "Add Record", 

 editCaption: "Edit Record", 

 bSubmit: "Submit", 

 bCancel: "Cancel", 

 bClose: "Close", 

 saveData: "Data has been changed! Save changes?", 

 bYes : "Yes", 

 bNo : "No", 

 bExit : "Cancel", 

 }, 

 view : { 

caption: "View Record", 

bClose: "Close" 

 }, 

 del : { 

caption: "Delete", 

msg: "Delete selected record(s)?", 

bSubmit: "Delete", 

bCancel: "Cancel" 

 }, 

 nav : { 

edittext: "", 

edittitle: "Edit selected row", 

addtext:"", 

addtitle: "Add new row", 

deltext: "", 

deltitle: "Delete selected row", 

searchtext: "", 

searchtitle: "Find records", 

refreshtext: "", 

refreshtitle: "Reload Grid", 

alertcap: "Warning", 

alerttext: "Please, select row", 

viewtext: "", 

viewtitle: "View selected row" 

 }, 

...
|属性|类型|说明|默认值|
|----|----|----|----|
|add|boolean|是否启用新增功能，当点击按钮时会触发editGridRow事件|true|
|addicon|string|给新增功能设置图标，只有UI theme里的图标才可以使用|ui-icon-plus|
|addtext|string|新增按钮上的文字|空|
|addtitle|string|当鼠标移到新增按钮上时显示的提示|新增一行|
|alertcap|string|当我们edit,delete or view一行记录时出现的提示信息|警告|
|alerttext|string|当edit,delete or view一行记录时的文本提示|请选择一行记录|
|closeOnEscape|boolean|是否可以使用esc键关闭对话框|true|
|del|boolean|是否启用删除功能，启用时会触发事件delGridRow|true|
|delicon|string|设置删除按钮的图标，只有UI theme里的图标才可以使用|ui-icon-trash|
|deltext|string|设置到删除按钮上的文字信息|空|
|deltitle|string|当鼠标移到删除按钮上时出现的提示|删除锁选择的行|
|edit|boolean|是否启用可编辑功能，当编辑时会触发事件editGridRow|true|
|editicon|string|设置编辑按钮的图标，只有UI theme里的图标才可以使用|ui-icon-pencil|
|edittext|string|编辑按钮上文字|空|
|edittitle|string|当鼠标移到编辑按钮上出现的提示信息|编辑所选择的行|
|position|string|定义按钮位置，可选值left, center and right.|left|
|refresh|boolean|是否启用刷新按钮，当点击刷新按钮时会触发trigger(“reloadGrid”)事件，而且会清空搜索条件值|true|
|refreshicon|string|设置刷新图标，只有UI theme里的图标才可以使用|ui-icon-refresh|
|refreshtext|string|刷新按钮上文字信息|空|
|refreshtitle|string|当鼠标移到刷新按钮上的提示信息|重新加载|
|refreshstate|string|指明表格如何刷新。firstpage：从第一页开始刷新；current：只刷新当前页内容|firstpage|
|afterRefresh|function|当点击刷新按钮之后触发此事件|null|
|search|boolean|是否启用搜索按钮，会触发searchGrid 事件|true|
|searchhicon|string|设置搜索按钮的图标，只有UI theme里的图标才可以使用|ui-icon-search|
|searchtext|string|搜索按钮上的文字|空|
|searchtitle|string|当鼠标移到搜索按钮上的提示信息|搜索|
|view|boolean|是否启用查看按钮，会触发事件viewGridRow|false|
|viewicon|string|设置查看按钮的图标，只有UI theme里的图标才可以使用|ui-icon-document|
|viewtext|string|查看按钮上文字|空|
|viewtitle|string|当鼠标移到查看按钮上的提示信息|查看所选记录|

jQuery("#grid_id").jqGrid({ 

... 

 pager : '#gridpager', 

... 

}).navGrid('#gridpager',{view:true, del:false}, 

{}, // use default settings for edit 

{}, // use default settings for add 

{}, // delete instead that del:false we need this 

{multipleSearch : true}, // enable the advanced searching 

{closeOnEscape:true} /* allow the view dialog to be closed when user press ESC key*/ 

);

# Jqgrid学习 -------自定义按钮

jQuery("#grid_id").navGrid("#pager",...).navButtonAdd("#pager",{parameters});

jQuery("#grid_id").jqGrid('navGrid',"#pager",...).jqGrid('navButtonAdd',"#pager",{parameters});

{ caption:"NewButton", buttonicon:"ui-icon-newwin", onClickButton:null, position: "last", title:"", cursor: "pointer"} 

caption：按钮名称，可以为空，string类型 

buttonicon：按钮的图标，string类型，必须为UI theme图标 

onClickButton：按钮事件，function类型，默认null 

position：first或者last，按钮位置 

title：string类型，按钮的提示信息 

cursor：string类型，光标类型，默认为pointer 

id：string类型，按钮id

如果设置多个按钮：

jQuery("#grid_id") 

.navGrid('#pager',{edit:false,add:false,del:false,search:false}) 

.navButtonAdd('#pager',{ 

 caption:"Add", 

 buttonicon:"ui-icon-add", 

 onClickButton: function(){ 

 alert("Adding Row"); 

 }, 

 position:"last" 

}) 

.navButtonAdd('#pager',{ 

 caption:"Del", 

 buttonicon:"ui-icon-del", 

 onClickButton: function(){ 

 alert("Deleting Row"); 

 }, 

 position:"last" 

});

按钮间的分隔

jQuery("#grid_id").navGrid("#pager",...).navButtonAdd("#pager",{parameters}).navSeparatorAdd("#pager",{separator_parameters}};

默认参数：

{sepclass : “ui-separator”,sepcontent: ''}

sepclass：ui-jqgrid的属性名 

sepcontent：分隔符的内容.

# Jqgrid学习 -------格式化

jqGrid的格式化是定义在语言包中

$jgrid = { 

... 

 formatter : { 

 integer : {thousandsSeparator: " ", defaultValue: '0'}, 

 number : {decimalSeparator:".", thousandsSeparator: " ", decimalPlaces: 2, defaultValue: '0.00'}, 

 currency : {decimalSeparator:".", thousandsSeparator: " ", decimalPlaces: 2, prefix: "", suffix:"", defaultValue: '0.00'}, 

 date : { 

 dayNames: [ 

 "Sun", "Mon", "Tue", "Wed", "Thr", "Fri", "Sat", 

 "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" 

 ], 

 monthNames: [ 

 "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec", 

 "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" 

 ], 

 AmPm : ["am","pm","AM","PM"], 

 S: function (j) {return j < 11 || j > 13 ? ['st', 'nd', 'rd', 'th'][Math.min((j - 1) % 10, 3)] : 'th'}, 

 srcformat: 'Y-m-d', 

 newformat: 'd/m/Y', 

 masks : { 

 ISO8601Long:"Y-m-d H:i:s", 

 ISO8601Short:"Y-m-d", 

 ShortDate: "n/j/Y", 

 LongDate: "l, F d, Y", 

 FullDateTime: "l, F d, Y g:i:s A", 

 MonthDay: "F d", 

 ShortTime: "g:i A", 

 LongTime: "g:i:s A", 

 SortableDateTime: "Y-m-d\\TH:i:s", 

 UniversalSortableDateTime: "Y-m-d H:i:sO", 

 YearMonth: "F, Y" 

 }, 

 reformatAfterEdit : false 

 }, 

 baseLinkUrl: '', 

 showAction: '', 

 target: '', 

 checkbox : {disabled:true}, 

 idName : 'id' 

 } 

...

这些设置可以通过colModel中的formatoptions参数修改

jQuery("#grid_id").jqGrid({ 

... 

 colModel : [ 

 ... 

 {name:'myname', ... formatter:'number', ...}, 

 ... 

 ], 

... 

});

此实例是对名为“myname”的列进行格式化，格式化类是“number”，假如初始值为“1234.1”则格式化后显示为“1 234.10” 。

如果给某列进行格式化：

jQuery("#grid_id").jqGrid({ 

... 

 colModel : [ 

 ... 

 {name:'myname', ... formatter:'currency', formatoptions:{decimalSeparator:",", thousandsSeparator: ",", decimalPlaces: 2, prefix: "$ "} } , 

 ... 

 ], 

... 

});

这个设置会覆盖语言包中的设置。

select类型的格式化实例：

原始数据

jQuery("#grid_id").jqGrid({ 

... 

 colModel : [ {name:'myname', edittype:'select', editoptions:{value:"1:One;2:Two"}} ... ], 

... 

});

使用格式化后

jQuery("#grid_id").jqGrid({ 

... 

 colModel : [ {name:'myname', edittype:'select', formatter:'select', editoptions:{value:"1:One;2:Two"}} ... ] 

... 

});

结果是，表格的数据值为1或者2但是现实的是One或者Two。

对超链接使用select类型的格式化：

jQuery("#grid_id").jqGrid({ 

... 

 colModel: [ {name:'myname', edittype:'select', formatter:'select', formatoptions:{baseLinkUrl:'someurl.[PHP](http://lib.csdn.net/base/php)', addParam: '&action=edit'}, ...} 

 ... 

 ] 

... 

});

得到http://localhost/someurl.php?id=123&action=edit

如果想改变id值则

jQuery("#grid_id").jqGrid({ 

... 

 colModel: [ {name:'myname', edittype:'select', formatter:'select', formatoptions:{baseLinkUrl:'someurl.php', addParam: '&action=edit', idName:'myid'}, ...} 

 ... 

 ] 

... 

});

得到http://localhost/someurl.php?myid=123&action=edit

# Jqgrid学习 -------自定义格式化

jQuery("#grid_id").jqGrid({ 

... 

 colModel: [ 

 ... 

 {name:'price', index:'price', width:60, align:"center", editable: true, formatter:currencyFmatter}, 

 ... 

 ] 

... 

}); 

function currencyFmatter (cellvalue, options, rowObject) 

{ 

 // do something here 

 return new_format_value 

}

cellvalue：要被格式化的值 

options：对数据进行格式化时的参数设置，格式为： 

{ rowId: rid, colModel: cm} 

rowObject：行数据

数据的反格式化跟格式化用法相似.

jQuery("#grid_id").jqGrid({ 

... 

 colModel: [ 

 ... 

 {name:'price', index:'price', width:60, align:"center", editable: true, formatter:currencyFmatter, unformat:unformatCurrency}, 

 ... 

 ] 

... 

}); 

function currencyFmatter (cellvalue, options, rowObject) 

{ 

 return "$"+cellvalue; 

} 

function unformatCurrency (cellvalue, options) 

{ 

 return cellvalue.replace("$",""); 

} 

表格中数据实际值为123.00，但是显示的是$123.00; 我们使用getRowData ，getCell 方法取得的值是123.00。 

创建通用的格式化函数

<script type="text/javascript"> 

jQuery.extend($.fn.fmatter , { 

currencyFmatter : function(cellvalue, options, rowdata) { 

return "$"+cellvalue; 

} 

}); 

jQuery.extend($.fn.fmatter.currencyFmatter , { 

unformat : function(cellvalue, options) { 

return cellvalue.replace("$",""); 

} 

}); 

</script>

具体使用：

jQuery("#grid_id").jqGrid({ 

... 

 colModel: [ 

 ... 

 {name:'price', index:'price', width:60, align:"center", editable: true, formatter:currencyFmatter}, 

 ... 

 ] 

... 

})

# Jqgrid学习 -------搜索

表格中所有的列都可以作为搜索条件。 

所用到的语言包文件

$.jgrid = { 

... 

 search : { 

 caption: "Search...", 

 Find: "Find", 

 Reset: "Reset", 

 odata : ['equal', 'not equal', 'less', 'less or equal','greater','greater or equal', 'begins with','does not begin with','is in','is not in','ends with','does not end with','contains','does not contain'], 

 groupOps: [ { op: "AND", text: "all" }, { op: "OR", text: "any" } ], 

 matchText: " match", 

 rulesText: " rules" 

 }

colModel 设置
|可选参数|类型|说明|默认值|
|----|----|----|----|
|search|boolean|是否是搜索列|true|
|stype|string|搜索类型，text类型或者select类型|text|
|searchoptions|object|对搜索条件进行一些设置||

searchoptions 参数
|属性|类型|说明|
|----|----|----|
|dataUrl|string|只有当搜索类型为select才起效|
|buildSelect|function|只有当dataUrl设置时此参数才起效，通过一个function来构建下拉框|
|dataInit|function|初始化时调用，用法：dataInit: function(elem) {do something}通常用在日期的选择上. Example:dataInit : function (elem) {$(elem).datepicker();}|
|dataEvents|array|事件列表，用法：dataEvents: [{ type: 'click', data: { i: 7 }, fn: function(e) { console.log(e.data.i); }},{ type: 'keypress', fn: function(e) { console.log('keypress'); } }]|
|attr|object|设置属性值。attr : { title: “Some title” }|
|searchhidden|boolean|默认情况下，隐藏值不是搜索列。为了使隐藏值可以作为搜索列则将此设为true|
|sopt|array|此参数只用到单列搜索上，说明搜索条件。可用值：['eq','ne','lt','le','gt','ge','bw','bn','in','ni','ew','en','cn','nc']意思为['equal','not equal', 'less', 'less or equal','greater','greater or equal', 'begins with','does not begin with','is in','is not in','ends with','does not end with','contains','does not contain']|
|defaultValue|string|默认值|
|value|mixed|只用在搜索类型为select下。可以是string或者object，如果为string则格式为value:label，且以“；”结尾；如果为object格式为editoptions:{value:{1:'One';2:'Two'}}|

jQuery("#grid_id").jqGrid({ 

... 

 colModel: [ 

 ... 

 {name:'price', index:'price', width:60, search:true, stype:'text', searchoptions:{dataInit:datePick, attr:{title:'Select Date'}} }, 

 ... 

 ] 

... 

}); 

datePick = function(elem) 

{ 

 jQuery(elem).datepicker(); 

}

需要说明的： 

所有的搜索都是使用url来到服务器端查询数据。 

当执行搜索时会用查询数据填充postData array 

发送到服务器端搜索字符串的名称为_search 

当点击刷新按钮时不会使用搜索条件 

每个搜索方法都有自己的数据清空方法

# Jqgrid学习 -------搜索工具栏

搜索工具栏只是在列标题下面构造一个输入框。且使用表格的url进行搜索记录，发到服务器端数据格式为name:value，而且是附加在postData之后。

jQuery("#grid_id").filterToolbar(options); 

jQuery("#grid_id").jqGrid('filterToolbar',options); 

options：参数
|可选参数|类型|描述|默认值|
|----|----|----|----|
|autosearch|boolean|查询规则，如果是text类型则是当按下回车键才去执行查询；如果是select类型的查询则当值改变时就去执行查询|true|
|beforeSearch|function|执行查询之前触发此事件|null|
|afterSearch|function|查询完成后触发事件|null|
|beforeClear|function|清空查询条件值时触发事件|null|
|afterClear|function|清空查询条件后触发事件|null|

方法：
|方法|描述|
|----|----|
|triggerToolbar|执行查询时调用此方法|
|clearToolbar|当清空查询条件值时触发此函数|
|toggleToolbar|Toggeles工具栏|

# Jqgrid学习 -------自定义搜索

<div id="mysearch"></div>

jQuery("#mysearch").filterGrid('#grid_id',options);

options：参数
|参数|描述|默认值|
|----|----|----|
|gridModel|当为ture我们会使用colModel中的属性构造查询条件，所用到的参数：name, index, edittype, editoptions, search.还有一个参数：defval：查询条件的默认值；surl：当edittype:'select'时获取select数据的url，格 式：<select><option value='val1'> Value1 </option><option value='val2'> Value2 </option>…<option value='valn'> ValueN </option></select>|false|
|gridNames|gridModel为true时起效，设置查询列的名称|false|
|filterModel|gridModel为false时起效，格式：{label:'LableFild', name: 'colname', stype: 'select', defval: 'default_value', surl: 'someurl', sopt:{optins for the select}}。label：字段显示名称；name：列名；stype：输入框类型，text或者select；surl：获取select数据的地 址，要求的内容为html格式：<select><option value='val1'> Value1 </option><option value='val2'> Value2 </option>…<option value='valn'> ValueN </option></select>；sopt：同editoptions|[]|
|formtype|定义表单如何被构造，'horizontal' or 'vertical'||
|autosearch|如果为true:当点击回车键触发查询；当select值变化时触发查询||
|formclass|可以使用的css|filterform|
|tableclass|可以使用到table上的css|filtertable|
|buttonclass|按钮上使用的css|filterbutton|
|searchButton|搜索按钮|Search|
|clearButton|清空数据的按钮|Clear|
|enableSearch|启用禁用搜索按钮|false|
|enableClear|启用禁用清空按钮|false|
|beforeSearch|搜索之前触发的事件|null|
|afterSearch|搜索完成之后触发的事件|null|
|beforeClear|清空数据之前触发的事件|null|
|afterClear|清空数据之后触发事件|null|
|url|搜索数据的url|‘’|
|marksearched|当为true时，每次查询之后所有查询的列都标记为可查询列|true|

[](http://blog.csdn.net/lionel1982/article/details/50545367#)[](http://blog.csdn.net/lionel1982/article/details/50545367#)[](http://blog.csdn.net/lionel1982/article/details/50545367#)[](http://blog.csdn.net/lionel1982/article/details/50545367#)[](http://blog.csdn.net/lionel1982/article/details/50545367#)[](http://blog.csdn.net/lionel1982/article/details/50545367#)

