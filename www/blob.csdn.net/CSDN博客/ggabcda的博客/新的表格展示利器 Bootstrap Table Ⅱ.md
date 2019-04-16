# 新的表格展示利器 Bootstrap Table Ⅱ - ggabcda的博客 - CSDN博客





2017年09月06日 13:05:49[ggabcda](https://me.csdn.net/ggabcda)阅读数：338








上一篇文章介绍了Bootstrap Table的基本知识点和应用，本文针对上一篇文章中未解决的文件导出问题进行分析，同时介绍BootStrap Table的扩展功能，当行表格数据修改。

### **1.Bootstrap Bable 全部数据导出分析**

     在表格导出数据中，发现设置了分页参数，导出的数据仅为表格加载的分页参数数据，于是，针对这样的情况，通过设置分页参数的值，使表格可以加载更多的数据，可达到导出所有数据的功能需求。然而，在实际的实验中，发现此方案存在以下问题：
- 表格一次加载一千条数据时，网页响应速度太慢，加载数据等待时间严重过长。（一分钟左右） 
- Bootsrtap Table 的文件导出是纯前端的js导出模式，它的数据源只能为表格中的数据集合 

    分析产生上述问题的原因，不难发现，html页面在渲染的时候，一次渲染过多的节点，网页性能必然降低。而通过去分析table-export.js中的导出源码，发现导出文件是以表格为数据源，导出多数据时，必然需要表格加载更多的数据，所以这样的循环，导致导出功能在实际项目中，导出特别耗时，应用不理想。要解决这样的问题，目前可采用如下两种方法：
- 修改table-export.js中的导出模块源码，当ajax从后台请求到数据成功后，不在渲染数据到Bootstrap Table中去，而是直接作为数据源提供给导出模块。（目前这样的方案，感觉编写js函数需要的逻辑比较复杂）

- 前端调用java程序中，成熟的导出文件功能，利用java程序处理文件导出。 

     本次测试文件导出时，采用了java后台程序处理的方案。具体操作如下：
- 前端界面仿造Bootstrap Table的js函数，编写一个导出所有文件的按钮 
- 利用java程序，调用**SXSSFWorkbook**组件，导出指定数据到Excel表中 

#### 构建导出按钮

     构建导出按钮，修改bootstrap-table.js中的定义事项，加入一个showExportAll参数，定义展示样式即可模拟一个导出按钮，重要实现代码如下：

```
1    //wanling add exportAll button 2017-8-7
 2         if (this.options.showExportAll) {
 3             html.push(sprintf('<button class="btn' +
 4                     sprintf(' btn-%s', this.options.buttonsClass) +
 5                     sprintf(' btn-%s', this.options.iconSize) +
 6                     '" type="button" name="exportAll" aria-label="exportAll" title="%s">',
 7                 this.options.formatExportAll()),
 8                 sprintf('<i class="%s %s"></i>', this.options.iconsPrefix, this.options.icons.exportAll),
 9                 '</button>');
10         }
```

     最终实现效果如下：

![](http://images2017.cnblogs.com/blog/626790/201709/626790-20170905212730022-1139910749.png)

#### java调用**SXSSFWorkbook**组件导出文件

     SXSSFWorkbook官网介绍 http://poi.apache.org/apidocs/org/apache/poi/xssf/streaming/SXSSFWorkbook.html。它是专门用来处理大量数据写入 Excel2007的工具。通过在java后台获取到数据后，传入数据，展示列等信息到SXSSFWorkbook中，即可完成数据写入到excel并以文件流的方式输出。核心调用代码如下：

```
1 @Override
 2     public Map perform(Map inMap) throws BizException, SysException {
 3         inMap.put("start", Integer.valueOf(0));
 4         inMap.put("limit", SysconfigHelper.getExportBatchSize());
 5         Map result = overTime.QueryAction(inMap);
 6         String exportFileName = "加班信息表";
 7         List resutList = (List) result.get(CommonAttribute.ROWS);
 8         List queryResultColumnNames = new ArrayList(Arrays.asList("createByDesc", "overTimeDate", "beginTime", "endTime", "overTimeHour", "checkPersonDesc", "overTimeStatus", "projectNameDesc", "overTimeAddressDesc", "eatMoney", "taxiMoney", "overTimeRemark"));
 9         List queryResultColumnFriendlyNames = new ArrayList(Arrays.asList("加班人", "加班日期", "加班开始时间", "加班结束时间", "加班小时", "审核人", "审核状态", "所属项目", "加班地点", "加班餐费", "加班车费", "备注"));
10         //List<Double> queryResultColumnWidths = new ArrayList(Arrays.asList(65.00, 40.00, 100.00, 120.00, 65.00, 100.00, 65.00, 100.00));
11         HttpServletResponse response = (HttpServletResponse) inMap.get("http_response");
12         try {
13             ExportToExcelHelper.ExportExcel(queryResultColumnNames,queryResultColumnFriendlyNames,resutList,false,exportFileName,response);
14         } catch (IOException e) {
15             e.printStackTrace();
16         }
17         return null;
18     }
```

```
1  protected static void exportExcel(List<Object> columnFriendlyNameList, List<Object> columnList, List<Double> columnWidths, Map inMap, OutputStream outputStream, boolean... hideSEQ) {
 2         boolean hideSeq = hideSEQ.length == 1 && hideSEQ[0];
 3         SXSSFWorkbook workbook = new SXSSFWorkbook(5000);
 4         Sheet sheet = workbook.createSheet("sheet1");
 5         initCellStyle(workbook);
 6         setHeaderLine(sheet, columnFriendlyNameList, hideSeq);
 7         setDataLine(sheet, columnList, inMap, hideSeq);
 8         setColumnWidth(sheet, columnWidths);
 9         outputWorkBook(workbook, outputStream);
10         resetTimeStyleStr();
11     }
```

####   导出数据效果

   通过前端自定义的按钮，调用后台的导出文件action,即可导出数据到excel文本中。展示效果如下（部分列数据我手动删除了）：

![](http://images2017.cnblogs.com/blog/626790/201709/626790-20170905214254679-1883136787.png)

### **2.bootstrap table 数据行修改**

   通过调用Bootstrap Table的扩展js(bootstrap-table-editable.js,bootstrap-editable.js)可设置编辑表格行内数据。前端设置编辑表格数据，界面展示效果如下：

![](http://images2017.cnblogs.com/blog/626790/201709/626790-20170905215949210-84651490.png)

    对于修改表格内容时的提示框样式，可修改bootstrap-table-editable.js中的源码处理noeditFormatter函数，修改样式参考代码如下：

```
1   var _dont_edit_formatter = false;
 2                 if (column.editable.hasOwnProperty('noeditFormatter')) {
 3                     var process = column.editable.noeditFormatter(value, row, index);
 4                     if(!process.hasOwnProperty('class')){
 5                         process.class = '';
 6                     }
 7                     if(!process.hasOwnProperty('style')){
 8                         process.style = 'color:#000;text-decoration:none;';
 9                     }
10                     _dont_edit_formatter = ['<a href="javascript:void(0)"',
11                         ' data-name="'+process.filed+'"',
12                         ' data-pk="' + row[that.options.idField] + '"',
13                         ' data-value="' + process.value + '"',
14                         ' class="'+process.class+'" style="'+process.style+'"',
15                         '>' + process.value + '</a>'
16                     ].join('');
17                 }
18 
19                 if (_dont_edit_formatter === false) {
20                     return ['<a href="javascript:void(0)"',
21                         ' data-name="' + column.field + '"',
22                         ' data-pk="' + row[that.options.idField] + '"',
23                         ' data-value="' + result + '"',
24                         editableDataMarkup.join(''),
25                         '>' + value + '</a>'
26                     ].join('');
27                 } else {
28                     return _dont_edit_formatter;
29                 }
```

  前端页面调用时，参考代码如下：

```
![](http://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)

  1  loadCharts: function () {
  2         var me = this;
  3         var tb_departments = me.getCmp("overtimeTable").bootstrapTable({
  4             method: 'post',                      //请求方式（*）
  5             height: MP.Const.dataGridHeight,
  6             toolbar: '#checkovertimeinfo_toolbar',                //工具按钮用哪个容器
  7             striped: true,                      //是否显示行间隔色
  8             cache: false,                       //是否使用缓存，默认为true，所以一般情况下需要设置一下这个属性（*）
  9             pagination: true,                   //是否显示分页（*）
 10             sortable: true,                     //是否启用排序
 11             sortOrder: "asc",                   //排序方式
 12             sidePagination: "server",           //分页方式：client客户端分页，server服务端分页（*）
 13             pageNumber: 1,                       //初始化加载第一页，默认第一页
 14             pageSize: 20,                       //每页的记录行数（*）
 15             pageList: [10, 20, 25, 30],        //可供选择的每页的行数（*）
 16             //search: true,                       //是否显示表格搜索，此搜索是客户端搜索，不会进服务端，所以，个人感觉意义不大
 17             strictSearch: true,
 18             singleSelect:false,
 19             showColumns: true,                  //是否显示所有的列
 20             showToggle:true,                    //是否显示详细视图和列表视图的切换按钮
 21             //showRefresh: false,                  //是否显示刷新按钮
 22             minimumCountColumns: 1,             //最少允许的列数
 23             //clickToSelect: true,                //是否启用点击选中行
 24             cardView: false,                    //是否显示详细视图
 25             detailView: false,                   //是否显示父子表
 26             showHeader: true,
 27             onEditableSave: function (field, row, oldValue, $el) {
 28                 //单行数据修改后，保存到后台
 29                 var param={};
 30                 var listUuid=[];
 31                 listUuid[0]=row.uuid;
 32                 param.listUuid=listUuid;
 33                 param.overTimeStatus=row.overTimeStatus;
 34                 MP.doAction("sccq-overtime-update", param, function(data)
 35                 {
 36                     if(data.success)
 37                     {
 38                         MP.Msg.info('审核操作完成');
 39                     }
 40                     me.ajaxGetData();
 41 
 42                 },null, true, true);
 43 
 44             },
 45             columns: [
 46                 {
 47                     title: "全选",
 48                     field: "select",
 49                     checkbox: true,
 50                     align: "left",//水平居中
 51                     halign: "left",//垂直居中
 52                 }, {
 53                     field: "uuid",
 54                     align: 'center',
 55                     title: "个人分析",
 56                     formatter: function (value) {
 57                         var html = "<a class='easyui-linkbutton l-btn l-btn-small l-btn-plain' name='" + value + "'>" +
 58                             "<span class='l-btn-left l-btn-icon-left'><span class='l-btn-icon icon-search'></span><span class='l-btn-text'>查看</span></span></a>&nbsp;";
 59                         return html;
 60                     }
 61                 }, {
 62                     align: "left",//水平居中
 63                     halign: "left",//垂直居中
 64                     field: "createByDesc",
 65                     title: "加班人"
 66                 },{
 67                     align: "left",
 68                     halign: "left",
 69                     field: "overTimeDate",
 70                     sortable:true,
 71                     title: "加班日期",
 72                     formatter: function (value) {
 73                         return MP.dateFormatter(value);
 74                     }
 75                 },
 76                 {
 77                     align: "left",
 78                     halign: "left",
 79                     field: "beginTime",
 80                     title: "加班开始时间"
 81                 },
 82                 {
 83                     align: "left",
 84                     halign: "left",
 85                     field: "endTime",
 86                     title: "加班结束时间"
 87                 },
 88                 {
 89                     align: "left",
 90                     halign: "left",
 91                     field: "overTimeHour",
 92                     sortable:true,
 93                     title: "加班小时"
 94                 },
 95                 {
 96                     align: "left",
 97                     halign: "left",
 98                     field: "overTimeStatus",
 99                     title: "审核状态",
100                     editable: {
101                         type: 'select',
102                         title: "审核状态",
103                         pk: 1,
104                         source: [
105                             {value: 2, text: '审核通过'},
106                             {value: 3, text: '驳回'}
107                         ],
108                         noeditFormatter: function (value,row,index) {
109                             var result;
110                             if (value == '1' || value == '待审核') {
111                                 result={filed:"overTimeStatus",value:"待审核",class:"badge bg-orange",style:"padding:5px 10px;"};
112                             } else if (value == '2' || value == '审核通过'){
113                                 result={filed:"overTimeStatus",value:"审核通过",class:"badge bg-green",style:"padding:5px 10px;"};
114                             }
115                             else if (value == '3' || value == '驳回'){
116                                 result={filed:"overTimeStatus",value:"驳回",class:"badge bg-red",style:"padding:5px 10px;"};
117                             }
118                             return result;
119                         }
120                     }
121                 },
122                 {
123                     align: "left",
124                     halign: "left",
125                     field: "projectNameDesc",
126                     sortable:true,
127                     title: "所属项目"
128                 },
129                 {
130                     align: "left",
131                     halign: "left",
132                     field: "overTimeAddressDesc",
133                     sortable:true,
134                     title: "加班地点"
135                 },
136                 {
137                     align: "left",
138                     halign: "left",
139                     field: "eatMoney",
140                     sortable:true,
141                     title: "加班餐费"
142                 },
143                 {
144                     align: "left",
145                     halign: "left",
146                     field: "taxiMoney",
147                     sortable:true,
148                     title: "加班车费"
149                 },
150                 {
151                     align: "left",
152                     halign: "left",
153                     field: "overTimeRemark",
154                     title: "备注"
155                 }
156             ],
157             onPageChange:function(number, size)
158             {
159                 //设置在分页事件触发时，传递分页参数给后台，重新加载数据
160                 me.queryBaseParam.limit=size;
161                 me.queryBaseParam.start=number;
162                 me.ajaxGetData();
163             },
164             onSort: function (name, order) {
165                 me.queryBaseParam.sort=name;
166                 me.queryBaseParam.order=order;
167                 me.ajaxGetData();
168             },
169             onClickRow: function (row, $elepment, field) {
170                 if (field == 'uuid') {
171                     //alert("查看页面");
172                     var params={};
173                     params.createBy=row.createBy;
174                     params.createByDesc=row.createByDesc;
175                     me.controller.showOvertimeSingleDetail(params);
176                 }
177             }
178         });
179     },
180     ajaxGetData: function () {
181         //加载后台数据
182         var me=this;
183         var params=MP.getFormData("searchOverTimeForm_person",this.controller);
184         params.QueryType=1;
185         params.limit= me.queryBaseParam.limit;
186         params.start= me.queryBaseParam.start;
187         params.sort= me.queryBaseParam.sort;
188         params.order= me.queryBaseParam.order;
189         MP.doAction("sccq-overtime-query", params, function (datas) {
190             if (datas.success) {
191                 me.getCmp("overtimeTable").bootstrapTable('load', datas);
192             }
193         }, function (datas) {
194             alert("数据加载失败");
195         }, true, true);
196     }


View Code
```



