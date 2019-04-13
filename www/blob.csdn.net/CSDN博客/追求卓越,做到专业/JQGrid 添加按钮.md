
# JQGrid 添加按钮 - 追求卓越,做到专业 - CSDN博客


2012年12月21日 12:47:18[Waldenz](https://me.csdn.net/enter89)阅读数：10329个人分类：[jqGrid																](https://blog.csdn.net/enter89/article/category/1309802)



JQGrid 添加自定义按钮, 按钮显示文字Save.  参考:[自定义按钮](http://www.trirand.com/jqgridwiki/doku.php?id=wiki:custom_buttons)
$("\#ReportList").jqGrid('navButtonAdd', '\#ReportListPager', { caption: "Save", onClickButton: function() { $("\#btnSaveAs").trigger("click"); } });
属性及方法:
caption: (string) the caption of the button, can be a empty string.
buttonicon: (string) is the ui icon name from UI theme icon set. If this option is set to “none” only the text appear.
onClickButton: (function) action to be performed when a button is clicked. Default null.
position: (“first” or “last”) the position where the button will be added (i.e., before or after the standard buttons).
title: (string) a tooltip for the button.
cursor : string (default pointer) determines the cursor when we mouseover the element
id : string (optional) - if set defines the id of the button (actually the id of TD element) for future manipulation



```python
$('#ReportList').jqGrid({
        url: jstUrl,
        datatype: 'json',
        mtype: 'POST',
        caption: jstCaption,
        colNames: jstColNames,
        colModel: jstColModels,
        postData: {
            StartDate: $("#txtStartDate").val(),
            EndDate: $("#txtEndDate").val()
        },
        rowNum: 100,
        rowList: [10, 100, 500, 1000],
        pager: '#ReportListPager',
        viewrecords: true,
        sortname: jstSortName,
        sortorder: 'desc',
        search: true,
        cellEdit: false,
        cellsubmit: 'clientArray',
        loadComplete: function(data) {
            $("#txtFilterString").val(data.filterString);
            $("#txtGridRecords").val(parseInt($(this).getGridParam("records")));
            LinkClickFunction();
        },
        //autowidth: true,
        forceFit: true,
        // height: '370',
        //width: '1100',
        shrinkToFit: true,
        multiselect: false,
        loadtext: 'Loading...',
        loadui: 'block',
        hoverrows: true,
        gridView: true
    }).navGrid('#ReportListPager', { edit: false, add: false, del: false, refresh: true, refreshtext: jstGridRefresh, view: false, search: false });
    $('#ReportList').filterToolbar({ autosearch: true, searchOnEnter: true, stringResult: true, gridModel: true, beforeSearch: function() { $("#lblError").text(""); } });
    $("#ReportList").jqGrid('navButtonAdd', '#ReportListPager', { caption: jstGridSave, onClickButton: function() { $("#btnSaveAs").trigger("click"); } });
```





