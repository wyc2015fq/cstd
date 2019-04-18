# easyui关于父jsp页面向子jsp页面(dialog中传参数) - 苝花向暖丨楠枝向寒 - CSDN博客

2018年04月19日 18:33:23[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：301


```java
//父jsp页面中创建dialog
$('<div></div>').dialog(
      {
        id : "divDialogEditGUnitG",
        title : '编辑',
        width : 600,
        height : 350,
        closed : false,
        cache : false,
          href : '子jsp的url',
          queryParams:{'pbgProduceModel':rows}, //重点是这行，将要传递的参数放入queryParams中,rows是datagrid数据表格中所选中那行的数据
        modal : true,
        buttons : [ {
          text : '保存',
          handler : function() {
            divDialogEditGUnitG_save();
            $('#divDialogEditGUnitG').dialog('destroy');
          }
        }, {
          text : '关闭',
          handler : function() {
            $('#divDialogEditGUnitG').dialog('destroy');
          }
        }]
      });
```

```
//子jsp页面获取值 并放入到input框中
var batchcode = $('#divDialogEditGUnitG').dialog('options')["queryParams"]["pbgProduceModel"];   //batchcode是一行数据，但返回来的是个数组
$("input[name='workPackage']").val(batchcode[0].workPackage);  //即[{"key1":"value1","key2":"value2"}]，batchcode[0]就是数组里的第一个json
$("input[name='taskType']").val(batchcode[0].taskType);        //当然我这里一行数据，也就只有一个json.
$("input[name='planRequirements']").val(batchcode[0].planRequirements);
$("input[name='planEnd']").val(batchcode[0].planEnd);

<input  name="workPackage" value=""   class=""  type="hidden" data-options="width:170,required:true,missingMessage:'请填写预处理'">
<input  name="taskType" value=""   class=""  type="hidden" data-options="width:170,required:true,missingMessage:'请填写预处理'">
<input  name="planRequirements" value=""   class=""  type="hidden" data-options="width:170,required:true,missingMessage:'请填写预处理'">
<input  name="planEnd" value=""   class=""  type="hidden" data-options="width:170,required:true,missingMessage:'请填写预处理'">
```

-------------------------------------------------------------------------------------------------------

注意：

上面的情况属于 ： 子jsp获取父jsp的datagrid生成的数据表中的一行数据。

如果是获取 父jsp页面中的 <input class="easyui-combox" ></input>   下拉框所选中的数据

就可以 直接在 子jsp页面获取，无需先将数据放入到父jsp的datagrid插件中的 queryParams 中。

示例请看我上一篇文章：[点击打开链接](https://blog.csdn.net/weixin_40247263/article/details/80001888)


